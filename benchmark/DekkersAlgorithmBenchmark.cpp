// To compare the implementations using a perf tool such as VTune or Performance Counters for Linux, PCL (perf)<
// run the benchmarks separately via:
// --benchmark_filter="BM_DekkersAlgorithm/DekkersAlgorithm/threads:2" --benchmark_min_time=10s
// --benchmark_filter="BM_DekkersAlgorithm/DekkersAlgorithmFences/threads:2" --benchmark_min_time=10s

#include "DekkersAlgorithm.h"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <numeric>
#include <ranges>

template <typename Algorithm> static void BM_DekkersAlgorithm(benchmark::State &State, Algorithm &Mutex) {
  for ([[maybe_unused]] auto _ : State) {
    Mutex.lock(State.thread_index());
    Mutex.unlock(State.thread_index());
  }
}

static DekkersAlgorithm DekkersMutex;
static DekkersAlgorithmFences DekkersMutexFences;

BENCHMARK_CAPTURE(BM_DekkersAlgorithm, DekkersAlgorithm, DekkersMutex)->ThreadRange(1, 2);
BENCHMARK_CAPTURE(BM_DekkersAlgorithm, DekkersAlgorithmFences, DekkersMutexFences)->ThreadRange(1, 2);

#include "StupidSelfishMutex.h"
#include <mutex>
#include <shared_mutex>
#include <tbb/spin_mutex.h>

template <typename MutexType> static void BM_Mutex(benchmark::State &State, MutexType &Mutex) {
  for ([[maybe_unused]] auto _ : State)
    std::scoped_lock Lock(Mutex);
}

static std::shared_mutex SharedMutex;
static std::mutex RegularMutex;
static tbb::spin_mutex TbbSpinMutex;
static StupidSelfishSpinMutex SimpleMutex;

BENCHMARK_CAPTURE(BM_Mutex, std::shared_mutex, SharedMutex)->ThreadRange(1, 32);
BENCHMARK_CAPTURE(BM_Mutex, std::mutex, RegularMutex)->ThreadRange(1, 32);
BENCHMARK_CAPTURE(BM_Mutex, tbb::spin_mutex, TbbSpinMutex)->ThreadRange(1, 32);
BENCHMARK_CAPTURE(BM_Mutex, StupidSelfishSpinMutex, SimpleMutex)->ThreadRange(1, 32);