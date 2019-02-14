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
#include <andres/marray.hxx>
#include <sys/stat.h>

using hdc_index_t = boost::variant<size_t,std::string>;
using hdc_path_t = std::list<hdc_index_t>;

/* ------------------------- UUID generation ----------------------------- */

void print_uuid(std::vector<char> uuid);
std::vector<char> generate_uuid();
std::string generate_uuid_str();
/* -------------------------  String manipulation ----------------------------- */
hdc_path_t split(const std::string& s);
hdc_path_t split_no_brackets(const std::string& s);
/* -------------------------  Types Definitions  ------------------------- */
size_t hdc_sizeof (hdc_type_t type);
bool hdc_is_primitive_type(hdc_type_t type);
std::string hdc_type_str(hdc_type_t type);

template <typename T>
hdc_type_t to_typeid(T a);
hdc_type_t to_typeid(double a);
hdc_type_t to_typeid(float a);
hdc_type_t to_typeid(int64_t a);
hdc_type_t to_typeid(int32_t a);
hdc_type_t to_typeid(int16_t a);
hdc_type_t to_typeid(int8_t a);
hdc_type_t to_typeid(uint64_t a);
hdc_type_t to_typeid(uint32_t a);
hdc_type_t to_typeid(uint16_t a);
hdc_type_t to_typeid(uint8_t a);
hdc_type_t to_typeid(char a);
hdc_type_t to_typeid(std::string a);
hdc_type_t to_typeid(char* a);
hdc_type_t to_typeid(bool a);
hdc_type_t numpy_format_to_typeid(std::string format, size_t itemsize);
hdc_type_t to_typeid(const std::type_info& t);
hdc_type_t uda_str_to_typeid(std::string& str);
bool hdc_is_numeric (hdc_type_t);
/* -------------------------  Other stuff ----------------------------- */

void hello__();

/* -------------------------  Buffer Manipulation  ------------------------- */

void transpose_buffer(char* new_buffer, char* buffer, int8_t rank, std::vector<size_t> shape, hdc_type_t type_, bool fortranOrder = false);
char* transpose_buffer(char* buffer);

bool fileExists(const std::string& file);

#endif
