#include "hdc_c.h"
#include "hdc.hpp"
#include "hdc_types.h"

struct hdc_t {
    void* obj;
};

extern "C" {

hdc_t* hdc_new_empty()
{
    HDC* node = new HDC();
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_string(const char* str)
{
    HDC* node = new HDC(str);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_array(size_t rank, size_t* shape, hdc_type_t type)
{
    HDC* node = new HDC(rank,shape,type);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_array2(size_t rank, size_t* shape, hdc_type_t type, size_t flags)
{
    HDC* node = new HDC(rank,shape,type, flags);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

void* hdc_new_void_ptr()
{
    return (void*)new HDC();
}

void hdc_delete(struct hdc_t* tree)
{
    delete (HDC*)tree->obj;
    return;
}

void hdc_print_info(struct hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    t->print_info();
    return;
}

void hdc_delete_ptr(void* tree)
{
    delete (HDC*)((struct hdc_t*)tree)->obj;
    return;
}

void* hdc_get_ptr(struct hdc_t* tree)
{
    return (void*)tree;
}

void hdc_add_child(struct hdc_t* tree, const char* path, struct hdc_t* n)
{
    HDC* t = (HDC*)tree->obj;
    t->add_child(path, (HDC*)n->obj);
    return;
}

void hdc_set_child(struct hdc_t* tree, const char* path, struct hdc_t* n)
{
    HDC* t = (HDC*)tree->obj;
    t->set_child(path, (HDC*)n->obj);
    return;
}

void hdc_delete_child(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    t->delete_child(path);
    return;
}

hdc_t* hdc_get(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    HDC* node = t->get_ptr(path);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

const char* hdc_get_uuid(struct hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    char* a = new char[t->get_uuid().size()];
    strcpy(a, t->get_uuid().c_str());
    return a;
}

hdc_t* hdc_get_slice(struct hdc_t* tree, const char* path, size_t i)
{
    HDC* t = (HDC*)tree->obj;
    HDC* node = t->get_slice_ptr(path, i);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n)
{
    HDC* t = (HDC*)tree->obj;
    t->set_slice(i, (HDC*)n->obj);
    return;
}

void hdc_insert_slice(struct hdc_t* tree, size_t i, struct hdc_t* n)
{
    HDC* t = (HDC*)tree->obj;
    t->insert_slice(i, (HDC*)n->obj);
    return;
}

void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n)
{
    HDC* t = (HDC*)tree->obj;
    t->append_slice((HDC*)n->obj);
    return;
}

bool hdc_exists(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->exists(path);
}

void hdc_set_int8(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_INT8, _flags);
    return;
}

void hdc_set_int16(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_INT16, _flags);
    return;
}

void hdc_set_int32(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_INT32, _flags);
    return;
}

void hdc_set_int64(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_INT64, _flags);
    return;
}

size_t hdc_get_rank(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->get(path).get_rank();
}

size_t* hdc_get_shape(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->get(path).get_shape();
}

size_t hdc_get_type(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->get(path).get_type();
}

void* hdc_as_voidptr(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<void*>(path);
}

int8_t* hdc_as_int8_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<int8_t*>(path);
}

int16_t* hdc_as_int16_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<int16_t*>(path);
}

int32_t* hdc_as_int32_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t*>(path);
}

int64_t* hdc_as_int64_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t*>(path);
}


double* hdc_as_double_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<double*>(path);
}


float* hdc_as_float_array(struct hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as<float*>(path);
}

void hdc_set_int8_scalar(hdc_t* tree, const char* path, int8_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path, data);
    return;
}

void hdc_set_int32_scalar(hdc_t* tree, const char* path, int32_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path, data);
    return;
}

void hdc_set_int64_scalar_path(hdc_t* tree, const char* path, int64_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path, data);
    return;
}

void hello()
{
    cout << "Hello" << endl;
    return;
}

void hdc_set_double(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_DOUBLE, _flags);
    return;
}

void hdc_set_double_scalar(hdc_t* tree, const char* path, double data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path, data);
    return;
}


void hdc_set_float(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data,
                        hdc_flags_t _flags UNUSED)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(path, rank, shape, data, HDC_FLOAT);
    return;
}

void hdc_set_float_scalar(hdc_t* tree, const char* path, float data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path, data);
    return;
}

struct hdc_t* hdc_copy(hdc_t* src)
{
    HDC* t_src = (HDC*)(src->obj);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)(t_src->copy(1));
    return h;
}

void hdc_set_string(hdc_t* tree, const char* path, const char* str)
{
    HDC* t = (HDC*)tree->obj;
    t->set_string(path, str);
    return;
}

const char* hdc_as_string(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    auto data = t->get_data(path);
    return reinterpret_cast<const char *>(data.data);
}

double hdc_as_double_scalar(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<double*>(path))[0];
}

float hdc_as_float_scalar(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<float*>(path))[0];
}

int8_t hdc_as_int8_scalar(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int8_t*>(path))[0];
}

int32_t hdc_as_int32_scalar_path(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int32_t*>(path))[0];
}

const char* hdc_get_type_str(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->get_type_str(path).c_str());
}


const char* hdc_get_datashape_str(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->get_datashape_str().c_str());
}

void hdc_to_json(hdc_t* tree, const char* filename, int mode)
{
    HDC* t = (HDC*)tree->obj;
    t->to_json(filename, mode);
}

void hdc_dump(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    t->dump();
}

char** hdc_keys(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    vector<string> keys = t->keys();
    char** arr;
    size_t size = keys.size();
    arr = (char**)malloc(sizeof(char) * size);
    for (size_t i = 0; i < size; i++) {
        size_t len = strlen(keys[i].c_str()) + 1;
        arr[i] = (char*)malloc(sizeof(char) * len);
        strcpy(arr[i], keys[i].c_str());
    }
    return arr;
}

void hdc_keys_py(hdc_t* tree, char** arr)
{
    HDC* t = (HDC*)tree->obj;
    vector<string> keys = t->keys();
    size_t size = keys.size();
    for (size_t i = 0; i < size; i++) {
        strcpy(arr[i], keys[i].c_str());
    }
}

const char* hdc_dumps(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    stringstream tmp;
    tmp << t->to_json(0);
    string dump_str = tmp.str();
    return strdup(dump_str.c_str());
}

size_t hdc_childs_count(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return t->childs_count();
}

void hdc_init(char* pluginFileName, char* pluginSettingsString)
{
    if (strlen(pluginFileName) != 0) {
        if (strlen(pluginSettingsString) == 0) {
            HDC::init(std::string(pluginFileName));
        } else {
            HDC::init(std::string(pluginFileName), std::string(pluginSettingsString));
        }
    } else {
        HDC::init();
    }
}

void HDC_destroy_c()
{
    if (global_storage != nullptr) HDC::destroy();
}

char** HDC_get_available_plugins_c()
{
    std::vector<std::string> cppkeys = HDC::get_available_plugins();
    const char** keys = new const char* [cppkeys.size() + 1];
    for (size_t i = 0; i < cppkeys.size(); i++) {
        keys[i] = cppkeys[i].c_str();
    }
    keys[cppkeys.size()] = NULL;
    return NULL;
}

const char* hdc_serialize(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    stringstream tmp;
    tmp << t->serialize();
    string dump_str = tmp.str();
    return strdup(dump_str.c_str());

}

struct hdc_t* hdc_deserialize(const char* str)
{
    HDC* node = HDC::deserialize_HDC_string(str);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_data_t hdc_get_data(hdc_t* tree, const char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->get_data(path);
}

void hdc_set_data(hdc_t* tree, const char* path, hdc_data_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path,data);
}

size_t hdc_get_size(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return t->get_size();
}

void hdc_set_scalar(struct hdc_t* tree, const char* path, const unsigned char* data, hdc_type_t _type)
{
    HDC* t = (HDC*)tree->obj;
    return t->set_data(path,(void*)data,_type);
}

// end extern C
}
