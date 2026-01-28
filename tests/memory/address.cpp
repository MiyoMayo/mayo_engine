#include <mayo/memory/address.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <cassert>

template <class T>
concept can_addressof = requires(T&& value) {
    mayo::addressof(static_cast<T&&>(value));
};

auto main() -> mayo::i32 {
    int v = 1;
    auto* ptr = mayo::addressof(v);
    assert(ptr == &v);

    static_assert(can_addressof<int&>);
    static_assert(!can_addressof<int&&>);

    return 0;
}
