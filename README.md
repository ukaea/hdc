You will need stdc++14 compliant compiler and libdynd installed

on Abacus you can source the devtoolset-3 environment:

scl enable devtoolset-3 bash

to build, please do following:

mkdir build
cd build
cmake ..
make

cmake gives us this:

c++ -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fPIC -I/sw/libdynd/include -I.    -o hdc.cpp.o -c hdc.cpp
c++ -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fPIC -I/sw/libdynd/include -I.    -o jsoncpp.cpp.o -c jsoncpp.cpp
c++  -fPIC  -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++   -shared -Wl,-soname,libhdc.so -o libhdc.so hdc.cpp.o jsoncpp.cpp.o  -L/sw/libdynd/lib -ldynd -ldyndt -Wl,-rpath,/sw/libdynd/lib 
c++ -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -fPIC -I/sw/libdynd/include -I.    -o CMakeFiles/chdc.dir/hdc_c.cpp.o -c hdc_c.cpp
c++  -fPIC  -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++   -shared -Wl,-soname,libchdc.so -o libchdc.so CMakeFiles/chdc.dir/hdc_c.cpp.o  -L/sw/libdynd/lib libhdc.so -lstdc++ -ldynd -ldyndt -Wl,-rpath,/sw/libdynd/lib:. 
gfortran -g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 -fPIC -I/sw/libdynd/include -I.    -c hdc_fortran.f90 -o hdc_fortran.f90.o
gfortran  -fPIC -g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008   -shared -Wl,-soname,libfhdc.so -o libfhdc.so hdc_fortran.f90.o  -L/sw/libdynd/lib libchdc.so libhdc.so -ldynd -ldyndt -lstdc++ -Wl,-rpath,/sw/libdynd/lib:. 
cc -g -std=c99  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -I/sw/libdynd/include -I.    -o test_hdc_c.c.o   -c test_hdc_c.c
cc -g -std=c99  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++    test_hdc_c.c.o  -o test_hdc_c  -L/sw/libdynd/lib -rdynamic libchdc.so libhdc.so -ldynd -ldyndt -lstdc++ -Wl,-rpath,/sw/libdynd/lib:. 
c++ -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++ -I/sw/libdynd/include -I.    -o test_hdc_cpp.cpp.o -c test_hdc_cpp.cpp
c++ -g -std=c++14  -Wno-type-limits -Wall -Wextra -lgfortran -lstdc++    test_hdc_cpp.cpp.o  -o test_hdc_cpp  -L/sw/libdynd/lib -rdynamic libhdc.so -ldynd -ldyndt -Wl,-rpath,/sw/libdynd/lib:.: 
gfortran -g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 -I/sw/libdynd/include -I.    -c test_hdc_fortran.f90 -o test_hdc_fortran.f90.o
gfortran -g -ffree-line-length-none -Wall -pedantic -fbacktrace -Wall -Wextra -lstdc++ -std=f2008 test_hdc_fortran.f90.o  -o test_hdc_fortran  -L/sw/libdynd/lib -rdynamic libfhdc.so libchdc.so libhdc.so -ldynd -ldyndt -lstdc++ -Wl,-rpath,/sw/libdynd/lib:.
