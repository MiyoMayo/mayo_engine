#include <mayo/types/container/option.hpp>
#include <mayo/types/core/basic_types.hpp>
#include <mayo/utility/transfer.hpp>

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

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

struct NoThrowMove {
    NoThrowMove() = default;
    NoThrowMove(NoThrowMove&&) noexcept = default;
    auto operator=(NoThrowMove&&) noexcept -> NoThrowMove& = default;
};

struct ThrowMove {
    ThrowMove() = default;
    ThrowMove(ThrowMove&&) noexcept(false) {}
};

auto main() -> mayo::i32 {
    {
        constexpr mayo::Option<mayo::i32> NONE{};
        static_assert(NONE.is_none());
        static_assert(!NONE.is_some());
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        assert(none.is_none());
        assert(!none.is_some());
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
        mayo::Option<mayo::i32>& b{a};
        assert(*b == 42);

        mayo::Option<mayo::i32> c{mayo::move(a)};
        assert(c.is_some());
        assert(*c == 42);

        mayo::Option<mayo::i32> d{mayo::NONE};
        d = *b;
        assert(d.is_some());
        assert(*d == 42);

        mayo::Option<mayo::i32> e{mayo::NONE};
        e = mayo::move(b);
        assert(e.is_some());
        assert(*e == 42);

        e = mayo::NONE;
        assert(e.is_none());

        e = 7;
        assert(e.is_some());
        assert(*e == 7);
    }

    {
        static_assert(std::is_constructible_v<mayo::Option<std::string>, const char*>);
        static_assert(!std::is_convertible_v<const char*, mayo::Option<std::string>>);
        static_assert(!std::is_constructible_v<mayo::Option<mayo::i32>, std::string>);
        static_assert(std::is_assignable_v<mayo::Option<std::string>&, const char*>);
        static_assert(!std::is_assignable_v<mayo::Option<mayo::i32>&, std::string>);
        static_assert(std::is_assignable_v<mayo::Option<mayo::i32>&, mayo::types::container::None>);

        static_assert(
            noexcept(mayo::Option<NoThrowMove>{std::declval<mayo::Option<NoThrowMove>&&>()}));
        static_assert(
            !noexcept(mayo::Option<ThrowMove>{std::declval<mayo::Option<ThrowMove>&&>()}));

        static_assert(noexcept(std::declval<mayo::Option<NoThrowMove>&>()
            = std::declval<mayo::Option<NoThrowMove>&&>()));
        static_assert(!noexcept(
            std::declval<mayo::Option<ThrowMove>&>() = std::declval<mayo::Option<ThrowMove>&&>()));
    }

    return 0;
}
