#pragma once

#include "mayo/utility/transfer.hpp"
#include <new>

namespace mayo {
namespace memory {

    template <class T, class... Args>
    constexpr auto construct_at(T* address, Args&&... args) -> T* {
        return ::new (static_cast<void*>(address)) T{forward<Args>(args)...};
    }

} // namespace memory

using memory::construct_at;
} // namespace mayo
