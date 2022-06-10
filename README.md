# Tic Tac Toe in C but with Auto Dereferencing

A simple experiment that adds [Auto Dereferencing](https://twitter.com/tsoding/status/1527660533201178625) of the Structures to C Programming Languages.

This is a simple Tic Tac Toe game that won't compile on your machine unless you change it's code or compile with [TCC](https://bellard.org/tcc/) that is patched with [tcc.patch](./tcc.patch)

## Quick Start

```console
$ wget http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.27.tar.bz2
$ tar fvx tcc-0.9.27.tar.bz2 && cd tcc-0.9.27
$ patch ./tccgen.c < ../tcc.patch
$ ./configure && make && make install && cd ..
$ tcc -o main main.c && ./main
```
