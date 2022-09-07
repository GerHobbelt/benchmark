
#pragma once

#if defined(BUILD_MONOLITHIC)

#ifdef __cplusplus
extern "C" {
#endif

	int gbenchmark_args_product_test_main(int argc, const char** argv);
	int gbenchmark_basic_test_main(int argc, const char** argv);
	int gbenchmark_bench_test_main(int argc, const char** argv);
	int gbenchmark_complexity_test_main(int argc, const char** argv);
	int gbenchmark_cxx03_test_main(int argc, const char** argv);
	int gbenchmark_setup_teardown_test_main(int argc, const char** argv);
	int gbenchmark_diagnostics_test_main(int argc, const char** argv);
	int gbenchmark_display_aggregates_only_test_main(int argc, const char** argv);
	int gbenchmark_do_not_optimize_test_main(int argc, const char** argv);
	int gbenchmark_filter_test_main(int argc, const char** argv);
	int gbenchmark_future_test_main(int argc, const char** argv);
	int gbenchmark_internal_threading_test_main(int argc, const char** argv);
	int gbenchmark_map_test_main(int argc, const char** argv);
	int gbenchmark_memory_manager_test_main(int argc, const char** argv);
	int gbenchmark_multiple_ranges_test_main(int argc, const char** argv);
	int gbenchmark_options_test_main(int argc, const char** argv);
	int gbenchmark_perf_counters_test_main(int argc, const char** argv);
	int gbenchmark_register_bench_test_main(int argc, const char** argv);
	int gbenchmark_repetitions_test_main(int argc, const char** argv);
	int gbenchmark_report_aggregates_only_test_main(int argc, const char** argv);
	int gbenchmark_reporter_output_test_main(int argc, const char** argv);
	int gbenchmark_skip_with_error_test_main(int argc, const char** argv);
	int gbenchmark_spec_arg_test_main(int argc, const char** argv);
	int gbenchmark_templated_future_test_main(int argc, const char** argv);
	int gbenchmark_user_counters_tabular_test_main(int argc, const char** argv);
	int gbenchmark_user_counters_test_main(int argc, const char** argv);
	int gbenchmark_user_counters_thousands_test_main(int argc, const char** argv);
	int gbenchmark_spec_arg_verbosity_test_main(int argc, const char** argv);

	int gbenchmark_main(int argc, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
