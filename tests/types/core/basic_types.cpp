#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(sizeof(mayo::i8) == 1);
    static_assert(sizeof(mayo::u8) == 1);
    static_assert(sizeof(mayo::i16) == 2);
    static_assert(sizeof(mayo::u16) == 2);
    static_assert(sizeof(mayo::i32) == 4);
    static_assert(sizeof(mayo::u32) == 4);
    static_assert(sizeof(mayo::i64) == 8);
    static_assert(sizeof(mayo::u64) == 8);

    static_assert(std::is_same_v<mayo::isize, std::ptrdiff_t>);
    static_assert(std::is_same_v<mayo::usize, std::size_t>);
    static_assert(!std::is_same_v<mayo::i32, mayo::i16>);
    static_assert(!std::is_same_v<mayo::u32, mayo::u64>);

    static_assert(!std::is_same_v<mayo::f32, mayo::f64>);

    return 0;
}
