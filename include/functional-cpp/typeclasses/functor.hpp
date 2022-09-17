#pragma once
#include "../core.hpp"
#include "../detail/type_traits/take_n.hpp"
#include "../detail/forward_like.hpp"
// FIX: use forward_like in the transform_result_fn and transform_fn's operator()

namespace fcpp::functor
{
    namespace detail {
        template <typename T, typename Val>
        using fwd_as = fcpp::forward_like_t<T, Val>;
        template <typename T, typename Val, typename Fn>
        using call_fn = decltype(FCPP_DECLVAL(Fn)(FCPP_DECLVAL(fwd_as<T, Val>)));

        template <typename T, typename... Fns, typename... Results>
        auto transform_result_fn(hkt::type_list<>, hkt::type_list<Fns...>, hkt::type_list<Results...>)
            -> hkt::type_list<Results...>;

        template <typename T, typename Val, typename... Vals, typename... Results>
        auto transform_result_fn(hkt::type_list<Val, Vals...>, hkt::type_list<>, hkt::type_list<Results...>)
            -> decltype(transform_result_fn<T>(
                hkt::type_list<>{},
                hkt::type_list<>{},
                hkt::type_list<Results..., fwd_as<T, Val>, fwd_as<T, Vals>...>{}
            ));

        template <typename T, typename Val, typename... Vals, typename Fn, typename... Fns, typename... Results>
        auto transform_result_fn(hkt::type_list<Val, Vals...>, hkt::type_list<Fn, Fns...>, hkt::type_list<Results...>)
            -> decltype(transform_result_fn<T>(
                hkt::type_list<Vals...>{},
                hkt::type_list<Fns...>{},
                hkt::type_list<Results..., call_fn<T, Val, Fn>>{}
            ));

        template <typename A, typename... Fns>
        using transform_result_t = decltype(transform_result_fn<A>(hkt::eliminate_t<A>{}, hkt::type_list<Fns...>{}, hkt::type_list<>{}));

        template <typename A, typename... Fns>
        concept TransformFns =
            requires {
                requires hkt::HigherKindedType<A>;
                requires sizeof...(Fns) > 0 && sizeof...(Fns) <= hkt::num_types_v<A>;
                typename transform_result_t<A, Fns...>;
            };
    }

    inline constexpr struct transform_fn
    {
        template <hkt::HigherKindedType A, typename... Fns>
            requires detail::TransformFns<A, Fns...>
        constexpr auto operator()(A&& val, Fns&&... fns) const
            FCPP_FWD_CALL(fcpp::tag_invoke(*this, FCPP_FWD(val), FCPP_FWD(fns)...))
    } transform {};

    namespace detail {
        template <typename T>
        using id_fn = T (*)(T val) noexcept;

        template <typename A, typename... Vals>
        constexpr auto transform_id(A&& val, hkt::type_list<Vals...>)
            -> decltype(functor::transform(FCPP_FWD(val), FCPP_DECLVAL(id_fn<Vals>)...));
    }

    template <typename A, std::size_t N = hkt::num_types_v<A>>
    concept Functor =
        N > 0 && N <= hkt::num_types_v<A> &&
        requires(A a) {
            requires hkt::HigherKindedType<A>;
            { detail::transform_id(FCPP_FWD(a), traits::take_n_t<hkt::eliminate_t<A>, N>{}) } -> hkt::HigherKindedType;
        };
}
