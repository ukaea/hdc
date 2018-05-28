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
import ctypes

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
    cdef size_t HDCDefault
    cdef size_t HDCFortranOrder
    cdef size_t HDC_MAX_DIMS
cdef struct hdc_t:
    voidptr obj

cdef extern from "hdc.hpp":
    cdef cppclass HDCStorage:
        pass
    cdef cppclass CppHDC "HDC":
        CppHDC() except +
        CppHDC(string str) except +
        CppHDC(HDCStorage* _storage, const string& _uuid) except +
        string serialize() except +
        size_t get_itemsize() except +
        size_t get_datasize() except +
        char * get_pybuf_format() except +
        string to_json_string(int mode = 0) except +
        void set_child(string path, CppHDC* n) except +
        void append_slice(CppHDC* h) except +
        void add_child(string path, CppHDC* n) except +
        CppHDC* get_slice_ptr(size_t i) except +
        CppHDC* get_ptr(string path) except +
        bool has_child(string path) except +
        void set_string(string data) except +
        void print_info() except +
        size_t get_type() except +
        string get_type_str() except +
        intptr_t as_void_ptr() except +
        int8_t get_ndim() except +
        size_t* get_shape() except +
        HDCStorage* get_storage() except +
        string get_uuid() except +

        # typedef unsigned long Flags;
        void set_data[T](int _ndim, size_t* _shape, T* _data, unsigned long _flags) except +
        # void set_data_c(int _ndim, size_t* _shape, void* _data, size_t _type)
        T as[T]() except +
        string as_string() except +
        vector[string] keys() except +
        size_t childs_count() except +
        # hdc_t* caused casting errors with except +
        void* as_hdc_ptr()  except +
        bool is_fortranorder() except +
        vector[size_t] get_strides() except +
        @staticmethod
        CppHDC* new_HDC_from_c_ptr(intptr_t c_ptr) except +
        void to_hdf5(string filename, string dataset_name) except +
        @staticmethod
        CppHDC* from_hdf5_ptr(const string& filename, const string& dataset_name) except +
        @staticmethod
        CppHDC from_json(const string& filename, const string& datapath) except +
        void to_json(string filename, int mode)


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
        elif isinstance(data, six.string_types):
            self._thisptr = new CppHDC(bytes(data, 'utf-8'))
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

    def to_python(self, deep=True):
        """Convert to native Python type data if possible

        Parameters
        ----------
        deep : bool
            True for recursive conversion
        """
        type_id = self.get_type()
        # check whether type id is not in non-array types

        if type_id == HDC_STRING:
            return str(self)
        elif type_id == HDC_EMPTY:
            return None
        elif type_id == HDC_STRUCT:
            if deep:
                return {k: self[k].to_python() for k in self}
            else:
                return {k: self[k] for k in self}
        elif type_id == HDC_LIST:
            if deep:
                return [x.to_python() for x in self]
            else:
                return [x for x in self]
        elif self.is_array():
            if len(self.shape) == 0:
                return np.asscalar(np.asarray(self))
            else:
                return np.asarray(self)

        else:
            raise TypeError('Type {} not supported'.format(self.get_type_str()))

    def __iter__(self):
        # TODO implement iteration on C++ level
        type_id = self.get_type()
        if type_id == HDC_STRUCT:
            return iter(self.keys())
        elif type_id == HDC_LIST:
            return (self[i] for i in range(len(self)))
        else:
            raise TypeError('HDC type {} is not iterable'.format(self.get_type_str()))

    def __len__(self):
        if self.shape:
            return self.shape[0]
        else:
            # TODO is 0 always correct?
            return 0

    cdef _set_data(self, cnp.ndarray data):
        cdef size_t flags  = HDCDefault
        #cdef size_t flags  = HDCFortranOrder
        cdef cnp.ndarray data_view
        # data_view = np.require(data, requirements=('C', 'O'))
        if data.ndim == 0:
            # ascontiguousarray forces ndim >0= 1
            data_view = data
        else:
            # require contiguous C-array
            # TODO C-ordering vs Fortran
            if data.flags['F_CONTIGUOUS'] and not data.flags['C_CONTIGUOUS']:
                flags |= HDCFortranOrder
            data_view = np.ascontiguousarray(data)
        data_view.setflags(write=True)

        # TODO support other types
        if np.issubdtype(data.dtype, np.bool_):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <bool*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.int8):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int8_t*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.int16):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int16_t*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.int32):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int32_t*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.int64):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <int64_t*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.float32):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <float*> data_view.data, flags)
        elif np.issubdtype(data.dtype, np.float64):
            deref(self._thisptr).set_data(data_view.ndim, <size_t*> data_view.shape, <double*> data_view.data, flags)

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

    @staticmethod
    def load(filename, datapath=''):
        res = HDC()
        cdef CppHDC new_hdc = CppHDC.from_json(filename.encode(), datapath.encode())
        res._thisptr = new CppHDC(new_hdc.get_storage(), new_hdc.get_uuid())
        return res

    def print_info(self):
        return deref(self._thisptr).print_info()

    @property
    def c_ptr(self):
        return ctypes.c_void_p(<intptr_t>deref(self._thisptr).as_hdc_ptr())

    @property
    def _as_parameter_(self):
        # used by ctypes automatic conversion
        return self.c_ptr

    def dump(self, filename, mode=0):
        """Save to json file

        Parameters
        ----------
        fp : .write supporting object (open file)
            target to write to
        """
        deref(self._thisptr).to_json(filename.encode(), mode)
        # fp.write(str(self.dumps()))

    # def load(self, fp):
    #     """Load from JSON file
    #
    #     Parameters
    #     ----------
    #     fp : .write supporting object (open file)
    #     target to write to
    #     """
    #     fp.write(self.dumps())

    def get_type_str(self):
        return deref(self._thisptr).get_type_str().decode()

    cdef get_type(self):
        return deref(self._thisptr).get_type()

    cdef is_array(self):
        type_id = self.get_type()
        # check whether type id is not in non-array types
        return type_id in (HDC_INT8, HDC_INT16, HDC_INT32, HDC_INT64, HDC_UINT8,
                           HDC_UINT16, HDC_UINT32, HDC_UINT64, HDC_FLOAT, HDC_DOUBLE,
                           HDC_BOOL, )

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
        cdef vector[size_t] strides = deref(self._thisptr).get_strides()
        cdef int ndim = deref(self._thisptr).get_ndim()
        cdef Py_ssize_t strides_buf[10]
        for i in range(ndim):
            strides_buf[i] = strides[i]
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
        buffer.strides = strides_buf
        #if HDCFortranOrder & flags:
            #buffer.strides = np.cumprod(np.hstack([np.array([1]),buffer.shape[:-1]]))*itemsize
        #else:
            #buffer.strides = np.cumprod(np.hstack([buffer.shape[1:],np.array([1])])[::-1])[::-1]*itemsize
        # wont work here: TODO: move this to C++
        # for pointer arrays only

        buffer.suboffsets = NULL

    @property
    def shape(self):
        cdef int ndim = deref(self._thisptr).get_ndim()
        cdef Py_ssize_t* shape = <Py_ssize_t*> deref(self._thisptr).get_shape()
        return tuple((shape[i] for i in range(ndim)))

    def __releasebuffer__(self, Py_buffer *buffer):
        pass

    def keys(self):
        """Get access keys of containers' children
        """
        keys = deref(self._thisptr).keys()
        return (k.decode() for k in keys)

    @staticmethod
    cdef HDC _from_c_ptr(intptr_t h):
        """Working horse for method below
        """
        res = HDC()
        hh = <hdc_t*> h
        res._thisptr = <CppHDC*> hh.obj
        return res

    @staticmethod
    def from_c_ptr(h):
        """Unwraps hdc_t created by C or FORTRAN function called by ctypes.
        """
        return HDC._from_c_ptr(h)

    def to_hdf5(self, filename, dataset_name="data"):
        deref(self._thisptr).to_hdf5(filename.encode(), dataset_name.encode())

    @staticmethod
    def from_hdf5(filename, dataset_name="data"):
        res = HDC()
        res._thisptr = CppHDC.from_hdf5_ptr(filename.encode(), dataset_name.encode())
        return res
