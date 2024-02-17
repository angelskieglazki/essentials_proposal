#pragma once
#include <type_traits>

template <typename T, template <typename...> typename Primary>
struct is_specialization_of : std::false_type {};

template <template <typename...> typename Primary, typename... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template <typename T, template <typename...> typename Primary>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<T, Primary>::value;

#define GENERATE_HAS_MEMBER_TRAIT(member)                           \
                                                                    \
  template <typename T>                                             \
  struct has_member_##member {                                      \
   private:                                                         \
    struct fallback {                                               \
      int member;                                                   \
    };                                                              \
    struct derived : T, fallback {};                                \
                                                                    \
    using true_t = char[1];                                         \
    using false_t = char[2];                                        \
    template <typename U>                                           \
    static constexpr auto _trait(decltype(U::member)*) -> false_t&; \
    template <typename U>                                           \
    static constexpr auto _trait(U*) -> true_t&;                    \
                                                                    \
   public:                                                          \
    static constexpr bool value =                                   \
        sizeof(_trait<derived>(nullptr)) == sizeof(true_t);         \
  };                                                                \
                                                                    \
  template <typename T>                                             \
  constexpr bool has_member_##member##_v = has_member_##member<T>::value;
