#pragma once
#include <algorithm>
#include <atomic>

#ifndef __cpp_lib_atomic_min_max

/// @note This implementation uses weaker atomics compared to a reference implementation
/// from the proposal P0493. It compiles to the same instructions on x86.
/// I have yet to test it on ARM to see if it actually works. ;-)
/// @see https://github.com/Bronek/wg21-p0493/blob/rev5/fetch_max.hpp#L19-L38
/// @see https://godbolt.org/z/r8TMzjea5
template <typename T>
auto atomic_fetch_max(std::atomic<T> *AtomicOperand, T Argument,
                      std::memory_order Order = std::memory_order_seq_cst) noexcept {
  auto Expected = AtomicOperand->load(std::memory_order_relaxed);
  while (!AtomicOperand->compare_exchange_weak(Expected, std::ranges::max(Argument, Expected), Order,
                                               std::memory_order_relaxed))
    ;
  return Expected;
}
#endif