# -*- coding: utf-8
# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool
cimport numpy as cnp
from cython cimport view
from libc.stdint cimport uint32_t, intptr_t
from libc.stdint cimport int8_t, int16_t, int32_t, int64_t
from libcpp.vector cimport vector
from cpython cimport Py_buffer, PyBUF_ND, PyBUF_C_CONTIGUOUS

import numbers
import collections
import six
import numpy as np

# NP_TYPES_MAP = {
#     'float64': cnp.float64_t),
#     'int32': cnp.int32_t,
#     'int64': cnp.int64_t,
# }

# TODO workaroud for https://github.com/cython/cython/issues/534
ctypedef double* doubleptr
ctypedef void* voidptr

cdef extern from "hdc_types.h":
    cdef size_t HDC_EMPTY
    cdef size_t HDC_STRUCT
    cdef size_t HDC_LIST
    cdef size_t HDC_INT8
    cdef size_t HDC_INT16
    cdef size_t HDC_INT32
    cdef size_t HDC_INT64
    cdef size_t HDC_UINT8
    cdef size_t HDC_UINT16
    cdef size_t HDC_UINT32
    cdef size_t HDC_UINT64
    cdef size_t HDC_FLOAT
    cdef size_t HDC_DOUBLE
    cdef size_t HDC_STRING
    cdef size_t HDC_BOOL
    cdef size_t HDC_ERROR


cdef extern from "hdc.hpp":

    cdef cppclass CppHDC "HDC":
        CppHDC()
        string serialize()
        size_t get_itemsize()
        size_t get_datasize()
        const char * get_pybuf_format()
        string to_json_string(int mode = 0)
        void set_child(string path, CppHDC* n)
        void append_slice(CppHDC* h)
        void add_child(string path, CppHDC* n)
        CppHDC* get_slice_ptr(size_t i)
        CppHDC* get_ptr(string path)
        bool has_child(string path)
        void set_string(string data)
        size_t get_type()
        string get_type_str()
        intptr_t as_void_ptr()
        int8_t get_ndim()
        size_t* get_shape()
        # typedef unsigned long Flags;
        void set_data[T](int _ndim, size_t* _shape, T* _data, unsigned long _flags)
        # void set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type)
        T as[T]()
        string as_string()
        vector[string] keys()
        size_t childs_count()


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
        else:
            # assert NotImplementedError()
            self._thisptr = new CppHDC()
            self.set_data(data)

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
        elif isinstance(key, numbers.Integral):
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._thisptr = deref(self._thisptr).get_slice_ptr(<size_t> key)
            return res
        else:
            raise ValueError("key must be either string or integer")

    cdef _set_data(self, cnp.ndarray data):

        cdef cnp.ndarray data_view
        # data_view = np.require(data, requirements=('C', 'O'))
        if data.ndim == 0:
            # ascontiguousarray forces ndim >0= 1
            data_view = data
        else:
            # require contiguous C-array
            # TODO C-ordering vs Fortran
            data_view = np.ascontiguousarray(data)
        data_view.setflags(write=True)

        # TODO support other types
        if np.issubdtype(data.dtype, np.int8):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int8_t*> data_view.data, 0)
        elif np.issubdtype(data.dtype, np.int16):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int16_t*> data_view.data, 0)
        elif np.issubdtype(data.dtype, np.int32):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int32_t*> data_view.data, 0)
        elif np.issubdtype(data.dtype, np.int64):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int64_t*> data_view.data, 0)
        elif np.issubdtype(data.dtype, np.float32):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <float*> data_view.data, 0)
        elif np.issubdtype(data.dtype, np.float64):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <double*> data_view.data, 0)

        else:
            NotImplementedError('Type not supported')

    def set_data(self, data):

        if isinstance(data, six.string_types):
            deref(self._thisptr).set_string(data.encode())
        elif isinstance(data, np.ndarray):
            self._set_data(data)
        elif isinstance(data, numbers.Number):
            # convert numbers to numpy
            self._set_data(np.asarray(data))
        elif isinstance(data, collections.Mapping):
            # dict-like data
            for key, value in data.items():
                self[key] = self.__class__(value)
        elif isinstance(data, collections.Sequence):
            # list, tuple etc., not string
            for value in data:
                self.append(self.__class__(value))
        else:
            raise ValueError('{} type not supported'.format(type(data)))

    def append(self, data):
        new_hdc = HDC(data)
        deref(self._thisptr).append_slice(new_hdc._thisptr)

    def dumps(self):
        return deref(self._thisptr).to_json_string().decode()

    def dump(self, fp):
        """Save to json file

        Parameters
        ----------
        fp : .write supporting object (open file)
            target to write to
        """
        fp.write(self.dumps())

    def get_type_str(self):
        return deref(self._thisptr).get_type_str().decode()

    cdef get_type(self):
        return deref(self._thisptr).get_type()

    cdef is_array(self):
        type_id = self.get_type()
        # check whether type id is not in non-array types
        return  type_id not in (HDC_EMPTY,
                                HDC_STRUCT,
                                HDC_LIST,
                                HDC_STRING,
                                HDC_BOOL,
                                HDC_ERROR)

    def __str__(self):
        # return string representation
        # TODO
        if self.get_type() == HDC_STRING:
            return deref(self._thisptr).as_string().decode()

        if self.is_array():
            return(str(np.asarray(self)))

        else:
            # fall back to repr
            return repr(self)

    def __getbuffer__(self, Py_buffer *buffer, int flags):

        # TODO raise exception for non-buffer types

        # TODO realize contiguity
        assert flags & PyBUF_ND
        assert flags & PyBUF_C_CONTIGUOUS
        
        # TODO generalize
        cdef Py_ssize_t itemsize = deref(self._thisptr).get_itemsize()

        cdef Py_ssize_t* shape = <Py_ssize_t*> deref(self._thisptr).get_shape()
        cdef int ndim = deref(self._thisptr).get_ndim()

        buffer.buf = <char *> deref(self._thisptr).as[voidptr]()
        # TODO https://docs.python.org/3/c-api/arg.html#arg-parsing
        buffer.format = deref(self._thisptr).get_pybuf_format()  # 'd'
        # This is for use internally by the exporting object
        buffer.internal = NULL
        # Item size in bytes of a single element
        buffer.itemsize = itemsize
        # product(shape) * itemsize
        buffer.len = deref(self._thisptr).get_datasize()
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

    def keys(self):
        """Get access keys of containers' children
        """
        keys = deref(self._thisptr).keys()
        return (k.decode() for k in keys)
