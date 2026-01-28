#pragma once

namespace mayo {
namespace types::core {
    template <bool Condition, class T, class F>
    struct Conditional {
        using type = T;
    };

    template <class T, class F>
    struct Conditional<false, T, F> {
        using type = F;
    };
} // namespace types::core

template <bool Condition, class T, class F>
using conditional = typename types::core::Conditional<Condition, T, F>::type;
} // namespace mayo
