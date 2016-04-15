#include "hdc.h"


extern "C" {

hdc_t* hdc_new_empty() {
    hdc* n = new hdc();
    hdc_t h;
    memset(&h,0,sizeof(h));
    h.obj = (void*)n;
    return h;
}

void hdc_delete(char* path) {
    delete (hdc*)tree->obj;
    return;
}

void hdc_add_child(hdc_t* tree, char* path, hdc_t* n) {
    hdc* t = (hdc*)tree->obj;
    t->add_child(path,(hdc*)n->obj);
    return;
}

void hdc_set_child(hdc_t* tree, char* path, hdc_t* n) {
    hdc* t = (hdc*)tree->obj;
    t->set_child(path, (hdc*)n->obj);
    return;
}
void hdc_delete_child(char* path) {
    hdc* t = (hdc*)tree->obj;
    t->delete_child(path);
    return;
}

hdc_t* hdc_get_child(hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    hdc* node = t->get_child(path);
    hdc_t h;
    memset(&h,0,sizeof(h));
    h.obj = (void*)node;
    return h;
}

bool hdc_has_child(hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->has_child(path);
}

void hdc_set_data_int(hdc_t* tree, int8_t ndim, const long int* shape, void* data) {
    hdc* t = (hdc*)tree->obj;
    t->set_data<int>(ndim, shape, data);
    return;
}

int8_t hdc_get_ndim(hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_ndim();
}

size_t* hdc_get_shape(hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->get_shape();
}

uint8_t hdc_get_type(hdc_t* tree, char* path) {
    hdc* t = (hdc*)tree->obj;
    return t->get_type();
}
void* hdc_as_voidptr(hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<void*>();
}

int* hdc_as_int_1d(hdc_t* tree) {
    hdc* t = (hdc*)tree->obj;
    return t->as<int*>();
}

}