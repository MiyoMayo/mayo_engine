#include <mayo/type_traits/add_cvref.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(std::is_same_v<mayo::add_lvalue_ref<int>, int&>);
    static_assert(std::is_same_v<mayo::add_rvalue_ref<int>, int&&>);
    static_assert(std::is_same_v<mayo::add_lvalue_ref<int&>, int&>);
    static_assert(std::is_same_v<mayo::add_rvalue_ref<int&>, int&>);

    return 0;
}
