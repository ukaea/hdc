#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>

using namespace std;

// Empty child creation and deletion
static void BM_HDC_CreationAndDeletion(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC* h = new HDC();
        delete h;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_HDC_CreationAndDeletion);

// Adding childs
static void BM_HDC_AddChild(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC* tree = new HDC(4*1024*1024<<5); // preallocate some space. TODO: at least discuss this with Jakub
        for (size_t i = 0;i < state.range(0);i++) tree->add_child(std::to_string(i),new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
}
BENCHMARK(BM_HDC_AddChild)->RangeMultiplier(2)->Range(1024,1024<<5);

// Adding child to deep path
static void BM_HDC_AddChildPathDepth(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range(0);k++) path += "aaa/";
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_HDC_AddChildPathDepth)->RangeMultiplier(2)->Range(1,16);

// Geting child from deep path
static void BM_HDC_GetChildPathDepth(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range(0);k++) path += "aaa/";
    HDC* tree = new HDC();
    tree->add_child(path,new HDC());
    while (state.KeepRunning()) {
        HDC* node = tree->get_ptr(path);
    }
    state.SetItemsProcessed(state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetChildPathDepth)->RangeMultiplier(2)->Range(1,16);


// Adding child to path with long string
static void BM_HDC_AddChildPathLength(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range(0);k++) path += "a";
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_HDC_AddChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Getting child to path with long string
static void BM_HDC_GetChildPathLength(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range(0);k++) path += "a";
    HDC* tree = new HDC();
    tree->add_child(path,new HDC());
    while (state.KeepRunning()) {
        HDC* node = tree->get_ptr(path);
    }
    state.SetItemsProcessed(state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetChildPathLength)->RangeMultiplier(2)->Range(2,1024);

// Appending slice to long (but not deep) path
static void BM_HDC_AppendSlice(benchmark::State& state) {
    state.SetItemsProcessed(state.range(0));
    while (state.KeepRunning()) {
        HDC* tree = new HDC(1024*1024*32);
//         HDC* tree = new HDC();
        for (size_t i=0;i<state.range(0);i++) tree->append_slice(new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.range(0) * state.iterations());
}
BENCHMARK(BM_HDC_AppendSlice)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test has_child()
static void BM_HDC_HasChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC* tree = new HDC(1024*1024*32);
    for(int i=0;i<n;i++) tree->add_child(to_string(i),new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            bool has_child = tree->has_child(std::to_string(i*n/100));
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_HasChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get()
static void BM_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC* tree = new HDC(1024*1024*32);
    for(int i=0;i<n;i++) tree->add_child(to_string(i),new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC* child = tree->get_ptr(std::to_string(i*n/100));
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get_slice()
static void BM_HDC_GetSliceMultipleItems(benchmark::State& state) {
    int n = state.range(0);
    HDC* tree = new HDC(1024*1024*32);
    for(int i=0;i<n;i++) tree->append_slice(new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            HDC* slice = tree->get_slice(i*n/100);
        }
    }
    state.SetItemsProcessed(100 * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetSliceMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);
