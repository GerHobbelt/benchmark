#include <cassert>
#include <cstdlib>
#include <memory>
#include <vector>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"

#define BENCHMARK_FAMILY_ID "profiler_manager_iterations"

// Tests that we can specify the number of profiler iterations with
// --benchmark_min_time=<NUM>x.
namespace {

int iteration_count = 0;
int end_profiler_iteration_count = 0;

class TestProfilerManager : public benchmark::ProfilerManager {
  void AfterSetupStart() override { iteration_count = 0; }
  void BeforeTeardownStop() override {
    end_profiler_iteration_count = iteration_count;
  }
};

class NullReporter : public benchmark::BenchmarkReporter {
 public:
  bool ReportContext(const Context& /*context*/) override { return true; }
  void ReportRuns(const std::vector<Run>& /* report */) override {}
  void List(const std::vector<benchmark::internal::BenchmarkInstance>&) override {}
};

}  // end namespace

static void BM_MyBench(benchmark::State& state) {
  for (auto s : state) {
    ++iteration_count;
  }
}
BENCHMARK(BM_MyBench);

#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr) gbenchmark_profiler_manager_iterations_test_main(cnt, arr)
#endif

int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  // Make a fake argv and append the new --benchmark_profiler_iterations=<foo>
  // to it.
  int fake_argc = argc + 1;
  std::vector<const char*> fake_argv(static_cast<size_t>(fake_argc));
  for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
    fake_argv[i] = argv[i];
  }
  fake_argv[static_cast<size_t>(argc)] = "--benchmark_min_time=4x";

  std::unique_ptr<benchmark::ProfilerManager> pm(new TestProfilerManager());
  benchmark::RegisterProfilerManager(pm.get());

  benchmark::Initialize(&fake_argc, fake_argv.data());

  NullReporter null_reporter;
  const size_t returned_count =
      benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &null_reporter, "BM_MyBench");
  assert(returned_count == 1);

  // Check the executed iters.
  assert(end_profiler_iteration_count == 4);

  benchmark::RegisterProfilerManager(nullptr);
  return 0;
}
