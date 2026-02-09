#pragma once

#include "mayo/types/concepts/std.hpp"
#include <type_traits>

namespace mayo::concepts {
    template<class F, class... Args>
    concept predicate_for = (concepts::predicate<std::remove_cvref_t<F>&, Args...>);
}
