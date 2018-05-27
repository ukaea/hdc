This shows how to build (and use) HDC with UDA on ubuntu 16.
# Build UDA

```bash
# install prerequisites
# mdslus via http://www.mdsplus.org/index.php/Latest_Ubuntu/Debian_Packages
apt install -y swig3.0
# get UDA from git
git clone ssh://git@git.iter.org/imas/uda.git -b develop
# optionally choose compilers
# for Ubuntu 18.04 Bionic install gcc version 6
export CC=gcc-6
export CXX=g++-6
export FC=gfortran-6
# build and install
cd uda/
# this will be used in the following
export UDA_HOME=$PWD/install
mkdir build
# netcdf seems broken, somehow remove the library, e.g. by brute force:
sudo mv /usr/lib/x86_64-linux-gnu/libnetcdf.so /usr/lib/x86_64-linux-gnu/_libnetcdf.so
# configure with cmake
cmake -Bbuild -H. -DTARGET_TYPE:STRING=OTHER -DSWIG_EXECUTABLE=$(which swig3.0) -DCMAKE_INSTALL_PREFIX=./install
# For Ubuntu 18.04, Postgres is not detected without some hints
cmake -Bbuild -H. -DTARGET_TYPE:STRING=OTHER -DSWIG_EXECUTABLE=$(which swig3.0) -DCMAKE_INSTALL_PREFIX=./install -DPostgreSQL_ADDITIONAL_VERSIONS=10.3 -DPostgreSQL_TYPE_ADDITIONAL_SEARCH_SUFFIXES=postgresql/10/server
# compile and install
make -C build
make -C build/ install
# pkg-config
export PKG_CONFIG_PATH=$UDA_HOME/lib/pkgconfig:$PKG_CONFIG_PATH

# IMPORTANT: Replace the last two lines in lib/pkgconfig/uda-fat-cpp.pc:
Cflags: -DFATCLIENT -std=c++11 -I${includedir} -I${includedir}/c++
Libs: -L${libdir} -lfatuda_cpp -ldl -lxml2 -lcrypto -lpq -L/usr/local/mdsplus/lib -lMdsLib -lMdsShr -lMdsIpShr -lTreeShr -lTdiShr

# finally, configure the environment for UDA
export UDA_PLUGIN_CONFIG=$UDA_HOME/etc/plugins/udaPlugins.conf
export UDA_SARRAY_CONFIG=$UDA_HOME/etc/udagenstruct.conf
export UDA_LOG_LEVEL=WARNING
export UDA_PLUGIN_DIR=$UDA_HOME/lib/plugins
export UDA_EXP2IMAS_MAPPING_FILE_DIRECTORY=$UDA_HOME/source/plugins/exp2imas/mappings
export UDA_PLUGIN_DEBUG_SINGLEFILE=1
export UDA_HOST="localhost"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$UDA_HOME/lib

# optionally build Python
cd $UDA_HOME/python/
python setup.py build
# this should better be done in a virtual env
pip install .

```

# Build HDC
```bash
git clone git@bitbucket.org:compass-tokamak/hdc.git
cd hdc
git checkout develop

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON -DBUILD_BENCHMARKS=OFF
make -j install

cd python
python setup.py build
# optionally test
# if hdf5 tests are failing, this is most likely because your h5py is built against a different HDF5 version
python setup.py test
# and install (into a virtual env again)
# (note that pyhdc does not depend on pyuda)
pip install .
```

# Example for TCV data
```bash
# create an ssh tunnel
ssh -fN lac911.epfl.ch -L 8001:tcvdata.epfl.ch:8000
# cd into hdc root
cd ../..
./install/bin/hdc_example_uda
```
