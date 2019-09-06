#include "hdc_serializer.h"

#include "hdc_hdc_serializer.h"
#include "hdc_json_serializer.h"
#ifdef _USE_HDF5
#  include "hdc_hdf5_serializer.h"
#endif
#ifdef _USE_UDA
#  include "hdc_uda_serializer.h"
#endif

#include <hdc_errors.hpp>

using namespace hdc::serialisation;

const SerialiserMap Serialiser::serializers_ = SerialiserMap();

SerialiserMap::SerialiserMap() noexcept
{
    serializers_["hdf5"] = std::make_unique<HDF5Serialiser>();
    serializers_["json"] = std::make_unique<JSONSerialiser>(0);
    serializers_["json_string"] = std::make_unique<JSONSerialiser>(0);
#ifdef _USE_UDA
    serializers_["uda"] = std::make_unique<UDASerialiser>(false);
    serializers_["uda_new"] = std::make_unique<UDASerialiser>(false);
#endif
#ifdef _USE_HDF5
    serializers_["hdc_file"] = std::make_unique<HDCSerialiser>();
    serializers_["hdc_string"] = std::make_unique<HDCSerialiser>();
#endif
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