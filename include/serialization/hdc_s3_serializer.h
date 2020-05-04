#pragma once

#include "hdc_serializer.h"

namespace hdc {
namespace serialization {

class S3Serializer : public Serialiser {
public:
    S3Serializer();
    ~S3Serializer() override = default;

    void serialize(const HDC& hdc, const std::string& filename, const std::string& datapath) override;
    HDC deserialize(const std::string& filename, const std::string& datapath) override;

    std::string to_string(const HDC& hdc) override;
    HDC from_string(const std::string& string) override;

private:
    const char* access_key_;
    const char* secret_key_;

    static const char* list_identifier_;

    void write_node(const HDC& hdc, const char* host, const char* bucket, const std::string& path) const;
    void read_node(HDC& hdc, const char* host, const char* bucket, const std::string& path) const;
    HDC read(const char* host, const char* bucket, const std::string& path) const;
};

}
}

