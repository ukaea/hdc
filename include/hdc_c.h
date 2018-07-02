#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "hdc_types.h"

struct hdc_t* hdc_new_empty(); /** Default constructor. Creates empty HDC  */
struct hdc_t* hdc_new_string(const char* str); /** Creates HDC from string/uri */
struct hdc_t* hdc_new_size(size_t size); /** Creates empty HDC of given size */
struct hdc_t*
hdc_new_int8(int rank, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t*
hdc_new_int32(int rank, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t*
hdc_new_double(int rank, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
struct hdc_t* hdc_new_dtype(int rank, size_t* shape,
                            hdc_type_t type); /** Creates new HDC empty array of a given type string, shape and dimension. */
void* hdc_new_void_ptr(); /** Alernative constuctor to be used in Fortran. */
void hdc_delete(struct hdc_t* tree); /** Deletes HDC object (Destructor). */
void hdc_delete_ptr(void* obj);
void* hdc_get_ptr(struct hdc_t* tree);
void hdc_add_child(struct hdc_t* tree, const char* path, struct hdc_t* n); /**  */
void hdc_set_child(struct hdc_t* tree, const char* path, struct hdc_t* n); /**  */
void hdc_delete_child(struct hdc_t* tree, const char* path); /**  */
struct hdc_t* hdc_get(struct hdc_t* tree, const char* path); /**  */
struct hdc_t* hdc_get_slice(struct hdc_t* tree, size_t i); /**  */
struct hdc_t*
hdc_get_slice_path(struct hdc_t* tree, const char* path, size_t i); /** Sets node to i-th slice of current node.  */
void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n); /**  */
void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
void hdc_insert_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
bool hdc_exists(struct hdc_t* tree, const char* path);  /**  */
void hdc_print_info(struct hdc_t* tree);
void hdc_set_int8(struct hdc_t* tree, int rank, size_t* shape, void* data, hdc_flags_t _flags);  /**  */
void hdc_set_int8_path(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */

void hdc_set_double(struct hdc_t* tree, int rank, size_t* shape, void* data, hdc_flags_t _flags);  /**  */
void hdc_set_float(struct hdc_t* tree, int rank, size_t* shape, void* data, hdc_flags_t _flags);  /**  */
void hdc_set_int32(struct hdc_t* tree, int rank, size_t* shape, void* data, hdc_flags_t _flags);  /**  */
void hdc_set_string_path(struct hdc_t* tree, const char* path, char* str);  /**  */
void hdc_set_string(struct hdc_t* tree, const char* str);  /**  */
const char* hdc_as_string_path(struct hdc_t* tree, const char* path);
const char* hdc_as_string(struct hdc_t* tree);

int8_t hdc_get_rank(struct hdc_t* tree);  /**  */
const char* hdc_get_uuid(struct hdc_t* tree);  /**  */
size_t* hdc_get_shape(struct hdc_t* tree);  /** Returns shape of array of node. */
int8_t hdc_get_rank_path(struct hdc_t* tree, const char* path);  /**  */
size_t* hdc_get_shape_path(struct hdc_t* tree, const char* path);  /** Returns shape of array at given path. */
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
int32_t* hdc_as_int32_1d_path(struct hdc_t* tree, const char* path); /**  */
int32_t** hdc_as_int32_2d_path(struct hdc_t* tree, const char* path); /**  */
void* hdc_as_voidptr(struct hdc_t* tree);  /**  */
void* hdc_as_voidptr_path(struct hdc_t* tree, const char* path); /**  */
void hdc_set_int8_scalar_path(struct hdc_t* tree, const char* path, int8_t data); /**  */
void hdc_set_int32_scalar_path(struct hdc_t* tree, const char* path, int32_t data); /**  */
void hdc_set_int32_scalar(struct hdc_t* tree, int32_t data); /**  */
void hdc_set_int32_path(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
void
hdc_set_double_path(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_double_scalar_path(struct hdc_t* tree, const char* path, double data); /**  */
void hdc_set_double_scalar(struct hdc_t* tree, double data); /**  */
void
hdc_set_float_path(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_float_scalar_path(struct hdc_t* tree, const char* path, float data); /**  */
void hdc_set_float_scalar(struct hdc_t* tree, float data); /**  */
double hdc_as_double_scalar(struct hdc_t* tree); /** Returns scalar double. */
double hdc_as_double_scalar_path(struct hdc_t* tree, const char* path); /**   Returns scalar double.  */
float hdc_as_float_scalar(struct hdc_t* tree); /** Returns scalar double. */
float hdc_as_float_scalar_path(struct hdc_t* tree, const char* path); /**   Returns scalar double.  */
int8_t hdc_as_int8_scalar(struct hdc_t* tree); /** Returns scalar int8. */
int8_t hdc_as_int8_scalar_path(struct hdc_t* tree, const char* path); /** Returns scalar int8.  */
int32_t hdc_as_int32_scalar(struct hdc_t* tree); /** Returns scalar int32.  */
int32_t hdc_as_int32_scalar_path(struct hdc_t* tree, const char* path); /** Returns scalar int32.  */
char** hdc_keys(struct hdc_t* tree);
void hdc_keys_py(struct hdc_t* tree, char** keys);
struct hdc_t* hdc_copy(struct hdc_t* src); /** Makes deep copy of HDC tree. */
void hdc_to_json(struct hdc_t* tree, const char* filename, int mode); /** Performs serialization of tree to JSON*/
void hdc_dump(struct hdc_t* tree);
const char* hdc_dumps(struct hdc_t* tree);
const char* hdc_get_datashape_str(struct hdc_t* tree);

struct hdc_t*
hdc_new_int64(int rank, size_t* shape); /** Creates new HDC with empty array of given shape and dimension.  */
void hdc_set_int64(struct hdc_t* tree, int rank, size_t* shape, void* data, hdc_flags_t _flags);  /**  */
int64_t* hdc_as_int64_1d(struct hdc_t* tree); /**  */
int64_t** hdc_as_int64_2d(struct hdc_t* tree); /**  */
int64_t*** hdc_as_int64_3d(struct hdc_t* tree); /**  */
int64_t**** hdc_as_int64_4d(struct hdc_t* tree); /**  */
int64_t* hdc_as_int64_1d_path(struct hdc_t* tree, const char* path); /**  */
int64_t** hdc_as_int64_2d_path(struct hdc_t* tree, const char* path); /**  */
void hdc_set_int64_scalar_path(struct hdc_t* tree, const char* path, int64_t data); /**  */
void hdc_set_int64_scalar(struct hdc_t* tree, int64_t data); /**  */
void hdc_set_int64_path(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
int64_t hdc_as_int64_scalar(struct hdc_t* tree); /** Returns scalar int64.  */
int64_t hdc_as_int64_scalar_path(struct hdc_t* tree, const char* path); /** Returns scalar int64.  */

// void hdc_as_string_fortran(struct hdc_t* tree, char* output, size_t* length);
// void hdc_as_string_path_fortran(struct hdc_t* tree, const char* path, char* output, size_t* length);

int8_t hdc_as_int8_sc_path(struct hdc_t* tree, const char* path); /**  */
const char* hdc_get_type_str(struct hdc_t* tree); /**  */
void hdc_print_type_str(struct hdc_t* tree);
const char* hdc_get_type_str_path(struct hdc_t* tree, const char* path); /**  */
size_t hdc_childs_count(struct hdc_t* tree); /** */
void test_str(char* str);
void HDC_init_c(char* pluginFileName, char* pluginSettingsFileName);
void HDC_destroy_c();
char** HDC_get_available_plugins_c();
const char* hdc_serialize(struct hdc_t* tree);
struct hdc_t* hdc_deserialize(const char* str);

struct hdc_data_t hdc_get_data(struct hdc_t* tree, const char* path);
void hdc_set_data(struct hdc_t* tree, const char* path, struct hdc_data_t data);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
