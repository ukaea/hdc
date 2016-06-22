HDC - Hierarchical Dynamic Containers                         {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python and Fortran.

As of June 2016 it is still in a preview state - it works, but many work is still need to be done.

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
# Go to directory
cd libdynd
# Checkout the last tested working commit
git checkout 6e81192606875a9408f19a997854a46959f48cb4
# Create build directory
mkdir build
cd build
# make
cmake ..
make
# Optionally install
make install
```

Building HDC
------------
There are several cmake options:

  - `-DBUILD_DOCUMENTATION=ON` Whether to build and install documentation.
  - `-DBUILD_EXAMPLES=ON` Whether to build and install examples.
  - `-DCMAKE_INSTALL_PREFIX=/where/to/install` make install destination.
  - `-DDEBUG=ON` Whether to prin debugging messages.
  - `-DPYTHON_LIBRARY=/path/to/libpython.so` On abacus set this to `/sw/python2/anaconda3/lib/libpython3.4m.so`
  - `-DPYTHON_INCLUDE_DIR=/path/to/python/include`  On abacus set this to `/sw/python2/anaconda3/include/python3.4m/`

Some of them can be edited using `ccmake .` in`build` directory. The example of build follows:


```
# clone the git repository
git clone git@bitbucket.org:compass-tokamak/hdc.git
# cd into hdc
cd hdc

# build in a separate build directory
mkdir build
cd build

cmake -DDEBUG=OFF ..
make -j4

# Optionally install
make install
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
The basic examples can be found in examples folder. The executables are built in CMAKE_BUILD_DIR/bin directory.

The Python examples can be run from any arbitraty folder, the only necessary thing is to set LD_LIBRARY_PATH properly:

```
LD_LIBRARY_PATH="../build/lib/" ipython hdc_fortran_module.py
```

Next steps
==========
- Use shared memory, for example via [mdbm](https://github.com/yahoo/mdbm).
- Read-only and/or copy-on-write potection of data.
- Serialization to HDF5 and other suitable formats.
- File systems and database access (key-value, object stores) via the HDC API.
- Support for metadata.
- A plugin system for, e.g., data systems validation or conversion, object-oriented features (methods for particular data types), ...
- Support for scientific data: dimensions, units, etc.
- More features: slicing, lazy evaluation, richer API, ...

Compared to ...
===============
HDF5
----
- HDC holds data buffers in (shared) memory, hence passing HDC containers means
    - no data copy,
    - no serialization / deserialization,
    - better performace.
- HDC is written in C++ with bindings to Fortran, C, Python and other languages in mind. 
- HDC API can abstract out various back-end storage solutions: file systems, key-value stores, clouds, ...

Conduit
-------
Citing [Conduit](http://software.llnl.gov/conduit/): provides an intuitive model for describing hierarchical scientific data in C++, C, Fortran, and Python and is used for data coupling between packages in-core, serialization, and I/O tasks.

*The goals are very close to ours, merging these two projects in near future is being considered.*