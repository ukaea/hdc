This shows how to build HDC with Anaconda3 based Python, MDBM and UDA support on ITM gateway.

## Load modules, set paths

```bash
module load gnu/6.1.0 cmake/3.5.2
module load profile/advanced
module load boost/1.61.0--gnu--6.1.0
module load mdsplus/7.7-6

export BOOST_ROOT=$BOOST_HOME
```

Use Python 3.6 Anaconda
```
source /marconi_work/eufus_gw/work/g2jurban/miniconda3/etc/profile.d/conda.sh
conda activate hdc-uda
```

## Build UDA

```bash
git clone ssh://git@git.iter.org/imas/uda.git
cd uda
git checkout develop

export UDA_HOME=$PWD/install
mkdir build
# configure with cmake
# netcdf seems broken, somehow remove the library, e.g. by brute force:
cmake -Bbuild2 -H. -DTARGET_TYPE:STRING=OTHER -DCMAKE_INSTALL_PREFIX=./install -DNETCDF_INCLUDES="" -DNETCDF_LIBRARIES_C=""
# compile and install
make -C build
make -C build/ install
# pkg-config
export PKG_CONFIG_PATH=$UDA_HOME/lib/pkgconfig:$PKG_CONFIG_PATH

# IMPORTANT: Replace the last two lines in lib/pkgconfig/uda-fat-cpp.pc:
sed -i 's/^Cflags:.*/Cflags: -DFATCLIENT -std=c++11 -I${includedir} -I${includedir}\/c++\/ /' install/lib/pkgconfig/uda-fat-cpp.pc
sed -i 's/^Libs:.*/Libs: -L${libdir} -lfatuda_cpp -ldl -lxml2 -lcrypto -lpq -L\/gw\/switm\/mdsplus\/7.7-6\/lib -lMdsLib -lMdsShr -lMdsIpShr -lTreeShr -lTdiShr /' install/lib/pkgconfig/uda-fat-cpp.pc
```

## HDC compilation
```bash
git clone https://d_fridrich@bitbucket.org/compass-tokamak/hdc.git
cd hdc
git checkout develop
```

HDC will be installed into hdc/install

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON -DBUILD_BENCHMARKS=OFF
# to use MDBM, use this instead
cmake -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON -DBUILD_BENCHMARKS=OFF \
  -DMDBM_INCLUDE_DIR=/afs/eufus.eu/user/g/g2dfrid/projects/mdbm/include \
  -DMDBM_LIBRARY=/afs/eufus.eu/user/g/g2dfrid/projects/mdbm/lib64/libmdbm.so

make -j install

cd python
python setup.py build
python setup.py test
pip install .
```
