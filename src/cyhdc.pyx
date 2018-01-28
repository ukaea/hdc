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
from cpython cimport Py_buffer, PyBUF_ND, PyBUF_C_CONTIGUOUS


# NP_TYPES_MAP = {
#     'float64': cnp.float64_t),
#     'int32': cnp.int32_t,
#     'int64': cnp.int64_t,
# }

# TODO workaroud for https://github.com/cython/cython/issues/534
ctypedef double* doubleptr
ctypedef void* voidptr


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
        T as[T]()


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

    cdef _set_data(self, cnp.ndarray data):

        print('set data cython')

        # require contiguous C-array
        cdef cnp.ndarray data_view
        data_view = np.require(data, requirements=('C', 'O'))
        data_view.setflags(write=True)
        data_view = np.ascontiguousarray(data_view)

        # TODO support other types
        deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <double*> data_view.data, 0)

    def set_data(self, data):

        print('set data DEFAULT')

        if isinstance(data, six.string_types):
            deref(self._thisptr).set_string(data.encode())
        elif isinstance(data, np.ndarray):
            self._set_data(data)
        elif isinstance(data, numbers.Number):
            # convert numbers to numpy
            self._set_data(np.asarray(data))

        else:
            raise ValueError('{} type not supported'.format(type(data)))

    def dumps(self):
        return deref(self._thisptr).to_json_string().decode()

    def get_type_str(self):
        return deref(self._thisptr).get_type_str().decode()

    def __getbuffer__(self, Py_buffer *buffer, int flags):

        # TODO raise exception for non-buffer types

        # TODO realize contiguity
        assert flags & PyBUF_ND
        assert flags & PyBUF_C_CONTIGUOUS
        
        # TODO generalize
        cdef Py_ssize_t itemsize = sizeof(double)

        cdef Py_ssize_t* shape = <Py_ssize_t*> deref(self._thisptr).get_shape()
        cdef int ndim = deref(self._thisptr).get_ndim()
        # cdef Py_ssize_t strides[3]
        cdef Py_ssize_t size
        cdef int i

        # C-order
        # strides[ndim - 1] = itemsize
        size = shape[ndim]
        for i in range(ndim - 2, -1, -1):
            # strides[i] = itemsize * shape[i + 1]
            size *= shape[i]

        # size = product(shape)
        size = 1
        for i in range(ndim):
            size *= shape[i]

        buffer.buf = <char *> deref(self._thisptr).as[voidptr]()
        # TODO https://docs.python.org/3/c-api/arg.html#arg-parsing
        buffer.format = 'd'
        # This is for use internally by the exporting object
        buffer.internal = NULL
        # Item size in bytes of a single element
        buffer.itemsize = itemsize
        # product(shape) * itemsize
        buffer.len = size * itemsize
        buffer.ndim = ndim
        # A new reference to the exporting object - for reference counting
        buffer.obj = self
        # An indicator of whether the buffer is read-only
        buffer.readonly = 0
        # the shape of the memory as an n-dimensional array
        buffer.shape = shape
        # strides can be NULL if contiguity is not explicitely requested
        buffer.strides = NULL  # strides
        # for pointer arrays only
        buffer.suboffsets = NULL                

    def __releasebuffer__(self, Py_buffer *buffer):
        pass
