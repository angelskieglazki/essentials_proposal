#include <gtest/gtest.h>
#include "ScopeGuard.h"


TEST(ScopeGuardTest, ScopeGuardTest)
{
    EXPECT_EQ(0, 0);

    // ScopeGuard removeFriend = ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
    // auto removeFriend = MakeGuard([&friends = friends_]()
    //                               { friends.pop_back(); }); // это должен быть ON_BLOCK_FAIL
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}