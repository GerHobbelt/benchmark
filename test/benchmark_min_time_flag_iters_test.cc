#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"

#define BENCHMARK_FAMILY_ID "min_time_flag_iters_tests"

// Tests that we can specify the number of iterations with
// --benchmark_min_time=<NUM>x.
namespace {

class TestReporter : public benchmark::ConsoleReporter {
 public:
  virtual bool ReportContext(const Context& context) BENCHMARK_OVERRIDE {
    return ConsoleReporter::ReportContext(context);
  };

  virtual void ReportRuns(const std::vector<Run>& report) BENCHMARK_OVERRIDE {
    assert(report.size() == 1);
    iter_nums_.push_back(report[0].iterations);
    ConsoleReporter::ReportRuns(report);
  };

  TestReporter() {}

  virtual ~TestReporter() {}

  const std::vector<benchmark::IterationCount>& GetIters() const {
    return iter_nums_;
  }

 private:
  std::vector<benchmark::IterationCount> iter_nums_;
};

}  // end namespace

static void BM_MyBench(benchmark::State& state) {
  for (auto s : state) {
  }
}
BENCHMARK(BM_MyBench);


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr) gbenchmark_min_time_flag_iters_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  // Make a fake argv and append the new --benchmark_min_time=<foo> to it.
  int fake_argc = argc + 1;
  const char** fake_argv = new const char*[static_cast<size_t>(fake_argc)];
  for (int i = 0; i < argc; ++i) fake_argv[i] = argv[i];
  fake_argv[argc] = "--benchmark_min_time=4x";

  benchmark::Initialize(&fake_argc, fake_argv);

  TestReporter test_reporter;
  const size_t returned_count =
      benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &test_reporter, "BM_MyBench");
  assert(returned_count == 1);

  // Check the executed iters.
  const std::vector<benchmark::IterationCount> iters = test_reporter.GetIters();
  assert(!iters.empty() && iters[0] == 4);

  benchmark::Shutdown();

  delete[] fake_argv;
  return 0;
}
