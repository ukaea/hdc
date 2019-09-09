#define CATCH_CONFIG_MAIN

#include "hdc_test_common.hpp"

TEST_CASE("SerializeString", "[HDC]")
{
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto ser = tree.serialize("hdc");
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
    HDC tree2 = HDC::deserialize("hdc", ser);
    auto tree2_dump = tree2.serialize("json");
    CHECK(strcmp(tree_dump.c_str(), tree2_dump.c_str()) == 0);
    HDC::destroy();
    //Remove mdbm file
    if (remove(fname.c_str()) != 0) {
        std::cerr << "Error removing file " << fname << std::endl;
    }
}

TEST_CASE("SerializeFile", "[HDC]")
{
    // the same thing to/from file
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto fname_hdc = make_tmp_name("hdc");
    tree.serialize(fname_hdc);
    HDC::destroy();
    // load back from the file
    HDC from_file = HDC::load(fname_hdc);
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

TEST_CASE("LoadString", "[HDC]")
{
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto ser = tree.serialize("hdc");
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
    HDC tree2 = HDC::load("hdc_string://" + ser);
    auto tree2_dump = tree2.serialize("json");
    CHECK(strcmp(tree_dump.c_str(), tree2_dump.c_str()) == 0);
    HDC::destroy();
    //Remove mdbm file
    if (remove(fname.c_str()) != 0) {
        std::cerr << "Error removing file " << fname << std::endl;
    }
}

TEST_CASE("LoadFile", "[HDC]")
{
    // the same thing to/from file
    HDC::destroy();
    auto fname = make_tmp_name("mdbm");
    auto settings_str = "{\"filename\": \"" + fname + "\", \"persistent\": true}";
    HDC::init("mdbm", settings_str);
    PREPARE_TREE();
    auto tree_dump = tree.serialize("json");
    auto fname_hdc = make_tmp_name("hdc");
    tree.serialize(fname_hdc);
    HDC::destroy();
    // load back from the file
    HDC from_file = HDC::load("hdc_file://" + fname_hdc);
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
    if (remove(fname.c_str()) != 0) std::cerr << "Error removing file " << fname << std::endl;
    //test that arguments have preffered
    HDC::init("mdbm", "{\"filename\":\"/tmp/aaa.mdbm\",\"persistent\": false}");
    Json::Value settings2;
    std::stringstream ss2(hdc_global.storage->get_settings());
    ss2 >> settings2;
    CHECK(settings2["persistent"].asBool() == false);
    CHECK(strcmp(settings2["filename"].asCString(), "/tmp/aaa.mdbm") == 0);
    HDC::destroy();
}
