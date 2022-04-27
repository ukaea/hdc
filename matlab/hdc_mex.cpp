#include <iostream>
#include <vector>
#include <string>

#include "mex.h"
#include "matrix.h"

#include "hdc.hpp"

#include "class_handle.hpp"

#define HDC_STR_LEN 1024

using namespace std;


hdc_type_t matlabClassID2HDCType(mxClassID matlab_type)
{
    switch(matlab_type) {
        case mxLOGICAL_CLASS:
            return HDC_BOOL;
        case mxDOUBLE_CLASS:
            return HDC_DOUBLE;
        case mxSINGLE_CLASS:
            return HDC_FLOAT;
        case mxINT8_CLASS:
            return HDC_INT8;
        case mxUINT8_CLASS:
            return HDC_UINT8;
        case mxINT16_CLASS:
            return HDC_INT16;
        case mxUINT16_CLASS:
            return HDC_UINT16;
        case mxINT32_CLASS:
            return HDC_INT32;
        case mxUINT32_CLASS:
            return HDC_UINT32;
        case mxINT64_CLASS:
            return HDC_INT64;
        case mxUINT64_CLASS:
            return HDC_UINT64;
        default:
            mexErrMsgTxt("matlabClassID2HDCType(): Unknown matlab_type.");
    };
    return (hdc_type_t)999; // Should newer go here
}

mxClassID HDCType2matlabClassID(hdc_type_t hdc_type)
{
    switch(hdc_type) {
        case HDC_BOOL:
            return mxLOGICAL_CLASS;
        case HDC_DOUBLE:
            return mxDOUBLE_CLASS;
        case HDC_FLOAT:
            return mxSINGLE_CLASS;
        case HDC_INT8:
            return mxINT8_CLASS;
        case HDC_UINT8:
            return mxUINT8_CLASS;
        case HDC_INT16:
            return mxINT16_CLASS;
        case HDC_UINT16:
            return mxUINT16_CLASS;
        case HDC_INT32:
            return mxINT32_CLASS;
        case HDC_UINT32:
            return mxUINT32_CLASS;
        case HDC_INT64:
            return mxINT64_CLASS;
        case HDC_UINT64:
            return mxUINT64_CLASS;
        default:
            mexErrMsgTxt("HDCType2matlabClassID(): Unknown hdc_type.");
    };
    return mxDOUBLE_CLASS; // Should newer go here
}

std::string HDCType2MatlabStr(hdc_type_t hdc_type)
{
    switch(hdc_type) {
        case HDC_BOOL:
            return "bool";
        case HDC_DOUBLE:
            return "double";
        case HDC_FLOAT:
            return "single";
        case HDC_INT8:
            return "int8";
        case HDC_UINT8:
            return "uint8";
        case HDC_INT16:
            return "int16";
        case HDC_UINT16:
            return "uint16";
        case HDC_INT32:
            return "int32";
        case HDC_UINT32:
            return "uint32";
        case HDC_INT64:
            return "int64";
        case HDC_UINT64:
            return "uint64";
        case HDC_STRUCT:
            return "HDC struct";
        case HDC_LIST:
            return "HDC list";
        case HDC_ERROR:
            return "HDC error";
        case HDC_EMPTY:
            return "HDC empty";
        default:
            mexErrMsgTxt("HDCType2MatlabStr(): Unknown hdc_type.");
    };
    return ""; // Should newer go here
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Get the command string
    char cmd[64];
        if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
                mexErrMsgTxt("First input should be a command string less than 64 characters long.");

    if (!strcmp("init", cmd)) {
        std::string options = "";
        std::string storage = "umap";
        if (nrhs > 2)
            options = mxArrayToUTF8String(prhs[2]);
        if (nrhs >= 2)
            storage = mxArrayToUTF8String(prhs[1]);
        HDC::init(storage,options);
        return;
    }

    if(!strcmp("destroy",cmd)) {
        HDC::destroy();
        return;
    }

    // New
    if (!strcmp("new", cmd)) {
        // Check parameters
        if (nlhs != 1)
            mexErrMsgTxt("New: One output expected.");
        if (nrhs == 2) {
            auto data_in = prhs[1];
            // Call appropriate constructor according to data
            if (mxIsClass(data_in,"char")) {
                plhs[0] = convertPtr2Mat<HDC>(new HDC(mxArrayToUTF8String(data_in)));
                return;
            }
            if (mxIsNumeric(data_in)) {
                auto t = matlabClassID2HDCType(mxGetClassID(data_in));
                auto n_elem = mxGetNumberOfElements(data_in);
                if (mxIsScalar(data_in) || n_elem == 1) {
                    double data = mxGetScalar(data_in);

                    bool data_b;
                    int8_t data_8;
                    int16_t data_16;
                    int32_t data_32;
                    int64_t data_64;
                    uint8_t data_u8;
                    uint16_t data_u16;
                    uint32_t data_u32;
                    uint64_t data_u64;
                    float data_f;
                    auto node = new HDC();
                    switch (t) {
                        case HDC_BOOL:
                            data_b = static_cast<bool>(data);
                            node->set_data(data_b);
                            break;
                        case HDC_DOUBLE:
                            node->set_data(data);
                            break;
                        case HDC_FLOAT:
                            data_f = static_cast<float>(data);
                            node->set_data(data_f);
                            break;
                        case HDC_INT8:
                            data_8 = static_cast<int8_t>(data);
                            node->set_data(data_8);
                            break;
                        case HDC_UINT8:
                            data_u8 = static_cast<int16_t>(data);
                            node->set_data(data_u8);
                            break;
                        case HDC_INT16:
                            data_16 = static_cast<int32_t>(data);
                            node->set_data(data_16);
                            break;
                        case HDC_UINT16:
                            data_u16 = static_cast<int32_t>(data);
                            node->set_data(data_u16);
                            break;
                        case HDC_INT32:
                            data_32 = static_cast<int32_t>(data);
                            node->set_data(data_32);
                            break;
                        case HDC_UINT32:
                            data_u32 = static_cast<int32_t>(data);
                            node->set_data(data_u32);
                            break;
                        case HDC_INT64:
                            data_64 = static_cast<int64_t>(data);
                            node->set_data(data_64);
                            break;
                        case HDC_UINT64:
                            data_u64 = static_cast<int64_t>(data);
                            node->set_data(data_u64);
                            break;
                        default:
                            mexErrMsgTxt("matlabClassID2HDCType(): Unknown matlab_type.");
                    }
                    plhs[0] = convertPtr2Mat<HDC>(node);
                    return;
                } else {
                    struct hdc_data_t obj;
                    auto data = mxGetPr(data_in);
                    // matlab::data::TypedArray<double> in = std::move(inputs[1]);
                    obj.rank = mxGetNumberOfDimensions(data_in);
                    const mwSize *mat_shape = mxGetDimensions(data_in);
                    size_t n_elem = 1;
                    for (size_t i = 0; i < obj.rank; i++) {
                        obj.shape[i] = mat_shape[i];
                        n_elem *= mat_shape[i];
                    };
                    //here we should change the shape for 1D case as MATLAB gives us always at least a matrix...
                    if (obj.rank == 2 && (n_elem == obj.shape[0] || n_elem == obj.shape[1])) {
                        obj.rank = 1;
                        obj.shape[0] = n_elem;
                        obj.shape[1] = 0;
                    }
                    obj.flags = HDCDefault | HDCFortranOrder;
                    obj.type = t;
                    obj.data = (char*)data;
                    plhs[0] = convertPtr2Mat<HDC>(new HDC(obj));
                    return;
                }
            }
        }
        if (nrhs > 2) {
            // TODO: change this from int to string
            int32_t constructor_variant = (int32_t)mxGetScalar(prhs[1]);
            switch (constructor_variant) {
                case 1: // Copy constructor
                    plhs[0] = convertPtr2Mat<HDC>(new HDC(*convertMat2Ptr<HDC>(prhs[2])));
                    return;
                //TODO: add more
                default:
                    mexErrMsgTxt("new(): Supplied variant of constructor not known");
            }
        }
        // Return a handle to a new C++ instance
        // plain constructor here
        plhs[0] = convertPtr2Mat<HDC>(new HDC());
        return;
    }

    // Load URI
    if (!strcmp("load", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("load: One output expected.");
        if (nrhs < 1)
            mexErrMsgTxt("load: One or two inputs expected.");
        auto uri = mxArrayToUTF8String(prhs[1]);
        std::string datapath = "";
        if (nrhs >=3) datapath = mxArrayToUTF8String(prhs[2]);
        plhs[0] = convertPtr2Mat<HDC>(new HDC(HDC::load(uri,datapath)));
        return;
    }

    // Check there is a second input, which should be the class instance handle
    if (nrhs < 2)
        mexErrMsgTxt("Second input should be a class instance handle.");

    // Delete
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
// //         destroyObject<HDC>(prhs[1]);
        // Warn if other commands were ignored
        if (nlhs != 0 || nrhs != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }

    // Get the class instance pointer from the second input
    HDC* hdc_instance = convertMat2Ptr<HDC>(prhs[1]);

    // All other methods must be below this line

    if (!strcmp("set_data", cmd)) {
        auto data_in = prhs[2];
        // Call appropriate constructor according to data
        if (mxIsClass(data_in,"char")) {
            hdc_instance->set_string(mxArrayToUTF8String(data_in));
            return;
        }
        else if (mxIsNumeric(data_in)) {
            auto t = matlabClassID2HDCType(mxGetClassID(data_in));
            auto n_elem = mxGetNumberOfElements(data_in);
            if (mxIsScalar(data_in) || n_elem == 1) {
                auto data = mxGetScalar(data_in);

                bool data_b;
                int8_t data_8;
                int16_t data_16;
                int32_t data_32;
                int64_t data_64;
                uint8_t data_u8;
                uint16_t data_u16;
                uint32_t data_u32;
                uint64_t data_u64;
                float data_f;

                switch (t) {
                    case HDC_BOOL:
                        data_b = static_cast<float>(data);
                        hdc_instance->set_data(data_b);
                        return;
                    case HDC_DOUBLE:
                        hdc_instance->set_data(data);
                        return;
                    case HDC_FLOAT:
                        data_f = static_cast<float>(data);
                        hdc_instance->set_data(data_f);
                        return;
                    case HDC_INT8:
                        data_8 = static_cast<int8_t>(data);
                        hdc_instance->set_data(data_8);
                        return;
                    case HDC_UINT8:
                        data_u8 = static_cast<int16_t>(data);
                        hdc_instance->set_data(data_u8);
                        return;
                    case HDC_INT16:
                        data_16 = static_cast<int32_t>(data);
                        hdc_instance->set_data(data_16);
                        return;
                    case HDC_UINT16:
                        data_u16 = static_cast<int32_t>(data);
                        hdc_instance->set_data(data_u16);
                        return;
                    case HDC_INT32:
                        data_32 = static_cast<int32_t>(data);
                        hdc_instance->set_data(data_32);
                        return;
                    case HDC_UINT32:
                        data_u32 = static_cast<int32_t>(data);
                        hdc_instance->set_data(data_u32);
                        return;
                    case HDC_INT64:
                        data_64 = static_cast<int64_t>(data);
                        hdc_instance->set_data(data_64);
                        return;
                    case HDC_UINT64:
                        data_u64 = static_cast<int64_t>(data);
                        hdc_instance->set_data(data_u64);
                        return;
                    default:
                        mexErrMsgTxt("matlabClassID2HDCType(): Unknown matlab_type.");
                }
//                 hdc_instance->set_data(data,t);
                return;
            } else {
                struct hdc_data_t obj;
                auto data = mxGetPr(data_in);
                // matlab::data::TypedArray<double> in = std::move(inputs[1]);
                obj.rank = mxGetNumberOfDimensions(data_in);
                const mwSize *mat_shape = mxGetDimensions(data_in);
                size_t n_elem = 1;
                for (size_t i = 0; i < obj.rank; i++) {
                    obj.shape[i] = mat_shape[i];
                    n_elem *= mat_shape[i];
                };
                //here we should change the shape for 1D case as MATLAB gives us always at least a matrix...
                if (obj.rank == 2 && (n_elem == obj.shape[0] || n_elem == obj.shape[1])) {
                    obj.rank = 1;
                    obj.shape[0] = n_elem;
                    obj.shape[1] = 0;
                }
                obj.flags = HDCDefault | HDCFortranOrder;
                obj.type = t;
                obj.data = (char*)mxGetPr(data_in);
                hdc_instance->set_data(obj);
                return;
            }
        }
    }


    if (!strcmp("get_data", cmd)) {

        mxArray* result = nullptr;
        auto obj = hdc_instance->get_data();
        if (obj.type == HDC_STRING) {
            mxArray* str = mxCreateString((char*) obj.data);
            plhs[0] = str;
            return;
        } else if (hdc_is_numeric(obj.type)) {
            mwSize my_rank = obj.rank;
            std::vector<size_t> my_shape(obj.rank);
            size_t n_elem = 1;
            for (int i = 0; i < obj.rank; i++) {
                my_shape[i] = obj.shape[i];
                n_elem *= obj.shape[i];
            }
            auto classid = HDCType2matlabClassID(obj.type);
            // create output matlab matrix and copy data
            if (n_elem > 1) {
                if (obj.rank == 1)
                    result = mxCreateNumericMatrix(1, n_elem, classid, mxREAL);
                else
                    result = mxCreateNumericArray(obj.rank, obj.shape, classid, mxREAL);
                memcpy(mxGetPr(result), obj.data, n_elem * hdc_sizeof(obj.type));
            } else {
                result = mxCreateNumericMatrix(1, 1, classid, mxREAL);
                //unfortunatelly, the following line has stopped working - replaced by the switch
                //memcpy(mxGetPr(result), obj.data, n_elem * hdc_sizeof(obj.type));
                switch (obj.type) {
                    case HDC_BOOL:
                        *(bool*)mxGetPr(result) = *(bool*)(&(obj.data));
                        break;
                    case HDC_DOUBLE:
                        *(double*)mxGetPr(result) = *(double*)(&(obj.data));
                        break;
                    case HDC_FLOAT:
                        *(float*)mxGetPr(result) = *(float*)(&(obj.data));
                        break;
                    case HDC_INT8:
                        *(int8_t*)mxGetPr(result) = *(int8_t*)(&(obj.data));
                        break;
                    case HDC_UINT8:
                        *(uint8_t*)mxGetPr(result) = *(uint8_t*)(&(obj.data));
                        break;
                    case HDC_INT16:
                        *(int16_t*)mxGetPr(result) = *(int16_t*)(&(obj.data));
                        break;
                    case HDC_UINT16:
                        *(uint16_t*)mxGetPr(result) = *(uint16_t*)(&(obj.data));
                        break;
                    case HDC_INT32:
                        *(int32_t*)mxGetPr(result) = *(int32_t*)(&(obj.data));
                        break;
                    case HDC_UINT32:
                        *(uint32_t*)mxGetPr(result) = *(uint32_t*)(&(obj.data));
                        break;
                    case HDC_INT64:
                        *(int64_t*)mxGetPr(result) = *(int64_t*)(&(obj.data));
                        break;
                    case HDC_UINT64:
                        *(uint64_t*)mxGetPr(result) = *(uint64_t*)(&(obj.data));
                        break;
                    default:
                        mexErrMsgTxt("matlabClassID2HDCType(): Unknown matlab_type.");
                }
            }
            //
            // zero-copy -- so far results in segfaults
            // most likely because matlab deallocates the memory
    //         result = mxCreateUninitNumericArray(rank, shape, mxDOUBLE_CLASS, mxREAL);
    //         mxSetData(result, hdc_instance->as<void*>());
            plhs[0] = result;
            return;
        } else if (obj.type == HDC_STRUCT || obj.type == HDC_LIST || obj.type == HDC_EMPTY || obj.type == HDC_ERROR) {
            mexErrMsgTxt("get_data(): Incompatible HDC data type. Non-terminal, empty or error container.");
        } else {
            mexErrMsgTxt("get_data(): Unknown HDC data type.");
        }
    }

    if (!strcmp("dump", cmd)) {
        hdc_instance->dump();
        return;
    }

    if (!strcmp("dumps", cmd)) {
        std::string str = hdc_instance->serialize("json");
        mxArray* mxstr = mxCreateString(str.c_str());
        plhs[0] = mxstr;
        return;
    }

    if (!strcmp("exists", cmd)) {
        char path[HDC_STR_LEN];
        if (mxGetString(prhs[2], path, sizeof(path)))
            mexErrMsgTxt("exists(): Second input should be a command string less than 1024 characters long.");
        auto res = hdc_instance->exists(path);
        plhs[0] = mxCreateLogicalMatrix(1,1);
        memcpy(mxGetLogicals(plhs[0]),&res,sizeof(res));
        return;
    }

    if (!strcmp("add_child", cmd)) {
        char path[HDC_STR_LEN];
        if (mxGetString(prhs[2], path, sizeof(path))) {
            mexErrMsgTxt("add_child(): Second input should be a command string less than 1024 characters long.");
        }
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        hdc_instance->add_child(path,*child);
        return;
    }

    if (!strcmp("delete_child", cmd)) {
        char path[HDC_STR_LEN];
        if (mxGetString(prhs[2], path, sizeof(path))) {
            mexErrMsgTxt("delete_child(): Second input should be a command string less than 1024 characters long.");
        }
        hdc_instance->delete_child(path);
        return;
    }

    if (!strcmp("set_child", cmd)) {
        // Get can eat string or index - get the class name:
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                mexErrMsgTxt("set_child(): Second input should be a command string less than 1024 characters long.");
            }
            hdc_instance->set_child(path,*child);
        } else {
            double index_ = mxGetScalar(prhs[2]);
            if (index_ < 0) {
                mexErrMsgTxt("set(): Supplied index cannot be negative.");
            }
            size_t index = (size_t)index_;
            hdc_instance->set_child(index,*child);
        }
        return;
    }

    if (!strcmp("get_child", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("get_child(): One output expected.");
        // Get can eat string or index - get the class name:
        HDC* ch;
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                std::cout << "path: " << path << std::endl;
                mexErrMsgTxt("get_child(): Second input should be a command string less than 1024 characters long.");
            }
            ch = new HDC(hdc_instance->get(path));
        } else {
            double index_ = mxGetScalar(prhs[2]);
            if (index_ < 0) {
                mexErrMsgTxt("get_child(): Supplied index cannont be negative.");
            }
            size_t index = (size_t)index_;
            ch = new HDC(hdc_instance->get(index));
        }
        plhs[0] = convertPtr2Mat<HDC>(ch);
        return;
    }

    if (!strcmp("get_or_create", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("get_or_create(): One output expected.");
        // Get can eat string or index - get the class name:
        HDC* ch;
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                std::cout << "path: " << path << std::endl;
                mexErrMsgTxt("get_or_create(): Second input should be a command string less than 1024 characters long.");
            }
            ch = new HDC(hdc_instance->get_or_create(path));
        } else {
            double index_ = mxGetScalar(prhs[2]);
            if (index_ < 0) {
                mexErrMsgTxt("get_or_create(): Supplied index cannont be negative.");
            }
            size_t index = (size_t)index_;
            ch = new HDC(hdc_instance->get_or_create(index));
        }
        plhs[0] = convertPtr2Mat<HDC>(ch);
        return;
    }

    if (!strcmp("type", cmd)) {
        auto type_str = HDCType2MatlabStr((hdc_type_t) hdc_instance->get_type());
        plhs[0] = mxCreateString((char*) type_str.c_str());
        return;
    }

    if (!strcmp("append", cmd)) {
        HDC* child = convertMat2Ptr<HDC>(prhs[2]);
        hdc_instance->append(*child);
        return;
    }

    if (!strcmp("insert", cmd)) {
        double index_ = mxGetScalar(prhs[2]);
        if (index_ < 0) {
            mexErrMsgTxt("insert(): Supplied index cannont be negative.");
        }
        size_t index = (size_t)index_;
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        hdc_instance->insert(index,*child);
        return;
    }

    if (!strcmp("copy", cmd)) {
        auto copy = hdc_instance->copy();
        auto copy_ptr = new HDC(copy);
        plhs[0] = convertPtr2Mat<HDC>(copy_ptr);
        return;
    }

    if (!strcmp("keys", cmd)) {
        auto keys = hdc_instance->keys();
        mwSize mxdims[] = {keys.size()};
        mxArray* mxkeys = mxCreateCellArray(1, mxdims);
        for (size_t i=0; i<keys.size(); i++) {
            mxArray* str = mxCreateString((char*) keys[i].c_str());
            mxSetCell(mxkeys, i, str);
        }
        plhs[0] = mxkeys;
        return;
    }

    if (!strcmp("shape", cmd)) {
        auto obj = hdc_instance->get_data();
        mwSize mxdims[] = {obj.rank};
        mxArray* mxshape = mxCreateNumericArray(1, mxdims, mxUINT64_CLASS, mxREAL);
        memcpy(mxGetPr(mxshape), &obj.shape, obj.rank*sizeof(size_t));
        plhs[0] = mxshape;
        return;
    }

    if (!strcmp("as_hdc_t", cmd)) {
        const char* fnames[] = {"storage_id","uuid"};
        plhs[0] = mxCreateStructMatrix(1,1,2,fnames);

        mxArray* storage_ = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
        size_t storage_id = hdc_instance->get_storage_id();
        memcpy(mxGetPr(storage_), &storage_id, sizeof(size_t));
        auto uuid = hdc_instance->get_uuid();
        //mxArray* uuid_ = mxCreateString((char*) hdc_instance->get_uuid().c_str());
        mxArray* uuid_ = mxCreateNumericMatrix(1,HDC_UUID_LENGTH, mxINT8_CLASS, mxREAL);
//         strcpy((char*) mxGetPr(uuid_), (char*) hdc_instance->get_uuid().c_str());
        memcpy(mxGetPr(uuid_),reinterpret_cast<char*>(&uuid), uuid.size());
        mxSetFieldByNumber(plhs[0],0,0,storage_);
        mxSetFieldByNumber(plhs[0],0,1,uuid_);
        return;
    }

    if (!strcmp("available_serializers", cmd)) {
        auto avail_sers = hdc_instance->available_serializers();
        mwSize mxdims[] = {avail_sers.size()};
        mxArray* mx_avail_sers = mxCreateCellArray(1, mxdims);
        for (size_t i=0; i<avail_sers.size(); i++) {
            mxArray* str = mxCreateString((char*) avail_sers[i].c_str());
            mxSetCell(mx_avail_sers, i, str);
        }
        plhs[0] = mx_avail_sers;
        return;
    }

    if (!strcmp("serialize", cmd)) {
        // Get can eat string or index - get the class name:
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                mexErrMsgTxt("set_child(): Second input should be a command string less than 1024 characters long.");
            }
            hdc_instance->set_child(path,*child);
        } else {
            double index_ = mxGetScalar(prhs[2]);
            if (index_ < 0) {
                mexErrMsgTxt("set(): Supplied index cannot be negative.");
            }
            size_t index = (size_t)index_;
            hdc_instance->set_child(index,*child);
        }
        return;
    }

//     if (!strcmp("save", cmd)) {
//         if (nrhs < 1)
//             mexErrMsgTxt("save: One or two inputs expected.");
//         auto uri = mxArrayToUTF8String(prhs[2]);
//         std::string datapath = "";
//         // This fails from unknown reason - lets work it around
//         if (nrhs >=3) datapath = mxArrayToUTF8String(prhs[3]);
//         hdc_instance->save(uri,datapath);
//         return;
//     }

    if (!strcmp("save", cmd)) {
        if (nrhs < 1)
            mexErrMsgTxt("save: One or two inputs expected.");
        std::string str = (mxArrayToUTF8String(prhs[2]));
        std::string delim = "|";
        auto pos = str.find(delim);
        std::string uri = "";
        std::string  data_path = "";
        if (pos != std::string::npos) {
            uri = str.substr(0,pos);
            data_path = str.substr(pos+1,str.length());
        } else {
            uri = str;
        }
        hdc_instance->save(uri,data_path);
        return;
    }


    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
