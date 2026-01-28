#pragma once

namespace mayo::concepts {
    /**
     * const修飾型かを判定する
     */
    template <class T>
    concept is_const = (__is_const(T));
}
