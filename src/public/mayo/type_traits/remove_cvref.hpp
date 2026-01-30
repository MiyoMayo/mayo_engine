#pragma once

namespace mayo {
namespace type_traits {
    /**
     * 参照修飾を除去した型
     */
    template <class T>
    using remove_ref = __remove_reference_t(T);

    /**
     * const/volatile修飾を除去した型
     */
    template <class T>
    using remove_cv = __remove_cv(T);

    /**
     * const/volatile/参照修飾を除去した型
     */
    template <class T>
    using remove_cvref = remove_cv<remove_ref<T>>;
} // namespace type_traits

using type_traits::remove_ref;
using type_traits::remove_cv;
using type_traits::remove_cvref;
}; // namespace mayo
