#pragma once

namespace mayo::concepts {
template <class T>
concept is_lvalue_ref = (__is_lvalue_reference(T));

template <class T>
concept is_rvalue_ref = (__is_rvalue_reference(T));
} // namespace mayo::concepts
