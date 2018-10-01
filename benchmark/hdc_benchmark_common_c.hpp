#ifndef HDC_BENCHMARK_COMMON_C
#define HDC_BENCHMARK_COMMON_C
#include "hdc_c.h"
#include "benchmark/benchmark.h"
#include <string>
#include <unistd.h>
using namespace std;
static void StorageReset() {
    hdc_destroy();
    hdc_init("mdbm","{\"filename\": \"/tmp/db1.mdbm\", \"persistent\": false}");
}
#endif
