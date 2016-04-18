#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "types.h"

struct hdc_t* hdc_new_empty();
// struct hdc_t* hdc_new_float(int8_t ndim, size_t* shape, float* data);
void hdc_delete(struct hdc_t* tree);
void hdc_add_child(struct hdc_t* tree, char* path, struct hdc_t* n);
void hdc_set_child(struct hdc_t* tree, char* path, struct hdc_t* n);
void hdc_delete_child(struct hdc_t* tree, char* path);
struct hdc_t* hdc_get_child(struct hdc_t* tree, char* path);
struct hdc_t* hdc_get_slice(struct hdc_t* tree, char* path, size_t i);
bool hdc_has_child(struct hdc_t* tree, char* path);
void hdc_set_data_int8(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data);
void hdc_set_data_double(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data) ;
// void hdc_set_data_int8_(struct hdc_t* tree, int8_t ndim, void* shape, void* data);
// void hdc_set_data(struct hdc_t* tree);
// another data setting methods
int8_t hdc_get_ndim(struct hdc_t* tree);
long int* hdc_get_shape(struct hdc_t* tree);
uint8_t hdc_get_type(struct hdc_t* tree, char* path);
int* hdc_as_int_1d(struct hdc_t* tree); // data getting methods
int** hdc_as_int_2d(struct hdc_t* tree);
void* hdc_as_voidptr(struct hdc_t* tree); // due to fortran???

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
