#include <mayo/concepts/literals.hpp>
#include <mayo/types/core/basic_types.hpp>

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_array<int[3]>);
    static_assert(mayo::concepts::is_array<int[]>);
    static_assert(!mayo::concepts::is_array<int>);

    return 0;
}
