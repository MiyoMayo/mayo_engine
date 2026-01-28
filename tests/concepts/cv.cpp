#include <mayo/concepts/cv.hpp>
#include <mayo/types/core/basic_types.hpp>

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_const<const int>);
    static_assert(!mayo::concepts::is_const<int>);

    return 0;
}
