#pragma once

struct Moveonly {
  constexpr Moveonly() noexcept = default;
  ~Moveonly() noexcept = default;
  Moveonly(Moveonly&&) = default;
  Moveonly& operator=(Moveonly&&) = default;

  Moveonly(const Moveonly&) = delete;
  Moveonly& operator=(const Moveonly&) = delete;
};

struct UncopyableUnmovable {
  constexpr UncopyableUnmovable() noexcept = default;
  ~UncopyableUnmovable() noexcept = default;

  UncopyableUnmovable(UncopyableUnmovable&&) = delete;
  UncopyableUnmovable& operator=(UncopyableUnmovable&&) = delete;
  UncopyableUnmovable(const UncopyableUnmovable&) = delete;
  UncopyableUnmovable& operator=(const UncopyableUnmovable&) = delete;
};

// обычный empty base class
struct __ebc {};

template <bool copy, bool move>
using EnableCopyMove =
    std::conditional_t<copy, __ebc,
                       std::conditional_t<move, Moveonly, UncopyableUnmovable>>;
