#include "hdc_benchmark_common.hpp"
// Empty child creation and deletion
static void BM_HDC_CreationAndDeletion(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC h;
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_CreationAndDeletion)->Arg(1000000);

// Adding childs
static void BM_HDC_AddChild(benchmark::State& state) {
    HDC tree, child;
    while (state.KeepRunning()) {
        for (auto i = 0;i < state.range(0);i++) tree.add_child(std::to_string(i),child);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChild)->RangeMultiplier(2)->Range(1024<<2,1024<<5);

static void BM_HDC_AddChildPreallocated(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC tree(4*1024*1024<<5);
        for (auto i = 0;i < state.range(0);i++) {
            HDC child;
            tree.add_child(std::to_string(i),child);
        }
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChildPreallocated)->RangeMultiplier(2)->Range(1024,1024<<5);

// Adding child to deep path
static void BM_HDC_AddChildPathDepth(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "aaa/";
    while (state.KeepRunning()) {
        HDC tree, child;
        tree.add_child(path,child);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChildPathDepth)->RangeMultiplier(2)->Range(1,16);

// Geting child from deep path
static void BM_HDC_GetChildPathDepth(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "aaa/";
    HDC tree, child;
    tree.add_child(path,child);
    while (state.KeepRunning()) {
        HDC node = tree.get(path);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_GetChildPathDepth)->RangeMultiplier(2)->Range(1,16);


// Adding child to path with long string
static void BM_HDC_AddChildPathLength(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "a";
    while (state.KeepRunning()) {
        HDC tree, child;
        tree.add_child(path,child);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AddChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Getting child to path with long string
static void BM_HDC_GetChildPathLength(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "a";
    HDC tree, child;
    tree.add_child(path,child);
    while (state.KeepRunning()) {
        HDC node = tree.get(path);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_GetChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Appending slice to long (but not deep) path
static void BM_HDC_AppendSlice(benchmark::State& state) {
    state.SetItemsProcessed(state.range(0));
    while (state.KeepRunning()) {
        HDC tree, child;
        for (auto i=0;i<state.range(0);i++) tree.append(child);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AppendSlice)->RangeMultiplier(2)->Range(1024,1024<<5);

// Appending slice to long (but not deep) path
static void BM_HDC_AppendSlicePreallocated(benchmark::State& state) {
    state.SetItemsProcessed(state.range(0));
    while (state.KeepRunning()) {
        HDC tree(1024*1024*32);
        for (auto i=0;i<state.range(0);i++) {
            HDC child;
            tree.append(child);
        }
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_AppendSlicePreallocated)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test exists()
static void BM_HDC_HasChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC tree, child;
    for(int i=0;i<n;i++) tree.add_child(to_string(i),child);
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            bool exists = tree.exists(std::to_string(i*n/100));
            if (!exists) std::cout << "Nope.\n"; //This should just trick compiler
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_HasChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get()
static void BM_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC tree, child;
    for(int i=0;i<n;i++) tree.add_child(to_string(i),child);
    std::string paths[100];
    for (int i=0;i<100;i++) {
        paths[i] = std::to_string(rand() % n);
    }
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC node = tree.get(paths[i]);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(16,1024<<5);

// Test get_slice()
static void BM_HDC_GetSliceMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC tree, child;
    for(int i=0;i<n;i++) tree.append(child);
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC slice = tree.get(i*n/100);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_HDC_GetSliceMultipleItems)->RangeMultiplier(2)->Range(16,1024<<5);

