
#include <cassert>
#include <memory>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"

#define BENCHMARK_FAMILY_ID "templated_fixture_method"

template <typename T>
class MyFixture : public ::benchmark::Fixture {
 public:
  MyFixture() : data(0) {}

  T data;

  using type = T;
};

BENCHMARK_TEMPLATE_METHOD_F(MyFixture, Foo)(benchmark::State& st) {
  for (auto _ : st) {
    this->data += typename Base::type(1);
  }
}

BENCHMARK_TEMPLATE_INSTANTIATE_F(MyFixture, Foo, int);
BENCHMARK_TEMPLATE_INSTANTIATE_F(MyFixture, Foo, double);

#if defined(BUILD_MONOLITHIC)
#define main gbenchmark_templated_fixture_method_test_main
#endif

BENCHMARK_MAIN();
