// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "benchmark/benchmark.h"

#define BENCHMARK_FAMILY_ID "register_list_gtests"

namespace benchmark {
namespace internal {

// Helper function to register benchmarks
void RegisterBenchmarks(const std::string& family) {
    benchmark::RegisterBenchmark(family, "BM_simple", [](benchmark::State& state) {
        for (auto _ : state) {}
    });
    benchmark::RegisterBenchmark(family, "BM_complex", [](benchmark::State& state) {
            for (auto _ : state) {
        // Simulating a complex operation
        for (int i = 0; i < 1000; ++i) {
            benchmark::DoNotOptimize(i * i);
        }
    }
    });
    benchmark::RegisterBenchmark(family, "BM_special_chars", [](benchmark::State& state) {
        for (auto _ : state) {}
    })->Name("BM_special!@#");
}

class ReporterListTest : public ::testing::Test {
protected:
    std::stringstream ss;
    BenchmarkReporter::Context context;

    void SetUp() override {
      benchmark::ClearRegisteredBenchmarks(BENCHMARK_FAMILY_ID);
        ss.str("");
        ss.clear();
    }

    void RunList(BenchmarkReporter& reporter) {
      RegisterBenchmarks(BENCHMARK_FAMILY_ID);  // Register all benchmarks
      benchmark::RunSpecifiedBenchmarks(BENCHMARK_FAMILY_ID, &reporter);
    }
};

// Tests the behavior of reporters when no benchmarks are registered
TEST_F(ReporterListTest, HandlesEmptyBenchmarkList) {
    benchmark::ConsoleReporter console_reporter;
    benchmark::JSONReporter json_reporter;
    benchmark::CSVReporter csv_reporter;

    console_reporter.SetOutputStream(&ss);
    console_reporter.SetErrorStream(&ss);

    json_reporter.SetOutputStream(&ss);
    json_reporter.SetErrorStream(&ss);

    csv_reporter.SetOutputStream(&ss);
    csv_reporter.SetErrorStream(&ss);

    // Expect no output as no benchmarks are registered
    RunList(console_reporter);
    EXPECT_TRUE(ss.str().empty());

    ss.str("");
    RunList(json_reporter);
    EXPECT_TRUE(ss.str().empty());

    ss.str("");
    RunList(csv_reporter);
    EXPECT_TRUE(ss.str().empty());
}

// Tests the output of reporters when benchmarks are listed
TEST_F(ReporterListTest, ListsBenchmarksWithDifferentReporters) {
    benchmark::ConsoleReporter console_reporter;
    benchmark::JSONReporter json_reporter;
    benchmark::CSVReporter csv_reporter;

    console_reporter.SetOutputStream(&ss);
    console_reporter.SetErrorStream(&ss);

    json_reporter.SetOutputStream(&ss);
    json_reporter.SetErrorStream(&ss);

    csv_reporter.SetOutputStream(&ss);
    csv_reporter.SetErrorStream(&ss);

    RegisterBenchmarks(BENCHMARK_FAMILY_ID); // Ensure some benchmarks are registered

    RunList(console_reporter);
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_simple"));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_complex"));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_special!@#"));

    ss.str("");
    RunList(json_reporter);
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("\"name\": \"BM_simple\""));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("\"name\": \"BM_complex\""));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("\"name\": \"BM_special!@#\""));

    // Check JSON structure
    std::string json_output = ss.str();
    EXPECT_THAT(json_output.front(), testing::Eq('['));
    EXPECT_THAT(json_output.back(), testing::Eq(']'));

    ss.str("");
    RunList(csv_reporter);
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_simple"));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_complex"));
    EXPECT_THAT(ss.str(), ::testing::HasSubstr("BM_special!@#"));
}

}  // namespace internal
}  // namespace benchmark
