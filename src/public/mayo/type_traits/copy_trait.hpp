#pragma once

#include "mayo/concepts/cv.hpp"
#include "mayo/concepts/ref.hpp"
#include "mayo/type_traits/remove_cvref.hpp"
#include "mayo/types/core/conditional.hpp"

namespace mayo {
namespace type_traits {
    template <class Original, class Target>
    using copy_const = conditional<concepts::is_const<Original>, const Target, Target>;

    template <class Original, class Target>
    using override_ref
        = conditional<concepts::is_rvalue_ref<Original>, remove_ref<Target>&&, Target&>;

    template <class Original, class T>
    using forward_like_type
        = override_ref<Original&&, copy_const<remove_ref<Original>, remove_ref<T>>>;
}; // namespace type_traits

using type_traits::copy_const;
using type_traits::override_ref;
using type_traits::forward_like_type;
} // namespace mayo
