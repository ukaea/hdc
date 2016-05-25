#include "hdc.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
struct hdc_t {
    void* obj;
};

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
        .def("__setitem__", (void (hdc::*)(std::string path, double data)) &hdc::set_data, "Sets data to node")
        .def("__setitem__", (void (hdc::*)(std::string path, vector<double> data)) &hdc::set_data, "Sets data to node")
        .def("__setitem__", (void (hdc::*)(vector<double> data)) &hdc::set_data, "Sets data to node")
        .def("__setitem__", (void (hdc::*)(std::string path, int8_t data)) &hdc::set_data, "Sets data to node")
        .def("__setitem__", (void (hdc::*)(std::string path, vector<int8_t> data)) &hdc::set_data, "Sets data to node")
        .def("__setitem__", (void (hdc::*)(vector<int8_t> data)) &hdc::set_data, "Sets data to node")
        .def("set_data_double", (void (hdc::*)(vector<double> data)) &hdc::set_data, "Sets data to node")
        .def("set_data_double", (void (hdc::*)(std::string path, vector<double> data)) &hdc::set_data, "Sets data to node")
        .def("add_child", (void (hdc::*)(std::string, hdc* h)) &hdc::add_child, "Adds an empty node to tree")
        .def("get_child", (hdc* (hdc::*)(std::string)) &hdc::get_child, "Gets node from tree")
        .def("delete_child", (void (hdc::*)(std::string)) &hdc::delete_child, "Deletes node from tree")
        .def("has_child", (bool (hdc::*)(std::string)) &hdc::has_child, "Returns True if the child with given address exists.")
        .def("as_void_ptr", (void* (hdc::*)()) &hdc::as_void_ptr, "Returns C pointer to self (Pycapsule stuff)")
        .def("as_int8_ptr", (int8_t* (hdc::*)()) &hdc::as<int8_t*>, "as int8")
        .def("as_int32_ptr", (int32_t* (hdc::*)()) &hdc::as<int32_t*>, "as int32")
        .def("as_double_ptr", (double* (hdc::*)()) &hdc::as<double*>, "as double")
        .def("as_double", (double (hdc::*)()) &hdc::as_double, "as double")
        .def("as_double", (double (hdc::*)(std::string)) &hdc::as_double, "as double")
        .def("as_hdc_ptr", (struct hdc_t* (hdc::*)()) &hdc::as_hdc_ptr, "as HDC ptr")
        .def("__repr__", []() {return "<libhdc.hdc>";})
        .def("__getitem__", (hdc* (hdc::*)(std::string path)) &hdc::get_child, "Returns C pointer to self (Pycapsule stuff)")
        .def("get_type_str", (std::string (hdc::*)()) &hdc::get_type_str, "get_type_str");
    return m.ptr();
}
