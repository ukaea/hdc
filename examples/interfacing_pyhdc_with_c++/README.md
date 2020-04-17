# Inro
This is a simple example of passing HDC objects from C++ to Python and vice versa.

# Contents
* `hdcdumper.hpp` - example class producing/consuming HDC objects (through hdc_t structs)
* `hdcdumper.cpp` - ditto
* `Makefile` - receipt for building C++ containing HDCDumper class
* `cyhdcdumper.pyx` - Cython wrapper of HDCDumper class
* `setup.py` - receipt for building Cython wrapper
* `test_hdcdumper.py` - Example usage of Cython wrapper of HDCDumper class
* `run_this.sh` - Script for buildng & running the entire example

# Prerequirements
In order to build and run this, both HDC and pyhdc have to be installed.

# Setup
This example assumes that HDC is installed in hdc/install
this can be changed by manually editing the following files

* `HDC_INSTALL_PREFIX` variable in **run_this.sh**

* `hdc_root`, `hdc_install_prefix` variable in **setup.py**

# Run
`./run_this.sh`
