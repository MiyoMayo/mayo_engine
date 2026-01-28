#pragma once

namespace mayo {
namespace memory {
    template <class T>
    constexpr auto addressof(T& ptr) noexcept -> T* {
        return __builtin_addressof(ptr);
    }

    template <class T>
    auto addressof(const T&&) noexcept
        -> T* = delete ("一時オブジェクトのアドレスは取得できません");
} // namespace memory

using memory::addressof;
} // namespace mayo
