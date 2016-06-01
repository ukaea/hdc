import ctypes
from ctypes import byref
import numpy as np
import six
# import libhdc_python as _libhdc

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


libchdc = ctypes.cdll.LoadLibrary('libchdc.so')
_hdc_new_empty = libchdc.hdc_new_empty
_hdc_new_empty.restype = _HDC_T_P
_hdc_as_int_1d = libchdc.hdc_as_int_1d
_hdc_as_int_1d.restype = ctypes.POINTER(ctypes.c_long)
_hdc_as_voidptr = libchdc.hdc_as_voidptr
_hdc_as_voidptr.restype = ctypes.c_void_p
_hdc_get_child = libchdc.hdc_get_child
_hdc_get_child.restype = ctypes.POINTER(ctypes.c_int8)
_hdc_has_child = libchdc.hdc_has_child
_hdc_has_child.restype = ctypes.c_bool
_hdc_get_type = libchdc.hdc_get_type
_hdc_get_type.restype = ctypes.c_uint8
_hdc_get_slice = libchdc.hdc_get_slice
_hdc_get_slice.restype = _HDC_T_P
_hdc_get_shape = libchdc.hdc_get_shape
_hdc_get_shape.restype = ctypes.POINTER(ctypes.c_long)
_hdc_get_ndim = libchdc.hdc_get_ndim
_hdc_get_ndim.restype = ctypes.c_int8
_hdc_get_type_str = libchdc.hdc_get_type_str
_hdc_get_type_str.restype = ctypes.c_char_p


class HDC(object):
    """HDC Python binding"""

    _NP_REFS = {}

    def __init__(self, data=None):
        super(HDC, self).__init__()
        self._c_ptr = _hdc_new_empty()
        if data is not None:
            self.set_data(data)

    @classmethod
    def from_c_ptr(cls, c_ptr):
        self = object.__new__(cls)
        self._c_ptr = c_ptr
        return self

    @property
    def c_ptr(self):
        return self._c_ptr

    @c_ptr.setter
    def c_ptr(self, value):
        raise ValueError("c_ptr cannot be set")

    @c_ptr.deleter
    def c_ptr(self, value):
        raise ValueError("c_ptr cannot be deleted")

    @property
    def shape(self):
        return self.get_shape()

    def __setitem__(self, key, value):
        if isinstance(key, six.string_types):
            ckey = key.encode()
            if not _hdc_has_child(self._c_ptr, ckey):
                new_hdc = self.__class__(value)
                # TODO this is the problem - new_hdc gets lost from Python
                # we have to explain numpy not to deallocate the buffer
                libchdc.hdc_add_child(self._c_ptr, ckey, new_hdc._c_ptr)
            else:
                self[key].set_data(value)

        else:
            # key is numeric
            libchdc.hdc_set_slice(self._c_ptr, int(key), value._c_ptr)

    def __getitem__(self, key):
        if isinstance(key, six.string_types):
            ckey = key.encode()
            if not _hdc_has_child(self._c_ptr, ckey):
                raise KeyError('key not found')
            res = self.from_c_ptr(_hdc_get_child(self._c_ptr, ckey))
            return res
        else:
            ckey = ctypes.c_size_t(key)
            res = self.from_c_ptr(_hdc_get_slice(self._c_ptr, ckey))
            return res

    def set_data(self, data):
        """Store data into the container
        """
        if isinstance(data, np.ndarray):
            # cdata = np.ctypeslib.as_ctypes(data)
            data = np.require(data, requirements=('C', 'O'))
            data.setflags(write=False)
            # data = np.ascontiguousarray(data)
            cshape = np.ctypeslib.as_ctypes(np.array(data.shape,
                                                     dtype=np.int64))
            cndim = ctypes.c_int8(data.ndim)
            if np.issubdtype(data.dtype, np.int8):
                cdata = data.ctypes.data_as(ctypes.POINTER(ctypes.c_int8))
                libchdc.hdc_set_data_int8(self._c_ptr, cndim, byref(cshape), cdata)
            elif np.issubdtype(data.dtype, np.int32):
                cdata = data.ctypes.data_as(ctypes.POINTER(ctypes.c_int32))
                libchdc.hdc_set_data_int32(self._c_ptr, cndim, byref(cshape), cdata)
            elif np.issubdtype(data.dtype, np.int64):
                cdata = data.ctypes.data_as(ctypes.POINTER(ctypes.c_int64))
                libchdc.hdc_set_data_int64(self._c_ptr, cndim, byref(cshape), cdata)
            elif np.issubdtype(data.dtype, np.float_):
                cdata = data.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
                libchdc.hdc_set_data_double(self._c_ptr, cndim, byref(cshape), cdata)
            # TODO temporary solution - keep reference to the original numpy object
            #      so that the memory bleck does not get deallocated
            self._push_np_ref(self._c_ptr, data)

    @classmethod
    def _push_np_ref(cls, hdc_c_ptr, np_obj):
        cls._NP_REFS[ctypes.addressof(hdc_c_ptr.contents)] = np_obj

    def get_type_str(self):
        return _hdc_get_type_str(self._c_ptr).decode()

    def get_shape(self):
        ndim = _hdc_get_ndim(self._c_ptr)
        cshape = _hdc_get_shape(self._c_ptr)
        shape = tuple((cshape[i] for i in range(ndim)))
        return shape

    def tolist(self):
        type_str = self.get_type_str()
        if type_str == 'hdc':
            res = [_hdc_get_slice(self._c_ptr, i) for i in range(self.shape[0])]
        else:
            res = self.as_array().tolist()

        return res

    def as_array(self):
        """Convert to a numpy array, sharing numerical data
        """
        type_str = self.get_type_str()
        if type_str == 'hdc':
            return np.array(self.tolist())
        ctype = C_TYPES_MAP.get(type_str, None)
        if ctype is None:
            raise ValueError('Cannot convert {} to numpy array'.format(type_str))
        c_void_ptr = _hdc_as_voidptr(self._c_ptr)
        cdata = ctypes.cast(c_void_ptr, ctype)
        res = np.ctypeslib.as_array(cdata, self.get_shape())

        return res


if __name__ == '__main__':

    for dtype in (np.float_, np.int32, ):
        tree = HDC()
        pydata = np.arange(1, 8, 2, dtype=dtype)
        tree["data"] = pydata
        del pydata
        data = tree["data"].as_array()
        print(data)
        pydata = np.arange(1, 8, 2, dtype=dtype)
        assert np.all(pydata == data)
        assert pydata.dtype == data.dtype
