#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "types.h"

struct hdc_t* hdc_new_empty(); /** Default constructor. Creates empty HDC  */

struct hdc_t* hdc_new_int8(int ndim, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t* hdc_new_int32(int ndim, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t* hdc_new_double(int ndim, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t* hdc_new_dtype(int ndim, size_t* shape, TypeID type); /** Creates new HDC empty array of a given type string, shape and dimension. */
void* hdc_new_void_ptr(); /** Alernative constuctor to be used in Fortran. */
void hdc_delete(struct hdc_t* tree); /** Deletes HDC object (Destructor). */
void hdc_delete_ptr(void* obj);
void* hdc_get_ptr(struct hdc_t* tree);
void hdc_add_child(struct hdc_t* tree, char* path, struct hdc_t* n); /**  */
void hdc_set_child(struct hdc_t* tree, char* path, struct hdc_t* n); /**  */
void hdc_delete_child(struct hdc_t* tree, char* path); /**  */
struct hdc_t* hdc_get(struct hdc_t* tree, char* path); /**  */
struct hdc_t* hdc_get_slice(struct hdc_t* tree, size_t i); /**  */
struct hdc_t* hdc_get_slice_path(struct hdc_t* tree, char* path, size_t i); /** Sets node to i-th slice of current node.  */
void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n); /**  */
void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
void hdc_insert_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
bool hdc_has_child(struct hdc_t* tree, char* path);  /**  */
void hdc_set_int8     (struct hdc_t* tree,             int ndim, size_t* shape, void* data);  /**  */
void hdc_set_int8_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data); /**  */

void hdc_set_double(struct hdc_t* tree, int ndim, size_t* shape, void* data);  /**  */
void hdc_set_float(struct hdc_t* tree, int ndim, size_t* shape, void* data);  /**  */
void hdc_set_int32(struct hdc_t* tree, int ndim, size_t* shape, void* data);  /**  */
void hdc_set_string_path(struct hdc_t* tree, char* path, char* str);  /**  */
void hdc_set_string(struct hdc_t* tree, char* str);  /**  */
const char* hdc_as_string_path(struct hdc_t* tree, char* path);
const char* hdc_as_string(struct hdc_t* tree);

int8_t hdc_get_ndim(struct hdc_t* tree);  /**  */
const char* hdc_get_uuid(struct hdc_t* tree);  /**  */
size_t* hdc_get_shape(struct hdc_t* tree);  /** Returns shape of array of node. */
int8_t hdc_get_ndim_path(struct hdc_t* tree, char* path);  /**  */
size_t* hdc_get_shape_path(struct hdc_t* tree, char* path);  /** Returns shape of array at given path. */
uint8_t hdc_get_type(struct hdc_t* tree);  /**  */
int32_t* hdc_as_int32_1d(struct hdc_t* tree); /**  */
int32_t** hdc_as_int32_2d(struct hdc_t* tree); /**  */
int32_t*** hdc_as_int32_3d(struct hdc_t* tree); /**  */
int32_t**** hdc_as_int32_4d(struct hdc_t* tree); /**  */
int8_t* hdc_as_int8_1d(struct hdc_t* tree); /**  */
int8_t** hdc_as_int8_2d(struct hdc_t* tree); /**  */
double* hdc_as_double_1d(struct hdc_t* tree); /**  */
double** hdc_as_double_2d(struct hdc_t* tree); /**  */
float* hdc_as_float_1d(struct hdc_t* tree); /**  */
float** hdc_as_float_2d(struct hdc_t* tree); /**  */
int32_t* hdc_as_int32_1d_path(struct hdc_t* tree, char* path); /**  */
int32_t** hdc_as_int32_2d_path(struct hdc_t* tree, char* path); /**  */
void* hdc_as_voidptr(struct hdc_t* tree);  /**  */
void* hdc_as_voidptr_path(struct hdc_t* tree, char* path); /**  */
void hdc_set_int8_scalar_path(struct hdc_t* tree, char* path, int8_t data); /**  */
void hdc_set_int32_scalar_path(struct hdc_t* tree, char* path, int32_t data); /**  */
void hdc_set_int32_scalar(struct hdc_t* tree, int32_t data); /**  */
void hdc_set_int32_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data);
void hdc_set_double_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data); /**  */
void hdc_set_double_scalar_path(struct hdc_t* tree, char* path, double data); /**  */
void hdc_set_double_scalar(struct hdc_t* tree, double data); /**  */
void hdc_set_float_path(struct hdc_t* tree, char* path, int ndim, size_t* shape, void* data); /**  */
void hdc_set_float_scalar_path(struct hdc_t* tree, char* path, float data); /**  */
void hdc_set_float_scalar(struct hdc_t* tree, float data); /**  */
double hdc_as_double_scalar(struct hdc_t* tree); /** Returns scalar double. */
double hdc_as_double_scalar_path(struct hdc_t* tree, char* path); /**   Returns scalar double.  */
float hdc_as_float_scalar(struct hdc_t* tree); /** Returns scalar double. */
float hdc_as_float_scalar_path(struct hdc_t* tree, char* path); /**   Returns scalar double.  */
int8_t hdc_as_int8_scalar(struct hdc_t* tree); /** Returns scalar int8. */
int8_t hdc_as_int8_scalar_path(struct hdc_t* tree, char* path); /** Returns scalar int8.  */
int32_t hdc_as_int32_scalar(struct hdc_t* tree); /** Returns scalar int32.  */
int32_t hdc_as_int32_scalar_path(struct hdc_t* tree, char* path); /** Returns scalar int32.  */
char** hdc_keys(struct hdc_t* tree);
void hdc_keys_py(struct hdc_t* tree, char** keys);
struct hdc_t*  hdc_copy(struct hdc_t* src); /** Makes deep copy of HDC tree. */
void hdc_to_json(struct hdc_t* tree, char* path, int mode); /** Performs serialization of tree to JSON*/
void hdc_dump(struct hdc_t* tree);
const char* hdc_dumps(struct hdc_t* tree);
const char* hdc_get_datashape_str(struct hdc_t* tree);

int8_t hdc_as_int8_sc_path(struct hdc_t* tree, char* path); /**  */
const char* hdc_get_type_str(struct hdc_t* tree); /**  */
void hdc_print_type_str(struct hdc_t* tree);
const char* hdc_get_type_str_path(struct hdc_t* tree, char* path); /**  */
size_t hdc_childs_count(struct hdc_t* tree); /** */
void test_str(char* str);
void HDC_init_c(char* pluginFileName, char* pluginSettingsFileName);
void HDC_init_c_plain();
void HDC_destroy_c();
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
