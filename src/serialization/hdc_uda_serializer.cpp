#include "hdc_uda_serializer.h"

#include "hdc.hpp"

#include <uda.h>
#include <c++/UDA.hpp>

namespace {

HDC udaData2HDC(uda::Data* uda_data, int rank)
{
    HDC result;
    auto& type = uda_data->type();
    auto type_name = type.name();
    if (type_name == typeid(char*).name()) {
        if (rank <= 1) {
            result.set_string(std::string(reinterpret_cast<const char*>(uda_data->byte_data())));
        } else if (rank == 2) {
            result.set_type(HDC_LIST);
            auto array = dynamic_cast<uda::Array*>(uda_data);
            auto dims = array->dims();
            std::vector<char> vec = array->as<char>();
            for (size_t i = 0; i < dims[1].size(); i++) {
                HDC n;
                n.set_string(std::string(vec.data() + i * dims[0].size(), strlen(vec.data() + i * dims[0].size())));
                result.append(n);
            }
        } else {
            throw HDCException("UDA string rank too high, not implemented yet\n");
        }
    } else if (uda_data->type().name() == typeid(std::string).name()) {
        auto array = dynamic_cast<uda::Array*>(uda_data);
        std::vector<std::string> strings = array->as<std::string>();
        for (size_t i = 0; i < strings.size(); i++) {
            HDC n;
            n.set_string(strings[i]);
            result.append(n);
        }
    } else if (type_name == typeid(short).name() || type_name == typeid(int).name() ||
               type_name == typeid(float).name() || type_name == typeid(double).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(uda_data);
            if (type_name == typeid(short).name()) result.set_data(value->as<short>());
            if (type_name == typeid(int).name()) result.set_data(value->as<int>());
            if (type_name == typeid(float).name()) result.set_data(value->as<float>());
            if (type_name == typeid(double).name()) result.set_data(value->as<double>());
        } else {
            uda::Array* value = dynamic_cast<uda::Array*>(uda_data);
            auto shape = value->shape();
            size_t rank = shape.size();
            vector<size_t> myshape(rank);
            for (size_t i = 0; i < rank; i++) myshape[i] = shape[i];
//             result.set_data(rank,myshape,value->byte_data(),static_cast<size_t>(to_typeid(type)));
            if (type_name == typeid(short).name()) result.set_data(myshape, &(value->as<short>())[0]);
            if (type_name == typeid(int).name()) result.set_data(myshape, &(value->as<int>())[0]);
            if (type_name == typeid(float).name()) result.set_data(myshape, &(value->as<float>())[0]);
            if (type_name == typeid(double).name()) result.set_data(myshape, &(value->as<double>())[0]);
        }
    } else if (type_name == typeid(char).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(uda_data);
            result.set_data(value->as<char>());
        } else {
            result.set_string(std::string(reinterpret_cast<const char*>(uda_data->byte_data())));
        }
    } else {
        throw HDCException("udaData2HDC(): Unsupported type: " + std::string(type_name) + "\n");
    }
    return result;
}

HDC udaTreeNode2HDC(uda::TreeNode& tree)
{
    HDC result;
    auto n_children = tree.numChildren();
    if (n_children) {
        if (n_children == 1) {
            auto children = tree.children();
            auto& ch = children[0];
            HDC n = udaTreeNode2HDC(ch);
            if (ch.name().length()) {
                result.add_child(ch.name(), n);
            } else {
                result = n;
            }
        } else {
            // This should be list
            for (auto& ch : tree.children()) {
                HDC n = udaTreeNode2HDC(ch);
                if (ch.name().length()) {
                    HDC nn;
                    nn.add_child(ch.name(), n);
                    result.append(nn);
                } else {
                    result.append(n);
                }
            }
        }
    } else {
        auto a_shape = tree.atomicShape();
        auto a_rank = tree.atomicRank();
        auto a_names = tree.atomicNames();
        auto a_types = tree.atomicTypes();
        auto a_pointers = tree.atomicPointers();
        auto a_count = tree.atomicCount();
        for (int i = 0; i < a_count; i++) {
            auto name = a_names[i];
            auto type_name = a_types[i];
            if (type_name == "STRING") {
                if (a_rank[i] <= 1) {
                    uda::Scalar value = tree.atomicScalar(name);
                    result[name].set_string(value.as<char*>());
                } else {
                    uda::Vector value = tree.atomicVector(name);
                    std::vector<char*> vec = value.as<char*>();
                    HDC list;
                    for (auto& v: vec) {
                        HDC ch;
                        ch.set_string(v);
                        list.append(ch);
                    }
                    result.add_child(name, list);
                }
            } else if (type_name == "STRING *") {
                uda::Vector value = tree.atomicVector(name);
                std::vector<char*> vec = value.as<char*>();
                HDC list;
                for (auto& v: vec) {
                    HDC ch;
                    ch.set_string(v);
                    list.append(ch);
                }
                result.add_child(name, list);
            } else if (type_name == "double" || type_name == "float" || type_name == "short" || type_name == "int" ||
                       type_name == "char") {
                if (a_rank[i] == 0) {
                    uda::Scalar value = tree.atomicScalar(name);
                    if (type_name == "short") result[name] = value.as<short>();
                    if (type_name == "int") result[name] = value.as<int>();
                    if (type_name == "float") result[name] = value.as<float>();
                    if (type_name == "double") result[name] = value.as<double>();
                } else {
                    uda::Array value = tree.atomicArray(name);
                    auto shape = value.shape();
                    auto rank = shape.size();
                    vector<size_t> myshape(rank);
                    for (size_t i = 0; i < rank; i++) myshape[i] = shape[i];
                    if (type_name == "short") result.set_data(myshape, &(value.as<short>())[0]);
                    if (type_name == "int") result.set_data(myshape, &(value.as<int>())[0]);
                    if (type_name == "float") result.set_data(myshape, &(value.as<float>())[0]);
                    if (type_name == "double") result.set_data(myshape, &(value.as<double>())[0]);
                }
            } else {
                if (a_rank[i] == 0) {
                    uda::Scalar value = tree.atomicScalar(name);
                    auto& type = value.type();
                    if (type.name() == typeid(int).name()) { result.set_data(value.as<int>()); }
                    else if (type.name() == typeid(short).name()) { result.set_data(value.as<short>()); }
                    else if (type.name() == typeid(double).name()) { result.set_data(value.as<double>()); }
                    else if (type.name() == typeid(float).name()) { result.set_data(value.as<float>()); }
                    else {
                        throw HDCException("udaTreeNode2HDC(): Unsupported type: " + std::string(type.name()) + "\n");
                    }
                } else {
                    uda::Array value = tree.atomicArray(name);
                    vector<size_t> myshape(HDC_MAX_DIMS);
                    int rank = a_rank[i];
                    std::vector<size_t> shape = value.shape();
                    for (int i = 0; i < rank; i++) myshape[i] = shape[i];
                    auto& type = value.type();
                    if (type.name() == typeid(int).name()) { result.set_data(myshape, &(value.as<int>())[0]); }
                    else if (type.name() == typeid(short).name()) {
                        result.set_data(myshape, &(value.as<short>()[0]));
                    } else if (type.name() == typeid(double).name()) {
                        result.set_data(myshape, &(value.as<double>()[0]));
                    } else if (type.name() == typeid(float).name()) {
                        result.set_data(myshape, &(value.as<float>()[0]));
                    } else {
                        throw HDCException("udaTreeNode2HDC(): Unsupported type: " + std::string(type.name()) + "\n");
                    }
                }
            }
        }
    }
    return result;
}

HDC udaResult2HDC(const uda::Result& uda_result, bool withMetadata = false)
{
    HDC result;
    HDC tmp;
    if (!uda_result.isTree()) {
        uda::Data* uda_data = uda_result.data();
        tmp = udaData2HDC(uda_data, uda_result.rank());
    } else {
        uda::TreeNode tree = uda_result.tree();
        tmp = udaTreeNode2HDC(tree);
    }
    if (withMetadata) {
        result.add_child("data", tmp);
        result["@label"].set_string(uda_result.label());
        result["@units"].set_string(uda_result.units());
        result["@description"].set_string(uda_result.description());
    } else {
        result = tmp;
    }
    return result;
}

} // anon namespace

void hdc::serialisation::UDASerialiser::serialize(const HDC& hdc, const std::string& filename, const std::string& datapath)
{

}

HDC hdc::serialisation::UDASerialiser::deserialize(const std::string& filename, const std::string& datapath)
{
    uda::Client client;
    try {
        const uda::Result& result = client.get(filename, "");
        return udaResult2HDC(result, with_metadata_);
    } catch (uda::UDAException& exc) {
        std::cerr << "Error fetching uda result\n" << "signalName:     " << filename << "\n" << std::endl;
        std::cerr << exc.what();
        return HDC();
    }
}

std::string hdc::serialisation::UDASerialiser::to_string(const HDC& hdc)
{
    return std::string();
}

HDC hdc::serialisation::UDASerialiser::from_string(const std::string& string)
{
    return HDC();
}
