# -*- coding: utf-8
# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref

from libcpp.memory cimport unique_ptr
from libcpp.string cimport string


cdef extern from "hdc.hpp":

    cdef cppclass CppHDC "HDC":
        CppHDC()
        string serialize()
        string to_json_string(int mode = 0)
        void set_child(string path, CppHDC* n)
        void add_child(string path, CppHDC* n)
        void set_string(string data)


cdef class HDC:
    # cdef unique_ptr[HDC] thisptr
    cdef CppHDC* thisptr

    def __init__(self):
        self.thisptr = new CppHDC()
        # create a new HDC object to test that it's not deallocated
        cdef CppHDC* n = new CppHDC()
        deref(self.thisptr).add_child('test', n)
        deref(n).set_string('n-data Ͼ Ϣ')

    def dumps(self):
        return deref(self.thisptr).to_json_string().decode()
