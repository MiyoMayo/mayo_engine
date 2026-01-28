#pragma once

#include "mayo/concepts/destruct.hpp"
#include "mayo/concepts/literals.hpp"
#include "mayo/memory/address.hpp"
#include <cassert>

namespace mayo {
namespace memory {
    template <class T>
    constexpr auto destroy_at(T* ptr) -> void {
        assert(ptr != nullptr);

        if constexpr (concepts::is_trivially_destructible<T>) {
            return;
        }

        ptr->~T();
    }

    template <concepts::is_array T>
    constexpr auto destroy_at(T* ptr) -> void {
        assert(ptr != nullptr);

        if constexpr (concepts::is_trivially_destructible<T>) {
            return;
        }

        for (auto&& v : *ptr) {
            destroy_at(addressof(v));
        }
    }
} // namespace memory

using memory::destroy_at;
} // namespace mayo
