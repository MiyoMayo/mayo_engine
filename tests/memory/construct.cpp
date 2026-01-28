#include <mayo/memory/construct.hpp>
#include <mayo/memory/destroy.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <cassert>

struct Widget {
    int a;
    int b;

    Widget(int x, int y)
        : a{x}
        , b{y} {}
};

auto main() -> mayo::i32 {
    alignas(Widget) unsigned char buffer[sizeof(Widget)];
    auto* raw = reinterpret_cast<Widget*>(buffer);

    auto* ptr = mayo::construct_at(raw, 1, 2);
    assert(ptr == raw);
    assert(ptr->a == 1);
    assert(ptr->b == 2);
    assert(ptr->a != 0);

    mayo::destroy_at(ptr);

    return 0;
}
