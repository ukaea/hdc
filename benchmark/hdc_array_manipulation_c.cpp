#include "hdc_benchmark_common_c.hpp"
#include <string.h>
// Plain memcpy test to show over overhead of HDC
static void BM_C_memcpy(benchmark::State& state) {
    size_t n = state.range(0);
    int32_t* src = new int32_t[n];
    int32_t* dst = new int32_t[n];
    memset(src, 1, n);
    while (state.KeepRunning())
        memcpy(dst, src, n*sizeof(int32_t));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(int32_t)));
    delete[] src;
    delete[] dst;
    StorageReset();
}
BENCHMARK(BM_C_memcpy)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_C_HDC_SetData(benchmark::State& state) {
    struct hdc_t* node = hdc_new_empty();
    size_t shape[1];
    shape[0] = state.range(0);
    int32_t* data = new int32_t[state.range(0)];
    memset(data,1,sizeof(int32_t)*state.range(0));
    while (state.KeepRunning()) {
        hdc_set_int32(node,1,shape,data,HDCDefault);
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(int32_t)));
    hdc_delete(node);
    delete[] data;
    StorageReset();
}
BENCHMARK(BM_C_HDC_SetData)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_C_HDC_GetData(benchmark::State& state) {
    struct hdc_t* node = hdc_new_empty();
    size_t shape[1];
    shape[0] = state.range(0);
    int32_t* data = new int32_t[state.range(0)];
    int32_t* data_copy = new int32_t[state.range(0)];
    memset(data,1,sizeof(int32_t)*state.range(0));
    hdc_set_int32(node,1,shape,data,HDCDefault);
    while (state.KeepRunning()) {
        int32_t* data2 = hdc_as_int32_1d(node);
        memcpy(data_copy,data2,state.range(0)*sizeof(int32_t));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(int32_t)));
    hdc_delete(node);
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_C_HDC_GetData)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_C_HDC_ZeroCopyDataRead(benchmark::State& state) {
    struct hdc_t* node = hdc_new_empty();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (size_t i=0;i<state.range(0);i++) data[i] = 1.0;
    double* data_copy = new double[state.range(0)];
    hdc_set_double(node,1,shape,data,HDCDefault);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = hdc_as_double_1d(node);
        for (size_t i=0;i<state.range(0);i++) {
            s = data2[i];}
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    if (s < 0) printf("%d\s",s); // foll compilator to not optimize :)
    hdc_delete(node);
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_C_HDC_ZeroCopyDataRead)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_C_HDC_ZeroCopyDataWrite(benchmark::State& state) {
    struct hdc_t* node = hdc_new_empty();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (size_t i=0;i<state.range(0);i++) data[i] = 1.0;
    double* data_copy = new double[state.range(0)];
    hdc_set_double(node,1,shape,data,HDCDefault);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = hdc_as_double_1d(node);
        for (size_t i=0;i<state.range(0);i++) data2[i] = s;
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    hdc_delete(node);
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_C_HDC_ZeroCopyDataWrite)->RangeMultiplier(2)->Range(2<<20,2<<25);
