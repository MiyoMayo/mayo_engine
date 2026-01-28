#pragma once

#include "mayo/types/core/basic_types.hpp"
#include "mayo/types/core/constant.hpp"

namespace mayo::concepts {
namespace detail {
    /**
     * 配列型判定の実装
     */
    template <class T>
    struct IsArray : FalseType {};

    /**
     * 未知要素数配列は真
     */
    template <class T>
    struct IsArray<T[]> : TrueType {};

    /**
     * 既知要素数配列は真
     */
    template <class T, usize Size>
    struct IsArray<T[Size]> : TrueType {};
} // namespace detail

    /**
     * 配列型かを判定する
     */
    template <class T>
    concept is_array = (detail::IsArray<T>::VALUE);
} // namespace mayo::concepts
