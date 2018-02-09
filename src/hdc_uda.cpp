#include "hdc.hpp"

#ifdef _USE_UDA
#include <vector>
#include <uda/uda.h>
#include <c++/UDA.hpp>

HDC udaData2HDC(uda::Data* data, int rank) {
    std::cout << "--- udaData2HDC(Data*)\n";
    std::cout << "data.size       : " << data->size() << std::endl;
    std::cout << "data.type       : " << data->type().name() << std::endl;
    std::cout << "data.byte_length: " << data->byte_length() << std::endl;
    HDC h;
    auto& type = data->type();
    if (type.name() == typeid(char).name() || type.name() == typeid(char*).name()) {
        if (rank <= 1) {
            h.set_string(std::string(reinterpret_cast<const char*>(data->byte_data())));
        } else if (rank == 2) {
            auto array = dynamic_cast<uda::Array*>(data);
            auto dims = array->dims();
            std::cout << "array.size: " << dims[0].size() << " " << dims[1].size() << std::endl;
            std::vector<char> vec = array->as<char>();
            for (int i=0;i<dims[1].size();i++) {
                std::cout << "Setting string "<<"["+std::to_string(i)+"]\n";
                h.set_string("["+std::to_string(i)+"]",std::string(vec.data() + i * dims[0].size(), strlen(vec.data() + i * dims[0].size())));
            }
        } else {
            std::cerr << "UDA string rank too high, not implemented yet\n";
            exit(-1);
        }
    } else if (data->type().name() == typeid(std::string).name()) {
        auto array = dynamic_cast<uda::Array*>(data);
        std::vector<uda::Dim> dims = array->dims();
        std::vector<std::string> strings = array->as<std::string>();
        for (int i=0;i<strings.size();i++) {
            std::cout << "Setting string "<<"["+std::to_string(i)+"]\n";
            h.set_string("["+std::to_string(i)+"]",strings[i]);
        }
//             TODO: move to char* case??
    } else if (type.name() == typeid(uda::Scalar).name()) {
        std::cout << "here\n";
        exit(0);
    } else if (type.name() == typeid(int).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<int>());
        } else {
//           TODO: swapping conditions could posibly enable some code reuse
            uda::Array* value = dynamic_cast<uda::Array*>(data);
            auto shape = value->shape();
            auto ndim = shape.size();
            size_t myshape[ndim];
            for (int i=0;i<ndim;i++) myshape[i] = shape[i];
            auto data = value->as<short>();
            h.set_data(ndim,myshape,&data[0]);
        }
    } else if (type.name() == typeid(short).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<short>());
        } else {
            uda::Array* value = dynamic_cast<uda::Array*>(data);
            auto shape = value->shape();
            auto ndim = shape.size();
            size_t myshape[ndim];
            for (int i=0;i<ndim;i++) myshape[i] = shape[i];
            auto data = value->as<short>();
            h.set_data(ndim,myshape,&data[0]);
        }
    } else if (type.name() == typeid(double).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<double>());
        } else {
            uda::Array* value = dynamic_cast<uda::Array*>(data);
            auto shape = value->shape();
            auto ndim = shape.size();
            size_t myshape[ndim];
            for (int i=0;i<ndim;i++) myshape[i] = shape[i];
            auto data = value->as<double>();
            h.set_data(ndim,myshape,&data[0]);
        }
    } else if (type.name() == typeid(float).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<float>());
        } else {
            uda::Array* value = dynamic_cast<uda::Array*>(data);
            auto shape = value->shape();
            auto ndim = shape.size();
            size_t myshape[ndim];
            for (int i=0;i<ndim;i++) myshape[i] = shape[i];
            auto data = value->as<float>();
            h.set_data(ndim,myshape,&data[0]);
        }
    } else if (type.name() == typeid(char).name()) {
        if (rank == 0) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<char>());
        } else {
            std::cerr << "Unsupported rank\n";
        }
/*    } else if (type.name() == typeid(byte).name()) {
        if (rank <= 1) {
            uda::Scalar* value = dynamic_cast<uda::Scalar*>(data);
            std::cout << "Type " << data->type().name() << "\n";
            h.set_data(value->as<float>());
        } else {
            std::cerr << "Unsupported rank\n";
        }*/
    } else {
        std::cerr << "Type " << data->type().name() << " not yet supported\n";
        exit(0);
    }
    return h;
}

HDC udaTreeNode2HDC(uda::TreeNode& tree) {
    std::cout << "--- udaTreeNode2HDC\n";
    HDC h;
    auto n_children = tree.numChildren();
    std::cout << "tree.numChildren   : " << n_children << std::endl;
    std::cout << "tree.printNode   : " << std::endl;
    tree.printNode();
    std::cout << "tree.name   : " << tree.name() << std::endl;
    if (n_children) {
        if (n_children == 1) {
            auto children = tree.children();
            auto& ch = children[0];
            std::cout << ">> Adding single child: " << ch.name() << " " << ch.name().length() << std::endl;
            HDC n = udaTreeNode2HDC(ch);
            if (ch.name().length()) {
                h.add_child(ch.name(),n);
            } else {
                h = n;
            }
        } else {
            // This should be list
            for (auto& ch : tree.children()) {
                std::cout << ">> Adding multiple childs: " << ch.name() << " " << ch.name().length() << std::endl;
                HDC n = udaTreeNode2HDC(ch);
                if (ch.name().length()) {
                    std::cout << "/*-+/*-+/*-+ 1\n";
                    HDC nn;
                    nn.add_child(ch.name(),n);
                    h.append_slice(nn);
                } else {
                    std::cout << "/*-+/*-+/*-+ 2\n";
                    h.append_slice(n);
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
        std::cout << "tree.atomicCount   : " << a_count << std::endl;
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

        for (int i=0;i<a_count;i++) {
            std::cout << "+++++ "<< i <<"\n";
            auto name = a_names[i];
            if (a_types[i] == "STRING") {
                std::cout << "+++++ STRING\n";
                if (a_rank[i] <= 1) {
                    std::cout << "singlestring\n";
                    uda::Scalar value = tree.atomicScalar(name);
                    h.set_string(name,value.as<char*>());
                } else {
                    uda::Vector value = tree.atomicVector(name);
                    std::vector<char*> vec = value.as<char*>();
                    HDC list;
                    for (auto& v: vec) {
                        HDC ch;
                        ch.set_string(v);
                        list.append_slice(ch);
                    }
                    h.add_child(name,list);
                }
            } else if (a_types[i] == "STRING *") {
                std::cout << "+++++ STRING *\n";
                uda::Vector value = tree.atomicVector(name);
                std::vector<char*> vec = value.as<char*>();
                HDC list;
                for (auto& v: vec) {
                    HDC ch;
                    ch.set_string(v);
                    list.append_slice(ch);
                }
                h.add_child(name,list);
            } else if (a_types[i] == "double") {
                std::cout << "+++++ DOUBLE\n";
                if (a_rank[i] == 0) {
                    std::cout << "singledouble\n";
                    uda::Scalar value = tree.atomicScalar(name);
                    h.set_data(name,value.as<double>());
                } else {
                    std::cout << "DOUBLE ARRAY\n";
                    uda::Array value = tree.atomicArray(name);
                    auto shape = value.shape();
                    auto ndim = shape.size();
                    size_t myshape[ndim];
                    for (int i=0;i<ndim;i++) myshape[i] = shape[i];
                    auto data = value.as<double>();
                    h.set_data(ndim,myshape,&data[0]);
                }
            } else if (a_types[i] == "float") {
                std::cout << "+++++ FLOAT\n";
                if (a_rank[i] == 0) {
                    std::cout << "singlefloat\n";
                    uda::Scalar value = tree.atomicScalar(name);
                    h.set_data(name,value.as<float>());
                } else {
                    std::cout << "FLOAT ARRAY\n";
                    uda::Array value = tree.atomicArray(name);
                    auto shape = value.shape();
                    auto ndim = shape.size();
                    size_t myshape[ndim];
                    for (int i=0;i<ndim;i++) myshape[i] = shape[i];
                    auto data = value.as<float>();
                    h.set_data(ndim,myshape,&data[0]);
                }
            } else if (a_types[i] == "char") {
                std::cout << "+++++ CHAR\n";
                if (a_rank[i] == 0) {
                    std::cout << "singlechar\n";
                    uda::Scalar value = tree.atomicScalar(name);
                    h.set_data(name,value.as<char>());
                } else {
                    std::cerr << "Unsupported rank on char\n";
                        exit(0);
                }
            } else {
                std::cout << "**** something else: ****\n";
                if (a_rank[i] == 0) {
                    uda::Scalar value = tree.atomicScalar(name);
                    auto& type = value.type();
                    std::cout << "====== value.type() -> " << type.name() << std::endl;
                    if (type.name() == typeid(int).name()) {
                        h.set_data(value.as<int>());
                    } else if (type.name() == typeid(short).name()) {
                        h.set_data(value.as<short>());
                    } else if (type.name() == typeid(double).name()) {
                        h.set_data(value.as<double>());
                    } else if (type.name() == typeid(float).name()) {
                        h.set_data(value.as<float>());
                    } else {
                        std::cerr << "Unsupported type: " << type.name() << "\n";
                        exit(0);
                    }
                } else {
                    uda::Array value = tree.atomicArray(name);
                    size_t myshape[HDC_MAX_DIMS];
                    int ndim = a_rank[i];
                    std::vector<size_t> shape = value.shape();
                    for (int i=0;i<ndim;i++) myshape[i] = shape[i];
                    auto& type = value.type();
                    if (type.name() == typeid(int).name()) {
                        vector<int> vec = value.as<int>();
                        for (auto v : vec) {
                            std::cout << "___DATA: " << v << std::endl;
                        }
                        h.set_data(ndim,myshape,&vec[0]);
                    } else if (type.name() == typeid(short).name()) {
                        h.set_data(ndim,myshape,&(value.as<short>()[0]));
                    } else if (type.name() == typeid(double).name()) {
                        h.set_data(ndim,myshape,&(value.as<double>()[0]));
                    } else if (type.name() == typeid(float).name()) {
                        h.set_data(ndim,myshape,&(value.as<float>()[0]));
                    } else {
                        std::cerr << "Unsupported type: " << type.name() << "\n";
                        exit(0);
                    }
                }
            }
        }
    }
    return h;
}


HDC udaResult2HDC(const uda::Result& result) {
    std::cout << "--- udaResult2HDC\n";

    std::cout << "result.hasErrors   : " << result.hasErrors() << std::endl;
    std::cout << "result.type        : " << result.type().name() << std::endl;
    std::cout << "result.label       : " << result.label() << std::endl;
    std::cout << "result.units       : " << result.units() << std::endl;
    std::cout << "result.description : " << result.description() << std::endl;
    std::cout << "result.size        : " << result.size() << std::endl;
    std::cout << "result.rank        : " << result.rank() << std::endl;
    std::cout << "result.shape       : ";
    for (auto& s : result.shape()) {
        std::cout << " " << s;
    }
    std::cout << std::endl;

    std::cout << "result.isTree: " << result.isTree() << std::endl;

    HDC h;
    if (!result.isTree()) {
        uda::Data* data = result.data();
        HDC d = udaData2HDC(data,result.rank());
        h = d;
    } else {
        uda::TreeNode tree = result.tree();
        HDC n = udaTreeNode2HDC(tree);
        h = n;
    }
    return h;
};

HDC HDC::from_uda(const std::string& signalName, const std::string& dataSource) {
    uda::Client client;
    try {
        const uda::Result& result = client.get(signalName,dataSource);
        return udaResult2HDC(result);
    } catch (uda::UDAException &exc) {
        std::cerr << "Error fetching uda result\n" << "signalName:     " << signalName << "\ndataSource:     " << dataSource << std::endl;
        std::cerr << exc.what();
        return HDC();
    }
}

#else // _USE_UDA

HDC HDC::from_uda(const std::string& signalName, const std::string& dataSource) {
    std::cerr << "UDA backend has been disabled at compile time." << std::endl;
    exit(-1);
}

#endif // _USE_UDA
