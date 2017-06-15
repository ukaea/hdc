#ifndef HDC_HELPERS
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

#endif
