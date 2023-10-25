#include <chrono>
#include <thread>

#include "benchmark/benchmark.h"

#if defined(NDEBUG)
#undef NDEBUG
#endif
#include <cassert>

#include "monolithic_examples.h"


void BM_basic(benchmark::State& state) {
  for (auto _ : state) {
  }
}

void BM_basic_slow(benchmark::State& state) {
  std::chrono::milliseconds sleep_duration(state.range(0));
  for (auto _ : state) {
    std::this_thread::sleep_for(
        std::chrono::duration_cast<std::chrono::nanoseconds>(sleep_duration));
  }
}

BENCHMARK(BM_basic);
BENCHMARK(BM_basic)->Arg(42);
BENCHMARK(BM_basic_slow)->Arg(10)->Unit(benchmark::kNanosecond);
BENCHMARK(BM_basic_slow)->Arg(100)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_basic_slow)->Arg(1000)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_basic_slow)->Arg(1000)->Unit(benchmark::kSecond);
BENCHMARK(BM_basic)->Range(1, 8);
BENCHMARK(BM_basic)->RangeMultiplier(2)->Range(1, 8);
BENCHMARK(BM_basic)->DenseRange(10, 15);
BENCHMARK(BM_basic)->Args({42, 42});
BENCHMARK(BM_basic)->Ranges({{64, 512}, {64, 512}});
BENCHMARK(BM_basic)->MinTime(0.7);
BENCHMARK(BM_basic)->MinWarmUpTime(0.8);
BENCHMARK(BM_basic)->MinTime(0.1)->MinWarmUpTime(0.2);
BENCHMARK(BM_basic)->UseRealTime();
BENCHMARK(BM_basic)->ThreadRange(2, 4);
BENCHMARK(BM_basic)->ThreadPerCpu();
BENCHMARK(BM_basic)->Repetitions(3);
BENCHMARK(BM_basic)
    ->RangeMultiplier(std::numeric_limits<int>::max())
    ->Range(std::numeric_limits<int64_t>::min(),
            std::numeric_limits<int64_t>::max());

// Negative ranges
BENCHMARK(BM_basic)->Range(-64, -1);
BENCHMARK(BM_basic)->RangeMultiplier(4)->Range(-8, 8);
BENCHMARK(BM_basic)->DenseRange(-2, 2, 1);
BENCHMARK(BM_basic)->Ranges({{-64, 1}, {-8, -1}});

void CustomArgs(benchmark::internal::Benchmark* b) {
  for (int i = 0; i < 10; ++i) {
    b->Arg(i);
  }
}

BENCHMARK(BM_basic)->Apply(CustomArgs);

void BM_explicit_iteration_count(benchmark::State& state) {
  // Test that benchmarks specified with an explicit iteration count are
  // only run once.
  static bool invoked_before = false;
  assert(!invoked_before);
  invoked_before = true;

  // Test that the requested iteration count is respected.
  assert(state.max_iterations == 42);
  for (auto _ : state) {
  }
  assert(state.iterations() == state.max_iterations);
  assert(state.iterations() == 42);
}
BENCHMARK(BM_explicit_iteration_count)->Iterations(42);



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_options_test_main(cnt, arr)
#endif

BENCHMARK_MAIN();
