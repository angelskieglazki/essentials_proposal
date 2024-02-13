#include <gtest/gtest.h>
#include "core.h"

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