
#undef NDEBUG

#include "benchmark/benchmark.h"
#include "output_test.h"

#include "monolithic_examples.h"


#define BENCHMARK_FAMILY_ID "user_counters_thousands_tests"

namespace gbench_user_counters_thousands_test {

// ========================================================================= //
// ------------------------ Thousands Customisation ------------------------ //
// ========================================================================= //

void BM_Counters_Thousands(benchmark::State& state) {
  for (auto _ : state) {
  }
  namespace bm = benchmark;
  state.counters.insert({
      {"t0_1000000DefaultBase",
       bm::Counter(1000 * 1000, bm::Counter::kDefaults)},
      {"t1_1000000Base1000", bm::Counter(1000 * 1000, bm::Counter::kDefaults,
                                         bm::Counter::OneK::kIs1000)},
      {"t2_1000000Base1024", bm::Counter(1000 * 1000, bm::Counter::kDefaults,
                                         bm::Counter::OneK::kIs1024)},
      {"t3_1048576Base1000", bm::Counter(1024 * 1024, bm::Counter::kDefaults,
                                         bm::Counter::OneK::kIs1000)},
      {"t4_1048576Base1024", bm::Counter(1024 * 1024, bm::Counter::kDefaults,
                                         bm::Counter::OneK::kIs1024)},
  });
}
BENCHMARK(BM_Counters_Thousands)->Repetitions(2);
ADD_CASES(
    TC_ConsoleOut,
    {
        {"^BM_Counters_Thousands/repeats:2 %console_report "
         "t0_1000000DefaultBase=1M "
         "t1_1000000Base1000=1M t2_1000000Base1024=976.56[23]Ki "
         "t3_1048576Base1000=1.04858M t4_1048576Base1024=1Mi$"},
        {"^BM_Counters_Thousands/repeats:2 %console_report "
         "t0_1000000DefaultBase=1M "
         "t1_1000000Base1000=1M t2_1000000Base1024=976.56[23]Ki "
         "t3_1048576Base1000=1.04858M t4_1048576Base1024=1Mi$"},
        {"^BM_Counters_Thousands/repeats:2_mean %console_report "
         "t0_1000000DefaultBase=1M t1_1000000Base1000=1M "
         "t2_1000000Base1024=976.56[23]Ki t3_1048576Base1000=1.04858M "
         "t4_1048576Base1024=1Mi$"},
        {"^BM_Counters_Thousands/repeats:2_median %console_report "
         "t0_1000000DefaultBase=1M t1_1000000Base1000=1M "
         "t2_1000000Base1024=976.56[23]Ki t3_1048576Base1000=1.04858M "
         "t4_1048576Base1024=1Mi$"},
        {"^BM_Counters_Thousands/repeats:2_stddev %console_time_only_report [ "
         "]*2 t0_1000000DefaultBase=0 t1_1000000Base1000=0 "
         "t2_1000000Base1024=0 t3_1048576Base1000=0 t4_1048576Base1024=0$"},
    });
ADD_CASES(TC_JSONOut,
          {{"\"name\": \"BM_Counters_Thousands/repeats:2\",$"},
           {"\"family_index\": 0,$", MR_Next},
           {"\"per_family_instance_index\": 0,$", MR_Next},
           {"\"run_name\": \"BM_Counters_Thousands/repeats:2\",$", MR_Next},
           {"\"run_type\": \"iteration\",$", MR_Next},
           {"\"repetitions\": 2,$", MR_Next},
           {"\"repetition_index\": 0,$", MR_Next},
           {"\"threads\": 1,$", MR_Next},
           {"\"iterations\": %int,$", MR_Next},
           {"\"real_time\": %float,$", MR_Next},
           {"\"cpu_time\": %float,$", MR_Next},
           {"\"time_unit\": \"ns\",$", MR_Next},
           {"\"t0_1000000DefaultBase\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t1_1000000Base1000\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t2_1000000Base1024\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t3_1048576Base1000\": 1\\.048576(0)*e\\+(0)*6,$", MR_Next},
           {"\"t4_1048576Base1024\": 1\\.048576(0)*e\\+(0)*6$", MR_Next},
           {"}", MR_Next}});
ADD_CASES(TC_JSONOut,
          {{"\"name\": \"BM_Counters_Thousands/repeats:2\",$"},
           {"\"family_index\": 0,$", MR_Next},
           {"\"per_family_instance_index\": 0,$", MR_Next},
           {"\"run_name\": \"BM_Counters_Thousands/repeats:2\",$", MR_Next},
           {"\"run_type\": \"iteration\",$", MR_Next},
           {"\"repetitions\": 2,$", MR_Next},
           {"\"repetition_index\": 1,$", MR_Next},
           {"\"threads\": 1,$", MR_Next},
           {"\"iterations\": %int,$", MR_Next},
           {"\"real_time\": %float,$", MR_Next},
           {"\"cpu_time\": %float,$", MR_Next},
           {"\"time_unit\": \"ns\",$", MR_Next},
           {"\"t0_1000000DefaultBase\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t1_1000000Base1000\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t2_1000000Base1024\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t3_1048576Base1000\": 1\\.048576(0)*e\\+(0)*6,$", MR_Next},
           {"\"t4_1048576Base1024\": 1\\.048576(0)*e\\+(0)*6$", MR_Next},
           {"}", MR_Next}});
ADD_CASES(TC_JSONOut,
          {{"\"name\": \"BM_Counters_Thousands/repeats:2_mean\",$"},
           {"\"family_index\": 0,$", MR_Next},
           {"\"per_family_instance_index\": 0,$", MR_Next},
           {"\"run_name\": \"BM_Counters_Thousands/repeats:2\",$", MR_Next},
           {"\"run_type\": \"aggregate\",$", MR_Next},
           {"\"repetitions\": 2,$", MR_Next},
           {"\"threads\": 1,$", MR_Next},
           {"\"aggregate_name\": \"mean\",$", MR_Next},
           {"\"aggregate_unit\": \"time\",$", MR_Next},
           {"\"iterations\": 2,$", MR_Next},
           {"\"real_time\": %float,$", MR_Next},
           {"\"cpu_time\": %float,$", MR_Next},
           {"\"time_unit\": \"ns\",$", MR_Next},
           {"\"t0_1000000DefaultBase\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t1_1000000Base1000\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t2_1000000Base1024\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t3_1048576Base1000\": 1\\.048576(0)*e\\+(0)*6,$", MR_Next},
           {"\"t4_1048576Base1024\": 1\\.048576(0)*e\\+(0)*6$", MR_Next},
           {"}", MR_Next}});
ADD_CASES(TC_JSONOut,
          {{"\"name\": \"BM_Counters_Thousands/repeats:2_median\",$"},
           {"\"family_index\": 0,$", MR_Next},
           {"\"per_family_instance_index\": 0,$", MR_Next},
           {"\"run_name\": \"BM_Counters_Thousands/repeats:2\",$", MR_Next},
           {"\"run_type\": \"aggregate\",$", MR_Next},
           {"\"repetitions\": 2,$", MR_Next},
           {"\"threads\": 1,$", MR_Next},
           {"\"aggregate_name\": \"median\",$", MR_Next},
           {"\"aggregate_unit\": \"time\",$", MR_Next},
           {"\"iterations\": 2,$", MR_Next},
           {"\"real_time\": %float,$", MR_Next},
           {"\"cpu_time\": %float,$", MR_Next},
           {"\"time_unit\": \"ns\",$", MR_Next},
           {"\"t0_1000000DefaultBase\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t1_1000000Base1000\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t2_1000000Base1024\": 1\\.(0)*e\\+(0)*6,$", MR_Next},
           {"\"t3_1048576Base1000\": 1\\.048576(0)*e\\+(0)*6,$", MR_Next},
           {"\"t4_1048576Base1024\": 1\\.048576(0)*e\\+(0)*6$", MR_Next},
           {"}", MR_Next}});
ADD_CASES(TC_JSONOut,
          {{"\"name\": \"BM_Counters_Thousands/repeats:2_stddev\",$"},
           {"\"family_index\": 0,$", MR_Next},
           {"\"per_family_instance_index\": 0,$", MR_Next},
           {"\"run_name\": \"BM_Counters_Thousands/repeats:2\",$", MR_Next},
           {"\"run_type\": \"aggregate\",$", MR_Next},
           {"\"repetitions\": 2,$", MR_Next},
           {"\"threads\": 1,$", MR_Next},
           {"\"aggregate_name\": \"stddev\",$", MR_Next},
           {"\"aggregate_unit\": \"time\",$", MR_Next},
           {"\"iterations\": 2,$", MR_Next},
           {"\"real_time\": %float,$", MR_Next},
           {"\"cpu_time\": %float,$", MR_Next},
           {"\"time_unit\": \"ns\",$", MR_Next},
           {"\"t0_1000000DefaultBase\": 0\\.(0)*e\\+(0)*,$", MR_Next},
           {"\"t1_1000000Base1000\": 0\\.(0)*e\\+(0)*,$", MR_Next},
           {"\"t2_1000000Base1024\": 0\\.(0)*e\\+(0)*,$", MR_Next},
           {"\"t3_1048576Base1000\": 0\\.(0)*e\\+(0)*,$", MR_Next},
           {"\"t4_1048576Base1024\": 0\\.(0)*e\\+(0)*$", MR_Next},
           {"}", MR_Next}});

ADD_CASES(
    TC_CSVOut,
    {{"^\"BM_Counters_Thousands/"
      "repeats:2\",%csv_report,1e\\+(0)*6,1e\\+(0)*6,1e\\+(0)*6,1\\.04858e\\+("
      "0)*6,1\\.04858e\\+(0)*6$"},
     {"^\"BM_Counters_Thousands/"
      "repeats:2\",%csv_report,1e\\+(0)*6,1e\\+(0)*6,1e\\+(0)*6,1\\.04858e\\+("
      "0)*6,1\\.04858e\\+(0)*6$"},
     {"^\"BM_Counters_Thousands/"
      "repeats:2_mean\",%csv_report,1e\\+(0)*6,1e\\+(0)*6,1e\\+(0)*6,1\\."
      "04858e\\+(0)*6,1\\.04858e\\+(0)*6$"},
     {"^\"BM_Counters_Thousands/"
      "repeats:2_median\",%csv_report,1e\\+(0)*6,1e\\+(0)*6,1e\\+(0)*6,1\\."
      "04858e\\+(0)*6,1\\.04858e\\+(0)*6$"},
     {"^\"BM_Counters_Thousands/repeats:2_stddev\",%csv_report,0,0,0,0,0$"}});
// VS2013 does not allow this function to be passed as a lambda argument
// to CHECK_BENCHMARK_RESULTS()
void CheckThousands(Results const& e) {
  if (e.name != "BM_Counters_Thousands/repeats:2") {
    return;  // Do not check the aggregates!
  }

  // check that the values are within 0.01% of the expected values
  CHECK_FLOAT_COUNTER_VALUE(e, "t0_1000000DefaultBase", EQ, 1000 * 1000,
                            0.0001);
  CHECK_FLOAT_COUNTER_VALUE(e, "t1_1000000Base1000", EQ, 1000 * 1000, 0.0001);
  CHECK_FLOAT_COUNTER_VALUE(e, "t2_1000000Base1024", EQ, 1000 * 1000, 0.0001);
  CHECK_FLOAT_COUNTER_VALUE(e, "t3_1048576Base1000", EQ, 1024 * 1024, 0.0001);
  CHECK_FLOAT_COUNTER_VALUE(e, "t4_1048576Base1024", EQ, 1024 * 1024, 0.0001);
}
CHECK_BENCHMARK_RESULTS("BM_Counters_Thousands", &CheckThousands);

// ========================================================================= //
// --------------------------- TEST CASES END ------------------------------ //
// ========================================================================= //

}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_user_counters_thousands_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  RunOutputTests(BENCHMARK_FAMILY_ID, argc, argv);
  return 0;
}
