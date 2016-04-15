#include "hdc_c.h"
#include "hdc.h"
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

bool hdc_has_child(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->has_child(path);
}

void hdc_set_data_int8(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    cout << (int)((int8_t*)data)[0] << (int)((int8_t*)data)[1] << (int)((int8_t*)data)[2] << (int)((int8_t*)data)[3] << endl;
    t->set_data<int8_t>(ndim, shape, data);
    return;
}

void hdc_set_data_int8_(struct hdc_t* tree, int8_t ndim, void* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    cout << (int)((int8_t*)data)[0] << (int)((int8_t*)data)[1] << (int)((int8_t*)data)[2] << (int)((int8_t*)data)[3] << endl;
    t->set_data<int8_t>(ndim, (long int*)shape, data);
    return;
}

int8_t hdc_get_ndim(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_ndim();
}

long int* hdc_get_shape(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_shape();
}

uint8_t hdc_get_type(struct hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->get_type();
}
void* hdc_as_voidptr(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<void*>();
}

int* hdc_as_int_1d(struct hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<int*>();
}

void hello() {cout << "Hello" << endl;}


}