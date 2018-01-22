# -*- coding: utf-8
# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool
import ctypes
import six
import numpy as np
cimport numpy as cnp
from cython cimport view
from libc.stdint cimport uint32_t, int64_t, intptr_t, int8_t
import numbers

# NP_TYPES_MAP = {
#     'float64': cnp.float64_t),
#     'int32': cnp.int32_t,
#     'int64': cnp.int64_t,
# }


cdef extern from "hdc.hpp":

    cdef cppclass CppHDC "HDC":
        CppHDC()
        string serialize()
        string to_json_string(int mode = 0)
        void set_child(string path, CppHDC* n)
        void add_child(string path, CppHDC* n)
        CppHDC* get_ptr(string path)
        bool has_child(string path)
        void set_string(string data)
        string get_type_str()
        intptr_t as_void_ptr()
        int8_t get_ndim()
        size_t* get_shape()
        # typedef unsigned long Flags;
        void set_data[T](int _ndim, size_t* _shape, T* _data, unsigned long _flags)
        # void set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type)


cdef class HDC:
    # data handle
    cdef CppHDC* _thisptr

    def __init__(self, data=None):

        if data is None:
            # create the cpp obeject
            self._thisptr = new CppHDC()
        elif isinstance(data, self.__class__):
            #  copy constructor
            self._thisptr = (<HDC> data)._thisptr
        # elif isinstance(data, CppHDC):
        #     #  copy constructor
        #     self._thisptr = <CppHDC> data
        # elif isinstance(data, ctypes.POINTER):
        #     # from C-pointer
        #     assert NotImplementedError()
        else:
            # assert NotImplementedError()
            self._thisptr = new CppHDC()
            self.set_data(data)
            # self._c_ptr = _hdc_new_empty()
            # if isinstance(data, collections.Mapping):
            #     # dict-like data
            #     for key, value in data.items():
            #         self[key] = self.__class__(value)
            # elif isinstance(data, collections.Sequence) and not isinstance(data, six.string_types):
            #     # list, tuple etc., not string
            #     for value in data:
            #         self.append(self.__class__(value))
            # else:
            #     self.set_data(data)

    def __contains__(self, key):
        if isinstance(key, six.string_types):
            return deref(self._thisptr).has_child(key.encode())
        else:
            raise ValueError('key must be a string')

    def __setitem__(self, key, value):
        if isinstance(key, six.string_types):
            if key not in self:
                # cdef CppHDC* new_hdc = new CppHDC() 
                # deref(new_hdc).set_data(value)
                new_hdc = HDC(value) 
                deref(self._thisptr).add_child(key.encode(), new_hdc._thisptr)
            else:
                self[key].set_data(value)

        # else:
        #     # key is numeric
        #     libchdc.hdc_set_slice(self._c_ptr, int(key), value._c_ptr)

    def __getitem__(self, key):
        if isinstance(key, six.string_types):
            ckey = key.encode()
            if not deref(self._thisptr).has_child(ckey):
                raise KeyError('{} key not found'.format(key))
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._thisptr = deref(self._thisptr).get_ptr(ckey)
            # deref(self._thisptr).get_ptr(ckey)
            return res
    #     else:
    #         ckey = ctypes.c_size_t(key)
    #         res = self.from_c_ptr(_hdc_get_slice(self._c_ptr, ckey))
    #         return res

    def set_data(self, data):

        cdef cnp.float64_t [:] data_view
        cdef double* data_ptr
        cdef size_t shape[1]
        cdef int ndim

        if isinstance(data, numbers.Number):
            # convert numbers to numpy
            data = np.asarray(data)

        if isinstance(data, six.string_types):
            deref(self._thisptr).set_string(data.encode())
        elif isinstance(data, np.ndarray):
            # data = np.require(data, requirements=('C', 'O'))
            # data.setflags(write=False)
            # data = np.ascontiguousarray(data)

            # Memoryview on a NumPy array
            print("Memoryview on a NumPy array")
            data_view = data
            print("data_ptr")
            # data_ptr = <double*> &data_view[0]
            print("shape")
            shape[0] = data_view.shape[0]
            print("ndim")
            ndim = data_view.ndim
            deref(self._thisptr).set_data(ndim, shape, <double*> &data_view[0], 0)
            print("set_data_c")
            # deref(self._thisptr).set_data_c(ndim, shape, <void*> data_ptr, <size_t> 13)

        else:
            raise ValueError('{} type not supported'.format(type(data)))

    def dumps(self):
        return deref(self._thisptr).to_json_string().decode()

    def get_type_str(self):
        return deref(self._thisptr).get_type_str().decode()

    def __array__(self):
        type_str = self.get_type_str()
        print('type_str: {}',format(type_str))
        # if type_str == 'hdc':
        #     return np.array(self.tolist())
        # ctype = cnp.float64_t
        # ctype = NP_TYPES_MAP.get(type_str, None)
        # if ctype is None:
        #     raise ValueError('Cannot convert {} to numpy array'.format(type_str))

        cdef int ndim = deref(self._thisptr).get_ndim()
        cdef size_t* shape = deref(self._thisptr).get_shape()

        cdef void* data_ptr
        data_ptr = <void*> deref(self._thisptr).as_void_ptr() 
        cdef view.array my_array
        my_array = <cnp.float64_t[:shape[0]]> data_ptr

        # cdef view.array my_array = <cnp.float64_t[:shape[0]]> deref(self._thisptr).as_void_ptr() 

        # numpy_array = np.asarray(my_array)

        # return numpy_array

    def asarray(self):
        return self.__array__()

