#include <mayo/types/core/conditional.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(std::is_same_v<mayo::conditional<true, int, float>, int>);
    static_assert(std::is_same_v<mayo::conditional<false, int, float>, float>);
    static_assert(!std::is_same_v<mayo::conditional<true, int, float>, float>);

    return 0;
}
