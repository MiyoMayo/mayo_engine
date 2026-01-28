#pragma once

#include "mayo/memory/address.hpp"
#include "mayo/memory/construct.hpp"
#include "mayo/memory/destroy.hpp"
#include "mayo/types/core/basic_types.hpp"
#include "mayo/types/core/byte.hpp"
#include "mayo/utility/place_tag.hpp"
#include "mayo/utility/transfer.hpp"
#include <cassert>
#include <cstddef>
#include <string>

namespace mayo {
namespace types::container {
    template <class T>
    class Option {
      private:
        union Storage {
            constexpr Storage(Valueless) noexcept
                : dummy{} {}

            template <class... Args>
            constexpr Storage(InPlace, Args&&... args)
                : value{mayo::forward<Args>(args)...} {}

            constexpr Storage(const Storage&) = default;
            constexpr Storage(Storage&&) = default;
            constexpr Storage& operator=(const Storage&) = default;
            constexpr Storage& operator=(Storage&&) = default;
            constexpr ~Storage() {}

            T value;
            Byte dummy;
        };

      public:
        constexpr Option()
            : storage{VALUELESS}
            , has_value{false} {}

        constexpr Option(T&& value)
            : storage{IN_PLACE, mayo::forward<T>(value)}
            , has_value{true} {}

        constexpr Option(const Option&) = default;
        constexpr Option(Option&&) = default;
        constexpr Option& operator=(const Option&) = default;
        constexpr Option& operator=(Option&&) = default;

        constexpr ~Option() {
            destroy();
        }

        constexpr auto is_some() const noexcept -> bool {
            return has_value;
        }

        constexpr auto is_none() const noexcept -> bool {
            return !has_value;
        }

        template <class Self>
        constexpr auto unwrap(this Self&& self) -> decltype(auto) {
            assert(self.has_value);
            return mayo::forward_like<Self>(self.storage.value);
        }

        template <class Self>
        constexpr auto operator*(this Self&& self) -> decltype(auto) {
            return mayo::forward_like<Self>(self.unwrap());
        }

        constexpr operator bool() const noexcept {
            return has_value;
        }

      private:
        template <class... Args>
        constexpr auto emplace(Args&&... args) -> T& {
            destroy();

            auto* ptr
                = mayo::construct_at(mayo::addressof(storage.value), mayo::forward<Args>(args)...);
            has_value = true;

            return *ptr;
        }

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

using types::container::Option;

constexpr auto check() -> bool {
    constexpr auto O0 = Option<i32>{};
    static_assert(O0.is_none());
    static_assert(!O0.is_some());

    constexpr auto O1 = Option<i32>{1};
    static_assert(*O1 == 1);

    constexpr auto O2 = Option<std::string>{""};
    static_assert(O2.unwrap() == "");

    auto o3 = Option<usize>{32uz};

    auto o4 = o3;
    assert(o3);
    auto o5 = move(o3);
    assert(o4);

    auto o6 = Option<std::string>{""};
    assert(o6.is_some());
    // auto o7 = std::move(o6);

    return true;
}
static_assert(check(), "check failed");
} // namespace mayo
