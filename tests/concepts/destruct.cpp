#include <mayo/concepts/destruct.hpp>
#include <mayo/types/core/basic_types.hpp>

struct NonTrivial {
    ~NonTrivial() {}
};

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_trivially_destructible<int>);
    static_assert(!mayo::concepts::is_trivially_destructible<NonTrivial>);

    return 0;
}
