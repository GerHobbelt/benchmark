
#undef NDEBUG
#include <cstdio>
#include <string>

#include "benchmark/benchmark.h"
#include "output_test.h"

#include "monolithic_examples.h"


#define BENCHMARK_FAMILY_ID "report_aggregates_only_tests"

namespace gbench_aggregates_only_test {

// Ok this test is super ugly. We want to check what happens with the file
// reporter in the presence of ReportAggregatesOnly().
// We do not care about console output, the normal tests check that already.

void BM_SummaryRepeat(benchmark::State& state) {
  for (auto _ : state) {
  }
}
BENCHMARK(BM_SummaryRepeat)->Repetitions(3)->ReportAggregatesOnly();

}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_report_aggregates_only_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  const std::string output = GetFileReporterOutput(BENCHMARK_FAMILY_ID, argc, argv);

  if (SubstrCnt(output, "\"name\": \"BM_SummaryRepeat/repeats:3") != 4 ||
      SubstrCnt(output, "\"name\": \"BM_SummaryRepeat/repeats:3_mean\"") != 1 ||
      SubstrCnt(output, "\"name\": \"BM_SummaryRepeat/repeats:3_median\"") !=
          1 ||
      SubstrCnt(output, "\"name\": \"BM_SummaryRepeat/repeats:3_stddev\"") !=
          1 ||
      SubstrCnt(output, "\"name\": \"BM_SummaryRepeat/repeats:3_cv\"") != 1) {
    std::cout << "Precondition mismatch. Expected to only find four "
                 "occurrences of \"BM_SummaryRepeat/repeats:3\" substring:\n"
                 "\"name\": \"BM_SummaryRepeat/repeats:3_mean\", "
                 "\"name\": \"BM_SummaryRepeat/repeats:3_median\", "
                 "\"name\": \"BM_SummaryRepeat/repeats:3_stddev\", "
                 "\"name\": \"BM_SummaryRepeat/repeats:3_cv\"\nThe entire "
                 "output:\n";
    std::cout << output;
    return 1;
  }

  return 0;
}
