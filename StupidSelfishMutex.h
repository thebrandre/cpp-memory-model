#pragma once
#include <atomic>

// The assembly generated for the two versions is basically the same:
// https://godbolt.org/z/zPTe3cW8G

namespace v1 {
class StupidSelfishSpinMutex {
  std::atomic_flag Locked{};

public:
  void lock() noexcept {
    while (Locked.test_and_set(std::memory_order_acquire))
      ;
  }
  void unlock() noexcept { Locked.clear(std::memory_order_release); }
};

} // namespace v1

namespace v2 {
class StupidSelfishSpinMutex {
  std::atomic<bool> Locked{};

public:
  void lock() noexcept {
    while (Locked.exchange(true, std::memory_order_acquire))
      ;
  }
  void unlock() noexcept { Locked.store(false, std::memory_order_release); }
};
} // namespace v2

namespace v3 {
class StupidSelfishSpinMutex {
  std::atomic<bool> Locked{};

public:
  bool try_lock() noexcept {
    bool Expected = false;
    return Locked.compare_exchange_weak(Expected, true, std::memory_order_acquire, std::memory_order_relaxed);
  }

  void lock() noexcept {
    while (!try_lock())
      ;
  }
  void unlock() noexcept { Locked.store(false, std::memory_order_release); }
};
} // namespace v3

using StupidSelfishSpinMutex = v3::StupidSelfishSpinMutex;