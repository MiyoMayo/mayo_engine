#pragma once

#include "mayo/utility/transfer.hpp"
#include <new>

namespace mayo {
namespace memory {

    /**
     * 指定アドレスにオブジェクトを構築する
     *
     * @tparam T 構築する型
     * @param address 構築先のアドレス
     * @param args コンストラクタ引数
     */
    template <class T, class... Args>
    constexpr auto construct_at(T* address, Args&&... args) -> T* {
        return ::new (static_cast<void*>(address)) T{forward<Args>(args)...};
    }

} // namespace memory

using memory::construct_at;
} // namespace mayo
