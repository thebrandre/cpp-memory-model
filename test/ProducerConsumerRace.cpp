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

TEST_CASE("single producer single consumer") {

  std::vector<int> Values;
  std::atomic<std::uint64_t> Size;

  const auto N = GENERATE(23, 42, 100);
  Values.resize(N);
  std::latch Latch(2);
  int TotalSum{};

  const auto Consumer = [&] {
    Latch.arrive_and_wait();
    for (std::uint64_t i = 0; i < Values.size();) {
      if (i >= Size.load(std::memory_order_acquire))
        continue;
      TotalSum += Values[i];
      ++i;
    }
  };

  const auto Producer = [&] {
    Latch.arrive_and_wait();
    for (auto i : std::views::iota(0, N)) {
      if (i % 7 == 0)
        std::this_thread::yield();
      Values[i] = i + 1;
      Size.store(i + 1, std::memory_order_release);
    }
  };

  {
    std::jthread Thread1(Producer);
    Consumer();
  }
  REQUIRE(TotalSum == N * (N + 1) / 2);
}

TEST_CASE("multiple producer single consumer") {

  std::vector<int> Values;
  std::atomic<std::uint64_t> NextWriteIndex;
  std::atomic<std::uint64_t> Size;

  const auto N = GENERATE(23, 42, 100);
  Values.resize(N);
  const int NumProducers = 2;
  std::latch Latch(1 + NumProducers);
  int TotalSum{};

  const auto Consumer = [&] {
    Latch.arrive_and_wait();
    std::uint64_t i = 0; 
    while (i < Values.size()) {
      if (i < Size.load(std::memory_order_acquire)) {
        TotalSum += Values[i];
        ++i;
      }
    }
  };

  const auto Producer = [&](std::ranges::input_range auto &&Data) {
    Latch.arrive_and_wait();
    std::uint64_t WriteIndex{};
    for (auto i : Data) {
      WriteIndex = NextWriteIndex.fetch_add(1, std::memory_order_relaxed);
      if (WriteIndex >= Values.size())
        break;
      Values[WriteIndex] = i;

      // Publish results in order:
      auto Expected = WriteIndex;
      while (
          !Size.compare_exchange_weak(Expected, WriteIndex + 1, std::memory_order_release, std::memory_order_relaxed)) {
        std::this_thread::yield();
        Expected = WriteIndex;
      }
    }
  };

  {
    std::jthread Thread1(Producer, std::views::iota(1, N / 2 + 1));
    std::jthread Thread2(Producer, std::views::iota(N / 2 + 1, N + 1));
    Consumer();
  }
  REQUIRE(TotalSum == N * (N + 1) / 2);
}

TEST_CASE("single producer multiple consumers") {

  std::vector<int> Values;
  std::atomic<std::uint64_t> Size;

  const auto N = GENERATE(23, 42, 100);
  Values.resize(N);
  std::latch Latch(4);
  std::atomic<int> TotalSum{};
  std::atomic<std::uint64_t> ConsumeIndex;

  const auto Consumer = [&] {
    Latch.arrive_and_wait();
    int Result = 0;
    std::uint64_t i = 0; 
    while (i < Values.size()) {
      if (i < Size.load(std::memory_order_acquire) &&
          ConsumeIndex.compare_exchange_weak(i, i + 1, std::memory_order_relaxed, std::memory_order_relaxed))
        Result += Values[i];
    }
    TotalSum.fetch_add(Result, std::memory_order_relaxed);
  };

  const auto Producer = [&] {
    int Value = 0;
    Latch.arrive_and_wait();
    for (int i = 0; i < Values.size(); ++i) {
      if (i % 7 == 0)
        std::this_thread::yield();
      Values[i] = ++Value;
      Size.store(i + 1, std::memory_order_release);
    }
  };

  {
    std::jthread Thread1(Producer);
    std::jthread Thread2(Consumer);
    std::jthread Thread3(Consumer);
    std::jthread Thread4(Consumer);
  }
  REQUIRE(TotalSum.load() == N * (N + 1) / 2);
}