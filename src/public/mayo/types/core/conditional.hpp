#pragma once

namespace mayo {
namespace types::core {
    /**
     * 条件によって型を切り替える
     */
    template <bool Condition, class T, class F>
    struct Conditional {
        using type = T;
    };

    /**
     * 条件がfalseの場合の型
     */
    template <class T, class F>
    struct Conditional<false, T, F> {
        using type = F;
    };
} // namespace types::core

/**
 * 条件によって型を切り替えるエイリアス
 */
template <bool Condition, class T, class F>
using conditional = typename types::core::Conditional<Condition, T, F>::type;
} // namespace mayo
