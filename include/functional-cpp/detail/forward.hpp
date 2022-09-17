#pragma once
#include <type_traits>

#define FCPP_FWD_CALL(...) \
    noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) \
        { return (__VA_ARGS__); }

#define FCPP_FWD(...) \
    static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

#define FCPP_DECLVAL(...) static_cast<__VA_ARGS__(*)()noexcept>(nullptr)()

#define FCPP_MOV(...) \
    static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

// namespace fcpp {
//     // implementation from https://en.cppreference.com/w/cpp/utility/forward_like
//     template <typename T, typename U>
//     [[nodiscard]] constexpr auto&& forward_like(U&& x) noexcept
//     {
//         constexpr bool is_adding_const = std::is_const_v<std::remove_reference_t<T>>;
//         if constexpr (std::is_lvalue_reference_v<T&&>) {
//             if constexpr (is_adding_const) {
//                 return std::as_const(x);
//             }
//             else {
//                 return static_cast<U&>(x);
//             }
//         }
//         else {
//             if constexpr (is_adding_const) {
//                 return FCPP_MOV(std::as_const(x));
//             }
//             else {
//                 return FCPP_MOV(x);
//             }
//         }
//     }
// }
