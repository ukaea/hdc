#include "hdc_c.h"
#include "hdc.hpp"
#include "hdc_types.h"

extern "C" {
hdc_t hdc_new_empty()
{
    return HDC().as_obj();
}

hdc_t hdc_new_size(size_t size)
{
    return HDC(size).as_obj();
}

hdc_t hdc_new_string(const char* str)
{
    return HDC(str).as_obj();
}

hdc_t hdc_new_array(size_t rank, size_t* shape_in, hdc_type_t type)
{
    std::vector<size_t> shape(&shape_in[0],&shape_in[0]+rank);
    return HDC(rank,shape,type).as_obj();
}

hdc_t hdc_new_array2(size_t rank, size_t* shape_in, hdc_type_t type, size_t flags)
{
    std::vector<size_t> shape(&shape_in[0],&shape_in[0]+rank);
    return HDC(rank,shape,type,flags).as_obj();
}

void hdc_delete(hdc_t tree UNUSED)
{
    std::cerr << "hdc_delete(): I do nothing TODO: remove me...\n";
    return;
}

void hdc_print_info(hdc_t tree UNUSED)
{
    HDC(tree).print_info();
    return;
}

void hdc_delete_ptr(void* tree UNUSED)
{
    std::cerr << "hdc_delete_ptr(): I do nothing TODO: remove me...\n";
    return;
}

void hdc_add_child(hdc_t tree, const char* path, hdc_t n)
{
    HDC node(n);
    HDC(tree).add_child(path, node);
    return;
}

void hdc_set_child(hdc_t tree, const char* path, hdc_t n)
{
    HDC node(n);
    HDC(tree).set_child(path, node);
    return;
}

void hdc_delete_child(hdc_t tree, const char* path)
{
    HDC(tree).delete_child(path);
    return;
}

hdc_t hdc_get(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as_obj();
}

hdc_t hdc_get_slice(hdc_t tree, const char* path, size_t i)
{
    return HDC(tree).get(path).get(i).as_obj();
}

void hdc_set_slice(hdc_t tree, size_t i, hdc_t n)
{
    HDC node(n);
    HDC(tree).set_child(i,node);
}

void hdc_insert_slice(hdc_t tree, size_t i, hdc_t n)
{
    HDC node(n);
    HDC(tree).insert(i,node);
}

void hdc_append_slice(hdc_t tree, hdc_t n)
{
    HDC node(n);
    HDC(tree).append(node);
}

bool hdc_exists(hdc_t tree, const char* path)
{
    return HDC(tree).exists(path);
}

void hdc_set_int8(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data, hdc_flags_t _flags)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_INT8, _flags);
    return;
}

void hdc_set_int16(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data, hdc_flags_t _flags)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_INT16, _flags);
    return;
}

void hdc_set_int32(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data, hdc_flags_t _flags)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_INT32, _flags);
    return;
}

void hdc_set_int64(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data, hdc_flags_t _flags)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_INT64, _flags);
    return;
}

size_t hdc_get_rank(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).get_rank();
}

size_t hdc_get_type(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).get_type();
}

void* hdc_as_voidptr(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<void*>();
}

int8_t* hdc_as_int8_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<int8_t*>();
}

int16_t* hdc_as_int16_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<int16_t*>();
}

int32_t* hdc_as_int32_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<int32_t*>();
}

int64_t* hdc_as_int64_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<int64_t*>();
}


double* hdc_as_double_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<double*>();
}


float* hdc_as_float_array(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).as<float*>();
}

void hdc_set_int8_scalar(hdc_t tree, const char* path, int8_t data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

void hdc_set_int16_scalar(hdc_t tree, const char* path, int16_t data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

void hdc_set_int32_scalar(hdc_t tree, const char* path, int32_t data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

void hdc_set_int64_scalar(hdc_t tree, const char* path, int64_t data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

void hello()
{
    cout << "Hello" << endl;
    return;
}

void hdc_set_double(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data, hdc_flags_t _flags)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_DOUBLE, _flags);
    return;
}

void hdc_set_double_scalar(hdc_t tree, const char* path, double data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

void hdc_set_float(hdc_t tree, const char* path, int rank, size_t* shape_in, void* data,
                        hdc_flags_t _flags UNUSED)
{
    std::vector<size_t> shape(shape_in, shape_in+rank);
    HDC(tree).get_or_create(path).set_data_c(rank, shape, data, HDC_FLOAT);
    return;
}

void hdc_set_float_scalar(hdc_t tree, const char* path, float data)
{
    HDC(tree).get_or_create(path).set_data(data);
    return;
}

hdc_t hdc_copy(hdc_t src)
{
    return HDC(src).copy().as_obj();
}

void hdc_set_string(hdc_t tree, const char* path, const char* str)
{
    HDC(tree).get_or_create(path).set_string(str);
    return;
}

const char* hdc_as_string(hdc_t tree, const char* path)
{
    auto data = HDC(tree).get(path).get_data();
    return reinterpret_cast<const char *>(data.data);
}

double hdc_as_double_scalar(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).as<double*>())[0];
}

float hdc_as_float_scalar(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).as<float*>())[0];
}

int8_t hdc_as_int8_scalar(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).as<int8_t*>())[0];
}

int16_t hdc_as_int16_scalar(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).as<int16_t*>())[0];
}

int32_t hdc_as_int32_scalar(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).as<int32_t*>())[0];
}

const char* hdc_get_type_str(hdc_t tree, const char* path)
{
    return (HDC(tree).get(path).get_type_str());
}

void hdc_to_json(hdc_t tree, const char* filename, int mode)
{
    HDC(tree).to_json(filename, mode);
}

void hdc_dump(hdc_t tree)
{
    HDC(tree).dump();
}

void hdc_keys(hdc_t tree, char** keys, size_t* nkeys)
{
    HDC t = HDC(tree);
    if (t.get_type() != HDC_STRUCT) {
        *nkeys = 0;
        return;
    }
    vector<string> cppkeys = HDC(tree).keys();
    *nkeys = cppkeys.size();
    for (size_t i = 0; i < *nkeys; i++) {
        strcpy(keys[i], cppkeys[i].c_str());
    }
}

const char* hdc_dumps(hdc_t tree)
{
    stringstream tmp;
    tmp << HDC(tree).to_json(0);
    string dump_str = tmp.str();
    return strdup(dump_str.c_str());
}

size_t hdc_childs_count(hdc_t tree)
{
    return HDC(tree).childs_count();
}

void hdc_init(const char* pluginFileName, const char* pluginSettingsFileName)
{
    if (strlen(pluginFileName) != 0) {
        if (strlen(pluginSettingsFileName) == 0) {
            HDC::init(std::string(pluginFileName));
        } else {
            HDC::init(std::string(pluginFileName), std::string(pluginSettingsFileName));
        }
    } else {
        HDC::init();
    }
}

void hdc_destroy()
{
    if (global_storage != nullptr) HDC::destroy();
}

void hdc_get_available_plugins(char** keys, int* num)
{
    std::vector<std::string> cppkeys = HDC::get_available_plugins();
    for (size_t i = 0; i < cppkeys.size(); i++) {
        strcpy(keys[i],cppkeys[i].c_str());
    }
    *num = cppkeys.size();
}

void hdc_serialize(hdc_t tree, char* buffer)
{
    stringstream tmp;
    tmp << HDC(tree).serialize();
    strcpy(buffer,tmp.str().c_str());
}

hdc_t hdc_deserialize(const char* str)
{
    return HDC::deserialize_HDC_string(str).as_obj();
}

hdc_data_t hdc_get_data(hdc_t tree, const char* path)
{
    return HDC(tree).get(path).get_data();
}

void hdc_set_data(hdc_t tree, const char* path, hdc_data_t data)
{
    HDC(tree).get_or_create(path).set_data(data);
}

size_t hdc_get_size(hdc_t tree)
{
    return HDC(tree).get_size();
}

void hdc_set_scalar(hdc_t tree, const char* path, void* data, hdc_type_t _type)
{
    return HDC(tree).get_or_create(path).set_data((void*)data,_type);
}

// end extern C
}
