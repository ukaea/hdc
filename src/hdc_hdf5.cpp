#include <iostream>
#include "types.h"
#include "hdc.hpp"
#include <H5Cpp.h>

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

void HDC::write_node(H5File* file, std::string path) {
    auto buffer = storage->get(uuid);
    auto data = buffer + sizeof(header_t);
    H5std_string DATASET_NAME(path);
    memcpy(&header,buffer,sizeof(header_t));
    try {
        hsize_t rank = header.ndim;
        hsize_t dimsf[10];
        for (unsigned int i=0;i<rank;i++) {
            dimsf[i] = header.shape[i];
        };
        DataSpace dataspace(rank, dimsf);
        DataSet dataset;
        IntType i8datatype(PredType::NATIVE_INT8);
        i8datatype.setOrder(H5T_ORDER_LE);
        IntType i16datatype(PredType::NATIVE_INT16);
        i16datatype.setOrder(H5T_ORDER_LE);
        IntType i32datatype(PredType::NATIVE_INT32);
        i32datatype.setOrder(H5T_ORDER_LE);
        IntType i64datatype(PredType::NATIVE_INT64);
        i64datatype.setOrder(H5T_ORDER_LE);
        IntType u8datatype(PredType::NATIVE_UINT8);
        u8datatype.setOrder(H5T_ORDER_LE);
        IntType u16datatype(PredType::NATIVE_UINT16);
        u16datatype.setOrder(H5T_ORDER_LE);
        IntType u32datatype(PredType::NATIVE_UINT32);
        u32datatype.setOrder(H5T_ORDER_LE);
        IntType u64datatype(PredType::NATIVE_UINT64);
        u64datatype.setOrder(H5T_ORDER_LE);
        FloatType fdatatype(PredType::NATIVE_FLOAT);
        fdatatype.setOrder(H5T_ORDER_LE);
        FloatType ddatatype(PredType::NATIVE_DOUBLE);
        ddatatype.setOrder(H5T_ORDER_LE);
        StrType sdatatype(PredType::C_S1);

        hsize_t dims1[] = {1};
        DataSpace edataspace(1,dims1);
        double edata[] = {NAN};

        map_t* children;

        switch(header.type) {
            case STRUCT_ID:
                children = get_children_ptr();
                if (children != nullptr) {
                    Group* group = new Group(file->createGroup(path));
                    map_t::nth_index<1>::type& ri=children->get<1>();
                    for (auto it = ri.begin(); it != ri.end(); ++it) {
                        auto key = it->key.c_str();
                        auto uuid = it->address.c_str();
                        HDC h(storage,uuid);
                        h.write_node(file,path+"/"+key);
                    }
                    delete group;
                }
                return;
            case LIST_ID:
                children = get_children_ptr();
                if (children != nullptr) {
                    Group* group = new Group(file->createGroup(path));
                    map_t::nth_index<1>::type& ri=children->get<1>();
                    size_t i = 0;
                    for (auto it = ri.begin(); it != ri.end(); ++it) {
                        auto key = it->key.c_str();
                        auto uuid = it->address.c_str();
                        HDC h(storage,uuid);
                        h.write_node(file,path+"/"+to_string(i++));
                    }
                    delete group;
                }
                return;
            case BOOL_ID:
                dataset = file->createDataSet( DATASET_NAME, u8datatype, dataspace );
                dataset.write( data, PredType::NATIVE_UINT_LEAST8 );
                return;
            case INT8_ID:
                dataset = file->createDataSet( DATASET_NAME, i8datatype, dataspace );
                dataset.write( data, PredType::NATIVE_INT_LEAST8 );
                return;
            case INT16_ID:
                dataset = file->createDataSet( DATASET_NAME, i16datatype, dataspace );
                dataset.write( data, PredType::NATIVE_INT_LEAST16 );
                return;
            case INT32_ID:
                dataset = file->createDataSet( DATASET_NAME, i32datatype, dataspace );
                dataset.write( data, PredType::NATIVE_INT_LEAST32 );
                return;
            case INT64_ID:
                dataset = file->createDataSet( DATASET_NAME, i64datatype, dataspace );
                dataset.write( data, PredType::NATIVE_INT_LEAST64 );
                return;
            case UINT8_ID:
                dataset = file->createDataSet( DATASET_NAME, u8datatype, dataspace );
                dataset.write( data, PredType::NATIVE_UINT_LEAST8 );
                return;
            case UINT16_ID:
                dataset = file->createDataSet( DATASET_NAME, u16datatype, dataspace );
                dataset.write( data, PredType::NATIVE_UINT_LEAST16 );
                return;
            case UINT32_ID:
                dataset = file->createDataSet( DATASET_NAME, u32datatype, dataspace );
                dataset.write( data, PredType::NATIVE_UINT_LEAST32 );
                return;
            case UINT64_ID:
                dataset = file->createDataSet( DATASET_NAME, u64datatype, dataspace );
                dataset.write( data, PredType::NATIVE_UINT_LEAST64 );
                return;
            case FLOAT_ID:
                dataset = file->createDataSet( DATASET_NAME, fdatatype, dataspace );
                dataset.write( data, PredType::NATIVE_FLOAT );
                return;
            case DOUBLE_ID:
                dataset = file->createDataSet( DATASET_NAME, ddatatype, dataspace );
                dataset.write( data, PredType::NATIVE_DOUBLE );
                return;
            case STRING_ID:
                dataset = file->createDataSet( DATASET_NAME, sdatatype, dataspace );
                dataset.write( data, PredType::C_S1 );
                return;
            case EMPTY_ID:
                dataset = file->createDataSet( DATASET_NAME, ddatatype, edataspace );
                dataset.write( edata, PredType::NATIVE_DOUBLE );
                return;
            default:
                cout << "Unknown data type."<<header.type<<"\n";
                exit(-5);
        }
    }  // end of try block
    // catch failure caused by the H5File operations
    catch( FileIException error )
    {
        error.printError();
        exit(-1);
    }
    // catch failure caused by the DataSet operations
    catch( DataSetIException error )
    {
        error.printError();
        exit(-1);
    }
    // catch failure caused by the DataSpace operations
    catch( DataSpaceIException error )
    {
        error.printError();
        exit(-1);
    }
    // catch failure caused by the DataSpace operations
    catch( DataTypeIException error )
    {
        error.printError();
        exit(-1);
    }
    return;
}

void HDC::to_hdf5(std::string filename, std::string dataset_name) {
    try {
        H5std_string FILE_NAME( filename );
        H5File* file = new H5File(FILE_NAME, H5F_ACC_TRUNC);
        write_node(file,"data");
        delete file;
    }  // end of try block
    catch( FileIException error )
    {
        error.printError();
        exit(-1);
    }
    return;
}

HDC from_hdf5(H5File* file, std::string dataset_name) {
    cout << "from_hdf5(H5File* file, std::string dataset_name)\n";getchar();
    
    try {
        herr_t      status;
        H5G_info_t  ginfo;
    }
    catch (Exception error) {
        error.printError();
        exit(111);
    }
    try {
        DataSet dataset = file->openDataSet( dataset_name );
        H5T_class_t type_class = dataset.getTypeClass();
        switch(type_class)
        {
            case H5T_INTEGER:
                cout << "integer\n";
                break;
            case H5T_FLOAT:
                cout << "float\n";
                break;
            case H5T_STRING:
                cout << "string\n";
                break;
            case H5T_VLEN:
                cout << "vlen\n";
                break;
            case H5T_ARRAY:
                cout << "array\n";
                break;
            default:
                std::cout << "from_hdf5(): Unknown datatype: " << type_class << ".\n";
                exit(5);
        }
    }
    // catch failure caused by the DataSet operations
    catch( Exception error )
    {
        error.printError();
        exit(222);
    }
    return HDC();
}

HDC from_hdf5(std::string filename, std::string dataset_name) {
    cout << "from_hdf5(std::string filename, std::string dataset_name)\n";
    try {
        H5std_string FILE_NAME( filename );
        H5File* file = new H5File(FILE_NAME, H5F_ACC_RDONLY);
        cout << "opened\n";
        return from_hdf5(file,dataset_name);
        delete file;
    }  // end of try block
    catch( FileIException error )
    {
        error.printError();
        exit(111);
    }
    return HDC();
}
