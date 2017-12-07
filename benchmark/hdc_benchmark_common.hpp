#ifndef HDC_BENCHMARK_COMMON
#define HDC_BENCHMARK_COMMON
#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>
#include <unistd.h>
using namespace std;
static void StorageReset() {
    if (global_storage != nullptr) HDC::destroy();
    HDC::init("mdbm","{\"filename\": \"/tmp/db1.mdbm\", \"persistent\": false}");
}
#endif
