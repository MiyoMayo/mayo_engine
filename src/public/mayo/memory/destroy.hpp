#pragma once

#include "mayo/concepts/destruct.hpp"
#include "mayo/concepts/literals.hpp"
#include "mayo/memory/address.hpp"
#include <cassert>

namespace mayo {
namespace memory {
    /**
     * 指定アドレスのオブジェクトを破棄する
     *
     * @tparam T 破棄する型
     * @param ptr 対象アドレス
     */
    template <class T>
    constexpr auto destroy_at(T* ptr) -> void {
        assert(ptr != nullptr);

        if constexpr (concepts::is_trivially_destructible<T>) {
            return;
        }

        ptr->~T();
    }

    /**
     * 配列オブジェクトを要素ごとに破棄する
     *
     * @tparam T 配列型
     * @param ptr 対象アドレス
     */
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
