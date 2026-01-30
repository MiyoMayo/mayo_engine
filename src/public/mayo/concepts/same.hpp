#pragma once

namespace mayo::concepts {
    /**
     * 同一型かを判定する
     */
    template <class T, class U>
    concept is_same = (__is_same(T, U));

    /**
     * 同一型ではないかを判定する
     */
    template <class T, class U>
    concept is_not_same = (!__is_same(T, U));
} // namespace mayo::concepts
