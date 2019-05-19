#include <mulle-atexit/mulle-atexit.h>

#include <stdlib.h>


static void   a( void)
{
   printf( "a\n");
}

static void   b( void)
{
   printf( "b\n");
}


int   main( void)
{
   mulle_atexit( a);
   mulle_atexit( b);

   return( 0);
}