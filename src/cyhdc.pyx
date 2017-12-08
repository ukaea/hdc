# -*- coding: utf-8
# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref

from libcpp.memory cimport unique_ptr
from libcpp.string cimport string


cdef extern from "hdc.hpp":

    cdef cppclass HDC:
        HDC()
        string serialize()
        string to_json_string(int mode = 0)
        void set_child(string path, HDC* n)
        void add_child(string path, HDC* n)
        void set_string(string data)


cdef class pyHDC:
    # cdef unique_ptr[HDC] thisptr
    cdef HDC* thisptr

    def __init__(self):
        self.thisptr = new HDC()
        # create a new HDC object to test that it's not deallocated
        cdef HDC* n = new HDC()
        deref(self.thisptr).add_child('test', n)
        deref(n).set_string('n-data Ͼ Ϣ')

    def dumps(self):
        return deref(self.thisptr).to_json_string().decode()
