#include "serialization/hdc_flatbuffers_serializer.h"

#include "hdc.hpp"

#include "hdc_generated.h"

using namespace hdc::serialization;

namespace {

size_t get_data_size(const hdc_header_t& hdc_header)
{
    size_t size = 1;
    for (size_t i = 0; i < hdc_header.rank; ++i) {
        size *= hdc_header.shape[i];
    }
    return size;
}

template <typename T>
struct TypeConverter {
    static Data type;
};

template <> Data TypeConverter<int8_t>::type = Data::Data_Int8Data;
template <> Data TypeConverter<int16_t>::type = Data::Data_Int16Data;
template <> Data TypeConverter<int32_t>::type = Data::Data_Int32Data;
template <> Data TypeConverter<int64_t>::type = Data::Data_Int64Data;
template <> Data TypeConverter<uint8_t>::type = Data::Data_UInt8Data;
template <> Data TypeConverter<uint16_t>::type = Data::Data_UInt16Data;
template <> Data TypeConverter<uint32_t>::type = Data::Data_UInt32Data;
template <> Data TypeConverter<uint64_t>::type = Data::Data_UInt64Data;
template <> Data TypeConverter<float>::type = Data::Data_FloatData;
template <> Data TypeConverter<double>::type = Data::Data_DoubleData;
template <> Data TypeConverter<std::string>::type = Data::Data_StringData;
template <> Data TypeConverter<bool>::type = Data::Data_BoolData;

template <typename T>
flatbuffers::Offset<void>
get_data(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data);

template <>
flatbuffers::Offset<void>
get_data<int8_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int8_t*>(hdc_data), get_data_size(hdc_header));
    return CreateInt8Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<int16_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int16_t*>(hdc_data), get_data_size(hdc_header));
    return CreateInt16Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<int32_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int32_t*>(hdc_data), get_data_size(hdc_header));
    return CreateInt32Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<int64_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int64_t*>(hdc_data), get_data_size(hdc_header));
    return CreateInt64Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<uint8_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint8_t*>(hdc_data), get_data_size(hdc_header));
    return CreateUInt8Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<uint16_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint16_t*>(hdc_data), get_data_size(hdc_header));
    return CreateUInt16Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<uint32_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint32_t*>(hdc_data), get_data_size(hdc_header));
    return CreateUInt32Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<uint64_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint64_t*>(hdc_data), get_data_size(hdc_header));
    return CreateUInt64Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<float>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const float*>(hdc_data), get_data_size(hdc_header));
    return CreateFloatData(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<double>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const double*>(hdc_data), get_data_size(hdc_header));
    return CreateDoubleData(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<std::string>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header UNUSED, const char* hdc_data UNUSED)
{
    const std::vector<std::string> vec{ std::string{ hdc_data } };
    auto data = builder.CreateVectorOfStrings(vec);
    return CreateStringData(builder, data).Union();
}

template <>
flatbuffers::Offset<void>
get_data<bool>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint8_t*>(hdc_data), get_data_size(hdc_header));
    return CreateBoolData(builder, data).Union();
}

flatbuffers::Offset<void>
get_data(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    switch (hdc_header.type) {
        case HDC_INT8:
            return get_data<int8_t>(builder, hdc_header, hdc_data);
        case HDC_INT16:
            return get_data<int16_t>(builder, hdc_header, hdc_data);
        case HDC_INT32:
            return get_data<int32_t>(builder, hdc_header, hdc_data);
        case HDC_INT64:
            return get_data<int64_t>(builder, hdc_header, hdc_data);
        case HDC_UINT8:
            return get_data<uint8_t>(builder, hdc_header, hdc_data);
        case HDC_UINT16:
            return get_data<uint16_t>(builder, hdc_header, hdc_data);
        case HDC_UINT32:
            return get_data<uint32_t>(builder, hdc_header, hdc_data);
        case HDC_UINT64:
            return get_data<uint64_t>(builder, hdc_header, hdc_data);
        case HDC_FLOAT:
            return get_data<float>(builder, hdc_header, hdc_data);
        case HDC_DOUBLE:
            return get_data<double>(builder, hdc_header, hdc_data);
        case HDC_STRING:
            return get_data<std::string>(builder, hdc_header, hdc_data);
        case HDC_BOOL:
            return get_data<bool>(builder, hdc_header, hdc_data);
        default:
            throw HDCException(std::string{ "Unknown data type: " } + hdc_type_str(hdc_header.type));
    }
}

Data get_type(const hdc_header_t& hdc_header)
{
    switch (hdc_header.type) {
        case HDC_INT8:
            return TypeConverter<int8_t>::type;
        case HDC_INT16:
            return TypeConverter<int16_t>::type;
        case HDC_INT32:
            return TypeConverter<int32_t>::type;
        case HDC_INT64:
            return TypeConverter<int64_t>::type;
        case HDC_UINT8:
            return TypeConverter<uint8_t>::type;
        case HDC_UINT16:
            return TypeConverter<uint16_t>::type;
        case HDC_UINT32:
            return TypeConverter<uint32_t>::type;
        case HDC_UINT64:
            return TypeConverter<uint64_t>::type;
        case HDC_FLOAT:
            return TypeConverter<float>::type;
        case HDC_DOUBLE:
            return TypeConverter<double>::type;
        case HDC_STRING:
            return TypeConverter<std::string>::type;
        case HDC_BOOL:
            return TypeConverter<bool>::type;
        default:
            throw HDCException(std::string{ "Unknown data type: " } + hdc_type_str(hdc_header.type));
    }
}

flatbuffers::Offset<HDCBuffer> write_node(const HDC& hdc, flatbuffers::FlatBufferBuilder& builder)
{
    auto hdc_buffer = hdc.get_buffer();
    auto hdc_header = reinterpret_cast<hdc_header_t*>(hdc_buffer);
    auto hdc_data = hdc_buffer + sizeof(hdc_header_t);
    std::vector<char> new_buffer(hdc_header->buffer_size);
    if (hdc.is_fortranorder()) {
        transpose_buffer(new_buffer.data() + sizeof(hdc_header_t), hdc.get_buffer() + sizeof(hdc_header_t),
                         hdc.get_rank(),
                         hdc.get_shape(), (hdc_type_t)hdc.get_type(),
                         hdc.is_fortranorder());
        hdc_data = new_buffer.data() + sizeof(hdc_header_t);
    }
    if (hdc_header->type == HDC_STRUCT) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            std::vector<flatbuffers::Offset<DictItem>> children_vec;

            for (const auto& child : hdc_children->get<1>()) {
                auto key = child.key;
                auto uuid = child.address;
                auto node = write_node(HDC(hdc_global.storage, uuid), builder);
                auto elem = CreateDictItem(builder, builder.CreateString(key), node);
                children_vec.push_back(elem);
            }

            auto children = builder.CreateVector(children_vec);
            auto dict = CreateDict(builder, children);
            auto buffer = CreateHDCBuffer(builder, Collection_Dict, dict.Union());
            builder.Finish(buffer);
            return buffer;
        }
    } else if (hdc_header->type == HDC_LIST) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            std::vector<flatbuffers::Offset<HDCBuffer>> children_vec;

            for (const auto& child : hdc_children->get<1>()) {
                auto uuid = child.address;
                auto node = write_node(HDC(hdc_global.storage, uuid), builder);
                children_vec.push_back(node);
            }

            auto children = builder.CreateVector(children_vec);
            auto list = CreateList(builder, children);
            auto buffer = CreateHDCBuffer(builder, Collection_List, list.Union());
            builder.Finish(buffer);
            return buffer;
        }
    } else if (hdc_header->type == HDC_EMPTY) {
        auto buffer = CreateHDCBuffer(builder);
        builder.Finish(buffer);
        return buffer;
    } else if (hdc_header->type == HDC_STRING) {
        auto data_type = get_type(*hdc_header);
        auto data = get_data(builder, *hdc_header, hdc_data);
        uint64_t size = get_data_size(*hdc_header);
        auto shape = builder.CreateVector(std::vector<uint64_t>{});

        auto buffer = CreateHDCBuffer(builder, Collection_NONE, 0, data_type, data, size, shape);
        builder.Finish(buffer);
        return buffer;
    } else {
        auto data_type = get_type(*hdc_header);

        auto data = get_data(builder, *hdc_header, hdc_data);
        uint64_t size = get_data_size(*hdc_header);

        std::vector<uint64_t> vec;
        vec.resize(hdc_header->rank);
        for (size_t i = 0; i < hdc_header->rank; ++i) {
            vec[i] = hdc_header->shape[i];
        }
        auto shape = builder.CreateVector(vec);

        auto buffer = CreateHDCBuffer(builder, Collection_NONE, 0, data_type, data, size, shape);
        builder.Finish(buffer);
        return buffer;
    }

    return CreateHDCBuffer(builder);
}

HDC read_data(const HDCBuffer& hdc_buffer)
{
    HDC hdc = {};
    if (hdc_buffer.data_type() != Data_NONE) {
        std::vector<size_t> shape{ hdc_buffer.shape()->begin(), hdc_buffer.shape()->end() };

        switch (hdc_buffer.data_type()) {
            case Data_DoubleData:
                hdc.set_data(shape, hdc_buffer.data_as_DoubleData()->data()->data());
                break;
            case Data_FloatData:
                hdc.set_data(shape, hdc_buffer.data_as_FloatData()->data()->data());
                break;
            case Data_Int8Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int8Data()->data()->data());
                break;
            case Data_Int16Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int16Data()->data()->data());
                break;
            case Data_Int32Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int32Data()->data()->data());
                break;
            case Data_Int64Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int64Data()->data()->data());
                break;
            case Data_UInt8Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt8Data()->data()->data());
                break;
            case Data_UInt16Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt16Data()->data()->data());
                break;
            case Data_UInt32Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt32Data()->data()->data());
                break;
            case Data_UInt64Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt64Data()->data()->data());
                break;
            case Data_StringData: {
                auto vec = hdc_buffer.data_as_StringData()->data();
                hdc.set_string((*vec)[0]->c_str());
                break;
            }
            case Data_BoolData:
                hdc.set_data(shape, reinterpret_cast<const bool*>(hdc_buffer.data_as_BoolData()->data()->data()));
                break;
            default:
                throw HDCException(std::string{ "Unknown data type " } + EnumNameData(hdc_buffer.data_type()));
        }
    }
    return hdc;
}

HDC to_hdc(const HDCBuffer& hdc_buffer)
{
    switch (hdc_buffer.children_type()) {
        case Collection::Collection_NONE: {
            HDC hdc = read_data(hdc_buffer);
            return hdc;
        }
        case Collection::Collection_Dict: {
            HDC hdc = read_data(hdc_buffer);
            auto dict = hdc_buffer.children_as_Dict()->items();
            for (uint32_t i = 0; i < dict->size(); ++i) {
                auto item = dict->Get(i);
                auto key = item->key();
                auto value = item->value();
                HDC child = to_hdc(*value);
                hdc.add_child(key->str(), child);
            }
            return hdc;
        }
        case Collection::Collection_List: {
            HDC hdc = read_data(hdc_buffer);
            auto list = hdc_buffer.children_as_List()->items();
            for (uint32_t i = 0; i < list->size(); ++i) {
                auto item = list->Get(i);
                auto el = to_hdc(*item);
                hdc.insert(i, el);
            }
            return hdc;
        }
        default:
            throw HDCException(std::string{ "Unknown child type " } + EnumNameCollection(hdc_buffer.children_type()));
    }
}

} // anon namespace

void FlatBuffersSerializer::serialize(const HDC& hdc, const std::string& filename, const std::string& datapath UNUSED)
{
    flatbuffers::FlatBufferBuilder builder(1024);
    write_node(hdc, builder);
    std::ofstream out{ filename, std::ios::binary };
    out.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());
}

HDC FlatBuffersSerializer::deserialize(const std::string& filename, const std::string& datapath UNUSED)
{
    std::ifstream in{ filename, std::ios::binary };
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(in), {});
    uint8_t* buffer_ptr = buffer.data();
    auto hdc_buffer = GetHDCBuffer(buffer_ptr);
    return to_hdc(*hdc_buffer);
}

std::string FlatBuffersSerializer::to_string(const HDC& hdc)
{
    flatbuffers::FlatBufferBuilder builder(1024);
    write_node(hdc, builder);
    return { reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize() };
}

HDC FlatBuffersSerializer::from_string(const std::string& string)
{
    auto hdc_buffer = GetHDCBuffer(string.data());
    return to_hdc(*hdc_buffer);
}
