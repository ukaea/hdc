HDC - Hierarchical Dynamic Containers                         {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python and Fortran.

As of September 2016 it is still in a preview state - it works, but many work is still need to be done.

Building instructions
=====================

Prerequisites
-------------

To build HDC, you will need:
- c++11 compliant compiler
- gfortran>=4.9
- Boost >= 1.48 (does not work with 1.41, versions 1.41-1.48 untested)
- Yahoo MDBM (optional, but recommended)
- Python 2.7 or any Python 3 (tested only with 3.4, 3.5 and 3.6)
- CMake >= 3.3
- Doxygen for documentation building
- Cython > 0.23 (there is some parsing error in 0.23)
- HDF5 devel libraries (optional, tested with 1.8 and 1.10)

Building HDC
------------
For building on ITM gateway, please, follow [these instructions](doxy/BUILDING_ON_ITM_GATEWAY.md).

There are several cmake options:

  - `-DCMAKE_INSTALL_PREFIX=/where/to/install` make install destination.
  - `-DBUILD_DOCUMENTATION=ON` Whether to build and install documentation.
  - `-DBUILD_EXAMPLES=ON` Whether to build and install examples.
  - `-DENABLE_HDF5=OFF` Switch off HDF5 serialization.
  - Python, if not detected correctly:
    - `-DPYTHON_LIBRARY=/path/to/libpython.so`
    - `-DPYTHON_INCLUDE_DIR=/path/to/python/include`
  - `-DDEBUG=ON` Whether to print debugging messages.

Some of them can be edited using `ccmake .` in`build` directory. The example of build follows:

```
# clone the git repository
git clone git@bitbucket.org:compass-tokamak/hdc.git
# cd into hdc
cd hdc

# build in a separate build directory
mkdir build
cd build

cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../install
make -j install

# Optionally build & install python module
cd  python
python setup.py build
python setup.py install
```

Basic ideas
===========
- The hierarchical structure is organized as tree.
- Subtrees (Nodes) can be accessed by path similarily to file system paths like "aaa/bbb"
- Each node can be one of several types:
  + Empty node - this is the initial state of node. Empty node does not store any data and does not have any children. By adding subnode, slice or data it's type is automaticaly changed to another type.
  + Structure/list node - the node has at least one children indexed by string path. It can only store subtrees indexed by path/integer index.
  + Array node - the node has at least one children indexed by integer. It can only store subtrees indexed by integer.
  + Data node - it only can be terminal node, it stores some data, currently in DyND array object.


Examples
========
The basic examples can be found in examples folder. The executables are built in CMAKE_BUILD_DIR/bin directory.

The Python examples can be run from any arbitraty folder, the only necessary thing is to set LD_LIBRARY_PATH properly:

```
LD_LIBRARY_PATH="../build/lib/" ipython hdc_fortran_module.py
```

Some benchmarks
===============
See [this file](doxy/PERFORMANCE_TESTING.md)

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
