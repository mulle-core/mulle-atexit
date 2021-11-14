#include <mulle-atexit/mulle-atexit.h>

#include <stdlib.h>


static void   a( void)
{
   printf( "a\n");
   fflush( stdout);  // flush needed for windows
}

static void   b( void)
{
   printf( "b\n");
   fflush( stdout);  // flush needed for windows
}


int   main( void)
{
   mulle_atexit( a);
   mulle_atexit( b);

   return( 0);
}