#pragma once

#include "hdc_serializer.h"

namespace hdc {
namespace serialization {

class HDF5Serialiser : public Serialiser {
public:
    explicit HDF5Serialiser() = default;
    ~HDF5Serialiser() override = default;

    void serialize(const HDC& hdc, const std::string& filename, const std::string& datapath) override;
    HDC deserialize(const std::string& filename, const std::string& datapath) override;

    std::string to_string(const HDC& hdc) override;
    HDC from_string(const std::string& string) override;
};

}
}
