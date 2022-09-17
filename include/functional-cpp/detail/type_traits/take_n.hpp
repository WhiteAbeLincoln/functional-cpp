#pragma once
#include <type_traits>
#include "fold.hpp"

namespace fcpp::traits {
    namespace detail {
        template <size_t N, typename... Ts>
        struct take_n_impl
        {
            template <typename T>
            auto operator+(std::type_identity<T>) -> take_n_impl<N - 1, Ts..., T>;
        };

        template <typename... Ts>
        struct take_n_impl<0, Ts...>
        {
            template <template <typename...> typename L>
            using as = L<Ts...>;

            template <typename T>
            auto operator+(std::type_identity<T>) -> take_n_impl<0, Ts...>;
        };
    }

    /**
     * @brief Takes the first N elements of the list-like type.
     *
     * @tparam T the list-like type
     * @tparam N the number of elements
     */
    template <typename T, std::size_t N>
    struct take_n;

    template <template <typename...> typename L, std::size_t N, typename... Els>
    struct take_n<L<Els...>, N> {
    private:
    // example
    // Fold_t<TakeNImpl<5>, int, bool, char, size_t, double> expands to decltype(TakeNImpl<5> + TI<int> + TI<bool> + TI<char> + TI<size_t> + TI<double>)
    // TakeNImpl<5> + TI<int> = TakeNImpl<4, int>                                         -> decltype(TakeNImpl<4, int> + TI<bool> + TI<char> + TI<size_t> + TI<double>)
    // TakeNImpl<4, int> + TI<bool> = TakeNImpl<3, int, bool>                             -> decltype(TakeNImpl<3, int, bool> + TI<char> + TI<size_t> + TI<double>)
    // TakeNImpl<3, int, bool> + TI<char> = TakeNImpl<2, int, bool, char>                 -> decltype(TakeNImpl<2, int, bool, char> + TI<size_t> + TI<double>)
    // TakeNImpl<2, int, bool, char> + TI<size_t> = TakeNImpl<1, int, bool, char, size_t> -> decltype(TakeNImpl<1, int, bool, char, size_t> + TI<double>)
    // TakeNImpl<2, int, bool, char, size_t> + TI<double> = TakeNImpl<0, int, bool, char, size_t, double>
    // now we can extract the final type by putting it in a list, using the TakeNImpl<0, Ts...>::as template
        struct impl { using type = typename fold_op_l_t<detail::take_n_impl<N>, Els...>::template as<L>; };
    public:
        using type = typename std::conditional_t<N >= sizeof...(Els), std::type_identity<L<Els...>>, impl>::type;
    };

    template <typename T, size_t N>
    using take_n_t = typename take_n<T, N>::type;

    // template <typename... Ts>
    // struct List {};

    // static_assert(std::is_same_v<take_n_t<List<int, bool, char>, 0>, List<>>);
    // static_assert(std::is_same_v<take_n_t<List<int, bool, char>, 1>, List<int>>);
    // static_assert(std::is_same_v<take_n_t<List<int, bool, char>, 2>, List<int, bool>>);
    // static_assert(std::is_same_v<take_n_t<List<int, bool, char>, 3>, List<int, bool, char>>);
    // static_assert(std::is_same_v<take_n_t<List<int, bool, char>, 4>, List<int, bool, char>>);
    // static_assert(std::is_same_v<take_n_t<List<>, 4>, List<>>);
}
