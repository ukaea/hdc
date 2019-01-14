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
    cdef size_t HDCExternal
    cdef size_t HDC_MAX_DIMS
    cdef size_t HDC_UUID_LENGTH
    ctypedef struct hdc_t:
        char uuid[37]
        voidptr storage
class hdc_t_(ctypes.Structure):
    _fields_ = [("uuid", ctypes.c_char * 37),
                ("storage_id", ctypes.c_size_t)]

cdef extern from "hdc.hpp":
    cdef cppclass HDCStorage:
        pass
    cdef cppclass CppHDC "HDC":
        CppHDC() except +
        CppHDC(string str) except +
        CppHDC(HDCStorage* _storage, const string& _uuid) except +
        CppHDC(hdc_t h) except +
        string serialize() except +
        size_t get_itemsize() except +
        size_t get_datasize() except +
        string to_json_string(int mode) except +
        void set_child(string path, CppHDC& n) except +
        void append(CppHDC& h) except +
        void add_child(string path, CppHDC& n) except +
        CppHDC get(size_t i) except +
        CppHDC get(string path) except +
        bool exists(string path) except +
        void set_string(string data) except +
        void print_info() except +
        size_t get_type() except +
        string get_type_str() except +
        intptr_t as_void_ptr() except +
        int8_t get_rank() except +
        vector[size_t] get_shape() except +
        size_t get_storage_id() except +
        string get_uuid() except +
        # typedef unsigned long hdc_flags_t;
        void set_data[T](vector[size_t]& _shape, T* _data, unsigned long _flags) except +
        void set_external[T](vector[size_t]& _shape, T* _data, unsigned long _flags) except +
        T as2[T]() except +
        voidptr as_void() except +
        string as_string() except +
        vector[string] keys() except +
        size_t childs_count() except +
        bool is_fortranorder() except +
        vector[size_t] get_strides() except +
        void to_hdf5(string filename, string dataset_name) except +
        @staticmethod
        CppHDC from_hdf5(const string& filename, const string& dataset_name) except +
        @staticmethod
        CppHDC from_json(const string& filename, const string& datapath) except +
        void to_json(string filename, int mode) except +
        @staticmethod
        CppHDC from_json_string(const string& json_string) except +
        @staticmethod
        CppHDC load(const string& uri, const string& datapath) except +


cdef class HDC:
    # data handle
    cdef CppHDC _this

    def __init__(self, data=None):

        if data is None:
            # create the cpp obeject
            self._this = CppHDC()
        elif isinstance(data, self.__class__):
            #  copy constructor
            self._this = (<HDC> data)._this
        elif isinstance(data, six.string_types):
            #self._this = CppHDC(bytes(data, 'utf-8'))
            self._this = CppHDC(<string> str(data).encode('utf-8'))
        else:
            # assert NotImplementedError()
            self._this = CppHDC()
            self.set_data(data)

    def __contains__(self, key):
        if isinstance(key, six.string_types):
            return self._this.exists(key.encode())
        else:
            raise ValueError('key must be a string')

    def __setitem__(self, key, value):
        if isinstance(key, six.string_types):
            if key not in self:
                # cdef CppHDC* new_hdc = new CppHDC()
                # deref(new_hdc).set_data(value)
                new_hdc = HDC(value)
                self._this.add_child(key.encode(), new_hdc._this)
            else:
                self[key].set_data(value)

        # else:
        #     # key is numeric
        #     libchdc.hdc_set_slice(self._c_ptr, int(key), value._c_ptr)

    def __getitem__(self, key):
        if isinstance(key, six.string_types):
            ckey = key.encode()
            if not self._this.exists(ckey):
                raise KeyError('{} key not found'.format(key))
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._this = self._this.get(<string> ckey)
            # self._this.get_ptr(ckey)
            return res
        elif isinstance(key, numbers.Integral):
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._this = self._this.get(<size_t> key)
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

    cdef _set_data(self, cnp.ndarray data, external=False):
        cdef size_t flags  = HDCDefault
        #cdef size_t flags  = HDCFortranOrder
        cdef cnp.ndarray data_view
        # data_view = np.require(data, requirements=('C', 'O'))
        if data.ndim == 0:
            # ascontiguousarray forces rank >0= 1
            data_view = data
        else:
            # require contiguous C-array
            # TODO C-ordering vs Fortran
            if data.flags['F_CONTIGUOUS'] and not data.flags['C_CONTIGUOUS']:
                flags |= HDCFortranOrder
            if external:
                flags |= HDCExternal
            data_view = np.ascontiguousarray(data)
        data_view.setflags(write=True)
        cdef cnp.ndarray[cnp.int64_t, ndim=1, mode="c"] _shape = np.zeros([data_view.ndim],dtype=np.int64,order="C")
        for i in range(data_view.ndim):
            _shape[i] = data_view.shape[i]
        if not external:
            # TODO support other types
            if np.issubdtype(data.dtype, np.bool_):
                self._this.set_data(_shape, <bool*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int8):
                self._this.set_data(_shape, <int8_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int16):
                self._this.set_data(_shape, <int16_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int32):
                self._this.set_data(_shape, <int32_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int64):
                self._this.set_data(_shape, <int64_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.float32):
                self._this.set_data(_shape, <float*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.float64):
                self._this.set_data(_shape, <double*> data_view.data, flags)
            else:
                NotImplementedError('Type not supported')
        else:
            # TODO support other types
            if np.issubdtype(data.dtype, np.bool_):
                self._this.set_external(_shape, <bool*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int8):
                self._this.set_external(_shape, <int8_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int16):
                self._this.set_external(_shape, <int16_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int32):
                self._this.set_external(_shape, <int32_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.int64):
                self._this.set_external(_shape, <int64_t*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.float32):
                self._this.set_external(_shape, <float*> data_view.data, flags)
            elif np.issubdtype(data.dtype, np.float64):
                self._this.set_external(_shape, <double*> data_view.data, flags)
            else:
                NotImplementedError('Type not supported')

    def set_data(self, data, external=False):
        if external:
            if not isinstance(data, np.ndarray):
                NotImplementedError('external=True not supported for non np.ndarray type data')

        if isinstance(data, six.string_types):
            self._this.set_string(data.encode())
        elif isinstance(data, np.ndarray):
            self._set_data(data, external=external)
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
        self._this.append(new_hdc._this)

    def dumps(self, mode=0):
        """Dump to JSON string"""
        return self._this.to_json_string(mode).decode()

    @staticmethod
    def loads(s):
        """Load from JSON string"""
        res = HDC()
        cdef CppHDC new_hdc = CppHDC.from_json_string(s.encode())
        #res._this = CppHDC(new_hdc.get_storage(), new_hdc.get_uuid())
        res._this = new_hdc
        return res

    def dump(self, filename, mode=0):
        """Save to json file

        Parameters
        ----------
        fp : .write supporting object (open file)
            target to write to
        """
        self._this.to_json(filename.encode(), mode)
        # with open(filename, 'w') as fp:
        #     fp.write(self.dumps())

    @staticmethod
    def load(uri, datapath=''):
        res = HDC()
        cdef CppHDC new_hdc = CppHDC.load(uri.encode(), datapath.encode())
        #res._this = CppHDC(new_hdc.get_storage(), new_hdc.get_uuid())
        res._this = new_hdc
        return res

    def print_info(self):
        return self._this.print_info()

    @property
    def _as_parameter_(self):
        # used by ctypes automatic conversion
        uuid = self._this.get_uuid()
        cdef size_t storage = self._this.get_storage_id()
        return hdc_t_(uuid,storage)

    def get_type_str(self):
        return self._this.get_type_str().decode()

    cdef get_type(self):
        return self._this.get_type()

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
            return self._this.as_string().decode()

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
        cdef Py_ssize_t itemsize = self._this.get_itemsize()

        cdef vector[size_t] shape = self._this.get_shape()
        cdef vector[size_t] strides = self._this.get_strides()
        cdef int rank = self._this.get_rank()
        cdef Py_ssize_t shape_buf[10]
        for i in range(rank):
            shape_buf[i] = shape[i]
        for i in range(rank,10):
            shape_buf[i] = 0
        cdef Py_ssize_t strides_buf[10]
        for i in range(rank):
            strides_buf[i] = strides[i]
        #buffer.buf = <char *> self._this.as2[voidptr]()
        buffer.buf = self._this.as_void()
        # TODO https://docs.python.org/3/c-api/arg.html#arg-parsing

        # Set buffer format here:
        type_id = self.get_type()
        if (type_id == HDC_EMPTY):
            buffer.format = 'null'
        elif (type_id == HDC_STRUCT):
            buffer.format = 'struct'
        elif (type_id == HDC_LIST):
            buffer.format = 'list'
        elif (type_id == HDC_INT8):
            buffer.format = 'b'
        elif (type_id == HDC_INT16):
            buffer.format = 'h'
        elif (type_id == HDC_INT32):
            buffer.format = 'i'
        elif (type_id == HDC_INT64):
            buffer.format = 'l'
        elif (type_id == HDC_UINT8):
            buffer.format = 'B'
        elif (type_id == HDC_UINT16):
            buffer.format = 'H'
        elif (type_id == HDC_UINT32):
            buffer.format = 'I'
        elif (type_id == HDC_UINT64):
            buffer.format = 'L'
        elif (type_id == HDC_FLOAT):
            buffer.format = 'f'
        elif (type_id == HDC_DOUBLE):
            buffer.format =  'd'
        elif (type_id == HDC_STRING):
            buffer.format =  's'
        elif (type_id == HDC_BOOL):
            buffer.format =  '?'
        elif (type_id == HDC_ERROR):
            buffer.format =  'error'
        else:
            buffer.format = 'unknown'

        # This is for use internally by the exporting object
        buffer.internal = NULL
        # Item size in bytes of a single element
        buffer.itemsize = itemsize
        # product(shape) * itemsize
        buffer.len = self._this.get_datasize()
        buffer.ndim = rank
        # A new reference to the exporting object - for reference counting
        buffer.obj = self
        # An indicator of whether the buffer is read-only
        buffer.readonly = 0
        # the shape of the memory as an n-dimensional array
        buffer.shape = shape_buf
        # strides can be NULL if contiguity is not explicitely requested
        buffer.strides = NULL  # strides
        buffer.strides = strides_buf
        buffer.suboffsets = NULL

    @property
    def shape(self):
        cdef int rank = self._this.get_rank()
        cdef vector[size_t] shape = self._this.get_shape()
        return tuple((shape[i] for i in range(rank)))

    def __releasebuffer__(self, Py_buffer *buffer):
        pass

    def keys(self):
        """Get access keys of containers' children
        """
        keys = self._this.keys()
        return (k.decode() for k in keys)

    def to_hdf5(self, filename, dataset_name="data"):
        self._this.to_hdf5(filename.encode(), dataset_name.encode())

    @staticmethod
    def from_hdf5(filename, dataset_name="data"):
        res = HDC()
        res._this = CppHDC.from_hdf5(filename.encode(), dataset_name.encode())
        return res
