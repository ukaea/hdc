#include "serialization/hdc_hdc_serializer.h"

#include "hdc.hpp"
#include <json/json.h>

namespace {

HDC deserialize_str(const std::string& string)
{
    // This function should be called only once at the beginning because it replaces initialization
    Json::Value root;
    stringstream ss;
    ss << string;
    ss >> root;
    auto storage_str = root.get("storage", "not_found").asString();
    if (storage_str == "not_found") {
        throw HDCException("deserialize_str(): \"storage\" field not found in string...");
    }
    auto uuid = root.get("uuid", "not_found").asString();
    if (uuid == "not_found") {
        throw HDCException("deserialize_str(): \"uuid\" field not found in string...");
    }
    auto persistent = root.get("persistent", false);
    if (persistent == false) {
        throw HDCException("deserialize_str(): \"persistent\" field not found or false in string...");
    }
    if (hdc_global.storage != nullptr) {
        throw HDCException("deserialize_str(): HDC is already initialized, call this only at the beginning...");
    }
    HDC::init(storage_str, string);
    return HDC(hdc_global.storage, uuid);
}

} // anon namespace

void hdc::serialization::HDCSerialiser::serialize(const HDC& hdc, const std::string& filename,
                                                  const std::string& datapath UNUSED)
{
    ofstream file(filename);
    file << to_string(hdc);
    file.close();
}

HDC hdc::serialization::HDCSerialiser::deserialize(const std::string& filename, const std::string& datapath UNUSED)
{
    std::ifstream file(filename);
    std::string string{ (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };
    return deserialize_str(string);
}

std::string hdc::serialization::HDCSerialiser::to_string(const HDC& hdc)
{
    Json::Value root;
    std::stringstream ss_in(hdc.get_storage()->get_settings());
    ss_in >> root;
    //TODO: switch persistent flag when wrongly initialized?
    root["uuid"] = hdc.get_uuid();
    std::stringstream ss_out;
    ss_out << root;
    return ss_out.str();
}

HDC hdc::serialization::HDCSerialiser::from_string(const std::string& string)
{
    return deserialize_str(string);
}
