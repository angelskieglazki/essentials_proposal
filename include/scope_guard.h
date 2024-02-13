#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_
#include <stdio.h>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 *
 */

class ScopeGuardBase {
 public:
  void dismiss() noexcept { dismissed_ = true; }
  void rehire() noexcept { dismissed_ = false; }

 protected:
  explicit ScopeGuardBase(bool dismissed = false) noexcept
      : dismissed_(dismissed) {}

 protected:
  bool dismissed_;
};

template <typename F, bool InvokeNoExecept>
class ScopeGuard : public ScopeGuardBase {
 public:
  explicit ScopeGuard(F&& func) : f(std::move(func)) {}

  ~ScopeGuard() noexcept(InvokeNoExecept) {
    if (!dismissed_) {
      if (InvokeNoExecept) {
        try {
          f();
        } catch (...) {
        }
      } else {
        f();
      }
    }
  }

 private:
  F f;
};

template <typename Fun, bool InvokeNoExecept>
using ScopeGuardDecay = ScopeGuard<typename std::decay_t<Fun>, InvokeNoExecept>;

template <typename Fun>
[[nodiscard]] auto MakeGuard(Fun&& func) noexcept(
    noexcept(ScopeGuardDecay<Fun, true>(std::forward<Fun>(func)))) {
  return ScopeGuardDecay<Fun, true>(std::forward<Fun>(func));
}

class UncaughtExceptionDetector {
 public:
  UncaughtExceptionDetector() = default;
  bool haveNewException() const {
    return std::uncaught_exceptions() > counter_;
  }

 private:
  const int counter_{std::uncaught_exceptions()};
};

template <typename F, bool ExecuteOnException>
class ScopeGuardNewException {
 public:
  explicit ScopeGuardNewException(F&& func) : guard_(std::move(func)) {}
  explicit ScopeGuardNewException(const F& func) : guard_(func) {}
  ScopeGuardNewException(ScopeGuardNewException&& other)  = default;

  ~ScopeGuardNewException() noexcept(ExecuteOnException) {
    if (ExecuteOnException != detector_.haveNewException()) {
      guard_.dismiss();
    }
  }

 private:
  ScopeGuard<F, ExecuteOnException> guard_;
  UncaughtExceptionDetector detector_;
};

namespace privat {
struct ScopeGuardOnFail {};

template <typename F>
ScopeGuardNewException<typename std::decay_t<F>, true> operator+(
    ScopeGuardOnFail, F&& func) {
  return ScopeGuardNewException<typename std::decay_t<F>, true>(
      std::forward<F>(func));
}

struct ScopeGuardOnSuccess {};

template <typename F>
ScopeGuardNewException<typename std::decay_t<F>, false> operator+(
    ScopeGuardOnSuccess, F&& func) {
  return ScopeGuardNewException<typename std::decay_t<F>, false>(
      std::forward<F>(func));
}

struct ScopeGuardOnExit {};
template <typename F>
ScopeGuard<typename std::decay_t<F>, true> operator+(ScopeGuardOnExit,
                                                     F&& func) {
  return ScopeGuard<typename std::decay_t<F>, true>(std::forward<F>(func));
}

}  // namespace privat

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)

#define SCOPE_EXIT                            \
  auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = \
      ::privat::ScopeGuardOnExit() + [&]() noexcept

#define SCOPE_FAIL                            \
  auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) = \
      ::privat::ScopeGuardOnFail() + [&]() noexcept

#define SCOPE_SUCCESS                            \
  auto ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) = \
      ::privat::ScopeGuardOnSuccess() + [&]()

#endif  // SCOPEGUARD_H_
