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
        if isinstance(data, libhdc_.HDC):
            print('# copy constructor')
            self._hdc_obj = data
        else:
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

    def __array__(self, copy=False, *args, **kwargs):
        return np.array(self._hdc_obj, *args, copy=copy, **kwargs)

    def asarray(self, copy=False):
        return np.asarray(self, copy=copy)

    def __contains__(self, item):
        print('contains')
        if isinstance(item, six.string_types):
            return self._hdc_obj.has_child(item)
        else:
            raise NotImplementedError()

    def __setitem__(self, key, value):
        print('setitem')
        if isinstance(key, six.string_types):
            if key in self:
                self[key].set_data(value)
            else:
                # TODO test if this is an empty or struct type container
                # new_hdc = self.__class__(value)
                # print('new_hdc')
                # new_hdc.dump()
                # # TODO this is the problem - new_hdc gets lost from Python
                # # we have to explain numpy not to deallocate the buffer
                # self._hdc_obj.add_child(key, new_hdc._hdc_obj)
                # self.dump()
                # print('done set')
                print('add empty')
                new_hdc_obj = self._hdc_obj.add_child(key)
                new_hdc = self.__class__(new_hdc_obj)
                # new_hdc = self.__class__(self._hdc_obj.get(key))
                new_hdc.dump()
                new_hdc.set_data(value)
                new_hdc.dump()
                self.dump()
                print('done set')
        else:
            # key is numeric
            raise NotImplementedError('Index assigment not implemented')

    def __getitem__(self, key):
        print('getitem')
        if isinstance(key, six.string_types):
            if key not in self:
                raise KeyError('key not found')
            res = self.__class__(self._hdc_obj.get(key))
            return res
        else:
            raise NotImplementedError('Slicing not implemented')
