This shows how to build (and use) HDC with UDA on ubuntu 16.
# Build UDA

```bash
#install mdslus

apt install -y swig3.0
git clone ssh://git@git.iter.org/imas/uda.git -b develop
cd uda/
mkdir build
sudo mv /usr/lib/x86_64-linux-gnu/libnetcdf.so /usr/lib/x86_64-linux-gnu/_libnetcdf.so
cmake -Bbuild -H. -DTARGET_TYPE:STRING=OTHER -DSWIG_EXECUTABLE=$(which swig3.0) -DCMAKE_INSTALL_PREFIX=./
make -C build
make -C build/ install
cd python/
python setup.py build
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/david/projects/uda/python/build/lib.linux-x86_64-2.7/ PYTHON_PATH=$PYTHON_PATH:/home/david/projects/uda/python/build/lib.linux-x86_64-2.7/ ipython

cd ../../ # UDA ROOT
```
Replace the last line in lib/pkgconfig/uda-fat-cpp.pc:
```
# pkg-config file for the Universal Data Access library
prefix=/home/david/projects/uda
exec_prefix=${prefix}
includedir=${prefix}/include/uda
libdir=${exec_prefix}/lib

Name: UDA
Description: The Universal Data Access library
URL: http://www.iter.org/UDA
Version: 2.1.0
Cflags: -DFATCLIENT -std=c++11 -I${includedir} -I${includedir}/c++
Libs: -L${libdir} -lfatuda_cpp -ldl -lxml2 -lcrypto -lpq -L/usr/local/mdsplus/lib -lMdsLib -lMdsShr -lMdsIpShr -lTreeShr -lTdiShr
```
# Build HDC
```bash
cd hdc
. doxy/ntb.env
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DENABLE_UDA=ON -DBUILD_BENCHMARKS=OFF
make -j install
```
# Run
```bash
ssh -fN fridrich@lac911.epfl.ch -L 8001:tcvdata.epfl.ch:8000
../install/bin/hdc_example_uda
```
