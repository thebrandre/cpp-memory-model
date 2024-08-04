#include "MutexConcept.h"
#include "StupidSelfishMutex.h"

#include <catch2/catch_test_macros.hpp>
#include <mutex>
#include <shared_mutex>
#include <tbb/spin_mutex.h>

namespace {
class NotAMutex {
public:
  NotAMutex() = default;
  NotAMutex(const NotAMutex &) = delete;
  NotAMutex(NotAMutex &&) noexcept = delete;
  NotAMutex &operator=(const NotAMutex &) = delete;
  NotAMutex &operator=(NotAMutex &&) noexcept = delete;

  void lock();
  void unlock();
  NotAMutex &try_lock();
};
}


TEST_CASE("MutexConceptChecks") {
  STATIC_REQUIRE(MutexConcept<std::mutex>);
  STATIC_REQUIRE(MutexConcept<std::shared_mutex>);
  STATIC_REQUIRE(MutexConcept<tbb::spin_mutex>);
  STATIC_REQUIRE(MutexConcept<v1::StupidSelfishSpinMutex>);
  STATIC_REQUIRE(MutexConcept<v2::StupidSelfishSpinMutex>);
  STATIC_REQUIRE(MutexConcept<v3::StupidSelfishSpinMutex>);

  STATIC_REQUIRE(!MutexConcept<int>);
  STATIC_REQUIRE(!MutexConcept<NotAMutex>);
}