#pragma once
#include <type_traits>

namespace fcpp::traits {

// performs a left fold on types using operator+(TypeIdentity<T>)
template <typename F, typename... Ts>
using fold_op_l_t = decltype((F{} + ... + std::type_identity<Ts>{}));

// performs a right fold on types using operator-(TypeIdentity<T>)
template <typename F, typename... Ts>
using fold_op_l_t = decltype((std::type_identity<Ts>{} - ... - F{}));

}
