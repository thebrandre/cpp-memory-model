#pragma once
#include <atomic>

class StupidSelfishSpinMutex {
  std::atomic_flag Locked{};
  // std::atomic<bool> Locked{};
public:
  bool try_lock() {
    return Locked.test_and_set(std::memory_order_acquire);
    // return Locked.exchange(true, std::memory_order_acquire);
  }
  void lock() {
    while (try_lock())
      ;
  }
  void unlock() {
    Locked.clear(std::memory_order_release);
    // Locked.store(false, std::memory_order_release);
  }
};