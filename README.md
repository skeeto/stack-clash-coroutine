# Stack Clash Coroutine Demonstration

Full article:
[Stack Clashing for Fun and Profit](http://nullprogram.com/blog/2017/06/21/)

## Instructions

You'll need a C99 compiler and your favorite `make`:

    $ make -k

If there's no compiler named `c99` on your system: 

    $ make -k CC=clang
    $ make -k CC='gcc -std=c99'

If your system doesn't support the `<ucontext.h>` XSI extension then one
of the binaries will fail to build, which is why the `-k` option is
useful.
