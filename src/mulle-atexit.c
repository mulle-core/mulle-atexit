#include "include-private.h"

#include "mulle-atexit.h"

#include <assert.h>

//#define DEBUG_VERBOSE

#ifdef DEBUG_VERBOSE
# include <stdio.h>
# define TRACE()    fprintf( stderr, "%s\n", __FUNCTION__)
# define TRACE1( s) fprintf( stderr, "%s: %s\n", __FUNCTION__, s)
#else
# define TRACE()    while( 0)
# define TRACE1(s)  while( 0)
#endif


//
// on some OS atexit just works correctly
//
#if defined( _WIN32)
# define USE_ATEXIT
# pragma message( "mulle_atexit uses atexit")
#endif

int   __MULLE_ATEXIT_ranlib__;


uint32_t   mulle_atexit_get_version( void)
{
   return( MULLE__ATEXIT_VERSION);
}


static struct
{
   mulle_thread_once_t    once;
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

   assert( MULLE_THREAD_ONCE_INIT == 0);
   mulle_thread_mutex_init( &vars.lock);
#ifdef USE_ATEXIT
   atexit( run_exit_callbacks);
#endif
}


MULLE_C_NEVER_INLINE
void   _mulle_atexit( void (*f)( void))
{
   TRACE();

   mulle_thread_once( &vars.once, init);
   if( ! f)
      return;

   //
   // If everything ran already, we could only execute zero priority code
   // with good conscience. But, as we want to support libraries being linked
   // statically and dynamically, we don't want to change the code depending
   // on the way it is compiled. So we just execute them. We are after all
   // just a fix for the ELF shared library loading.
   //
   mulle_thread_mutex_lock( &vars.lock);
   {
      if( ! vars.n && vars.size)
      {
         mulle_thread_mutex_unlock( &vars.lock);
         TRACE1( "call");
         (*f)();
         return;
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
}



int   mulle_atexit( void (*f)( void))
{
   TRACE();

   _mulle_atexit( f);
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
MULLE_C_CONSTRUCTOR( load_atexit)
static void   load_atexit( void)
{
   TRACE();

   _mulle_atexit( 0); // protect from evil linker optimization and do "once"
}


#ifndef USE_ATEXIT

MULLE_C_DESTRUCTOR( unload_atexit)
static void   unload_atexit( void)
{
   TRACE();

   _mulle_atexit( 0); // protect from evil linker optimization and do "once"
   run_exit_callbacks();
}

#endif


