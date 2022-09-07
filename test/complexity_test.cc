#undef NDEBUG
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "benchmark/benchmark.h"
#include "output_test.h"

#include "monolithic_examples.h"


namespace {

#define ADD_COMPLEXITY_CASES(...) \
  int CONCAT(dummy, __LINE__) = AddComplexityTest(__VA_ARGS__)

static int AddComplexityTest(const std::string &test_name,
                      const std::string &big_o_test_name,
                      const std::string &rms_test_name,
                      const std::string &big_o, int family_index) {
  SetSubstitutions({{"%name", test_name},
                    {"%bigo_name", big_o_test_name},
                    {"%rms_name", rms_test_name},
                    {"%bigo_str", "[ ]* %float " + big_o},
                    {"%bigo", big_o},
                    {"%rms", "[ ]*[0-9]+ %"}});
  AddCases(
      TC_ConsoleOut,
      {{"^%bigo_name %bigo_str %bigo_str[ ]*$"},
       {"^%bigo_name", MR_Not},  // Assert we we didn't only matched a name.
       {"^%rms_name %rms %rms[ ]*$", MR_Next}});
  AddCases(
      TC_JSONOut,
      {{"\"name\": \"%bigo_name\",$"},
       {"\"family_index\": " + std::to_string(family_index) + ",$", MR_Next},
       {"\"per_family_instance_index\": 0,$", MR_Next},
       {"\"run_name\": \"%name\",$", MR_Next},
       {"\"run_type\": \"aggregate\",$", MR_Next},
       {"\"repetitions\": %int,$", MR_Next},
       {"\"threads\": 1,$", MR_Next},
       {"\"aggregate_name\": \"BigO\",$", MR_Next},
       {"\"aggregate_unit\": \"time\",$", MR_Next},
       {"\"cpu_coefficient\": %float,$", MR_Next},
       {"\"real_coefficient\": %float,$", MR_Next},
       {"\"big_o\": \"%bigo\",$", MR_Next},
       {"\"time_unit\": \"ns\"$", MR_Next},
       {"}", MR_Next},
       {"\"name\": \"%rms_name\",$"},
       {"\"family_index\": " + std::to_string(family_index) + ",$", MR_Next},
       {"\"per_family_instance_index\": 0,$", MR_Next},
       {"\"run_name\": \"%name\",$", MR_Next},
       {"\"run_type\": \"aggregate\",$", MR_Next},
       {"\"repetitions\": %int,$", MR_Next},
       {"\"threads\": 1,$", MR_Next},
       {"\"aggregate_name\": \"RMS\",$", MR_Next},
       {"\"aggregate_unit\": \"percentage\",$", MR_Next},
       {"\"rms\": %float$", MR_Next},
       {"}", MR_Next}});
  AddCases(TC_CSVOut, {{"^\"%bigo_name\",,%float,%float,%bigo,,,,,$"},
                       {"^\"%bigo_name\"", MR_Not},
                       {"^\"%rms_name\",,%float,%float,,,,,,$", MR_Next}});
  return 0;
}

}  // end namespace

// ========================================================================= //
// --------------------------- Testing BigO O(1) --------------------------- //
// ========================================================================= //

static void BM_Complexity_O1(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < 1024; ++i) {
      benchmark::DoNotOptimize(&i);
    }
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Complexity_O1)->Range(1, 1 << 18)->Complexity(benchmark::o1);
BENCHMARK(BM_Complexity_O1)->Range(1, 1 << 18)->Complexity();
BENCHMARK(BM_Complexity_O1)
    ->Range(1, 1 << 18)
    ->Complexity([](benchmark::IterationCount) { return 1.0; });

static const char *one_test_name = "BM_Complexity_O1";
static const char *big_o_1_test_name = "BM_Complexity_O1_BigO";
static const char *rms_o_1_test_name = "BM_Complexity_O1_RMS";
static const char *enum_big_o_1 = "\\([0-9]+\\)";
// FIXME: Tolerate both '(1)' and 'lgN' as output when the complexity is auto
// deduced.
// See https://github.com/google/benchmark/issues/272
static const char *auto_big_o_1 = "(\\([0-9]+\\))|(lgN)";
static const char *lambda_big_o_1 = "f\\(N\\)";

// Add enum tests
ADD_COMPLEXITY_CASES(one_test_name, big_o_1_test_name, rms_o_1_test_name,
                     enum_big_o_1, /*family_index=*/0);

// Add auto enum tests
ADD_COMPLEXITY_CASES(one_test_name, big_o_1_test_name, rms_o_1_test_name,
                     auto_big_o_1, /*family_index=*/1);

// Add lambda tests
ADD_COMPLEXITY_CASES(one_test_name, big_o_1_test_name, rms_o_1_test_name,
                     lambda_big_o_1, /*family_index=*/2);

// ========================================================================= //
// --------------------------- Testing BigO O(N) --------------------------- //
// ========================================================================= //

static std::vector<int> ConstructRandomVector(int64_t size) {
  std::vector<int> v;
  v.reserve(static_cast<size_t>(size));
  for (int i = 0; i < size; ++i) {
    v.push_back(static_cast<int>(std::rand() % size));
  }
  return v;
}

static void BM_Complexity_O_N(benchmark::State &state) {
  auto v = ConstructRandomVector(state.range(0));
  // Test worst case scenario (item not in vector)
  const int64_t item_not_in_vector = state.range(0) * 2;
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::find(v.begin(), v.end(), item_not_in_vector));
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Complexity_O_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity(benchmark::oN);
BENCHMARK(BM_Complexity_O_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity([](benchmark::IterationCount n) -> double {
      return static_cast<double>(n);
    });
BENCHMARK(BM_Complexity_O_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity();

static const char *n_test_name = "BM_Complexity_O_N";
static const char *big_o_n_test_name = "BM_Complexity_O_N_BigO";
static const char *rms_o_n_test_name = "BM_Complexity_O_N_RMS";
static const char *enum_auto_big_o_n = "N";
static const char *lambda_big_o_n = "f\\(N\\)";

// Add enum tests
ADD_COMPLEXITY_CASES(n_test_name, big_o_n_test_name, rms_o_n_test_name,
                     enum_auto_big_o_n, /*family_index=*/3);

// Add lambda tests
ADD_COMPLEXITY_CASES(n_test_name, big_o_n_test_name, rms_o_n_test_name,
                     lambda_big_o_n, /*family_index=*/4);

// ========================================================================= //
// ------------------------- Testing BigO O(N*lgN) ------------------------- //
// ========================================================================= //

static void BM_Complexity_O_N_log_N(benchmark::State &state) {
  auto v = ConstructRandomVector(state.range(0));
  for (auto _ : state) {
    std::sort(v.begin(), v.end());
  }
  state.SetComplexityN(state.range(0));
}
static const double kLog2E = 1.44269504088896340736;
BENCHMARK(BM_Complexity_O_N_log_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity(benchmark::oNLogN);
BENCHMARK(BM_Complexity_O_N_log_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity([](benchmark::IterationCount n) {
      return kLog2E * static_cast<double>(n) * log(static_cast<double>(n));
    });
BENCHMARK(BM_Complexity_O_N_log_N)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 16)
    ->Complexity();

static const char *n_lg_n_test_name = "BM_Complexity_O_N_log_N";
static const char *big_o_n_lg_n_test_name = "BM_Complexity_O_N_log_N_BigO";
static const char *rms_o_n_lg_n_test_name = "BM_Complexity_O_N_log_N_RMS";
static const char *enum_auto_big_o_n_lg_n = "NlgN";
static const char *lambda_big_o_n_lg_n = "f\\(N\\)";

// Add enum tests
ADD_COMPLEXITY_CASES(n_lg_n_test_name, big_o_n_lg_n_test_name,
                     rms_o_n_lg_n_test_name, enum_auto_big_o_n_lg_n,
                     /*family_index=*/6);

// Add lambda tests
ADD_COMPLEXITY_CASES(n_lg_n_test_name, big_o_n_lg_n_test_name,
                     rms_o_n_lg_n_test_name, lambda_big_o_n_lg_n,
                     /*family_index=*/7);

// ========================================================================= //
// -------- Testing formatting of Complexity with captured args ------------ //
// ========================================================================= //

static void BM_ComplexityCaptureArgs(benchmark::State &state, int n) {
  for (auto _ : state) {
    // This test requires a non-zero CPU time to avoid divide-by-zero
    benchmark::DoNotOptimize(state.iterations());
  }
  state.SetComplexityN(n);
}

BENCHMARK_CAPTURE(BM_ComplexityCaptureArgs, capture_test, 100)
    ->Complexity(benchmark::oN)
    ->Ranges({{1, 2}, {3, 4}});

static const std::string complexity_capture_name =
    "BM_ComplexityCaptureArgs/capture_test";

ADD_COMPLEXITY_CASES(complexity_capture_name, complexity_capture_name + "_BigO",
                     complexity_capture_name + "_RMS", "N", /*family_index=*/9);

// ========================================================================= //
// --------------------------- TEST CASES END ------------------------------ //
// ========================================================================= //



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_complexity_test_main(cnt, arr)
#endif

int main(int argc, const char** argv) {
    RunOutputTests(argc, argv);
    return 0;
}
