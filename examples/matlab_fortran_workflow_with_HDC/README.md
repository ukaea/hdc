In this example, we prepare HDC object (using mex HDC interface), and then pass it into FORTRAN subroutine using MATLAB foreign function interface.

The example is organized similarly to the Python worksheet and utilizes the same library (**libhdc_fortran_module.so**) ans subroutine (**test_cpos_f2c**).

A very brief list of necessary files follows:

- **hdc_fortran_module.h:** As MATLAB's FFI needs a C header file, but FORTRAN does not have one, we have to mock it up.

- **test_cpos.m: ** This is the FORTRAN function wrapper, which loads the library, creates output HDC object (as MATBAL FFI does not support structs as a return type), creates *struct hdc_t* for both input and output and call the funtion.

- **run_this.m: ** This pre-fills some initial data to be expepected by the test_cpos_f2c() subroutine.
