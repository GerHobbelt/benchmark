#include <memory>

#include "../src/check.h"
#include "benchmark/benchmark.h"
#include "output_test.h"

#include "monolithic_examples.h"


#define BENCHMARK_FAMILY_ID "memory_manager_tests"

namespace gbench_mm_test {

class TestMemoryManager : public benchmark::MemoryManager {
  void Start() override {}
  void Stop(Result& result) override {
    result.num_allocs = 42;
    result.max_bytes_used = 42000;
  }
};

static void BM_empty(benchmark::State& state) {
  for (auto _ : state) {
    auto iterations = static_cast<double>(state.iterations()) *
                      static_cast<double>(state.iterations());
    benchmark::DoNotOptimize(iterations);
  }
}
BENCHMARK(BM_empty);

ADD_CASES(TC_ConsoleOut, {{"^BM_empty %console_report$"}});
ADD_CASES(TC_JSONOut, {{"\"name\": \"BM_empty\",$"},
                       {"\"family_index\": 0,$", MR_Next},
                       {"\"per_family_instance_index\": 0,$", MR_Next},
                       {"\"run_name\": \"BM_empty\",$", MR_Next},
                       {"\"run_type\": \"iteration\",$", MR_Next},
                       {"\"repetitions\": 1,$", MR_Next},
                       {"\"repetition_index\": 0,$", MR_Next},
                       {"\"threads\": 1,$", MR_Next},
                       {"\"iterations\": %int,$", MR_Next},
                       {"\"real_time\": %float,$", MR_Next},
                       {"\"cpu_time\": %float,$", MR_Next},
                       {"\"time_unit\": \"ns\",$", MR_Next},
                       {"\"allocs_per_iter\": %float,$", MR_Next},
                       {"\"max_bytes_used\": 42000$", MR_Next},
                       {"}", MR_Next}});
ADD_CASES(TC_CSVOut, {{"^\"BM_empty\",%csv_report$"}});

}

using namespace gbench_mm_test;


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_memory_manager_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  std::unique_ptr<benchmark::MemoryManager> mm(new TestMemoryManager());

  benchmark::RegisterMemoryManager(mm.get());
  RunOutputTests(BENCHMARK_FAMILY_ID, argc, argv);
  benchmark::RegisterMemoryManager(nullptr);
  return 0;
}
