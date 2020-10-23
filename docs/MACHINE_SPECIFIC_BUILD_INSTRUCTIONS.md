Building on specific machines
=============================

Build on ITER
-------------

Setup UDA & run test
```
source setup.env
bash test_uda.cpp
./test.x
```
Build HDC and run it
```
cd ~/projects/hdc
mkdir build
module load Boost/1.66.0-foss-2018a MDSplus/7.7.12-intel-2018a hdf5/1.8.15 openssl/1.0.2g postgresql/9.4.4 libmemcached/1.0.18
module load CMake/3.10.2-GCCcore-6.4.0
module load netCDF-Fortran/4.4.4-intel-2018a
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON
make -j install
../install/bin/hdc_test
```

Building on ITM Gateway How-to
------------------------------

This shows how to build HDC with MDBM support on ITM gateway.

Load modules (GCC)

```bash
module load cmake/3.5.2
module load itm-gcc/6.1.0
module load zlib/1.2.8--gnu--6.1.0  szip/2.1--gnu--6.1.0
module load hdf5/1.8.17--gnu--6.1.0
module load openmpi/1.10.7--gnu--6.1.0
module load boost/1.61.0--gnu--6.1.0
```

Load modules (Intel compiler suite)

```bash
module load cmake/3.5.2
module load intel/pe-xe-2017--binary
module load intelmpi/2017--binary
module load boost/1.61.0--intelmpi--2017--binary
module load zlib/1.2.8--gnu--6.1.0  szip/2.1--gnu--6.1.0
module load hdf5/1.8.17--intelmpi--2017--binary
export CC=mpiicc CXX=mpiicpc FC=mpiifort
```

Build MDBM backend (optional)

```
git clone https://github.com/yahoo/mdbm
cd mdbm
```
Set install prefix:
```
MDBM_PREFIX=$(realpath ~/projects/mdbm)/install
sed "s|/tmp/install|$MDBM_PREFIX|g" Makefile.base
```
Optionally disable tests (they need libcppunit):
```
sed -i.bak '/SUBDIRS/s/test //' src/Makefile
```
And finally build & install:
```
make install
```
In order to use MDBM, please, append this to `cmake` command line:
```
-DMDBM_LIBRARY=$MDBM_PREFIX/lib64 -DMDBM_INCLUDE_DIR=$MDBM_PREFIX/include
```

Build HDC
```
git clone https://bitbucket.org/compass-tokamak/hdc.git
cd hdc
```
Optionally checkout develop branch:
```
git checkout develop
```
Build & install
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
```
If you have set up **MDBM** and you still have `MDBM_PREFIX` variable:
```
cmake -DCMAKE_INSTALL_PREFIX=../install -DMDBM_LIBRARY=$MDBM_PREFIX/lib64/libmdbm.so -DMDBM_INCLUDE_DIR=$MDBM_PREFIX/include ..
```
And finally build&install:
```
make -j install
```

Test
```
make test
```

Build Python bindings (optional)
```
module load itm-python/3.6
```
We will use virtual env here, butr it can be skipped. Assuming we are still in `hdc/build` directory and have `python3` interpreter:
```
cd python
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install numpy
python3 -m pip install .
python3 -m pip install -r dev-requirements.txt
pytest
```

Test hdc-binder
```
cd tests_binder
# we we do not have C.UTF-8 locale:
export LC_ALL=en_GB.utf8 LANG=en_GB.utf8
./run
```
