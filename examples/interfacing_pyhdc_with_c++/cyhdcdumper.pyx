# -*- coding: utf-8
# cython: language_level=3, binding=True, profile=True, linetrace=True
# This is example of Cython-wrapper of C++ class producing/consuming HDC objects.
# Because AFAIK Cython cannot work directly with C++ instance, you have to pass hdc_t structure instead.
import ctypes
from libc.string cimport memcpy
from pyhdc.cyhdc import hdc_t_
from pyhdc import HDC

cdef extern from "hdc_types.h":
    ctypedef struct hdc_t:
        char uuid[16]
        size_t storage_id

cdef extern from "hdc.hpp":
    cdef cppclass CppHDC "HDC":
        CppHDC() except +

cdef extern from "hdcdumper.hpp":
    cdef cppclass CppHDCDumper  "HDCDumper":
        CppHDCDumper() except +
        void dump(hdc_t h) except +
        hdc_t make_hdc_t() except +

cdef class HDCDumper:
    """
    Python wrapper class for the HDCDumper class
    """
    cdef CppHDCDumper _this

    def __init__(self):
        self._this = CppHDCDumper()

    def dump(self, n):
        n_par = n._as_parameter_
        cdef hdc_t obj
        cdef size_t ctypes_buffer = ctypes.addressof(n_par)
        memcpy(&obj, <void*> ctypes_buffer, sizeof(hdc_t))
        self._this.dump(obj)

    def make_hdc_t(self):
        cdef hdc_t obj = self._this.make_hdc_t()
        ret = hdc_t_()
        cdef size_t ctypes_buffer = ctypes.addressof(ret)
        memcpy(<void*> ctypes_buffer, &obj, sizeof(hdc_t))
        return ret
