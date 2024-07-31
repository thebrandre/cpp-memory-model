#pragma once
#include <atomic>

// The assembly generated for the two versions is basically the same:
// https://godbolt.org/z/zPTe3cW8G

namespace v1 {
class StupidSelfishSpinMutex {
  std::atomic_flag Locked{};

public:
  void lock() {
    while (Locked.test_and_set(std::memory_order_acquire))
      ;
  }
  void unlock() { Locked.clear(std::memory_order_release); }
};

} // namespace v1

namespace v2 {
class StupidSelfishSpinMutex {
  std::atomic<bool> Locked{};

public:
  void lock() {
    while (Locked.exchange(true, std::memory_order_acquire))
      ;
  }
  void unlock() { Locked.store(false, std::memory_order_release); }
};
} // namespace v2

using StupidSelfishSpinMutex = v2::StupidSelfishSpinMutex;