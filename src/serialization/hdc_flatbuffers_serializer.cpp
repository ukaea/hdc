#include "hdc_flatbuffers_serializer.h"

#include "hdc.hpp"

#include "hdc_generated.h"

const char* ref_group_name = "__hdc";

size_t get_data_size(const hdc_header_t& hdc_header)
{
    size_t size = 1;
    for (int i = 0; i < hdc_header.rank; ++i) {
        size *= hdc_header.shape[i];
    }
    return size;
}

template <typename T>
struct TypeConverter {
    static hdc::serialization::Data type;
};

template <> hdc::serialization::Data TypeConverter<int8_t>::type = hdc::serialization::Data::Data_Int8Data;
template <> hdc::serialization::Data TypeConverter<int16_t>::type = hdc::serialization::Data::Data_Int16Data;
template <> hdc::serialization::Data TypeConverter<int32_t>::type = hdc::serialization::Data::Data_Int32Data;
template <> hdc::serialization::Data TypeConverter<int64_t>::type = hdc::serialization::Data::Data_Int64Data;
template <> hdc::serialization::Data TypeConverter<uint8_t>::type = hdc::serialization::Data::Data_UInt8Data;
template <> hdc::serialization::Data TypeConverter<uint16_t>::type = hdc::serialization::Data::Data_UInt16Data;
template <> hdc::serialization::Data TypeConverter<uint32_t>::type = hdc::serialization::Data::Data_UInt32Data;
template <> hdc::serialization::Data TypeConverter<uint64_t>::type = hdc::serialization::Data::Data_UInt64Data;
template <> hdc::serialization::Data TypeConverter<float>::type = hdc::serialization::Data::Data_FloatData;
template <> hdc::serialization::Data TypeConverter<double>::type = hdc::serialization::Data::Data_DoubleData;
template <> hdc::serialization::Data TypeConverter<bool>::type = hdc::serialization::Data::Data_BoolData;

template <typename T>
flatbuffers::Offset<void> get_data(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data);

template <>
flatbuffers::Offset<void> get_data<int8_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int8_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateInt8Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<int16_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int16_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateInt16Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<int32_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int32_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateInt32Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<int64_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const int64_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateInt64Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<uint8_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint8_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateUInt8Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<uint16_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint16_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateUInt16Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<uint32_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint32_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateUInt32Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<uint64_t>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint64_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateUInt64Data(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<float>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const float*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateFloatData(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<double>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const double*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateDoubleData(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<char*>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    const std::vector<std::string> vec{}; // TODO: How does HDC handle strings?
    auto data = builder.CreateVectorOfStrings(vec);
    return hdc::serialization::CreateStringData(builder, data).Union();
}

template <>
flatbuffers::Offset<void> get_data<bool>(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    auto data = builder.CreateVector(reinterpret_cast<const uint8_t*>(hdc_data), get_data_size(hdc_header));
    return hdc::serialization::CreateBoolData(builder, data).Union();
}

flatbuffers::Offset<void> get_data(flatbuffers::FlatBufferBuilder& builder, const hdc_header_t& hdc_header, const char* hdc_data)
{
    switch (hdc_header.type) {
        case HDC_INT8: return get_data<int8_t>(builder, hdc_header, hdc_data);
        case HDC_INT16: return get_data<int16_t>(builder, hdc_header, hdc_data);
        case HDC_INT32: return get_data<int32_t>(builder, hdc_header, hdc_data);
        case HDC_INT64: return get_data<int64_t>(builder, hdc_header, hdc_data);
        case HDC_UINT8: return get_data<uint8_t>(builder, hdc_header, hdc_data);
        case HDC_UINT16: return get_data<uint16_t>(builder, hdc_header, hdc_data);
        case HDC_UINT32: return get_data<uint32_t>(builder, hdc_header, hdc_data);
        case HDC_UINT64: return get_data<uint64_t>(builder, hdc_header, hdc_data);
        case HDC_FLOAT: return get_data<float>(builder, hdc_header, hdc_data);
        case HDC_DOUBLE: return get_data<double>(builder, hdc_header, hdc_data);
        case HDC_STRING: return get_data<char*>(builder, hdc_header, hdc_data);
        case HDC_BOOL: return get_data<bool>(builder, hdc_header, hdc_data);
        default:
            throw HDCException();
    }
}

hdc::serialization::Data get_type(const hdc_header_t& hdc_header)
{
    switch (hdc_header.type) {
        case HDC_INT8: return TypeConverter<int8_t>::type;
        case HDC_INT16: return TypeConverter<int16_t>::type;
        case HDC_INT32: return TypeConverter<int32_t>::type;
        case HDC_INT64: return TypeConverter<int64_t>::type;
        case HDC_UINT8: return TypeConverter<int8_t>::type;
        case HDC_UINT16: return TypeConverter<int16_t>::type;
        case HDC_UINT32: return TypeConverter<int32_t>::type;
        case HDC_UINT64: return TypeConverter<int64_t>::type;
        case HDC_FLOAT: return TypeConverter<float>::type;
        case HDC_DOUBLE: return TypeConverter<double>::type;
        case HDC_BOOL: return TypeConverter<bool>::type;
        default:
            throw HDCException();
    }
}


void write_node(const HDC& hdc, flatbuffers::FlatBufferBuilder& builder, hdc::serialization::HDCBufferBuilder& hdc_buffer_builder)
{
    auto hdc_buffer = hdc.get_buffer();
    auto hdc_header = reinterpret_cast<hdc_header_t*>(hdc_buffer);
    auto hdc_data = hdc_buffer + sizeof(hdc_header_t);
    std::vector<char> new_buffer(hdc_header->buffer_size);
    if (hdc.is_fortranorder()) {
        transpose_buffer(new_buffer.data() + sizeof(hdc_header_t), hdc.get_buffer() + sizeof(hdc_header_t), hdc.get_rank(),
                         hdc.get_shape(), (hdc_type_t)hdc.get_type(),
                         hdc.is_fortranorder());
        hdc_data = new_buffer.data() + sizeof(hdc_header_t);
    }
    if (hdc_header->type == HDC_STRUCT) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            std::vector<flatbuffers::Offset<hdc::serialization::DictItem>> children_vec;

            for (const auto& child : hdc_children->get<1>()) {
                auto key = child.key;
                auto uuid = child.address.c_str();
                hdc::serialization::HDCBufferBuilder hdc_child_buffer_builder(builder);
                write_node(HDC(hdc_global.storage, uuid), builder, hdc_child_buffer_builder);
                auto elem = hdc::serialization::CreateDictItem(builder, builder.CreateString(key), hdc_child_buffer_builder.Finish());
                children_vec.push_back(elem);
            }

            auto children = builder.CreateVector(children_vec);
            auto dict = hdc::serialization::CreateDict(builder, children);
            hdc_buffer_builder.add_children_type(hdc::serialization::Collection::Collection_Dict);
            hdc_buffer_builder.add_children(dict.Union());
        }
        return;
    } else if (hdc_header->type == HDC_LIST) {
        hdc_map_t* hdc_children = hdc.get_children_ptr();
        if (hdc_children != nullptr) {
            std::vector<flatbuffers::Offset<hdc::serialization::HDCBuffer>> children_vec;

            for (const auto& child : hdc_children->get<1>()) {
                auto uuid = child.address.c_str();
                //TODO: We could probably re-do this using get_children() method
                HDC hh(hdc_global.storage, uuid);
                hdc::serialization::HDCBufferBuilder hdc_child_buffer_builder(builder);
                write_node(HDC(hdc_global.storage, uuid), builder, hdc_child_buffer_builder);
            }

            auto children = builder.CreateVector(children_vec);
            auto list = hdc::serialization::CreateList(builder, children);
            hdc_buffer_builder.add_children_type(hdc::serialization::Collection::Collection_List);
            hdc_buffer_builder.add_children(list.Union());
        }
        return;
    } else if (hdc_header->type == HDC_EMPTY) {
        return;
    } else {
        auto uuid = builder.CreateString(hdc.get_uuid());

        auto data_type = get_type(*hdc_header);
        auto data = get_data(builder, *hdc_header, hdc_data);
        uint64_t size = get_data_size(*hdc_header);

        std::vector<uint64_t> vec{ hdc_header->rank };
        for (size_t i = 0; i < hdc_header->rank; ++i) {
            vec[i] = hdc_header->shape[i];
        }
        auto shape = builder.CreateVector(vec);

        hdc_buffer_builder.add_uuid(uuid);
        hdc_buffer_builder.add_data_type(data_type);
        hdc_buffer_builder.add_data(data);
        hdc_buffer_builder.add_size(size);
        hdc_buffer_builder.add_shape(shape);

        return;
    }
}

HDC read_data(const hdc::serialization::HDCBuffer& hdc_buffer)
{
    HDC hdc = {};
    if (hdc_buffer.data_type() != hdc::serialization::Data_NONE) {
        std::vector<size_t> shape{ hdc_buffer.shape()->begin(), hdc_buffer.shape()->end() };

        switch (hdc_buffer.data_type()) {
            case hdc::serialization::Data_DoubleData:
                hdc.set_data(shape, hdc_buffer.data_as_DoubleData()->data()->data()); break;
            case hdc::serialization::Data_FloatData:
                hdc.set_data(shape, hdc_buffer.data_as_FloatData()->data()->data()); break;
            case hdc::serialization::Data_Int8Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int8Data()->data()->data()); break;
            case hdc::serialization::Data_Int16Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int16Data()->data()->data()); break;
            case hdc::serialization::Data_Int32Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int32Data()->data()->data()); break;
            case hdc::serialization::Data_Int64Data:
                hdc.set_data(shape, hdc_buffer.data_as_Int64Data()->data()->data()); break;
            case hdc::serialization::Data_UInt8Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt8Data()->data()->data()); break;
            case hdc::serialization::Data_UInt16Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt16Data()->data()->data()); break;
            case hdc::serialization::Data_UInt32Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt32Data()->data()->data()); break;
            case hdc::serialization::Data_UInt64Data:
                hdc.set_data(shape, hdc_buffer.data_as_UInt64Data()->data()->data()); break;
//            case hdc::serialization::Data_StringData:
//                hdc.set_data(shape, hdc_buffer.data_as_StringData()->data()->data()); break;
            case hdc::serialization::Data_BoolData:
                hdc.set_data(shape, hdc_buffer.data_as_BoolData()->data()->data()); break;
            default:
                throw HDCException(std::string{ "Unknown data type " } + EnumNameData(hdc_buffer.data_type()));
        }
    }
    return hdc;
}

HDC to_hdc(const hdc::serialization::HDCBuffer& hdc_buffer)
{
    switch (hdc_buffer.children_type()) {
        case hdc::serialization::Collection::Collection_NONE: {
            HDC hdc = read_data(hdc_buffer);
            return hdc;
        }
        case hdc::serialization::Collection::Collection_Dict: {
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
        case hdc::serialization::Collection::Collection_List: {
            HDC hdc = read_data(hdc_buffer);
            auto list = hdc_buffer.children_as_List()->items();
            for (uint32_t i = 0; i < list->size(); ++i) {
                auto item = list->Get(i);
                hdc[i] = to_hdc(*item);
            }
            return hdc;
        }
        default:
            throw HDCException(std::string{ "Unknown child type " } + EnumNameCollection(hdc_buffer.children_type()));
    }
}

void hdc::serialization::FlatBuffersSerializer::serialize(const HDC& hdc, const std::string& filename,
                                                          const std::string& datapath)
{
    flatbuffers::FlatBufferBuilder builder(1024);
    hdc::serialization::HDCBufferBuilder hdc_buffer_builder(builder);
    auto hdc_buffer = hdc_buffer_builder.Finish();
    builder.Finish(hdc_buffer);
    std::ofstream out{ filename, std::ios::binary };
    out.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());
}

HDC hdc::serialization::FlatBuffersSerializer::deserialize(const std::string& filename, const std::string& datapath)
{
    std::ifstream in{ filename, std::ios::binary };
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(in), {});
    uint8_t* buffer_ptr = buffer.data();
    auto hdc_buffer = hdc::serialization::GetHDCBuffer(buffer_ptr);
    return to_hdc(*hdc_buffer);
}

std::string hdc::serialization::FlatBuffersSerializer::to_string(const HDC& hdc)
{
    flatbuffers::FlatBufferBuilder builder(1024);
    hdc::serialization::HDCBufferBuilder hdc_buffer_builder(builder);
    auto hdc_buffer = hdc_buffer_builder.Finish();
    builder.Finish(hdc_buffer);
    return { reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize() };
}

HDC hdc::serialization::FlatBuffersSerializer::from_string(const std::string& string)
{
    return HDC();
}
