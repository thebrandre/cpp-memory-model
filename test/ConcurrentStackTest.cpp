#include "ConcurrentStack.h"

#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <ranges>
#include <utility>

TEST_CASE("concurrent stack test", "[lockfree]") {
  ConcurrentStack<int> Stack;
  for (auto i : std::views::iota(1, 5)) {
    REQUIRE(*Stack.pushFront(i) == i);
    REQUIRE(*Stack.begin() == i);
  }
  REQUIRE(static_cast<bool>(std::ranges::find(Stack, 4)));
  REQUIRE(*std::ranges::find(Stack, 4) == 4);
  REQUIRE(*std::ranges::find(std::as_const(Stack), 4) == 4);
  REQUIRE(*Stack.popFront() == 4);
  REQUIRE(!std::ranges::find(Stack, 4));
  REQUIRE(std::ranges::find(Stack, 4) == Stack.end());
  REQUIRE(Stack.end() == std::ranges::find(Stack, 4));

  auto StoredFront = Stack.begin();
  REQUIRE(*Stack.popFront() == 3);
  REQUIRE(!static_cast<bool>(std::ranges::find(Stack, 3)));
  REQUIRE(std::ranges::find(Stack, 3) == Stack.end());
  REQUIRE(Stack.end() == std::ranges::find(Stack, 3));
  REQUIRE(*Stack.popFront() == 2);
  // REQUIRE(*Stack.popFront() == 1);
  // REQUIRE(!static_cast<bool>(Stack.popFront()));

  REQUIRE(*StoredFront == 3);
  REQUIRE(*++StoredFront == 2);
  REQUIRE(*++StoredFront == 1);
  REQUIRE(!static_cast<bool>(++StoredFront));
}


static_assert(std::input_iterator<ConcurrentStack<int>::iterator>);
static_assert(std::input_iterator<ConcurrentStack<int>::const_iterator>);
static_assert(std::ranges::input_range<ConcurrentStack<int>>);