#pragma once
#include <atomic>
#include <memory>

#if defined(__cpp_lib_atomic_shared_ptr) && defined(__cpp_impl_three_way_comparison)

template <typename T> class ConcurrentStack {
  struct Node {
    T Element;
    std::shared_ptr<Node> Next;
  };
  std::atomic<std::shared_ptr<Node>> Head{};

  template<class ValueType>
  class BorrowedIterator {
    friend class ConcurrentStack;
    std::shared_ptr<Node> Data;
    explicit BorrowedIterator(std::shared_ptr<Node> Node) : Data{std::move(Node)} {}

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;

    value_type &operator*() const { return Data->Element; }
    value_type *operator->() const { return &Data->Element; }
    BorrowedIterator &operator++() {
      Data = Data->Next;
      return *this;
    }
    BorrowedIterator operator++(int) {
      auto Old = *this;
      ++*this;
      return Old;
    }
    explicit operator bool() const noexcept { return static_cast<bool>(Data); }

    friend bool operator==(const BorrowedIterator &Left, const BorrowedIterator &Right) noexcept {
      return Left.Data == Right.Data;
    }
    friend bool operator==(const BorrowedIterator &Left, std::nullptr_t) noexcept {
      return !static_cast<bool>(Left.Data);
    }
  };

public:
  ConcurrentStack() = default;
  ConcurrentStack(const ConcurrentStack &) = delete;
  ConcurrentStack(ConcurrentStack &&) = delete;
  ConcurrentStack &operator=(const ConcurrentStack &) noexcept = delete;
  ConcurrentStack &operator=(ConcurrentStack &&) noexcept = delete;
  ~ConcurrentStack() = default;

  using iterator = BorrowedIterator<T>;
  using const_iterator = BorrowedIterator<const T>;

  auto begin() { return iterator{Head.load()}; }
  auto begin() const { return const_iterator{Head.load()}; }
  // ReSharper disable once CppMemberFunctionMayBeStatic
  auto end() const { return nullptr; }

  auto pushFront(T Value) {
    auto NewElement = std::make_shared<Node>(Value, Head);
    while (!Head.compare_exchange_weak(NewElement->Next, NewElement))
      ;
    return iterator{std::move(NewElement)};
  }
  auto popFront() {
    auto CurrentHead = Head.load();
    while (CurrentHead && !Head.compare_exchange_weak(CurrentHead, CurrentHead->Next))
      ;
    return iterator{std::move(CurrentHead)};
  }
};

#endif