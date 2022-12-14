#include "hdc_benchmark_common.hpp"

// Plain memcpy test to show over overhead of HDC
static void BM_memcpy(benchmark::State& state) {
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
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_HDC_SetData(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    int32_t* data = new int32_t[state.range(0)];
    memset(data,1,sizeof(int32_t)*state.range(0));
    while (state.KeepRunning()) {
        node.set_data(shape,data);
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(int32_t)));
    delete[] data;
    node.clean();
    StorageReset();
}
BENCHMARK(BM_HDC_SetData)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_HDC_GetData(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    int32_t* data = new int32_t[state.range(0)];
    int32_t* data_copy = new int32_t[state.range(0)];
    memset(data,1,sizeof(int32_t)*state.range(0));
    node.set_data(shape,data);
    while (state.KeepRunning()) {
        int32_t* data2 = node.as<int32_t>();
        memcpy(data_copy,data2,state.range(0)*sizeof(int32_t));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(int32_t)));
    delete[] data;
    delete[] data_copy;
    node.clean();
    StorageReset();
}
BENCHMARK(BM_HDC_GetData)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_HDC_ZeroCopyDataRead(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (auto i=0;i<state.range(0);i++) data[i] = 1.0;
    node.set_data(shape,data);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node.as<double>();
        for (auto i=0;i<state.range(0);i++) {
            s = data2[i];}
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    if (s < 0) cout << s << endl; // fool compilator to not optimize :)
    node.clean();
    delete[] data;
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataRead)->RangeMultiplier(2)->Range(2<<20,2<<25);

static void BM_HDC_ZeroCopyDataWrite(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (auto i=0;i<state.range(0);i++) data[i] = 1.0;
    node.set_data(shape,data);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node.as<double>();
        for (auto i=0;i<state.range(0);i++) data2[i] = s;
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    node.clean();
    delete[] data;
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataWrite)->RangeMultiplier(2)->Range(2<<20,2<<25);
