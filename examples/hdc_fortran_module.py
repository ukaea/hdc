import ctypes
from pyhdc import HDC
import sys
import numpy as np

fm = ctypes.cdll.LoadLibrary('libhdc_fortran_module.so')


def test_cpos(equilibriumin):
    """test_cpos Python wrapper

    Args:
        equilibriumin(HDC): input
        result(HDC): output container
    """

    # in the Python / C wrapper, the output tree must be constructed
    tree = HDC()
    # the Fortran wrapper is called with output tree already allocated
    fm.test_cpos_f2c(equilibriumin.c_ptr, tree.c_ptr)

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

    print("equilibrium['time'] -> {}".format(np.asarray(equilibrium['time'])))
    print("equilibrium['profiles_1d/psi'] -> {}".format(np.asarray(equilibrium['profiles_1d/psi'])))

    # print('py call c_test_cpos')
    # fm.c_test_cpos(equilibrium.c_ptr)
    # print('-- py call c_test_cpos end --')

    tree = test_cpos(equilibrium)

    print("=== Python dump")
    tree.dumps()

    print('tree["distsourceout/source/profiles_1d/psi"] %s' %
          np.asarray(tree["distsourceout/source/profiles_1d/psi"]))

    print("tree.keys() = %s" % list(tree.keys()))
    print("tree['distsourceout'].keys() = %s" % list(tree["distsourceout"].keys()))
    print("tree['distsourceout/source/profiles_1d'].keys() = %s" % list(tree['distsourceout/source/profiles_1d'].keys()))     # distsource = test_cpos(equilibrium)

    fm.test_hdc_create.restype = ctypes.c_void_p
    h = HDC.from_c_ptr(fm.test_hdc_create())
    print(np.asarray(h))

    fm.test_hdc_dump(h.c_ptr)

    fm.test_hdc_modify(h.c_ptr)
    print(np.asarray(h))
