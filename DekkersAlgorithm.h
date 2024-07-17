#pragma once
#include <atomic>

class DekkersAlgorithm {
public:
  void lock(int Slot);
  void unlock(int Slot);

private:
  std::atomic<bool> WantsToEnter[2]{};
  std::atomic<int> Turn{};
};

class DekkersAlgorithmFences {
public:
  void lock(int Slot);
  void unlock(int Slot);

private:
  std::atomic<bool> WantsToEnter[2]{};
  std::atomic<int> Turn{};
};