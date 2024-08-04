#pragma once
#include <concepts>

template <typename MutexType>
concept MutexConcept = !std::copyable<MutexType> && !std::movable<MutexType> && requires(MutexType &Mutex) {
  Mutex.lock();
  Mutex.unlock();
  { Mutex.try_lock() } -> std::convertible_to<bool>;
};