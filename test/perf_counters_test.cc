#undef NDEBUG

#include "../src/perf_counters.h"

#include "benchmark/benchmark.h"
#include "output_test.h"

#include "monolithic_examples.h"


static void BM_Simple(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(state.iterations());
  }
}
BENCHMARK(BM_Simple);
ADD_CASES(TC_JSONOut, {{"\"name\": \"BM_Simple\",$"}});

static void CheckSimple(Results const& e) {
  CHECK_COUNTER_VALUE(e, double, "CYCLES", GT, 0);
  CHECK_COUNTER_VALUE(e, double, "BRANCHES", GT, 0.0);
}
CHECK_BENCHMARK_RESULTS("BM_Simple", &CheckSimple);



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_perf_counters_test_main(cnt, arr)
#endif

int main(int argc, const char** argv) {
  if (!benchmark::internal::PerfCounters::kSupported) {
    return 0;
  }
  RunOutputTests(argc, argv);
  return 0;
}
