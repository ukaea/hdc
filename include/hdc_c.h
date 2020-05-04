#ifndef HDC_C_H
#define HDC_C_H

#ifdef __cplusplus
extern "C" {
#endif
#include "hdc_types.h"
// ------------------------ Constructors --------------------------- //
hdc_t hdc_new_empty(); /** Default constructor. Creates empty HDC  */
hdc_t hdc_new_string(const char* str); /** Creates HDC from string/uri */
hdc_t hdc_new_size(size_t size); /** Creates empty HDC of given size */
hdc_t hdc_new_array(size_t rank, size_t* shape, hdc_type_t type); /** Array constructor */
hdc_t hdc_new_array2(size_t rank, size_t* shape, hdc_type_t type, size_t flags); /** Array constructor */
void* hdc_new_void_ptr(); /** Alernative constuctor to be used in Fortran. */

// ----------------------- Node manipulation ---------------------- //

void hdc_delete_ptr(void* obj);
void* hdc_get_ptr(hdc_t tree);
void hdc_add_child(hdc_t tree, const char* path, hdc_t n); /**  */
void hdc_set_child(hdc_t tree, const char* path, hdc_t n); /**  */
void hdc_delete_child(hdc_t tree, const char* path); /**  */
hdc_t hdc_get(hdc_t tree, const char* path); /**  */
hdc_t
hdc_get_slice(hdc_t tree, const char* path, size_t i); /** Sets node to i-th slice of current node.  */
void hdc_append_slice(hdc_t tree, hdc_t n); /**  */
void hdc_set_slice(hdc_t tree, size_t i, hdc_t n); /**  */
void hdc_insert_slice(hdc_t tree, size_t i, hdc_t n); /**  */
bool hdc_exists(hdc_t tree, const char* path);  /**  */
void hdc_print_info(hdc_t tree);
const char* hdc_get_uuid(hdc_t tree);  /**  */
size_t hdc_get_rank(hdc_t tree, const char* path);  /**  */
size_t hdc_get_type(hdc_t tree, const char* path);  /**  */
void hdc_keys(hdc_t tree, char** keys, size_t* nkeys);
hdc_t hdc_copy(hdc_t src, bool deep_copy); /** Makes copy of HDC tree. */
//void hdc_to_json(hdc_t tree, const char* filename, int mode); /** Performs serialization of tree to JSON*/
void hdc_dump(hdc_t tree);
const char* hdc_dumps(hdc_t tree);
const char* hdc_get_datashape_str(hdc_t tree);
const char* hdc_get_type_str(hdc_t tree, const char* path); /**  */
void test_str(char* str);
hdc_t hdc_load(const char* uri, const char* datapath);
void hdc_save(hdc_t tree, const char* uri);
void hdc_serialize(hdc_t tree, char* buffer);
hdc_t hdc_deserialize(const char* str);
size_t hdc_get_size(hdc_t tree);
void hdc_search_plugins(char** keys, int* num);
void hdc_clean(hdc_t tree);

// ----------------------- Data setters --------------------------- //

void hdc_set_int8_scalar(hdc_t tree, const char* path, int8_t data); /**  */
void hdc_set_int8(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_int16_scalar(hdc_t tree, const char* path, int16_t data); /**  */
void hdc_set_int16(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_int32_scalar(hdc_t tree, const char* path, int32_t data); /**  */
void hdc_set_int32(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
void hdc_set_int64_scalar(hdc_t tree, const char* path, int64_t data); /**  */
void hdc_set_int64(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags);
void hdc_set_float_scalar(hdc_t tree, const char* path, float data); /**  */
void
hdc_set_float(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_double_scalar(hdc_t tree, const char* path, double data); /**  */
void
hdc_set_double(hdc_t tree, const char* path, int rank, size_t* shape, void* data, hdc_flags_t _flags); /**  */
void hdc_set_string(hdc_t tree, const char* path, const char* str);  /**  */

// -------------------------------- Data getters -----------------------------------------------//
struct hdc_data_t hdc_get_data(hdc_t tree, const char* path);

const char* hdc_as_string(hdc_t tree, const char* path);
int8_t* hdc_as_int8_array(hdc_t tree, const char* path); /**  */
int16_t* hdc_as_int16_array(hdc_t tree, const char* path); /**  */
int32_t* hdc_as_int32_array(hdc_t tree, const char* path); /**  */
int64_t* hdc_as_int64_array(hdc_t tree, const char* path); /**  */
double* hdc_as_double_array(hdc_t tree, const char* path); /**  */
float* hdc_as_float_array(hdc_t tree, const char* path); /**  */
void* hdc_as_voidptr(hdc_t tree, const char* path); /**  */

double  hdc_as_double_scalar(hdc_t tree, const char* path); /**   Returns scalar double.  */
float   hdc_as_float_scalar(hdc_t tree, const char* path); /**   Returns scalar double.  */
int8_t  hdc_as_int8_scalar(hdc_t tree, const char* path); /** Returns scalar int8.  */
int16_t hdc_as_int16_scalar(hdc_t tree, const char* path); /** Returns scalar int16.  */
int32_t hdc_as_int32_scalar(hdc_t tree, const char* path); /** Returns scalar int32.  */
int64_t hdc_as_int64_scalar(hdc_t tree, const char* path); /** Returns scalar int64.  */

void hdc_set_data(hdc_t tree, const char* path, struct hdc_data_t data);
void hdc_set_external(hdc_t tree, const char* path, struct hdc_data_t data);
void hdc_set_scalar(hdc_t tree, const char* path, void* data, hdc_type_t _type);

// ----------------------------- Static methods wrappers ----------------------------------------//

void hdc_init(const char* pluginFileName, const char* pluginSettingsFileName);
void hdc_destroy();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HDC_C_H
