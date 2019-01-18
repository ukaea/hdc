#include "hdc_benchmark_common.hpp"

// Plain memcpy test to show over overhead of HDC
static void BM_memcpy(benchmark::State& state) {
    size_t n = state.range(0);
    std::vector<double> src(n);
    std::vector<double> dst(n);
    memset(src.data(), 1, n);
    while (state.KeepRunning())
        memcpy(dst.data(), src.data(), n*sizeof(double));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
}
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_SetData(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    std::vector<double> data(state.range(0));
    memset(data.data(),1,sizeof(double)*state.range(0));
    while (state.KeepRunning()) {
        node.set_data(shape,data.data());
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    StorageReset();
}
BENCHMARK(BM_HDC_SetData)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_GetData(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    std::vector<double>  data(state.range(0));
    std::vector<double>  data_copy(state.range(0));
    memset(data.data(),1,sizeof(double)*state.range(0));
    node.set_data(shape,data.data());
    while (state.KeepRunning()) {
        double* data2 = node.as<double>();
        memcpy(data_copy.data(),data2,state.range(0)*sizeof(double));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    StorageReset();
}
BENCHMARK(BM_HDC_GetData)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_ZeroCopyDataRead(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    std::vector<double> data(state.range(0));
    for (auto i=0;i<state.range(0);i++) data[i] = 1.0;
    std::vector<double>  data_copy(state.range(0));
    node.set_data(shape,data.data());
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node.as<double>();
        for (auto i=0;i<state.range(0);i++) {
            s = data2[i];}
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    if (s < 0) std::cout << s << std::endl; // trick compilator to not optimize :)
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataRead)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_ZeroCopyDataWrite(benchmark::State& state) {
    HDC node;
    std::vector<size_t> shape(1);
    shape[0] = state.range(0);
    std::vector<double> data(state.range(0));
    for (auto i=0;i<state.range(0);i++) data[i] = 1.0;
    std::vector<double>  data_copy(state.range(0));
    node.set_data(shape,data.data());
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node.as<double>();
        for (auto i=0;i<state.range(0);i++) data2[i] = s;
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataWrite)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC tree(2<<22);
    for(int i=0;i<n;i++) {
        HDC h;
        tree.add_child(to_string(i),h);
    }
    std::string paths[100];
    for (int i=0;i<100;i++) {
        paths[i] = std::to_string(rand() % n);
    }
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC child = tree.get(paths[i]);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(100000,100000);

static void BM_HDC_AddChild(benchmark::State& state) {
    HDC tree(2<<21);
    while (state.KeepRunning()) {
        HDC h;
        for (auto i = 0;i < state.range(0);i++) tree.add_child(std::to_string(i),h);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChild)->RangeMultiplier(2)->Range(100000,100000);

BENCHMARK_MAIN();
