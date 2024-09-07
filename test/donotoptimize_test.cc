#include <cstdint>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"


namespace {
#if defined(__GNUC__)
std::int64_t double_up(const std::int64_t x) __attribute__((const));
#endif
std::int64_t double_up(const std::int64_t x) { return x * 2; }
}  // namespace

// Using DoNotOptimize on types like BitRef seem to cause a lot of problems
// with the inline assembly on both GCC and Clang.
struct BitRef {
  int index;
  unsigned char& byte;

 public:
  static BitRef Make() {
    static unsigned char arr[2] = {};
    BitRef b(1, arr[0]);
    return b;
  }

 private:
  BitRef(int i, unsigned char& b) : index(i), byte(b) {}
};



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      gbenchmark_do_not_optimize_test_main(cnt, arr)
#endif

int main(int argc, const char** argv) {
  // this test verifies compilation of DoNotOptimize() for some types

  char buffer1[1] = "";
  benchmark::DoNotOptimize(buffer1);

  char buffer2[2] = "";
  benchmark::DoNotOptimize(buffer2);

  char buffer3[3] = "";
  benchmark::DoNotOptimize(buffer3);

  char buffer8[8] = "";
  benchmark::DoNotOptimize(buffer8);

  char buffer20[20] = "";
  benchmark::DoNotOptimize(buffer20);

  char buffer1024[1024] = "";
  benchmark::DoNotOptimize(buffer1024);
  char* bptr = &buffer1024[0];
  benchmark::DoNotOptimize(bptr);

  int x = 123;
  benchmark::DoNotOptimize(x);
  int* xp = &x;
  benchmark::DoNotOptimize(xp);
  benchmark::DoNotOptimize(x += 42);

  std::int64_t y = double_up(x);
  benchmark::DoNotOptimize(y);

  // These tests are to e
  BitRef lval = BitRef::Make();
  benchmark::DoNotOptimize(lval);

#ifdef BENCHMARK_HAS_CXX11
  // Check that accept rvalue.
  benchmark::DoNotOptimize(BitRef::Make());
#endif
  return 0;
}
