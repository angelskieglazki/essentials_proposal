#include "core.h"
#include <gtest/gtest.h>

TEST(Core, Moveonly_Test) {
  EXPECT_TRUE(std::is_move_constructible<Moveonly>::value);
  EXPECT_FALSE(std::is_copy_constructible<Moveonly>::value);
  EXPECT_TRUE(std::is_move_assignable<Moveonly>::value);
  EXPECT_FALSE(std::is_copy_assignable<Moveonly>::value);
}

TEST(Core, UncopyableUnmovable_Test) {
  EXPECT_FALSE(std::is_move_constructible<UncopyableUnmovable>::value);
  EXPECT_FALSE(std::is_copy_constructible<UncopyableUnmovable>::value);
  EXPECT_FALSE(std::is_move_assignable<UncopyableUnmovable>::value);
  EXPECT_FALSE(std::is_copy_assignable<UncopyableUnmovable>::value);
}

class Foo_ALL : private EnableCopyMove<true, false> {};
class Foo_MoveOnly : private EnableCopyMove<false, true> {};
class Foo_NoMoveNoCopy : private EnableCopyMove<false, false> {};
TEST(Core, Foo_Test) {
  EXPECT_TRUE(std::is_move_constructible<Foo_ALL>::value);
  EXPECT_TRUE(std::is_copy_constructible<Foo_ALL>::value);
  EXPECT_TRUE(std::is_move_assignable<Foo_ALL>::value);
  EXPECT_TRUE(std::is_copy_assignable<Foo_ALL>::value);

  EXPECT_TRUE(std::is_move_constructible<Foo_MoveOnly>::value);
  EXPECT_FALSE(std::is_copy_constructible<Foo_MoveOnly>::value);
  EXPECT_TRUE(std::is_move_assignable<Foo_MoveOnly>::value);
  EXPECT_FALSE(std::is_copy_assignable<Foo_MoveOnly>::value);

  EXPECT_FALSE(std::is_move_constructible<Foo_NoMoveNoCopy>::value);
  EXPECT_FALSE(std::is_copy_constructible<Foo_NoMoveNoCopy>::value);
  EXPECT_FALSE(std::is_move_assignable<Foo_NoMoveNoCopy>::value);
  EXPECT_FALSE(std::is_copy_assignable<Foo_NoMoveNoCopy>::value);
}

template <bool copy, bool move>
class TestCopyMove {
  using T = EnableCopyMove<copy, move>;
  static_assert(std::is_copy_constructible<T>::value == copy);
  static_assert(std::is_move_constructible<T>::value == (copy || move));
  static_assert(std::is_copy_assignable<T>::value == copy);
  static_assert(std::is_move_assignable<T>::value == (copy || move));
};

template class TestCopyMove<true, true>;
template class TestCopyMove<true, false>;
template class TestCopyMove<false, true>;
template class TestCopyMove<false, false>;

TEST(Core, Moveonly2_Test) {
  class Foo : Moveonly {
    int a;
  };

  static_assert(!std::is_copy_constructible_v<Foo>,
                "Should not be copy constructible");

  // Test that move actually works.
  Foo foo;
  Foo foo2(std::move(foo));
  (void)foo2;

  // Test that inheriting from MoveOnly doesn't prevent the move
  // constructor from being noexcept.
  static_assert(std::is_nothrow_move_constructible_v<Foo>,
                "Should have noexcept move constructor");
}

TEST(Core, UncopyableUnmovable2_Test) {
  class Foo : UncopyableUnmovable {
    int a;
  };

  static_assert(!std::is_copy_constructible_v<Foo>,
                "Should not be copy constructible");
  static_assert(!std::is_move_constructible_v<Foo>,
                "Should not be move constructible");
}

TEST(Core, Misc) {
  class abstract {
   public:
    virtual ~abstract() = default;
    virtual void fn() = 0;
  };
  class Foo : public abstract {
  public:
    void fn() override {}
  };
  static_assert(std::is_copy_constructible_v<Foo>,
                "Should be copy constructible");
  static_assert(std::is_move_constructible_v<Foo>,
                "Should be move constructible");
}