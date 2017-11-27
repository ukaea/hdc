#include "hdc_c.h"
#include "hdc.hpp"
#include "types.h"
struct hdc_t {
    void* obj;
};

extern "C" {

struct hdc_t* hdc_new_empty() {
    HDC* node = new HDC();
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

struct hdc_t* hdc_new_size(size_t s) {
    HDC* node = new HDC(s);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}


hdc_t* hdc_new_int8(int ndim, size_t* shape)
{
    HDC* node = new HDC(ndim, shape, INT8_ID);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_int32(int ndim, size_t* shape)
{
    HDC* node = new HDC(ndim, shape, INT32_ID);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_int64(int ndim, size_t* shape)
{
    HDC* node = new HDC(ndim, shape, INT64_ID);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_double(int ndim, size_t* shape)
{
    HDC* node = new HDC(ndim, shape,DOUBLE_ID);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

void* hdc_new_void_ptr() {
    return (void*) new HDC();
}

void hdc_delete(struct hdc_t* tree) {
    delete (HDC*)tree->obj;
    return;
}

void hdc_delete_ptr(void* tree) {
    delete (HDC*)((struct hdc_t*)tree)->obj;
    return;
}

void* hdc_get_ptr(struct hdc_t* tree) {
    return (void*) tree;
}

void hdc_add_child(struct hdc_t* tree, char* path, struct hdc_t* n) {
    HDC* t = (HDC*)tree->obj;
    t->add_child(path,(HDC*)n->obj);
    return;
}

void hdc_set_child(struct hdc_t* tree, char* path, struct hdc_t* n) {
    HDC* t = (HDC*)tree->obj;
    t->set_child((string)path, (HDC*)n->obj);
    return;
}
void hdc_delete_child(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    t->delete_child((string)path);
    return;
}

struct hdc_t* hdc_get(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    HDC* node = t->get_ptr(path);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

const char* hdc_get_uuid(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    char* a = new char[t->get_uuid().size()];
    strcpy(a,t->get_uuid().c_str());
    return a;
}


struct hdc_t* hdc_get_slice(struct hdc_t* tree, size_t i) {
    HDC* t = (HDC*)tree->obj;
    HDC* node = t->get_slice(i);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

struct hdc_t* hdc_get_slice_path(struct hdc_t* tree, char* path, size_t i) {
    HDC* t = (HDC*)tree->obj;
    HDC* node = t->get_slice((string)path, i);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n) {
    HDC* t = (HDC*)tree->obj;
    t->set_slice(i, (HDC*)n->obj);
    return;
}


void hdc_insert_slice(struct hdc_t* tree, size_t i, struct hdc_t* n) {
    HDC* t = (HDC*)tree->obj;
    t->insert_slice(i, (HDC*)n->obj);
    return;
}

void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n) {
    HDC* t = (HDC*)tree->obj;
    t->append_slice((HDC*)n->obj);
    return;
}

bool hdc_has_child(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->has_child((string)path);
}

void hdc_set_int8(struct hdc_t* tree, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(ndim, shape, data, INT8_ID);
    return;
}
//void set_data(int _ndim, size_t* _shape, void* _data, TypeID _type, Flags _flags = HDCDefault)



void hdc_set_double(struct hdc_t* tree, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(ndim, shape, data, DOUBLE_ID);
    return;
}

void hdc_set_float(struct hdc_t* tree, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(ndim, shape, data, FLOAT_ID);
    return;
}

void hdc_set_int32(struct hdc_t* tree, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(ndim, shape, data, INT32_ID);
    return;
}

void hdc_set_int32_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data, INT32_ID);
    return;
}

void hdc_set_int64(struct hdc_t* tree, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c(ndim, shape, data, INT64_ID);
    return;
}

void hdc_set_int64_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data, INT64_ID);
    return;
}

void hdc_set_int8_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data, INT8_ID);
    return;
}

int8_t hdc_get_ndim(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->get_ndim();
}

int8_t hdc_get_ndim_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->get_ndim((string)path);
}


size_t* hdc_get_shape(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->get_shape();
}

size_t* hdc_get_shape_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->get_shape((string)path);
}

uint8_t hdc_get_type(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->get_type();
}
void* hdc_as_voidptr(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<void*>();
}

void* hdc_as_voidptr_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<void*>(path);
}

int32_t* hdc_as_int32_1d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t*>();
}

int32_t** hdc_as_int32_2d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t**>();
}

int32_t*** hdc_as_int32_3d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t***>();
}

int32_t**** hdc_as_int32_4d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t****>();
}

int64_t* hdc_as_int64_1d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t*>();
}

int64_t** hdc_as_int64_2d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t**>();
}

int64_t*** hdc_as_int64_3d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t***>();
}

int64_t**** hdc_as_int64_4d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t****>();
}

int8_t* hdc_as_int8_1d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int8_t*>();
}

int8_t** hdc_as_int8_2d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int8_t**>();
}

double* hdc_as_double_1d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<double*>();
}

double** hdc_as_double_2d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<double**>();
}

float* hdc_as_float_1d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<float*>();
}

float** hdc_as_float_2d(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->as<float**>();
}

int32_t* hdc_as_int32_1d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t*>((string)path);
}

int32_t** hdc_as_int32_2d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t**>((string)path);
}

int64_t* hdc_as_int64_1d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t*>((string)path);
}

int64_t** hdc_as_int64_2d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int64_t**>((string)path);
}

double* hdc_as_double_1d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<double*>((string)path);
}

double** hdc_as_double_2d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<double**>((string)path);
}

float* hdc_as_float_1d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<float*>((string)path);
}

float** hdc_as_float_2d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<float**>((string)path);
}

void hdc_set_int8_scalar(hdc_t* tree, int8_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(data);
    return;
}

void hdc_set_int8_scalar_path(hdc_t* tree, char* path, int8_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data((string)path, data);
    return;
}

void hdc_set_int32_scalar(hdc_t* tree, int32_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(data);
    return;
}

void hdc_set_int32_scalar_path(hdc_t* tree, char* path, int32_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data((string)path, data);
    return;
}

void hdc_set_int64_scalar(hdc_t* tree, int64_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(data);
    return;
}

void hdc_set_int64_scalar_path(hdc_t* tree, char* path, int64_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data((string)path, data);
    return;
}

void hello() {cout << "Hello" << endl; return;}

void hdc_set_double_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data, DOUBLE_ID);
    return;
}

void hdc_set_double_sc_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data,DOUBLE_ID);
    return;
}

void hdc_set_double_scalar_path(hdc_t* tree, char* path, double data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data((string)path, data);
    return;
}

void hdc_set_double_scalar(hdc_t* tree, double data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(data);
    return;
}


void hdc_set_float_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data, FLOAT_ID);
    return;
}

void hdc_set_float_sc_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data_c((string)path, ndim, shape, data,FLOAT_ID);
    return;
}

void hdc_set_float_scalar_path(hdc_t* tree, char* path, float data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data((string)path, data);
    return;
}

void hdc_set_float_scalar(hdc_t* tree, float data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(data);
    return;
}

struct hdc_t* hdc_copy(hdc_t* src)
{
    HDC* t_src = (HDC*)(src->obj);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)(t_src->copy(1));
    return h;
}

void hdc_set_string(hdc_t* tree, char* str)
{
    HDC* t = (HDC*)tree->obj;
    t->set_string((std::string)str);
    return;
}

void hdc_set_string_path(hdc_t* tree, char* path, char* str)
{
    HDC* t = (HDC*)tree->obj;
    t->set_string(path,(std::string)str);
    return;
}

double hdc_as_double_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<double*>())[0];
}

float hdc_as_float_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<float*>())[0];
}

const char* hdc_as_string(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    std::string* str = new std::string(t->as_string()); //Unfortunatelly, we need to make copy here...
    return str->c_str();
}

const char* hdc_as_string_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as_string(path).c_str();
}
/*
void hdc_as_string_fortran(hdc_t* tree, char* output, size_t* length) //TODO: can we do zero copy here???
{
    HDC* t = (HDC*)tree->obj;
    std::string str = t->as_string(); //Unfortunatelly, we need to make copy here...
    output = new char[str.length() + 1];
    strcpy(output, str.c_str());
    *length = str.length();
}

void hdc_as_string_path_fortran(hdc_t* tree, char* path, char* output, size_t* length)
{
    HDC* t = (HDC*)tree->obj;
    std::string str = t->as_string(path); //Unfortunatelly, we need to make copy here...
    output = new char[str.length() + 1];
    strcpy(output, str.c_str());
    *length = str.length();
}
*/

double hdc_as_double_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<double*>((string)path))[0];
}

float hdc_as_float_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<float*>((string)path))[0];
}

int8_t hdc_as_int8_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int8_t*>())[0];
}

int8_t hdc_as_int8_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int8_t*>((string)path))[0];
}

int32_t hdc_as_int32_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int32_t*>())[0];
}

int32_t hdc_as_int32_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int32_t*>((string)path))[0];
}

int64_t hdc_as_int64_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int64_t*>())[0];
}

int64_t hdc_as_int64_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<int64_t*>((string)path))[0];
}


void test_str(char* str) {
    cout << "test_str(): " << (string)str << endl;
    return;
}

const char* hdc_get_type_str(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->get_type_str().c_str());
}

void hdc_print_type_str(hdc_t* tree)
{
    printf("dtype: %s\n",hdc_get_type_str(tree));
    return;
}

const char* hdc_get_type_str_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->get_type_str((string)path).c_str());
}


const char* hdc_get_datashape_str(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->get_datashape_str().c_str());
}

void hdc_to_json(hdc_t* tree, char* filename, int mode) {
    HDC* t = (HDC*)tree->obj;
    t->to_json((string)filename, mode);
    return;
}

void hdc_dump(hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    t->dump();
    return;
}

char** hdc_keys(hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    vector<string> keys = t->keys();
    char** arr;
    size_t size = keys.size();
    arr = (char**)malloc(sizeof(char)*size);
    for (size_t i=0;i<size;i++) {
        size_t len = strlen(keys[i].c_str())+1;
        arr[i] = (char*)malloc(sizeof(char)*len);
        strcpy(arr[i],keys[i].c_str());
    }
    return arr;
}

void hdc_keys_py(hdc_t* tree, char** arr) {
    HDC* t = (HDC*)tree->obj;
    vector<string> keys = t->keys();
    size_t size = keys.size();
    for (size_t i=0;i<size;i++) {
        strcpy(arr[i],keys[i].c_str());
    }
}

hdc_t* hdc_new_dtype(int ndim, size_t* shape, TypeID type)
{
    HDC* t = new HDC(ndim,shape,type);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)t;
    return h;
}

const char* hdc_dumps(hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    stringstream tmp;
    tmp << t->to_json(0);
    string dump_str = tmp.str();
    return strdup(dump_str.c_str());
}

size_t hdc_childs_count(hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->childs_count();
}

void HDC_init_c(char* pluginFileName, char* pluginSettingsString) {
    if (strlen(pluginFileName) != 0) {
        if (strlen(pluginSettingsString)== 0)
            HDC_init(std::string(pluginFileName));
        else
            HDC_init(std::string(pluginFileName),std::string(pluginSettingsString));
    }
    else
        HDC_init();
}

void HDC_destroy_c() {
    if (global_storage != nullptr) HDC_destroy();
}

char** HDC_get_available_plugins_c() {
    std::vector<std::string> cppkeys = HDC_get_available_plugins();
    const char** keys = new const char* [cppkeys.size()+1];
    for (int i=0;i<cppkeys.size();i++) {
        keys[i] = cppkeys[i].c_str();
    }
    keys[cppkeys.size()] = NULL;
};

const char* hdc_serialize(hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    stringstream tmp;
    tmp << t->serialize();
    string dump_str = tmp.str();
    return strdup(dump_str.c_str());

}

struct hdc_t* hdc_deserialize(const char* str) {
    HDC* node = deserialize_HDC_string((string) str);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}


// end extern C
}
