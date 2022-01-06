//
//  mulle-atinit.h
//  mulle-atinit
//
//  Created by Nat!
//  Copyright (c) 2017 Nat! - Mulle kybernetiK.
//  Copyright (c) 2017 Codeon GmbH.
//  All rights reserved.
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#ifndef mulle_atexit_h__
#define mulle_atexit_h__

#include "include.h"

#include <stdint.h>

/*
 *  (c) 2019 nat ORGANIZATION
 *
 *  version:  major, minor, patch
 */
#define MULLE_ATEXIT_VERSION  ((0 << 20) | (0 << 8) | 10)


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


MULLE_ATEXIT_EXTERN_GLOBAL
uint32_t   mulle_atexit_get_version( void);



MULLE_ATEXIT_EXTERN_GLOBAL
int   mulle_atexit( void (*f)( void));



#ifdef __has_include
# if __has_include( "_mulle-atexit-versioncheck.h")
#  include "_mulle-atexit-versioncheck.h"
# endif
#endif

#endif
