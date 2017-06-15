#ifndef HDC_BENCHMARK_COMMON
#define HDC_BENCHMARK_COMMON
#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>
using namespace std;
static void StorageReset() {
    HDC_destroy();
    HDC_init();
}
#endif
