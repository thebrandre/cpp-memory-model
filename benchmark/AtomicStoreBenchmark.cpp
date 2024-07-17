#include "MemoryFence.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>
#include <ranges>

static constexpr int ItemRangeBegin = 1 << 8;
static constexpr int ItemRangEnd = 1 << 12;

static void BM_AtomicStore(benchmark::State &State, std::memory_order Order) {
  const int NumItems = static_cast<int>(State.range(0));
  std::atomic<int> AtomicVariable{};

  for (auto _ : State) {
    for (const auto i : std::views::iota(0, NumItems))
      AtomicVariable.store(i, Order);
  }

  State.counters["duration_per_operation"] =
      benchmark::Counter(static_cast<double>(State.iterations()) * State.range(0),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
  State.SetComplexityN(State.range(0));
}

BENCHMARK_CAPTURE(BM_AtomicStore, std::memory_order_seq_cst, std::memory_order_seq_cst)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicStore, std::memory_order_release, std::memory_order_release)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

static void BM_AtomicStoreFenced(benchmark::State &State) {
  const int NumItems = static_cast<int>(State.range(0));
  std::atomic<int> AtomicVariable{};

  for (auto _ : State) {
    for (const auto i : std::views::iota(0, NumItems)) {
      AtomicVariable.store(i, std::memory_order_relaxed);
      MemoryFence();
    }
  }

  State.SetComplexityN(State.range(0));
  State.counters["duration_per_operation"] =
      benchmark::Counter(static_cast<double>(State.iterations()) * State.range(0),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
}

BENCHMARK(BM_AtomicStoreFenced)->RangeMultiplier(2)->Range(ItemRangeBegin, ItemRangEnd)->Complexity(benchmark::o1);