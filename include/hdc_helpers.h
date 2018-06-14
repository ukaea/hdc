#ifndef HDC_HELPERS
#include <iostream>
#define HDC_HELPERS
#ifdef DEBUG
#define DEBUG_STDERR(x) (std::cerr << (x) << std::endl)
#define DEBUG_STDOUT(x) (std::cout << (x) << std::endl)
#define D(x) x
#else
#define DEBUG_STDERR(x)
#define DEBUG_STDOUT(x)
#define D(x)
#endif

#define REGISTER_PO() ()

#define UNUSED __attribute__((unused))

#endif
