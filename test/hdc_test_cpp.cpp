#include "catch.hpp"
#include "hdc.hpp"

TEST_CASE("StringParsing", "[HDCUtils]")
{
    std::vector<bool> expected_is_num = { false, true, false, true, true, false, false };
    auto vs = split_no_brackets("flux/3444/lalala/5/4/l///l");
    CHECK(vs.size() == 7);
    int i = 0;
    for (auto item : vs) {
        CHECK((item.type() == typeid(size_t)) == expected_is_num[i]);
        i++;
    }
    auto vs2 = split("flux[3444]/lalala[5][4]l///l");
    CHECK(vs2.size() == 7);
    i = 0;
    for (auto item : vs2) {
        CHECK((item.type() == typeid(size_t)) == expected_is_num[i]);
        i++;
    }
    //TODO: add protocols???
}


TEST_CASE("EmptyNode", "[HDC]")
{
    HDC h = HDC();
    CHECK(0 == h.get_shape()[0]);
    CHECK(1 == h.get_rank());
    CHECK(HDC_EMPTY == h.get_type());
    CHECK(strcmp("null", h.get_type_str().c_str()) == 0);
    CHECK(false == h.exists("aaa"));
}

TEST_CASE("EmptyNodePtr", "[HDC]")
{
    HDC* h = new HDC();
    CHECK(0 == h->get_shape()[0]);
    CHECK(1 == h->get_rank());
    CHECK(HDC_EMPTY == h->get_type());
    CHECK(strcmp("null", h->get_type_str().c_str()) == 0);
    CHECK(false == h->exists("aaa"));
    delete h;
}

TEST_CASE("EmptyArrayNode", "[HDC]")
{
    int8_t rank = 1;
    std::vector<size_t> shape = { 4 };
    HDC* hi8 = new HDC(rank, shape, HDC_INT8);
    CHECK(1 == hi8->get_rank());
    CHECK(4 == hi8->get_shape()[0]);
    CHECK(HDC_INT8 == hi8->get_type());
    CHECK(strcmp("int8", hi8->get_type_str().c_str()) == 0);
    delete hi8;

    HDC* hi32 = new HDC(rank, shape, HDC_INT32);
    CHECK(1 == hi32->get_rank());
    CHECK(4 == hi32->get_shape()[0]);
    CHECK(HDC_INT32 == hi32->get_type());
    CHECK(strcmp("int32", hi32->get_type_str().c_str()) == 0);
    delete hi32;

    HDC* hd = new HDC(rank, shape, HDC_DOUBLE);
    CHECK(1 == hd->get_rank());
    CHECK(4 == hd->get_shape()[0]);
    CHECK(HDC_DOUBLE == hd->get_type());
    CHECK(strcmp("float64", hd->get_type_str().c_str()) == 0);
    delete hd;
}

TEST_CASE("NodeManipulation", "[HDC]")
{
    HDC* tree = new HDC();
    HDC* n1 = new HDC();
    HDC* n2 = new HDC();
    // Try different
    CHECK(strcmp(n1->get_uuid().c_str(), tree->get_uuid().c_str()) != 0);
    CHECK(strcmp(n2->get_uuid().c_str(), tree->get_uuid().c_str()) != 0);
    CHECK(strcmp(n1->get_uuid().c_str(), n2->get_uuid().c_str()) != 0);

    // Try add
    tree->add_child("aaa/bbb", n1);
    CHECK(HDC_STRUCT == tree->get_type());
    CHECK(strcmp("struct", tree->get_type_str().c_str()) == 0);
    CHECK(true == tree->exists("aaa/bbb"));
    CHECK(true == tree->exists("aaa"));
    CHECK(strcmp(n1->get_uuid().c_str(), tree->get_ptr("aaa/bbb")->get_uuid().c_str()) == 0);
    CHECK(strcmp(n2->get_uuid().c_str(), tree->get_ptr("aaa/bbb")->get_uuid().c_str()) != 0);
    // Try add and get index
    HDC* n3 = new HDC();
    tree->add_child("aaa/list[0]/ddd", n3);
    CHECK(tree->exists("aaa/list[0]/ddd") == true);
    tree->add_child("aaa/list[1]", new HDC());
    tree->add_child("aaa/list[0]/eee", new HDC());
    HDC* list = tree->get_ptr("aaa/list");
    CHECK(HDC_LIST == list->get_type());
    HDC h = tree->get("aaa/list[0]/ddd");
    CHECK(true == tree->exists("aaa/list[0]/ddd"));
    CHECK(true == tree->exists("aaa/list[0]/eee"));
    tree->get_ptr("aaa/list")->get_ptr(0)->add_child("kkk", new HDC()); // Remove this - unnecessary
    CHECK(2LU == tree->get_ptr("aaa/list")->get_shape()[0]);
    CHECK(true == tree->get_ptr("aaa/list")->get_ptr(0)->exists("ddd"));
    CHECK(true == tree->get_ptr("aaa/list")->get_ptr(0)->exists("eee"));
    // Try subtree
    HDC* sub = tree->get_ptr("aaa");
    CHECK(true == sub->exists("bbb"));
    CHECK(strcmp(n1->get_uuid().c_str(), sub->get_ptr("bbb")->get_uuid().c_str()) == 0);
    // Test set
    tree->set_child("aaa/bbb", n2);
    CHECK(true == sub->exists("bbb"));
    CHECK(strcmp(n2->get_uuid().c_str(), sub->get_ptr("bbb")->get_uuid().c_str()) == 0);
    CHECK(strcmp(n1->get_uuid().c_str(), tree->get_ptr("aaa/bbb")->get_uuid().c_str()) != 0);
    // Test delete
    tree->delete_child("aaa/bbb");
    CHECK(false == tree->exists("aaa/bbb"));
    CHECK(true == tree->exists("aaa"));
    tree->add_child("aaa/bbb", n1);
    tree->delete_child("aaa");
    CHECK(false == tree->exists("aaa"));
    // Try add to empty path
    HDC t;
    HDC d;
    CHECK_THROWS_AS(t.add_child("", d), HDCException);
    delete tree;
    delete n1;
    delete n2;
    delete n3;
    delete sub;
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
#ifdef _USE_HDF5
    h.to_hdf5("aaa.hdf5", "data");
#endif // _USE_HDF5
}


TEST_CASE("Int8DataManipulation", "[HDC]")
{
    int rank = 1;
    std::vector<size_t> shape = { 4 };
    int8_t data[] = { 7, 20, 3, 5 };
    HDC* h = new HDC();
    h->set_data(rank, shape, data);
    CHECK(HDC_INT8 == h->get_type());
    CHECK(1 == h->get_rank());
    CHECK(4 == h->get_shape()[0]);
    CHECK(strcmp("int8", h->get_type_str().c_str()) == 0);
    int8_t* data2 = h->as<int8_t*>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    // This is no longer possible as for some storages data have to be copied (all for now, maybe we can enable specifically for umap storage in future)
    // All further occurencies will be removed.
    data[3] = 120;
    h->set_data(rank, shape, data);
    data2 = h->as<int8_t*>();
    CHECK(120 == data2[3]);
    delete h;
}

TEST_CASE("Int16DataManipulation", "[HDC]")
{
    int8_t rank = 1;
    std::vector<size_t> shape = { 4 };
    int16_t data[] = { 777, 30000, 3333, 22222 };
    HDC* h = new HDC();
    h->set_data<int16_t>(rank, shape, data);
    CHECK(HDC_INT16 == h->get_type());
    CHECK(1 == h->get_rank());
    CHECK(4 == h->get_shape()[0]);
    CHECK(strcmp("int16", h->get_type_str().c_str()) == 0);
    int16_t* data2 = h->as<int16_t*>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    delete h;
}

TEST_CASE("Int32DataManipulation", "[HDC]")
{
    int8_t rank = 1;
    std::vector<size_t> shape = { 4 };
    int32_t data[] = { 777, 20202020, 3333, 555555 };
    HDC* h = new HDC();
    h->set_data<int32_t>(rank, shape, data);
    CHECK(HDC_INT32 == h->get_type());
    CHECK(1 == h->get_rank());
    CHECK(4 == h->get_shape()[0]);
    CHECK(strcmp("int32", h->get_type_str().c_str()) == 0);
    int32_t* data2 = h->as<int32_t*>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    delete h;
}

TEST_CASE("Int64DataManipulation", "[HDC]")
{
    int8_t rank = 1;
    std::vector<size_t> shape = { 4 };
    int64_t data[] = { 777, 20202020, 3333, 2000000000 };
    HDC* h = new HDC();
    h->set_data<int64_t>(rank, shape, data);
    CHECK(HDC_INT64 == h->get_type());
    CHECK(1 == h->get_rank());
    CHECK(4 == h->get_shape()[0]);
    CHECK(strcmp("int64", h->get_type_str().c_str()) == 0);
    int64_t* data2 = h->as<int64_t*>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    delete h;
}


TEST_CASE("DoubleDataManipulation", "[HDC]")
{
    int8_t rank = 1;
    std::vector<size_t> shape = { 4 };
    double data[] = { 0.0, 1000.0, 1.0e-200, 1.0e200 };
    HDC* h = new HDC();
    h->set_data<double>(rank, shape, data);
    CHECK(HDC_DOUBLE == h->get_type());
    CHECK(1 == h->get_rank());
    CHECK(4 == h->get_shape()[0]);
    CHECK(strcmp("float64", h->get_type_str().c_str()) == 0);
    double* data2 = h->as<double*>();
    for (int i = 0; i < 3; i++) CHECK(data[i] == data2[i]);
    delete h;
}

TEST_CASE("StringDataManipulation", "[HDC]")
{
    HDC* h = new HDC();
    std::string str("lalalalala  bleble");
    h->set_string(str);
    std::string str2 = std::string(h->as_string());
    CHECK(strcmp(str.c_str(), str2.c_str()) == 0);
    delete h;
}

TEST_CASE("GetStrides", "[HDC]")
{
    int32_t array3d[24] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
    std::vector<size_t> shape3d = { 2, 3, 4 };
    HDC threed_f;
    threed_f.set_data<int32_t>(3, shape3d, array3d, HDCFortranOrder);
    auto strides_f = threed_f.get_strides();
    CHECK(4 == strides_f[0]);
    CHECK(8 == strides_f[1]);
    CHECK(24 == strides_f[2]);
    HDC threed_c;
    threed_c.set_data<int32_t>(3, shape3d, array3d);
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
    CHECK(strcmp("list", h.get_type_str().c_str()) == 0);
    CHECK(1 == h.get_rank());
    CHECK(2 == h.get_shape()[0]);
    CHECK(strcmp("1", h.get_ptr(0)->as_string().c_str()) == 0);
    CHECK(strcmp("2", h.get_ptr(1)->as_string().c_str()) == 0);
    HDC sl3;
    sl3.set_string("3");
    h.insert(1, sl3);
    vector<string> keys = h.keys();
    CHECK(strcmp("3", h.get_ptr(1)->as_string().c_str()) == 0);
    CHECK(strcmp("2", h.get_ptr(2)->as_string().c_str()) == 0);
    HDC sl4;
    sl4.set_string("4");
    h.set_child(1, sl4);
    CHECK(strcmp("4", h.get_ptr(1)->as_string().c_str()) == 0);

    HDC n;
    HDC ch;
    n.insert(10, ch);
    CHECK(11 == n.get_shape()[0]);
    CHECK(HDC_LIST == n.get_type());
    for (int i = 0; i < 11; i++) CHECK(HDC_EMPTY == n.get(i).get_type());
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

#define PREPARE_TREE()                                                                              \
    int rank = 1;                                                                                   \
    std::vector<size_t> shape = {4};                                                                           \
    double data_double[] = {0.0,1000.0,1.0e-200,1.0e200};                                           \
    int32_t data_int[] = {777,20202020,3333,555555};                                                \
    HDC tree;                                                                                       \
    HDC scalar;                                                                                     \
    scalar.set_data(333.333);                                                                       \
    tree.add_child("aaa/bbb/_scalar", scalar);                                                      \
    tree.set_data<double>("aaa/bbb/double",rank,shape,data_double);                                 \
    tree.set_data<double>("aaa/bbb/double2",rank,shape,data_double);                                \
    tree.set_data<int>("aaa/bbb/int",rank,shape,data_int);                                          \
    HDC ch;                                                                                         \
    tree.add_child("aaa/bbb/empty", ch);                                                            \
    HDC list;                                                                                       \
    HDC lch;                                                                                        \
    for (int i=0;i<5;i++) {                                                                         \
        HDC lch;                                                                                    \
        list.append(lch);                                                                     \
    }                                                                                               \
    tree.add_child("aaa/list", list);                                                               \
    tree.set_string("aaa/string","Lorem ipsum dolor sit amet, consectetuer adipiscing elit.");      \

TEST_CASE("JsonComplete", "[HDC]")
{
    PREPARE_TREE()

    // Save JSON
    tree.to_json("tree.txt");
    // Load JSON
    HDC tree2 = HDC::from_json("tree.txt");
    tree2.to_json("tree2.txt");
    // test tree
    HDC s = tree2.get("aaa/bbb/double");
    // Test double
    CHECK(1 == s.get_rank());
    CHECK(4 == s.get_shape()[0]);
    CHECK(HDC_DOUBLE == s.get_type());
    CHECK(strcmp(tree.get("aaa/bbb/double").get_type_str().c_str(), s.get_type_str().c_str()) == 0);
    double* data_double_in = s.as<double*>();
    for (size_t i = 0; i < shape[0]; i++) CHECK(data_double[i] == data_double_in[i]);

    // Test int
    s = tree2.get("aaa/bbb/int");
    CHECK(1 == s.get_rank());
    CHECK(4 == s.get_shape()[0]);
    CHECK(HDC_INT32 == s.get_type());
    CHECK(strcmp(tree.get("aaa/bbb/int").get_type_str().c_str(), tree2.get("aaa/bbb/int").get_type_str().c_str()) == 0);
    int32_t* data_int_in = s.as<int32_t*>();
    for (size_t i = 0; i < shape[0]; i++) CHECK(data_int[i] == data_int_in[i]);

    // Test empty
    CHECK(HDC_EMPTY == tree2.get_ptr("aaa/bbb/empty")->get_type());

    // Test list
    s = tree2.get("aaa/list");
    CHECK(1 == s.get_rank());
    CHECK(5 == s.get_shape()[0]);
    CHECK(HDC_LIST == s.get_type());
    CHECK(strcmp(tree.get("aaa/list").get_type_str().c_str(), tree2.get("aaa/list").get_type_str().c_str()) == 0);
    for (int i = 0; i < 5; i++) CHECK(HDC_EMPTY == s.get(i).get_type());

    // Test string
    CHECK(strcmp(tree.get("aaa/string").as_string().c_str(), tree2.get("aaa/string").as_string().c_str()) ==
          0);

    HDC j = HDC::load("json://tree2.txt|aaa/string");
    CHECK(strcmp(j.as_string().c_str(), "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.") == 0);
}

TEST_CASE("CopyConstructor", "[HDC]")
{
    PREPARE_TREE()
    // test copy c-tor
    HDC copy(tree);
    HDC d = copy.get("aaa/bbb/double");
    CHECK(1 == d.get_rank());
    CHECK(4 == d.get_shape()[0]);
    CHECK(HDC_DOUBLE == d.get_type());
    CHECK(strcmp(tree.get("aaa/bbb/double").get_type_str().c_str(), d.get_type_str().c_str()) == 0);
}

TEST_CASE("load", "[HDC]")
{
    CHECK_THROWS_AS(HDC::load("json://aaa|aaa", "aaa"), HDCException);
}

#ifdef _USE_HDF5
TEST_CASE("HDF5", "[HDC]")
{
    PREPARE_TREE()
    tree.to_hdf5("tree.h5");
    HDC tree2 = HDC::from_hdf5("tree.h5");
    double data = tree2.get("aaa/bbb/_scalar").as<double*>()[0];
    CHECK(data == 333.333);
    HDC h5 = HDC::load("hdf5://tree.h5|/data/aaa/bbb/_scalar");
    data = h5.as<double*>()[0];
    CHECK(data == 333.333);
}

#endif

#ifdef _USE_UDA
TEST_CASE("StringConstructor", "[HDC]")
{
    HDC h = HDC::load("uda://HELP::help()");
    std::string expected =
            "\nHelp\tList of HELP plugin functions:\n"
            "\n"
            "services()\tReturns a list of available services with descriptions\n"
            "ping()\t\tReturn the Local Server Time in seconds and microseonds\n"
            "servertime()\tReturn the Local Server Time in seconds and microseonds\n\n";
    CHECK(strcmp(h.as_string().c_str(), expected.c_str()) == 0);
}

#endif
