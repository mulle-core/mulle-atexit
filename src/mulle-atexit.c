#include "include-private.h"

#include "mulle-atexit.h"



int   __MULLE_ATEXIT_ranlib__;


uint32_t   mulle_atexit_get_version( void)
{
   return( MULLE_ATEXIT_VERSION);
}

#ifdef __linux

static struct
{
   mulle_thread_once_t     once;
   mulle_thread_mutex_t    lock;
   unsigned int            n;
   void                    (*f[ 32])( void);
} vars =
{
   MULLE_THREAD_ONCE_INIT
};


static void   run_exit_callbacks( void)
{
   void (*f)( void);

loop:
   mulle_thread_mutex_lock( &vars.lock);
   f = vars.n ? vars.f[ --vars.n] : 0;
   mulle_thread_mutex_unlock( &vars.lock);
   if( f)
   {
      (*f)();
      goto loop;
   }
}


static void   init( void)
{
   mulle_thread_mutex_init( &vars.lock);
   atexit( run_exit_callbacks);
}


int   mulle_atexit( void (*f)( void))
{
   int   rval;

   mulle_thread_once( &vars.once, init);

   mulle_thread_mutex_lock( &vars.lock);
   rval = -1;
   if( vars.n < 32)
   {
      vars.f[ vars.n++] = f;
      rval = 0;
   }
   mulle_thread_mutex_unlock( &vars.lock);

   return( rval);
}

#endif
