#pragma once

namespace mayo::concepts {
    /**
     * 破棄が自明かを判定する
     */
    template <class T>
    concept is_trivially_destructible = (__is_trivially_destructible(T));
}
