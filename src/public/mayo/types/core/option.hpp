#pragma once

#include "mayo/types/concepts.hpp"
#include "mayo/types/concepts/functional.hpp"
#include "mayo/types/concepts/std.hpp"
#include "mayo/types/core/numeric.hpp"
#include "mayo/types/core/ref.hpp"
#include <cassert>
#include <compare>
#include <cstddef>
#include <functional>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
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
        [[nodiscard]]
        constexpr auto operator*(this Self&& self) -> decltype(auto) {
            return std::forward_like<Self>(self.storage.value);
        }

        /**
         * 値ポインタの取得
         *
         * @note 値が無い場合は例外を送出する
         */
        template <class Self>
        constexpr auto operator->(this Self& self) -> auto {
            if (!self.has_value) {
                throw std::runtime_error{"Optionに値がありません。"};
            }

            return std::addressof(self.storage.value);
        }

        /**
         * 値の有無をboolとして取得
         */
        [[nodiscard]]
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
        [[nodiscard]]
        constexpr auto is_some() const noexcept -> bool {
            return has_value;
        }

        template <class Self, class Pred>
        [[nodiscard]]
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
        [[nodiscard]]
        constexpr auto is_none() const noexcept -> bool {
            return !has_value;
        }

        template <class Self, class Pred>
        [[nodiscard]]
        constexpr auto is_none_or(this Self&& self, Pred&& pred) -> bool
            requires(concepts::predicate_for<Pred, decltype(std::forward_like<Self>(self.storage.value))>)
        {
            if (!self.has_value) {
                return true;
            }

            return static_cast<bool>(std::invoke(std::forward<Pred>(pred), std::forward_like<Self>(self.storage.value)));
        }

        /**
         * 値へのconst参照ラッパーを返す
         *
         * - Some(T) -> Some(Ref<const T>)
         * - None    -> None
         */
        [[nodiscard]]
        constexpr auto as_ref(this const Option& self) noexcept -> Option<Ref<const T>> {
            if (!self.has_value) {
                return NONE;
            }

            return Option<Ref<const T>>{Ref<const T>{self.storage.value}};
        }

        // 一時オブジェクトからの参照取り出しを防ぐ
        constexpr auto as_ref(this Option&&) = delete;
        constexpr auto as_ref(this const Option&&) = delete;

        /**
         * 値への可変参照ラッパーを返す
         *
         * - Some(T) -> Some(Ref<T>)
         * - None    -> None
         */
        [[nodiscard]]
        constexpr auto as_mut(this Option& self) noexcept -> Option<Ref<T>>
            requires(!concepts::is_const<T>)
        {
            if (!self.has_value) {
                return NONE;
            }

            return Option<Ref<T>>{Ref<T>{self.storage.value}};
        }

        // 一時オブジェクトからの参照取り出しを防ぐ
        constexpr auto as_mut(this Option&&) = delete;
        constexpr auto as_mut(this const Option&&) = delete;

        /**
         * 値を長さ0/1のconstスライスとして返す
         *
         * - Some(T) -> span<const T>{&value, 1}
         * - None    -> 空span
         */
        [[nodiscard]]
        constexpr auto as_slice(this const Option& self) noexcept -> std::span<const T> {
            if (!self.has_value) {
                return std::span<const T>{};
            }

            return std::span<const T>{std::addressof(self.storage.value), 1};
        }

        // 一時オブジェクトからのスライス取り出しを防ぐ
        constexpr auto as_slice(this Option&&) = delete;
        constexpr auto as_slice(this const Option&&) = delete;

        /**
         * 値を長さ0/1の可変スライスとして返す
         *
         * - Some(T) -> span<T>{&value, 1}
         * - None    -> 空span
         */
        [[nodiscard]]
        constexpr auto as_mut_slice(this Option& self) noexcept -> std::span<T>
            requires(!concepts::is_const<T>)
        {
            if (!self.has_value) {
                return std::span<T>{};
            }

            return std::span<T>{std::addressof(self.storage.value), 1};
        }

        // 一時オブジェクトからのスライス取り出しを防ぐ
        constexpr auto as_mut_slice(this Option&&) = delete;
        constexpr auto as_mut_slice(this const Option&&) = delete;

        /**
         * 値を取り出す。Noneなら指定メッセージで例外を送出する
         */
        template <class Self>
        constexpr auto expect(this Self&& self, std::string_view msg) -> decltype(auto) {
            if (!self.has_value) {
                // string_viewの範囲をそのまま例外メッセージへコピーする
                throw std::runtime_error{std::string{msg}};
            }

            return std::forward_like<Self>(self.storage.value);
        }

        /**
         * 値を取り出す
         *
         * @note 値が無い場合は例外を送出する
         */
        template <class Self>
        [[nodiscard("unwrapの戻り値を捨てる検証は避けて、検証目的ならexpectを使ってください。")]]
        constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            if (!self.has_value) {
                throw std::runtime_error{"Optionに値がありません。"};
            }

            return std::forward_like<Self>(self.storage.value);
        }

        /**
         * 値を取り出す。Noneなら代替値を返す
         *
         * - Some(T) -> thisの値カテゴリに応じて値を返す
         * - None    -> fallback を返す
         */
        template <class Self>
        [[nodiscard]]
        constexpr auto unwrap_or(this Self&& self, T fallback) -> T
            requires(concepts::constructible_from<T, decltype(std::forward_like<Self>(self.storage.value))>)
        {
            return self.has_value ? static_cast<T>(std::forward_like<Self>(self.storage.value)) : std::move(fallback);
        }

        /**
         * 値を取り出す。Noneなら遅延評価で代替値を生成して返す
         */
        template <class Self, class F>
        [[nodiscard]]
        constexpr auto unwrap_or_else(this Self&& self, F&& fallback) -> T
            requires(concepts::constructible_from<T, decltype(std::forward_like<Self>(self.storage.value))> && concepts::is_invocable_result_convertible<T, F>)
        {
            return self.has_value ? static_cast<T>(std::forward_like<Self>(self.storage.value)) : std::invoke(std::forward<F>(fallback));
        }

        /**
         * 値を取り出す。Noneならデフォルト値を返す
         */
        template <class Self>
        [[nodiscard]]
        constexpr auto unwrap_or_default(this Self&& self) -> T
            requires(concepts::default_initializable<T> && concepts::constructible_from<T, decltype(std::forward_like<Self>(self.storage.value))>)
        {
            return self.has_value ? static_cast<T>(std::forward_like<Self>(self.storage.value)) : T{};
        }

        /**
         * 値をチェックせずに取り出す
         *
         * @note Noneで呼ぶと未定義動作
         */
        template <class Self>
        [[nodiscard]]
        constexpr auto unwrap_unchecked(this Self&& self) noexcept -> decltype(auto) {
            return std::forward_like<Self>(self.storage.value);
        }

        /**
         * 関数で値を変換する
         *
         * - Some(T) -> Some(f(T&&))
         * - None    -> None
         */
        template <class F>
        [[nodiscard]]
        constexpr auto map(this Option&& self, F&& f) -> Option<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>
            requires(concepts::is_invocable_result_convertible<std::remove_cvref_t<std::invoke_result_t<F, T&&>>, F, T&&>
                && concepts::is_object<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>)
        {
            using U = std::remove_cvref_t<std::invoke_result_t<F, T&&>>;

            if (!self.has_value) {
                return NONE;
            }

            return Option<U>{std::invoke(std::forward<F>(f), std::forward<T>(self.storage.value))};
        }

        /**
         * 値を観察する副作用フック
         *
         * - Some(T) -> f(const T&) を呼んで Some(T) を返す
         * - None    -> 何も呼ばずに None を返す
         */
        template <class F>
        constexpr auto inspect(this Option&& self, F&& f) -> Option<T>
            requires(concepts::invocable<F, const T&>)
        {
            if (self.has_value) {
                std::invoke(std::forward<F>(f), std::as_const(self.storage.value));
            }

            return std::move(self);
        }

        /**
         * 値を変換して取り出す。Noneなら代替値を返す
         */
        template <class U, class F>
        [[nodiscard]]
        constexpr auto map_or(this Option&& self, U fallback, F&& f) -> U
            requires(concepts::is_object<U> && concepts::is_invocable_result_convertible<U, F, T&&>)
        {
            if (!self.has_value) {
                return std::move(fallback);
            }

            return static_cast<U>(std::invoke(std::forward<F>(f), std::forward<T>(self.storage.value)));
        }

        /**
         * 値を変換して取り出す。Noneなら遅延評価で代替値を返す
         */
        template <class D, class F>
        [[nodiscard]]
        constexpr auto map_or_else(this Option&& self, D&& fallback, F&& f) -> std::remove_cvref_t<std::invoke_result_t<F, T&&>>
            requires(concepts::is_invocable_result_convertible<std::remove_cvref_t<std::invoke_result_t<F, T&&>>, F, T&&>
                && concepts::is_invocable_result_convertible<std::remove_cvref_t<std::invoke_result_t<F, T&&>>, D>
                && concepts::is_object<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>)
        {
            using U = std::remove_cvref_t<std::invoke_result_t<F, T&&>>;
            return self.has_value ? static_cast<U>(std::invoke(std::forward<F>(f), std::forward<T>(self.storage.value)))
                                  : static_cast<U>(std::invoke(std::forward<D>(fallback)));
        }

        /**
         * 値を変換して取り出す。Noneなら変換先型のデフォルト値を返す
         */
        template <class F>
        [[nodiscard]]
        constexpr auto map_or_default(this Option&& self, F&& f) -> std::remove_cvref_t<std::invoke_result_t<F, T&&>>
            requires(concepts::is_invocable_result_convertible<std::remove_cvref_t<std::invoke_result_t<F, T&&>>, F, T&&>
                && concepts::is_object<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>
                && concepts::default_initializable<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>)
        {
            using U = std::remove_cvref_t<std::invoke_result_t<F, T&&>>;

            if (!self.has_value) {
                return U{};
            }

            return static_cast<U>(std::invoke(std::forward<F>(f), std::forward<T>(self.storage.value)));
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
