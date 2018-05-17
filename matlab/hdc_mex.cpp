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
        destroyObject<HDC>(prhs[1]);
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

        mxArray *result;
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

        // create output matlab matrix and copy data
        result = mxCreateDoubleMatrix(m, n, mxREAL);
        memcpy(mxGetPr(result), hdc_instance->as<void*>(), m * n * sizeof(double));

        // zero-copy -- so far results into segfaults
        // most likely because matlab deallocates the memory
        // result = mxCreateUninitNumericArray(ndim, shape, mxDOUBLE_CLASS, mxREAL);
        // mxSetDoubles(result, (mxDouble*) hdc_instance->as<void*>());

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
