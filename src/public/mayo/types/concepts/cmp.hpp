#pragma once

#include "mayo/types/concepts/type_traits.hpp"
#include <compare>

namespace mayo::concepts {

// 三方比較可能判定
template <class T, class Category = std::partial_ordering>
concept three_way_comparable = (std::three_way_comparable<T, Category>);

// 三方比較可能判定（2型）
template <class T, class U, class Category = std::partial_ordering>
concept three_way_comparable_with = (std::three_way_comparable_with<T, U, Category>);

// 等値比較可能判定
template <class T, class U, class Result = bool>
concept is_equality_comparable = requires(T x, U y) {
    { x == y } -> concepts::is_convertible<Result>;
};
} // namespace mayo::concepts
