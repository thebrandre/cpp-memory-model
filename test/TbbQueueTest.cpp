#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <tbb/combinable.h>
#include <tbb/concurrent_queue.h>

#include <algorithm>
#include <barrier>
#include <latch>
#include <list>
#include <ranges>
#include <thread>

TEST_CASE("concurrent queue test", "[tbb]") {
  tbb::concurrent_bounded_queue<int> Values;
  Values.set_capacity(5);

  tbb::combinable<int> TotalSum{0};
  std::atomic<int> Done{};
  static constexpr int NumProducers = 2;
  static constexpr int NumConsumers = 4;
  std::latch Latch(NumProducers + NumConsumers);

  const auto Consumer = [&]() {
    Latch.arrive_and_wait();
    while (true) {
      if (int Value{}; Values.try_pop(Value)) {
        TotalSum.local() += Value;
      } else if (Done.load(std::memory_order_acquire) == NumProducers) {
        break;
      }
    }
  };

  const int N = GENERATE(23, 42, 1000);
  const auto Producer = [&]() {
    Latch.arrive_and_wait();
    for (int Value : std::views::iota(0, N + 1)) {
      while (!Values.try_push(Value))
        ;
    }
    Done.fetch_add(1, std::memory_order_release);
  };

  {
    std::list<std::jthread> ProducerThreads;
    for ([[maybe_unused]] const auto _ : std::views::iota(0, NumProducers))
      ProducerThreads.emplace_back(Producer);

    std::list<std::jthread> ConsumerThreads;
    for ([[maybe_unused]] const auto _ : std::views::iota(0, NumConsumers))
      ConsumerThreads.emplace_back(Consumer);
  }

  REQUIRE(TotalSum.combine(std::plus<int>{}) == N * (N + 1) / 2 * NumProducers);
}