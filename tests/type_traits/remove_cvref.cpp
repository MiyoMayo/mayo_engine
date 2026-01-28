#include <mayo/type_traits/remove_cvref.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    using RemoveRefL = mayo::remove_ref<int&>;
    using RemoveRefR = mayo::remove_ref<const int&&>;

    static_assert(std::is_same_v<RemoveRefL, int>);
    static_assert(std::is_same_v<RemoveRefR, const int>);
    static_assert(!std::is_same_v<mayo::remove_ref<int&&>, int&&>);

    using RemoveCv = mayo::remove_cv<const volatile int>;

    static_assert(std::is_same_v<RemoveCv, int>);
    static_assert(!std::is_same_v<mayo::remove_cv<const int>, const int>);

    return 0;
}
