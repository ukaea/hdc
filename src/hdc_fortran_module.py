import ctypes
from pyhdc import _HDC_T_P, HDC, libchdc
import sys
import numpy as np

fmodule = ctypes.cdll.LoadLibrary('libhdc_fortran_module.so')


def test_cpos(equilibriumin):
    """test_cpos Python wrapper

    Args:
        equilibriumin(HDC): input
        result(HDC): output container
    """

    # in the Python / C wrapper, the output tree must be constructed
    tree = HDC()
    # the Fortran wrapper is called with output tree already allocated
    fmodule.test_cpos_f2c(equilibriumin.c_ptr, tree.c_ptr)

    return tree


if __name__ == '__main__':
    # equilibrium = test_test_cpos()

    equilibrium = HDC()
    psi = np.linspace(0, 1, 5)
    equilibrium['profiles_1d/psi'] = psi
    del psi
    # equilibrium['profiles_1d/psi'] = np.array(4.5)
    # libchdc.hdc_set_data_double_path(
    #     equilibrium.c_ptr, "time".encode(), ctypes.c_int8(0),
    #     ctypes.byref(ctypes.c_int64(0)), ctypes.byref(ctypes.c_double(1.1)))
    equilibrium['time'] = np.array(2.34)

    print("equilibrium['time'] -> {}".format(equilibrium['time'].as_array()))
    print("equilibrium['profiles_1d/psi'] -> {}".format(equilibrium['profiles_1d/psi'].as_array()))

    # print('py call c_test_cpos')
    # fmodule.c_test_cpos(equilibrium.c_ptr)
    # print('-- py call c_test_cpos end --')

    tree = test_cpos(equilibrium)

    print("=== Python dump")
    tree.dump()

    print('tree["distsourceout/source/profiles_1d/psi"] %s' %
          tree["distsourceout/source/profiles_1d/psi"].as_array())

    # distsource = test_cpos(equilibrium)
