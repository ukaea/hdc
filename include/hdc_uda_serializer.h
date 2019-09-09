#pragma once

#include "hdc_serializer.h"

namespace hdc {
namespace serialisation {

class UDASerialiser : public Serialiser {
public:
    explicit UDASerialiser(bool with_metadata) : with_metadata_{ with_metadata } {}
    ~UDASerialiser() override = default;

    void serialize(const HDC& hdc, const std::string& filename, const std::string& datapath) override;
    HDC deserialize(const std::string& filename, const std::string& datapath) override;

    std::string to_string(const HDC& hdc) override;
    HDC from_string(const std::string& string) override;

private:
    bool with_metadata_;
};

}
}
