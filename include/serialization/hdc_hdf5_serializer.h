#pragma once

#include "hdc_serializer.h"
#include <H5Cpp.h>
#include <hdf5.h>
#if ((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR < 12))
#define HDC_HDF5_ADDR_T haddr_t
#define HDC_HDF5_ADDR_NAME addr
#else
#define HDC_HDF5_ADDR_T H5O_token_t
#define HDC_HDF5_ADDR_NAME token
#endif

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
