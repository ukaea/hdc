#include "hdc_c.h"
#include "hdc.hpp"
#include "types.h"
struct hdc_t {
    void* obj;
};

extern "C" {

struct hdc_t* hdc_new_empty() {
    hdc* node = new hdc();
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

void hdc_delete(struct hdc_t* tree) {
    delete (hdc*)tree->obj;
    return;
}

void hdc_add_child(struct hdc_t* tree, char* path, struct hdc_t* n) {
    hdc* t = (hdc*)tree->obj;
    t->add_child(path,(hdc*)n->obj);
    return;
}

void hdc_set_child(struct hdc_t* tree, char* path, struct hdc_t* n) {
    hdc* t = (hdc*)tree->obj;
    t->set_child(path, (hdc*)n->obj);
    return;
}
void hdc_delete_child(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    t->delete_child(path);
    return;
}

struct hdc_t* hdc_get_child(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    hdc* node = t->get_child(path);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

struct hdc_t* hdc_get_slice(struct hdc_t* tree, size_t i) {
    hdc* t = (hdc*)tree->obj;
    hdc* node = t->get_slice(i);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}


struct hdc_t* hdc_get_slice_path(struct hdc_t* tree, char* path, size_t i) {
    hdc* t = (hdc*)tree->obj;
    hdc* node = t->get_slice(path, i);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)node;
    return h;
}

bool hdc_has_child(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->has_child(path);
}

void hdc_set_data_int8(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
//     cout << (int)((int8_t*)data)[0] << (int)((int8_t*)data)[1] << (int)((int8_t*)data)[2] << (int)((int8_t*)data)[3] << endl;
    t->set_data<int8_t>(ndim, shape, data);
    return;
}

void hdc_set_data_double(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    t->set_data<double>(ndim, shape, data);
    return;
}

void hdc_set_data_int32(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    t->set_data<int32_t>(ndim, shape, data);
    return;
}

int8_t hdc_get_ndim(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_ndim();
}

int8_t hdc_get_ndim_path(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->get_ndim(path);
}


long int* hdc_get_shape(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_shape();
}

long int* hdc_get_shape_path(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->get_shape(path);
}

uint8_t hdc_get_type(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->get_type();
}
void* hdc_as_voidptr(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<void*>();
}

void* hdc_as_voidptr_path(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->as<void*>(path);
}

int* hdc_as_int_1d(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<int*>();
}

void hdc_set_data_int8_scalar(hdc_t* tree, int8_t data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<int8_t>(data);
    return;
}

void hdc_set_data_int8_scalar_path(hdc_t* tree, char* path, int8_t data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<int8_t>(path, data);
    return;
}

void hdc_set_data_int32_scalar(hdc_t* tree, int32_t data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<int32_t>(data);
    return;
}

void hdc_set_data_int32_scalar_path(hdc_t* tree, char* path, int32_t data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<int32_t>(path, data);
    return;
}


void hello() {cout << "Hello" << endl; return;}

void hdc_set_data_double_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    t->set_data<double>(path, ndim, shape, data);
    return;
}

void hdc_set_data_double_sc_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    t->set_data<double>(path, ndim, shape, data);
    return;
}

void hdc_set_data_double_scalar_path(hdc_t* tree, char* path, double data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<double>(path, data);
    return;
}

void hdc_set_data_double_scalar(hdc_t* tree, double data)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data<double>(data);
    return;
}

struct hdc_t* hdc_copy(hdc_t* src)
{
    hdc* t_src = (hdc*)(src->obj);
    struct hdc_t* h = new struct hdc_t;
    h->obj = (void*)(t_src->copy(1));
    return h;
}

void hdc_set_data_string(hdc_t* tree, char* str)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data((string)str);
    return;
}

void hdc_set_data_string_path(hdc_t* tree, char* path, char* str)
{
    hdc* t = (hdc*)tree->obj;
    t->set_data(path,(string)str);
    return;
}

double hdc_as_double_scalar(hdc_t* tree)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<double*>())[0];
}

double hdc_as_double_scalar_path(hdc_t* tree, char* path)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<double*>(path))[0];
}

int8_t hdc_as_int8_scalar(hdc_t* tree)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<int8_t*>())[0];
}

int8_t hdc_as_int8_scalar_path(hdc_t* tree, char* path)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<int8_t*>(path))[0];
}

int32_t hdc_as_int32_scalar(hdc_t* tree)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<int32_t*>())[0];
}

int32_t hdc_as_int32_scalar_path(hdc_t* tree, char* path)
{
    hdc* t = (hdc*)tree->obj;
    return (t->as<int32_t*>(path))[0];
}

void test_str(char* str) {
    cout << "test_str(): " << (string)str << endl;
    return;
}

const char* hdc_get_type_str(hdc_t* tree)
{
    hdc* t = (hdc*)tree->obj;
    return (t->get_type_str().c_str());
}

// end exter C
}