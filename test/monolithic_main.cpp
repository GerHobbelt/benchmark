
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>

#define BUILD_MONOLITHIC 1
#include "monolithic_examples.h"


// for clobber_memory_assembly_test.cc et al:
extern "C" {

	int ExternInt;
	int ExternInt2;
	int ExternInt3;
}



typedef int tool_f();
typedef int tool_fa(int argc, const char** argv);

static int usage();


static struct cmd_info
{
	const char* cmd;
	union
	{
		tool_f* f;
		tool_fa* fa;
	} f;
} commands[] = {
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

	{ "gbenchmark", {.fa = gbenchmark_main } },

	{ "?", {.f = usage } },
	{ "h", {.f = usage } },
	{ "help", {.f = usage } },
	{ "-?", {.f = usage } },
	{ "-h", {.f = usage } },
	{ "--help", {.f = usage } },
};


static void help(void)
{
	fprintf(stderr, "Commands:\n");
	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
	{
		struct cmd_info el = commands[i];
		fprintf(stderr, "  %s\n", el.cmd);
	}
}


static int usage()
{
	fprintf(stderr, "Usage: clipp_demo [command]\n");
	help();
	return 1;
}


static int parse(const char* source, int argc, const char** argv)
{
	if (source)
	{
		for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		{
			struct cmd_info el = commands[i];
			size_t cmd_len = strlen(el.cmd);
			char sentinel = 0;

			if (strlen(source) >= cmd_len)
				sentinel = source[cmd_len];

			if (strncmp(source, el.cmd, cmd_len) == 0 && (sentinel == 0 || isspace(sentinel)))
			{
				return el.f.fa(argc, argv);
			}
		}

		fprintf(stderr, "Unknown command '%s'.\n\nUse 'h' or 'help' command to get a list of supported commands.\n", source);
		return 1;
	}
	else
	{
		return usage();
	}
}

int main(int argc, const char** argv)
{
	return parse(argv[1], argc - 1, argv + 1);
}

