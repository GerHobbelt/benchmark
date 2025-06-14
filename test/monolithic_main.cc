
#define BUILD_MONOLITHIC 1
#include "monolithic_examples.h"

#include "gtest/gtest.h"


// for clobber_memory_assembly_test.cc et al:
#if defined(_MSC_VER)
#define WEAK_ATTR __declspec(selectany)
#elif __has_attribute(weak) || (defined(__GNUC__) && defined(__attribute__))
// On gcc & clang, __has_attribute can test for __attribute__((attr)); we used
// that feature to see if the __attribute__() feature is supported at all in
// your active compiler.
#define WEAK_ATTR __attribute__((weak))
#else
#undef WEAK_ATTR
#endif

// only can do this at linker time when weak references are supported by the compiler:
#ifdef WEAK_ATTR
#ifdef __cplusplus
extern "C" {
#endif

WEAK_ATTR int ExternInt = 10101;
WEAK_ATTR int ExternInt2 = 20202;
WEAK_ATTR int ExternInt3 = 30303;

#ifdef __cplusplus
}
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////


static int gbenchmark_unit_tests_main(int argc, const char** argv) {
  printf("Running gbenchmark's unit tests...\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

////////////////////////////////////////////////////////////////////////////////////////////////


#define USAGE_NAME   "gbenchtest"

#include "monolithic_main_internal_defs.h"

MONOLITHIC_CMD_TABLE_START()
	{ "unit_tests", {.fa = gbenchmark_unit_tests_main } },
	{ "args_product", {.fa = gbenchmark_args_product_test_main } },
	{ "basic", {.fa = gbenchmark_basic_test_main } },
	{ "bench", {.fa = gbenchmark_bench_test_main } },
	{ "complexity", {.fa = gbenchmark_complexity_test_main } },
	{ "diagnostics", {.fa = gbenchmark_diagnostics_test_main } },
	{ "display_aggregates_only", {.fa = gbenchmark_display_aggregates_only_test_main } },
	{ "do_not_optimize", {.fa = gbenchmark_do_not_optimize_test_main } },
	{ "filter", {.fa = gbenchmark_filter_test_main } },
	{ "future", {.fa = gbenchmark_future_test_main } },
	{ "internal_threading", {.fa = gbenchmark_internal_threading_test_main } },
	{ "map", {.fa = gbenchmark_map_test_main } },
	{ "min_time_flag_iters", {.fa = gbenchmark_min_time_flag_iters_test_main } },
	{ "min_time_flag_time", {.fa = gbenchmark_min_time_flag_time_test_main } },
	{ "profiler_manager", {.fa = gbenchmark_profiler_manager_test_main } },
	{ "memory_manager", {.fa = gbenchmark_memory_manager_test_main } },
	{ "multiple_ranges", {.fa = gbenchmark_multiple_ranges_test_main } },
	{ "options", {.fa = gbenchmark_options_test_main } },
	{ "perf_counters", {.fa = gbenchmark_perf_counters_test_main } },
	{ "register_bench", {.fa = gbenchmark_register_bench_test_main } },
	{ "repetitions", {.fa = gbenchmark_repetitions_test_main } },
	{ "report_aggregates_only", {.fa = gbenchmark_report_aggregates_only_test_main } },
	{ "reporter_output", {.fa = gbenchmark_reporter_output_test_main } },
	{ "skip_with_error", {.fa = gbenchmark_skip_with_error_test_main } },
	{ "spec_arg", {.fa = gbenchmark_spec_arg_test_main } },
	{ "setup_teardown", {.fa = gbenchmark_setup_teardown_test_main } },
	{ "templated_future", {.fa = gbenchmark_templated_future_test_main } },
	{ "user_counters", {.fa = gbenchmark_user_counters_test_main } },
	{ "user_counters_tabular", {.fa = gbenchmark_user_counters_tabular_test_main } },
	{ "user_counters_thousands", {.fa = gbenchmark_user_counters_thousands_test_main } },
	{ "spec_arg_verbosity", {.fa = gbenchmark_spec_arg_verbosity_test_main } },
	{ "min_rel_accuracy_flag", {.fa = gbenchmark_min_rel_accuracy_flag_test_main } },
	{ "manual_threading", {.fa = gbenchmark_manual_threading_test_main } },
	{ "profiler_manager_iterations", {.fa = gbenchmark_profiler_manager_iterations_test_main } },
	{ "templated_fixture_method", {.fa = gbenchmark_templated_fixture_method_test_main } },

	{ "gbenchmark", {.fa = gbenchmark_main } },
MONOLITHIC_CMD_TABLE_END();

#include "monolithic_main_tpl.h"
