#pragma once
#include "tag_invoke.hpp"
#include <concepts>
#include <type_traits>
#include <cstddef>
#include "detail/type_traits/is_specialization.hpp"
#include "detail/type_traits/delay.hpp"

namespace fcpp::hkt
{
    template <typename... Ts>
    struct type_list {
        static constexpr std::size_t size = sizeof...(Ts);
    };

    struct uri_base {};

    struct uri_tag {};
    struct eliminate_tag {};
    struct construct_tag {};

    namespace detail {
        template <typename URI, typename... Ts>
        auto do_construct(URI uri, type_list<Ts...>) -> decltype(fcpp::tag_invoke(construct_tag{}, uri, FCPP_DECLVAL(Ts)...));
    }

    template <typename T>
    concept URI = std::is_base_of_v<uri_base, T>;

    template <typename T>
    concept HigherKindedType =
        requires(T a) {
            { fcpp::tag_invoke(uri_tag{}, a) } -> URI;
            {
                fcpp::tag_invoke(
                    eliminate_tag{},
                    fcpp::tag_invoke(uri_tag{}, a),
                    a
                )
            } -> fcpp::concepts::SpecializationOf<type_list>;
            {
                detail::do_construct(
                    fcpp::tag_invoke(uri_tag{}, a),
                    fcpp::tag_invoke(
                        eliminate_tag{},
                        fcpp::tag_invoke(uri_tag{}, a),
                        a
                    )
                )
            } -> std::same_as<T>;
        };

    template <HigherKindedType T>
    using uri_t = tag_invoke_result_t<uri_tag, T>;

    template <HigherKindedType T>
    using eliminate_t = tag_invoke_result_t<eliminate_tag, uri_t<T>, T>;

    template <URI U, typename... Ts>
    using construct_t = tag_invoke_result_t<construct_tag, U, Ts...>;

    namespace detail {
        template <typename T> requires HigherKindedType<T>
        inline constexpr auto get_size() { return eliminate_t<T>::size; }
        template <typename T>
        inline constexpr auto get_size() { return 0; }
    }

    template <typename T>
    inline constexpr std::size_t num_types_v = detail::get_size<T>();
}
