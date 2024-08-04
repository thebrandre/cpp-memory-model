#pragma once
#include <atomic>

// The assembly generated for the two versions is basically the same:
// https://godbolt.org/z/zPTe3cW8G

class StupidSelfishSpinMutexBase {
public:
  template <typename Derived> void lock(this Derived &&Self) noexcept {
    while (!Self.try_lock())
      ;
  }
};

namespace v1 {
class StupidSelfishSpinMutex : public StupidSelfishSpinMutexBase {
  std::atomic_flag Locked{};

public:
  bool try_lock() noexcept { return !Locked.test_and_set(std::memory_order_acquire); }
  void unlock() noexcept { Locked.clear(std::memory_order_release); }
};

} // namespace v1

namespace v2 {
class StupidSelfishSpinMutex : public StupidSelfishSpinMutexBase {
  std::atomic<bool> Locked{};

public:
  bool try_lock() noexcept { return !Locked.exchange(true, std::memory_order_acquire); }
  void unlock() noexcept { Locked.store(false, std::memory_order_release); }
};
} // namespace v2

namespace v3 {
class StupidSelfishSpinMutex : public StupidSelfishSpinMutexBase {
  std::atomic<bool> Locked{};

public:
  bool try_lock() noexcept {
    bool Expected = false;
    return Locked.compare_exchange_weak(Expected, true, std::memory_order_acquire, std::memory_order_relaxed);
  }
  void unlock() noexcept { Locked.store(false, std::memory_order_release); }
};
} // namespace v3

using StupidSelfishSpinMutex = v3::StupidSelfishSpinMutex;
