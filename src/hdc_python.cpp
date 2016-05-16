#include "hdc.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_PLUGIN(hdc_python) {
    py::module m("libhdc", "hdc python plugin");

    m.def("hello__",&hello__,"This function will greet you.");

    py::class_<hdc>(m,"hdc")
        .def(py::init<>())
        .def("add_child", (void (hdc::*)(std::string, hdc* h)) &hdc::add_child, "Adds an empty node to tree")
        .def("get_child", (hdc* (hdc::*)(std::string)) &hdc::get_child, "Gets node from tree")
        .def("delete_child", (void (hdc::*)(std::string)) &hdc::delete_child, "Deletes node from tree")
        .def("__repr__", [](const hdc &t) {return "<libhdc.hdc>";});
    return m.ptr();
}
