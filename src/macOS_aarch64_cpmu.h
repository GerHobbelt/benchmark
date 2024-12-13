// WARNING: must be run as root on an M1 device
// WARNING: fragile, uses private apple APIs

/*
  Based on https://github.com/travisdowns/robsize
  Henry Wong <henry@stuffedcow.net>
  http://blog.stuffedcow.net/2013/05/measuring-rob-capacity/
  2014-10-14
*/

#ifndef MACOS_AARCH64_CPMU_H_
#define MACOS_AARCH64_CPMU_H_

#include "internal_macros.h"

#ifdef BENCHMARK_MACOS_AARCH64

namespace benchmark {
namespace internal {

/*
  Initialise and configure the rdtsc counter modules.
  Return false if it failed to initialise or failed to configure,
  otherwise return true.
 */
bool init_macOS_rdtsc();

/*
  Return the counter value.
  Return 0 if it is failed to get the counter value.
 */
unsigned long long int macOS_rdtsc();

/*
  Return true if PMU was configured, otherwise false.
 */
bool macOS_is_PMU_ON();

}  // namespace internal
}  // namespace benchmark
#endif  // BENCHMARK_MACOS_AARCH64

#endif  // MACOS_AARCH64_CPMU_H_
