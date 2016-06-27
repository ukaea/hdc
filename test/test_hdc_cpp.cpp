#include "hdc.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(HDC,EmptyNode) {
    HDC* h = new HDC();
    EXPECT_EQ(0,h->get_shape()[0]);
    EXPECT_EQ(1,h->get_ndim());
    EXPECT_EQ(HDC_EMPTY,h->get_type());
    EXPECT_STREQ("null",h->get_type_str().c_str());
    EXPECT_EQ(false,h->has_child("aaa"));
}

TEST(HDC,NodeManipulation) {
    HDC* tree = new HDC();
    HDC* n1 = new HDC();
    HDC* n2 = new HDC();
    tree->add_child("aaa/bbb",n1);
    EXPECT_EQ(HDC_STRUCT,tree->get_type());
    EXPECT_STREQ("hdc",tree->get_type_str().c_str());
    EXPECT_EQ(true,tree->has_child("aaa/bbb"));
    EXPECT_EQ(true,tree->has_child("aaa"));
    EXPECT_EQ(n1,tree->get("aaa/bbb"));
    EXPECT_NE(n2,tree->get("aaa/bbb"));
    // Try subtree
    HDC* sub = tree->get("aaa");
    EXPECT_EQ(true,sub->has_child("bbb"));
    EXPECT_EQ(n1,sub->get("bbb"));
    // Test set
    tree->set_child("aaa/bbb",n2);
    EXPECT_EQ(true,sub->has_child("bbb"));
    EXPECT_EQ(n2,sub->get("bbb"));
    EXPECT_NE(n1,tree->get("aaa/bbb"));
    // Test delete
    tree->delete_child("aaa/bbb");
    EXPECT_EQ(false,tree->has_child("aaa/bbb"));
    EXPECT_EQ(true,tree->has_child("aaa"));
    tree->add_child("aaa/bbb",n1);
    tree->delete_child("aaa");
    EXPECT_EQ(false,tree->has_child("aaa"));
    EXPECT_EQ(HDC_EMPTY,tree->get_type());
}


TEST(HDC,Int8DataManipulation) {
    int8_t ndim = 1;
    long shape[] = {4};
    int8_t data[] = {7,20,3,5};
    HDC* h = new HDC();
    h->set_data<int8_t>(ndim,shape,data);
    EXPECT_EQ(HDC_DYND,h->get_type());
    EXPECT_EQ(1,h->get_ndim());
    EXPECT_EQ(4,h->get_shape()[0]);
    EXPECT_STREQ("int8",h->get_type_str().c_str());
    int8_t* data2 = h->as<int8_t*>();
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 120;
    h->set_data<int8_t>(ndim,shape,data);
    data2 = h->as<int8_t*>();
    EXPECT_EQ(120,data2[3]);
}

TEST(HDC,Int32DataManipulation) {
    int8_t ndim = 1;
    long shape[] = {4};
    int32_t data[] = {777,20202020,3333,555555};
    HDC* h = new HDC();
    h->set_data<int32_t>(ndim,shape,data);
    EXPECT_EQ(HDC_DYND,h->get_type());
    EXPECT_EQ(1,h->get_ndim());
    EXPECT_EQ(4,h->get_shape()[0]);
    EXPECT_STREQ("int32",h->get_type_str().c_str());
    int32_t* data2 = h->as<int32_t*>();
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 666;
    h->set_data<int32_t>(ndim,shape,data);
    data2 = h->as<int32_t*>();
    EXPECT_EQ(666,data2[3]);
}


TEST(HDC,DoubleDataManipulation) {
    int8_t ndim = 1;
    long shape[] = {4};
    double data[] = {0.0,1000.0,1.0e-200,1.0e200};
    HDC* h = new HDC();
    h->set_data<double>(ndim,shape,data);
    EXPECT_EQ(HDC_DYND,h->get_type());
    EXPECT_EQ(1,h->get_ndim());
    EXPECT_EQ(4,h->get_shape()[0]);
    EXPECT_STREQ("float64",h->get_type_str().c_str());
    double* data2 = h->as<double*>();
    for (int i=0;i<3;i++) EXPECT_EQ(data[i],data2[i]);
    data[3] = 666.666;
    h->set_data<double>(ndim,shape,data);
    data2 = h->as<double*>();
    EXPECT_EQ(666.666,data2[3]);
}

TEST(HDC,StringDataManipulation) {
    HDC* h = new HDC();
    std::string str("lalalalala  bleble");
    h->set_data<std::string>(str);
    std::string str2 = std::string(h->as_string());
    EXPECT_STREQ(str.c_str(), str2.c_str());
}

TEST(HDC,SliceManipulation) {
    HDC* h = new HDC();
    HDC* sl = new HDC();
    HDC* sl2 = new HDC();
    h->append_slice(sl);
    h->append_slice(sl2);
    EXPECT_EQ(HDC_LIST, h->get_type());
    EXPECT_STREQ("hdc", h->get_type_str().c_str());
    EXPECT_EQ(1,h->get_ndim());
    EXPECT_EQ(2,h->get_shape()[0]);
    EXPECT_EQ(sl,h->get_slice(0));
    EXPECT_EQ(sl2,h->get_slice(1));
    HDC* sl3 = new HDC();
    h->insert_slice(1,sl3);
    EXPECT_EQ(sl3,h->get_slice(1));
    EXPECT_EQ(sl2,h->get_slice(2));
    HDC* sl4 = new HDC();
    h->set_slice(1,sl4);
    EXPECT_EQ(sl4,h->get_slice(1));
}

TEST(HDC,GetKeys) {
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