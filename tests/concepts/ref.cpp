#include <mayo/concepts/ref.hpp>
#include <mayo/types/core/basic_types.hpp>

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_lvalue_ref<int&>);
    static_assert(!mayo::concepts::is_lvalue_ref<int&&>);
    static_assert(!mayo::concepts::is_lvalue_ref<int>);

    static_assert(mayo::concepts::is_rvalue_ref<int&&>);
    static_assert(!mayo::concepts::is_rvalue_ref<int&>);
    static_assert(!mayo::concepts::is_rvalue_ref<int>);

    return 0;
}
