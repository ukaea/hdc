#include "hdc.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_PLUGIN(libhdc_python) {
    py::module m("libhdc", "hdc python plugin");

    m.def("hello__",&hello__,"This function will greet you.");

    py::class_<hdc>(m,"hdc")
        .def(py::init<>())
        .def("set_data_int8", (void (hdc::*)(int8_t data)) &hdc::set_data, "Sets data to node")
        .def("set_data_int32", (void (hdc::*)(int32_t data)) &hdc::set_data, "Sets data to node")
        .def("set_data_double", (void (hdc::*)(double data)) &hdc::set_data, "Sets data to node")
        .def("set_data_int8", (void (hdc::*)(std::string path, int8_t data)) &hdc::set_data, "Sets data to node")
        .def("set_data_int32", (void (hdc::*)(std::string path, int32_t data)) &hdc::set_data, "Sets data to node")
        .def("set_data_double", (void (hdc::*)(std::string path, double data)) &hdc::set_data, "Sets data to node")
        .def("add_child", (void (hdc::*)(std::string, hdc* h)) &hdc::add_child, "Adds an empty node to tree")
        .def("get_child", (hdc* (hdc::*)(std::string)) &hdc::get_child, "Gets node from tree")
        .def("delete_child", (void (hdc::*)(std::string)) &hdc::delete_child, "Deletes node from tree")
        .def("has_child", (bool (hdc::*)(std::string)) &hdc::has_child, "Returns True if the child with given address exists.")
        .def("as_void_ptr", (void* (hdc::*)()) &hdc::as_void_ptr, "Returns C pointer to self")
        /*.def("as_int8", (T (hdc::*)()) &hdc::as, "as int8")*/
        .def("__repr__", [](const hdc &t) {return "<libhdc.hdc>";});
    return m.ptr();
}
