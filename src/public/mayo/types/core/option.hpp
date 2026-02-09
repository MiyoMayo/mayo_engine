#pragma once

#include "mayo/types/concepts.hpp"
#include "mayo/types/concepts/functional.hpp"
#include "mayo/types/core/numeric.hpp"
#include <cassert>
#include <compare>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace mayo {
namespace types::core {
    template <class T>
    class Option;
}

namespace concepts {
    namespace detail {
        template <class>
        struct IsOptionType : std::false_type {};

        template <class V>
        struct IsOptionType<types::core::Option<V>> : std::true_type {};
    } // namespace detail

    template <class T>
    concept is_option_type = (detail::IsOptionType<std::remove_cvref_t<T>>::value);
} // namespace concepts

namespace types::core {

    /**
     * 値なしを表すタグ
     */
    struct None {
        explicit constexpr None() noexcept = default;
    };

    /**
     * 値なしタグの定数
     */
    inline constexpr auto NONE = None{};

    /**
     * 値があるかもしれないコンテナ
     *
     * @tparam T 格納する型
     */
    template <class T>
    class Option {
      private:
        /**
         * 値の生存領域
         */
        union Storage {
            constexpr Storage() noexcept
                : dummy{} {}

            template <class... Args>
            constexpr Storage(std::in_place_t, Args&&... args)
                : value{std::forward<Args>(args)...} {}

            constexpr Storage(const Storage&) = delete;
            constexpr Storage(Storage&&) = delete;
            constexpr Storage& operator=(const Storage&) = delete;
            constexpr Storage& operator=(Storage&&) = delete;
            constexpr ~Storage() {}

            T value;
            std::byte dummy;
        };

      public:
        /**
         * 値なしで構築する
         */
        explicit constexpr Option() noexcept
            : storage{}
            , has_value{false} {}

        /**
         * 値なしを明示して構築する
         */
        constexpr Option(None) noexcept
            : storage{}
            , has_value{false} {}

        /**
         * コピー構築する
         */
        constexpr Option(const Option& other)
            requires(concepts::copy_constructible<T>)
            : storage{}
            , has_value{false} {
            init_from_other(other);
        }

        /**
         * ムーブ構築する
         */
        constexpr Option(Option&& other) noexcept(concepts::is_nothrow_move_constructible<T>)
            requires(concepts::move_constructible<T>)
            : storage{}
            , has_value{false} {
            init_from_other(std::move(other));
        }

        /**
         * 値を変換構築する
         *
         * @tparam U 変換元の型
         * @param value 格納する値
         */
        template <concepts::convertible_to<T> U>
        explicit constexpr Option(U&& value)
            : storage{std::in_place, std::forward<U>(value)}
            , has_value{true} {}

        /**
         * 値を破棄する
         */
        constexpr ~Option() {
            destroy();
        }

        /**
         * 値なしにする
         */
        constexpr auto operator=(None) noexcept -> Option& {
            if (!has_value) {
                return *this;
            }

            destroy();
            return *this;
        }

        /**
         * コピー代入する
         */
        constexpr auto operator=(const Option& other) -> Option&
            requires(concepts::copy_constructible<T>)
        {
            return assign_from_other(other);
        }

        /**
         * ムーブ代入する
         */
        constexpr auto operator=(Option&& other) noexcept(concepts::is_nothrow_move_constructible<T>) -> Option&
            requires(concepts::move_constructible<T>)
        {
            return assign_from_other(std::move(other));
        }

        /**
         * 値を変換代入する
         *
         * @tparam U 変換元の型
         * @param value 代入する値
         */
        template <concepts::convertible_to<T> U>
        constexpr auto operator=(U&& value) -> Option& {
            // o = *oのような自己参照代入は処理しない
            if constexpr (concepts::is_lvalue_ref<U&&> && concepts::same_as<std::remove_cvref_t<U>, T>) {
                if (has_value && std::addressof(storage.value) == std::addressof(value)) {
                    return *this;
                }
            }

            if (has_value) {
                // 既存値があって代入可能なら、再構築せず代入のみ行う
                if constexpr (concepts::assignable_from<T&, U&&>) {
                    storage.value = std::forward<U>(value);
                } else {
                    // 代入不可なら、先に一時を作ってから再構築する（破棄後参照の回避）
                    T tmp{std::forward<U>(value)};
                    emplace(std::move(tmp));
                }
            } else {
                // Noneの場合は新規構築する
                emplace(std::forward<U>(value));
            }

            return *this;
        }

        /**
         * 値参照の取得
         *
         * @note 値が無い場合は未定義
         */
        template <class Self>
        constexpr auto operator*(this Self&& self) -> decltype(auto) {
            return std::forward_like<Self>(self.storage.value);
        }

        /**
         * 値ポインタの取得
         *
         * @note 値が無い場合はassertで停止する
         */
        template <class Self>
        constexpr auto operator->(this Self& self) noexcept -> auto {
            assert(self.has_value);
            return std::addressof(self.storage.value);
        }

        /**
         * 値の有無をboolとして取得
         */
        explicit constexpr operator bool() const noexcept {
            return has_value;
        }

        template <class Self>
        constexpr auto begin(this Self& self) noexcept {
            using Ptr = std::add_pointer_t<std::remove_reference_t<decltype(self.storage.value)>>;
            return self.has_value ? std::addressof(self.storage.value) : static_cast<Ptr>(nullptr);
        }

        template <class Self>
        constexpr auto end(this Self& self) noexcept {
            auto begin = self.begin();
            return self.has_value ? begin + 1 : begin;
        }

        /**
         * 値が存在するか
         */
        constexpr auto is_some() const noexcept -> bool {
            return has_value;
        }

        template <class Self, class Pred>
        constexpr auto is_some_and(this Self&& self, Pred&& pred) -> bool
            requires(concepts::predicate_for<Pred, decltype(std::forward_like<Self>(self.storage.value))>)
        {
            if (!self.has_value) {
                return false;
            }

            return static_cast<bool>(std::invoke(std::forward<Pred>(pred), std::forward_like<Self>(self.storage.value)));
        }

        /**
         * 値が存在しないか
         */
        constexpr auto is_none() const noexcept -> bool {
            return !has_value;
        }

        template <class Self, class Pred>
        constexpr auto is_none_or(this Self&& self, Pred&& pred) -> bool
            requires(concepts::predicate_for<Pred, decltype(std::forward_like<Self>(self.storage.value))>)
        {
            if (!self.has_value) {
                return true;
            }

            return static_cast<bool>(std::invoke(std::forward<Pred>(pred), std::forward_like<Self>(self.storage.value)));
        }

        /**
         * 値を取り出す
         *
         * @note 値が無い場合はassertで停止する
         */
        template <class Self>
        constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            assert(self.has_value);
            return std::forward_like<Self>(self.storage.value);
        }

      private:
        /**
         * 値を再構築する
         */
        template <class... Args>
        constexpr auto emplace(Args&&... args) -> T& {
            destroy();

            auto* ptr = std::construct_at(std::addressof(storage.value), std::forward<Args>(args)...);
            has_value = true;

            return *ptr;
        }

        /**
         * 同型Optionから構築する
         */
        template <class Other>
        constexpr auto init_from_other(Other&& other) -> void
            requires(concepts::same_as<Option, std::remove_cvref_t<Other>>)
        {
            if (!other.has_value) {
                return;
            }

            emplace(std::forward_like<Other>(other.storage.value));
        }

        /**
         * 同型Optionから代入する
         */
        template <class Other>
        constexpr auto assign_from_other(Other&& other) -> Option&
            requires(concepts::same_as<Option, std::remove_cvref_t<Other>>)
        {
            // 自己代入は何もしない
            if (std::addressof(*this) == std::addressof(other)) {
                return *this;
            }

            if (has_value && other.has_value) {
                // `Some` <- `Some`: 代入可能なら代入、不可なら再構築する
                if constexpr (concepts::assignable_from<T&, decltype(std::forward_like<Other>(other.storage.value))>) {
                    storage.value = std::forward_like<Other>(other.storage.value);
                } else {
                    destroy();
                    emplace(std::forward_like<Other>(other.storage.value));
                }
            } else if (has_value && !other.has_value) {
                // `Some` <- `None`: 値を破棄する
                destroy();
            } else if (!has_value && other.has_value) {
                // `None` <- `Some`: 新規構築する
                emplace(std::forward_like<Other>(other.storage.value));
            }

            return *this;
        }

        /**
         * 値があれば破棄する
         */
        constexpr auto destroy() noexcept -> void {
            if (!has_value) {
                return;
            }

            if (!concepts::is_trivially_destructible<T>) {
                std::destroy_at(std::addressof(storage.value));
            }

            std::construct_at(std::addressof(storage.dummy), std::byte{});
            has_value = false;
        }

      public:
        /**
         * 値との等値比較（Optionと値）
         */
        template <class U>
        constexpr auto operator==(const U& y) const noexcept(noexcept(std::declval<const T&>() == std::declval<const U&>())) -> bool
            requires(!concepts::is_option_type<U> && concepts::equality_comparable_with<T, U>)
        {
            if (is_none()) {
                return false;
            }

            return **this == y;
        }

        /**
         * Option同士の等値比較
         */
        template <class U>
        friend constexpr auto operator==(const Option& x, const Option<U>& y) noexcept(noexcept(*x == *y)) -> bool
            requires(concepts::equality_comparable_with<T, U>)
        {
            if (x.is_none() && y.is_none()) {
                return true;
            } else if (x.is_none() || y.is_none()) {
                return false;
            }

            return *x == *y;
        }

        /**
         * Noneとの等値比較
         */
        friend constexpr auto operator==(const Option& x, None) noexcept -> bool {
            return x.is_none();
        }

        /**
         * Option同士の三方比較
         */
        template <class U>
        friend constexpr auto operator<=>(const Option& x, const Option<U>& y) noexcept(noexcept(*x <=> *y))
            requires(concepts::three_way_comparable_with<T, U>)
        {
            using R = std::compare_three_way_result_t<T, U>;
            using C = std::common_comparison_category_t<R, std::strong_ordering>;

            if (x.is_none() || y.is_none()) {
                return static_cast<C>(x.is_some() <=> y.is_some());
            }

            return static_cast<C>(*x <=> *y);
        }

        /**
         * 値との三方比較（Optionと値）
         */
        template <class U>
        constexpr auto operator<=>(const U& y) const noexcept(noexcept(std::declval<const T&>() <=> std::declval<const U&>()))
            requires(!concepts::is_option_type<U> && concepts::three_way_comparable_with<T, U>)
        {
            using R = std::compare_three_way_result_t<T, U>;
            using C = std::common_comparison_category_t<R, std::strong_ordering>;

            if (is_none()) {
                return static_cast<C>(false <=> true);
            }

            return static_cast<C>(**this <=> y);
        }

        /**
         * Noneとの三方比較
         */
        friend constexpr auto operator<=>(const Option& x, None) noexcept -> std::strong_ordering {
            return x.is_some() <=> false;
        }

      private:
        Storage storage;
        bool has_value;
    };
} // namespace types::core

using types::core::NONE;
using types::core::Option;

/**
 * Optionの簡易チェック
 */
constexpr auto check() -> bool {
    constexpr auto O0 = Option<i32>{};
    static_assert(O0.is_none());
    static_assert(!O0.is_some());

    constexpr auto O1 = Option<i32>{1};
    static_assert(*O1 == 1);

    constexpr auto O2 = Option<std::string>{""};
    static_assert(O2.unwrap() == "");

    auto o3 = Option<usize>{32uz};

    auto o4 = Option{o3};
    assert(o3);
    auto o5 = Option{std::move(o3)};
    assert(o4);

    auto o6 = Option<std::string>{""};
    assert(o6.is_some());
    // auto o7 = std::move(o6);

    constexpr auto S = "0";
    constexpr auto O8 = Option<std::string>{S};

    constexpr auto O9 = Option<std::string>{O8};

    constexpr auto OO0 = Option<std::string>{};
    constexpr auto OO1 = Option<std::string>{""};
    constexpr auto OO2 = Option<std::string>{OO0};

    auto& o_o3 = OO1;
    assert(o_o3.is_some());

    auto o = Option<std::string>{""};
    auto s = o->begin();
    const auto CO = Option{o};
    auto cs = CO->begin();

    return true;
}
static_assert(check(), "check failed");
} // namespace mayo
