#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>

using namespace std;

// Empty child creation and deletion
static void BM_HDCCreationAndDeletion(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC* h = new HDC();
        delete h;
    }
}
BENCHMARK(BM_HDCCreationAndDeletion);

static void BM_HDCAddChild(benchmark::State& state) {
    state.SetItemsProcessed(state.range_x());
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        for (size_t i = 0;i < state.range_x();i++) tree->add_child(std::to_string(i),new HDC());
        delete tree;
    }
}
BENCHMARK(BM_HDCAddChild)->Range(1000,1000<<5);


static void BM_HDCAddChildPathDepth(benchmark::State& state) {
    state.SetItemsProcessed(state.range_x());
    string path = "";
    for (size_t k=0;k<state.range_x();k++) path += "aaa/";
    int i = 0;
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
}
BENCHMARK(BM_HDCAddChildPathDepth)->Range(1,16);


static void BM_HDCAddChildPathLength(benchmark::State& state) {
    state.SetBytesProcessed(state.range_x());
    string path = "";
    for (size_t k=0;k<state.range_x();k++) path += "a";
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        tree->add_child(path,new HDC());
        delete tree;
    }
}
BENCHMARK(BM_HDCAddChildPathLength)->Range(100,100<<5);

static void BM_HDCAppendSlice(benchmark::State& state) {
    state.SetItemsProcessed(state.range_x());
    while (state.KeepRunning()) {
        HDC* tree = new HDC();
        for (size_t i=0;i<state.range_x();i++) tree->append_slice(new HDC());
        delete tree;
    }
}
BENCHMARK(BM_HDCAppendSlice)->Range(100,100<<3);


//test createlarge
static void BM_HDCCreateDeleteLarge(benchmark::State& state) {
    HDC* g_tree = new HDC();
    int n = state.range_x();
    state.SetItemsProcessed(state.range_x());
    while (state.KeepRunning()) {
        HDC* g_tree = new HDC();
        for(int i=0;i<n;i++) g_tree->add_child(to_string(i),new HDC());
        delete g_tree;
    }
}
BENCHMARK(BM_HDCCreateDeleteLarge)->Range(1000,1000<<5);

// Test has() -- substract previous
static void BM_HDCHasChildMultipleItems(benchmark::State& state) {
    int n = state.range_x();
    state.SetItemsProcessed(state.range_x());
    HDC* g_tree = new HDC();
    for(int i=0;i<n;i++) g_tree->add_child(to_string(i),new HDC());
    while (state.KeepRunning()) {
        // measure
        for (int i=0;i<100;i++) {
            bool has_child = g_tree->has_child(std::to_string(i*99));
        }
    }
    delete g_tree;
}
BENCHMARK(BM_HDCHasChildMultipleItems)->Range(1000,1000<<5);




