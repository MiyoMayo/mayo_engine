#pragma once

#include "mayo/type_traits/add_cvref.hpp"

namespace mayo::concepts {

    /**
     * 指定引数で構築可能かを判定する
     */
    template <class T, class... Args>
    concept is_constructible = (__is_constructible(T, Args...));

    /**
     * コピー構築可能かを判定する
     */
    template <class T>
    concept is_copy_constructible
        = (__is_constructible(T, mayo::add_lvalue_ref<const T>));

    /**
     * ムーブ構築可能かを判定する
     */
    template <class T>
    concept is_move_constructible
        = (__is_constructible(T, mayo::add_rvalue_ref<T>));

    /**
     * 例外なしで構築可能かを判定する
     */
    template <class T, class... Args>
    concept is_nothrow_constructible = (__is_nothrow_constructible(T, Args...));

    /**
     * 例外なしでコピー構築可能かを判定する
     */
    template <class T>
    concept is_nothrow_copy_constructible
        = (__is_nothrow_constructible(T, mayo::add_lvalue_ref<const T>));

    /**
     * 例外なしでムーブ構築可能かを判定する
     */
    template <class T>
    concept is_nothrow_move_constructible
        = (__is_nothrow_constructible(T, mayo::add_rvalue_ref<T>));
} // namespace mayo::concepts
