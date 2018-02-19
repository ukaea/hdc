#ifndef HDC_HDF5_H
#define HDC_HDF5_H

#include <iostream>
#include "types.h"
#include "hdc.hpp"
#include <H5Cpp.h>
#include <hdf5.h>

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

#define HDC_HDF5_VALID_ID( hdf5_id )   hdf5_id  >= 0

#define HDC_HDF5_STATUS_OK(hdf5_id )   hdf5_id  >= 0

#define HDC_CHECK_HDF5_ERROR( hdf5_err, msg )                             \
{                                                                         \
    if( hdf5_err < 0 )                                                    \
    {                                                                     \
        cerr << "HDF5 Error (error code: "                                \
            <<  hdf5_err                                                  \
            <<  ") " << msg;                                              \
            exit(111);                                                    \
    }                                                                     \
}

#define HDC_CHECK_HDF5_ERROR_WITH_REF( hdf5_err, ref_path, msg )          \
{                                                                         \
    if( hdf5_err < 0 )                                                    \
    {                                                                     \
        cerr << "HDF5 Error (error code: "                                \
            <<  hdf5_err                                                  \
            <<  ", reference path: \""                                    \
            <<  ref_path << "\""                                          \
            <<  ") " << msg;                                              \
            exit(111);                                                    \
    }                                                                     \
}

#define HDC_HDF5_ERROR( ref_path, msg )                                   \
{                                                                         \
    cerr << "HDF5 Error (reference path: \"" << ref_path                  \
                    << ref_path << "\") " <<  msg;                        \
    exit(111);                                                            \
}

struct h5_read_opdata
{
    unsigned                recurs;      /* Recursion level.  0=root */
    struct h5_read_opdata   *prev;        /* Pointer to previous opdata */
    haddr_t                 addr;        /* Group address */

    // pointer to HDC node, anchors traversal to
    HDC*             node;
    std::string      ref_path;
};

void write_node(H5File* file, std::string path);
void to_hdf5(std::string filename, std::string dataset_name);
HDC hdf_dataset_to_node();
TypeID hdf5_type_to_hdc_type(hid_t hdf5_dtype_id, const std::string& ref_path);
void hdf5_dataset_to_hdc(hid_t hdf5_dset_id, const std::string &ref_path, HDC& dest);
int h5_group_check(h5_read_opdata *od, haddr_t target_addr);
herr_t h5_literate_traverse_op_func(hid_t hdf5_id,const char *hdf5_path,const H5L_info_t *, void *hdf5_operator_data);
void hdf5_group_to_hdc(hid_t hdf5_group_id, const std::string  &ref_path, HDC& dest);
void hdf5_tree_to_hdc(hid_t hdf5_id, const std::string  &ref_path, HDC& dest);
void hdf5_read(hid_t hdf5_id, std::string hdf5_path, HDC& dest);
void hdf5_read(const std::string& file_path, const std::string& hdf5_path, HDC& node);
HDC from_hdf5(H5File* file, std::string dataset_name);
HDC from_hdf5(const std::string& filename, const std::string& dataset_name);
HDC* from_hdf5_ptr(const std::string& filename);

#endif // HDC_HDF5_H
