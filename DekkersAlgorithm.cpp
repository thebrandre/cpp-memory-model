#include "DekkersAlgorithm.h"

void DekkersAlgorithm::lock(int Slot) {
  WantsToEnter[Slot] = true;

  const int Other = 1 - Slot;
  while (WantsToEnter[Other]) {
    if (Turn != Slot) {
      WantsToEnter[Slot] = false;
      while (Turn != Slot) {
        // busy wait until it is our turn
      }
      WantsToEnter[Slot] = true;
    }
  }
}

void DekkersAlgorithm::unlock(int Slot) {
  const int Other = 1 - Slot;
  Turn = Other;
  WantsToEnter[Slot] = false;
}

/// @see "Anthony Williams: Implementing Dekker's algorithm with Fences"
void DekkersAlgorithmFences::lock(int Slot) {
  WantsToEnter[Slot].store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_seq_cst);

  const auto Other = 1 - Slot;
  while (WantsToEnter[Other].load(std::memory_order_relaxed)) {
    if (Turn.load(std::memory_order_relaxed) != Slot) {
      WantsToEnter[Slot].store(false, std::memory_order_relaxed);
      while (Turn.load(std::memory_order_relaxed) != Slot) {
      }
      WantsToEnter[Slot].store(true, std::memory_order_relaxed);
      std::atomic_thread_fence(std::memory_order_seq_cst);
    }
  }
  std::atomic_thread_fence(std::memory_order_acquire);
}

void DekkersAlgorithmFences::unlock(int Slot) {
  const auto Other = 1 - Slot;
  Turn.store(Other, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  WantsToEnter[Slot].store(false, std::memory_order_relaxed);
}