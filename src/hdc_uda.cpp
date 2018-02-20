#include "hdc.hpp"

#ifdef _USE_UDA
#include <vector>
#include <uda/uda.h>
#include <c++/UDA.hpp>

HDC udaData2HDC(uda::Data* uda_data, int rank) {
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
            result.set_type(LIST_ID);
            auto array = dynamic_cast<uda::Array*>(uda_data);
            auto dims = array->dims();
            std::vector<char> vec = array->as<char>();
            for (int i=0;i<dims[1].size();i++) {
                HDC n;
                n.set_string(std::string(vec.data() + i * dims[0].size(), strlen(vec.data() + i * dims[0].size())));
                result.append_slice(n);
            }
        } else {
            std::cerr << "UDA string rank too high, not implemented yet\n";
            exit(-1);
        }
    } else if (uda_data->type().name() == typeid(std::string).name()) {
        auto array = dynamic_cast<uda::Array*>(uda_data);
        std::vector<uda::Dim> dims = array->dims();
        std::vector<std::string> strings = array->as<std::string>();
        for (int i=0;i<strings.size();i++) {
            HDC n;
            n.set_string(strings[i]);
            result.append_slice(n);
        }
    } else if (type_name == typeid(short).name() || type_name == typeid(int).name() || type_name == typeid(float).name() || type_name == typeid(double).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(uda_data);
            if (type_name == typeid(short).name()) result.set_data(value->as<short>());
            if (type_name == typeid(int).name()) result.set_data(value->as<int>());
            if (type_name == typeid(float).name()) result.set_data(value->as<float>());
            if (type_name == typeid(double).name()) result.set_data(value->as<double>());
        } else {
            uda::Array* value = dynamic_cast<uda::Array*>(uda_data);
            auto shape = value->shape();
            auto ndim = shape.size();
            size_t myshape[ndim];
            for (int i=0;i<ndim;i++) myshape[i] = shape[i];
//             result.set_data(ndim,myshape,value->byte_data(),static_cast<size_t>(to_typeid(type)));
            if (type_name == typeid(short).name()) result.set_data(ndim,myshape,&(value->as<short>())[0]);
            if (type_name == typeid(int).name()) result.set_data(ndim,myshape,&(value->as<int>())[0]);
            if (type_name == typeid(float).name()) result.set_data(ndim,myshape,&(value->as<float>())[0]);
            if (type_name == typeid(double).name()) result.set_data(ndim,myshape,&(value->as<double>())[0]);
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
        throw HDCException("udaData2HDC(): Unsupported type: "+std::string(type_name)+"\n");
    }
    return result;
}

HDC udaTreeNode2HDC(uda::TreeNode& tree) {
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
                result.add_child(ch.name(),n);
            } else {
                result = n;
            }
        } else {
            // This should be list
            for (auto& ch : tree.children()) {
                HDC n = udaTreeNode2HDC(ch);
                if (ch.name().length()) {
                    HDC nn;
                    nn.add_child(ch.name(),n);
                    result.append_slice(nn);
                } else {
                    result.append_slice(n);
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
        for (int i=0;i<a_count;i++) {
            auto name = a_names[i];
            auto type_name = a_types[i];
            if (type_name == "STRING") {
                if (a_rank[i] <= 1) {
                    uda::Scalar value = tree.atomicScalar(name);
                    result.set_string(name,value.as<char*>());
                } else {
                    uda::Vector value = tree.atomicVector(name);
                    std::vector<char*> vec = value.as<char*>();
                    HDC list;
                    for (auto& v: vec) {
                        HDC ch;
                        ch.set_string(v);
                        list.append_slice(ch);
                    }
                    result.add_child(name,list);
                }
            } else if (type_name == "STRING *") {
                uda::Vector value = tree.atomicVector(name);
                std::vector<char*> vec = value.as<char*>();
                HDC list;
                for (auto& v: vec) {
                    HDC ch;
                    ch.set_string(v);
                    list.append_slice(ch);
                }
                result.add_child(name,list);
            } else if (type_name == "double" || type_name == "float" || type_name == "short" || type_name == "int" || type_name == "char") {
                if (a_rank[i] == 0) {
                    uda::Scalar value = tree.atomicScalar(name);
                    if (type_name == "short") result.set_data(name,value.as<short>());
                    if (type_name == "int") result.set_data(name,value.as<int>());
                    if (type_name == "float") result.set_data(name,value.as<float>());
                    if (type_name == "double") result.set_data(name,value.as<double>());
//                     if (type_name == "char") result.set_data(name,value.as<char>());
                } else {
                    uda::Array value = tree.atomicArray(name);
                    auto shape = value.shape();
                    auto ndim = shape.size();
                    size_t myshape[ndim];
                    for (int i=0;i<ndim;i++) myshape[i] = shape[i];
                    if (type_name == "short") result.set_data(ndim,myshape,&(value.as<short>())[0]);
                    if (type_name == "int") result.set_data(ndim,myshape,&(value.as<int>())[0]);
                    if (type_name == "float") result.set_data(ndim,myshape,&(value.as<float>())[0]);
                    if (type_name == "double") result.set_data(ndim,myshape,&(value.as<double>())[0]);
//                     if (type_name == "char") {
//                         if (a_rank[i] > 1) {
//                             std::cerr << "Unsupported rank for \"char\"\n";
//                             exit(1);
//                         }
//                         result.set_string(name,&(value.as<char*>())[0]);
//                     }
                }
            } else {
                if (a_rank[i] == 0) {
                    uda::Scalar value = tree.atomicScalar(name);
                    auto& type = value.type();
                    if (type.name() == typeid(int).name()) result.set_data(value.as<int>());
                    else if (type.name() == typeid(short).name()) result.set_data(value.as<short>());
                    else if (type.name() == typeid(double).name()) result.set_data(value.as<double>());
                    else if (type.name() == typeid(float).name()) result.set_data(value.as<float>());
                    else {
                        throw HDCException("udaTreeNode2HDC(): Unsupported type: "+std::string(type.name())+"\n");
                    }
                } else {
                    uda::Array value = tree.atomicArray(name);
                    size_t myshape[HDC_MAX_DIMS];
                    int ndim = a_rank[i];
                    std::vector<size_t> shape = value.shape();
                    for (int i=0;i<ndim;i++) myshape[i] = shape[i];
                    auto& type = value.type();
                    if (type.name() == typeid(int).name()) result.set_data(ndim,myshape,&(value.as<int>())[0]);
                    else if (type.name() == typeid(short).name()) result.set_data(ndim,myshape,&(value.as<short>()[0]));
                    else if (type.name() == typeid(double).name()) result.set_data(ndim,myshape,&(value.as<double>()[0]));
                    else if (type.name() == typeid(float).name()) result.set_data(ndim,myshape,&(value.as<float>()[0]));
                    else {
                        throw HDCException("udaTreeNode2HDC(): Unsupported type: "+std::string(type.name())+"\n");
                    }
                }
            }
        }
    }
    return result;
}


HDC udaResult2HDC(const uda::Result& uda_result, bool withMetadata=false) {
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
        tmp = udaData2HDC(uda_data,uda_result.rank());
    } else {
        uda::TreeNode tree = uda_result.tree();
        tmp = udaTreeNode2HDC(tree);
    }
    if (withMetadata) {
        result.add_child("data",tmp);
        result.set_string("@label",uda_result.label());
        result.set_string("@units",uda_result.units());
        result.set_string("@description",uda_result.description());
    }
    else
        result = tmp;
    return result;
};

HDC HDC::from_uda(const std::string& signalName, const std::string& dataSource, bool withMetadata) {
    uda::Client client;
    try {
        const uda::Result& result = client.get(signalName,dataSource);
        return udaResult2HDC(result,withMetadata);
    } catch (uda::UDAException &exc) {
        std::cerr << "Error fetching uda result\n" << "signalName:     " << signalName << "\ndataSource:     " << dataSource << std::endl;
        std::cerr << exc.what();
        return HDC();
    }
}


#else // _USE_UDA

HDC HDC::from_uda(const std::string& signalName, const std::string& dataSource, bool withMetadata) {
    throw HDCException("UDA backend has been disabled at compile time.\n");
}

#endif // _USE_UDA
