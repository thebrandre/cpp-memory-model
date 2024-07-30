#include "MemoryFence.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>
#include <ranges>
#include <stop_token>
#include <thread>

static constexpr int ItemRangeBegin = 1 << 8;
static constexpr int ItemRangEnd = 1 << 12;

// ReSharper disable once CppPassValueParameterByConstReference
static void StoreSpammer(std::stop_token StopToken, std::atomic<std::int64_t> *Variable, std::int64_t *Counter,
                         const std::memory_order Order) {
  auto &i = *Counter;
  i = 0;
  while (!StopToken.stop_requested())
    Variable->store(++i, Order);
}

// ReSharper disable once CppPassValueParameterByConstReference
static void StoreSpammerFence(std::stop_token StopToken, [[maybe_unused]] std::atomic<std::int64_t> *Variable,
                              std::int64_t *Counter, [[maybe_unused]] const std::memory_order Order) {
  auto &i = *Counter;
  i = 0;
  while (!StopToken.stop_requested()) {
    Variable->store(++i, std::memory_order_relaxed);
    MemoryFence();
  }
}
static void StoreSpammerSomewhereElse(std::stop_token StopToken, [[maybe_unused]] std::atomic<std::int64_t> *Variable,
                                      std::int64_t *Counter, const std::memory_order Order) {
  auto MyOwnAtomicVariable = std::make_unique<std::atomic<std::int64_t>>();
  StoreSpammer(std::move(StopToken), MyOwnAtomicVariable.get(), Counter, Order);
}

static void StoreSpammerFenceSomewhereElse(std::stop_token StopToken,
                                           [[maybe_unused]] std::atomic<std::int64_t> *Variable, std::int64_t *Counter,
                                           const std::memory_order Order) {
  auto MyOwnAtomicVariable = std::make_unique<std::atomic<std::int64_t>>();
  StoreSpammerFence(std::move(StopToken), MyOwnAtomicVariable.get(), Counter, Order);
}

template <typename SpammerType>
static void BM_AtomicLoad(benchmark::State &State, const std::memory_order Order, SpammerType SpammerCallback) {
  const int NumItems = static_cast<int>(State.range(0));
  auto AtomicVariable = std::make_unique<std::atomic<std::int64_t>>();
  const auto WriteMemoryOrder = Order == std::memory_order_acquire ? std::memory_order_release : Order;
  std::int64_t WriteCounter{};
  std::jthread Spammer(SpammerCallback, AtomicVariable.get(), &WriteCounter, WriteMemoryOrder);

  for (auto _ : State) {
    std::int64_t Sum = 0;
    for ([[maybe_unused]] const auto i : std::views::iota(0, NumItems))
      Sum += AtomicVariable->load(Order);
    benchmark::DoNotOptimize(Sum);
  }
  Spammer.request_stop();
  State.counters["duration_per_operation"] = benchmark::Counter(
      static_cast<double>(State.range(0)), benchmark::Counter::kIsIterationInvariantRate | benchmark::Counter::kInvert);
  State.counters["duration_per_write"] =
      benchmark::Counter(static_cast<double>(WriteCounter), benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
  State.SetComplexityN(State.range(0));
}

BENCHMARK_CAPTURE(BM_AtomicLoad, sequential_consistency, std::memory_order_seq_cst, &StoreSpammer)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, acquire, std::memory_order_acquire, &StoreSpammer)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, mfence, std::memory_order_seq_cst, &StoreSpammerFence)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, sequential_consistency_separate_objects, std::memory_order_seq_cst,
                  &StoreSpammerSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, acquire_separate_objects, std::memory_order_acquire, &StoreSpammerSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, mfence_separate_objects, std::memory_order_seq_cst, &StoreSpammerFenceSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);
