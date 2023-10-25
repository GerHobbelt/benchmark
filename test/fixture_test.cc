
#include <cassert>
#include <memory>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"


namespace gbench_fxture_test {

#define FIXTURE_BECHMARK_NAME MyFixture

class FIXTURE_BECHMARK_NAME : public ::benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State& state) override {
    if (state.thread_index() == 0) {
      assert(data.get() == nullptr);
      data.reset(new int(42));
    }
  }

  void TearDown(const ::benchmark::State& state) override {
    if (state.thread_index() == 0) {
      assert(data.get() != nullptr);
      data.reset();
    }
  }

  ~FIXTURE_BECHMARK_NAME() override { assert(data == nullptr); }

  std::unique_ptr<int> data;
};

BENCHMARK_F(FIXTURE_BECHMARK_NAME, Foo)(benchmark::State& st) {
  assert(data.get() != nullptr);
  assert(*data == 42);
  for (auto _ : st) {
  }
}

BENCHMARK_DEFINE_F(FIXTURE_BECHMARK_NAME, Bar)(benchmark::State& st) {
  if (st.thread_index() == 0) {
    assert(data.get() != nullptr);
    assert(*data == 42);
  }
  for (auto _ : st) {
    assert(data.get() != nullptr);
    assert(*data == 42);
  }
  st.SetItemsProcessed(st.range(0));
}
BENCHMARK_REGISTER_F(FIXTURE_BECHMARK_NAME, Bar)->Arg(42);
BENCHMARK_REGISTER_F(FIXTURE_BECHMARK_NAME, Bar)->Arg(42)->ThreadPerCpu();

}


#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_future_test_main(cnt, arr)
#endif

BENCHMARK_MAIN();
