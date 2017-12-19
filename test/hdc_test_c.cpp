#include "gtest/gtest.h"
#include "hdc.hpp"
#include "hdc_c.h"
//#include "types.h"
struct hdc_t {
    void* obj;
};

TEST(CHDC,EmptyNode) {
    struct hdc_t* h = hdc_new_empty();
    EXPECT_EQ(0,hdc_get_shape(h)[0]);
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(HDC_EMPTY,hdc_get_type(h));
    EXPECT_STREQ("null",hdc_get_type_str(h));
    EXPECT_EQ(false,hdc_has_child(h,"aaa"));
    hdc_delete(h);
}

TEST(CHDC,EmptyArrayNode) {
    int ndim = 1;
    size_t shape[] = {4};
    
    struct hdc_t* hi8 = hdc_new_dtype(ndim, shape, INT8_ID);
    EXPECT_EQ(1,hdc_get_ndim(hi8));
    EXPECT_EQ(4,hdc_get_shape(hi8)[0]);
    EXPECT_EQ(INT8_ID,hdc_get_type(hi8));
    EXPECT_STREQ("int8",hdc_get_type_str(hi8));
    hdc_delete(hi8);
    
    struct hdc_t* hi32 = hdc_new_dtype(ndim, shape, INT32_ID);
    EXPECT_EQ(1,hdc_get_ndim(hi32));
    EXPECT_EQ(4,hdc_get_shape(hi32)[0]);
    EXPECT_EQ(INT32_ID,hdc_get_type(hi32));
    EXPECT_STREQ("int32",hdc_get_type_str(hi32));
    hdc_delete(hi32);
    
    struct hdc_t* hi64 = hdc_new_dtype(ndim, shape, INT64_ID);
    EXPECT_EQ(1,hdc_get_ndim(hi64));
    EXPECT_EQ(4,hdc_get_shape(hi64)[0]);
    EXPECT_EQ(INT64_ID,hdc_get_type(hi64));
    EXPECT_STREQ("int64",hdc_get_type_str(hi64));
    hdc_delete(hi64);
    
    struct hdc_t* hd = hdc_new_dtype(ndim, shape, DOUBLE_ID);
    EXPECT_EQ(1,hdc_get_ndim(hd));
    EXPECT_EQ(4,hdc_get_shape(hd)[0]);
    EXPECT_EQ(DOUBLE_ID,hdc_get_type(hd));
    EXPECT_STREQ("float64",hdc_get_type_str(hd));
    hdc_delete(hd);
    
    hd = hdc_new_dtype(ndim, shape, DOUBLE_ID);
    EXPECT_EQ(1,hdc_get_ndim(hd));
    EXPECT_EQ(4,hdc_get_shape(hd)[0]);
    EXPECT_EQ(DOUBLE_ID,hdc_get_type(hd));
    EXPECT_STREQ("float64",hdc_get_type_str(hd));
    hdc_delete(hd);
}


TEST(CHDC,NodeManipulation) {
    struct hdc_t* tree = hdc_new_empty();
    struct hdc_t* n1 = hdc_new_empty();
    struct hdc_t* n2 = hdc_new_empty();
    hdc_add_child(tree,"aaa/bbb",n1);
    EXPECT_EQ(HDC_STRUCT,hdc_get_type(tree));
    EXPECT_STREQ("struct",hdc_get_type_str(tree));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa/bbb"));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa"));
    struct hdc_t* tmp = hdc_get(tree,"aaa/bbb");
    EXPECT_STREQ(hdc_get_uuid(n1),hdc_get_uuid(tmp));
    EXPECT_STRNE(hdc_get_uuid(n2),hdc_get_uuid(hdc_get(tree,"aaa/bbb")));
    // Try subtree
    struct hdc_t* sub = hdc_get(tree,"aaa");
    EXPECT_EQ(true,hdc_has_child(sub,"bbb"));
    EXPECT_STREQ(hdc_get_uuid(n1),hdc_get_uuid(hdc_get(sub,"bbb")));
    // Test set
    hdc_set_child(tree,"aaa/bbb",n2);
    EXPECT_EQ(true,hdc_has_child(sub,"bbb"));
    EXPECT_STREQ(hdc_get_uuid(n2),hdc_get_uuid(hdc_get(sub,"bbb")));
    EXPECT_STRNE(hdc_get_uuid(n1),hdc_get_uuid(hdc_get(tree,"aaa/bbb")));
    // Test delete
    hdc_delete_child(tree,"aaa/bbb");
    EXPECT_EQ(false,hdc_has_child(tree,"aaa/bbb"));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa"));
    hdc_add_child(tree,"aaa/bbb",n1);
    hdc_delete_child(tree,"aaa");
    EXPECT_EQ(false,hdc_has_child(tree,"aaa"));
    hdc_delete(tree);
    hdc_delete(n1);
    hdc_delete(sub);
}

TEST(CHDC,Int8DataManipulation) {
    int ndim = 1;
    size_t shape[] = {4};
    int8_t data[] = {7,20,3,5};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int8(h,ndim,shape,(void*)data);
    EXPECT_EQ(INT8_ID,hdc_get_type(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(4,hdc_get_shape(h)[0]);
    EXPECT_STREQ("int8",hdc_get_type_str(h));
    int8_t* data2 = hdc_as_int8_1d(h);
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 120;
    hdc_set_int8(h,ndim,shape,(void*)data);
    data2 = hdc_as_int8_1d(h);
    EXPECT_EQ(120,data2[3]);
}

TEST(CHDC,Int32DataManipulation) {
    int ndim = 1;
    size_t shape[] = {4};
    int32_t data[] = {777,20202020,3333,555555};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int32(h,ndim,shape,(void*)data);
    EXPECT_EQ(INT32_ID,hdc_get_type(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(4,hdc_get_shape(h)[0]);
    EXPECT_STREQ("int32",hdc_get_type_str(h));
    int32_t* data2 = hdc_as_int32_1d(h);
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 666;
    hdc_set_int32(h,ndim,shape,(void*)data);
    data2 = hdc_as_int32_1d(h);
    EXPECT_EQ(666,data2[3]);
}

TEST(CHDC,Int64DataManipulation) {
    int ndim = 1;
    size_t shape[] = {4};
    int64_t data[] = {777,20202020,3333,555555};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int64(h,ndim,shape,(void*)data);
    EXPECT_EQ(INT64_ID,hdc_get_type(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(4,hdc_get_shape(h)[0]);
    EXPECT_STREQ("int64",hdc_get_type_str(h));
    int64_t* data2 = hdc_as_int64_1d(h);
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 666;
    hdc_set_int64(h,ndim,shape,(void*)data);
    data2 = hdc_as_int64_1d(h);
    EXPECT_EQ(666,data2[3]);
}


TEST(CHDC,DoubleDataManipulation) {
    int ndim = 1;
    size_t shape[] = {4};
    double data[] = {0.0,1000.0,1.0e-200,1.0e200};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_double(h,ndim,shape,data);
    EXPECT_EQ(DOUBLE_ID,hdc_get_type(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(4,hdc_get_shape(h)[0]);
    EXPECT_STREQ("float64",hdc_get_type_str(h));
    double* data2 = hdc_as_double_1d(h);
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 666.666;
    hdc_set_double(h,ndim,shape,data);
    data2 = hdc_as_double_1d(h);
    EXPECT_EQ(666.666,data2[3]);
}

TEST(CHDC,StringDataManipulation) {
    struct hdc_t* h = hdc_new_empty();
    char* str = "lalalalala  bleble";
    hdc_set_string(h,str);
    const char* str2 = hdc_as_string(h);
    EXPECT_STREQ(str, str2);
}

TEST(CHDC,SliceManipulation) {
    struct hdc_t* h = hdc_new_empty();
    struct hdc_t* sl = hdc_new_empty();
    struct hdc_t* sl2 = hdc_new_empty();
    hdc_append_slice(h,sl);
    hdc_append_slice(h,sl2);
    EXPECT_EQ(HDC_LIST, hdc_get_type(h));
    EXPECT_STREQ("list", hdc_get_type_str(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(2,hdc_get_shape(h)[0]);
    EXPECT_STREQ(hdc_get_uuid(sl),hdc_get_uuid(hdc_get_slice(h,0)));
    EXPECT_STREQ(hdc_get_uuid(sl2),hdc_get_uuid(hdc_get_slice(h,1)));
    struct hdc_t* sl3 = hdc_new_empty();
    hdc_insert_slice(h,1,sl3);
    EXPECT_STREQ(hdc_get_uuid(sl3),hdc_get_uuid(hdc_get_slice(h,1)));
    EXPECT_STREQ(hdc_get_uuid(sl2),hdc_get_uuid(hdc_get_slice(h,2)));
    struct hdc_t* sl4 = hdc_new_empty();
    hdc_set_slice(h,1,sl4);
    EXPECT_STREQ(hdc_get_uuid(sl4),hdc_get_uuid(hdc_get_slice(h,1)));
}

//-----------------------------------------------------------------------------------------
/*
TEST(CHDC,GetKeys) {
    HDC* list = new HDC();
    list->set_type(HDC_LIST);
    EXPECT_EQ(true,list->keys().empty());
    HDC* val = new HDC();
    EXPECT_EQ(true,val->keys().empty());
    HDC* empty = new HDC();
    EXPECT_EQ(true,empty->keys().empty());
    HDC* tree = new HDC();
    tree->add_child("aaa",new HDC());
    tree->add_child("bbb",new HDC());
    tree->add_child("ccc/sss",new HDC());
    EXPECT_EQ(3u,tree->keys().size());
    vector<string> keys = tree->keys();
    for (size_t i=0;i<keys.size();i++) EXPECT_EQ(true,tree->has_child(keys[i]));
}
*/
