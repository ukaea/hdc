HDC_PATH=../../build

all: libhdc_fortran_test_module.so libhdc_fortran_test_module_bind.so

libhdc_fortran_test_module.so: hdc_fortran_test_module.f90
	gfortran  -fPIC -g -O0 -ffree-line-length-none -pedantic -fbacktrace -std=f2008 -shared \
	    -o hdc_fortran_test_module.so hdc_fortran_test_module.f90 -I$(HDC_PATH)/mod -L$(HDC_PATH)/src -lfhdc

libhdc_fortran_test_module_bind.so: hdc_fortran_test_module_bind.f90
	gfortran  -fPIC -g -O0 -ffree-line-length-none -pedantic -fbacktrace -std=f2008 -shared \
	    -o hdc_fortran_test_module_bind.so $< -I. -L. -lhdc_fortran_test_module \
	    -I$(HDC_PATH)/mod -L$(HDC_PATH)/src -lfhdc
