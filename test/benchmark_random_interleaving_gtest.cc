#include <queue>
#include <string>
#include <vector>

#include "../src/commandlineflags.h"
#include "../src/string_util.h"
#include "benchmark/benchmark.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define BENCHMARK_FAMILY_ID "random_interleaving_gtests"

namespace benchmark {

BM_DECLARE_bool(benchmark_enable_random_interleaving);
BM_DECLARE_string(benchmark_filter);
BM_DECLARE_int32(benchmark_repetitions);

namespace internal {
namespace {

class EventQueue : public std::queue<std::string> {
 public:
  void Put(const std::string& event) { push(event); }

  void Clear() {
    while (!empty()) {
      pop();
    }
  }

  std::string Get() {
    std::string event = front();
    pop();
    return event;
  }
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
EventQueue* const queue = new EventQueue();

class NullReporter : public BenchmarkReporter {
 public:
  bool ReportContext(const Context& /*context*/) override { return true; }
  void ReportRuns(const std::vector<Run>& /* report */) override {}
  void List(const std::vector<benchmark::internal::BenchmarkInstance>&) override {}
};

class BenchmarkTest : public testing::Test {
 public:
  static void SetupHook(int /* num_threads */) { queue->push("Setup"); }

  static void TeardownHook(int /* num_threads */) { queue->push("Teardown"); }

  static void Execute(const std::string& pattern) {
    queue->Clear();

    std::unique_ptr<BenchmarkReporter> reporter(new NullReporter());
    FLAGS_benchmark_filter = pattern;
    RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, reporter.get());

    queue->Put("DONE");  // End marker
  }

  // make sure we backup & recover the FLAGS_benchmark_filter setting which is changed in the Execute() call above!
	// ditto for the global FLAGS_benchmark_repetitions setting which is tweaked in some of these tests below!

  void SetUp() override {
		old_filter_setting_ = FLAGS_benchmark_filter;
    old_repetitions_ = FLAGS_benchmark_repetitions;
	}

  void TearDown() override {
		FLAGS_benchmark_filter = old_filter_setting_;
    FLAGS_benchmark_repetitions = old_repetitions_;
  }
	
private:
	std::string old_filter_setting_;
  int32_t old_repetitions_;
};

void BM_Match1(benchmark::State& state) {
  const int64_t arg = state.range(0);

  for (auto _ : state) {
  }
  queue->Put(StrFormat("BM_Match1/%d", static_cast<int>(arg)));
}
BENCHMARK(BM_Match1)
    ->Iterations(100)
    ->Arg(1)
    ->Arg(2)
    ->Arg(3)
    ->Range(10, 80)
    ->Args({90})
    ->Args({100});

TEST_F(BenchmarkTest, Match1) {
  Execute("BM_Match1");
  ASSERT_EQ("BM_Match1/1", queue->Get());
  ASSERT_EQ("BM_Match1/2", queue->Get());
  ASSERT_EQ("BM_Match1/3", queue->Get());
  ASSERT_EQ("BM_Match1/10", queue->Get());
  ASSERT_EQ("BM_Match1/64", queue->Get());
  ASSERT_EQ("BM_Match1/80", queue->Get());
  ASSERT_EQ("BM_Match1/90", queue->Get());
  ASSERT_EQ("BM_Match1/100", queue->Get());
  ASSERT_EQ("DONE", queue->Get());
}

TEST_F(BenchmarkTest, Match1WithRepetition) {
  FLAGS_benchmark_repetitions = 2;

  Execute("BM_Match1/(64|80)");
  ASSERT_EQ("BM_Match1/64", queue->Get());
  ASSERT_EQ("BM_Match1/64", queue->Get());
  ASSERT_EQ("BM_Match1/80", queue->Get());
  ASSERT_EQ("BM_Match1/80", queue->Get());
  ASSERT_EQ("DONE", queue->Get());
}

TEST_F(BenchmarkTest, Match1WithRandomInterleaving) {
  FLAGS_benchmark_enable_random_interleaving = true;
  FLAGS_benchmark_repetitions = 100;

  std::map<std::string, int> element_count;
  std::map<std::string, int> interleaving_count;
  Execute("BM_Match1/(64|80)");
  for (int i = 0; i < 100; ++i) {
    std::vector<std::string> interleaving;
    interleaving.push_back(queue->Get());
    interleaving.push_back(queue->Get());
    element_count[interleaving[0]]++;
    element_count[interleaving[1]]++;
    interleaving_count[StrFormat("%s,%s", interleaving[0].c_str(),
                                 interleaving[1].c_str())]++;
  }
  EXPECT_EQ(element_count["BM_Match1/64"], 100) << "Unexpected repetitions.";
  EXPECT_EQ(element_count["BM_Match1/80"], 100) << "Unexpected repetitions.";
  EXPECT_GE(interleaving_count.size(), 2) << "Interleaving was not randomized.";
  ASSERT_EQ("DONE", queue->Get());
}

}  // namespace
}  // namespace internal
}  // namespace benchmark
