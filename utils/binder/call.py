#! /usr/bin/env python3
from pyhdc import HDC
from  hdc_fortran_test_module_interface import test_cpos

def init_cpo():
    import numpy as np
    equilibrium = HDC()
    equilibrium['profiles_1d/psi'] = np.linspace(0, 1, 5)
    equilibrium['time'] = 2.34
    return equilibrium

if __name__ == "__main__":
    equilibriumin = init_cpo()
    distsourceout, float_out = test_cpos(equilibriumin,1)
    print("float_out = {}".format(float_out))
