#include "hdc.hpp"

#ifdef _USE_UDA

#include <vector>
#include <uda.h>
#include <c++/UDA.hpp>
#include <tuple>
#include <boost/tokenizer.hpp>
#include <boost/variant.hpp>
#include <typeinfo>

hdc_type_t UdaType2HDCType(size_t data_type)
{
    switch (data_type) {
        case UDA_TYPE_UNKNOWN:
            return HDC_EMPTY;
        case UDA_TYPE_CHAR:
            return HDC_INT8;
        case UDA_TYPE_SHORT:
            return HDC_INT16;
        case UDA_TYPE_INT:
            return HDC_INT32;
        case UDA_TYPE_UNSIGNED_INT:
            return HDC_UINT32;
        case UDA_TYPE_LONG:
            return HDC_INT64;
        case UDA_TYPE_FLOAT:
            return HDC_FLOAT;
        case UDA_TYPE_DOUBLE:
            return HDC_DOUBLE;
        case UDA_TYPE_UNSIGNED_CHAR:
            return HDC_UINT8;
        case UDA_TYPE_UNSIGNED_SHORT:
            return HDC_UINT16;
        case UDA_TYPE_UNSIGNED_LONG:
            return HDC_UINT64;
        case UDA_TYPE_LONG64:
            return HDC_INT64;
        case UDA_TYPE_UNSIGNED_LONG64:
            return HDC_UINT64;
        case UDA_TYPE_STRING:
            return HDC_STRING;
        default:
            throw HDCException("Unkown UDA data type: " + std::to_string(data_type));
    }
}


hdc_path_t split_uda(const std::string& s)
{
    hdc_path_t parts;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> sep("/", "");
    tokenizer tok{ s, sep };
    boost::variant<size_t, std::string> var;
    for (const auto& t : tok) {
        try {
            var = boost::lexical_cast<size_t>(t) - 1;
        } catch (boost::bad_lexical_cast& e) {
            var = t;
        }
        parts.push_back(var);
    }
    return parts;
}

HDC HDC::uda2HDC(const std::string& data_object, const std::string& data_source)
{
    D(std::cout << "HDC::uda2HDC()" << data_object << "\n";)

    int handle = idamGetAPI(data_object.c_str(), data_source.c_str());
    if (handle < 0) throw HDCException("idamGetAPI() Failed to obtain handle. handle = " + std::to_string(handle));
    DATA_BLOCK* result = (DATA_BLOCK*)getIdamDataBlock(handle);
    DATA_BLOCK* list = (DATA_BLOCK*)result->data;
    std::vector<DATA_BLOCK> items(list, list + result->data_n);

    HDC tree;

    for (const auto& item : items) {
        std::string path = item.data_desc;
        // Check for empty path and skip the node in such case:
        if (path.empty()) {
            std::cerr << "Warning: UDA has returned empty node. Skipping...\n";
            continue;
        }
        D(
                std::cout << "desc: " << item.data_desc << std::endl;
                std::cout << "type: " << item.data_type << std::endl;
                std::cout << "size: " << item.data_n << std::endl;
                std::cout << "rank: " << item.rank << std::endl;
                std::cout << " ----- " << path << "\n";
        )
        auto split_path = split_uda(path);

        if (item.rank > 1) throw HDCException("uda2HDC(): Rank > 1 is not supported yet");

        //TODO: more types, rank > 1, integer keys -> list (not struct)
        HDC empty;
        switch (item.data_type) {
            case UDA_TYPE_UNKNOWN:
                std::cerr << "Warning: UDA has returned UDA_TYPE_UNKNOWN at node " << path << " size = " << item.data_n
                          << std::endl;
                tree.add_child(split_path, empty);
                break;
            case UDA_TYPE_STRING: {
                std::string str = item.data;
                tree.get_or_create(split_path) = str;
                break;
            }
            case UDA_TYPE_CHAR:
            case UDA_TYPE_SHORT:
            case UDA_TYPE_INT:
            case UDA_TYPE_UNSIGNED_INT:
            case UDA_TYPE_LONG:
            case UDA_TYPE_FLOAT:
            case UDA_TYPE_DOUBLE:
            case UDA_TYPE_UNSIGNED_CHAR:
            case UDA_TYPE_UNSIGNED_SHORT:
            case UDA_TYPE_UNSIGNED_LONG:
            case UDA_TYPE_LONG64:
            case UDA_TYPE_UNSIGNED_LONG64: {
                auto type_ = UdaType2HDCType(item.data_type);
                std::vector<size_t> shp(item.rank);
                shp[0] = item.data_n;
                tree.get_or_create(split_path).set_data_c(shp, item.data, type_);
                break;
            }
            default:
                throw HDCException("Unkown UDA data type: " + std::to_string(item.data_type));
        }

    }
    return tree;
}

HDC udaData2HDC(uda::Data* uda_data, int rank)
{
/*
    std::cout << "--- udaData2HDC(Data*)\n";
    std::cout << "data.size       : " << data->size() << std::endl;
    std::cout << "data.type       : " << data->type().name() << std::endl;
    std::cout << "data.byte_length: " << data->byte_length() << std::endl;
*/
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
        std::vector<uda::Dim> dims = array->dims();
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
/*    } else if (type_name == typeid(byte).name()) {
        if (rank <= 1) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            result.set_data(value->as<float>());
        } else {
            std::cerr << "Unsupported rank\n";
        }*/
    } else {
        throw HDCException("udaData2HDC(): Unsupported type: " + std::string(type_name) + "\n");
    }
    return result;
}

HDC udaTreeNode2HDC(uda::TreeNode& tree)
{
    HDC result;
    auto n_children = tree.numChildren();
/*
    std::cout << "tree.numChildren   : " << n_children << std::endl;
    std::cout << "tree.printNode   : " << std::endl;
    tree.printNode();
    std::cout << "tree.name   : " << tree.name() << std::endl;
*/
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
/*        std::cout << "tree.atomicCount   : " << a_count << std::endl;
        std::cout << "tree.atomicShape       : ";
        for (auto shape : a_shape) {
            for (auto s : shape)
                std::cout << ", " << s;
        }
        std::cout << std::endl;
        std::cout << "tree.atomicRank        : ";
        for (auto r : a_rank) {
            std::cout << ", " << r;
        }
        std::cout << std::endl;
        std::cout << "tree.atomicNames       : ";
        for (auto r : a_names) {
            std::cout << ", " << r;
        }
        std::cout << std::endl;
        std::cout << "tree.atomicTypes       : ";
        for (auto r : a_types) {
            std::cout << ", " << r;
        }
        std::cout << std::endl;
        std::cout << "tree.atomicPointers       : ";
        for (auto r : a_pointers) {
            std::cout << ", " << r;
        }
        std::cout << std::endl;
        tree.printUserDefinedTypeTable();
        tree.printStructureNames();
*/
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


//                     if (type_name == "char") result.set_data(name,value.as<char>());
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
    /*
    std::cout << "--- udaResult2HDC\n";

    std::cout << "uda_result.hasErrors   : " << uda_result.hasErrors() << std::endl;
    std::cout << "uda_result.type        : " << uda_result.type().name() << std::endl;
    std::cout << "uda_result.label       : " << uda_result.label() << std::endl;
    std::cout << "uda_result.units       : " << uda_result.units() << std::endl;
    std::cout << "uda_result.description : " << uda_result.description() << std::endl;
    std::cout << "uda_result.size        : " << uda_result.size() << std::endl;
    std::cout << "uda_result.rank        : " << uda_result.rank() << std::endl;
    std::cout << "uda_result.shape       : ";
    for (auto& s : uda_result.shape()) {
        std::cout << " " << s;
    }
    std::cout << std::endl;

    std::cout << "uda_result.isTree: " << uda_result.isTree() << std::endl;
    */
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
};

HDC HDC::from_uda(const std::string& signalName, const std::string& dataSource, bool withMetadata)
{
    uda::Client client;
    try {
        const uda::Result& result = client.get(signalName, dataSource);
        return udaResult2HDC(result, withMetadata);
    } catch (uda::UDAException& exc) {
        std::cerr << "Error fetching uda result\n" << "signalName:     " << signalName << "\ndataSource:     "
                  << dataSource << std::endl;
        std::cerr << exc.what();
        return HDC();
    }
}

#else // _USE_UDA

HDC HDC::uda2HDC(const std::string& data_object UNUSED, const std::string& data_source UNUSED) {
    throw HDCException("UDA backend has been disabled at compile time.\n");
}

HDC HDC::from_uda(const std::string& signalName UNUSED, const std::string& dataSource UNUSED, bool withMetadata UNUSED) {
    throw HDCException("UDA backend has been disabled at compile time.\n");
}

#endif // _USE_UDA
