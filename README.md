# mulle-atexit

This is a little bit of cross-platform glue, to work arounds bugs in
deficient `atexit` implementations like glibc.


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
