#include "gtest/gtest.h"
#include "hdc.hpp"
#include "hdc_c.h"
#include "types.h"
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
}

TEST(CHDC,EmptyArrayNode) {
    int8_t ndim = 1;
    long shape[] = {4};
    
    struct hdc_t* hi8 = hdc_new_dtype(ndim, shape, "int8");
    EXPECT_EQ(1,hdc_get_ndim(hi8));
    EXPECT_EQ(4,hdc_get_shape(hi8)[0]);
    EXPECT_EQ(HDC_DYND,hdc_get_type(hi8));
    EXPECT_STREQ("int8",hdc_get_type_str(hi8));
    
    struct hdc_t* hi32 = hdc_new_dtype(ndim, shape, "int32");
    EXPECT_EQ(1,hdc_get_ndim(hi32));
    EXPECT_EQ(4,hdc_get_shape(hi32)[0]);
    EXPECT_EQ(HDC_DYND,hdc_get_type(hi32));
    EXPECT_STREQ("int32",hdc_get_type_str(hi32));
    
    struct hdc_t* hd = hdc_new_dtype(ndim, shape, "float64");
    EXPECT_EQ(1,hdc_get_ndim(hd));
    EXPECT_EQ(4,hdc_get_shape(hd)[0]);
    EXPECT_EQ(HDC_DYND,hdc_get_type(hd));
    EXPECT_STREQ("float64",hdc_get_type_str(hd));
    
    hd = hdc_new_dtype(ndim, shape, "double");
    EXPECT_EQ(1,hdc_get_ndim(hd));
    EXPECT_EQ(4,hdc_get_shape(hd)[0]);
    EXPECT_EQ(HDC_DYND,hdc_get_type(hd));
    EXPECT_STREQ("float64",hdc_get_type_str(hd));
}


TEST(CHDC,NodeManipulation) {
    struct hdc_t* tree = hdc_new_empty();
    struct hdc_t* n1 = hdc_new_empty();
    struct hdc_t* n2 = hdc_new_empty();
    hdc_add_child(tree,"aaa/bbb",n1);
    EXPECT_EQ(HDC_STRUCT,hdc_get_type(tree));
    EXPECT_STREQ("hdc",hdc_get_type_str(tree));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa/bbb"));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa"));
    struct hdc_t* tmp = hdc_get(tree,"aaa/bbb");
    EXPECT_EQ(n1->obj,tmp->obj);
    EXPECT_NE(n2->obj,hdc_get(tree,"aaa/bbb")->obj);
    // Try subtree
    struct hdc_t* sub = hdc_get(tree,"aaa");
    EXPECT_EQ(true,hdc_has_child(sub,"bbb"));
    EXPECT_EQ(n1->obj,hdc_get(sub,"bbb")->obj);
    // Test set
    hdc_set_child(tree,"aaa/bbb",n2);
    EXPECT_EQ(true,hdc_has_child(sub,"bbb"));
    EXPECT_EQ(n2->obj,hdc_get(sub,"bbb")->obj);
    EXPECT_NE(n1->obj,hdc_get(tree,"aaa/bbb")->obj);
    // Test delete
    hdc_delete_child(tree,"aaa/bbb");
    EXPECT_EQ(false,hdc_has_child(tree,"aaa/bbb"));
    EXPECT_EQ(true,hdc_has_child(tree,"aaa"));
    hdc_add_child(tree,"aaa/bbb",n1);
    hdc_delete_child(tree,"aaa");
    EXPECT_EQ(false,hdc_has_child(tree,"aaa"));
    EXPECT_EQ(HDC_EMPTY,hdc_get_type(tree));
}

TEST(CHDC,Int8DataManipulation) {
    int8_t ndim = 1;
    long shape[] = {4};
    int8_t data[] = {7,20,3,5};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int8(h,ndim,shape,(void*)data);
    EXPECT_EQ(HDC_DYND,hdc_get_type(h));
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
    int8_t ndim = 1;
    long shape[] = {4};
    int32_t data[] = {777,20202020,3333,555555};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_int32(h,ndim,shape,(void*)data);
    EXPECT_EQ(HDC_DYND,hdc_get_type(h));
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

TEST(CHDC,DoubleDataManipulation) {
    int8_t ndim = 1;
    long shape[] = {4};
    double data[] = {0.0,1000.0,1.0e-200,1.0e200};
    struct hdc_t* h = hdc_new_empty();
    hdc_set_double(h,ndim,shape,data);
    EXPECT_EQ(HDC_DYND,hdc_get_type(h));
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
    EXPECT_STREQ("hdc", hdc_get_type_str(h));
    EXPECT_EQ(1,hdc_get_ndim(h));
    EXPECT_EQ(2,hdc_get_shape(h)[0]);
    EXPECT_EQ(sl->obj,hdc_get_slice(h,0)->obj);
    EXPECT_EQ(sl2->obj,hdc_get_slice(h,1)->obj);
    struct hdc_t* sl3 = hdc_new_empty();
    hdc_insert_slice(h,1,sl3);
    EXPECT_EQ(sl3->obj,hdc_get_slice(h,1)->obj);
    EXPECT_EQ(sl2->obj,hdc_get_slice(h,2)->obj);
    struct hdc_t* sl4 = hdc_new_empty();
    hdc_set_slice(h,1,sl4);
    EXPECT_EQ(sl4->obj,hdc_get_slice(h,1)->obj);
}

/*
TEST(CHDC,GetKeys) {
    HDC* list = new HDC();
    list->set_type(HDC_LIST);
    EXPECT_EQ(true,list->keys().empty());
    HDC* val = new HDC();
    list->set_type(HDC_DYND);
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


TEST(HDC,JsonComplete) {
    // Prepare tree
    int8_t ndim = 1;
    long shape[] = {4};
    double data_double[] = {0.0,1000.0,1.0e-200,1.0e200};
    int32_t data_int[] = {777,20202020,3333,555555};
    HDC* tree = new HDC();
    tree->set_data<double>("aaa/bbb/double",ndim,shape,data_double);
    tree->set_data<int>("aaa/bbb/int",ndim,shape,data_int);
    tree->add_child("aaa/bbb/empty", new HDC());
    HDC* list = new HDC();
    for (int i=0;i<5;i++) list->append_slice(new HDC());
    tree->add_child("aaa/list", list);
    tree->set_data("aaa/string","Lorem ipsum dolor sit amet, consectetuer adipiscing elit.");
    
    // Save JSON
    tree->to_json("tree.txt");
    
    // Load JSON
    HDC* tree2 = from_json("tree.txt");
    tree2->to_json("tree2.txt");
    
    // test tree
    HDC* s = tree2->get("aaa/bbb/double");
    
    // Test double
    EXPECT_EQ(1,s->get_ndim());
    EXPECT_EQ(4,s->get_shape()[0]);
    EXPECT_EQ(HDC_DYND,s->get_type());
    EXPECT_STREQ(tree->get("aaa/bbb/double")->get_type_str().c_str(), tree2->get("aaa/bbb/double")->get_type_str().c_str());
    double* data_double_in = s->as<double*>();
    for (int i=0;i < shape[0];i++) EXPECT_EQ(data_double[i],data_double_in[i]);
    
    // Test int
    s = tree2->get("aaa/bbb/int");
    EXPECT_EQ(1,s->get_ndim());
    EXPECT_EQ(4,s->get_shape()[0]);
    EXPECT_EQ(HDC_DYND,s->get_type());
    EXPECT_STREQ(tree->get("aaa/bbb/int")->get_type_str().c_str(), tree2->get("aaa/bbb/int")->get_type_str().c_str());
    int32_t* data_int_in = s->as<int32_t*>();
    for (int i=0;i < shape[0];i++) EXPECT_EQ(data_int[i],data_int_in[i]);
    
    // Test empty
    EXPECT_EQ(HDC_EMPTY,tree2->get("aaa/bbb/empty")->get_type());
    
    // Test list
    s = tree2->get("aaa/list");
    EXPECT_EQ(1,s->get_ndim());
    EXPECT_EQ(5,s->get_shape()[0]);
    EXPECT_EQ(HDC_LIST,s->get_type());
    EXPECT_STREQ(tree->get("aaa/list")->get_type_str().c_str(), tree2->get("aaa/list")->get_type_str().c_str());
    for (int i=0;i<5;i++) EXPECT_EQ(HDC_EMPTY,s->get_slice(i)->get_type());
    
    // Test string
    EXPECT_STREQ(tree->get("aaa/string")->as_string().c_str(), tree2->get("aaa/string")->as_string().c_str());
    
}
*/