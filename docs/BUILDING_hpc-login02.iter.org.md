# Setup UDA & run test
source setup.env
bash test_uda.cpp
./test.x

# Build HDC and run it
cd ~/projects/hdc
mkdir build
module load Boost/1.66.0-foss-2018a MDSplus/7.7.12-intel-2018a hdf5/1.8.15 openssl/1.0.2g postgresql/9.4.4 libmemcached/1.0.18
module load CMake/3.10.2-GCCcore-6.4.0
module load netCDF-Fortran/4.4.4-intel-2018a
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON
make -j install
../install/bin/hdc_test

