#include "catch.hpp"
#include "hdc.hpp"
#include "hdc_c.h"

TEST_CASE("C_EmptyNode", "[CHDC]")
{
    hdc_t h = hdc_new_empty();
    hdc_data_t h_data = hdc_get_data(h,"");
    CHECK(0 == h_data.shape[0]);
    CHECK(1 == h_data.rank);
    CHECK(HDC_EMPTY == h_data.type);
    CHECK(strcmp("null", hdc_get_type_str(h, "")) == 0);
    CHECK(false == hdc_exists(h, "aaa"));
}

TEST_CASE("C_EmptyArrayNode", "[CHDC]")
{
    int rank = 1;
    size_t shape[] = { 4 };

    hdc_t hi8 = hdc_new_array(rank, shape, HDC_INT8);
    hdc_data_t hi8_data = hdc_get_data(hi8, "");
    CHECK(1 == hi8_data.rank);
    CHECK(4 == hi8_data.shape[0]);
    CHECK(HDC_INT8 == hi8_data.type);
    CHECK(strcmp("int8", hdc_get_type_str(hi8, "")) == 0);

    hdc_t hi16 = hdc_new_array(rank, shape, HDC_INT16);
    hdc_data_t hi16_data = hdc_get_data(hi16, "");
    CHECK(1 == hi16_data.rank);
    CHECK(4 == hi16_data.shape[0]);
    CHECK(HDC_INT16 == hi16_data.type);
    CHECK(strcmp("int16", hdc_get_type_str(hi16, "")) == 0);

    hdc_t hi32 = hdc_new_array(rank, shape, HDC_INT32);
    hdc_data_t hi32_data = hdc_get_data(hi32, "");
    CHECK(1 == hi32_data.rank);
    CHECK(4 == hi32_data.shape[0]);
    CHECK(HDC_INT32 == hi32_data.type);
    CHECK(strcmp("int32", hdc_get_type_str(hi32, "")) == 0);

    hdc_t hi64 = hdc_new_array(rank, shape, HDC_INT64);
    hdc_data_t hi64_data = hdc_get_data(hi64, "");
    CHECK(1 == hi64_data.rank);
    CHECK(4 == hi64_data.shape[0]);
    CHECK(HDC_INT64 == hi64_data.type);
    CHECK(strcmp("int64", hdc_get_type_str(hi64, "")) == 0);

    hdc_t hd = hdc_new_array(rank, shape, HDC_DOUBLE);
    hdc_data_t hd_data = hdc_get_data(hd, "");
    CHECK(1 == hd_data.rank);
    CHECK(4 == hd_data.shape[0]);
    CHECK(HDC_DOUBLE == hd_data.type);
    CHECK(strcmp("float64", hdc_get_type_str(hd, "")) == 0);

    hdc_t hf = hdc_new_array(rank, shape, HDC_FLOAT);
    hdc_data_t hf_data = hdc_get_data(hf, "");
    CHECK(1 == hf_data.rank);
    CHECK(4 == hf_data.shape[0]);
    CHECK(HDC_FLOAT == hf_data.type);
    CHECK(strcmp("float32", hdc_get_type_str(hf, "")) == 0);
}


TEST_CASE("C_NodeManipulation", "[CHDC]")
{
    hdc_t tree = hdc_new_empty();
    hdc_t n1 = hdc_new_empty();
    hdc_t n2 = hdc_new_empty();
    hdc_add_child(tree, "aaa/bbb", n1);
    CHECK(HDC_STRUCT == hdc_get_type(tree, ""));
    CHECK(strcmp("struct", hdc_get_type_str(tree, "")) == 0);
    CHECK(true == hdc_exists(tree, "aaa/bbb"));
    CHECK(true == hdc_exists(tree, "aaa"));
    hdc_t tmp = hdc_get(tree, "aaa/bbb");
    CHECK(strcmp(n1.uuid, tmp.uuid) == 0);
    CHECK(strcmp(n2.uuid, hdc_get(tree, "aaa/bbb").uuid) != 0);
    // Try subtree
    hdc_t sub = hdc_get(tree, "aaa");
    CHECK(true == hdc_exists(sub, "bbb"));
    CHECK(strcmp(n1.uuid, hdc_get(sub, "bbb").uuid) == 0);
    // Test set
    hdc_set_child(tree, "aaa/bbb", n2);
    CHECK(true == hdc_exists(sub, "bbb"));
    CHECK(strcmp(n2.uuid, hdc_get(sub, "bbb").uuid) == 0);
    CHECK(strcmp(n1.uuid, hdc_get(tree, "aaa/bbb").uuid) != 0);
    // Test delete
    hdc_delete_child(tree, "aaa/bbb");
    CHECK(false == hdc_exists(tree, "aaa/bbb"));
    CHECK(true == hdc_exists(tree, "aaa"));
    hdc_add_child(tree, "aaa/bbb", n1);
    hdc_delete_child(tree, "aaa");
    CHECK(false == hdc_exists(tree, "aaa"));
}

TEST_CASE("C_Int8DataManipulation", "[CHDC]")
{
    int rank = 1;
    size_t shape[] = { 4 };
    int8_t data_in[] = { 7, 2, 3, 5 };

    hdc_t h = hdc_new_empty();
    hdc_set_int8(h, "", rank, shape, (void*)data_in, HDCDefault);
    hdc_data_t h_data = hdc_get_data(h, "");
    CHECK(HDC_INT8 == h_data.type);
    CHECK(1 == h_data.rank);
    CHECK(4 == h_data.shape[0]);
    int8_t* data_out = hdc_as_int8_array(h, "");
    for (int i = 0; i < 3; i++) CHECK(data_in[i] == data_out[i]);
    data_in[3] = 120;
    hdc_set_int8(h, "", rank, shape, (void*)data_in, HDCDefault);
    data_out = hdc_as_int8_array(h, "");
    CHECK(120 == data_out[3]);

    int8_t data_in2[] = { 70, 20, 30, 50 };
    hdc_set_int8(h, "", rank, shape, (void*)data_in2, HDCDefault);
    data_out = hdc_as_int8_array(h, "");
    for (int i = 0; i < 3; i++) CHECK(data_in2[i] == data_out[i]);
}

TEST_CASE("C_Int32DataManipulation", "[CHDC]")
{
    int rank = 1;
    size_t shape[] = { 4 };
    int32_t data[] = { 777, 20202020, 3333, 555555 };
    hdc_t h = hdc_new_empty();
    hdc_set_int32(h, "", rank, shape, (void*)data, HDCDefault);
    hdc_data_t h_data = hdc_get_data(h, "");
    CHECK(HDC_INT32 == h_data.type);
    CHECK(1 == h_data.rank);
    CHECK(4 == h_data.shape[0]);
    CHECK(strcmp("int32", hdc_get_type_str(h, "")) == 0);
    int32_t* data2 = hdc_as_int32_array(h, "");
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666;
    hdc_set_int32(h, "", rank, shape, (void*)data, HDCDefault);
    data2 = hdc_as_int32_array(h, "");
    CHECK(666 == data2[3]);
}

TEST_CASE("C_Int64DataManipulation", "[CHDC]")
{
    int rank = 1;
    size_t shape[] = { 4 };
    int64_t data[] = { 777, 20202020, 3333, 555555 };
    hdc_t h = hdc_new_empty();
    hdc_set_int64(h, "", rank, shape, (void*)data, HDCDefault);
    hdc_data_t h_data = hdc_get_data(h, "");
    CHECK(HDC_INT64 == h_data.type);
    CHECK(1 == h_data.rank);
    CHECK(4 == h_data.shape[0]);
    CHECK(strcmp("int64", hdc_get_type_str(h, "")) == 0);
    int64_t* data2 = hdc_as_int64_array(h, "");
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666;
    hdc_set_int64(h, "", rank, shape, (void*)data, HDCDefault);
    data2 = hdc_as_int64_array(h, "");
    CHECK(666 == data2[3]);
}


TEST_CASE("C_DoubleDataManipulation", "[CHDC]")
{
    int rank = 1;
    size_t shape[] = { 4 };
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    hdc_t h = hdc_new_empty();
    hdc_set_double(h, "", rank, shape, data, HDCDefault);
    hdc_data_t h_data = hdc_get_data(h, "");
    CHECK(HDC_DOUBLE == h_data.type);
    CHECK(1 == h_data.rank);
    CHECK(4 == h_data.shape[0]);
    CHECK(strcmp("float64", hdc_get_type_str(h, "")) == 0);
    double* data2 = hdc_as_double_array(h, "");
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    data[3] = 666.666;
    hdc_set_double(h, "", rank, shape, data, HDCDefault);
    data2 = hdc_as_double_array(h, "");
    CHECK(666.666 == data2[3]);
}

TEST_CASE("C_SetDataObj", "[CHDC]")
{
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    hdc_data_t d;
    d.data = (char*)&data;
    d.type = HDC_DOUBLE;
    d.flags = HDCDefault;
    d.rank = 1;
    d.shape[0] = 4;
    hdc_t h = hdc_new_size(4*sizeof(double));
    hdc_set_data(h,"",d);
    double* gdata = hdc_as_double_array(h,"");
    CHECK(hdc_get_type(h,"") == HDC_DOUBLE);
    for (auto i = 0;i<1;i++) {
        CHECK(gdata[i] == data[i]);
    }
}

TEST_CASE("C_SetExternal", "[CHDC]")
{
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    hdc_data_t d;
    d.data = (char*)&data;
    d.type = HDC_DOUBLE;
    d.flags = HDCDefault;
    d.rank = 1;
    d.shape[0] = 4;
    hdc_t h = hdc_new_empty();
    hdc_set_external(h,"",d);
    double* gdata = hdc_as_double_array(h,"");
    CHECK(hdc_get_type(h,"") == HDC_DOUBLE);
    for (auto i = 0;i<1;i++) {
        CHECK(gdata[i] == data[i]);
    }
}

TEST_CASE("C_StringDataManipulation", "[CHDC]")
{
    hdc_t h = hdc_new_empty();
    const char* str = "lalalalala  bleble";
    hdc_set_string(h, "", str);
    const char* str2 = hdc_as_string(h, "");
    CHECK(strcmp(str, str2) == 0);
    hdc_t h2 = hdc_new_string(str);
    const char* str22 = hdc_as_string(h2, "");
    CHECK(strcmp(str, str22) == 0);
}

TEST_CASE("C_SliceManipulation", "[CHDC]")
{
    hdc_t h = hdc_new_empty();
    hdc_t sl = hdc_new_empty();
    hdc_t sl2 = hdc_new_empty();
    hdc_append_slice(h, sl);
    hdc_append_slice(h, sl2);
    hdc_data_t h_data = hdc_get_data(h, "");
    CHECK(HDC_LIST == h_data.type);
    CHECK(strcmp("list", hdc_get_type_str(h, "")) == 0);
    CHECK(1 == h_data.rank);
    CHECK(2 == h_data.shape[0]);
    CHECK(strcmp(sl.uuid, hdc_get_slice(h, "", 0).uuid) == 0);
    CHECK(strcmp(sl2.uuid, hdc_get_slice(h, "", 1).uuid) == 0);
    hdc_t sl3 = hdc_new_empty();
    hdc_insert_slice(h, 1, sl3);
    CHECK(strcmp(sl3.uuid, hdc_get_slice(h, "", 1).uuid) == 0);
    CHECK(strcmp(sl2.uuid, hdc_get_slice(h, "", 2).uuid) == 0);
    hdc_t sl4 = hdc_new_empty();
    hdc_set_slice(h, 1, sl4);
    CHECK(strcmp(sl4.uuid, hdc_get_slice(h, "", 1).uuid) == 0);
}


TEST_CASE("C_get_data", "[CHDC]")
{
    hdc_t h = hdc_new_empty();
    double array_in[4] = {1.1,2.2,3.3,4.4};
    hdc_data_t data_in;
    memset(&data_in, 0, sizeof(hdc_data_t));
    data_in.rank = 1;
    data_in.shape[0] = 4;
    data_in.type = HDC_DOUBLE;
    data_in.flags = HDCDefault;
    data_in.data = (char*)(&array_in);
    hdc_set_data(h, "", data_in);
    hdc_data_t data_out = hdc_get_data(h, "");
    CHECK(data_in.rank == data_out.rank);
    CHECK(data_in.type == data_out.type);
    CHECK(data_in.flags == data_out.flags);
    for (size_t i=0; i<HDC_MAX_DIMS; i++) CHECK(data_in.shape[i] == data_out.shape[i]);
}

TEST_CASE("C_SetDataScalar","[CHDC]")
{
    hdc_t hd = hdc_new_empty();
    double d = 3.14;
    hdc_set_scalar(hd,"",(void*)(&d),HDC_DOUBLE);
    CHECK(hdc_as_double_scalar(hd,"") == d);

    hdc_t hi8 = hdc_new_empty();
    int8_t i8 = 8;
    hdc_set_scalar(hi8,"",(void*)(&i8),HDC_INT8);
    CHECK(hdc_as_int8_scalar(hi8,"") == i8);

    hdc_t hi16 = hdc_new_empty();
    int16_t i16 = 16;
    hdc_set_scalar(hi16,"",(void*)(&i16),HDC_INT16);
    CHECK(hdc_as_int16_scalar(hi16,"") == i16);

    hdc_t hi32 = hdc_new_empty();
    int32_t i32 = 32;
    hdc_set_scalar(hi32,"",(void*)(&i32),HDC_INT32);
    CHECK(hdc_as_int32_scalar(hi32,"") == i32);

    hdc_t hi64 = hdc_new_empty();
    int64_t i64 = 64;
    hdc_set_scalar(hi64,"",(void*)(&i64),HDC_INT64);
    CHECK(hdc_as_int64_scalar(hi64,"") == i64);
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
