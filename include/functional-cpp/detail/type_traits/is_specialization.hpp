#pragma once

namespace fcpp::traits {

    /**
     * Tests whether a type @p T is a specialization of the template @p Tmpl meaning
     * @p T is the template @p Tmpl fully applied.
     *
     * @param T the type to test
     * @param Tmpl the matching template
     */
    template <typename T, template <typename...> typename Tmpl>
    struct is_specialization : std::false_type {};
    template <template <typename...> class Tmpl, typename... Args>
    struct is_specialization<Tmpl<Args...>, Tmpl>: std::true_type {};

    template <typename T, template <typename...> typename Tmpl>
    inline constexpr bool is_specialization_v = is_specialization<T, Tmpl>::value;

}

namespace fcpp::concepts {
    template <typename T, template <typename...> typename Tmpl>
    concept SpecializationOf = traits::is_specialization_v<T, Tmpl>;
}
