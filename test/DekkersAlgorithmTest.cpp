#include "DekkersAlgorithm.h"
#include "StupidSelfishMutex.h"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <atomic>
#include <functional>
#include <latch>
#include <print>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

template <typename MutexType> struct MutexWrapper {
private:
  MutexType Mutex;

public:
  void lock([[maybe_unused]] int Slot) { Mutex.lock(); }
  void unlock([[maybe_unused]] int Slot) { Mutex.unlock(); }
};

TEMPLATE_TEST_CASE("Dekkers Algorithm", "[atomics]", DekkersAlgorithm, DekkersAlgorithmFences,
                   MutexWrapper<v1::StupidSelfishSpinMutex>, MutexWrapper<v2::StupidSelfishSpinMutex>) {
  const auto NumIncrements = GENERATE(100, 1000, 10000);

  const auto FibonacciIterationsMod97 = [](std::span<std::uint64_t, 2> x, int Count) {
    for (int i = 0; i < Count; ++i) {
      const auto Temp = (x[0] + x[1]) % 97;
      x[0] = x[1];
      x[1] = Temp;
    }
  };

  TestType Mutex;
  std::latch Latch(2);
  std::uint64_t SharedData[2]{1, 1};
  const auto IncrementLatched = [&](int Slot) {
    Latch.arrive_and_wait();

    for ([[maybe_unused]] const auto _ : std::views::iota(0, NumIncrements)) {
      Mutex.lock(Slot);
      FibonacciIterationsMod97(SharedData, 1);
      Mutex.unlock(Slot);
    }
  };

  {
    std::jthread Thread1(IncrementLatched, 0);
    std::jthread Thread2(IncrementLatched, 1);
  }

  std::uint64_t Reference[2]{1, 1};
  FibonacciIterationsMod97(Reference, 2 * NumIncrements);
  REQUIRE(Reference[1] == SharedData[1]);
}
