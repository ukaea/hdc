#include "hdc_s3_serializer.h"

#include "hdc.hpp"

class S3 {};

const char* ref_group_name = "__hdc";

HDC read(S3& connection)
{
    return {};
}

void write_node(const HDC& h, S3& connection, const std::string& path)
{
    auto buffer = h.get_buffer();
    auto header = reinterpret_cast<hdc_header_t*>(buffer);
    auto data = buffer + sizeof(hdc_header_t);
    std::vector<char> new_buffer(header->buffer_size);
    if (h.is_fortranorder()) {
        transpose_buffer(new_buffer.data() + sizeof(hdc_header_t), h.get_buffer() + sizeof(hdc_header_t), h.get_rank(),
                         h.get_shape(), (hdc_type_t)h.get_type(),
                         h.is_fortranorder());
        data = new_buffer.data() + sizeof(hdc_header_t);
    }
    if (header->type == HDC_STRUCT) {
        hdc_map_t* children = h.get_children_ptr();
        hdc_map_t::nth_index<1>::type& ri = children->get<1>();
        if (children != nullptr) {
            for (auto it = ri.begin(); it != ri.end(); ++it) {
                auto key = it->key.c_str();
                auto uuid = it->address.c_str();
                write_node(HDC(hdc_global.storage, uuid), connection, path + "/" + key);
            }
        }
        return;
    } else if (header->type == HDC_LIST) {
        hdc_map_t* children = h.get_children_ptr();
        if (children != nullptr) {
            hdc_map_t::nth_index<1>::type& ri = children->get<1>();
            size_t n_child = children->size();
            auto wbuf = new hobj_ref_t[n_child];
            for (auto it = ri.begin(); it != ri.end(); ++it) {
                //TODO: We could probably re-do this using get_children() method
                auto uuid = it->address.c_str();
                HDC hh(hdc_global.storage, uuid);
                auto _uuid = hh.get_uuid();
                std::string full_path = std::string(ref_group_name) + "/" + _uuid;
                write_node(hh, connection, full_path.c_str());
            }
        }
        return;
    } else if (header->type == HDC_EMPTY) {
    } else {
        hsize_t rank = header->rank;
        hsize_t dimsf[10];
        for (hsize_t i = 0; i < rank; i++) {
            dimsf[i] = header->shape[i];
        };
        return;
    }
}

void
hdc::serialization::S3Serializer::serialize(const HDC& hdc, const std::string& filename, const std::string& datapath)
{
    S3 connection;
    write_node(hdc, connection, "data");
}

HDC hdc::serialization::S3Serializer::deserialize(const std::string& filename, const std::string& datapath)
{
    S3 connection;
    HDC hdc = read(connection);
    return hdc;
}

std::string hdc::serialization::S3Serializer::to_string(const HDC& hdc)
{
    return std::string();
}

HDC hdc::serialization::S3Serializer::from_string(const std::string& string)
{
    return HDC();
}
