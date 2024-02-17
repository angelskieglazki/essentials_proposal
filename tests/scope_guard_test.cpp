#include <gtest/gtest.h>
#include "scope_guard.h"

enum class Result { SUCCESS, HANDLED_FAIL, UNHANDLED_FAIL };

void testResultExit(Result result) {
  /**
   * Иногда возникает необходимость, выполнить какое-то действие независимо от
   * того, произошло исключение или нет. Например, вы хотите закрыть соединение
   * с базой данных независимо от того, возникло ли исключение во время работы
   * c ней или нет.
   */
  bool res = false;

  try {
    auto guard = makeGuard([&] { res = true; });
    (void)guard;
    // SCOPE_EXIT { res = true; }; - этот макрос делает тоже самое что и код
    // выше.

    try {
      if (result == Result::HANDLED_FAIL) {
        throw std::runtime_error("test");
      } else if (result == Result::UNHANDLED_FAIL) {
        throw "don't do this!";
      }
    } catch (const std::runtime_error&) {
      // поймали HANDLED_FAIL
    }
  } catch (...) {
    // поймали UNHANDLED_FAIL
  }
}

TEST(ScopeGuard, TryCatchResultExit) {
  testResultExit(Result::SUCCESS);
  testResultExit(Result::HANDLED_FAIL);
  testResultExit(Result::UNHANDLED_FAIL);
}

TEST(ScopeGuard, ScopeExit_test) {
  int x = 0;
  {
    SCOPE_EXIT { ++x; };
    EXPECT_EQ(x, 0);
  }
  EXPECT_EQ(x, 1);
}

struct Foo {
  Foo() = default;
  ~Foo() {
    try {
      auto e = std::current_exception();
      int x = 0;
      {
        SCOPE_EXIT { ++x; };
        EXPECT_EQ(x, 0);
      }
      EXPECT_EQ(x, 1);
    } catch (const std::exception& ex) {
      std::cerr << "Unexpected exception: " << ex.what();
    }
  }
};

TEST(ScopeGuard, ScopeFail_test) {
  try {
    Foo f;
    throw std::runtime_error("test");
  } catch (...) {
  }
}

void testFailAndSuccess(Result result, bool expect_fail) {
  bool fail_executed = false;
  bool success_executed = false;

  try {
    SCOPE_FAIL { fail_executed = true; };
    SCOPE_SUCCESS { success_executed = true; };

    try {
      if (result == Result::HANDLED_FAIL) {
        throw std::runtime_error("expected error");
      } else if (result == Result::UNHANDLED_FAIL) {
        throw "don't do this!";
      }
    } catch (const std::runtime_error&) {
    }
  } catch (...) {
    // поймали UNHANDLED_FAIL
  }

  EXPECT_EQ(fail_executed, expect_fail);
  EXPECT_EQ(success_executed, !expect_fail);
}

TEST(ScopeGuard, ScopeFailAndSuccess_Test) {
  testFailAndSuccess(Result::SUCCESS, false);
  testFailAndSuccess(Result::HANDLED_FAIL, false);
  testFailAndSuccess(Result::UNHANDLED_FAIL, true);
}

TEST(ScopeGuard, ScopeFailExceptionPtr_Test) {
  bool catch_exception = false;
  bool fail_executed = false;

  try {
    SCOPE_FAIL { fail_executed = true; };

    std::exception_ptr ep;
    try {
      throw std::runtime_error("test");
    } catch (...) {
      ep = std::current_exception();
    }
    std::rethrow_exception(ep);
  } catch (const std::exception&) {
    catch_exception = true;
  }

  EXPECT_TRUE(catch_exception);
  EXPECT_TRUE(fail_executed);
}

TEST(ScopeGuard, ScopeSuccessThrow_Test) {
  auto l = []() {
    SCOPE_SUCCESS { throw std::runtime_error("test"); };
  };

  EXPECT_THROW(l(), std::runtime_error);
}

TEST(ScopeGuard, ThrowingCleanupAction_Test) {
  struct ThrowingCleanupAction {
    explicit ThrowingCleanupAction(int& x) : x_(x) {}
    ThrowingCleanupAction(const ThrowingCleanupAction& other) : x_(other.x_) {
      std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
      throw std::runtime_error("test");
    }

    void operator()() { ++x_; }

   private:
    int& x_;
  };

  int scope_exit = 0;
  ThrowingCleanupAction on_exit(scope_exit);
  EXPECT_THROW((void)makeGuard(on_exit), std::runtime_error);
  EXPECT_EQ(scope_exit, 1);
  EXPECT_THROW((void)makeGuard(ThrowingCleanupAction(scope_exit)), std::runtime_error);
  EXPECT_EQ(scope_exit, 2);
}
