#include <iostream>
#include <vector>
#include <string>

#include "mex.h"
#include "matrix.h"

#include "hdc.hpp"

#include "class_handle.hpp"


using namespace std;


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
        // Return a handle to a new C++ instance
        plhs[0] = convertPtr2Mat<HDC>(new HDC);

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
        double* data = mxGetPr(prhs[2]);
        // matlab::data::TypedArray<double> in = std::move(inputs[1]);
        int ndim = *mxGetDimensions(prhs[2]);
        size_t shape[ndim];
        const mwSize *mat_shape = mxGetDimensions(prhs[2]);
        for (size_t i = 0; i < ndim; i++) {
          shape[i] = mat_shape[i];
        };
        size_t flags = HDCDefault;
        flags |= HDCFortranOrder;

        hdc_instance->set_data(ndim, shape, data, flags);

        return;
    }

    if (!strcmp("get_data", cmd)) {

        mxArray *result = nullptr;
        // matlab::data::TypedArray<double> in = std::move(inputs[1]);
        int ndim = hdc_instance->get_ndim();
        size_t *shape = hdc_instance->get_shape();
        assert (0 < ndim < 3);
        mwSize m, n;
        m = shape[0];
        if (ndim == 1) {
          n = 1;
        } else {
          n = shape[1];
        }
std::cout <<"here1\n";
        mwSize my_ndim = ndim;
        void *my_shape = mxMalloc(ndim*sizeof(mwSize));
        for (int i = 0; i < ndim; i++) ((mwSize*)my_shape)[i] = shape[i];
std::cout <<"here2\n";
        // create output matlab matrix and copy data
        //result = mxCreateDoubleMatrix(m, n, mxREAL);
        //memcpy(mxGetPr(result), hdc_instance->as<void*>(), m * n * sizeof(double));

        // zero-copy -- so far results into segfaults
        // most likely because matlab deallocates the memory
//         result = mxCreateUninitNumericArray(ndim, shape, mxDOUBLE_CLASS, mxREAL);
//         mxSetData(result, hdc_instance->as<void*>());
        result = mxCreateDoubleMatrix(0,0,mxREAL);
std::cout <<"here3\n";
        double *dset_data = (double *)malloc(sizeof(double) * shape[0] * shape[1]);
std::cout <<"here4\n";
        mxSetData(result, dset_data);
        mxSetDimensions(result, shape, ndim);
std::cout <<"here5\n";
        plhs[0] = result;
        return;
    }

    if (!strcmp("dump", cmd)) {

        hdc_instance->dump();

        return;
    }



    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
