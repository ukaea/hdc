# -*- coding: utf-8
# cython: language_level=3, binding=True, profile=True, linetrace=True

# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool
cimport numpy as cnp
from cython cimport view
from libc.stdint cimport uint8_t, uint16_t, uint32_t, uint64_t, intptr_t
from libc.stdint cimport int8_t, int16_t, int32_t, int64_t
from libcpp.vector cimport vector
from cpython cimport Py_buffer, PyBUF_ND, PyBUF_C_CONTIGUOUS
import ctypes

import six
import numbers
if six.PY3:
    import collections.abc as collections_abc
else:
    import collections as collections_abc
import six
import numpy as np

# TODO workaroud for https://github.com/cython/cython/issues/534
ctypedef double* doubleptr
ctypedef void* voidptr


from libc.string cimport memcpy

cdef int from_str_to_chararray(source, char *dest, size_t N, bint ensure_nullterm) except -1:
    cdef size_t source_len = len(source)
    cdef bytes as_bytes = source.encode('ascii')    #hold reference to the underlying byte-object
    cdef const char *as_ptr = <const char *>(as_bytes)
    if ensure_nullterm:
        source_len+=1
    if source_len > N:
        raise IndexError("destination array too small")
    memcpy(dest, as_ptr, source_len)
    return 0





# we have to cdef all the constants and types we need
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
    """The ctypes equivalent of hdc_t"""
    _fields_ = [("uuid", ctypes.c_char * 37),
                ("storage_id", ctypes.c_size_t)]


# cdef the C++ interface, any method we need must be here
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
        int8_t get_rank() except +
        vector[size_t] get_shape() except +
        size_t get_storage_id() except +
        string get_uuid() except +
        void set_data[T](vector[size_t]& _shape, T* _data, unsigned long _flags) except +
        void set_data_Py(vector[size_t]& _shape, voidptr _data, char kind, int8_t itemsize, unsigned long _flags) except +
        void set_external_Py(vector[size_t]& _shape, voidptr _data, char kind, int8_t itemsize, unsigned long _flags) except +
        void set_external[T](vector[size_t]& _shape, T* _data, unsigned long _flags) except +
        voidptr as_void_ptr() except +
        string as_string() except +
        vector[string] keys() except +
        size_t childs_count() except +
        bool is_fortranorder() except +
        vector[size_t] get_strides() except +
        string serialize(string  protocol) except +
        @staticmethod
        CppHDC deserialize( string protocol, string string) except +
        @staticmethod
        CppHDC load(string uri, string datapath) except +
        void save(string uri) except +
cdef class HDC:
    """
    Python wrapper class for the HDC C++ class
    """
    # C++ class handle
    cdef CppHDC _this

    def __init__(self, data=None):
        """
        HDC constructor

        Parameters
        ----------
        data : HDC instance, six.string_types, np.ndarray, numbers.Number, collections_abc.Mapping, or collections_abc.Sequence
            Data to set (default None)
        """
        if data is None:
            # create a new C++ instance
            self._this = CppHDC()
        elif isinstance(data, self.__class__):
            #  copy constructor
            self._this = (<HDC> data)._this
        elif isinstance(data, six.string_types):
            self._this = CppHDC(<string> str(data).encode('utf-8'))
        else:
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
                new_hdc = HDC(value)
                self._this.add_child(key.encode(), new_hdc._this)
            else:
                self[key].set_data(value)
        else:
            raise NotImplementedError('Non-string keys not supported')
            # TODO support set_slice
            # libchdc.hdc_set_slice(self._c_ptr, int(key), value._c_ptr)

    def __getitem__(self, key):
        if isinstance(key, six.string_types):
            ckey = key.encode()
            if not self._this.exists(ckey):
                raise KeyError('{} key not found'.format(key))
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._this = self._this.get(<string> ckey)
            return res
        elif isinstance(key, numbers.Integral):
            res = <HDC> self.__class__()
            # TODO move to constructor
            res._this = self._this.get(<size_t> key)
            return res
        else:
            raise ValueError("key must be either string or integer")

    def to_python(self, deep=True):
        """
        Convert to native Python type data if possible

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

    cdef _set_array_data(self, cnp.ndarray data, external=False):
        cdef size_t flags  = HDCDefault
        cdef cnp.ndarray data_view
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
        cdef cnp.ndarray[cnp.int64_t, ndim=1, mode="c"] _shape = np.zeros([data_view.ndim],dtype=np.int64,order="C")
        for i in range(data_view.ndim):
            _shape[i] = data_view.shape[i]
        cdef char kind_arr[2]
        from_str_to_chararray(data.dtype.kind,kind_arr,2,1)
        cdef char kind = kind_arr[0]
        cdef int8_t itemsize = data.dtype.itemsize
        #TODO merge set_data and set_external?
        if not external:
            self._this.set_data_Py(_shape, <voidptr> data_view.data, kind, <int8_t> itemsize, flags)
        else:
            self._this.set_external_Py(_shape, <voidptr> data_view.data, kind, <int8_t> itemsize, flags)
    def set_data(self, data, external=False):
        """
        Sets data to a HDC node.

        Parameters
        ----------
        data : HDC instance, six.string_types, np.ndarray, numbers.Number, collections_abc.Mapping, or collections_abc.Sequence
            Data to set.
        external : bool
            Whether to copy data into HDC or just store a pointer
        """
        if external:
            if not isinstance(data, np.ndarray):
                NotImplementedError('external=True not supported for non np.ndarray type data')

        if isinstance(data, six.string_types):
            self._this.set_string(data.encode())
        elif isinstance(data, np.ndarray):
            self._set_array_data(data, external=external)
        elif isinstance(data, numbers.Number):
            # convert numbers to numpy
            self._set_array_data(np.asarray(data))
        elif isinstance(data, collections_abc.Mapping):
            # dict-like data
            for key, value in data.items():
                self[key] = self.__class__(value)
        elif isinstance(data, collections_abc.Sequence):
            # list, tuple etc., not string
            for value in data:
                self.append(self.__class__(value))
        else:
            raise ValueError('{} type not supported'.format(type(data)))

    def append(self, data):
        """
        Appends data to a HDC list

        Parameters
        ----------
        data : HDC instance, six.string_types, np.ndarray, numbers.Number, collections_abc.Mapping, or collections_abc.Sequence
            Data to set.
        """
        new_hdc = HDC(data)
        self._this.append(new_hdc._this)

    def dumps(self, verbose=False):
        """
        Dump to JSON string

        Parameters
        ----------
        verbose : bool
            False .. just pure JSON (default)
            True  .. append additional metadata

        Returns
        -------
        string
            A JSON string.
        """
        if verbose:
            return self._this.serialize("json_verbose".encode()).decode()
        else:
            return self._this.serialize("json".encode()).decode()

    @staticmethod
    def loads(s):
        """
        Load from JSON string

        Parameters
        ----------
        s : string
            JSON string to be loaded
        """
        res = HDC()
        cdef CppHDC new_hdc = CppHDC.deserialize("json".encode(),s.encode())
        res._this = new_hdc
        return res

    def dump(self, filename, verbose=False):
        """
        Save to json file

        Parameters
        ----------
        filename : .write supporting object (open file)
            target to write to
        verbose : bool
            False .. just pure JSON (default)
            True  .. append additional metadata
        """
        if verbose:
            self._this.save(("json://"+filename).encode())
        else:
            self._this.save(("json_verbose://"+filename).encode())

    @staticmethod
    def load(uri, datapath=''):
        """
        Loads data from some external storage

        Parameters
        ----------
        uri : string
            string specifying data source in protocol://path_to_the_file format
            e.g. json://a.txt, or hdf5://a.h5
        datapath : string
            additional path within the file, e.g. subgroup in HDF5
        """
        res = HDC()
        cdef CppHDC new_hdc = CppHDC.load(uri.encode(), datapath.encode())
        res._this = new_hdc
        return res

    def print_info(self):
        """Prints info about HDC node"""
        return self._this.print_info()

    @property
    def _as_parameter_(self):
        # used by ctypes automatic conversion
        uuid = self._this.get_uuid()
        cdef size_t storage = self._this.get_storage_id()
        return hdc_t_(uuid,storage)

    def get_type_str(self):
        """Returns string type description of HDC node, e.g. int32"""
        return self._this.get_type_str().decode()

    cdef get_type(self):
        """Returns integer representing type according to hdc_type_t enum in include/hdc_types.h """
        return self._this.get_type()

    cdef is_array(self):
        """Returns True if the node is an array, and False otherwise."""
        type_id = self.get_type()
        # check whether type id is not in non-array types
        return type_id in (HDC_INT8, HDC_INT16, HDC_INT32, HDC_INT64, HDC_UINT8,
                           HDC_UINT16, HDC_UINT32, HDC_UINT64, HDC_FLOAT, HDC_DOUBLE,
                           HDC_BOOL, )

    def __str__(self):
        # return string representation
        if self.get_type() == HDC_STRING:
            print(self._this.as_string())
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
        buffer.buf = self._this.as_void_ptr()
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
        """Returns shape of the HDC node"""
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
        """
        Saves data into HFD5 file.

        Parameters
        ----------
        filename : string
            filename of HDF5 file
        dataset_name : string
            HDF5 dataset name within the file (default "data")
        """
        self._this.save(("hdf5://"+filename).encode())

    @staticmethod
    def from_hdf5(filename, dataset_name="data"):
        """
        Loads data from HFD5 file into a new HDC container

        Parameters
        ----------
        filename : string
            filename of HDF5 file
        dataset_name : string
            HDF5 dataset name within the file (default "data")
        """
        res = HDC()
        res._this = CppHDC.load(("hdf5://"+filename).encode(), dataset_name.encode())
        return res
