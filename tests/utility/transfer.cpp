#include <mayo/utility/transfer.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <cassert>
#include <type_traits>

auto main() -> mayo::i32 {
    int value = 10;

    auto&& moved = mayo::move(value);
    static_assert(std::is_same_v<decltype(moved), int&&>);
    assert(&moved == &value);

    auto&& fwd_l = mayo::forward<int&>(value);
    static_assert(std::is_same_v<decltype(fwd_l), int&>);
    assert(&fwd_l == &value);

    auto&& fwd_r = mayo::forward<int>(20);
    static_assert(std::is_same_v<decltype(fwd_r), int&&>);
    assert(fwd_r == 20);
    assert(fwd_r != 0);

    auto&& like_const = mayo::forward_like<const int&>(value);
    static_assert(std::is_same_v<decltype(like_const), const int&>);
    assert(&like_const == &value);

    auto&& like_r = mayo::forward_like<int&&>(value);
    static_assert(std::is_same_v<decltype(like_r), int&&>);
    assert(&like_r == &value);

    return 0;
}
