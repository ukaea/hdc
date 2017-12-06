# https://github.com/cython/cython/wiki/WrappingCPlusPlus
# https://dmtn-013.lsst.io/

from cython.operator cimport dereference as deref

from libcpp.memory cimport unique_ptr
from libcpp.string cimport string


cdef extern from "hdc.hpp":

    cdef cppclass HDC:
        HDC()
        string serialize()
        string to_json_string(int mode)


cdef class pyHDC:
    cdef unique_ptr[HDC] thisptr

    def __init__(self):
        self.thisptr.reset(new HDC())

    def dumps(self):
        return deref(self.thisptr).serialize()
