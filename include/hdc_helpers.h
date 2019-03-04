#ifndef HDC_HELPERS
#define HDC_HELPERS

#include <iostream>

#ifdef DEBUG
    #define DEBUG_STDERR(x) (std::cerr << (x) << std::endl)
    #define DEBUG_STDOUT(x) (std::cout << (x) << std::endl)
    #define D(x) x
#else //DEBUG
    #define DEBUG_STDERR(x)
    #define DEBUG_STDOUT(x)
    #define D(x)
#endif //DEBUG

#define REGISTER_PO() ()
#define UNUSED __attribute__((unused))

#define HDC_STORAGE_INIT()              \
    if (hdc_global.storage == nullptr) {    \
        HDC::init();                    \
        atexit(HDC::destroy);           \
    }

#endif  //HDC_HELPERS
