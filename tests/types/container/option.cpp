#include <mayo/types/container/option.hpp>
#include <mayo/types/core/basic_types.hpp>
#include <mayo/utility/transfer.hpp>

#include <cassert>

struct Counted {
    inline static int destroyed = 0;
    int value = 0;

    explicit Counted(int v)
        : value{v} {}

    Counted(const Counted&) = default;
    Counted(Counted&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    ~Counted() {
        ++destroyed;
    }
};

auto main() -> mayo::i32 {
    {
        constexpr mayo::Option<mayo::i32> NONE{};
        static_assert(NONE.is_none());
        static_assert(!NONE.is_some());
    }

    {
        mayo::Option<mayo::i32> some{1};
        assert(some.is_some());
        assert(!some.is_none());
        assert(*some == 1);
        assert(some.unwrap() == 1);
        assert(static_cast<bool>(some));
    }

    Counted::destroyed = 0;
    {
        Counted local{7};
        mayo::Option<Counted> value{mayo::move(local)};
        assert(value);
        assert(value.unwrap().value == 7);
        assert(value.unwrap().value != 0);
    }
    assert(Counted::destroyed == 2);
    assert(Counted::destroyed != 1);

    {
        mayo::Option<mayo::i32> a{42};
        mayo::Option<mayo::i32> b = a;
        assert(*b == 42);

        mayo::Option<mayo::i32> c = mayo::move(a);
        assert(c.is_some());
        assert(*c == 42);
    }

    return 0;
}
