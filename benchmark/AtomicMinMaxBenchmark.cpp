#include "AtomicMinMax.h"
#include <benchmark/benchmark.h>

#include <algorithm>
#include <numeric>
#include <ranges>

static constexpr int ItemRangeBegin = 1 << 8;
static constexpr int ItemRangEnd = 1 << 12;

// https://godbolt.org/z/Ec45sxj8G
// https://godbolt.org/z/xPTh7Tcns
// difference only on ARM https://godbolt.org/z/EYcGx6zdx

static void BM_AtomicMinMax(benchmark::State &State, std::memory_order Order) {
  const int NumItems = static_cast<int>(State.range(0));
  std::atomic<int> AtomicVariable{};

  for (auto _ : State) {
    for (const auto i : std::views::iota(0, NumItems))
      atomic_fetch_max(&AtomicVariable, i, Order);
    benchmark::DoNotOptimize(AtomicVariable);
  }

  if (AtomicVariable.load(std::memory_order_relaxed) != NumItems - 1)
    std::abort();

  State.counters["duration_per_operation"] =
      benchmark::Counter(static_cast<double>(State.iterations()) * State.range(0),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
  State.SetComplexityN(State.range(0));
}

BENCHMARK_CAPTURE(BM_AtomicMinMax, memory_order_seq_cst, std::memory_order_seq_cst)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

BENCHMARK_CAPTURE(BM_AtomicMinMax, memory_order_relaxed, std::memory_order_relaxed)
    ->RangeMultiplier(2)
    ->Range(ItemRangeBegin, ItemRangEnd)
    ->Complexity(benchmark::o1);

static void BM_RegularMinMax(benchmark::State &State) {
  const int NumItems = static_cast<int>(State.range(0));
  int AutomaticVariable = -1;
  for (auto _ : State) {
    for (const auto i : std::views::iota(0, NumItems))
      AutomaticVariable = std::max(AutomaticVariable, i);
    benchmark::DoNotOptimize(AutomaticVariable);
  }
  if (AutomaticVariable != NumItems - 1)
    std::abort();

  State.counters["duration_per_operation"] =
      benchmark::Counter(static_cast<double>(State.iterations()) * State.range(0),
                         benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
  State.SetComplexityN(State.range(0));
}

BENCHMARK(BM_RegularMinMax)->RangeMultiplier(2)->Range(ItemRangeBegin, ItemRangEnd)->Complexity(benchmark::o1);
