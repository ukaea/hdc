#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "hdc_types.h"
// ------------------------ Constructors --------------------------- //
struct hdc_t* hdc_new_empty(); /** Default constructor. Creates empty HDC  */
struct hdc_t* hdc_new_string(const char* str); /** Creates HDC from string/uri */
struct hdc_t* hdc_new_size(size_t size); /** Creates empty HDC of given size */
struct hdc_t* hdc_new_array(size_t rank, size_t* shape, hdc_type_t type); /** Array constructor */
struct hdc_t* hdc_new_array2(size_t rank, size_t* shape, hdc_type_t type, size_t flags); /** Array constructor */
void* hdc_new_void_ptr(); /** Alernative constuctor to be used in Fortran. */

// ----------------------- Node manipulation ---------------------- //

void hdc_delete(struct hdc_t* tree); /** Deletes HDC object (Destructor). */
void hdc_delete_ptr(void* obj);
void* hdc_get_ptr(struct hdc_t* tree);
void hdc_add_child(struct hdc_t* tree, const char* path, struct hdc_t* n); /**  */
void hdc_set_child(struct hdc_t* tree, const char* path, struct hdc_t* n); /**  */
void hdc_delete_child(struct hdc_t* tree, const char* path); /**  */
struct hdc_t* hdc_get(struct hdc_t* tree, const char* path); /**  */
struct hdc_t*
hdc_get_slice(struct hdc_t* tree, const char* path, size_t i); /** Sets node to i-th slice of current node.  */
void hdc_append_slice(struct hdc_t* tree, struct hdc_t* n); /**  */
void hdc_set_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
void hdc_insert_slice(struct hdc_t* tree, size_t i, struct hdc_t* n); /**  */
bool hdc_exists(struct hdc_t* tree, const char* path);  /**  */
void hdc_print_info(struct hdc_t* tree);
const char* hdc_get_uuid(struct hdc_t* tree);  /**  */
size_t hdc_get_rank(struct hdc_t* tree, const char* path);  /**  */
size_t* hdc_get_shape(struct hdc_t* tree, const char* path);  /** Returns shape of array at given path. */
size_t hdc_get_type(struct hdc_t* tree, const char* path);  /**  */
char** hdc_keys(struct hdc_t* tree);
void hdc_keys_py(struct hdc_t* tree, char** keys);
struct hdc_t* hdc_copy(struct hdc_t* src); /** Makes deep copy of HDC tree. */
void hdc_to_json(struct hdc_t* tree, const char* filename, int mode); /** Performs serialization of tree to JSON*/
void hdc_dump(struct hdc_t* tree);
const char* hdc_dumps(struct hdc_t* tree);
const char* hdc_get_datashape_str(struct hdc_t* tree);
const char* hdc_get_type_str(struct hdc_t* tree, const char* path); /**  */
size_t hdc_childs_count(struct hdc_t* tree); /** */
void test_str(char* str);
const char* hdc_serialize(struct hdc_t* tree);
struct hdc_t* hdc_deserialize(const char* str);
size_t hdc_get_size(struct hdc_t* tree);
char** HDC_get_available_plugins_c();

// ----------------------- Data setters --------------------------- //

void hdc_set_int8_scalar(struct hdc_t* tree, const char* path, int8_t data); /**  */
void hdc_set_int8(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_int16_scalar(struct hdc_t* tree, const char* path, int16_t data); /**  */
void hdc_set_int16(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_int32_scalar(struct hdc_t* tree, const char* path, int32_t data); /**  */
void hdc_set_int32(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
void hdc_set_int64_scalar(struct hdc_t* tree, const char* path, int64_t data); /**  */
void hdc_set_int64(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
void hdc_set_float_scalar(struct hdc_t* tree, const char* path, float data); /**  */
void
hdc_set_float(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_double_scalar(struct hdc_t* tree, const char* path, double data); /**  */
void
hdc_set_double(struct hdc_t* tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_string(struct hdc_t* tree, const char* path, const char* str);  /**  */

// -------------------------------- Data getters -----------------------------------------------//
struct hdc_data_t hdc_get_data(struct hdc_t* tree, const char* path);

const char* hdc_as_string(struct hdc_t* tree, const char* path);
int8_t* hdc_as_int8_array(struct hdc_t* tree, const char* path); /**  */
int16_t* hdc_as_int16_array(struct hdc_t* tree, const char* path); /**  */
int32_t* hdc_as_int32_array(struct hdc_t* tree, const char* path); /**  */
int64_t* hdc_as_int64_array(struct hdc_t* tree, const char* path); /**  */
double* hdc_as_double_array(struct hdc_t* tree, const char* path); /**  */
float* hdc_as_float_array(struct hdc_t* tree, const char* path); /**  */
void* hdc_as_voidptr(struct hdc_t* tree, const char* path); /**  */

double  hdc_as_double_scalar(struct hdc_t* tree, const char* path); /**   Returns scalar double.  */
float   hdc_as_float_scalar(struct hdc_t* tree, const char* path); /**   Returns scalar double.  */
int8_t  hdc_as_int8_scalar(struct hdc_t* tree, const char* path); /** Returns scalar int8.  */
int16_t hdc_as_int16_scalar(struct hdc_t* tree, const char* path); /** Returns scalar int16.  */
int32_t hdc_as_int32_scalar(struct hdc_t* tree, const char* path); /** Returns scalar int32.  */
int64_t hdc_as_int64_scalar(struct hdc_t* tree, const char* path); /** Returns scalar int64.  */

void hdc_set_data(struct hdc_t* tree, const char* path, struct hdc_data_t data);
void hdc_set_scalar(struct hdc_t* tree, const char* path, void* data, hdc_type_t _type);

// ----------------------------- Static methods wrappers ----------------------------------------//

void hdc_init(char* pluginFileName, char* pluginSettingsFileName);
void HDC_destroy_c();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
