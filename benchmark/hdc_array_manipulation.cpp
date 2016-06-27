#include "hdc.hpp"
#include "benchmark/benchmark.h"
#include <string>

using namespace std;
/*
static void BM_StringCreation(benchmark::State& state) {
    while (state.KeepRunning())
        std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    while (state.KeepRunning())
        std::string copy(x);
}
BENCHMARK(BM_StringCopy);

// Empty child creation
static void BM_HDCCreation(benchmark::State& state) {
    while (state.KeepRunning()) {
        HDC* h = new HDC();
    }
}
BENCHMARK(BM_HDCCreation);*/
