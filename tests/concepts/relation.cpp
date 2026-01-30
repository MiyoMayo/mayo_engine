#include <mayo/concepts/relation.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <string>

struct Base {};
struct Derived : Base {};

auto main() -> mayo::i32 {
    static_assert(mayo::concepts::is_base_of<Base, Derived>);
    static_assert(!mayo::concepts::is_base_of<Derived, Base>);

    static_assert(mayo::concepts::is_convertible<const char*, std::string>);
    static_assert(!mayo::concepts::is_convertible<std::string, const char*>);

    static_assert(mayo::concepts::is_nothrow_convertible<int, double>);
    static_assert(!mayo::concepts::is_nothrow_convertible<std::string, int>);

    return 0;
}
