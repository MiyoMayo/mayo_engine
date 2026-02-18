#pragma once

#include "mayo/types/concepts.hpp"
#include <compare>
#include <memory>
#include <type_traits>

namespace mayo {
namespace types::core {
    template <class T>
        requires(concepts::is_object<T> && !concepts::is_ref<T>)
    class Ref;
}

namespace concepts {
    namespace detail {
        /**
         * Ref型判定の補助
         */
        template <class>
        struct IsRefType : std::false_type {};

        template <class V>
        struct IsRefType<types::core::Ref<V>> : std::true_type {};
    } // namespace detail

    /**
     * Ref型かどうか
     */
    template <class T>
    concept is_ref_type = (detail::IsRefType<std::remove_cvref_t<T>>::value);
} // namespace concepts

namespace types::core {
    /**
     * 非所有の参照ラッパー
     *
     * @tparam T 参照先の型
     */
    template <class T>
        requires(concepts::is_object<T> && !concepts::is_ref<T>)
    class Ref {
      public:
        /**
         * ポインタ型
         */
        using ptr_type = std::add_pointer_t<T>;

        /**
         * const参照ラッパー用の型
         */
        using const_type = std::add_const_t<std::remove_const_t<T>>;

      public:
        Ref() = delete;

        /**
         * 左辺値参照から構築する
         *
         * @tparam U 構築元の型
         * @param ref 参照先
         *
         * @note 一時オブジェクトからの構築を防ぐため左辺値のみ許可する
         */
        template <class U>
        constexpr explicit Ref(U&& ref) noexcept
            requires(!concepts::is_ref_type<U> && concepts::is_lvalue_ref<U> && concepts::convertible_to<U&&, T&>)
            : ptr{std::addressof(ref)} {}

        constexpr Ref(const Ref&) noexcept = default;
        constexpr Ref(Ref&&) noexcept = default;
        constexpr auto operator=(const Ref&) noexcept -> Ref& = default;
        constexpr auto operator=(Ref&&) noexcept -> Ref& = default;
        constexpr ~Ref() = default;

        /**
         * 参照先を取得する
         */
        constexpr auto get() const noexcept -> T& {
            return *ptr;
        }

        /**
         * 参照先のデリファレンス
         */
        constexpr auto operator*() const noexcept -> T& {
            return *ptr;
        }

        /**
         * 参照先ポインタを取得する
         */
        constexpr auto operator->() const noexcept -> ptr_type {
            return ptr;
        }

        /**
         * const参照ラッパーへ変換する
         */
        constexpr operator Ref<const_type>() const noexcept
            requires(!concepts::is_const<T>)
        {
            return Ref<const_type>(*ptr);
        }

        /**
         * 参照先へ暗黙変換する
         */
        constexpr operator T&() const noexcept {
            return *ptr;
        }

      public:
        friend constexpr auto operator==(Ref x, Ref y) noexcept(noexcept(*x == *y)) -> bool
            requires(concepts::equality_comparable<T>)
        {
            return *x == *y;
        }

        friend constexpr auto operator==(Ref x, const T& y) noexcept(noexcept(*x == y)) -> bool
            requires(concepts::equality_comparable<T>)
        {
            return *x == y;
        }

        friend constexpr auto operator==(Ref x, Ref<const_type> y) noexcept(noexcept(*x == *y)) -> bool
            requires(!concepts::is_const<T> && concepts::equality_comparable_with<T, const_type>)
        {
            return *x == *y;
        }

        friend constexpr auto operator<=>(Ref x, Ref y) noexcept(noexcept(*x <=> *y))
            requires(concepts::three_way_comparable<T>)
        {
            using R = std::compare_three_way_result_t<T, T>;
            using C = std::common_comparison_category_t<R, std::strong_ordering>;

            return static_cast<C>(*x <=> *y);
        }

        friend constexpr auto operator<=>(Ref x, const T& y) noexcept(noexcept(*x <=> y))
            requires(concepts::three_way_comparable<T>)
        {
            using R = std::compare_three_way_result_t<T, T>;
            using C = std::common_comparison_category_t<R, std::strong_ordering>;

            return static_cast<C>(*x <=> y);
        }

        friend constexpr auto operator<=>(Ref x, Ref<const_type> y) noexcept(noexcept(*x <=> *y))
            requires(!concepts::is_const<T> && concepts::three_way_comparable_with<T, const_type>)
        {
            using R = std::compare_three_way_result_t<T, const_type>;
            using C = std::common_comparison_category_t<R, std::strong_ordering>;

            return static_cast<C>(*x <=> *y);
        }

      private:
        /**
         * 参照先ポインタ
         */
        ptr_type ptr;
    };
} // namespace types::core

using types::core::Ref;
} // namespace mayo
