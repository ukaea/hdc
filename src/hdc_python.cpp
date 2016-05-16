#include "hdc.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_PLUGIN(libhdc) {
    py::module m("libhdc", "pybind11 example plugin");

    m.def("hello",&hello,"This function will greet you.");

    // Container class
    py::class_<Container>(m,"Container")
        .def("__init__", [](Container &c, pybind11::buffer buffer_d, size_t size, signed char t) {
            pybind11::buffer_info info_d = buffer_d.request();
            /*if (info_d.format != py::format_descriptor< int64_t >::value() ||
            info_d.ndim != 1)
                throw std::runtime_error("Incompatible data buffer format!");*/
            new (&c) Container((char*)info_d.ptr,size,11);
        })
        .def("__getitem__",[](Container &c, size_t i) {
            if (i > c.shape()[0])
                throw pybind11::index_error();
            return c(i);
        })
        .def("__setitem__",[](Container &c, size_t i, int64_t v) {
            if (i > c.shape()[0])
                throw pybind11::index_error();
            c(i) = v;
        })
        .def("__repr__", (std::string (Container::*)(void)) &Container::stringify, "Prints out the container")
        .def_buffer([](Container &c) -> py::buffer_info {
            return py::buffer_info(
                c.obj(),
                sizeof(int64_t),
                py::format_descriptor<int64_t>::value(),
                1,
                {c.shape()[0]},
                {sizeof(int64_t) * c.shape()[0]}
            );
        });

    py::class_<HDC>(m,"HDC")
        .def(py::init<>())
        .def("addNode", (void (HDC::*)(std::string)) &HDC::addNode, "Adds an empty node to tree")
        .def("addNode", (void (HDC::*)(std::string, Container*)) &HDC::addNode, "Adds node with some data to tree")
        .def("getNode", (Container* (HDC::*)(std::string)) &HDC::getNodeClass, "Gets node from tree")
        .def("__repr__", [](const HDC &t) {return "<libhdc.HDC>";});
    return m.ptr();
}