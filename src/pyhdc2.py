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
    def from_cpp_ptr(cls, cpp_ptr):
        """Construct HDC Python class instance from (void) pointer to a HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        self = super(HDC, cls).__new__(cls)
        self._hdc_obj = libhdc_.from_cpp_ptr(cpp_ptr)
        return self

    @classmethod
    def from_c_ptr(cls, c_ptr):
        """Construct HDC Python class instance from (void) pointer to a HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        self = super(HDC, cls).__new__(cls)
        self._hdc_obj = libhdc_.from_c_ptr(c_ptr)
        return self

    @property
    def cpp_ptr(self):
        """Return the (void) pointer to the HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        return self._hdc_obj.as_cpp_ptr()

    @property
    def c_ptr(self):
        """Return the (void) pointer to the HDC C++ instance
        (i.e. not struct hdc_t*)
        """
        return self._hdc_obj.as_c_ptr()

    @property
    def stype(self):
        """Return string type
        """
        return self._hdc_obj.get_type_str()

    def dump(self):
        print(self._hdc_obj.to_json_string())

    def set_data(self, data):
        """Store data into the container
        """
        if isinstance(data, (np.ndarray, np.generic)):
            # cdata = np.ctypeslib.as_ctypes(data)
            data = np.require(data, requirements=('C', 'O'))
            data.setflags(write=False)

            self._hdc_obj.set_numpy(data)
        elif isinstance(data, six.string_types):
            self._hdc_obj.set_string(data)
        else:
            raise NotImplementedError("{typ} not supported in HDC.set_data".format(typ=type(data)))

    # def __setitem__(self, key, value):
    #     if isinstance(key, six.string_types):
    #         ckey = key.encode()
    #         if not _hdc_has_child(self._c_ptr, ckey):
    #             new_hdc = self.__class__(value)
    #             # TODO this is the problem - new_hdc gets lost from Python
    #             # we have to explain numpy not to deallocate the buffer
    #             libchdc.hdc_add_child(self._c_ptr, ckey, new_hdc._c_ptr)
    #         else:
    #             self[key].set_data(value)

    #     else:
    #         # key is numeric
    #         libchdc.hdc_set_slice(self._c_ptr, int(key), value._c_ptr)

    # def __getitem__(self, key):
    #     if isinstance(key, six.string_types):
    #         ckey = key.encode()
    #         if not _hdc_has_child(self._c_ptr, ckey):
    #             raise KeyError('key not found')
    #         res = self.from_c_ptr(_hdc_get(self._c_ptr, ckey))
    #         return res
    #     else:
    #         ckey = ctypes.c_size_t(key)
    #         res = self.from_c_ptr(_hdc_get_slice(self._c_ptr, ckey))
    #         return res
