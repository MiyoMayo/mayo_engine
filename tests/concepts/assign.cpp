#include <mayo/concepts/assign.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

struct NoCopyAssign {
    NoCopyAssign& operator=(const NoCopyAssign&) = delete;
};

struct NoMoveAssign {
    NoMoveAssign& operator=(NoMoveAssign&&) = delete;
};

struct ThrowAssign {
    ThrowAssign& operator=(ThrowAssign&&) noexcept(false) {
        return *this;
    }
};

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_assignable<int&, int>);
    static_assert(!mayo::concepts::is_assignable<int&, const char*>);

    static_assert(mayo::concepts::is_copy_assignable<int, int>);
    static_assert(!mayo::concepts::is_copy_assignable<NoCopyAssign, NoCopyAssign>);

    static_assert(mayo::concepts::is_move_assignable<int, int>);
    static_assert(!mayo::concepts::is_move_assignable<NoMoveAssign, NoMoveAssign>);

    static_assert(mayo::concepts::is_nothrow_assignable<int&, int>);
    static_assert(mayo::concepts::is_nothrow_copy_assignable<int, int>);
    static_assert(!mayo::concepts::is_nothrow_move_assignable<ThrowAssign, ThrowAssign>);

    return 0;
}
