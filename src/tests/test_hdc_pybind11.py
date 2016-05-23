import libhdc_python as hdc
import ctypes

# Get pointer from PyCapsuleObject:
hh = hdc.hdc()
capsule = hh.as_void_ptr()

from ctypes import pythonapi
pythonapi.PyCapsule_GetPointer.restype = ctypes.c_void_p
pythonapi.PyCapsule_GetPointer.argtypes = [ctypes.py_object]
c_ptr = pythonapi.PyCapsule_GetPointer(capsule,None)

# Define types

from ctypes import Structure
class HDC_T(Structure):
    _fields_ = [("obj", ctypes.c_void_p)]
HDC_T_P = ctypes.POINTER(HDC_T)

# And its instances

hdc_t =  HDC_T(c_ptr)
hdc_t_p = HDC_T_P(hdc_t)

# Now load the function and call it

fmodule = ctypes.cdll.LoadLibrary('./libhdc_fortran_module.so')
change_data = fmodule.change_data
change_data.restype = ctypes.c_void_p
change_data.argtypes = (HDC_T_P,)

change_data(hdc_t)
