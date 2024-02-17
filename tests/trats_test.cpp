#include "traits.h"
#include <gtest/gtest.h>


struct Foo1 {
  const int Tra2323ceId1 = 1;
  int bar() { return Tra2323ceId1; }
};

struct Bar {
  using Foo = struct Foo1;
  int TraceId = 1;
  int baz() { return TraceId; }
  Foo f;
  Foo *fptr = nullptr;
};

GENERATE_HAS_MEMBER_TRAIT(bar)
GENERATE_HAS_MEMBER_TRAIT(Tra2323ceId1)
GENERATE_HAS_MEMBER_TRAIT(fptr)

TEST(Traits, HasMember_Test) {
  const Foo1 test;
  EXPECT_TRUE(has_member_bar_v<decltype(test)>);
  EXPECT_TRUE(has_member_Tra2323ceId1_v<Foo1>);
  EXPECT_FALSE(has_member_Tra2323ceId1_v<Bar>);
  EXPECT_TRUE(has_member_fptr_v<Bar>);
}

