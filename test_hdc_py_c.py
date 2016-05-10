# -*- coding: utf-8 -*-

import ctypes
import numpy as np


class HDC_T(ctypes.Structure):
    pass


HDC_T_P = ctypes.POINTER(HDC_T)

# chdc = ctypes.cdll.LoadLibrary('./libobj.so')

# hdc_new_empty = chdc.obj_new
# hdc_new_empty.restype = HDC_T_P
# obj_setid = chdc.obj_setid

# # obj_setid = chdc.obj_setid
# # obj_setid.restype = ctypes.c_int
# # obj_setid.argtypes =

# tree = hdc_new_empty()
# print(type(tree))

# obj_setid(tree, 2)

# chdc.obj_dump(tree)

# chdc = ctypes.cdll.LoadLibrary('./build/libchdc.so')
chdc = ctypes.cdll.LoadLibrary('libchdc.so')

hdc_new_empty = chdc.hdc_new_empty
hdc_new_empty.restype = HDC_T_P

print('Python: hdc_new_empty')
tree = hdc_new_empty()
print('Python: hdc_add_child')
chdc.hdc_add_child(tree, "group1/int8_data", hdc_new_empty())
print('Python: hdc_new_empty data')
data = hdc_new_empty()
pydata = np.arange(1, 8, 2, dtype=np.int8)
pyshape = np.array(pydata.shape)
cdata = pydata.ctypes.data_as(ctypes.POINTER(ctypes.c_int8))
cshape = pyshape.ctypes.data_as(ctypes.POINTER(ctypes.c_int64))
print('Python: hdc_set_data_int8 data')
chdc.hdc_set_data_int8(data, pydata.ndim, cshape, cdata)
print('Python: hdc_as_int_1d data')
hdc_as_int_1d = chdc.hdc_as_int_1d
hdc_as_int_1d.restype = ctypes.POINTER(ctypes.c_int8)
cdata_test = hdc_as_int_1d(data)
pydata_test = np.asarray(cdata_test[:pydata.size], dtype=pydata.dtype)
assert np.all(pydata_test == pydata)
