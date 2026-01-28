#pragma once

#include "mayo/concepts/ref.hpp"
#include "mayo/type_traits/copy_trait.hpp"
#include "mayo/type_traits/remove_cvref.hpp"

namespace mayo {
namespace utility {
    /**
     * 右辺値としてムーブする
     */
    template <class T>
    constexpr auto move(T&& value) noexcept -> remove_ref<T>&& {
        return static_cast<remove_ref<T>&&>(value);
    }

    /**
     * 左辺値を完全転送する
     */
    template <class T>
    constexpr auto forward(remove_ref<T>& value) noexcept -> T&& {
        return static_cast<T&&>(value);
    }

    /**
     * 右辺値を完全転送する
     */
    template <class T>
    constexpr auto forward(remove_ref<T>&& value) noexcept -> T&& {
        static_assert(!concepts::is_lvalue_ref<T>, "右辺値を左辺値として転送できません");
        return static_cast<T&&>(value);
    }

    /**
     * 参照/const修飾を元の型に合わせて転送する
     */
    template <class Original, class T>
    constexpr auto forward_like(T&& value) noexcept
        -> forward_like_type<Original, decltype(value)> {
            using Return = forward_like_type<Original, decltype(value)>;
            return static_cast<Return>(value);
        }
} // namespace utility

using utility::move;
using utility::forward;
using utility::forward_like;
} // namespace mayo
