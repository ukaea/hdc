# -*- coding: utf-8
# cython: language_level=3, binding=True, profile=True, linetrace=True
import ctypes
from libc.string cimport memcpy
from pyhdc.cyhdc import hdc_t_

cdef extern from "hdc_types.h":
    ctypedef struct hdc_t:
        char uuid[16]
        size_t storage_id

cdef extern from "hdc.hpp":
    cdef cppclass HDCStorage:
        pass
    cdef cppclass CppHDC "HDC":
        CppHDC() except +

cdef extern from "hdcdumper.hpp":
    cdef cppclass CppHDCDumper  "HDCDumper":
        CppHDCDumper() except +
        void dump_obj(CppHDC h) except +
        #void dump_struct(hdc_t h) except +
        CppHDC make_hdc() except +
        hdc_t make_hdc_t() except +

cdef class HDCDumper:
    """
    PYthon wrapper class for the HDCDumper class
    """
    cdef CppHDCDumper _this

    def __init__(self):
        self._this = CppHDCDumper()

    #def dump_obj(self, HDC h):
        #self._this.dump_obj(h)

    #def dump_struct(self, hdc_t_ h):
        #self._this.dump_obj(HDC(h))

    #def make_hdc(self):
        #return self._this.make_hdc()

    def make_hdc_t(self):
        cdef hdc_t obj = self._this.make_hdc_t()
        ret = hdc_t_()
        cdef size_t ctypes_buffer = ctypes.addressof(ret)
        memcpy(<void*> ctypes_buffer, &obj, sizeof(hdc_t))
        return ret
