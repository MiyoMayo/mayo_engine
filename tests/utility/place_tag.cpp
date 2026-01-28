#include <mayo/utility/place_tag.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(std::is_same_v<decltype(mayo::VALUELESS), const mayo::Valueless>);
    static_assert(std::is_same_v<decltype(mayo::IN_PLACE), const mayo::InPlace>);
    static_assert(
        std::is_same_v<decltype(mayo::IN_PLACE_INDEX<3>), const mayo::InPlaceIndex<3>>);
    static_assert(std::is_same_v<decltype(mayo::IN_PLACE_TYPE<mayo::i32>),
        const mayo::InPlaceType<mayo::i32>>);
    static_assert(!std::is_same_v<mayo::Valueless, mayo::InPlace>);

    return 0;
}
