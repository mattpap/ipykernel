# ipykernel

**IPyKernel** is a pure C implementation of [IPython](http://ipython.org)'s
messaging protocol.

## Requirements

* [IPython](http://ipython.org/ipython-doc/stable/install/install.html) 1.0+
* [ZeroMQ](http://zeromq.org) 2.x
* [Jansson](http://www.digip.org/jansson/) 2.1+

## Building

Make sure that appropriate versions of libzmq and libjansson are installed
on your system. For example in Ubuntu install `libzmq-dev` and `libjansson-dev`
using `apt-get`. Otherwise you can build those libraries from source. Clone
https://github.com/zeromq/zeromq2-x and https://github.com/akheron/jansson/tree/2.5,
and follow their instructions how to build and install those libraries.

Now you can build ipykernel:
```
$ git clone git@github.com:mattpap/ipykernel.git
$ cd ipykernel
$ cmake -DCMAKE_C_COMPILER=/usr/bin/clang .
$ make
```
This created `libipykernel.a` and `libipykernel.so`. If you want to use the system
wide compiler (possibly `gcc`) then remove `CMAKE_C_COMPILER` from `cmake`'s argumet
list.

## License

Copyright &copy; 2013 by Mateusz Paprocki and contributors.

Published under The MIT License, see LICENSE.
