#include "hdc.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "hdc_storage.h"
struct hdc_t {
    void* obj;
};

namespace py = pybind11;

PYBIND11_PLUGIN(libhdc_python) {
    py::module m("libhdc_python", "HDC python plugin");

    m.def("hello__",&hello__,"This function will greet you.");

    py::class_<HDC>(m,"HDC")
    .def(py::init<>())
        .def("set_data_int8", (void (HDC::*)(int8_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_int32", (void (HDC::*)(int32_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(double data)) &HDC::set_data, "Sets data to node")
        .def("set_data_int8", (void (HDC::*)(std::string path, int8_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_int32", (void (HDC::*)(std::string path, int32_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(std::string path, double data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(std::string path, double data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(std::string path, vector<double> data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(vector<double> data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(std::string path, int8_t data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(std::string path, vector<int8_t> data)) &HDC::set_data, "Sets data to node")
        .def("__setitem__", (void (HDC::*)(vector<int8_t> data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(vector<double> data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(double data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(std::string path, vector<double> data)) &HDC::set_data, "Sets data to node")
        .def("add_child", (void (HDC::*)(std::string, HDC* h)) &HDC::add_child, "Adds an empty node to tree")
        .def("get", (HDC* (HDC::*)(std::string)) &HDC::get, "Gets node from tree")
        .def("delete_child", (void (HDC::*)(std::string)) &HDC::delete_child, "Deletes node from tree")
        .def("has_child", (bool (HDC::*)(std::string)) &HDC::has_child, "Returns True if the child with given address exists.")
        .def("as_void_ptr", (void* (HDC::*)()) &HDC::as_void_ptr, "Returns C pointer to self (Pycapsule stuff)")
        .def("as_int8_ptr", (int8_t* (HDC::*)()) &HDC::as<int8_t*>, "as int8")
        .def("as_int32_ptr", (int32_t* (HDC::*)()) &HDC::as<int32_t*>, "as int32")
        .def("as_double_ptr", (double* (HDC::*)()) &HDC::as<double*>, "as double")
        .def("as_double", (double (HDC::*)()) &HDC::as_double, "as double")
        .def("as_double", (double (HDC::*)(std::string)) &HDC::as_double, "as double")
        .def("as_hdc_ptr", (struct hdc_t* (HDC::*)()) &HDC::as_hdc_ptr, "as HDC ptr")
        .def("dump", (void (HDC::*)()) &HDC::dump, "Prints JSON representation of object")
        .def("__repr__", []() {return "<libHDC.HDC>";})
        .def("__getitem__", (HDC* (HDC::*)(std::string path)) &HDC::get, "Returns C pointer to self (Pycapsule stuff)")
        .def("get_type_str", (std::string (HDC::*)()) &HDC::get_type_str, "get_type_str");
    return m.ptr();
}
