HDC - Hierarchical Dynamic Containers                         {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python and Fortran.

As of January 2019 it should mostly work

Building instructions
=====================

Prerequisites
-------------

To build HDC, you will need:
- c++11 compliant compiler (tested with intelstudio>=2018 and gcc>=5.0)
- gfortran>=4.9
- Boost >= 1.48
- Yahoo MDBM (optional, but recommended)
- Python 2.7 or any Python 3 (tested only with 3.4, 3.5, 3.6 and 3.7)
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
cd ..

# Optionally build & run MATLAB mex interface
cd matlab
make -f Makefile_
LD_LIBRARY_PATH=../../install/lib matlab -nojvm -r "run('test_matlab')"
# You should see "All tests are OK..." message - in such case, the mex interface should work fine...
```

Building using IntelStudio
--------------------------
```
Instrucions are similar as above, just setup your environment and CC,CXX and FORTRAN variables:
source /sw/intel/parallel_studio_xe_2018/psxevars.sh
CC=icc FORTRAN=ifort CXX=icpc cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../install

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
- Read-only and/or copy-on-write potection of data.
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
- HDC is written in C++ with bindings to Fortran, C, Python, MATLAB and other languages in mind.
- HDC API can abstract out various back-end storage solutions: file systems, key-value stores, clouds, ...

Conduit
-------
Citing [Conduit](http://software.llnl.gov/conduit/): provides an intuitive model for describing hierarchical scientific data in C++, C, Fortran, and Python and is used for data coupling between packages in-core, serialization, and I/O tasks.

*The goals are very close to ours, merging these two projects in near future is being considered.*
