import ctypes
from pyhdc import _HDC_T_P, HDC, libchdc
import sys
import numpy as np

fmodule = ctypes.cdll.LoadLibrary('libhdc_fortran_module.so')


def change_data(data):
    func = fmodule.change_data
    func.restype = ctypes.c_void_p
    func.argtypes = (_HDC_T_P, )

    func(data)

    return data


def test_cpos(equilibrium):
    distsource = HDC()
    func = fmodule.test_cpos
    func.restype = ctypes.c_void_p
    # func.argtypes = (_HDC_T_P, )
    func(equilibrium.c_ptr, distsource.c_ptr)

    return distsource


def test_test_cpos():
    import numpy as np

    # for dtype in (np.float_, np.int32, ):
    #     tree = HDC()
    #     pydata = np.arange(1, 8, 2, dtype=dtype) * 0.1
    #     tree["data"] = pydata
    #     data = tree["data"].as_array()
    #     print(data)
    #     assert np.all(pydata == data)
    #     assert pydata.dtype == data.dtype

    # equilibrium = HDC()
    for i in range(2):
        print(i)
        # equi_slice = HDC()
        # equi_slice['time'] = float(i)
        # equi_slice['profiles_1d'] = HDC()
        for j in range(2):
            print("j = %d" % j)
            profiles_1d = HDC()
            # put some data
            pydata = np.arange(4, dtype=np.float) * 0.1 + i * 10 + j
            # print('nrefs: {}'.format(sys.getrefcount(pydata)))
            # print(id(pydata))
            profiles_1d['psi'] = pydata
            print('-- nrefs after: {}'.format(sys.getrefcount(pydata)))
            # print(profiles_1d['psi']._np_data is pydata)
            # buf = np.getbuffer(pydata)
            # buf = memoryview(pydata.data)
            # print(buf)
            del pydata
            # del buf
            # profiles_1d['psi'] = np.arange(4, dtype=np.float) * 0.1 + i * 10 + j
            # profiles_1d['psi'] = np.random.rand(4)
            # append slice
            # equi_slice['profiles_1d'].append(profiles_1d)
            # or using set_slice
            # equi_slice['profiles_1d'][j] = profiles_1d
            psi = profiles_1d['psi'].as_array()
            print(psi)
            print(psi.dtype)
            # assert np.all(pydata == psi)

            # equilibrium.append(equi_slice)

            # return equilibrium


def test_cpos_f2c(equilibriumin):

    tree = HDC()

    fmodule.test_cpos_f2c(equilibriumin.c_ptr, tree.c_ptr)

    return tree


if __name__ == '__main__':
    # equilibrium = test_test_cpos()

    equilibrium = HDC()
    equilibrium['profiles_1d_psi'] = np.linspace(0, 1, 5)
    # equilibrium['profiles_1d_psi'] = np.array(4.5)
    # libchdc.hdc_set_data_double_path(
    #     equilibrium.c_ptr, "time".encode(), ctypes.c_int8(0),
    #     ctypes.byref(ctypes.c_int64(0)), ctypes.byref(ctypes.c_double(1.1)))
    equilibrium['time'] = np.array(2.34)

    print("equilibrium['time'] -> {}".format(equilibrium['time'].as_array()))
    print("equilibrium['profiles_1d_psi'] -> {}".format(equilibrium['profiles_1d_psi'].as_array()))

    # print('py call c_test_cpos')
    # fmodule.c_test_cpos(equilibrium.c_ptr)
    # print('-- py call c_test_cpos end --')

    tree = test_cpos_f2c(equilibrium)

    print("=== Python dump")
    tree.dump()

    print('tree["distsourceout/source/profiles_1d_psi"] %s' %
          tree["distsourceout/source/profiles_1d_psi"].as_array())

    # distsource = test_cpos(equilibrium)
