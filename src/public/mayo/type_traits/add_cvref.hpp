#pragma once

namespace mayo {
namespace type_traits {
    /**
     * 左辺値参照を付与した型
     */
    template <class T>
    using add_lvalue_ref = __add_lvalue_reference(T);

    /**
     * 右辺値参照を付与した型
     */
    template <class T>
    using add_rvalue_ref = __add_rvalue_reference(T);
} // namespace type_traits

using type_traits::add_lvalue_ref;
using type_traits::add_rvalue_ref;
} // namespace mayo
