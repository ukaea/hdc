#include "serialization/hdc_serializer.h"

#include "serialization/hdc_json_serializer.h"
#ifdef _USE_MDBM
#  include "serialization/hdc_hdc_serializer.h"
#endif
#ifdef _USE_HDF5
#  include "serialization/hdc_hdf5_serializer.h"
#endif
#ifdef _USE_UDA
  #include "serialization/hdc_uda_serializer.h"
#endif
#ifdef _USE_FLATBUFFERS
#  include "serialization/hdc_flatbuffers_serializer.h"
#endif
#ifdef _USE_S3
#  include "serialization/hdc_s3_serializer.h"
#endif

#include <hdc_errors.hpp>

using namespace hdc::serialization;

const SerialiserMap Serialiser::serializers_ = SerialiserMap();

SerialiserMap::SerialiserMap() noexcept
{
    serializers_["json"] = std::make_unique<JSONSerialiser>(0);
    serializers_["json_string"] = std::make_unique<JSONSerialiser>(0);
    serializers_["json_verbose"] = std::make_unique<JSONSerialiser>(1);
#ifdef _USE_UDA
    serializers_["uda"] = std::make_unique<UDASerialiser>(false);
    serializers_["uda_new"] = std::make_unique<UDASerialiser>(false);
#endif
#ifdef _USE_HDF5
    serializers_["hdf5"] = std::make_unique<HDF5Serialiser>();
#endif
#ifdef _USE_MDBM
    serializers_["hdc_file"] = std::make_unique<HDCSerialiser>();
    serializers_["hdc_string"] = std::make_unique<HDCSerialiser>();
#endif
#ifdef _USE_FLATBUFFERS
    serializers_["flatbuffers"] = std::make_unique<FlatBuffersSerializer>();
#endif
#ifdef _USE_S3
    serializers_["s3"] = std::make_unique<S3Serializer>();
#endif
}

std::vector<std::string> SerialiserMap::available_serializers() const
{
    std::vector<std::string> keys;
    keys.reserve(serializers_.size());
    for (auto& item: serializers_) {
        keys.push_back(item.first);
    }
    return keys;
}

Serialiser& SerialiserMap::operator[](const std::string& key) const
{
    if (serializers_.count(key)) {
        return *serializers_.at(key);
    }
    throw HDCException("Protocol " + key + " not known\n");
}

Serialiser& Serialiser::find_serializer(const std::string& protocol)
{
    return serializers_[protocol];
}

std::vector<std::string> Serialiser::available_serializers()
{
    return serializers_.available_serializers();
}
