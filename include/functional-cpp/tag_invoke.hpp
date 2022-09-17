#pragma once
#include "detail/Forward.hpp"

namespace fcpp {
  namespace _tag_invoke {
    void tag_invoke();

    struct _fn {
      template <typename Tag, typename... Args>
      constexpr auto operator()(Tag tag, Args&&... args) const
        FCPP_FWD_CALL(tag_invoke((Tag &&) tag, (Args &&) args...))
    };

    template <typename Tag, typename... Args>
    using tag_invoke_result_t = decltype(
        tag_invoke(FCPP_DECLVAL(Tag &&), FCPP_DECLVAL(Args &&)...));

    using yes_t = char;
    using no_t = char(&)[2];

    template <typename Tag, typename... Args>
    auto try_tag_invoke(int) //
        noexcept(noexcept(tag_invoke(
            FCPP_DECLVAL(Tag &&), FCPP_DECLVAL(Args &&)...)))
        -> decltype(static_cast<void>(tag_invoke(
            FCPP_DECLVAL(Tag &&), FCPP_DECLVAL(Args &&)...)), yes_t{});

    template <typename Tag, typename... Args>
    no_t try_tag_invoke(...) noexcept(false);

    template <template <typename...> class T, typename... Args>
    struct defer {
      using type = T<Args...>;
    };

    struct empty {};
  }  // namespace _tag_invoke

  namespace _tag_invoke_cpo {
    inline constexpr _tag_invoke::_fn tag_invoke{};
  }
  using namespace _tag_invoke_cpo;

  template <auto& Tag>
  using tag_t = remove_cvref_t<decltype(Tag)>;

  // Manually implement the traits here rather than defining them in terms of
  // the corresponding std::invoke_result/is_invocable/is_nothrow_invocable
  // traits to improve compile-times. We don't need all of the generality of the
  // std:: traits and the tag_invoke traits are used heavily through libunifex
  // so optimising them for compile time makes a big difference.

  using _tag_invoke::tag_invoke_result_t;

  template <typename Tag, typename... Args>
  inline constexpr bool is_tag_invocable_v =
      (sizeof(_tag_invoke::try_tag_invoke<Tag, Args...>(0)) ==
       sizeof(_tag_invoke::yes_t));

  template <typename Tag, typename... Args>
  struct tag_invoke_result
    : conditional_t<
          is_tag_invocable_v<Tag, Args...>,
          _tag_invoke::defer<tag_invoke_result_t, Tag, Args...>,
          _tag_invoke::empty>
  {};

  template <typename Tag, typename... Args>
  using is_tag_invocable = std::bool_constant<is_tag_invocable_v<Tag, Args...>>;

  template <typename Tag, typename... Args>
  inline constexpr bool is_nothrow_tag_invocable_v =
      noexcept(_tag_invoke::try_tag_invoke<Tag, Args...>(0));

  template <typename Tag, typename... Args>
  using is_nothrow_tag_invocable =
      std::bool_constant<is_nothrow_tag_invocable_v<Tag, Args...>>;

  template <typename Tag, typename... Args>
  concept tag_invocable =
      (sizeof(_tag_invoke::try_tag_invoke<Tag, Args...>(0)) ==
       sizeof(_tag_invoke::yes_t));
} // namespace fcpp

#define FCPP_MAKE_TAG_CPO(name) \
    inline constexpr struct name##_fn { \
        template <typename... Ts> \
        constexpr auto operator()(Ts&&... args) \
            FCPP_FWD_CALL(fcpp::tag_invoke(*this, FCPP_FWD(args)...)) \
    } name {}; \
    using name##_tag_t = fcpp::tag_t<name>;
