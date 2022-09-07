
#define BUILD_MONOLITHIC 1
#include "monolithic_examples.h"


// for clobber_memory_assembly_test.cc et al:
#ifdef __cplusplus
extern "C" {
#endif

	int ExternInt;
	int ExternInt2;
	int ExternInt3;

#ifdef __cplusplus
}
#endif


#define USAGE_NAME   "gbenchtest"

#include "monolithic_main_internal_defs.h"

MONOLITHIC_CMD_TABLE_START()
	{ "args_product", {.fa = gbenchmark_args_product_test_main } },
	{ "basic", {.fa = gbenchmark_basic_test_main } },
	{ "bench", {.fa = gbenchmark_bench_test_main } },
	{ "complexity", {.fa = gbenchmark_complexity_test_main } },
	{ "cxx03", {.fa = gbenchmark_cxx03_test_main } },
	{ "diagnostics", {.fa = gbenchmark_diagnostics_test_main } },
	{ "display_aggregates_only", {.fa = gbenchmark_display_aggregates_only_test_main } },
	{ "do_not_optimize", {.fa = gbenchmark_do_not_optimize_test_main } },
	{ "filter", {.fa = gbenchmark_filter_test_main } },
	{ "future", {.fa = gbenchmark_future_test_main } },
	{ "internal_threading", {.fa = gbenchmark_internal_threading_test_main } },
	{ "map", {.fa = gbenchmark_map_test_main } },
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

	{ "gbenchmark", {.fa = gbenchmark_main } },
MONOLITHIC_CMD_TABLE_END();

#include "monolithic_main_tpl.h"
