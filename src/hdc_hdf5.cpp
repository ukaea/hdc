// TODO: add license

#include "hdc_hdf5.h"

#ifdef _USE_HDF5

const char* ref_group_name = "__hdc";


void hdf5_tree_to_hdc(hid_t hdf5_id, const std::string& ref_path, HDC& dest);

void write_node(HDC h, H5File* file, std::string path)
{
    auto buffer = h.get_buffer();
    hdc_header_t header;
    memcpy(&header, buffer, sizeof(hdc_header_t));

    auto data = buffer+sizeof(hdc_header_t);
    char* transposed_data = nullptr;
    if (h.is_fortranorder()) {
        transposed_data = transpose_buffer(h.get_buffer(), h.get_rank(), h.get_shape(), (hdc_type_t)h.get_type(),
                                           h.is_fortranorder());
        data = transposed_data;
    }
    H5std_string DATASET_NAME(path);
    memcpy(&header, buffer, sizeof(hdc_header_t));
    try {
        hsize_t rank = header.rank;
        hsize_t dimsf[10];
        for (unsigned int i = 0; i < rank; i++) {
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

        hsize_t dims1[] = { 1 };
        DataSpace edataspace(1, dims1);
        double edata[] = { NAN };

        hdc_map_t* children;

        switch (header.type) {
            case HDC_STRUCT:
                children = h.get_children_ptr();
                if (children != nullptr) {
                    Group* group = new Group(file->createGroup(path));
                    hdc_map_t::nth_index<1>::type& ri = children->get<1>();
                    for (auto it = ri.begin(); it != ri.end(); ++it) {
                        auto key = it->key.c_str();
                        auto uuid = it->address.c_str();
                        write_node(HDC(global_storage, uuid), file, path + "/" + key);
                    }
                    delete group;
                }
                return;
            case HDC_LIST:
                children = h.get_children_ptr();
                if (children != nullptr) {
                    Group ref_group;
                    try {
                        Exception::dontPrint(); //TODO: Do this just temporarily???
                        ref_group = file->openGroup(ref_group_name);
                    } catch (FileIException& e) {
                        ref_group = file->createGroup(ref_group_name);
                    }
                    hdc_map_t::nth_index<1>::type& ri = children->get<1>();
                    size_t n_child = children->size();
                    hobj_ref_t* wbuf = new hobj_ref_t[n_child];
                    size_t i = 0;
                    for (auto it = ri.begin(); it != ri.end(); ++it) {
                        auto uuid = it->address.c_str();
                        HDC h(global_storage, uuid);
                        auto _uuid = h.get_uuid();
//                         write_node(h,file,path+"/"+to_string(i++));
                        std::string full_path = std::string(ref_group_name) + "/" + _uuid;
                        write_node(h, file, full_path.c_str());
                        auto ret = H5Rcreate(&wbuf[i],file->getId(),full_path.c_str(),H5R_OBJECT,-1);
                        if (ret < 0) throw HDCException("Could not create refference to " + full_path);
                        i++;
                    }

                    hsize_t ref_dims[1];
                    ref_dims[0] = n_child;
                    DataSpace ref_dataspace(1, ref_dims);
                    DataSet ref_dataset = file->createDataSet(DATASET_NAME, PredType::STD_REF_OBJ, ref_dataspace);
                    ref_dataset.write(wbuf, PredType::STD_REF_OBJ);

//                     delete group;
                    delete[] wbuf;
                }
                return;
            case HDC_BOOL:
                dataset = file->createDataSet(DATASET_NAME, u8datatype, dataspace);
                dataset.write(data, PredType::NATIVE_UINT_LEAST8);
                return;
            case HDC_INT8:
                dataset = file->createDataSet(DATASET_NAME, i8datatype, dataspace);
                dataset.write(data, PredType::NATIVE_INT_LEAST8);
                return;
            case HDC_INT16:
                dataset = file->createDataSet(DATASET_NAME, i16datatype, dataspace);
                dataset.write(data, PredType::NATIVE_INT_LEAST16);
                return;
            case HDC_INT32:
                dataset = file->createDataSet(DATASET_NAME, i32datatype, dataspace);
                dataset.write(data, PredType::NATIVE_INT_LEAST32);
                return;
            case HDC_INT64:
                dataset = file->createDataSet(DATASET_NAME, i64datatype, dataspace);
                dataset.write(data, PredType::NATIVE_INT_LEAST64);
                return;
            case HDC_UINT8:
                dataset = file->createDataSet(DATASET_NAME, u8datatype, dataspace);
                dataset.write(data, PredType::NATIVE_UINT_LEAST8);
                return;
            case HDC_UINT16:
                dataset = file->createDataSet(DATASET_NAME, u16datatype, dataspace);
                dataset.write(data, PredType::NATIVE_UINT_LEAST16);
                return;
            case HDC_UINT32:
                dataset = file->createDataSet(DATASET_NAME, u32datatype, dataspace);
                dataset.write(data, PredType::NATIVE_UINT_LEAST32);
                return;
            case HDC_UINT64:
                dataset = file->createDataSet(DATASET_NAME, u64datatype, dataspace);
                dataset.write(data, PredType::NATIVE_UINT_LEAST64);
                return;
            case HDC_FLOAT:
                dataset = file->createDataSet(DATASET_NAME, fdatatype, dataspace);
                dataset.write(data, PredType::NATIVE_FLOAT);
                return;
            case HDC_DOUBLE:
                dataset = file->createDataSet(DATASET_NAME, ddatatype, dataspace);
                dataset.write(data, PredType::NATIVE_DOUBLE);
                return;
            case HDC_STRING:
                dataset = file->createDataSet(DATASET_NAME, sdatatype, dataspace);
                dataset.write(data, PredType::C_S1);
                return;
            case HDC_EMPTY:
                dataset = file->createDataSet(DATASET_NAME, ddatatype, edataspace);
                dataset.write(edata, PredType::NATIVE_DOUBLE);
                return;
            default:
                throw HDCException("Unknown data type." + std::to_string(header.type) + "\n");

        }
        if (transposed_data != NULL) delete[] transposed_data;
    }  // end of try block
        // catch failure caused by the H5File operations
    catch (FileIException& error) {
        #if H5_VERSION_GE(1,8,13)
            error.printErrorStack();
        #else
            std::cerr << "write_node(): " << "FileIException" << std::endl;
        #endif
        exit(-1);
    }
        // catch failure caused by the DataSet operations
    catch (DataSetIException& error) {
        #if H5_VERSION_GE(1,8,13)
            error.printErrorStack();
        #else
            std::cerr << "write_node(): " << "DataSetIException" << std::endl;
        #endif
        exit(-1);
    }
        // catch failure caused by the DataSpace operations
    catch (DataSpaceIException& error) {
        #if H5_VERSION_GE(1,8,13)
            error.printErrorStack();
        #else
            std::cerr << "write_node(): " << "DataSpaceIException" << std::endl;
        #endif
        exit(-1);
    }
        // catch failure caused by the DataSpace operations
    catch (DataTypeIException& error) {
        #if H5_VERSION_GE(1,8,13)
            error.printErrorStack();
        #else
            std::cerr << "write_node(): " << "DataTypeIException" << std::endl;
        #endif
        exit(-1);
    }
    return;
}

void HDC::to_hdf5(std::string filename, std::string dataset_name UNUSED)
{
    try {
        H5std_string FILE_NAME(filename);
        H5File file(FILE_NAME, H5F_ACC_TRUNC);
        write_node(HDC(this->storage, this->uuid), &file, "data");
    }  // end of try block
    catch (FileIException& error) {
        #if H5_VERSION_GE(1,8,13)
            error.printErrorStack();
        #else
            std::cerr << "to_hdf5(): " << "FileIException" << std::endl;
        #endif
        exit(-1);
    }
    return;
}

hdc_type_t hdf5_type_to_hdc_type(hid_t hdf5_dtype_id, const std::string& ref_path UNUSED)
{
    DEBUG_STDOUT("hdf5_type_to_hdc_type(" + to_string(hdf5_dtype_id) + "," + ref_path + ")");
    hdc_type_t res;
    if (H5Tequal(hdf5_dtype_id, H5T_STD_I8LE)) {
        res = HDC_INT8;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_I16LE)) {
        res = HDC_INT16;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_I32LE)) {
        res = HDC_INT32;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_I64LE)) {
        res = HDC_INT64;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_U8LE)) {
        res = HDC_UINT8;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_U16LE)) {
        res = HDC_UINT16;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_U32LE)) {
        res = HDC_UINT32;
    } else if (H5Tequal(hdf5_dtype_id, H5T_STD_U64LE)) {
        res = HDC_UINT64;
    } else if (H5Tequal(hdf5_dtype_id, H5T_IEEE_F32LE)) {
        res = HDC_FLOAT;
    } else if (H5Tequal(hdf5_dtype_id, H5T_IEEE_F64LE)) {
        res = HDC_DOUBLE;
    } else if (H5Tequal(hdf5_dtype_id, H5T_C_S1)) {
        res = HDC_STRING;
    } else if (H5Tequal(hdf5_dtype_id, H5T_ENUM)) {
        //TODO: Here it can be probably more things.
        res = HDC_BOOL;
    } else {
        throw HDCException("Error with HDF5 DataType to DataType Leaf Conversion");
    }
    return res;
};

void hdf5_dataset_to_hdc(hid_t hdf5_dset_id, const std::string& ref_path, HDC& dest)
{
    DEBUG_STDOUT("hdf5_dataset_to_hdc(" + to_string(hdf5_dset_id) + "," + ref_path + ",...)");
    hid_t h5_dspace_id = H5Dget_space(hdf5_dset_id);
    HDC_CHECK_HDF5_ERROR_WITH_REF(h5_dspace_id,
                                  ref_path,
                                  "Error reading HDF5 Dataspace: "
                                          << hdf5_dset_id);
    // check for empty case
    if (H5Sget_simple_extent_type(h5_dspace_id) == H5S_NULL) {
        // change to empty
        dest.set_type(HDC_EMPTY);
    } else {

        hid_t h5_dtype_id = H5Dget_type(hdf5_dset_id);

        if (H5Tget_class(h5_dtype_id) == H5T_REFERENCE) {
            // Inspired by this: https://www.physics.ohio-state.edu/~wilkins/computing/HDF/hdf5tutorial/reftoobj.html
            size_t nelems = H5Sget_simple_extent_npoints(h5_dspace_id);
            size_t rank = H5Sget_simple_extent_ndims(h5_dspace_id);
            if (rank > 1) throw HDCException("Cannot handle array of refferences of rank > 1");
            hobj_ref_t ref_out[nelems];
            hid_t h5_status = H5Dread(hdf5_dset_id, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, ref_out);
            HDC_CHECK_HDF5_ERROR_WITH_REF(h5_status,
                                      ref_path,
                                      "Error reading HDF5 Dataset: "
                                              << hdf5_dset_id);
            dest.set_type(HDC_LIST);
            for (size_t i = 0; i < nelems; i++) {
                // C function H5Rdereference renamed to H5Rdereference1 and deprecated in this release.
                // https://support.hdfgroup.org/HDF5/doc/RM/RM_H5R.html#Reference-Dereference1
#if ((H5_VERS_MAJOR == 1) && (H5_VERS_MINOR < 10))
                auto dsetv_id = H5Rdereference(hdf5_dset_id, H5R_OBJECT, &ref_out[i]);
#else
                // TODO Use the new H5Rdereference API
                auto dsetv_id = H5Rdereference1(hdf5_dset_id, H5R_OBJECT, &ref_out[i]);
#endif
                HDC h;
                hdf5_tree_to_hdc(dsetv_id, "", h);
                dest.append(h);
            }
            return;
        }
        HDC_CHECK_HDF5_ERROR_WITH_REF(h5_dtype_id,
                                      ref_path,
                                      "Error reading HDF5 Datatype: "
                                              << hdf5_dset_id);

        size_t nelems = H5Sget_simple_extent_npoints(h5_dspace_id);
        size_t rank = H5Sget_simple_extent_ndims(h5_dspace_id);
        hdc_type_t dt = hdf5_type_to_hdc_type(h5_dtype_id, ref_path);
        if (dt == HDC_STRING) nelems++;
        hid_t h5_status = 0;
        char buffer[nelems * hdc_sizeof(dt)];
        memset(&buffer, 0, nelems * hdc_sizeof(dt));
        h5_status = H5Dread(hdf5_dset_id, h5_dtype_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, &buffer);
        hsize_t hshape[rank];
        H5Sget_simple_extent_dims(h5_dspace_id, hshape, NULL);
        std::vector<size_t> shape(rank);
        for (size_t i = 0; i < rank; i++) shape[i] = hshape[i];
        if (dt == HDC_STRING) {
            dest.set_string(buffer);
        } else { dest.set_data_c(shape, buffer, dt); } //TODO: do something more inteligent here
        HDC_CHECK_HDF5_ERROR_WITH_REF(h5_status,
                                      ref_path,
                                      "Error reading HDF5 Dataset: "
                                              << hdf5_dset_id);

        HDC_CHECK_HDF5_ERROR_WITH_REF(H5Tclose(h5_dtype_id),
                                      ref_path,
                                      "Error closing HDF5 Datatype: "
                                              << h5_dtype_id);

    }

    HDC_CHECK_HDF5_ERROR_WITH_REF(H5Sclose(h5_dspace_id),
                                  ref_path,
                                  "Error closing HDF5 Dataspace: "
                                          << h5_dspace_id);

};

int h5_group_check(h5_read_opdata* od, haddr_t target_addr)
{
    DEBUG_STDOUT("h5_group_check(h5_read_opdata *od, haddr_t target_addr)");
    if (od->addr == target_addr) {
        /* Addresses match */
        return 1;
    } else if (!od->recurs) {
        /* Root group reached with no matches */
        return 0;
    } else {
        /* Recursively examine the next node */
        return h5_group_check(od->prev, target_addr);
    }
}


herr_t h5_literate_traverse_op_func(hid_t hdf5_id, const char* hdf5_path, const H5L_info_t*, void* hdf5_operator_data)
{
    DEBUG_STDOUT(
            "h5_literate_traverse_op_func(hid_t hdf5_id,const char *hdf5_path,const H5L_info_t *, void *hdf5_operator_data)");
    herr_t h5_status = 0;
    herr_t h5_return_val = 0;
    H5O_info_t h5_info_buf;

    /* Type conversion */
    struct h5_read_opdata* h5_od = (struct h5_read_opdata*)hdf5_operator_data;


    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library.
     */
    h5_status = H5Oget_info_by_name(hdf5_id,
                                    hdf5_path,
                                    &h5_info_buf,
                                    H5P_DEFAULT);

    HDC_CHECK_HDF5_ERROR_WITH_REF(h5_status,
                                  h5_od->ref_path,
                                  "Error fetching HDF5 Object info: "
                                          << " parent: " << hdf5_id
                                          << " path:" << hdf5_path);

    std::string chld_ref_path = h5_od->ref_path +
                                std::string("/") +
                                std::string(hdf5_path);
    switch (h5_info_buf.type) {
        case H5O_TYPE_GROUP: {
            /*
             * Check group address against linked list of operator
             * data structures.  We will always run the check, as the
             * reference count cannot be relied upon if there are
             * symbolic links, and H5Oget_info_by_name always follows
             * symbolic links.  Alternatively we could use H5Lget_info
             * and never recurse on groups discovered by symbolic
             * links, however it could still fail if an object's
             * reference count was manually manipulated with
             * H5Odecr_refcount.
             */
            if (h5_group_check(h5_od, h5_info_buf.addr)) {
                // skip cycles in the graph ...
            } else {
                hid_t h5_group_id = H5Gopen(hdf5_id,
                                            hdf5_path,
                                            H5P_DEFAULT);
                HDC_CHECK_HDF5_ERROR_WITH_REF(h5_group_id,
                                              h5_od->ref_path,
                                              "Error opening HDF5 "
                                                      << "Group: "
                                                      << " parent: "
                                                      << hdf5_id
                                                      << " path:"
                                                      << hdf5_path);

                // execute traversal for this group
                HDC chld_node;
                h5_od->node->add_child(hdf5_path, chld_node);
                hdf5_group_to_hdc(h5_group_id,
                                  chld_ref_path,
                                  chld_node);

                // close the group
                HDC_CHECK_HDF5_ERROR_WITH_REF(H5Gclose(h5_group_id),
                                              h5_od->ref_path,
                                              "Error closing HDF5 "
                                                      << "Group: "
                                                      << h5_group_id);
            }
            break;
        }
        case H5O_TYPE_DATASET: {
            HDC leaf;
            h5_od->node->add_child(hdf5_path, leaf);
            // open hdf5 dataset at path
            hid_t h5_dset_id = H5Dopen(hdf5_id,
                                       hdf5_path,
                                       H5P_DEFAULT);

            HDC_CHECK_HDF5_ERROR_WITH_REF(h5_dset_id,
                                          h5_od->ref_path,
                                          "Error opening HDF5 "
                                                  << " Dataset: "
                                                  << " parent: "
                                                  << hdf5_id
                                                  << " path:"
                                                  << hdf5_path);

            hdf5_dataset_to_hdc(h5_dset_id,
                                chld_ref_path,
                                leaf);

            // close the dataset
            HDC_CHECK_HDF5_ERROR_WITH_REF(H5Dclose(h5_dset_id),
                                          h5_od->ref_path,
                                          "Error closing HDF5 "
                                                  << " Dataset: "
                                                  << h5_dset_id);
            break;
        }
        default: {
            throw HDCException("h5_literate_traverse_op_func(): Unsupported H5O_TYPE\n");
        }
    }

    return h5_return_val;
}

void hdf5_group_to_hdc(hid_t hdf5_group_id, const std::string& ref_path, HDC& dest)
{
    DEBUG_STDOUT("void hdf5_group_to_hdc(" + to_string(hdf5_group_id) + "," + ref_path + "," + ")");
    //dest.set_type(HDC_EMPTY);

    // get info, we need to get the obj addr for cycle tracking
    H5O_info_t h5_info_buf;
    herr_t h5_status = H5Oget_info(hdf5_group_id,
                                   &h5_info_buf);

    // setup the callback struct we will use for  H5Literate
    struct h5_read_opdata h5_od;
    // setup linked list tracking that allows us to detect cycles
    h5_od.recurs = 0;
    h5_od.prev = NULL;
    h5_od.addr = h5_info_buf.addr;
    // attach the pointer to our node
    h5_od.node = &dest;
    // keep ref path
    h5_od.ref_path = ref_path;

    H5_index_t h5_grp_index_type = H5_INDEX_NAME;

    // check for creation order index using propertylist

    hid_t h5_gc_plist = H5Gget_create_plist(hdf5_group_id);

    if (HDC_HDF5_VALID_ID(h5_gc_plist)) {
        unsigned int h5_gc_flags = 0;
        h5_status = H5Pget_link_creation_order(h5_gc_plist,
                                               &h5_gc_flags);

        // first make sure we have the link creation order plist
        if (HDC_HDF5_STATUS_OK(h5_status)) {
            // check that we have both order_tracked and order_indexed
            if (h5_gc_flags & (H5P_CRT_ORDER_TRACKED | H5P_CRT_ORDER_INDEXED)) {
                // if so, we can use creation order in h5literate
                h5_grp_index_type = H5_INDEX_CRT_ORDER;
            }
        }

        HDC_CHECK_HDF5_ERROR_WITH_REF(H5Pclose(h5_gc_plist),
                                      ref_path,
                                      "Failed to close HDF5 "
                                              << "H5P_GROUP_CREATE "
                                              << "property list: "
                                              << h5_gc_plist);
    }

    // use H5Literate to traverse
    h5_status = H5Literate(hdf5_group_id,
                           h5_grp_index_type,
                           H5_ITER_INC,
                           NULL,
                           h5_literate_traverse_op_func,
                           (void*)&h5_od);

    HDC_CHECK_HDF5_ERROR_WITH_REF(h5_status,
                                  ref_path,
                                  "Error calling H5Literate to "
                                          << "traverse and read HDF5 "
                                          << "hierarchy: "
                                          << hdf5_group_id);
};

void hdf5_tree_to_hdc(hid_t hdf5_id, const std::string& ref_path, HDC& dest)
{
    DEBUG_STDOUT("hdf5_tree_to_hdc(hid_t hdf5_id, const std::string  &ref_path, HDC& dest)");
    herr_t h5_status = 0;
    H5O_info_t h5_info_buf;
    h5_status = H5Oget_info(hdf5_id, &h5_info_buf);
    HDC_CHECK_HDF5_ERROR_WITH_REF(h5_status, ref_path, "Error fetching HDF5 object " << "info from: " << hdf5_id);
    switch (h5_info_buf.type) {
        // if hdf5_id + hdf5_path points to a group,
        // use a H5Literate traversal
        case H5O_TYPE_GROUP: {
            hdf5_group_to_hdc(hdf5_id, ref_path, dest);
            break;
        }
            // if hdf5_id + hdf5_path points directly to a dataset
            // skip the H5Literate traversal
        case H5O_TYPE_DATASET: {
            hdf5_dataset_to_hdc(hdf5_id, ref_path, dest);
            break;
        }
            // unsupported types
        case H5O_TYPE_UNKNOWN: {
            HDC_HDF5_ERROR(ref_path, "Cannot read HDF5 Object : " << "(type == H5O_TYPE_UNKNOWN )");
            break;
        }
        case H5O_TYPE_NAMED_DATATYPE: {
            HDC_HDF5_ERROR(ref_path, "Cannot read HDF5 Object " << "(type == H5O_TYPE_NAMED_DATATYPE )");
            break;
        }
        case H5O_TYPE_NTYPES: {
            HDC_HDF5_ERROR(ref_path,
                           "Cannot read HDF5 Object " << "(type == H5O_TYPE_NTYPES [This is an invalid HDF5 type!]");
            break;
        }
        default: {
            HDC_HDF5_ERROR(ref_path, "Cannot read HDF5 Object (type == Unknown )");
        }
    }
};

void hdf5_read(hid_t hdf5_id, std::string hdf5_path, HDC& dest)
{
    DEBUG_STDOUT("hdf5_read(hid_t hdf5_id, std::string hdf5_path, HDC& dest)");
    // get hdf5 object at path, then call read_hdf5_tree_into_conduit_node
    hid_t h5_child_obj = H5Oopen(hdf5_id, hdf5_path.c_str(), H5P_DEFAULT);
    HDC_CHECK_HDF5_ERROR(h5_child_obj, "Failed to fetch HDF5 object from: " << hdf5_id << ":" << hdf5_path);
    hdf5_tree_to_hdc(h5_child_obj, hdf5_path, dest);
    HDC_CHECK_HDF5_ERROR(H5Oclose(h5_child_obj), "Failed to close HDF5 Object: " << h5_child_obj);
};

void hdf5_read(const std::string& file_path, const std::string& hdf5_path, HDC& node)
{
    DEBUG_STDOUT("hdf5_read(const std::string& file_path, const std::string& hdf5_path, HDC& node)");
    // open file
    hid_t h5_file_id = H5Fopen(file_path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    HDC_CHECK_HDF5_ERROR(h5_file_id, "Error opening HDF5 file for reading: " << file_path);
    hdf5_read(h5_file_id, hdf5_path, node);
    // close the hdf5 file
    HDC_CHECK_HDF5_ERROR(H5Fclose(h5_file_id), "Error closing HDF5 file: " << file_path);
};

HDC from_hdf5(hid_t file, const std::string& dataset_name)
{
    DEBUG_STDOUT("from_hdf5(hid_t file, const std::string& dataset_name)");
    HDC h;
    hdf5_read(file, dataset_name, h);
    return h;
};

HDC HDC::from_hdf5(const std::string& filename, const std::string& dataset_name)
{
    DEBUG_STDOUT("from_hdf5(const std::string& filename, const std::string& dataset_name)");
    HDC h;
    if (dataset_name != "") {
        hdf5_read(filename, dataset_name, h);
    } else {
        hdf5_read(filename, "/data", h);
    }
    return h;
};

#else // _USE_HDF5
void to_hdf5(std::string filename, std::string dataset_name) {
    throw HDCException("HDC has not been compiled with HDF5 support!\n");
}
HDC HDC::from_hdf5(const std::string& filename, const std::string& dataset_name) {
    throw HDCException("HDC has not been compiled with HDF5 support!\n");
}
#endif // _USE_HDF5
