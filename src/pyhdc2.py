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
# libchdc_ = ctypes.cdll.LoadLibrary('libchdc.so')


class HDC(object):
    """HDC Python binding"""

    def __init__(self, data=None):
        super(HDC, self).__init__()
        self._hdc_obj = libhdc_.HDC()
        if data is not None:
            self.set_data(data)

    @classmethod
    def from_void_ptr(cls, c_ptr):
        """Construct HDC Python class instance from (void) pointer to a HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        self = super(HDC, cls).__new__(cls)
        self._hdc_obj = libhdc_._from_void_ptr(c_ptr)
        return self

    @property
    def c_ptr(self):
        """Return the (void) pointer to the HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        return self._hdc_obj.as_void_ptr()
