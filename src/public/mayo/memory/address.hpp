#pragma once

namespace mayo {
namespace memory {
    /**
     * オブジェクトの実アドレスを取得する
     *
     * @tparam T 対象の型
     * @param ptr 対象オブジェクト
     */
    template <class T>
    constexpr auto addressof(T& ptr) noexcept -> T* {
        return __builtin_addressof(ptr);
    }

    /**
     * 一時オブジェクトのアドレス取得は禁止
     */
    template <class T>
    auto addressof(const T&&) noexcept
        -> T* = delete ("一時オブジェクトのアドレスは取得できません");
} // namespace memory

using memory::addressof;
} // namespace mayo
