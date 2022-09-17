#pragma once

namespace fcpp::traits {
    template <template <typename...> typename Tmpl, typename... Ts>
    struct delay
    {
        using type = Tmpl<Ts...>;
    };

    struct empty {};
}
