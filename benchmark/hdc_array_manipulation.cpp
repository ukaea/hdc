#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>

using namespace std;

// Plain memcpy test to show over overhead of HDC
static void BM_memcpy(benchmark::State& state) {
    size_t n = state.range_x();
    int32_t* src = new int32_t[n];
    int32_t* dst = new int32_t[n];
    memset(src, 1, n);
    while (state.KeepRunning())
        memcpy(dst, src, n*sizeof(int32_t));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range_x()) * int64_t(sizeof(int32_t)));
    delete[] src;
    delete[] dst;
}
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(1024*1024,1024*1024<<5);

// Set Data
static void BM_HDC_SetData(benchmark::State& state) {
    HDC* node = new HDC();
    long shape[1];
    shape[0] = state.range_x();
    int32_t* data = new int32_t[state.range_x()];
    memset(data,1,sizeof(int32_t)*state.range_x());
    while (state.KeepRunning()) {
        node->set_data<int32_t>(1,shape,(void*)data);
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range_x()) * int64_t(sizeof(int32_t)));
    delete node;
    delete[] data;
}
BENCHMARK(BM_HDC_SetData)->RangeMultiplier(2)->Range(1024*1024,1024*1024<<5);


// Set Data - Zero Copy
static void BM_HDC_SetDataZeroCopy(benchmark::State& state) {
    HDC* node = new HDC();
    long shape[1];
    shape[0] = state.range_x();
    int32_t* data = new int32_t[state.range_x()];
    memset(data,1,sizeof(int32_t)*state.range_x());
    while (state.KeepRunning()) {
        node->set_data<int32_t>(1,shape,(void*)data,false);
    }
//    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range_x()) * int64_t(sizeof(int32_t)));
    delete node;
    delete[] data;
}
BENCHMARK(BM_HDC_SetDataZeroCopy)->RangeMultiplier(2)->Range(1024,1024<<5);

// As Data
static void BM_HDC_GetData(benchmark::State& state) {
    HDC* node = new HDC();
    long shape[1];
    shape[0] = state.range_x();
    int32_t* data = new int32_t[state.range_x()];
    memset(data,1,sizeof(int32_t)*state.range_x());
    node->set_data<int32_t>(1,shape,(void*)data);
    while (state.KeepRunning()) {
        int32_t* data2 = node->as<int32_t*>();
    }
    delete node;
    delete[] data;
}
BENCHMARK(BM_HDC_GetData)->RangeMultiplier(2)->Range(1024,1024<<5);
