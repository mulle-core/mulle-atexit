#include "include.h"

#include <stdint.h>

/*
 *  (c) 2019 nat ORGANIZATION
 *
 *  version:  major, minor, patch
 */
#define MULLE_ATEXIT_VERSION  ((0 << 20) | (0 << 8) | 5)


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




int   mulle_atexit( void (*f)( void));

