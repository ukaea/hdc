#pragma once

#include "hdc_serializer.h"
#include <H5Cpp.h>
#include <hdf5.h>
#include <string.h>

#include "hdc.hpp"

#define HDC_HDF5_VALID_ID(hdf5_id)   hdf5_id  >= 0

#define HDC_HDF5_STATUS_OK(hdf5_id)   hdf5_id  >= 0

#define HDC_CHECK_HDF5_ERROR(hdf5_err, msg)                               \
{                                                                         \
    if( hdf5_err < 0 )                                                    \
    {                                                                     \
        std::cerr << "HDF5 Error (error code: "                           \
            <<  hdf5_err                                                  \
            <<  ") " << msg;                                              \
            throw HDCException();                                         \
    }                                                                     \
}


#define HDC_CHECK_HDF5_ERROR_WITH_REF(hdf5_err, ref_path, msg)            \
{                                                                         \
    if( hdf5_err < 0 )                                                    \
    {                                                                     \
        std::cerr << "HDF5 Error (error code: "                           \
            <<  hdf5_err                                                  \
            <<  ", reference path: \""                                    \
            <<  ref_path << "\""                                          \
            <<  ") " << msg;                                              \
            throw HDCException();                                         \
    }                                                                     \
}

#define HDC_HDF5_ERROR(ref_path, msg)                                     \
{                                                                         \
    std::cerr << "HDF5 Error (reference path: \"" << ref_path             \
                    << ref_path << "\") " <<  msg;                        \
    throw HDCException();                                                 \
}


//Compatibility with new HDF5 token API
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
