#include <mayo/concepts/same.hpp>
#include <mayo/types/core/basic_types.hpp>

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_same<int, int>);
    static_assert(!mayo::concepts::is_same<int, float>);

    static_assert(mayo::concepts::is_not_same<int, float>);
    static_assert(!mayo::concepts::is_not_same<int, int>);

    return 0;
}
