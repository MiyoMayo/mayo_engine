#pragma once

#include "mayo/type_traits/add_cvref.hpp"

namespace mayo::concepts {

    /**
     * 代入可能かを判定する
     */
    template <class Target, class T>
    concept is_assignable = (__is_assignable(Target, T));

    /**
     * コピー代入可能かを判定する
     */
    template <class Target, class T>
    concept is_copy_assignable
        = (__is_assignable(mayo::add_lvalue_ref<Target>, mayo::add_lvalue_ref<const T>));

    /**
     * ムーブ代入可能かを判定する
     */
    template <class Target, class T>
    concept is_move_assignable
        = (__is_assignable(mayo::add_lvalue_ref<Target>, mayo::add_rvalue_ref<T>));

    /**
     * 例外なしで代入可能かを判定する
     */
    template <class Target, class T>
    concept is_nothrow_assignable = (__is_nothrow_assignable(Target, T));

    /**
     * 例外なしでコピー代入可能かを判定する
     */
    template <class Target, class T>
    concept is_nothrow_copy_assignable
        = (__is_nothrow_assignable(mayo::add_lvalue_ref<Target>, mayo::add_lvalue_ref<const T>));

    /**
     * 例外なしでムーブ代入可能かを判定する
     */
    template <class Target, class T>
    concept is_nothrow_move_assignable
        = (__is_nothrow_assignable(mayo::add_lvalue_ref<Target>, mayo::add_rvalue_ref<T>));
} // namespace mayo::concepts
