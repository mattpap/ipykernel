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
This created `src/libipykernel.a` and built examples in `examples/`. If you want to
use the system wide compiler (possibly `gcc`) then remove `CMAKE_C_COMPILER` from
`cmake`'s argument list.

## Examples

Start `examples/echo` in one terminal:
```bash
$ examples/echo
connect ipython with --existing profile-30914.json
```
Now in another terminal issue:
```bash
ipython console --existing profile-30914.json
Python 2.7.5 |Anaconda 1.7.0 (64-bit)| (default, Oct 25 2013, 18:42:10)
Type "copyright", "credits" or "license" for more information.

In [1]: Hi!
Out[1]: echo: Hi!
```
This simple kernel just echoes what you enter at the command prompt. By using command
line options you can later reuse existing profile as follows:
```bash
$ examples/echo --profile profile-30914.json
```
Add ``--verbose`` option to see flow of messages between this kernel and IPython.

## License

Copyright &copy; 2013 by Mateusz Paprocki and contributors.

Published under The MIT License, see LICENSE.
