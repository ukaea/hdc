#include "hdc.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "hdc_storage.h"
#include "types.h"
struct hdc_t {
    void* obj;
};

namespace py = pybind11;

PYBIND11_PLUGIN(libhdc_python) {
    py::module m("libhdc_python", "HDC python plugin");

    m.def("hello__",&hello__,"This function will greet you.");

    m.def("from_cpp_ptr", &new_HDC_from_cpp_ptr, "New HDC from CPP pointer");
    m.def("from_c_ptr", &new_HDC_from_c_ptr, "New HDC from hdc_t struct pointer");

    py::enum_<TypeID>(m, "TypeID")
        .value("EMPTY_ID", EMPTY_ID)
        .value("STRUCT_ID", STRUCT_ID)
        .value("LIST_ID", LIST_ID)     
        .value("INT8_ID", INT8_ID)     
        .value("INT16_ID", INT16_ID)    
        .value("INT32_ID", INT32_ID)    
        .value("INT64_ID", INT64_ID)    
        .value("UINT8_ID", UINT8_ID)    
        .value("UINT16_ID", UINT16_ID)   
        .value("UINT32_ID", UINT32_ID)   
        .value("UINT64_ID", UINT64_ID)   
        .value("FLOAT_ID", FLOAT_ID)    
        .value("DOUBLE_ID", DOUBLE_ID)   
        .value("STRING_ID", STRING_ID)   
        .value("BOOL_ID", BOOL_ID)     
        .value("ERROR_ID", ERROR_ID)    
        .value("Internal_ForceMyEnumIntSize", Internal_ForceMyEnumIntSize)
        ;


    py::class_<HDC>(m, "HDC", py::buffer_protocol())
        .def(py::init<>())
        .def("set_data_int8", (void (HDC::*)(int8_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_int32", (void (HDC::*)(int32_t data)) &HDC::set_data, "Sets data to node")
        .def("set_data_double", (void (HDC::*)(double data)) &HDC::set_data, "Sets data to node")
        .def("set_string", (void (HDC::*)(std::string str)) &HDC::set_string, "Sets data to node")
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
        .def("set_data_c", (void (HDC::*)(int _ndim, size_t* _shape, void* _data, TypeID _type)) &HDC::set_data_c, "Sets data to node")
        .def("add_child", (void (HDC::*)(std::string, HDC* h)) &HDC::add_child, "Adds an empty node to tree")
        .def("get", (HDC* (HDC::*)(std::string)) &HDC::get, "Gets node from tree")
        .def("delete_child", (void (HDC::*)(std::string)) &HDC::delete_child, "Deletes node from tree")
        .def("has_child", (bool (HDC::*)(std::string)) &HDC::has_child, "Returns True if the child with given address exists.")
        .def("as_int8_ptr", (int8_t* (HDC::*)()) &HDC::as<int8_t*>, "as int8")
        .def("as_int32_ptr", (int32_t* (HDC::*)()) &HDC::as<int32_t*>, "as int32")
        .def("as_double_ptr", (double* (HDC::*)()) &HDC::as<double*>, "as double")
        .def("as_double", (double (HDC::*)()) &HDC::as_double, "as double")
        .def("as_double", (double (HDC::*)(std::string)) &HDC::as_double, "as double")
        .def("as_cpp_ptr", (intptr_t (HDC::*)()) &HDC::as_void_ptr, "Returns pointer to the CPP object")
        .def("as_c_ptr", (intptr_t (HDC::*)()) &HDC::as_hdc_ptr, "Return pointer to the C wrapper hdc_t struct")
        .def("get_type_str", (string (HDC::*)()) &HDC::get_type_str, "Return type string")
        .def("to_json_string", (std::string (HDC::*)(int mode)) &HDC::to_json_string, "Prints JSON representation of object",
                                py::arg("mode") = 0)
        .def("__repr__", []() {return "<libHDC.HDC>";})
        .def("__getitem__", (HDC* (HDC::*)(std::string path)) &HDC::get, "Returns C pointer to self (Pycapsule stuff)")
        .def("get_type_str", (std::string (HDC::*)()) &HDC::get_type_str, "get_type_str")
        // .def("set_numpy", (std::string (HDC::*)(py::buffer b)) {
        .def("set_numpy", [](HDC &self, py::buffer b) {

            /* Request a buffer descriptor from Python */
            py::buffer_info info = b.request();
            std::cout << "ndim: " << info.ndim << std::endl;
            std::cout << info.format << std::to_string(info.itemsize) << std::endl;
            // std::cout << "shape: " << info.shape << std::endl;

            TypeID typ = to_typeid(info.format, info.itemsize);
            if (typ == ERROR_ID)
            {
                std::cout << "error id" << std::endl;
            } else {
                self.set_data_c(info.ndim, &info.shape[0], info.ptr, typ);
            }

            // /* Some sanity checks ... */
            // if (info.format != py::format_descriptor<Scalar>::format())
            //     throw std::runtime_error("Incompatible format: expected a double array!");

            // if (info.ndim != 2)
            //     throw std::runtime_error("Incompatible buffer dimension!");

            // auto strides = Strides(
            //     info.strides[rowMajor ? 0 : 1] / sizeof(Scalar),
            //     info.strides[rowMajor ? 1 : 0] / sizeof(Scalar));

            // auto map = Eigen::Map<Matrix, 0, Strides>(
            //     static_cat<Scalar *>(info.ptr), info.shape[0], info.shape[1], strides);

            // new (&m) Matrix(map);
            // return info.format + std::to_string(info.itemsize);
            return typ;
            });

    return m.ptr();

}
