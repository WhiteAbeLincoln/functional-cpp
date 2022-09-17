#pragma once
#include <optional>
#include "../core.hpp"
#include "../typeclasses/functor.hpp"
#include "../detail/type_traits/is_specialization.hpp"

namespace fcpp::hkt
{
    struct option_uri : uri_base {};

    template <typename T>
    auto tag_invoke(uri_tag, std::optional<T>) -> option_uri;
    template <typename T>
    auto tag_invoke(eliminate_tag, option_uri, std::optional<T>) -> type_list<T>;
    template <typename T>
    auto tag_invoke(construct_tag, option_uri, T) -> std::optional<T>;

    static_assert(HigherKindedType<std::optional<int>>);
}

namespace fcpp::functor
{
    template <concepts::SpecializationOf<std::optional> T, typename Fn>
    constexpr auto tag_invoke(transform_fn, T&& opt, Fn&& fn) -> std::optional<decltype(fn(FCPP_FWD(opt).value()))>
    {
        if (!opt) return std::nullopt;
        return fn(FCPP_FWD(opt).value());
    }

    static_assert(Functor<std::optional<int>>);
}
