#pragma once

namespace mayo {
namespace type_traits {
    template <class T>
    using remove_ref = __remove_reference_t(T);

    template <class T>
    using remove_cv = __remove_cv(T);
} // namespace type_traits

using type_traits::remove_ref;
using type_traits::remove_cv;
}; // namespace mayo
