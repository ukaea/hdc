#include "catch.hpp"
#include "hdc.hpp"
#include "hdc_c.h"

//#include "types.h"
struct hdc_t {
    void* obj;
};

TEST_CASE("C_EmptyNode", "[CHDC]")
{
    struct hdc_t* h = hdc_new_empty();
    CHECK(0 == hdc_get_shape(h)[0]);
    CHECK(1 == hdc_get_ndim(h));
    CHECK(HDC_EMPTY == hdc_get_type(h));
    CHECK(strcmp("null", hdc_get_type_str(h)) == 0);
    CHECK(false == hdc_exists(h, "aaa"));
    hdc_delete(h);
}

TEST_CASE("C_EmptyArrayNode", "[CHDC]")
{
    int ndim = 1;
    size_t shape[] = { 4 };

    struct hdc_t* hi8 = hdc_new_dtype(ndim, shape, HDC_INT8);
    CHECK(1 == hdc_get_ndim(hi8));
    CHECK(4 == hdc_get_shape(hi8)[0]);
    CHECK(HDC_INT8 == hdc_get_type(hi8));
    CHECK(strcmp("int8", hdc_get_type_str(hi8)) == 0);
    hdc_delete(hi8);

    struct hdc_t* hi32 = hdc_new_dtype(ndim, shape, HDC_INT32);
    CHECK(1 == hdc_get_ndim(hi32));
    CHECK(4 == hdc_get_shape(hi32)[0]);
    CHECK(HDC_INT32 == hdc_get_type(hi32));
    CHECK(strcmp("int32", hdc_get_type_str(hi32)) == 0);
    hdc_delete(hi32);

    struct hdc_t* hi64 = hdc_new_dtype(ndim, shape, HDC_INT64);
    CHECK(1 == hdc_get_ndim(hi64));
    CHECK(4 == hdc_get_shape(hi64)[0]);
    CHECK(HDC_INT64 == hdc_get_type(hi64));
    CHECK(strcmp("int64", hdc_get_type_str(hi64)) == 0);
    hdc_delete(hi64);

    struct hdc_t* hd = hdc_new_dtype(ndim, shape, HDC_DOUBLE);
    CHECK(1 == hdc_get_ndim(hd));
    CHECK(4 == hdc_get_shape(hd)[0]);
    CHECK(HDC_DOUBLE == hdc_get_type(hd));
    CHECK(strcmp("float64", hdc_get_type_str(hd)) == 0);
    hdc_delete(hd);

    hd = hdc_new_dtype(ndim, shape, HDC_DOUBLE);
    CHECK(1 == hdc_get_ndim(hd));
    CHECK(4 == hdc_get_shape(hd)[0]);
    CHECK(HDC_DOUBLE == hdc_get_type(hd));
    CHECK(strcmp("float64", hdc_get_type_str(hd)) == 0);
    hdc_delete(hd);
}


TEST_CASE("C_NodeManipulation", "[CHDC]")
{
    struct hdc_t* tree = hdc_new_empty();
    struct hdc_t* n1 = hdc_new_empty();
    struct hdc_t* n2 = hdc_new_empty();
    hdc_add_child(tree, "aaa/bbb", n1);
    CHECK(HDC_STRUCT == hdc_get_type(tree));
    CHECK(strcmp("struct", hdc_get_type_str(tree)) == 0);
    CHECK(true == hdc_exists(tree, "aaa/bbb"));
    CHECK(true == hdc_exists(tree, "aaa"));
    struct hdc_t* tmp = hdc_get(tree, "aaa/bbb");
    CHECK(strcmp(hdc_get_uuid(n1), hdc_get_uuid(tmp)) == 0);
    CHECK(strcmp(hdc_get_uuid(n2), hdc_get_uuid(hdc_get(tree, "aaa/bbb"))) != 0);
    // Try subtree
    struct hdc_t* sub = hdc_get(tree, "aaa");
    CHECK(true == hdc_exists(sub, "bbb"));
    CHECK(strcmp(hdc_get_uuid(n1), hdc_get_uuid(hdc_get(sub, "bbb"))) == 0);
    // Test set
    hdc_set_child(tree, "aaa/bbb", n2);
    CHECK(true == hdc_exists(sub, "bbb"));
    CHECK(strcmp(hdc_get_uuid(n2), hdc_get_uuid(hdc_get(sub, "bbb"))) == 0);
    CHECK(strcmp(hdc_get_uuid(n1), hdc_get_uuid(hdc_get(tree, "aaa/bbb"))) != 0);
    // Test delete
    hdc_delete_child(tree, "aaa/bbb");
    CHECK(false == hdc_exists(tree, "aaa/bbb"));
    CHECK(true == hdc_exists(tree, "aaa"));
    hdc_add_child(tree, "aaa/bbb", n1);
    hdc_delete_child(tree, "aaa");
    CHECK(false == hdc_exists(tree, "aaa"));
    hdc_delete(tree);
    hdc_delete(n1);
    hdc_delete(sub);
}

TEST_CASE("C_Int8DataManipulation", "[CHDC]")
{
    int ndim = 1;
    size_t shape[] = { 4 };
    int8_t data[] = { 7, 20, 3, 5 };
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int8(h, ndim, shape, (void*)data, HDCDefault);
    CHECK(HDC_INT8 == hdc_get_type(h));
    CHECK(1 == hdc_get_ndim(h));
    CHECK(4 == hdc_get_shape(h)[0]);
    CHECK(strcmp("int8", hdc_get_type_str(h)) == 0);
    int8_t* data2 = hdc_as_int8_1d(h);
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 120;
    hdc_set_int8(h, ndim, shape, (void*)data, HDCDefault);
    data2 = hdc_as_int8_1d(h);
    CHECK(120 == data2[3]);
}

TEST_CASE("C_Int32DataManipulation", "[CHDC]")
{
    int ndim = 1;
    size_t shape[] = { 4 };
    int32_t data[] = { 777, 20202020, 3333, 555555 };
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int32(h, ndim, shape, (void*)data, HDCDefault);
    CHECK(HDC_INT32 == hdc_get_type(h));
    CHECK(1 == hdc_get_ndim(h));
    CHECK(4 == hdc_get_shape(h)[0]);
    CHECK(strcmp("int32", hdc_get_type_str(h)) == 0);
    int32_t* data2 = hdc_as_int32_1d(h);
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666;
    hdc_set_int32(h, ndim, shape, (void*)data, HDCDefault);
    data2 = hdc_as_int32_1d(h);
    CHECK(666 == data2[3]);
}

TEST_CASE("C_Int64DataManipulation", "[CHDC]")
{
    int ndim = 1;
    size_t shape[] = { 4 };
    int64_t data[] = { 777, 20202020, 3333, 555555 };
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int64(h, ndim, shape, (void*)data, HDCDefault);
    CHECK(HDC_INT64 == hdc_get_type(h));
    CHECK(1 == hdc_get_ndim(h));
    CHECK(4 == hdc_get_shape(h)[0]);
    CHECK(strcmp("int64", hdc_get_type_str(h)) == 0);
    int64_t* data2 = hdc_as_int64_1d(h);
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666;
    hdc_set_int64(h, ndim, shape, (void*)data, HDCDefault);
    data2 = hdc_as_int64_1d(h);
    CHECK(666 == data2[3]);
}


TEST_CASE("C_DoubleDataManipulation", "[CHDC]")
{
    int ndim = 1;
    size_t shape[] = { 4 };
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    struct hdc_t* h = hdc_new_empty();
    hdc_set_double(h, ndim, shape, data, HDCDefault);
    CHECK(HDC_DOUBLE == hdc_get_type(h));
    CHECK(1 == hdc_get_ndim(h));
    CHECK(4 == hdc_get_shape(h)[0]);
    CHECK(strcmp("float64", hdc_get_type_str(h)) == 0);
    double* data2 = hdc_as_double_1d(h);
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666.666;
    hdc_set_double(h, ndim, shape, data, HDCDefault);
    data2 = hdc_as_double_1d(h);
    CHECK(666.666 == data2[3]);
}

TEST_CASE("C_StringDataManipulation", "[CHDC]")
{
    struct hdc_t* h = hdc_new_empty();
    const char* str = "lalalalala  bleble";
    hdc_set_string(h, str);
    const char* str2 = hdc_as_string(h);
    CHECK(strcmp(str, str2) == 0);
    struct hdc_t* h2 = hdc_new_string(str);
    const char* str22 = hdc_as_string(h2);
    CHECK(strcmp(str, str22) == 0);
}

TEST_CASE("C_SliceManipulation", "[CHDC]")
{
    struct hdc_t* h = hdc_new_empty();
    struct hdc_t* sl = hdc_new_empty();
    struct hdc_t* sl2 = hdc_new_empty();
    hdc_append_slice(h, sl);
    hdc_append_slice(h, sl2);
    CHECK(HDC_LIST == hdc_get_type(h));
    CHECK(strcmp("list", hdc_get_type_str(h)) == 0);
    CHECK(1 == hdc_get_ndim(h));
    CHECK(2 == hdc_get_shape(h)[0]);
    CHECK(strcmp(hdc_get_uuid(sl), hdc_get_uuid(hdc_get_slice(h, 0))) == 0);
    CHECK(strcmp(hdc_get_uuid(sl2), hdc_get_uuid(hdc_get_slice(h, 1))) == 0);
    struct hdc_t* sl3 = hdc_new_empty();
    hdc_insert_slice(h, 1, sl3);
    CHECK(strcmp(hdc_get_uuid(sl3), hdc_get_uuid(hdc_get_slice(h, 1))) == 0);
    CHECK(strcmp(hdc_get_uuid(sl2), hdc_get_uuid(hdc_get_slice(h, 2))) == 0);
    struct hdc_t* sl4 = hdc_new_empty();
    hdc_set_slice(h, 1, sl4);
    CHECK(strcmp(hdc_get_uuid(sl4), hdc_get_uuid(hdc_get_slice(h, 1))) == 0);
}


TEST_CASE("C_get_data", "[CHDC]")
{
    struct hdc_t* h = hdc_new_empty();
    double array_in[4] = {1.1,2.2,3.3,4.4};
    hdc_data_t data_in;
    memset(&data_in, 0, sizeof(hdc_data_t));
    data_in.ndim = 1;
    data_in.shape[0] = 4;
    data_in.type = HDC_DOUBLE;
    data_in.flags = HDCDefault;
    data_in.data = (char*)(&array_in);
    hdc_set_data(h, "", data_in);
    hdc_data_t data_out = hdc_get_data(h, "");
    CHECK(data_in.ndim == data_out.ndim);
    CHECK(data_in.type == data_out.type);
    CHECK(data_in.flags == data_out.flags);
    for (size_t i=0; i<HDC_MAX_DIMS; i++) CHECK(data_in.shape[i] == data_out.shape[i]);
    hdc_delete(h);
}

//-----------------------------------------------------------------------------------------
/*
TEST_CASE("C_GetKeys","[CHDC]") {
    HDC* list = new HDC();
    list->set_type(HDC_LIST);
    CHECK(true == list->keys().empty());
    HDC* val = new HDC();
    CHECK(true == val->keys().empty());
    HDC* empty = new HDC();
    CHECK(true == empty->keys().empty());
    HDC* tree = new HDC();
    tree->add_child("aaa",new HDC());
    tree->add_child("bbb",new HDC());
    tree->add_child("ccc/sss",new HDC());
    CHECK(3u == tree->keys().size());
    vector<string> keys = tree->keys();
    for (size_t i=0;i<keys.size();i++) CHECK(true,tree->exists(keys[i]));
}
*/
