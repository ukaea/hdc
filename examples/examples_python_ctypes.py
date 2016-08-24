# -*- coding: utf-8 -*-

import ctypes
import numpy as np
import sys

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

chdc = ctypes.cdll.LoadLibrary('./libchdc.so')
# chdc = ctypes.cdll.LoadLibrary('./libchdc.so')

# function return values
hdc_new_empty = chdc.hdc_new_empty
hdc_new_empty.restype = HDC_T_P
hdc_as_int8_1d = chdc.hdc_as_int8_1d
hdc_as_int8_1d.restype = ctypes.POINTER(ctypes.c_int8)
hdc_get = chdc.hdc_get
hdc_get.restype = ctypes.POINTER(ctypes.c_int8)
hdc_has_child = chdc.hdc_has_child
hdc_has_child.restype = ctypes.c_bool

print('Python: hdc_new_empty')
tree = hdc_new_empty()

print('Python: hdc_new_empty data')
data = hdc_new_empty()
pydata = np.arange(1, 8, 2, dtype=np.int8)
pyshape = np.array(pydata.shape)
cdata = pydata.ctypes.data_as(ctypes.POINTER(ctypes.c_int8))
cshape = pyshape.ctypes.data_as(ctypes.POINTER(ctypes.c_int64))

print('Python: hdc_set_int8 data')
chdc.hdc_set_int8(data, pydata.ndim, cshape, cdata)

print('Python: hdc_as_int8_1d data')
cdata_test = hdc_as_int8_1d(data)
pydata_test = np.asarray(cdata_test[:pydata.size], dtype=pydata.dtype)
assert np.all(pydata_test == pydata)

print('Python: hdc_add_child')
path = b"group1/int8_data"
chdc.hdc_add_child(tree, path, data)

print('Python: hdc_get')
data = hdc_get(tree, path)
cdata_test = hdc_as_int8_1d(data)
pydata_test = np.asarray(cdata_test[:pydata.size], dtype=pydata.dtype)
assert np.all(pydata_test == pydata)

print('Python: hdc_has_child')
print(hdc_has_child(tree, path))

# Fortran
print("Python -> Fortran")

# fhdc = ctypes.cdll.LoadLibrary('libfhdc.so')
fhdc = ctypes.cdll.LoadLibrary('./libfhdc.so')
fhdc.hello()

# fmodule = ctypes.cdll.LoadLibrary('./hdc_fortran_module.so')
fmodule = ctypes.cdll.LoadLibrary('./libhdc_fortran_module.so')

change_data = fmodule.change_data
change_data.restype = ctypes.c_void_p
change_data.argtypes = (HDC_T_P, )

print(hdc_has_child(tree, path))

change_data(tree)
print('here???')
sys.exit(0)
node1 = hdc_get(tree, path)
cdata_test = hdc_as_int8_1d(node1)



pydata_test = np.asarray(cdata_test[:pydata.size], dtype=pydata.dtype)
# assert np.all(pydata_test == pydata)

path2 = b"group1/double_2d_data"
node2 = hdc_get(tree, path2)
