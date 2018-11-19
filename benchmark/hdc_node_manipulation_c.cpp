#include "hdc_benchmark_common_c.hpp"

// Empty child creation and deletion
static void BM_C_HDC_CreationAndDeletion(benchmark::State& state) {
    while (state.KeepRunning()) {
        hdc_t h = hdc_new_empty();
        hdc_clean(h);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_CreationAndDeletion);

// Adding childs
static void BM_C_HDC_AddChild(benchmark::State& state) {
    hdc_t tree = hdc_new_empty();
    char str[20];
    while (state.KeepRunning()) {
        for (auto i = 0;i < state.range(0);i++) {
            sprintf(str,"%d",i);
            hdc_add_child(tree,str,hdc_new_empty());
        }
    }
    hdc_clean(tree);
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AddChild)->RangeMultiplier(2)->Range(1024<<2,1024<<5);

static void BM_C_HDC_AddChildPreallocated(benchmark::State& state) {
    char str[20];
    while (state.KeepRunning()) {
        hdc_t tree = hdc_new_size(4*1024*1024<<5);
        for (auto i = 0;i < state.range(0);i++) {
            sprintf(str,"%d",i);
            hdc_add_child(tree,str,hdc_new_empty());
        }
        hdc_clean(tree);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AddChildPreallocated)->RangeMultiplier(2)->Range(1024,1024<<5);

// Adding child to deep path
static void BM_C_HDC_AddChildPathDepth(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "aaa/";
    const char* cpath = path.c_str();
    while (state.KeepRunning()) {
        hdc_t tree = hdc_new_empty();
        hdc_add_child(tree,(char*)cpath,hdc_new_empty());
        hdc_clean(tree);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AddChildPathDepth)->RangeMultiplier(2)->Range(1,16);

// Geting child from deep path
static void BM_C_HDC_GetChildPathDepth(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "aaa/";
    const char* cpath = path.c_str();
    hdc_t tree = hdc_new_empty();
    hdc_add_child(tree,(char*)cpath,hdc_new_empty());
    while (state.KeepRunning()) {
        hdc_t node = hdc_get(tree,(char*)cpath);
        if (false) hdc_dump(node); // I do not like the warnings
    }
    state.SetItemsProcessed(state.iterations());
    hdc_clean(tree);
    StorageReset();
}
BENCHMARK(BM_C_HDC_GetChildPathDepth)->RangeMultiplier(2)->Range(1,16);


// Adding child to path with long string
static void BM_C_HDC_AddChildPathLength(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "a";
    const char* cpath = path.c_str();
    while (state.KeepRunning()) {
        hdc_t tree = hdc_new_empty();
        hdc_add_child(tree,(char*)cpath,hdc_new_empty());
        hdc_clean(tree);
    }
    state.SetItemsProcessed(state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AddChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Getting child to path with long string
static void BM_C_HDC_GetChildPathLength(benchmark::State& state) {
    string path = "";
    for (auto k=0;k<state.range(0);k++) path += "a";
    const char* cpath = path.c_str();
    hdc_t tree = hdc_new_empty();
    hdc_add_child(tree,(char*)cpath,hdc_new_empty());
    while (state.KeepRunning()) {
        hdc_t node = hdc_get(tree,(char*)cpath);
        if (false) hdc_dump(node);
    }
    state.SetItemsProcessed(state.iterations());
    hdc_clean(tree);
    StorageReset();
}
BENCHMARK(BM_C_HDC_GetChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Appending slice to long (but not deep) path
static void BM_C_HDC_AppendSlice(benchmark::State& state) {
    state.SetItemsProcessed(state.range(0));
    while (state.KeepRunning()) {
        hdc_t tree = hdc_new_empty();
        for (auto i=0;i<state.range(0);i++) hdc_append_slice(tree,hdc_new_empty());
        hdc_clean(tree);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AppendSlice)->RangeMultiplier(2)->Range(1024,1024<<5);

// Appending slice to long (but not deep) path
static void BM_C_HDC_AppendSlicePreallocated(benchmark::State& state) {
    state.SetItemsProcessed(state.range(0));
    while (state.KeepRunning()) {
        hdc_t tree = hdc_new_size(1024*1024*32);
        for (auto i=0;i<state.range(0);i++) hdc_append_slice(tree,hdc_new_empty());
        hdc_clean(tree);
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_AppendSlicePreallocated)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test exists()
static void BM_C_HDC_HasChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    hdc_t tree = hdc_new_empty();
    for(int i=0;i<n;i++) hdc_add_child(tree,(char*)to_string(i).c_str(),hdc_new_empty());
    char cpath[20];
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            sprintf(cpath,"%d",i*n/100);
            hdc_exists(tree,cpath);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    hdc_clean(tree);
    StorageReset();
}
BENCHMARK(BM_C_HDC_HasChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get()
static void BM_C_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    hdc_t tree = hdc_new_empty();
    for(int i=0;i<n;i++) hdc_add_child(tree,(char*)to_string(i).c_str(),hdc_new_empty());
    std::string paths[100];
    for (int i=0;i<100;i++) {
        paths[i] = std::to_string(rand() % n);
    }
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            hdc_t child = hdc_get(tree,(char*)paths[i].c_str());
            if (false) hdc_dump(child); // I do not like the warnings
        }
    }
    hdc_clean(tree);
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(16,1024<<5);

// Test get_slice()
static void BM_C_HDC_GetSliceMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    hdc_t tree = hdc_new_empty();
    for(int i=0;i<n;i++) hdc_append_slice(tree,hdc_new_empty());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            hdc_t slice = hdc_get_slice(tree,"",i*n/100);
            if (false) hdc_dump(slice); // I do not like the warnings
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    StorageReset();
}
BENCHMARK(BM_C_HDC_GetSliceMultipleItems)->RangeMultiplier(2)->Range(16,1024<<5);
