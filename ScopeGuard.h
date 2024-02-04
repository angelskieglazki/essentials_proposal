#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <utility>
#include <type_traits>
#include <exception>
#include <stdexcept>

template <typename F, bool InvokeNoExecept>
class ScopeGuard
{
private:
	F f;
	bool dismissed_ = false;

public:
	ScopeGuard(F &&func)
		: f(std::move(func))
	{
	}

	~ScopeGuard() noexcept(InvokeNoExecept)
	{
		if (!dismissed_)
		{
			if (InvokeNoExecept) {
				try
				{
					f();
				}
				catch (...)
				{
				}
			} else {
				f();
			}
		}
	}

	void dismiss() { dismissed_ = true; }
	void rehire() { dismissed_ = false; }
};

template <typename Fun, bool InvokeNoExecept>
using ScopeGuardDecay = ScopeGuard<typename std::decay_t<Fun>, InvokeNoExecept>;

template <typename Fun>
inline auto MakeGuard(Fun &&func) noexcept(noexcept(ScopeGuardDecay<Fun, true>(std::forward<Fun>(func))))
{
	return ScopeGuardDecay<Fun, true>(std::forward<Fun>(func));
}

//============================================================================================================
template <typename F, bool ExecuteOnException>
class ScopeGuardNewException
{
private:
	ScopeGuard<F, ExecuteOnException> guard_;
	int exceptionCounter_{std::uncaught_exceptions()};

public:
	ScopeGuardNewException(F &&func)
		: guard_(std::move(func))
	{
	}
	ScopeGuardNewException(const F &func)
		: guard_(func)
	{
	}
	ScopeGuardNewException(ScopeGuardNewException&& other) = default;

	~ScopeGuardNewException() noexcept(ExecuteOnException)
	{
		if (ExecuteOnException != (exceptionCounter_ < std::uncaught_exceptions()))
		{
			guard_.dismiss();
		}
	}
};

namespace privat {
struct ScopeGuardOnFail
{
};

template <typename F>
ScopeGuardNewException<typename std::decay_t<F>, true>
operator+(ScopeGuardOnFail, F &&func)
{
	return ScopeGuardNewException<typename std::decay_t<F>, true>(std::forward<F>(func));
}

struct ScopeGuardOnSuccess
{
};

template <typename F>
ScopeGuardNewException<typename std::decay_t<F>, false>
operator+(ScopeGuardOnSuccess, F &&func)
{
	return ScopeGuardNewException<typename std::decay_t<F>, false>(std::forward<F>(func));
}

struct ScopeGuardOnExit
{
};
template <typename F>
ScopeGuard<typename std::decay_t<F>, true>
operator+(ScopeGuardOnExit, F &&func)
{
	return ScopeGuard<typename std::decay_t<F>, true>(std::forward<F>(func));
}

} // namespace privat

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)

#define SCOPE_EXIT                              \
	auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = \
		::privat::ScopeGuardOnExit() + [&]() noexcept

#define SCOPE_FAIL                              \
	auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) = \
		::privat::ScopeGuardOnFail() + [&]() noexcept

#define SCOPE_SUCCESS                              \
	auto ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) = \
		::privat::ScopeGuardOnSuccess() + [&]()

#endif // SCOPEGUARD_H_
