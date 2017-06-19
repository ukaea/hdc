#include "hdc_benchmark_common.hpp"

// Plain memcpy test to show over overhead of HDC
static void BM_memcpy(benchmark::State& state) {
    size_t n = state.range(0);
    double* src = new double[n];
    double* dst = new double[n];
    memset(src, 1, n);
    while (state.KeepRunning())
        memcpy(dst, src, n*sizeof(double));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    delete[] src;
    delete[] dst;
    StorageReset();
}
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_SetData(benchmark::State& state) {
    HDC* node = new HDC();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    memset(data,1,sizeof(double)*state.range(0));
    while (state.KeepRunning()) {
        node->set_data(1,shape,data);
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    delete node;
    delete[] data;
    StorageReset();
}
BENCHMARK(BM_HDC_SetData)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_GetData(benchmark::State& state) {
    HDC* node = new HDC();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    double* data_copy = new double[state.range(0)];
    memset(data,1,sizeof(double)*state.range(0));
    node->set_data(1,shape,data);
    while (state.KeepRunning()) {
        double* data2 = node->as<double*>();
        memcpy(data_copy,data2,state.range(0)*sizeof(double));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    delete node;
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_HDC_GetData)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_ZeroCopyDataRead(benchmark::State& state) {
    HDC* node = new HDC();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (size_t i=0;i<state.range(0);i++) data[i] = 1.0;
    double* data_copy = new double[state.range(0)];
    node->set_data(1,shape,data);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node->as<double*>();
        for (size_t i=0;i<state.range(0);i++) {
            s = data2[i];}
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    if (s < 0) cout << s << endl; // foll compilator to not optimize :)
    delete node;
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataRead)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_ZeroCopyDataWrite(benchmark::State& state) {
    HDC* node = new HDC();
    size_t shape[1];
    shape[0] = state.range(0);
    double* data = new double[state.range(0)];
    for (size_t i=0;i<state.range(0);i++) data[i] = 1.0;
    double* data_copy = new double[state.range(0)];
    node->set_data(1,shape,data);
    double s=0.0;
    while (state.KeepRunning()) {
        double* data2 = node->as<double*>();
        for (size_t i=0;i<state.range(0);i++) data2[i] = s;
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)) * int64_t(sizeof(double)));
    delete node;
    delete[] data;
    delete[] data_copy;
    StorageReset();
}
BENCHMARK(BM_HDC_ZeroCopyDataWrite)->RangeMultiplier(2)->Range(4194304,4194304);

static void BM_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC* tree = new HDC(2<<21);
    for(int i=0;i<n;i++) tree->add_child(to_string(i),new HDC());
    std::string paths[100];
    for (int i=0;i<100;i++) {
        paths[i] = std::to_string(rand() % n);
    }
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC* child = tree->get_ptr(paths[i]);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    delete tree;
    StorageReset();
}
BENCHMARK(BM_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(100000,100000);

static void BM_HDC_AddChild(benchmark::State& state) {
    HDC* tree = new HDC(2<<21);
    while (state.KeepRunning()) {
        for (size_t i = 0;i < state.range(0);i++) tree->add_child(std::to_string(i),new HDC());
    }
    delete tree;
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChild)->RangeMultiplier(2)->Range(100000,100000);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
