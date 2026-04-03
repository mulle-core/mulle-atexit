#include "include-private.h"

#include "mulle-atexit.h"

#include <assert.h>

#ifdef DEBUG
//# define MULLE_ATEXIT_DEBUG
#endif


#ifdef MULLE_ATEXIT_DEBUG
# include <stdio.h>
# define TRACE()    fprintf( stderr, "%s\n", __FUNCTION__)
# define TRACE1( s) fprintf( stderr, "%s: %s\n", __FUNCTION__, s)
#else
# define TRACE()    while( 0)
# define TRACE1(s)  while( 0)
#endif

#ifdef MULLE_INCLUDE_DYNAMIC
# error "You can't make mulle-atexit part of a shared library"
#endif

//
// on some OS atexit just works correctly
//
// #if defined( _WIN32)
// # define USE_ATEXIT
// # pragma message( "mulle_atexit uses atexit")
// #endif


uint32_t   mulle_atexit_get_version( void)
{
   return( MULLE__ATEXIT_VERSION);
}


static struct
{
   mulle_thread_mutex_t   lock;
   unsigned int           n;
   unsigned int           size;
   void                   (**fs)( void);
} vars;


static void   run_exit_callbacks( void)
{
   void   (*f)( void);

   TRACE();

   mulle_thread_mutex_lock( &vars.lock);
loop:
   f = 0;
   if( vars.n)
   {
      --vars.n;
      f = vars.fs[ vars.n];
   }

   if( ! f)
   {
      free( vars.fs);
      vars.fs    = NULL;  // keep size alive though as an indicator that we ran
      vars.size  = -1;
      vars.n     = 0;
   }
   mulle_thread_mutex_unlock( &vars.lock);
   if( f)
   {
      TRACE1( "call");
      (*f)();
      mulle_thread_mutex_lock( &vars.lock);
      goto loop;
   }
}



static void   init( void)
{
   TRACE();

   assert( MULLE_THREAD_ONCE_DATA == 0);
   mulle_thread_mutex_init( &vars.lock);
#ifdef USE_ATEXIT
   atexit( run_exit_callbacks);
#endif
}


MULLE_C_NEVER_INLINE
int   _mulle_atexit( void (*f)( void))
{
   TRACE();

#ifdef MULLE_ATEXIT_DEBUG
   fprintf( stderr , "0x%tx: _mulle_atexit %p starts\n", (intptr_t) mulle_thread_id(), (void *) f);
#endif
   mulle_thread_once_do( once)
   {
      init();
   }

   if( ! f)
   {
#ifdef MULLE_ATEXIT_DEBUG
      fprintf( stderr , "0x%tx: _mulle_atexit returns with no function to add\n", (intptr_t) mulle_thread_id());
#endif
     return( 0);
   }
   //
   // If everything ran already, we could only execute zero priority code
   // with good conscience. But, as we want to support libraries being linked
   // statically and dynamically, we don't want to change the code depending
   // on the way it is compiled. So we just execute them. We are after all
   // just a fix for the ELF shared library loading.
   //
#ifdef MULLE_ATEXIT_DEBUG
   fprintf( stderr , "0x%tx: _mulle_atexit locks mutex\n", (intptr_t) mulle_thread_id());
#endif

   mulle_thread_mutex_lock( &vars.lock);
   {
      if( ! vars.n && vars.size)
      {
         mulle_thread_mutex_unlock( &vars.lock);
         TRACE1( "call");
         (*f)();
         return( 0);
      }

      if( vars.n >= vars.size)
      {
         vars.size = vars.size ? 2 * vars.size : 32;
         vars.fs   = realloc( vars.fs, vars.size * sizeof( void (*)( void)));
         if( ! vars.fs)
            abort();
      }
      vars.fs[ vars.n++] = f;
      TRACE1( "add");
   }
   mulle_thread_mutex_unlock( &vars.lock);

#ifdef MULLE_ATEXIT_DEBUG
   fprintf( stderr , "0x%tx: _mulle_atexit unlocked mutex\n", (intptr_t) mulle_thread_id());
#endif
   return( 0);
}


//
// this is supposed to be statically linked. The initializer will then run
// guaranteed inside the exe and the `atexit`, that we use, will be run outside
// dlclose
//
// (this could run in a shared lib too), but because of the availability of
// the `mulle_atinit` symbol
//
MULLE_C_CONSTRUCTOR( mulle_atexit_load)
static void   mulle_atexit_load( void)
{
   TRACE();

   _mulle_atexit( 0); // protect from evil linker optimization and do "once"
}


#ifndef USE_ATEXIT

MULLE_C_DESTRUCTOR( mulle_atexit_unload)
static void   mulle_atexit_unload( void)
{
   TRACE();

   _mulle_atexit( 0); // protect from evil linker optimization and do "once"
   run_exit_callbacks();
}

#endif



#if defined( _WIN32) && defined( MULLE_INCLUDE_DYNAMIC) && defined( MULLE_ATEXIT_DEBUG) && ! defined( MULLE__CORE__ALL_LOAD_BUILD)
BOOL WINAPI   DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
   MULLE_C_UNUSED( hinstDLL);
   MULLE_C_UNUSED( fdwReason);
   MULLE_C_UNUSED( lpvReserved);

   // BASICALLY: if you see this at all, its wrong!
   if( fdwReason == DLL_PROCESS_ATTACH)
      fprintf( stderr, "mulle_atexit DLL loaded\n");
   return TRUE;
}
#endif
