HDC - Hierarchical Dynamic Containers                         {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python and Fortran.

As of May 2016 it is in a preview state - it works, but many work is still need to be done.

Building instructions
=====================

Prerequisites
-------------

To build HDC, you will need:
- c++14 compliant compiler (at least gcc-4.9)
- libDyND library
- Python 2.7 or any Python 3 (tested only with 3.4 and 3.5)
- CMake >= 2.8.11
- Doxygen for documentation building

Building libDyND
----------------
For complete guide please follow instructions on [libDyND project page](https://github.com/libdynd/libdynd)
```
git clone --recursive https://github.com/libdynd/libdynd.git
# Checkout the last tested working commit
git checkout 6e81192606875a9408f19a997854a46959f48cb4
# Go to directory
cd libdynd
# Create build directory
mkdir build
cd build
# make
cmake ..
make
make install
```

Building HDC
------------
```
cd hdc_new
mkdir build cd build
cmake ..
make
# Optionally build documentation
make doc
```

Basic ideas
===========
- The hierarchical structure is organized as tree.
- Subtrees (Nodes) can be accessed by path similarily to file system paths like "aaa/bbb"
- Each node can be one of several types:
  + Empty node - this is the initial state of node. Empty node does not store any data and does not have any children. By adding subnode, slice or data it's type is automaticaly changed to another type.
  + Structure node - the node has at least one children indexed by string path. It can only store subtrees indexed by path.
  + Array node - the node has at least one children indexed by integer. It can only store subtrees indexed by integer.
  + Data node - it only can be terminal node, it stores some data, currently in DyND array object.


Examples
========
The basic examples can be found in src/examples folder. The executables are built in CMAKE_BUILD_DIR/bin directory.

The Python examples can be run from any arbitraty folder, the only necessary thing is to set LD_LIBRARY_PATH properly:

```
LD_LIBRARY_PATH="../build/lib/" ipython hdc_fortran_module.py
```

