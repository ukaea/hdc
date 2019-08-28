This is the example of automatic bindings (Python interface to FORTRAN function) generation.

The interface constists from the following stages:

 1. generating FORTRAN wrapping `module_bind(data_in, data_out)` subroutine with inputs and outputs stored into arguments:

     type(hdc_t), value ::  data_in
     type(hdc_t), value ::  data_out

 2. generating python interface for FORTRAN wrapping subroutine from the first stage.

We assume that the interface specification is provided (see `func` in `generate_bindings.py` for details).

The example can be run by the following commands (Python module `jinja` has to be installed):

    ./generate_bindings.py
    make
    ./call.py

List of files:

 - `hdc_fortran_test_module.f90`: example of wrapped module.
 - `generate_bindings.py`: generates `module_bind.f90`, `module_interface.py` and `Makefile`.
 - `call.py`:  prepares the input data and calls the generated python interface.
