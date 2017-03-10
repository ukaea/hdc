#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
// add uuid generation
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include "types.h"

using namespace std;

/* ------------------------- UUID generation ----------------------------- */

void print_uuid(vector<char> uuid);
vector<char> generate_uuid();
string generate_uuid_str();
/* -------------------------  String manipulation ----------------------------- */
vector<string> split(const string &s, char delim);
void split(const string &s, char delim, vector<string>& elems);
void replace_all(string& str, const string& from, const string& to);

/* -------------------------  Types Definitions  ------------------------- */
size_t hdc_sizeof (TypeID type);
bool hdc_is_primitive_type(TypeID type);
string hdc_get_type_str(TypeID type);

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
TypeID to_typeid(string a);
TypeID to_typeid(char* a);
TypeID to_typeid(bool a);
TypeID to_typeid(string format, size_t itemsize);

/* -------------------------  Other stuff ----------------------------- */

void hello__();


#endif