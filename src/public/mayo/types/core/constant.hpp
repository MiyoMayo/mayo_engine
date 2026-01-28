#pragma once

namespace mayo {
namespace types::core {

    /**
     * 定数を表す型
     *
     * @tparam T 定数の型
     * @tparam Value 定数の値
     */
    template <class T, T Value>
    struct IntegralConstant {
        static constexpr T VALUE = Value;

        constexpr operator T() const noexcept {
            return VALUE;
        }
    };
} // namespace types::core

using types::core::IntegralConstant;

/**
 * bool型の定数を表す型
 *
 * @tparam Value 定数の値
 */
template <bool Value>
using BoolConstant = IntegralConstant<bool, Value>;

/**
 * trueを表す型
 */
using TrueType = BoolConstant<true>;

/**
 * falseを表す型
 */
using FalseType = BoolConstant<false>;
}; // namespace mayo
