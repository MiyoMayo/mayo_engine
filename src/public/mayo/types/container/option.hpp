#pragma once

#include "mayo/concepts/construct.hpp"
#include "mayo/concepts/relation.hpp"
#include "mayo/concepts/same.hpp"
#include "mayo/memory/address.hpp"
#include "mayo/memory/construct.hpp"
#include "mayo/memory/destroy.hpp"
#include "mayo/type_traits/remove_cvref.hpp"
#include "mayo/types/core/basic_types.hpp"
#include "mayo/types/core/byte.hpp"
#include "mayo/utility/place_tag.hpp"
#include "mayo/utility/transfer.hpp"
#include <cassert>
#include <cstddef>
#include <string>

namespace mayo {
namespace types::container {
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
            constexpr Storage(Valueless) noexcept
                : dummy{} {}

            template <class... Args>
            constexpr Storage(InPlace, Args&&... args)
                : value{mayo::forward<Args>(args)...} {}

            constexpr Storage(const Storage&) = delete;
            constexpr Storage(Storage&&) = delete;
            constexpr Storage& operator=(const Storage&) = delete;
            constexpr Storage& operator=(Storage&&) = delete;
            constexpr ~Storage() {}

            T value;
            Byte dummy;
        };

      public:
        /**
         * 値なしで構築する
         */
        explicit constexpr Option() noexcept
            : storage{VALUELESS}
            , has_value{false} {}

        /**
         * 値なしを明示して構築する
         */
        constexpr Option(None) noexcept
            : storage{VALUELESS}
            , has_value{false} {}

        explicit constexpr Option(const Option& other)
            : storage{VALUELESS}
            , has_value{false} {
            construct_from_other(other);
        }

        explicit constexpr Option(Option&& other)
            noexcept(concepts::is_nothrow_move_constructible<T>)
            : storage{VALUELESS}
            , has_value{false} {
            construct_from_other(mayo::move(other));
        }

        /**
         * 値を変換構築する
         *
         * @tparam U 変換元の型
         * @param value 格納する値
         */
        template <concepts::is_convertible<T> U>
        explicit constexpr Option(U&& value)
            : storage{IN_PLACE, mayo::forward<U>(value)}
            , has_value{true} {}

        constexpr Option& operator=(const Option& other) {
            return construct_from_other(other);
        }

        constexpr Option& operator=(Option&& other) noexcept {
            return construct_from_other(mayo::move(other));
        }

        /**
         * 値を破棄する
         */
        constexpr ~Option() {
            destroy();
        }

        /**
         * 値が存在するか
         */
        constexpr auto is_some() const noexcept -> bool {
            return has_value;
        }

        /**
         * 値が存在しないか
         */
        constexpr auto is_none() const noexcept -> bool {
            return !has_value;
        }

        /**
         * 値を取り出す
         *
         * @note 値が無い場合はassertで停止する
         */
        template <class Self>
        constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            assert(self.has_value);
            return mayo::forward_like<Self>(self.storage.value);
        }

        /**
         * 値参照の取得
         */
        template <class Self>
        constexpr auto operator*(this Self&& self) -> decltype(auto) {
            return mayo::forward_like<Self>(self.unwrap());
        }

        /**
         * 値の有無をboolとして取得
         */
        constexpr operator bool() const noexcept {
            return has_value;
        }

      private:
        /**
         * 値を再構築する
         */
        template <class... Args>
        constexpr auto emplace(Args&&... args) -> T& {
            destroy();

            auto* ptr
                = mayo::construct_at(mayo::addressof(storage.value), mayo::forward<Args>(args)...);
            has_value = true;

            return *ptr;
        }

        /**
         * 同型Optionから構築/代入する
         */
        template <class Other>
        constexpr auto construct_from_other(Other&& other) -> Option&
            requires(concepts::is_same<Option, mayo::remove_cvref<Other>>)
        {
            if (mayo::addressof(*this) == mayo::addressof(other)) {
                return *this;
            }

            if (has_value && other.has_value) {
                storage.value = mayo::forward_like<Other>(other.storage.value);
            } else if (has_value && !other.has_value) {
                destroy();
            } else if (!has_value && other.has_value) {
                emplace(mayo::forward_like<Other>(other.storage.value));
            }

            return *this;
        }

        /**
         * 値があれば破棄する
         */
        constexpr auto destroy() -> void {
            if (!has_value) {
                return;
            }

            mayo::destroy_at(mayo::addressof(storage.value));

            mayo::construct_at(mayo::addressof(storage.dummy), Byte{});
            has_value = false;
        }

      private:
        Storage storage;
        bool has_value;
    };
} // namespace types::container

using types::container::NONE;
using types::container::Option;

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
    auto o5 = Option{move(o3)};
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

    return true;
}
static_assert(check(), "check failed");
} // namespace mayo
