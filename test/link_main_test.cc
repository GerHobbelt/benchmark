#include "benchmark/benchmark.h"

#define BENCHMARK_FAMILY_ID "link_main_tests"

static void BM_empty(benchmark::State& state) {
  for (auto _ : state) {
    auto iterations = static_cast<double>(state.iterations()) *
                      static_cast<double>(state.iterations());
    benchmark::DoNotOptimize(iterations);
  }
}

BENCHMARK(BM_empty);
