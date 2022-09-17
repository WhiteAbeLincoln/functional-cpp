#pragma once
#include "../core.hpp"
#include "../typeclasses/functor.hpp"

namespace fcpp::hkt
{
    struct pointer_uri : uri_base {};

    template <typename T>
    auto tag_invoke(uri_tag, T*) -> pointer_uri;
    template <typename T>
    auto tag_invoke(eliminate_tag, pointer_uri, T*) -> type_list<T&>;
    template <typename T>
    auto tag_invoke(construct_tag, pointer_uri, T&) -> T*;

    static_assert(HigherKindedType<int*>);
}

namespace fcpp::functor
{
    template <typename T, typename Fn>
    constexpr auto tag_invoke(transform_fn, T* ptr, Fn&& fn) -> decltype(&fn(*ptr))
    {
        if (!ptr) return nullptr;
        return &fn(*ptr);
    }

    using Fn = char& (*) (int&);
    using T = int*;
    using Val = int&;

    using FWDED = fcpp::forward_like_t<T, Val>;
    static_assert(std::is_same_v<FWDED, int&>);
    // using T = decltype(FCPP_DECLVAL(Fn)(FCPP_DECLVAL(FWDED)));
    // static_assert(Functor<int*>);
}
