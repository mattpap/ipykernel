# IAldor

**IAldor** is an [Aldor-language](https://github.com/pippijn/aldor) backend for [IPython](http://ipython.org).

## Requirements

* [IPython](http://ipython.org/ipython-doc/stable/install/install.html) 1.0+
* [Aldor](https://github.com/pippijn/aldor) master
* [ZeroMQ](http://zeromq.org) 3.0+
* [Jansson](http://www.digip.org/jansson/) 2.5+

## Building

Make sure that appropriate versions of libzmq and libjansson are installed on your
system. Next you have build and install Aldor:
```
$ git clone git@github.com:pippijn/aldor.git
$ cd aldor/aldor
$ ./autogen.sh
$ ./configure --prefix=$HOME/aldor CFLAGS="-O0 -g -ggdb"
$ make -j8
$ make install
$ cd ../..
```
Choose prefix and `CFLAGS` as you want and adjust `make -jN` according to your CPU's
capabilities. This build procedure allows for root-less installtion and debugging
with gdb.

Now you can build and run IAldor:
```
$ git clone git@github.com:mattpap/IAldor.git
$ cd IAldor
$ cmake -DALDOR=../aldor
$ make
$ ALDORROOT=$HOME/aldor ./ialdor
```
`ialdor` binary is statically linked with Aldor's compiler, but still needs configuration
and include files, and Aldor standard library, etc., so you have to set up `ALDORROOT`
anyway.

## License

Copyright &copy; 2013 by Mateusz Paprocki and contributors.

Published under The MIT License, see LICENSE.
