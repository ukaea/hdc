#include "serialization/hdc_s3_serializer.h"

#include "hdc.hpp"

#include "libs3.h"
#include <json/json.h>

using namespace hdc::serialization;

namespace {

struct Data {
    S3Status status;
    std::string error;
};

struct PutData {
    S3Status status;
    std::string error;
    size_t position;
    size_t size;
    const char* data;
};

struct GetData {
    S3Status status;
    std::string error;
    std::vector<char> buffer;
};

struct ListData {
    S3Status status;
    std::string error;
    std::vector<std::string> items;
};

S3Status responsePropertiesCallback(
        const S3ResponseProperties* properties UNUSED,
        void* callbackData UNUSED)
{
    return S3StatusOK;
}

void responseCompleteCallback(
        S3Status status,
        const S3ErrorDetails* error,
        void* callbackData)
{
    auto data = reinterpret_cast<Data*>(callbackData);
    data->status = status;
    if (error->message != nullptr) {
        data->error = error->message;
    }
}

int putObjectDataCallback(int bufferSize, char* buffer, void* callbackData)
{
    auto putData = reinterpret_cast<PutData*>(callbackData);

    if (putData->position >= putData->size) {
        return 0;
    }

    size_t len = std::min(static_cast<size_t>(bufferSize), putData->size - putData->position);

    memcpy(buffer, &putData->data[putData->position], len);

    putData->position += len;

    return len;
}

S3Status getObjectDataCallback(int bufferSize, const char* buffer, void* callbackData)
{
    auto data = reinterpret_cast<GetData*>(callbackData);

    size_t pos = data->buffer.size();
    data->buffer.reserve(pos + bufferSize);

    std::copy(buffer, buffer + bufferSize, std::back_inserter(data->buffer));

    return S3StatusOK;
}

S3Status listBucketCallback(int isTruncated UNUSED,
                            const char* nextMarker UNUSED,
                            int contentsCount,
                            const S3ListBucketContent* contents,
                            int commonPrefixesCount UNUSED,
                            const char** commonPrefixes UNUSED,
                            void* callbackData)
{
    auto data = reinterpret_cast<ListData*>(callbackData);

    for (int i = 0; i < contentsCount; ++i) {
        data->items.emplace_back(contents[i].key);
    }

    return S3StatusOK;
}

} // anon namespace

const char* S3Serializer::list_identifier_ = "__list_";

void S3Serializer::read_node(HDC& hdc, const char* host, const char* bucket, const std::string& path) const
{
    GetData data = { S3StatusOK, "", {}};

    S3ResponseHandler responseHandler =
            {
                    &responsePropertiesCallback,
                    &responseCompleteCallback
            };

    S3GetObjectHandler getHandler =
            {
                    responseHandler,
                    &getObjectDataCallback
            };

    S3BucketContext bucketContext =
            {
                    host,
                    bucket,
                    S3ProtocolHTTP,
                    S3UriStylePath,
                    access_key_,
                    secret_key_
            };

    S3_get_object(&bucketContext, path.c_str(), nullptr, 0, 0, nullptr, &getHandler, reinterpret_cast<void*>(&data));

    if (data.status != S3StatusOK) {
        throw HDCException(data.error);
    }

    std::string buffer = data.buffer.data();

    HDC node = HDC::deserialize("json", buffer);

    std::deque<std::string> tokens;
    boost::split(tokens, path, boost::is_any_of("/"));
    tokens.pop_front();

    if (boost::starts_with(tokens.back(), list_identifier_)) {
        boost::replace_first(tokens.back(), list_identifier_, "");
        long index = std::strtol(tokens.back().c_str(), nullptr, 10);
        tokens.pop_back();
        std::string node_name = boost::join(tokens, "/");
        HDC list_node = hdc.get_or_create(node_name);
        list_node.insert(index, node);
    } else {
        std::string node_name = boost::join(tokens, "/");
        hdc.add_child(node_name, node);
    }
}

HDC S3Serializer::read(const char* host, const char* bucket, const std::string& path) const
{
    S3ResponseHandler responseHandler =
            {
                    &responsePropertiesCallback,
                    &responseCompleteCallback
            };

    S3ListBucketHandler listBucketHandler =
            {
                    responseHandler,
                    &listBucketCallback
            };

    S3BucketContext bucketContext =
            {
                    host,
                    bucket,
                    S3ProtocolHTTP,
                    S3UriStylePath,
                    access_key_,
                    secret_key_
            };

    ListData list_data = { S3StatusOK, "", {}};

    S3_list_bucket(&bucketContext, path.c_str(), nullptr, nullptr, 0, nullptr, &listBucketHandler, &list_data);

    if (list_data.status != S3StatusOK) {
        throw HDCException(list_data.error);
    }

    HDC hdc = {};

    for (const auto& node : list_data.items) {
        read_node(hdc, host, bucket, node);
    }

    return hdc;
}

void S3Serializer::write_node(const HDC& hdc, const char* host, const char* bucket, const std::string& path) const
{
    auto buffer = hdc.get_buffer();
    auto header = reinterpret_cast<hdc_header_t*>(buffer);
    std::vector<char> new_buffer(header->buffer_size);
    if (hdc.is_fortranorder()) {
        transpose_buffer(new_buffer.data() + sizeof(hdc_header_t), hdc.get_buffer() + sizeof(hdc_header_t),
                         hdc.get_rank(),
                         hdc.get_shape(), (hdc_type_t)hdc.get_type(),
                         hdc.is_fortranorder());
    }
    if (header->type == HDC_STRUCT) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            for (const auto& child : hdc_children->get<1>()) {
                auto key = child.key;
                auto uuid = child.address;
                write_node(HDC(hdc_global.storage, uuid), host, bucket, path + "/" + key.c_str());
            }
        }
        return;
    } else if (header->type == HDC_LIST) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            int index = 0;
            for (const auto& child : hdc_children->get<1>()) {
                auto uuid = child.address;
                write_node(HDC(hdc_global.storage, uuid), host, bucket, path + "/" + list_identifier_ + std::to_string(index));
                ++index;
            }
        }
        return;
//    } else if (header->type == HDC_EMPTY) {
//
    } else {
        std::vector<S3NameValue> meta_data;

//        std::string rank_string = std::to_string(header->rank);
//        meta_data.emplace_back(S3NameValue{ "rank", rank_string.c_str() });

        size_t rank = header->rank;
        std::string shape_string;
        if (rank > 0) {
            std::string delim;
            for (size_t i = 0; i < rank; i++) {
                shape_string += (delim + std::to_string(header->shape[i]));
                delim = ",";
            };
//            meta_data.emplace_back(S3NameValue{ "shape", shape_string.c_str() });
        }

        S3PutProperties putProperties = {};
        putProperties.metaDataCount = meta_data.size();
        putProperties.metaData = meta_data.data();
        putProperties.contentType = "application/json";

        std::string json = hdc.serialize("json");

//        PutData putData{ 0, header->data_size, data };
        PutData putData{ S3StatusOK, "", 0, json.size(), json.data() };

        S3BucketContext bucketContext =
                {
                        host,
                        bucket,
                        S3ProtocolHTTP,
                        S3UriStylePath,
                        access_key_,
                        secret_key_
                };

        S3ResponseHandler responseHandler =
                {
                        &responsePropertiesCallback,
                        &responseCompleteCallback
                };

        S3PutObjectHandler putHandler =
                {
                        responseHandler,
                        &putObjectDataCallback
                };

//        S3_put_object(&bucketContext, path.c_str(), header->data_size, &putProperties, nullptr, &putHandler,
//                      reinterpret_cast<void*>(&putData));
        S3_put_object(&bucketContext, path.c_str(), json.size(), &putProperties, nullptr, &putHandler,
                      reinterpret_cast<void*>(&putData));

        if (putData.status != S3StatusOK) {
            throw HDCException(putData.error);
        }

        return;
    }
}

S3Serializer::S3Serializer()
{
    access_key_ = "SDKF33R4IJ4HGQ6XKJX1";
    secret_key_ = "i0XTHx4uwxpILPGHdsq+8kZxX14qnzUMirFpHFpg";
}

void
S3Serializer::serialize(const HDC& hdc, const std::string& filename, const std::string& datapath UNUSED)
{
    S3_initialize(nullptr, S3_INIT_ALL, nullptr);

    std::deque<std::string> tokens;
    boost::algorithm::split(tokens, filename, boost::is_any_of("/"));

    std::string host = tokens.front();
    tokens.pop_front();

    std::string bucket = tokens.front();
    tokens.pop_front();

    std::string path = boost::algorithm::join(tokens, "/");

    write_node(hdc, host.c_str(), bucket.c_str(), path);

    S3_deinitialize();
}

HDC S3Serializer::deserialize(const std::string& filename, const std::string& datapath UNUSED)
{
    S3_initialize(nullptr, S3_INIT_ALL, nullptr);

    std::deque<std::string> tokens;
    boost::algorithm::split(tokens, filename, boost::is_any_of("/"));

    std::string host = tokens.front();
    tokens.pop_front();

    std::string bucket = tokens.front();
    tokens.pop_front();

    std::string path = boost::algorithm::join(tokens, "/");

    HDC hdc = read(host.c_str(), bucket.c_str(), path);
    return hdc;

    S3_deinitialize();
}

std::string S3Serializer::to_string(const HDC& hdc UNUSED)
{
    return std::string();
}

HDC S3Serializer::from_string(const std::string& string UNUSED)
{
    return HDC();
}
