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

hdc_t* hdc_new_int8(int8_t ndim, long int* shape)
{
    HDC* node = hdc_empty_array<int8_t>(ndim, shape);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_int32(int8_t ndim, long int* shape)
{
    HDC* node = hdc_empty_array<int32_t>(ndim, shape);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

hdc_t* hdc_new_double(int8_t ndim, long int* shape)
{
    HDC* node = hdc_empty_array<double>(ndim, shape);
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
    t->add_child((string)path,(HDC*)n->obj);
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
    HDC* node = t->get((string)path);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
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

void hdc_set_int8(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<int8_t>(ndim, shape, data);
    return;
}

void hdc_set_double(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<double>(ndim, shape, data);
    return;
}

void hdc_set_int32(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<int32_t>(ndim, shape, data);
    return;
}

void hdc_set_int32_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<int32_t>((string)path, ndim, shape, data);
    return;
}

void hdc_set_int8_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<int8_t>((string)path, ndim, shape, data);
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


long int* hdc_get_shape(struct hdc_t* tree) {
    HDC* t = (HDC*)tree->obj;
    return t->get_shape();
}

long int* hdc_get_shape_path(struct hdc_t* tree, char* path) {
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

int32_t* hdc_as_int32_1d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t*>((string)path);
}

int32_t** hdc_as_int32_2d_path(struct hdc_t* tree, char* path) {
    HDC* t = (HDC*)tree->obj;
    return t->as<int32_t**>((string)path);
}


void hdc_set_int8_scalar(hdc_t* tree, int8_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<int8_t>(data);
    return;
}

void hdc_set_int8_scalar_path(hdc_t* tree, char* path, int8_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<int8_t>((string)path, data);
    return;
}

void hdc_set_int32_scalar(hdc_t* tree, int32_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<int32_t>(data);
    return;
}

void hdc_set_int32_scalar_path(hdc_t* tree, char* path, int32_t data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<int32_t>((string)path, data);
    return;
}


void hello() {cout << "Hello" << endl; return;}

void hdc_set_double_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<double>((string)path, ndim, shape, data);
    return;
}

void hdc_set_double_sc_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    HDC* t = (HDC*)tree->obj;
    t->set_data<double>((string)path, ndim, shape, data);
    return;
}

void hdc_set_double_scalar_path(hdc_t* tree, char* path, double data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<double>((string)path, data);
    return;
}

void hdc_set_double_scalar(hdc_t* tree, double data)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data<double>(data);
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
    t->set_data((string)str);
    return;
}

void hdc_set_string_path(hdc_t* tree, char* path, char* str)
{
    HDC* t = (HDC*)tree->obj;
    t->set_data(path,(string)str);
    return;
}

double hdc_as_double_scalar(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<double*>())[0];
}

const char* hdc_as_string(hdc_t* tree)
{
    HDC* t = (HDC*)tree->obj;
    return t->as_string().c_str();
}

const char* hdc_as_string_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return t->as_string(path).c_str();
}


double hdc_as_double_scalar_path(hdc_t* tree, char* path)
{
    HDC* t = (HDC*)tree->obj;
    return (t->as<double*>((string)path))[0];
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

// end exter C
}