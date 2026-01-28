#pragma once

namespace mayo::concepts {
    /**
     * 左辺値参照型かを判定する
     */
    template <class T>
    concept is_lvalue_ref = (__is_lvalue_reference(T));

    /**
     * 右辺値参照型かを判定する
     */
    template <class T>
    concept is_rvalue_ref = (__is_rvalue_reference(T));
} // namespace mayo::concepts
