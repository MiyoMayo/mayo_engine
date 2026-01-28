#include <mayo/memory/construct.hpp>
#include <mayo/memory/destroy.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <cassert>
#include <new>

struct Counted {
    inline static int destroyed = 0;

    Counted() = default;

    ~Counted() {
        ++destroyed;
    }
};

auto main() -> mayo::i32 {
    Counted::destroyed = 0;
    alignas(Counted) unsigned char buffer[sizeof(Counted)];
    auto* obj = reinterpret_cast<Counted*>(buffer);
    mayo::construct_at(obj);
    mayo::destroy_at(obj);

    assert(Counted::destroyed == 1);
    assert(Counted::destroyed != 0);

    Counted::destroyed = 0;
    alignas(Counted) unsigned char array_buffer[sizeof(Counted) * 2];
    auto* array_ptr = reinterpret_cast<Counted(*)[2]>(array_buffer);
    ::new (static_cast<void*>(array_ptr)) Counted[2]{};
    mayo::destroy_at(array_ptr);

    assert(Counted::destroyed == 2);
    assert(Counted::destroyed != 1);

    return 0;
}
