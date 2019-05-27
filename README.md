# mulle-atexit

👼 Compatibility library to fix atexit

See [Challenges of shared library environments, Part 2](https://www.mulle-kybernetik.com/weblog/2019/atexit_is_broken.html), why and when this is necessary to use.

This reimplements `atexit` as it should be, just under a different name.


You must **statically** link this library with your executable. Ensure that
global symbols are exported and that the whole library is linked to the
executable and not optimized away by the linker.



## Build

This is a [mulle-sde](https://mulle-sde.github.io/) project.

It has it's own virtual environment, that will be automatically setup for you
once you enter it with:

```
mulle-sde mulle-atexit
```

Now you can let **mulle-sde** fetch the required dependencies and build the
project for you:

```
mulle-sde craft
```
