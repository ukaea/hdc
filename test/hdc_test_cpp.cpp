#include "catch.hpp"
#include "hdc.hpp"
#include <boost/filesystem.hpp>


bool in_vector(std::string str, const std::vector<std::string>& vector)
{
    return (std::find(vector.begin(), vector.end(), str) != vector.end());
}

const std::string make_tmp_name(const std::string& suffix = "h5")
{
    boost::filesystem::path temp = boost::filesystem::unique_path();
    temp += std::string(".")+suffix;
    const std::string tempstr = temp.native();
    return tempstr;
}

#define PREPARE_TREE()                                                                              \
    std::vector<size_t> shape = {4};                                                                \
    double data_double[] = {0.0,1000.0,1.0e-200,1.0e200};                                           \
    int32_t data_int[] = {777,20202020,3333,555555};                                                \
    HDC tree;                                                                                       \
    HDC scalar;                                                                                     \
    scalar.set_data(333.333);                                                                       \
    tree.add_child("aaa/bbb/_scalar", scalar);                                                      \
    tree["aaa/bbb/double"].set_data<double>(shape,data_double);                                     \
    tree["aaa/bbb/double2"].set_data<double>(shape,data_double);                                    \
    tree["aaa/bbb/int"].set_data<int>(shape,data_int);                                              \
    HDC ch;                                                                                         \
    tree.add_child("aaa/bbb/empty", ch);                                                            \
    HDC list;                                                                                       \
    HDC lch;                                                                                        \
    for (int i=0;i<5;i++) {                                                                         \
        HDC lch;                                                                                    \
        list.append(lch);                                                                           \
    }                                                                                               \
    tree.add_child("aaa/list", list);                                                               \
    HDC str("Lorem ipsum dolor sit amet, consectetuer adipiscing elit.");                           \
    tree.add_child("aaa/string",str);                                                               \


TEST_CASE("StringParsing", "[HDCUtils]")
{
    std::vector<bool> expected_is_num = { false, true, false, true, true, false, false };
    auto vs = split_no_brackets("flux/3444/lalala/5/4/l///l");
    CHECK(7 == vs.size());
    int i = 0;
    for (auto item : vs) {
        CHECK((item.type() == typeid(size_t)) == expected_is_num[i]);
        i++;
    }
    auto vs2 = split("flux[3444]/lalala[5][4]l///l");
    CHECK(7 == vs2.size());
    i = 0;
    for (auto item : vs2) {
        CHECK((item.type() == typeid(size_t)) == expected_is_num[i]);
        i++;
    }
}

TEST_CASE("GetPlugins","[HDC]")
{
    //HDC::search_plugins(); // This is called in main.cpp
    auto plugins = HDC::get_available_plugins();
    // "umap" should be always present
    CHECK(in_vector("umap",plugins));
#ifdef _USE_MDBM
    // "mdbm" has been built and  should be found
    CHECK(in_vector("mdbm",plugins));
#endif // _USE_MDBM
}

TEST_CASE("EmptyNode", "[HDC]")
{
    HDC h = HDC();
    CHECK(0 == h.get_shape()[0]);
    CHECK(1 == h.get_rank());
    CHECK(HDC_EMPTY == h.get_type());
    CHECK(strcmp("null", h.get_type_str()) == 0);
    CHECK(false == h.exists("aaa"));
    CHECK(h.get_datasize() == 0);
    CHECK(h.get_size() == sizeof(hdc_header_t));
    CHECK(h.get_flags() == HDCDefault);
    CHECK(h.is_external() == false);
    CHECK(h.is_readonly() == false);
    CHECK(strcmp(h.as_cstring(),"null\n") == 0);
    CHECK(strcmp(h.as_string().c_str(),"null\n") == 0);
}


TEST_CASE("EmptyNodePtr", "[HDC]")
{
    HDC h;
    CHECK(0 == h.get_shape()[0]);
    CHECK(1 == h.get_rank());
    CHECK(HDC_EMPTY == h.get_type());
    CHECK(strcmp("null", h.get_type_str()) == 0);
    CHECK(false == h.exists("aaa"));
}

TEST_CASE("EmptyArrayNode", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    HDC hi8(shape, HDC_INT8);
    CHECK(1 == hi8.get_rank());
    CHECK(4 == hi8.get_shape()[0]);
    CHECK(HDC_INT8 == hi8.get_type());
    CHECK(strcmp("int8", hi8.get_type_str()) == 0);

    HDC hi32(shape, HDC_INT32);
    CHECK(1 == hi32.get_rank());
    CHECK(4 == hi32.get_shape()[0]);
    CHECK(HDC_INT32 == hi32.get_type());
    CHECK(strcmp("int32", hi32.get_type_str()) == 0);

    HDC hd = HDC(shape, HDC_DOUBLE);
    CHECK(1 == hd.get_rank());
    CHECK(4 == hd.get_shape()[0]);
    CHECK(HDC_DOUBLE == hd.get_type());
    CHECK(strcmp("float64", hd.get_type_str()) == 0);

    //check throws if more dimensions
    std::vector<size_t> sh(HDC_MAX_DIMS+1);
    for (size_t i=0; i<HDC_MAX_DIMS+1;i++) sh[i] = 1;
    CHECK_THROWS(HDC{sh,HDC_DOUBLE});
}

TEST_CASE("NodeManipulation", "[HDC]")
{
    HDC tree;
    HDC n1;
    HDC n2;
    // Try different
    CHECK(strcmp(n1.get_uuid().c_str(), tree.get_uuid().c_str()) != 0);
    CHECK(strcmp(n2.get_uuid().c_str(), tree.get_uuid().c_str()) != 0);
    CHECK(strcmp(n1.get_uuid().c_str(), n2.get_uuid().c_str()) != 0);

    // Try add
    tree.add_child("aaa/bbb", n1);
    CHECK(HDC_STRUCT == tree.get_type());
    CHECK(strcmp("struct", tree.get_type_str()) == 0);
    CHECK(true == tree.exists("aaa/bbb"));
    CHECK(true == tree.exists("aaa"));
    CHECK(strcmp(n1.get_uuid().c_str(), tree.get("aaa/bbb").get_uuid().c_str()) == 0);
    CHECK_THROWS(tree.get("not_here"));
    CHECK(strcmp(n2.get_uuid().c_str(), tree.get("aaa/bbb").get_uuid().c_str()) != 0);
    // Try add and get index
    HDC n3 = HDC();
    tree.add_child("aaa/list[0]/ddd", n3);
    CHECK(tree.exists("aaa/list[0]/ddd") == true);
    HDC dummy1, dummy2, dummy3;
    tree.add_child("aaa/list[1]", dummy1);
    tree.add_child("aaa/list[0]/eee", dummy2);
    HDC list = tree.get("aaa/list");
    CHECK(HDC_LIST == list.get_type());
    HDC h = tree.get("aaa/list[0]/ddd");
    CHECK(true == tree.exists("aaa/list[0]/ddd"));
    CHECK(true == tree.exists("aaa/list[0]/eee"));
    tree.get("aaa/list").get(0).add_child("kkk", dummy3); // Remove this - unnecessary
    CHECK(2LU == tree.get("aaa/list").get_shape()[0]);
    CHECK(true == tree.get("aaa/list").get(0).exists("ddd"));
    CHECK(true == tree.get("aaa/list").get(0).exists("eee"));
    // Try subtree
    HDC sub = tree.get("aaa");
    CHECK(true == sub.exists("bbb"));
    CHECK(strcmp(n1.get_uuid().c_str(), sub.get("bbb").get_uuid().c_str()) == 0);
    // Test set
    tree.set_child("aaa/bbb", n2);
    CHECK(true == sub.exists("bbb"));
    CHECK(strcmp(n2.get_uuid().c_str(), sub.get("bbb").get_uuid().c_str()) == 0);
    CHECK(strcmp(n1.get_uuid().c_str(), tree.get("aaa/bbb").get_uuid().c_str()) != 0);
    // Test delete
    tree.delete_child("aaa/bbb");
    CHECK(false == tree.exists("aaa/bbb"));
    CHECK(true == tree.exists("aaa"));
    tree.add_child("aaa/bbb", n1);
    tree.delete_child("aaa");
    CHECK(false == tree.exists("aaa"));
    // Try add to empty path
    HDC t;
    HDC d;
    CHECK_THROWS_AS(t.add_child("", d), HDCException);

    // check set_type wher already allocated
    HDC a(HDC_NODE_SIZE_DEFAULT);
    a.set_type(HDC_STRUCT);
    CHECK(a.get_type() == HDC_STRUCT);
}


TEST_CASE("ListManipulation", "[HDC]")
{
    HDC h;
    HDC k;
    for (size_t i = 0; i < 5; i++) {
        HDC ch("data" + std::to_string(i));
        k.append(ch);
    }
    h.add_child("k", k);

    HDC l;
    for (size_t i = 0; i < 5; i++) {
        HDC ch("data" + std::to_string(i * 10));
        l.append(ch);
    }
    h.add_child("l", l);

    CHECK(h.get("k[1]").as_string() == "data1");
    CHECK(h.get("l[1]").as_string() == "data10");
}

TEST_CASE("Int8DataManipulation", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    int8_t data[] = { 7, 20, 3, 5 };
    HDC h = HDC();
    h.set_data(shape, data);
    CHECK(HDC_INT8 == h.get_type());
    CHECK(1 == h.get_rank());
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_itemsize() == sizeof(int8_t));
    CHECK(strcmp("int8", h.get_type_str()) == 0);
    int8_t* data2 = h.as<int8_t>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    // This is no longer possible as for some storages data have to be copied (all for now, maybe we can enable specifically for umap storage in future)
    // All further occurencies will be removed.
    data[3] = 120;
    h.set_data(shape, data);
    data2 = h.as<int8_t>();
    CHECK(120 == data2[3]);
}

TEST_CASE("Int16DataManipulation", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    int16_t data[] = { 777, 30000, 3333, 22222 };
    HDC h;
    h.set_data<int16_t>(shape, data);
    CHECK(HDC_INT16 == h.get_type());
    CHECK(1 == h.get_rank());
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_itemsize() == sizeof(int16_t));
    CHECK(strcmp("int16", h.get_type_str()) == 0);
    int16_t* data2 = h.as<int16_t>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);

}

TEST_CASE("Int32DataManipulation", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    int32_t data[] = { 777, 20202020, 3333, 555555 };
    HDC h;
    h.set_data<int32_t>(shape, data);
    CHECK(HDC_INT32 == h.get_type());
    CHECK(1 == h.get_rank());
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_itemsize() == sizeof(int32_t));
    CHECK(strcmp("int32", h.get_type_str()) == 0);
    int32_t* data2 = h.as<int32_t>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);

}

TEST_CASE("Int64DataManipulation", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    int64_t data[] = { 777, 20202020, 3333, 2000000000 };
    HDC h;
    h.set_data<int64_t>(shape, data);
    CHECK(HDC_INT64 == h.get_type());
    CHECK(1 == h.get_rank());
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_itemsize() == sizeof(int64_t));
    CHECK(strcmp("int64", h.get_type_str()) == 0);
    int64_t* data2 = h.as<int64_t>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);

}


TEST_CASE("DoubleDataManipulation", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    HDC h;
    h.set_data<double>(shape, data);
    CHECK(HDC_DOUBLE == h.get_type());
    CHECK(1 == h.get_rank());
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_itemsize() == sizeof(double));
    CHECK(strcmp("float64", h.get_type_str()) == 0);
    double* data2 = h.as<double>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
}

TEST_CASE("SetExternal", "[HDC]")
{
    HDC node, external, e2, e3;
    int64_t array_in[4] = { 7, 2, 3, 4 };
    std::vector<size_t> shape = {4};
    node.set_data(shape,array_in);
    external.set_external(shape, array_in);
    int64_t* array_out = external.as<int64_t>();
    CHECK(*array_in == *array_out);
    CHECK(*array_in == *(int64_t*)(external.as_void_ptr()));
    CHECK(external.is_external() == true);
    auto external_str = external.to_json_string();
    auto node_str = node.to_json_string();
    CHECK(strcmp(external_str.c_str(),node_str.c_str()) == 0);

    //update
    int64_t array_in2[4] = { 70, 20, 30, 40 };
    external.set_external(shape, array_in2);
    array_out = external.as<int64_t>();
    CHECK(*array_in2 == *array_out);

    e2.set_external({1},array_in);
    array_out = e2.as<int64_t>();
    CHECK(*array_in == *array_out);

    hdc_data_t data;
    data.data = (char*)array_in;
    data.rank = 1;
    data.shape[0] = 4;
    data.flags = HDCDefault;
    data.type = HDC_INT64;
    e3.set_external(data);
    array_out = e3.as<int64_t>();
    CHECK(*array_in == *array_out);

    HDC e4 = HDC::make_external(data);
    array_out = e3.as<int64_t>();
    CHECK(*array_in == *array_out);

    //update
    hdc_data_t data2;
    memcpy(&data2,&data,sizeof(hdc_data_t));
    data2.data = (char*)array_in2;
    e3.set_external(data2);
    array_out = e3.as<int64_t>();
    CHECK(*array_in2 == *array_out);

    //constructor
    data2.flags |= HDCExternal;
    HDC e5(data2);
    array_out = e5.as<int64_t>();
    CHECK(*array_in2 == *array_out);

}

TEST_CASE("as_vector_int8", "[HDC]") {
    int8_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<int8_t>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_uint8", "[HDC]") {
    uint8_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<uint8_t>({4},array_in);
    auto vector_out = h.as_vector<uint64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<uint32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<uint16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<uint8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_int16", "[HDC]") {
    int16_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<int16_t>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_uint16", "[HDC]") {
    uint16_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<uint16_t>({4},array_in);
    auto vector_out = h.as_vector<uint64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<uint32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<uint16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<uint8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_int32", "[HDC]") {
    int32_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<int32_t>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_uint32", "[HDC]") {
    uint32_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<uint32_t>({4},array_in);
    auto vector_out = h.as_vector<uint64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<uint32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<uint16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<uint8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_int64", "[HDC]") {
    int64_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<int64_t>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_uint64", "[HDC]") {
    uint64_t array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<uint64_t>({4},array_in);
    auto vector_out = h.as_vector<uint64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<uint32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<uint16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<uint8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_float", "[HDC]") {
    float array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<float>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_double", "[HDC]") {
    double array_in[4] = { 7, 2, 3, 4 };
    HDC h;
    h.set_data<double>({4},array_in);
    auto vector_out = h.as_vector<int64_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector_out[i]);
    auto vector32 = h.as_vector<int32_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector32[i]);
    auto vector16 = h.as_vector<int16_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector16[i]);
    auto vector8 = h.as_vector<int8_t>();
    for (size_t i=0; i<4; i++) CHECK(array_in[i] == vector8[i]);
    auto vectord = h.as_vector<double>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<double>(array_in[i]) == vectord[i]);
    auto vectorf = h.as_vector<float>();
    for (size_t i=0; i<4; i++) CHECK(static_cast<float>(array_in[i]) == vectorf[i]);
}

TEST_CASE("as_vector_unknown", "[HDC]") {
    HDC h,n;
    h.add_child("aaa",n);
    CHECK_THROWS(h.as_vector<int8_t>());
}

TEST_CASE("StringDataManipulation", "[HDC]")
{
    HDC h;
    std::string str("lalalalala  bleble");
    h.set_string(str);
    std::string str2 = std::string(h.as_string());
    CHECK(strcmp(str.c_str(), str2.c_str()) == 0);
    CHECK(strcmp("string", h.get_type_str()) == 0);
}

TEST_CASE("GetStrides", "[HDC]")
{
    int32_t array3d[24] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
    std::vector<size_t> shape3d = { 2, 3, 4 };
    HDC threed_f;
    threed_f.set_data<int32_t>(shape3d, array3d, HDCFortranOrder);
    auto strides_f = threed_f.get_strides();
    CHECK(4 == strides_f[0]);
    CHECK(8 == strides_f[1]);
    CHECK(24 == strides_f[2]);
    HDC threed_c;
    threed_c.set_data<int32_t>(shape3d, array3d);
    auto strides_c = threed_c.get_strides();
    CHECK(48 == strides_c[0]);
    CHECK(16 == strides_c[1]);
    CHECK(4 == strides_c[2]);
}

TEST_CASE("SliceManipulation", "[HDC]")
{
    HDC h;
    HDC sl;
    sl.set_string("1");
    HDC sl2;
    sl2.set_string("2");
    h.set_type(HDC_LIST);
    h.append(sl);
    h.append(sl2);
    CHECK(HDC_LIST == h.get_type());
    CHECK(strcmp("list", h.get_type_str()) == 0);
    CHECK(1 == h.get_rank());
    CHECK(2 == h.get_shape()[0]);
    CHECK(strcmp("1", h[0].as_cstring()) == 0);
    CHECK(strcmp("2", h[1].as_cstring()) == 0);
    HDC sl3;
    sl3.set_string("3");
    h.insert(1, sl3);
    auto keys = h.keys();
    CHECK(strcmp("3", h[1].as_cstring()) == 0);
    CHECK(strcmp("2", h[2].as_cstring()) == 0);
    HDC sl4;
    sl4.set_string("4");
    h.set_child(1, sl4);
    CHECK(strcmp("4", h[1].as_cstring()) == 0);
    HDC n;
    n.set_type(HDC_LIST);
    HDC ch;
    n.insert(10, ch);
    CHECK(11 == n.get_shape()[0]);
    CHECK(HDC_LIST == n.get_type());
    for (int i = 0; i < 11; i++) CHECK(HDC_EMPTY == n.get(i).get_type());
    //Check exception on assiged data
    HDC h2 = HDC::make_scalar(8.8);
    CHECK_THROWS(h2.insert(0,sl3));
}

TEST_CASE("GetKeys", "[HDC]")
{
    HDC list;
    list.set_type(HDC_LIST);
    CHECK(true == list.keys().empty());
    HDC val;
    CHECK(true == val.keys().empty());
    HDC empty;
    CHECK(true == empty.keys().empty());
    HDC tree;
    HDC h1,h2,h3;
    tree.add_child("aaa", h1);
    tree.add_child("bbb", h2);
    tree.add_child("ccc/sss", h3);
    CHECK(3u == tree.keys().size());
    vector<string> keys = tree.keys();
    for (size_t i = 0; i < keys.size(); i++) CHECK(true == tree.exists(keys[i]));
}

TEST_CASE("BracketOperators", "[HDC]")
{
    HDC tree;
    tree["aaa/bbb/ccc"] = HDC();
    CHECK(tree.get_type() == HDC_STRUCT);
    CHECK(tree.exists("aaa/bbb/ccc"));
    CHECK(tree["aaa/bbb/ccc"].get_type() == HDC_EMPTY);

    int8_t i8 = 8;
    int16_t i16 = 16;
    int32_t i32 = 32;
    int64_t i64 = 64;
    float f = 3.14;
    double d = 3.141592;
    std::string str = "test";
    tree["i8"] = i8;
    tree["i16"] = i16;
    tree["i32"] = i32;
    tree["i64"] = i64;
    tree["f"] = f;
    tree["d"] = d;
    tree["str"] = str;
    tree["empty"] = HDC();
    CHECK(tree.exists("i8"));
    CHECK(tree.exists("i16"));
    CHECK(tree.exists("i32"));
    CHECK(tree.exists("i64"));
    CHECK(tree.exists("f"));
    CHECK(tree.exists("d"));
    CHECK(tree.exists("str"));
    CHECK(tree.exists("empty"));
    CHECK(tree["i8"].get_type() == HDC_INT8);
    CHECK(tree["i16"].get_type() == HDC_INT16);
    CHECK(tree["i32"].get_type() == HDC_INT32);
    CHECK(tree["i64"].get_type() == HDC_INT64);
    CHECK(tree["f"].get_type() == HDC_FLOAT);
    CHECK(tree["d"].get_type() == HDC_DOUBLE);
    CHECK(tree["str"].get_type() == HDC_STRING);
    CHECK(tree["empty"].get_type() == HDC_EMPTY);
    CHECK(tree["i8"].as_scalar<int8_t>() == i8);
    CHECK(tree["i16"].as_scalar<int16_t>() == i16);
    CHECK(tree["i32"].as_scalar<int32_t>() == i32);
    CHECK(tree["i64"].as_scalar<int64_t>() == i64);
    CHECK(tree["f"].as_scalar<float>() == f);
    CHECK(tree["d"].as_scalar<double>() == d);
    CHECK(strcmp(tree["str"].as_cstring(),str.c_str()) == 0);
    //test replace
    tree["i8"] = static_cast<int8_t>(9);
    CHECK(9 == tree["i8"].as_scalar<int8_t>());

    HDC list;
    list[0] = HDC();
    list[1] = i8;
    CHECK(list[0].get_type() == HDC_EMPTY);
    CHECK(list[1].as_scalar<int8_t>() == i8);
    // test replace
    list[1] = i32;
    CHECK(list[0].get_type() == HDC_EMPTY);
    CHECK(list[1].as_scalar<int32_t>() == i32);
}


TEST_CASE("JsonComplete", "[HDC]")
{
    PREPARE_TREE()
    // create temporary file name
    auto fname = make_tmp_name("txt");
    auto fname2 = make_tmp_name("txt");
    // Save JSON
    tree.to_json(fname);
    // Load JSON
    HDC tree2 = HDC::from_json(fname);
    tree2.to_json(fname2);
    // test tree
    HDC s = tree2.get("aaa/bbb/double");
    // Test double
    CHECK(1 == s.get_rank());
    CHECK(4 == s.get_shape()[0]);
    CHECK(HDC_DOUBLE == s.get_type());
    CHECK(strcmp(tree.get("aaa/bbb/double").get_type_str(), s.get_type_str()) == 0);
    double* data_double_in = s.as<double>();
    for (size_t i = 0; i < shape[0]; i++) CHECK(data_double[i] == data_double_in[i]);
    // Test int
    s = tree2.get("aaa/bbb/int");
    CHECK(1 == s.get_rank());
    CHECK(4 == s.get_shape()[0]);
    CHECK(HDC_INT32 == s.get_type());
    CHECK(strcmp(tree.get("aaa/bbb/int").get_type_str(), tree2.get("aaa/bbb/int").get_type_str()) == 0);
    int32_t* data_int_in = s.as<int32_t>();
    for (size_t i = 0; i < shape[0]; i++) CHECK(data_int[i] == data_int_in[i]);
    // Test empty
    CHECK(HDC_EMPTY == tree2["aaa/bbb/empty"].get_type());
    // Test list
    s = tree2.get("aaa/list");
    CHECK(1 == s.get_rank());
    CHECK(5 == s.get_shape()[0]);
    CHECK(HDC_LIST == s.get_type());
    CHECK(strcmp(tree.get("aaa/list").get_type_str(), tree2.get("aaa/list").get_type_str()) == 0);
    for (int i = 0; i < 5; i++) CHECK(HDC_EMPTY == s.get(i).get_type());
    // Test string
    CHECK(strcmp(tree.get("aaa/string").as_cstring(), tree2.get("aaa/string").as_cstring()) == 0);
    auto __path = std::string("json://") + fname2;
    HDC j = HDC::load(__path,"aaa/string");
    CHECK(strcmp(j.as_cstring(), "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.") == 0);
    if(remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
    if(remove(fname2.c_str()) != 0) std::cerr << "Error removing file " << fname2 << std::endl;
}

TEST_CASE("Copy", "[HDC]")
{
    PREPARE_TREE()
    HDC _copy = tree.copy();
    auto tree_dump = tree.to_json_string();
    auto copy_dump = _copy.to_json_string();
    CHECK(strcmp(tree_dump.c_str(), copy_dump.c_str()) == 0);
    // Check also that UUIDs are the same - this behaviour can be changed later
    CHECK(strcmp(tree["aaa/bbb/double"].get_uuid().c_str(), _copy["aaa/bbb/double"].get_uuid().c_str()) != 0);
}

TEST_CASE("BufferGrowArray", "[HDC]")
{
    std::vector<size_t> shape = { 4 };
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    HDC h;
    h.set_data<double>(shape, data);
    h.grow(4096);
    double* data2 = h.as<double>();
    CHECK(h.get_datasize() == 4*sizeof(double) + 4096);
    CHECK(4 == h.get_shape()[0]);
    CHECK(h.get_type() == HDC_DOUBLE);
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
}

TEST_CASE("BufferGrowStruct", "[HDC]")
{
    std::vector<std::string> keys = {"aaa","bbb","ccc","ddd"};
    HDC h;
    for (auto k : keys) {
        HDC ch;
        h.add_child(k,ch);
    };
    h.grow(4096);
    auto keys2 = h.keys();
    CHECK(keys2.size() == keys.size());
    for (size_t i=0; i<keys.size(); i++) {
        CHECK(keys[i] == keys2[i]);
    }
}

TEST_CASE("BufferGrowList", "[HDC]")
{
    HDC h;
    size_t n = 4;
    for (size_t i=0; i<n; i++) {
        HDC ch(std::to_string(i));
        h.append(ch);
    }
    h.grow(4096);
    for (size_t i=0; i<n; i++) {
        CHECK(strcmp(h[i].as_string().c_str(), std::to_string(i).c_str()) == 0);
    }
}

TEST_CASE("SerializeString", "[HDC]")
{
    // This does not makes sense for umap storage
    if (global_storage->name() == "mdbm") {
        PREPARE_TREE();
        std::string ser = tree.serialize();
        HDC tree2 = HDC::deserialize_HDC_string(ser);
        auto tree_dump = tree.to_json_string();
        auto tree2_dump = tree2.to_json_string();
        CHECK(strcmp(tree_dump.c_str(), tree2_dump.c_str()) == 0);
    }
}

TEST_CASE("SerializeFile", "[HDC]")
{
    // This does not makes sense for umap storage
    if (global_storage->name() == "mdbm") {
        // the same thing to/from file
        auto fname = make_tmp_name("h5");
        PREPARE_TREE();
        tree.serialize(fname);
        auto tree_dump = tree.to_json_string();
        HDC from_file = HDC::deserialize_HDC_file(fname);
        auto file_dump = from_file.to_json_string();
        CHECK(strcmp(tree_dump.c_str(), file_dump.c_str()) == 0);
        if(remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
    }
}

TEST_CASE("GetChildren", "[HDC]")
{
    std::vector<std::string> lst = {"aaa","bbb","ccc","ddd"};
    HDC h;
    for (auto& str: lst) {
        HDC ch(str);
        h.add_child(str,ch);
    }
    auto children = h.get_children();
    CHECK(children.size() == lst.size());
    for (size_t i=0;i<children.size();i++) {
        CHECK(strcmp(h[lst[i]].as_string().c_str(),lst[i].c_str()) == 0);
    }
}

TEST_CASE("GetSlices", "[HDC]")
{
    size_t n = 5;
    HDC h;
    for (size_t i=0;i<n;i++) {
        HDC ch(std::to_string(i));
        h.append(ch);
    }
    auto slices = h.get_slices();
    CHECK(slices.size() == n);
    for (size_t i=0;i<slices.size();i++) {
        CHECK(strcmp(h[i].as_string().c_str(),slices[i].as_string().c_str()) == 0);
    }
}

TEST_CASE("clean", "[HDC]")
{
    HDC tree;
    auto uuid = tree.get_uuid();
    tree["ch"] = HDC();
    auto ch_uuid = tree["ch"].get_uuid();
    tree.clean();
    CHECK(global_storage->has(uuid) == false);
    CHECK(global_storage->has(ch_uuid) == false);
}

TEST_CASE("load", "[HDC]")
{
    CHECK_THROWS_AS(HDC::load("json://aaa|aaa", "aaa"), HDCException);
}

TEST_CASE("scalar", "[HDC]")
{
    bool      b = true;
    int8_t   i8 =  -8;
    int16_t i16 = -16;
    int32_t i32 = -32;
    int64_t i64 = -64;
    uint8_t   u8 =  8;
    uint16_t u16 = 16;
    uint32_t u32 = 32;
    uint64_t u64 = 64;
    float     f = 3.14;
    double    d = 3.141592;

    HDC hb = HDC::make_scalar(b);
    HDC hi8 = HDC::make_scalar(i8);
    HDC hi16 = HDC::make_scalar(i16);
    HDC hi32 = HDC::make_scalar(i32);
    HDC hi64 = HDC::make_scalar(i64);
    HDC hu8 = HDC::make_scalar(u8);
    HDC hu16 = HDC::make_scalar(u16);
    HDC hu32 = HDC::make_scalar(u32);
    HDC hu64 = HDC::make_scalar(u64);
    HDC hf = HDC::make_scalar(f);
    HDC hd = HDC::make_scalar(d);

    CHECK(hb.as_scalar<bool>() == b);
    CHECK(hi8.as_scalar<int8_t>() == i8);
    CHECK(hi16.as_scalar<int16_t>() == i16);
    CHECK(hi32.as_scalar<int32_t>() == i32);
    CHECK(hi64.as_scalar<int64_t>() == i64);
    CHECK(hu8.as_scalar<uint8_t>() == u8);
    CHECK(hu16.as_scalar<uint16_t>() == u16);
    CHECK(hu32.as_scalar<uint32_t>() == u32);
    CHECK(hu64.as_scalar<uint64_t>() == u64);
    CHECK(hf.as_scalar<float>() == f);
    CHECK(hd.as_scalar<double>() == d);

    CHECK(hb.get_type() == HDC_BOOL);
    CHECK(hi8.get_type() == HDC_INT8);
    CHECK(hi16.get_type() == HDC_INT16);
    CHECK(hi32.get_type() == HDC_INT32);
    CHECK(hi64.get_type() == HDC_INT64);
    CHECK(hu8.get_type() == HDC_UINT8);
    CHECK(hu16.get_type() == HDC_UINT16);
    CHECK(hu32.get_type() == HDC_UINT32);
    CHECK(hu64.get_type() == HDC_UINT64);
    CHECK(hf.get_type() == HDC_FLOAT);
    CHECK(hd.get_type() == HDC_DOUBLE);

    HDC tree,ch;
    tree.add_child("aaa",ch);
    CHECK_THROWS(tree.as_scalar<double>());
}

TEST_CASE("get_data", "[HDC]")
{
    double array_in[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    hdc_data_t data_in;
    data_in.data = (char*)&array_in;
    data_in.type = HDC_DOUBLE;
    data_in.flags = HDCDefault;
    data_in.rank = 1;
    data_in.shape[0] = 4;
    HDC h;
    h.set_data(data_in);
    auto data_out = h.get_data();
    CHECK(data_in.type == data_out.type);
    CHECK(data_in.flags == data_out.flags);
    CHECK(data_in.rank == data_out.rank);
    CHECK(data_in.shape[0] == data_out.shape[0]);
    for (size_t i=0;i<4;i++) CHECK(array_in[i] == reinterpret_cast<double*>(data_out.data)[i]);

    //update
    double array_in2[] = { 110.0, 100.0, 1.0e-20, 1.0e20 };
    data_in.data = (char*)&array_in2;
    h.set_data(data_in);
    auto data_out2 = h.get_data();
    for (size_t i=0;i<4;i++) CHECK(array_in2[i] == reinterpret_cast<double*>(data_out2.data)[i]);

    //check if constructor throws if large rank supplied;
    data_in.rank = 20;
    CHECK_THROWS(HDC(data_in));

}

TEST_CASE("hdc_t_manipulation", "[HDC]")
{
    HDC h;
    auto obj = h.as_obj();
    CHECK(strcmp(h.get_uuid().c_str(), obj.uuid) == 0);
    CHECK(h.get_storage_id() == obj.storage_id);
    HDC h2(obj);
    CHECK(strcmp(h2.get_uuid().c_str(), obj.uuid) == 0);
    CHECK(h2.get_storage_id() == obj.storage_id);
}

#ifdef _USE_HDF5
TEST_CASE("HDF5", "[HDC]")
{
    PREPARE_TREE()
    // create temporary file name
    auto fname = make_tmp_name("h5");
    tree.to_hdf5(fname);
    HDC tree2 = HDC::from_hdf5(fname);
    double data = tree2.get("aaa/bbb/_scalar").as<double>()[0];
    CHECK(333.333 == data);
    auto path = std::string("hdf5://")+fname+"|/data/aaa/bbb/_scalar";
    HDC h5 = HDC::load(path);
    data = h5.as<double>()[0];
    CHECK(333.333 == data);
    if(remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
}

#endif

#ifdef _USE_UDA
TEST_CASE("UDA", "[HDC]")
{
    HDC h = HDC::load("uda://HELP::help()");
    h.dump();
    std::string expected =
            "\nHelp\tList of HELP plugin functions:\n"
            "\n"
            "services()\tReturns a list of available services with descriptions\n"
            "ping()\t\tReturn the Local Server Time in seconds and microseonds\n"
            "servertime()\tReturn the Local Server Time in seconds and microseonds\n\n";
    CHECK(strcmp(h.as_cstring(), expected.c_str()) == 0);
}

#endif
