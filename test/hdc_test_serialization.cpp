#define CATCH_CONFIG_MAIN

#include "hdc_test_common.hpp"
#include <json/json.h>

TEST_CASE("SerializeDeserialize", "[HDC]")
{
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto ser = tree.serialize("hdc_string");
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
    HDC tree2 = HDC::deserialize("hdc_string",ser);
    auto tree2_dump = tree2.serialize("json");
    CHECK(strcmp(tree_dump.c_str(), tree2_dump.c_str()) == 0);
    HDC::destroy();
    //Remove mdbm file
    if (remove(fname.c_str()) != 0) {
        std::cerr << "Error removing file " << fname << std::endl;
    }
}

TEST_CASE("LoadSave", "[HDC]")
{
    // the same thing to/from file
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto fname_hdc = make_tmp_name("hdc");
    auto uri_hdc = "hdc_file://" + fname_hdc;
    tree.save(uri_hdc);
    HDC::destroy();
    // load back from the file
    HDC from_file = HDC::load(uri_hdc);
    auto file_dump = from_file.serialize("json");
    CHECK(strcmp(tree_dump.c_str(), file_dump.c_str()) == 0);
    HDC::destroy();
    //Remove hdc file
    if (remove(fname_hdc.c_str()) != 0) {
        std::cerr << "Error removing file " << fname_hdc << std::endl;
    }
    //Remove mdbm file
    if (remove(fname.c_str()) != 0) {
        std::cerr << "Error removing file " << fname << std::endl;
    }
}

TEST_CASE("TestSettings", "[HDC]")
{
    HDC::destroy();
    putenv((char*)"HDC_PLUGIN_PATH=/1");
    putenv((char*)"HDC_PERSISTENT=TRUE");
    auto fname = make_tmp_name("mdbm");
    std::string file_env = "HDC_DB_FILE=" + fname;
    putenv((char*)file_env.data());
    putenv((char*)"HDC_PLUGIN=mdbm");
    HDC::init();
    CHECK(strcmp(hdc_global.storage->name().c_str(), "mdbm") == 0);
    Json::Value settings;
    std::stringstream ss(hdc_global.storage->get_settings());
    ss >> settings;
    CHECK(settings["persistent"].asBool() == true);
    CHECK(strcmp(settings["filename"].asCString(), fname.c_str()) == 0);
    HDC::destroy();
    if (remove(fname.c_str()) != 0) {
        std::cerr << "Error removing file " << fname << std::endl;
    }
    //test that arguments have preffered
    HDC::init("mdbm", "{\"filename\":\"/tmp/aaa.mdbm\",\"persistent\": false}");
    Json::Value settings2;
    std::stringstream ss2(hdc_global.storage->get_settings());
    ss2 >> settings2;
    CHECK(settings2["persistent"].asBool() == false);
    CHECK(strcmp(settings2["filename"].asCString(), "/tmp/aaa.mdbm") == 0);
    HDC::destroy();
}

#ifdef _USE_S3
TEST_CASE("Test serialise to S3", "[HDC][S3]")
{
    HDC::destroy();
    HDC::init();
    PREPARE_TREE();
    tree.save("s3://10.209.2.145:9000/test/test1");
}

TEST_CASE("Test deserialise from S3", "[HDC][S3]")
{
    HDC::destroy();
    HDC::init();
    HDC hdc = HDC::load("s3://10.209.2.145:9000/test/test1");
    //std::cout << hdc.serialize("json") << std::endl;

    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto hdc_dump = hdc.serialize("json");

    CHECK(tree_dump == hdc_dump);
}
#endif

#ifdef _USE_FLATBUFFERS
TEST_CASE("Test serialise to flatbuffer", "[HDC][FLATBUFFERS]")
{
    HDC::destroy();
    HDC::init();
    PREPARE_TREE();
    tree.save("flatbuffers://flatbuffer-test.dat");
}

TEST_CASE("Test deserialise from flatbuffer", "[HDC][FLATBUFFERS]")
{
    HDC::destroy();
    HDC::init();
    HDC hdc = HDC::load("flatbuffers://flatbuffer-test.dat");

    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto hdc_dump = hdc.serialize("json");

    std::cout << tree_dump;

    CHECK(tree_dump == hdc_dump);
}
#endif
