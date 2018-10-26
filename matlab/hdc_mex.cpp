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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Get the command string
    char cmd[64];
        if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
                mexErrMsgTxt("First input should be a command string less than 64 characters long.");

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
                if (mxIsScalar(data_in)) {
                    auto data = mxGetScalar(data_in);
                    plhs[0] = convertPtr2Mat<HDC>(new HDC((void*)&data,t));
                    return;
                } else {
                    struct hdc_data_t obj;
                    auto data = mxGetPr(data_in);
                    // matlab::data::TypedArray<double> in = std::move(inputs[1]);
                    obj.rank = mxGetNumberOfDimensions(data_in);
                    const mwSize *mat_shape = mxGetDimensions(data_in);
                    for (size_t i = 0; i < obj.rank; i++) {
                        obj.shape[i] = mat_shape[i];
                    };
                    obj.flags = HDCDefault | HDCFortranOrder;
                    obj.type = t;
                    obj.data = (char*)mxGetPr(data_in);
                    plhs[0] = convertPtr2Mat<HDC>(new HDC(obj));
                    return;
                }
            }
        }
        if (nrhs > 2) {
            // TODO: change this from int to string
            int32_t constructor_variant = (int32_t)mxGetScalar(prhs[1]);
            std::cout << "Constructor variant: " << constructor_variant << std::endl;
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

    // Load JSON
    if (!strcmp("load_json", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("load_json: One output expected.");
        if (nrhs < 1)
            mexErrMsgTxt("load_json: One or two inputs expected.");
        auto path = mxArrayToUTF8String(prhs[1]);
        std::string datapath = "";
        if (nrhs >=3) datapath = mxArrayToUTF8String(prhs[2]);
        plhs[0] = convertPtr2Mat<HDC>(new HDC(HDC::from_json(path,datapath)));
        return;
    }

    // Load HDF5
    if (!strcmp("load_hdf5", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("load_hdf5: One output expected.");
                if (nrhs != 1)
            mexErrMsgTxt("load_hdf5: One or two inputs expected.");
        auto path = mxArrayToUTF8String(prhs[1]);
        std::string datapath = "";
        if (nrhs >=3) datapath = mxArrayToUTF8String(prhs[2]);
        plhs[0] = convertPtr2Mat<HDC>(new HDC(HDC::from_hdf5(path,datapath)));
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
            if (mxIsScalar(data_in)) {
                auto data = mxGetScalar(data_in);
                hdc_instance->set_data((void*)&data,t);
                return;
            } else {
                struct hdc_data_t obj;
                auto data = mxGetPr(data_in);
                // matlab::data::TypedArray<double> in = std::move(inputs[1]);
                obj.rank = mxGetNumberOfDimensions(data_in);
                const mwSize *mat_shape = mxGetDimensions(data_in);
                for (size_t i = 0; i < obj.rank; i++) {
                    obj.shape[i] = mat_shape[i];
                };
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
            auto size = hdc_instance->get_datasize();
            mwSize my_rank = obj.rank;
            std::vector<size_t> my_shape(obj.rank);
            for (int i = 0; i < obj.rank; i++) {
                my_shape[i] = obj.shape[i];
            }
            auto classid = HDCType2matlabClassID(obj.type);
            // create output matlab matrix and copy data
            result = mxCreateNumericArray(obj.rank, obj.shape, classid, mxREAL);
            memcpy(mxGetPr(result), obj.data, size);

            // zero-copy -- so far results into segfaults
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

    if (!strcmp("add", cmd)) {
        char path[HDC_STR_LEN];
        if (mxGetString(prhs[2], path, sizeof(path))) {
            mexErrMsgTxt("add(): Second input should be a command string less than 1024 characters long.");
        }
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        hdc_instance->add_child(path,*child);
        return;
    }

    if (!strcmp("set", cmd)) {
        // Get can eat string or index - get the class name:
        HDC* child = convertMat2Ptr<HDC>(prhs[3]);
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                mexErrMsgTxt("set(): Second input should be a command string less than 1024 characters long.");
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

    if (!strcmp("get", cmd)) {
        if (nlhs != 1)
            mexErrMsgTxt("get(): One output expected.");
        // Get can eat string or index - get the class name:
        HDC* ch;
        if (mxIsClass(prhs[2],"char")) {
            char path[HDC_STR_LEN];
            if (mxGetString(prhs[2], path, sizeof(path))) {
                std::cout << "path: " << path << std::endl;
                mexErrMsgTxt("get(): Second input should be a command string less than 1024 characters long.");
            }
            ch = new HDC(hdc_instance->get(path));
        } else {
            double index_ = mxGetScalar(prhs[2]);
            if (index_ < 0) {
                mexErrMsgTxt("get(): Supplied index cannont be negative.");
            }
            size_t index = (size_t)index_;
            ch = new HDC(hdc_instance->get(index));
        }
        plhs[0] = convertPtr2Mat<HDC>(ch);
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

    if (!strcmp("get_uuid", cmd)) {
        mxArray* str = mxCreateString((char*) hdc_instance->get_uuid().c_str());
        plhs[0] = str;
        return;
    }

    if (!strcmp("get_storage", cmd)) {
        auto storage = hdc_instance->get_storage();
        mxArray *out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
        *((uint64_t *)mxGetData(out)) = reinterpret_cast<uint64_t>(storage);
        plhs[0] = out;
        return;
    }

    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
