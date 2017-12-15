# -*- coding: utf-8
# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref
from libcpp.string cimport string
from libcpp cimport bool
import ctypes
import six


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
        if isinstance(data, six.string_types):
            deref(self._thisptr).set_string(data.encode())
        else:
            raise ValueError('{} type not supported'.format(type(data)))

    def dumps(self):
        return deref(self._thisptr).to_json_string().decode()
