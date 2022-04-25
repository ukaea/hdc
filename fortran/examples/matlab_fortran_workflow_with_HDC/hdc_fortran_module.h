// Here we have to mock up the function header for MATLAB's FFI. We should also include some HDC API. It works with C-API, C++ would be probably also ok -> TODO
#include "../../include/hdc_c.h"
void test_cpos_f2c(struct hdc_t equilibriumin, struct hdc_t distsourceout);
