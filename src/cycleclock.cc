// Copyright 2016 Kris Kwiatkowski. All rights reserved.
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

#include "cycleclock.h"

#include "internal_macros.h"

#if defined(BENCHMARK_MACOS_AARCH64)
#include "macOS_aarch64_cpmu.h"
#endif

namespace benchmark {
namespace internal {
void InitializeCyclecount() {
#ifdef BENCHMARK_MACOS_AARCH64
  (void)::benchmark::internal::init_macOS_rdtsc();
#endif
}
}  // namespace internal

namespace cycleclock {
bool IsCycleClockEnabled() {
#if BENCHMARK_MACOS_AARCH64
  return ::benchmark::internal::macOS_is_PMU_ON();
#elif defined(__aarch64__) || (defined(__ARM_ARCH) && (__ARM_ARCH >= 6))
// Check ARM PMU.
// NOTE: only i386 and x86_64 have been well tested.
// PPC, sparc, alpha, and ia64 are based on
//    http://peter.kuscsik.com/wordpress/?p=14
// with modifications by m3b.  See also
//    https://setisvn.ssl.berkeley.edu/svn/lib/fftw-3.0.1/kernel/cycle.h
#if defined(__aarch64__)
  uint64_t pmuseren;
  asm volatile("MRS %0, pmuserenr_el0" : "=r"(pmuseren));
  return (1 == (pmuseren & 1));
#else
  uint32_t pmuseren;
  asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  return (1 == (pmuseren & 1));
#endif
#endif
  // By default we return true and allow caller to perform further checks.
  return true;
}

}  // namespace cycleclock
}  // namespace benchmark
