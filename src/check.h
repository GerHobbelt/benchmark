#ifndef CHECK_H_
#define CHECK_H_

#include <cmath>
#include <cstdlib>
#include <ostream>
#include <string_view>

#include "gbenchmark/export.h"
#include "internal_macros.h"
#include "log.h"

#if defined(__GNUC__) || defined(__clang__)
#define BENCHMARK_NOEXCEPT noexcept
#define BENCHMARK_NOEXCEPT_OP(x) noexcept(x)
#elif defined(_MSC_VER) && !defined(__clang__)
#if _MSC_VER >= 1900
#define BENCHMARK_NOEXCEPT noexcept
#define BENCHMARK_NOEXCEPT_OP(x) noexcept(x)
#else
#define BENCHMARK_NOEXCEPT
#define BENCHMARK_NOEXCEPT_OP(x)
#endif
#define __func__ __FUNCTION__
#else
#define BENCHMARK_NOEXCEPT
#define BENCHMARK_NOEXCEPT_OP(x)
#endif

#if !defined(BUILDING_LIBGBENCHMARK) && !defined(BENCHMARK_STATIC_DEFINE)
#if defined(_WIN32) && !defined(BUILDING_LIBGBENCHMARK)
#define WEAK_ATTR __declspec(selectany)
#else
#define WEAK_ATTR __attribute__((weak))
#endif
#else
#define WEAK_ATTR 
#endif

namespace benchmark {
namespace internal {

typedef void(AbortHandlerT)();

BENCHMARK_EXPORT WEAK_ATTR
AbortHandlerT*& GetAbortHandler();

BENCHMARK_NORETURN inline void CallAbortHandler() {
  GetAbortHandler()();
  std::flush(std::cout);
  std::flush(std::cerr);
  std::abort();  // fallback to enforce noreturn
}

// CheckHandler is the class constructed by failing BM_CHECK macros.
// CheckHandler will log information about the failures and abort when it is
// destructed.
class CheckHandler {
 public:
  CheckHandler(std::string_view check, std::string_view file,
               std::string_view func, int line)
      : log_(GetErrorLogInstance()) {
    log_ << file << ":" << line << ": " << func << ": Check `" << check
         << "' failed. ";
  }

  LogType& GetLog() { return log_; }

#if defined(COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable : 4722)
#endif
  BENCHMARK_NORETURN ~CheckHandler() BENCHMARK_NOEXCEPT_OP(false) {
    log_ << '\n';
    CallAbortHandler();
  }
#if defined(COMPILER_MSVC)
#pragma warning(pop)
#endif

  CheckHandler& operator=(const CheckHandler&) = delete;
  CheckHandler(const CheckHandler&) = delete;
  CheckHandler() = delete;

 private:
  LogType& log_;
};

}  // end namespace internal
}  // end namespace benchmark

// The BM_CHECK macro returns a std::ostream object that can have extra
// information written to it.
#ifndef NDEBUG
#define BM_CHECK(b)                                          \
  (b ? ::benchmark::internal::GetNullLogInstance()           \
     : ::benchmark::internal::CheckHandler(                  \
           std::string_view(#b), std::string_view(__FILE__), \
           std::string_view(__func__), __LINE__)             \
           .GetLog())
#else
#define BM_CHECK(b) ::benchmark::internal::GetNullLogInstance()
#endif

// clang-format off
// preserve whitespacing between operators for alignment
#define BM_CHECK_EQ(a, b) BM_CHECK((a) == (b))
#define BM_CHECK_NE(a, b) BM_CHECK((a) != (b))
#define BM_CHECK_GE(a, b) BM_CHECK((a) >= (b))
#define BM_CHECK_LE(a, b) BM_CHECK((a) <= (b))
#define BM_CHECK_GT(a, b) BM_CHECK((a) > (b))
#define BM_CHECK_LT(a, b) BM_CHECK((a) < (b))

#define BM_CHECK_FLOAT_EQ(a, b, eps) BM_CHECK(std::fabs((a) - (b)) <  (eps))
#define BM_CHECK_FLOAT_NE(a, b, eps) BM_CHECK(std::fabs((a) - (b)) >= (eps))
#define BM_CHECK_FLOAT_GE(a, b, eps) BM_CHECK((a) - (b) > -(eps))
#define BM_CHECK_FLOAT_LE(a, b, eps) BM_CHECK((b) - (a) > -(eps))
#define BM_CHECK_FLOAT_GT(a, b, eps) BM_CHECK((a) - (b) >  (eps))
#define BM_CHECK_FLOAT_LT(a, b, eps) BM_CHECK((b) - (a) >  (eps))
//clang-format on

#endif  // CHECK_H_
