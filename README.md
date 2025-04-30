# mulle-atexit

#### 👼 Compatibility library to fix atexit

See [Challenges of shared library environments, Part 2](//www.mulle-kybernetik.com/weblog/2019/atexit_is_broken.html), why and when this is necessary to use.

This reimplements `atexit` as it should be, just under a different name.



| Release Version                                       | Release Notes  | AI Documentation
|-------------------------------------------------------|----------------|---------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-core/mulle-atexit.svg) [![Build Status](https://github.com/mulle-core/mulle-atexit/workflows/CI/badge.svg)](//github.com/mulle-core/mulle-atexit/actions) | [RELEASENOTES](RELEASENOTES.md) | [DeepWiki for mulle-atexit](https://deepwiki.com/mulle-core/mulle-atexit)



You must **statically** link the mulle-atexit library with your executable.
Ensure that global symbols are exported and that the whole library is linked
to the executable and not optimized away by the linker.

Platform | Linker Flags
---------|--------------------------------------------------
Linux    | `-Wl,--export-dynamic -Wl,--whole-archive`
macos    | -force_load <library>




### You are here

![Overview](overview.dot.svg)





## Add

Use [mulle-sde](//github.com/mulle-sde) to add mulle-atexit to your project:

``` sh
mulle-sde add github:mulle-core/mulle-atexit
```

To only add the sources of mulle-atexit with dependency
sources use [clib](https://github.com/clibs/clib):


``` sh
clib install --out src/mulle-core mulle-core/mulle-atexit
```

Add `-isystem src/mulle-core` to your `CFLAGS` and compile all the sources that were downloaded with your project.


## Install

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-atexit and all dependencies:

``` sh
mulle-sde install --prefix /usr/local \
   https://github.com/mulle-core/mulle-atexit/archive/latest.tar.gz
```

### Legacy Installation

#### Requirements

Preferably install mulle-core and be done with it:

| Requirements                                     | Description
|--------------------------------------------------|-----------------------
| [mulle-core](//github.com/mulle-core/mulle-core) |🌋 Almagamated library of mulle-core + mulle-concurrent + mulle-c


Or if you really want to do it exhaustively:

| Requirements                                 | Description
|----------------------------------------------|-----------------------
| [mulle-thread](https://github.com/mulle-concurrent/mulle-thread)             | 🔠 Cross-platform thread/mutex/tss/atomic operations in C

#### Download & Install

Download the latest [tar](https://github.com/mulle-core/mulle-atexit/archive/refs/tags/latest.tar.gz) or [zip](https://github.com/mulle-core/mulle-atexit/archive/refs/tags/latest.zip) archive and unpack it.

Install **mulle-atexit** into `/usr/local` with [cmake](https://cmake.org):

``` sh
PREFIX_DIR="/usr/local"
cmake -B build                               \
      -DMULLE_SDK_PATH="${PREFIX_DIR}"       \
      -DCMAKE_INSTALL_PREFIX="${PREFIX_DIR}" \
      -DCMAKE_PREFIX_PATH="${PREFIX_DIR}"    \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



