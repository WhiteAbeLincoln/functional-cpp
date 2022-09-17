#pragma once
#include <type_traits>

// implementation for forward_like from https://www.open-std.org/jtc1/sc22/WG21/docs/papers/2021/p2445r0.pdf

namespace fcpp {
    namespace detail {
        template <typename T, typename U>
        using override_ref_t = std::conditional_t<std::is_rvalue_reference_v<T>,
            std::remove_reference_t<U> &&, U &>;
        template <typename T, typename U>
        using copy_const_t =
            std::conditional_t<std::is_const_v<std::remove_reference_t<T>>, U const, U>;
    }

    template <typename T, typename U>
    using forward_like_t =
        detail::override_ref_t<T &&, detail::copy_const_t<T, std::remove_reference_t<U>>>;

    template <typename T>
    [[nodiscard]] constexpr auto forward_like(auto&& x) noexcept -> forward_like_t<T, decltype(x)> {
        return static_cast<forward_like_t<T, decltype(x)>>(x);
    }
}
