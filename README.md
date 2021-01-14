HDC - Hierarchical Dynamic Containers {#mainpage}
=====================================

HDC is tiny library for exchanging hierarchical data (arrays of structures) in shared memory between multiple programming languages, currently supporting C, C++, Python, Fortran and MATLAB.


Repository
==========
The master repository lives in [IPP CAS gitlab instance](https://repo.tok.ipp.cas.cz/compass-tokamak/HDC).

The bitbucket repo is just a read-only mirror.

The access to the master repo can be requested via email *fridrich at ipp.cas.cz*

API documentation
=================
The API documentation is available [here.](http://compass-tokamak.pages.tok.ipp.cas.cz/HDC/)

Building instructions
=====================

Prerequisites
-------------

To build HDC, you will need:
- c++11 compliant compiler (tested with intelstudio>=2018 and gcc>=5.0)
- gfortran >= 4.9
- Boost >= 1.48
- CMake >= 3.3
- Doxygen for documentation building
- Cython > 0.23 (there is some parsing error in 0.23)

Optionally it can use:
- Python > 3.4 (Python HDC bindings "pyHCD" and hdc-binder support)
- MATLAB > 2018a (MATLAB mex interface)
- Yahoo MDBM (recommended storage plugin working within shared memory)
- Redis + libhiredis-dev (storage plugin working within distributed memory)
- HDF5 devel libraries ((de)serialization, tested with 1.8 and 1.10)
- libs3 ((de)serialization plugin)
- flatbuffers ((de)serialization plugin)

Supported OS:
-------------

Currently all commits are automatically tested against:

 - Ubuntu 16.04 (xenial)
 - Ubuntu 18.04 (bionic)
 - Ubuntu 20.04 (focal)
 - Fedora 31
 - Centos 7

But HDC should work on any not-too-obsolette distro. If you face any problems, please, report it via email or [project issue tracker](https://repo.tok.ipp.cas.cz/dashboard/issues).


Building HDC
------------
Machine specific build instructions are available [here](docs/MACHINE_SPECIFIC_BUILD_INSTRUCTIONS.md).

There are several cmake options. The most important are:

  - `-DCMAKE_INSTALL_PREFIX=/where/to/install` make install destination.
  - `-DBUILD_DOC=ON` Whether to build and install documentation.
  - `-DBUILD_EXAMPLES=ON` Whether to build and install examples.
  - `-DENABLE_HDF5=OFF` Switch off HDF5 serialization.
  - Python, if not detected correctly:
    - `-DPYTHON_LIBRARY=/path/to/libpython.so`
    - `-DPYTHON_INCLUDE_DIR=/path/to/python/include`
  - `-DDEBUG=ON` Whether to print debugging messages.

Some of them can be edited using `ccmake .` in `build` directory.

The example of build follows:

 1. clone the git repository
 ```
 git clone git@bitbucket.org:compass-tokamak/hdc.git
 # cd into hdc
 cd hdc
 ```
 2. build in a separate build directory
 ```
 mkdir build
 cd build
 cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../install
 make -j install
 ```

 3. Optionally build & install python module. Important: numpy must be installed before pyhdc.
 ```
 cd  python
 python setup.py build
 python setup.py install
 cd ..
 ```

 4. Optionally, run Python tests (requires building HDC with HDF5).
 ```
 cd python
 python setup.py test
 cd tests_binder
 ./run
 cd ..
 ```

 5. Optionally build & run MATLAB mex interface
 ```
 cd matlab
 make -f Makefile_
 LD_LIBRARY_PATH=../../install/lib matlab -nojvm -r "run('test_matlab')"
 ```
You should see "All tests are OK..." message - in such case, the mex interface should work fine...

Building using IntelStudio
--------------------------
```
Instrucions are similar as above, just setup your environment and CC,CXX and FORTRAN variables:
source /sw/intel/parallel_studio_xe_2018/psxevars.sh
CC=icc FORTRAN=ifort CXX=icpc cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../install

```

Contributing
============

Everyone is welcome to contribute. Please use Git and merge resquest.

Versioning
----------

[bumpversion](https://github.com/c4urself/bump2version) is used for maintaining version consistently
across the repository and files.

To create a new version, use

```
bumpversion PART
```

where `PART` is either `patch`, `minor` or `major`.

Git repository needs to be synchronized afterwards:

```
git push
git push --tags
```

Basic ideas
===========
- The hierarchical structure is organized as tree.
- Subtrees (Nodes) can be accessed by path similarily to file system paths like "aaa/bbb"
- Each node can be one of several types:
  + Empty node - this is the initial state of node. Empty node does not store any data and does not have any children. By adding subnode, slice or data it's type is automaticaly changed to another type.
  + Structure/list node - the node has at least one children indexed by string path. It can only store subtrees indexed by path/integer index.
  + Array node - the node has at least one children indexed by integer. It can only store subtrees indexed by integer.
  + Data node - it only can be terminal node, it stores some data, currently char* buffer.

Path syntax
-----------
If working directly with HDC tree (i.e.: methods like `get()`, `put()`, `set()`)
The path string is internally converted to
```
hdc_path_t = std::list<hdc_index_t>
```
type where
```
hdc_index_t = boost::variant<size_t, std::string>
```
type represents single level of path. Therefore every tree node can be refferenced by:

- **empty string** representing identity, e.g.: `"//"` is ommited unless part of protocol specification like `"json://"`
- **key** (string) referencing children of hash map/dict
- **index** (non-negative integer) for referencing value in list/array
Individual keys are separated by slash, indexes are surrounded by brakets.

For example the following string
```
"aaa/bbb//ccc[5]/ddd"
```
represents the following node within the HDC tree:
```
"aaa" -> "bbb" -> "ccc" -> 5 -> "ddd"
```

For loading from or saving to outside HDC tree (methods `load()` and `save()` ), one has to also specify protocol and file path. Is such case there are two ekvivalent options:

- protocol+file path and the path within the file data are concatenated using pipe character `|`: `HDC n = HDC::load("protocol://path/to/file|path/within/the/file")`

-  two arguments are provided: `HDC n = HDC::load("protocol://path/to/file", "path/within/the/file")`

Internally the first option calls the second one, so usage of the second form spares some method calls.

The supported protocols are:

- **json**
- **json_string**
- **json_verbose**
- **uda**
- **uda_new**
- **hdf5**
- **hdc_file**
- **hdc_string**
- **flatbuffers**
- **s3**



Examples
========
The basic examples can be found in examples folder. The executables are built in CMAKE_BUILD_DIR/bin directory.

The Python examples can be run from any arbitraty folder, the only necessary thing is to set LD_LIBRARY_PATH properly:

```
LD_LIBRARY_PATH="../build/lib/" ipython hdc_fortran_module.py
```

Some benchmarks
===============
See [this file](docs/PERFORMANCE_TESTING.md)

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

- HDC supports (in private/shared memory) zero-copy data access.
- The goals are very close to ours, the way is slightly different.
