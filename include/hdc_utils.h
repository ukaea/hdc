#ifndef HDC_UTILS_H
#define HDC_UTILS_H
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tuple>
// add uuid generation
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include "hdc_types.h"
#include "hdc_errors.hpp"

/* ------------------------- UUID generation ----------------------------- */

void print_uuid(std::vector<char> uuid);
std::vector<char> generate_uuid();
std::string generate_uuid_str();
/* -------------------------  String manipulation ----------------------------- */
std::vector <boost::variant<size_t,std::string>> split(const std::string& s);
/* -------------------------  Types Definitions  ------------------------- */
size_t hdc_sizeof (TypeID type);
bool hdc_is_primitive_type(TypeID type);
std::string hdc_type_str(TypeID type);

template <typename T>
TypeID to_typeid(T a);
TypeID to_typeid(double a);
TypeID to_typeid(float a);
TypeID to_typeid(int64_t a);
TypeID to_typeid(int32_t a);
TypeID to_typeid(int16_t a);
TypeID to_typeid(int8_t a);
TypeID to_typeid(uint64_t a);
TypeID to_typeid(uint32_t a);
TypeID to_typeid(uint16_t a);
TypeID to_typeid(uint8_t a);
TypeID to_typeid(char a);
TypeID to_typeid(std::string a);
TypeID to_typeid(char* a);
TypeID to_typeid(bool a);
TypeID numpy_format_to_typeid(std::string format, size_t itemsize);
TypeID to_typeid(const std::type_info& t);
TypeID uda_str_to_typeid(std::string& str);
/* -------------------------  Other stuff ----------------------------- */

void hello__();


#endif
