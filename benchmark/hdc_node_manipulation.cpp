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
        HDC* tree = new HDC();
        for (size_t i = 0;i < state.range_x();i++) tree->add_child(std::to_string(i),new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
}
BENCHMARK(BM_HDC_AddChild)->RangeMultiplier(2)->Range(1024,1024<<5);

// Adding child to long path
static void BM_HDC_AddChildPathDepth(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range_x();k++) path += "aaa/";
    int i = 0;
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
}
BENCHMARK(BM_HDC_AddChildPathDepth)->Range(1,16);

// Adding child to path with long string
static void BM_HDC_AddChildPathLength(benchmark::State& state) {
    string path = "";
    for (size_t k=0;k<state.range_x();k++) path += "a";
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
}
BENCHMARK(BM_HDC_AddChildPathLength)->RangeMultiplier(2)->Range(1024,1024<<5);

// Appending slice
static void BM_HDC_AppendSlice(benchmark::State& state) {
    state.SetItemsProcessed(state.range_x());
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        for (size_t i=0;i<state.range_x();i++) tree->append_slice(new HDC());
        delete tree;
    }
}
BENCHMARK(BM_HDC_AppendSlice)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test has_child()
static void BM_HDC_HasChildMultipleItems(benchmark::State& state) {
    int n = state.range_x();
    HDC* tree = new HDC();
    for(int i=0;i<n;i++) tree->add_child(to_string(i),new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<n/100;i++) {
            bool has_child = tree->has_child(std::to_string(i*99));
        }
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_HasChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get()
static void BM_HDC_GetChildMultipleItems(benchmark::State& state) {
    int n = state.range_x();
    HDC* tree = new HDC();
    for(int i=0;i<n;i++) tree->add_child(to_string(i),new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<n/100;i++) {
            HDC* child = tree->get(std::to_string(i*99));
        }
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetChildMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);

// Test get_slice()
static void BM_HDC_GetSliceMultipleItems(benchmark::State& state) {
    int n = state.range_x();
    HDC* tree = new HDC();
    for(int i=0;i<n;i++) tree->append_slice(new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<n/100;i++) {
            HDC* slice = tree->get_slice(i*99);
        }
    }
    state.SetItemsProcessed(state.range_x() * state.iterations());
    delete tree;
}
BENCHMARK(BM_HDC_GetSliceMultipleItems)->RangeMultiplier(2)->Range(1024,1024<<5);