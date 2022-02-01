#include <cstdint>

#include "benchmark/benchmark.h"

#include "monolithic_examples.h"


namespace {
#if defined(__GNUC__)
std::uint64_t double_up(const std::uint64_t x) __attribute__((const));
#endif
std::uint64_t double_up(const std::uint64_t x) { return x * 2; }
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

  char buffer8[8] = "";
  benchmark::DoNotOptimize(buffer8);

  char buffer20[20] = "";
  benchmark::DoNotOptimize(buffer20);

  char buffer1024[1024] = "";
  benchmark::DoNotOptimize(buffer1024);
  benchmark::DoNotOptimize(&buffer1024[0]);

  int x = 123;
  benchmark::DoNotOptimize(x);
  benchmark::DoNotOptimize(&x);
  benchmark::DoNotOptimize(x += 42);

  benchmark::DoNotOptimize(double_up(x));

  // These tests are to e
  benchmark::DoNotOptimize(BitRef::Make());
  BitRef lval = BitRef::Make();
  benchmark::DoNotOptimize(lval);
  return 0;
}
