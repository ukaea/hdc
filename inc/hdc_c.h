#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "types.h"

    struct hdc_t* hdc_new_empty(); /** Default constructor. Creates empty HDC  */
void hdc_delete(struct hdc_t* tree); /** Deletes HDC object (Destructor). */
void hdc_add_child(struct hdc_t* tree, char* path, struct hdc_t* n); /**  */
void hdc_set_child(struct hdc_t* tree, char* path, struct hdc_t* n); /**  */
void hdc_delete_child(struct hdc_t* tree, char* path); /**  */
struct hdc_t* hdc_get_child(struct hdc_t* tree, char* path); /**  */
struct hdc_t* hdc_get_slice(struct hdc_t* tree, size_t i); /**  */
struct hdc_t* hdc_get_slice_path(struct hdc_t* tree, char* path, size_t i); /** Sets node to i-th slice of current node.  */
void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n); /**  */
void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
bool hdc_has_child(struct hdc_t* tree, char* path);  /**  */
void hdc_set_data_int8(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data);  /**  */
void hdc_set_data_double(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data);  /**  */
void hdc_set_data_int32(struct hdc_t* tree, int8_t ndim, const long int* shape, void* data);  /**  */
void hdc_set_data_string_path(struct hdc_t* tree, char* path, char* str);  /**  */
void hdc_set_data_string(struct hdc_t* tree, char* str);  /**  */


int8_t hdc_get_ndim(struct hdc_t* tree);  /**  */
long int* hdc_get_shape(struct hdc_t* tree);  /**  */
int8_t hdc_get_ndim_path(struct hdc_t* tree, char* path);  /**  */
long int* hdc_get_shape_path(struct hdc_t* tree, char* path);  /**  */
uint8_t hdc_get_type(struct hdc_t* tree, char* path);  /**  */
int* hdc_as_int_1d(struct hdc_t* tree); /**  */
int** hdc_as_int_2d(struct hdc_t* tree); /**  */
void* hdc_as_voidptr(struct hdc_t* tree);  /**  */
void* hdc_as_voidptr_path(struct hdc_t* tree, char* path); /**  */
void hdc_set_data_int8_path(struct hdc_t* tree, int8_t data); /**  */
void hdc_set_data_int8_scalar_path(struct hdc_t* tree, char* path, int8_t data); /**  */
void hdc_set_data_int32_path(struct hdc_t* tree, int32_t data); /**  */
void hdc_set_data_int32_scalar_path(struct hdc_t* tree, char* path, int32_t data); /**  */
void hdc_set_data_double_path(struct hdc_t* tree, char* path, int8_t ndim, const long int* shape, void* data); /**  */
void hdc_set_data_double_scalar_path(struct hdc_t* tree, char* path, double data); /**  */
void hdc_set_data_double_scalar(struct hdc_t* tree, double data); /**  */
double hdc_as_double_scalar(struct hdc_t* tree); /** Returns scalar double. */
double hdc_as_double_scalar_path(struct hdc_t* tree, char* path); /**   Returns scalar double.  */
int8_t hdc_as_int8_scalar(struct hdc_t* tree); /** Returns scalar int8. */
int8_t hdc_as_int8_scalar_path(struct hdc_t* tree, char* path); /** Returns scalar int8.  */
int32_t hdc_as_int32_scalar(struct hdc_t* tree); /** Returns scalar int32.  */
int32_t hdc_as_int32_scalar_path(struct hdc_t* tree, char* path); /** Returns scalar int32.  */

struct hdc_t*  hdc_copy(struct hdc_t* src); /** Makes deep copy of HDC tree. */


int8_t hdc_as_int8_sc_path(struct hdc_t* tree, char* path); /**  */
const char* hdc_get_type_str(struct hdc_t* tree); /**  */

void test_str(char* str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
