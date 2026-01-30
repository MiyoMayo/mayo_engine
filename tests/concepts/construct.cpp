#include <mayo/concepts/construct.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <string>
#include <type_traits>

struct NoCopy {
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;
    NoCopy(NoCopy&&) = default;
};

struct ThrowCopy {
    ThrowCopy() = default;
    ThrowCopy(const ThrowCopy&) noexcept(false) {}
};

struct ThrowMove {
    ThrowMove() = default;
    ThrowMove(ThrowMove&&) noexcept(false) {}
};

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_constructible<int, int>);
    static_assert(!mayo::concepts::is_constructible<int, std::string>);

    static_assert(mayo::concepts::is_copy_constructible<int>);
    static_assert(!mayo::concepts::is_copy_constructible<NoCopy>);

    static_assert(mayo::concepts::is_move_constructible<int>);
    static_assert(mayo::concepts::is_move_constructible<NoCopy>);

    static_assert(mayo::concepts::is_nothrow_constructible<int, int>);
    static_assert(mayo::concepts::is_nothrow_copy_constructible<int>);
    static_assert(!mayo::concepts::is_nothrow_copy_constructible<ThrowCopy>);

    static_assert(mayo::concepts::is_nothrow_move_constructible<int>);
    static_assert(!mayo::concepts::is_nothrow_move_constructible<ThrowMove>);

    return 0;
}
