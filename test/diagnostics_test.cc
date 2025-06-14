// Testing:
//   State::PauseTiming()
//   State::ResumeTiming()
// Test that CHECK's within these function diagnose when they are called
// outside of the KeepRunning() loop.
//
// NOTE: Users should NOT include or use src/check.h. This is only done in
// order to test library internals.

#include <cstdlib>
#include <stdexcept>

#include "../src/check.h"
#include "benchmark/benchmark.h"

#include "monolithic_examples.h"

#define BENCHMARK_FAMILY_ID   "diagnostics_tests"

#ifndef NDEBUG

#if defined(__GNUC__) && !defined(__EXCEPTIONS)
#define TEST_HAS_NO_EXCEPTIONS
#endif

void TestHandler() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  throw std::logic_error("");
#else
  std::abort();
#endif
}

void try_invalid_pause_resume(benchmark::State& state) {
#if !defined(TEST_BENCHMARK_LIBRARY_HAS_NO_ASSERTIONS) && \
    !defined(TEST_HAS_NO_EXCEPTIONS)
  try {
    state.PauseTiming();
    std::abort();
  } catch (std::logic_error const&) {
  }
  try {
    state.ResumeTiming();
    std::abort();
  } catch (std::logic_error const&) {
  }
#else
  (void)state;  // avoid unused warning
#endif
}

void BM_diagnostic_test(benchmark::State& state) {
  static bool called_once = false;

  if (!called_once) {
    try_invalid_pause_resume(state);
  }

  for (auto _ : state) {
    auto iterations = static_cast<double>(state.iterations()) *
                      static_cast<double>(state.iterations());
    benchmark::DoNotOptimize(iterations);
  }

  if (!called_once) {
    try_invalid_pause_resume(state);
  }

  called_once = true;
}
BENCHMARK(BM_diagnostic_test);

void BM_diagnostic_test_keep_running(benchmark::State& state) {
  static bool called_once = false;

  if (!called_once) {
    try_invalid_pause_resume(state);
  }

  while (state.KeepRunning()) {
    auto iterations = static_cast<double>(state.iterations()) *
                      static_cast<double>(state.iterations());
    benchmark::DoNotOptimize(iterations);
  }

  if (!called_once) {
    try_invalid_pause_resume(state);
  }

  called_once = true;
}
BENCHMARK(BM_diagnostic_test_keep_running);

#endif // NDEBUG



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_diagnostics_test_main(cnt, arr)
#endif

extern "C"
int main(int argc, const char** argv) {
#ifdef NDEBUG
  // This test is exercising functionality for debug builds, which are not
  // available in release builds. Skip the test if we are in that environment
  // to avoid a test failure.
  std::cout << "Diagnostic test disabled in release build\n";
  (void)argc;
  (void)argv;
#else
  benchmark::MaybeReenterWithoutASLR(argc, argv);
  benchmark::internal::GetAbortHandler() = &TestHandler;
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, false);
  benchmark::Shutdown();
#endif
  return 0;
}
