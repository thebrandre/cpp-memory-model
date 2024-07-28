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
static void StoreSpammer(std::stop_token StopToken, std::atomic<int> *Variable, const std::memory_order Order) {
  int i = 0;
  while (!StopToken.stop_requested())
    Variable->store(++i, Order);
}

// ReSharper disable once CppPassValueParameterByConstReference
static void StoreSpammerFence(std::stop_token StopToken, [[maybe_unused]] std::atomic<int> *Variable,
                              [[maybe_unused]] const std::memory_order Order) {
  int i = 0;
  while (!StopToken.stop_requested()) {
    Variable->store(++i, std::memory_order_relaxed);
    MemoryFence();
  }
}
static void StoreSpammerSomewhereElse(std::stop_token StopToken, [[maybe_unused]] std::atomic<int> *Variable,
                                      const std::memory_order Order) {
  auto MyOwnAtomicVariable = std::make_unique<std::atomic<int>>();
  StoreSpammer(std::move(StopToken), MyOwnAtomicVariable.get(), Order);
}

static void StoreSpammerFenceSomewhereElse(std::stop_token StopToken, [[maybe_unused]] std::atomic<int> *Variable,
                                           const std::memory_order Order) {
  auto MyOwnAtomicVariable = std::make_unique<std::atomic<int>>();
  StoreSpammerFence(std::move(StopToken), MyOwnAtomicVariable.get(), Order);
}

template <typename SpammerType>
static void BM_AtomicLoad(benchmark::State &State, const std::memory_order Order, SpammerType SpammerCallback) {
  const int NumItems = static_cast<int>(State.range(0));
  auto AtomicVariable = std::make_unique<std::atomic<int>>();
  auto WriteMemoryOrder = Order == std::memory_order_acquire ? std::memory_order_release : Order;
  std::jthread Spammer(SpammerCallback, AtomicVariable.get(), WriteMemoryOrder);

  for (auto _ : State) {
    int Sum = 0;
    for ([[maybe_unused]] const auto i : std::views::iota(0, NumItems))
      Sum += AtomicVariable->load(Order);
    benchmark::DoNotOptimize(Sum);
  }
  Spammer.request_stop();
  State.counters["duration_per_operation"] =
      benchmark::Counter(static_cast<double>(State.iterations()) * State.range(0),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
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

BENCHMARK_CAPTURE(BM_AtomicLoad, sequential_consistency_without_cache_invalidation, std::memory_order_seq_cst,
                  &StoreSpammerSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, acquire_without_cache_invalidation, std::memory_order_acquire,
                  &StoreSpammerSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicLoad, mfence_without_cache_invalidation, std::memory_order_seq_cst,
                  &StoreSpammerFenceSomewhereElse)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);
