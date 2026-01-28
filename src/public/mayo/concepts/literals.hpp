#pragma once

#include "mayo/types/core/basic_types.hpp"
#include "mayo/types/core/constant.hpp"

namespace mayo::concepts {
namespace detail {
    template <class T>
    struct IsArray : FalseType {};

    template <class T>
    struct IsArray<T[]> : TrueType {};

    template <class T, usize Size>
    struct IsArray<T[Size]> : TrueType {};
} // namespace detail

template <class T>
concept is_array = (detail::IsArray<T>::VALUE);
} // namespace mayo::concepts
