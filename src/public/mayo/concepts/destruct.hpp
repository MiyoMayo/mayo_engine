#pragma once

namespace mayo::concepts {
template <class T>
concept is_trivially_destructible = (__is_trivially_destructible(T));
}
