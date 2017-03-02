import ctypes
import numpy as np
import six
import libhdc_python as libhdc_

__all__ = ['HDC']


C_TYPES_MAP = {
    'float64': ctypes.POINTER(ctypes.c_double),
    'int32': ctypes.POINTER(ctypes.c_int32),
    'int64': ctypes.POINTER(ctypes.c_int64),
}


# ctypes
class _HDC_T(ctypes.Structure):
    """HDC C pointer (opaque)
    """
    pass


_HDC_T_P = ctypes.POINTER(_HDC_T)


# TODO perhaps we do not need it
libchdc_ = ctypes.cdll.LoadLibrary('libchdc.so')


class HDC(object):
    """HDC Python binding"""

    def __init__(self, data=None):
        super(HDC, self).__init__()
        self._hdc_obj = libhdc_.HDC()
        if data is not None:
            self.set_data(data)

    @classmethod
    def from_void_ptr(cls, c_ptr):
        self = super(HDC, cls).__new__(cls)
        ctypes.pythonapi.PyCapsule_New.restype = ctypes.py_object
        ctypes.pythonapi.PyCapsule_New.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
                                                   ctypes.py_object]
        # TODO this crashes Python badly for an unknown reason
        # although it works well interactively :-Z
        # even in %debug !!!
        capsule = ctypes.pythonapi.PyCapsule_New(c_ptr, None, None)
        self._hdc_obj = libhdc_._from_void_ptr(capsule)
        # self._hdc_obj = libhdc_.HDC()
        return self

    @property
    def c_ptr(self):
        capsule = self._hdc_obj.as_void_ptr()
        ctypes.pythonapi.PyCapsule_GetPointer.restype = ctypes.c_void_p
        ctypes.pythonapi.PyCapsule_GetPointer.argtypes = [ctypes.py_object]
        c_ptr = ctypes.pythonapi.PyCapsule_GetPointer(capsule, None)

        return c_ptr
