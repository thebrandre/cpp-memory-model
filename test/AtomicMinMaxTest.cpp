#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <atomic>
#include <functional>
#include <latch>
#include <barrier>
#include <print>
#include <ranges>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include "AtomicMinMax.h"

TEST_CASE("simple sequential examples of atomic_fetch_max") {
  std::atomic<int> x = 5;
  REQUIRE(::atomic_fetch_max(&x, 10) == 5);
  REQUIRE(x.load() == 10);
  REQUIRE(::atomic_fetch_max(&x, 10) == 10);
  REQUIRE(x.load() == 10);
}


