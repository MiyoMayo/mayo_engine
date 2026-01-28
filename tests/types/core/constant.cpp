#include <mayo/types/core/constant.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    static_assert(mayo::IntegralConstant<int, 3>::VALUE == 3);
    static_assert(mayo::BoolConstant<true>::VALUE == true);
    static_assert(mayo::TrueType::VALUE);
    static_assert(!mayo::FalseType::VALUE);
    static_assert(!std::is_same_v<mayo::TrueType, mayo::FalseType>);

    return 0;
}
