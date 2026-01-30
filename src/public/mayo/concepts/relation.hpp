#pragma once

namespace mayo::concepts {

    /**
     * BaseがTの基底型かを判定する
     */
    template <class Base, class T>
    concept is_base_of = (__is_base_of(Base, T));

    /**
     * FromがToへ暗黙変換可能かを判定する
     */
    template <class From, class To>
    concept is_convertible = (__is_convertible(From, To));

    /**
     * FromがToへ例外なしで変換可能かを判定する
     */
    template <class From, class To>
    concept is_nothrow_convertible = (__is_nothrow_convertible(From, To));
} // namespace mayo::concepts
