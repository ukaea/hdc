#define CATCH_CONFIG_MAIN
#include "hdc_test_common.hpp"

#define PREPARE_TREE()                                                                              \
    std::vector<size_t> shape = {4};                                                                \
    double data_double[] = {0.0,1000.0,1.0e-200,1.0e200};                                           \
    int8_t data_int8[] = {-7,2,3,4};                                                                \
    int16_t data_int16[] = {-7000,2000,3000,4000};                                                  \
    int32_t data_int32[] = {-70000000,10000000,20000000,300000000};                                 \
    int64_t data_int64[] = {-70000000,10000000,20000000,300000000};                                 \
    uint8_t data_uint8[] = {7,2,3,4};                                                               \
    uint16_t data_uint16[] = {7000,2000,3000,4000};                                                 \
    uint32_t data_uint32[] = {70000000,10000000,20000000,300000000};                                \
    uint64_t data_uint64[] = {70000000,10000000,20000000,300000000};                                \
    float data_float[] = {0.0,1000.0,1.0e-20,1.0e20};                                               \
    bool data_bool[] = {false,true,false,true};                                                     \
    HDC tree;                                                                                       \
    HDC scalar;                                                                                     \
    scalar.set_data(333.333);                                                                       \
    tree.add_child("aaa/bbb/_scalar", scalar);                                                      \
    tree["aaa/bbb/double"].set_data<double>(shape,data_double);                                     \
    tree["aaa/bbb/float"].set_data<float>(shape,data_float);                                        \
    tree["aaa/bbb/double2"].set_data<double>(shape,data_double);                                    \
    tree["aaa/bbb/int8"].set_data<int8_t>(shape,data_int8);                                         \
    tree["aaa/bbb/int16"].set_data<int16_t>(shape,data_int16);                                      \
    tree["aaa/bbb/int32"].set_data<int32_t>(shape,data_int32);                                      \
    tree["aaa/bbb/int64"].set_data<int64_t>(shape,data_int64);                                      \
    tree["aaa/bbb/uint8"].set_data<uint8_t>(shape,data_uint8);                                      \
    tree["aaa/bbb/uint16"].set_data<uint16_t>(shape,data_uint16);                                   \
    tree["aaa/bbb/uint32"].set_data<uint32_t>(shape,data_uint32);                                   \
    tree["aaa/bbb/uint64"].set_data<uint64_t>(shape,data_uint64);                                   \
    tree["aaa/bbb/bool"].set_data<bool>(shape,data_bool);                                           \
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

TEST_CASE("SerializeString", "[HDC]")
{
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm",settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.to_json_string();
    auto ser = tree.serialize();
    auto uuid = tree.get_uuid();
    //Check serialized string for arbitrary filelds.
    std::stringstream ss(ser);
    Json::Value root;
    ss >> root;
    auto fname_out = root["filename"].asString();
    auto persistent_out = root["persistent"].asBool();
    auto uuid_out = root["uuid"].asString();
    CHECK(strcmp(fname.c_str(), fname_out.c_str()) == 0);
    CHECK(persistent_out == true);
    CHECK(strcmp(fname.c_str(), fname_out.c_str()) == 0);
    HDC::destroy();
    // load back from the file
    HDC tree2 = HDC::deserialize_str(ser);
    auto tree2_dump = tree2.to_json_string();
    CHECK(strcmp(tree_dump.c_str(), tree2_dump.c_str()) == 0);
    HDC::destroy();
    //Remove mdbm file
    if(remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
}

TEST_CASE("SerializeFile", "[HDC]")
{
    // the same thing to/from file
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm",settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.to_json_string();
    auto fname_hdc = make_tmp_name("hdc");
    tree.serialize(fname_hdc);
    HDC::destroy();
    // load back from the file
    HDC from_file = HDC::deserialize_file(fname_hdc);
    auto file_dump = from_file.to_json_string();
    CHECK(strcmp(tree_dump.c_str(), file_dump.c_str()) == 0);
    HDC::destroy();
    //Remove hdc file
    if(remove(fname_hdc.c_str()) != 0) std::cerr << "Error removing file " << fname_hdc << std::endl;
    //Remove mdbm file
    if(remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
}

TEST_CASE("TestSettings" ,"[HDC]")
{
    HDC::destroy();
    putenv((char *)"HDC_PLUGIN_PATH=/1");
    putenv((char *)"HDC_PERSISTENT=TRUE");
    putenv((char *)"HDC_DB_FILE=/tmp/pokus.db");
    putenv((char *)"HDC_PLUGIN=mdbm");
    HDC::init();
    CHECK(strcmp(hdc_global.storage->name().c_str(), "mdbm") == 0);
    Json::Value settings;
    std::stringstream ss(hdc_global.storage->get_settings());
    ss >> settings;
    CHECK(settings["persistent"].asBool() == true);
    CHECK(strcmp(settings["filename"].asCString(),"/tmp/pokus.db") == 0);
    HDC::destroy();
    //test that arguments have preffered
    HDC::init("mdbm","{\"filename\":\"/tmp/aaa.mdbm\",\"persistent\": false}");
    Json::Value settings2;
    std::stringstream ss2(hdc_global.storage->get_settings());
    ss2 >> settings2;
    CHECK(settings2["persistent"].asBool() == false);
    CHECK(strcmp(settings2["filename"].asCString(),"/tmp/aaa.mdbm") == 0);
    HDC::destroy();
}
