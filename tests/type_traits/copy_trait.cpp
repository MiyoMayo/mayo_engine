#include <mayo/type_traits/copy_trait.hpp>
#include <mayo/types/core/basic_types.hpp>

#include <type_traits>

auto main() -> mayo::i32 {
    using CopyConstOn = mayo::copy_const<const int, float>;
    using CopyConstOff = mayo::copy_const<int, float>;

    static_assert(std::is_same_v<CopyConstOn, const float>);
    static_assert(std::is_same_v<CopyConstOff, float>);
    static_assert(!std::is_same_v<CopyConstOff, const float>);

    using RefL = mayo::override_ref<int&, float>;
    using RefR = mayo::override_ref<int&&, float>;

    static_assert(std::is_same_v<RefL, float&>);
    static_assert(std::is_same_v<RefR, float&&>);
    static_assert(!std::is_same_v<RefL, float&&>);

    using ForwardLikeConst = mayo::forward_like_type<const int&, float>;
    using ForwardLikeR = mayo::forward_like_type<int&&, float>;

    static_assert(std::is_same_v<ForwardLikeConst, const float&>);
    static_assert(std::is_same_v<ForwardLikeR, float&&>);
    static_assert(!std::is_same_v<ForwardLikeR, float&>);

    return 0;
}
