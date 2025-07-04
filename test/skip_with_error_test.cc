
#undef NDEBUG
#include <cassert>
#include <vector>

#include "../src/check.h"  // NOTE: check.h is for internal use only!
#include "benchmark/benchmark.h"

#include "monolithic_examples.h"


#define BENCHMARK_FAMILY_ID "skip_with_error_tests"

namespace {

class TestReporter : public benchmark::ConsoleReporter {
 public:
  bool ReportContext(const Context& context) override {
    return ConsoleReporter::ReportContext(context);
  };

  void ReportRuns(const std::vector<Run>& report) override {
    all_runs_.insert(all_runs_.end(), begin(report), end(report));
    ConsoleReporter::ReportRuns(report);
  }

  TestReporter() {}
  ~TestReporter() override {}

  mutable std::vector<Run> all_runs_;
};

struct TestCase {
  std::string name;
  bool error_occurred;
  std::string error_message;

  typedef benchmark::BenchmarkReporter::Run Run;

  void CheckRun(Run const& run) const {
    BM_CHECK(name == run.benchmark_name())
        << "expected " << name << " got " << run.benchmark_name();
    BM_CHECK_EQ(error_occurred,
                benchmark::internal::SkippedWithError == run.skipped);
    BM_CHECK(error_message == run.skip_message);
    if (error_occurred) {
      // BM_CHECK(run.iterations == 0);
    } else {
      BM_CHECK(run.iterations != 0);
    }
  }
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<TestCase> ExpectedResults;

int AddCases(const std::string& base_name,
             std::initializer_list<TestCase> const& v) {
  for (auto TC : v) {
    TC.name = base_name + TC.name;
    ExpectedResults.push_back(std::move(TC));
  }
  return 0;
}

#define CONCAT(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x##y
#define ADD_CASES(...) const int CONCAT(dummy, __LINE__) = AddCases(__VA_ARGS__)

}  // end namespace

void BM_error_no_running(benchmark::State& state) {
  state.SkipWithError("error message");
}
BENCHMARK(BM_error_no_running);
ADD_CASES("BM_error_no_running", {{"", true, "error message"}});

void BM_error_before_running(benchmark::State& state) {
  state.SkipWithError("error message");
  while (state.KeepRunning()) {
    assert(false);
  }
}
BENCHMARK(BM_error_before_running);
ADD_CASES("BM_error_before_running", {{"", true, "error message"}});

void BM_error_before_running_batch(benchmark::State& state) {
  state.SkipWithError("error message");
  while (state.KeepRunningBatch(17)) {
    assert(false);
  }
}
BENCHMARK(BM_error_before_running_batch);
ADD_CASES("BM_error_before_running_batch", {{"", true, "error message"}});

void BM_error_before_running_range_for(benchmark::State& state) {
  state.SkipWithError("error message");
  for (auto _ : state) {
    assert(false);
  }
}
BENCHMARK(BM_error_before_running_range_for);
ADD_CASES("BM_error_before_running_range_for", {{"", true, "error message"}});

void BM_error_during_running(benchmark::State& state) {
  int first_iter = 1;
  while (state.KeepRunning()) {
    if (state.range(0) == 1 && state.thread_index() <= (state.threads() / 2)) {
      assert(first_iter != 0);
      first_iter = 0;
      state.SkipWithError("error message");
    } else {
      state.PauseTiming();
      state.ResumeTiming();
    }
  }
}
BENCHMARK(BM_error_during_running)->Arg(1)->Arg(2)->ThreadRange(1, 8);
ADD_CASES("BM_error_during_running", {{"/1/threads:1", true, "error message"},
                                      {"/1/threads:2", true, "error message"},
                                      {"/1/threads:4", true, "error message"},
                                      {"/1/threads:8", true, "error message"},
                                      {"/2/threads:1", false, ""},
                                      {"/2/threads:2", false, ""},
                                      {"/2/threads:4", false, ""},
                                      {"/2/threads:8", false, ""}});

void BM_error_during_running_ranged_for(benchmark::State& state) {
  assert(state.max_iterations > 3 && "test requires at least a few iterations");
  bool first_iter = true;
  // NOTE: Users should not write the for loop explicitly.
  for (auto It = state.begin(), End = state.end(); It != End; ++It) {
    if (state.range(0) == 1) {
      assert(first_iter);
      first_iter = false;
      (void)first_iter;
      state.SkipWithError("error message");
      // Test the unfortunate but documented behavior that the ranged-for loop
      // doesn't automatically terminate when SkipWithError is set.
      assert(++It != End);
      break;  // Required behavior
    }
  }
}
BENCHMARK(BM_error_during_running_ranged_for)->Arg(1)->Arg(2)->Iterations(5);
ADD_CASES("BM_error_during_running_ranged_for",
          {{"/1/iterations:5", true, "error message"},
           {"/2/iterations:5", false, ""}});

void BM_error_after_running(benchmark::State& state) {
  for (auto _ : state) {
    auto iterations = static_cast<double>(state.iterations()) *
                      static_cast<double>(state.iterations());
    benchmark::DoNotOptimize(iterations);
  }
  if (state.thread_index() <= (state.threads() / 2)) {
    state.SkipWithError("error message");
  }
}
BENCHMARK(BM_error_after_running)->ThreadRange(1, 8);
ADD_CASES("BM_error_after_running", {{"/threads:1", true, "error message"},
                                     {"/threads:2", true, "error message"},
                                     {"/threads:4", true, "error message"},
                                     {"/threads:8", true, "error message"}});

void BM_error_while_paused(benchmark::State& state) {
  bool first_iter = true;
  while (state.KeepRunning()) {
    if (state.range(0) == 1 && state.thread_index() <= (state.threads() / 2)) {
      assert(first_iter);
      first_iter = false;
      state.PauseTiming();
      state.SkipWithError("error message");
    } else {
      state.PauseTiming();
      state.ResumeTiming();
    }
  }
}
BENCHMARK(BM_error_while_paused)->Arg(1)->Arg(2)->ThreadRange(1, 8);
ADD_CASES("BM_error_while_paused", {{"/1/threads:1", true, "error message"},
                                    {"/1/threads:2", true, "error message"},
                                    {"/1/threads:4", true, "error message"},
                                    {"/1/threads:8", true, "error message"},
                                    {"/2/threads:1", false, ""},
                                    {"/2/threads:2", false, ""},
                                    {"/2/threads:4", false, ""},
                                    {"/2/threads:8", false, ""}});



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_skip_with_error_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  benchmark::Initialize(&argc, argv);

  TestReporter test_reporter;
  benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &test_reporter);

  typedef benchmark::BenchmarkReporter::Run Run;
  auto EB = ExpectedResults.begin();

  for (Run const& run : test_reporter.all_runs_) {
    assert(EB != ExpectedResults.end());
    EB->CheckRun(run);
    ++EB;
  }
  assert(EB == ExpectedResults.end());

  benchmark::Shutdown();

  return 0;
}

void BM_malformed(benchmark::State&) {
  // NOTE: empty body wanted. No thing else.
}
BENCHMARK(BM_malformed);
ADD_CASES("BM_malformed",
          {{"", true,
            "The benchmark didn't run, nor was it explicitly skipped. Please "
            "call 'SkipWithXXX` in your benchmark as appropriate."}});
