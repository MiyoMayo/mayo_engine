#include <mayo/types/core/byte.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(std::is_enum_v<mayo::Byte>);
    static_assert(std::is_same_v<std::underlying_type_t<mayo::Byte>, mayo::u8>);
    static_assert(!std::is_same_v<std::underlying_type_t<mayo::Byte>, mayo::u16>);

    return 0;
}
