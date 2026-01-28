#pragma once

namespace mayo::concepts {
template <class T>
concept is_const = (__is_const(T));
}
