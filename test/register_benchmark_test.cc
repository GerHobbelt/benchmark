
#undef NDEBUG
#include <cassert>
#include <vector>

#include "../src/check.h"  // NOTE: check.h is for internal use only!
#include "benchmark/benchmark.h"

#include "monolithic_examples.h"

#define BENCHMARK_FAMILY_ID "register_benchmark_tests"

namespace {

class TestReporter : public benchmark::ConsoleReporter {
 public:
  void ReportRuns(const std::vector<Run>& report) override {
    all_runs_.insert(all_runs_.end(), begin(report), end(report));
    ConsoleReporter::ReportRuns(report);
  }

  std::vector<Run> all_runs_;
};

struct TestCase {
  const std::string name;
  const std::string label;
  // Note: not explicit as we rely on it being converted through ADD_CASES.
  TestCase(const std::string& xname) : TestCase(xname, "") {}
  TestCase(const std::string& xname, const std::string& xlabel)
      : name(xname), label(xlabel) {}

  typedef benchmark::BenchmarkReporter::Run Run;

  void CheckRun(Run const& run) const {
    // clang-format off
    BM_CHECK(name == run.benchmark_name()) << "expected " << name << " got "
                                      << run.benchmark_name();
    if (!label.empty()) {
      BM_CHECK(run.report_label == label) << "expected " << label << " got "
                                       << run.report_label;
    } else {
      BM_CHECK(run.report_label.empty());
    }
    // clang-format on
  }
};

std::vector<TestCase> ExpectedResults;

int AddCases(std::initializer_list<TestCase> const& v) {
  for (const auto& N : v) {
    ExpectedResults.push_back(N);
  }
  return 0;
}

#define CONCAT(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x##y
#define ADD_CASES(...) \
  const int CONCAT(dummy, __LINE__) = AddCases({__VA_ARGS__})

}  // end namespace

using ReturnVal = benchmark::internal::Benchmark const* const;

//----------------------------------------------------------------------------//
// Test RegisterBenchmark with no additional arguments
//----------------------------------------------------------------------------//
void BM_function(benchmark::State& state) {
  for (auto _ : state) {
  }
}
BENCHMARK(BM_function);
ReturnVal dummy = benchmark::RegisterBenchmark(BENCHMARK_FAMILY_ID,
    "BM_function_manual_registration", BM_function);
ADD_CASES({"BM_function"}, {"BM_function_manual_registration"});

//----------------------------------------------------------------------------//
// Test RegisterBenchmark with additional arguments
// Note: GCC <= 4.8 do not support this form of RegisterBenchmark because they
//       reject the variadic pack expansion of lambda captures.
//----------------------------------------------------------------------------//

void BM_extra_args(benchmark::State& st, const char* label) {
  for (auto _ : st) {
  }
  st.SetLabel(label);
}
int RegisterFromFunction() {
  std::pair<const char*, const char*> cases[] = {
      {"test1", "One"}, {"test2", "Two"}, {"test3", "Three"}};
  for (auto const& c : cases) {
    benchmark::RegisterBenchmark(BENCHMARK_FAMILY_ID, c.first, &BM_extra_args, c.second);
  }
  return 0;
}
const int dummy2 = RegisterFromFunction();
ADD_CASES({"test1", "One"}, {"test2", "Two"}, {"test3", "Three"});

//----------------------------------------------------------------------------//
// Test RegisterBenchmark with DISABLED_ benchmark
//----------------------------------------------------------------------------//
void DISABLED_BM_function(benchmark::State& state) {
  for (auto _ : state) {
  }
}
BENCHMARK(DISABLED_BM_function);
ReturnVal dummy3 = benchmark::RegisterBenchmark(BENCHMARK_FAMILY_ID, "DISABLED_BM_function_manual",
                                                DISABLED_BM_function);
// No need to add cases because we don't expect them to run.

//----------------------------------------------------------------------------//
// Test RegisterBenchmark with different callable types
//----------------------------------------------------------------------------//

struct CustomFixture {
  void operator()(benchmark::State& st) {
    for (auto _ : st) {
    }
  }
};

void TestRegistrationAtRuntime() {
  {
    CustomFixture fx;
    benchmark::RegisterBenchmark(BENCHMARK_FAMILY_ID, "custom_fixture", fx);
    AddCases({std::string("custom_fixture")});
  }
  {
    const char* x = "42";
    auto capturing_lam = [=](benchmark::State& st) {
      for (auto _ : st) {
      }
      st.SetLabel(x);
    };
    benchmark::RegisterBenchmark(BENCHMARK_FAMILY_ID, "lambda_benchmark", capturing_lam);
    AddCases({{"lambda_benchmark", x}});
  }
}

// Test that all benchmarks, registered at either during static init or runtime,
// are run and the results are passed to the reported.
void RunTestOne() {
  TestRegistrationAtRuntime();

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
}

// Test that ClearRegisteredBenchmarks() clears all previously registered
// benchmarks.
// Also test that new benchmarks can be registered and ran afterwards.
void RunTestTwo() {
  assert(!ExpectedResults.empty() &&
         "must have at least one registered benchmark");
  ExpectedResults.clear();
  benchmark::ClearRegisteredBenchmarks(BENCHMARK_FAMILY_ID);

  TestReporter test_reporter;
  size_t num_ran = benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &test_reporter);
  assert(num_ran == 0);
  assert(test_reporter.all_runs_.begin() == test_reporter.all_runs_.end());

  TestRegistrationAtRuntime();
  num_ran = benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &test_reporter);
  assert(num_ran == ExpectedResults.size());

  typedef benchmark::BenchmarkReporter::Run Run;
  auto EB = ExpectedResults.begin();

  for (Run const& run : test_reporter.all_runs_) {
    assert(EB != ExpectedResults.end());
    EB->CheckRun(run);
    ++EB;
  }
  assert(EB == ExpectedResults.end());
}



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_register_bench_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  benchmark::Initialize(&argc, argv);

  RunTestOne();
  RunTestTwo();

  benchmark::Shutdown();
  return 0;
}
