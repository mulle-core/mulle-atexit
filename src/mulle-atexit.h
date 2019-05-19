#include "include.h"

#include <stdint.h>
#include <stdlib.h>


/*
 *  (c) 2019 nat
 *
 *  version:  major, minor, patch
 */
#define MULLE_ATEXIT_VERSION  ((0 << 20) | (7 << 8) | 56)


static inline unsigned int   mulle_atexit_get_version_major( void)
{
   return( MULLE_ATEXIT_VERSION >> 20);
}


static inline unsigned int   mulle_atexit_get_version_minor( void)
{
   return( (MULLE_ATEXIT_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   mulle_atexit_get_version_patch( void)
{
   return( MULLE_ATEXIT_VERSION & 0xFF);
}


extern uint32_t   mulle_atexit_get_version( void);


// linux specifically glibc 2.29 is broken, so we need mulle_atexit.
// other OS are assumed to be OK, so we just use atexit
#ifdef __linux


int   mulle_atexit( void (*f)( void));

#else

static inline int   mulle_atexit( void (*f)( void))
{
   atexit( f);
}

#endif

