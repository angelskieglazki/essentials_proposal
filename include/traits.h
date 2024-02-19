#pragma once
#include <type_traits>

/**
 * Признак для определения, является ли тип специализацией шаблона.
 * Этот признак проверяет, является ли данный тип T специализацией
 * предоставленного шаблона Primary.
 */
template <typename T, template <typename...> typename Primary>
struct is_specialization_of : std::false_type {};

template <template <typename...> typename Primary, typename... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type {};

template <typename T, template <typename...> typename Primary>
inline constexpr bool is_specialization_of_v =
    is_specialization_of<T, Primary>::value;

/**
 * Макрос GENERATE_HAS_MEMBER_TRAIT генерирует класс признака (trait) для
 * проверки наличия конкретного члена в заданном типе. Макрос принимает один
 * аргумент 'member', который является именем проверяемого члена.
 */
#define GENERATE_HAS_MEMBER_TRAIT(member)                          \
                                                                   \
  template <typename, typename = std::void_t<>>                    \
  struct has_member_##member : std::false_type {};                 \
                                                                   \
  template <typename T>                                            \
  struct has_member_##member<T, std::void_t<decltype(&T::member)>> \
      : std::true_type {};                                         \
                                                                   \
  template <typename T>                                            \
  inline constexpr bool has_member_##member##_v =                  \
      has_member_##member<T>::value;


// OLDSCHOOL STYLE

// Макрос GENERATE_HAS_MEMBER_TRAIT генерирует класс признака (trait) для
// проверки наличия конкретного члена в заданном типе. Макрос принимает один
// аргумент 'member', который является именем проверяемого члена.

// Сгенерированный класс признака has_member_<member> использует SFINAE
// (Substitution Failure Is Not An Error - Ошибка Подстановки Не Является
// Ошибкой) для определения наличия у типа T члена с именем 'member'. Мы
// специально сосдаём класс fallback c членом member, далее наследум внутренний
// класс от проверяемого типа Т и fallback. после этого перегружаем две функции,
// одна из которых примет аргумент типа, только если кможно получить доступ к
// члену 'member', это означает,  что в классе derived он находится в
// единственном экземпляре и до него можно достучаться без разрешения
// дополнительных неоднозначностей. В противном случае сработает пдругая
// перегрузка функции, которая возвращает true_t  - это означает, что есть
// неразрешимые однозначности и внутри типа T тоже есть член 'member'. Этот
// макрос может быть использован для создания компиляционных проверок наличия
// членов, что может быть полезно // для метапрограммирования с шаблонами,
// статических утверждений, или для условного включения/выключения
// функциональности // на основе характеристик типа.

// Пример использования этого макроса:
// GENERATE_HAS_MEMBER_TRAIT(foo)
// struct MyStruct { int foo; };
// static_assert(has_member_foo<MyStruct>::value, "MyStruct should have a member
// named 'foo'");

// #define GENERATE_HAS_MEMBER_TRAIT(member)                           \
//                                                                     \
//   template <typename T>                                             \
//   struct has_member_##member {                                      \
//    private:                                                         \
//     struct fallback {                                               \
//       int member;                                                   \
//     };                                                              \
//     struct derived : T, fallback {};                                \
//                                                                     \
//     using true_t = char[1];                                         \
//     using false_t = char[2];                                        \
//     template <typename U>                                           \
//     static constexpr auto _trait(decltype(U::member)*) -> false_t&; \
//     template <typename U>                                           \
//     static constexpr auto _trait(U*) -> true_t&;                    \
//                                                                     \
//    public:                                                          \
//     static constexpr bool value =                                   \
//         sizeof(_trait<derived>(nullptr)) == sizeof(true_t);         \
//   };                                                                \
//                                                                     \
//   template <typename T>                                             \
//   constexpr bool has_member_##member##_v = has_member_##member<T>::value;

