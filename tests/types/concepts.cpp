#include "mayo/types/concepts.hpp"

#include <compare>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

namespace {
struct Empty {};
struct NonEmpty {
    int value;
};

struct MemberHolder {
    int value;
    int method() const {
        return value;
    }
};

enum PlainEnum : unsigned char {
    PLAIN_A
};
enum class ScopedEnum : unsigned char {
    A
};

union SomeUnion {
    int a;
    float b;
};

struct Aggregate {
    int value;
};

struct NonAggregate {
    NonAggregate()
        : value(0) {}
    int value;
};

struct IAbstract {
    virtual void f() = 0;
};

struct Poly {
    virtual ~Poly() = default;
};

struct Final final {};

struct ImplicitLifetime {
    int value;
};

struct NonImplicitLifetime {
    ~NonImplicitLifetime() {}
};

struct NoDefault {
    NoDefault() = delete;
};

struct Copyable {
    Copyable() = default;
    Copyable(const Copyable&) = default;
    Copyable& operator=(const Copyable&) = default;
};

struct MoveOnly {
    MoveOnly() = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

struct NonMovable {
    NonMovable() = default;
    NonMovable(NonMovable&&) = delete;
};

struct NoAssign {
    NoAssign& operator=(const NoAssign&) = delete;
};

struct NoMoveAssign {
    NoMoveAssign& operator=(NoMoveAssign&&) = delete;
};

struct NoDtor {
    ~NoDtor() = delete;
};

struct NonTrivial {
    NonTrivial() {}
};

struct NonTrivialCopy {
    NonTrivialCopy(const NonTrivialCopy&) {}
};

struct NonTrivialMove {
    NonTrivialMove(NonTrivialMove&&) noexcept {}
};

struct NonTrivialCopyAssign {
    NonTrivialCopyAssign& operator=(const NonTrivialCopyAssign&) {
        return *this;
    }
};

struct NonTrivialMoveAssign {
    NonTrivialMoveAssign& operator=(NonTrivialMoveAssign&&) noexcept {
        return *this;
    }
};

struct NonTrivialDtor {
    ~NonTrivialDtor() {}
};

struct ThrowDefault {
    ThrowDefault() noexcept(false) {}
};

struct ThrowCopy {
    ThrowCopy() = default;
    ThrowCopy(const ThrowCopy&) noexcept(false) {}
};

struct ThrowMove {
    ThrowMove() = default;
    ThrowMove(ThrowMove&&) noexcept(false) {}
    ThrowMove& operator=(ThrowMove&&) noexcept(false) {
        return *this;
    }
};

struct NoThrowMove {
    NoThrowMove() = default;
    NoThrowMove(NoThrowMove&&) noexcept = default;
    NoThrowMove& operator=(NoThrowMove&&) noexcept = default;
};

struct ThrowCopyAssign {
    ThrowCopyAssign& operator=(const ThrowCopyAssign&) noexcept(false) {
        return *this;
    }
};

struct ThrowDtor {
    ~ThrowDtor() noexcept(false) {}
};

struct HasVirtualDtor {
    virtual ~HasVirtualDtor() = default;
};

struct NoVirtualDtor {};

struct NoThrowSwap {
    int value;
};

inline void swap(NoThrowSwap& a, NoThrowSwap& b) noexcept {
    std::swap(a.value, b.value);
}

struct ThrowSwap {
    int value;
};

inline void swap(ThrowSwap& a, ThrowSwap& b) noexcept(false) {
    std::swap(a.value, b.value);
}

struct ThrowConvert {
    operator int() noexcept(false) {
        return 0;
    }
};

struct LayoutA {
    int x;
};

struct LayoutB {
    double x;
};

struct PBase {
    int x;
};

struct PDerived : PBase {
    int y;
};

struct InvocableType {
    int operator()(int) const noexcept {
        return 0;
    }
};

struct ThrowInvocableType {
    int operator()(int) const noexcept(false) {
        return 0;
    }
};

struct Padded {
    char c;
    int i;
};

struct ThreeWayTotal {
    int value;
    auto operator<=>(const ThreeWayTotal&) const = default;
};

struct ThreeWayType {
    int value;
    auto operator<=>(const ThreeWayType&) const = default;
};

struct ThreeWayNone {
    int value;
};

int free_func(int) {
    return 0;
}
} // namespace

int main() {
    using namespace mayo::concepts;

    static_assert(is_void<void>);
    static_assert(!is_void<int>);

    static_assert(is_nullptr<std::nullptr_t>);
    static_assert(!is_nullptr<void*>);

    static_assert(is_int<int>);
    static_assert(!is_int<float>);

    static_assert(is_float<double>);
    static_assert(!is_float<int>);

    static_assert(is_array<int[2]>);
    static_assert(!is_array<int>);

    static_assert(is_ptr<int*>);
    static_assert(!is_ptr<int>);

    static_assert(is_lvalue_ref<int&>);
    static_assert(!is_lvalue_ref<int&&>);

    static_assert(is_rvalue_ref<int&&>);
    static_assert(!is_rvalue_ref<int&>);

    static_assert(is_member_object_ptr<int MemberHolder::*>);
    static_assert(!is_member_object_ptr<int*>);

    static_assert(is_member_func_ptr<int (MemberHolder::*)() const>);
    static_assert(!is_member_func_ptr<int*>);

    static_assert(is_enum<PlainEnum>);
    static_assert(!is_enum<int>);

    static_assert(is_union<SomeUnion>);
    static_assert(!is_union<int>);

    static_assert(is_class<Empty>);
    static_assert(!is_class<int>);

    static_assert(is_function<int(int)>);
    static_assert(!is_function<int>);

    static_assert(is_ref<int&>);
    static_assert(!is_ref<int>);

    static_assert(is_arithmetic<int>);
    static_assert(!is_arithmetic<void*>);

    static_assert(is_fundamental<int>);
    static_assert(!is_fundamental<Empty>);

    static_assert(is_object<int>);
    static_assert(!is_object<void>);

    static_assert(is_scalar<int*>);
    static_assert(!is_scalar<Empty>);

    static_assert(is_compound<int*>);
    static_assert(!is_compound<int>);

    static_assert(is_member_ptr<int MemberHolder::*>);
    static_assert(!is_member_ptr<int*>);

    static_assert(is_scoped_enum<ScopedEnum>);
    static_assert(!is_scoped_enum<PlainEnum>);

    static_assert(is_const<const int>);
    static_assert(!is_const<int>);

    static_assert(is_volatile<volatile int>);
    static_assert(!is_volatile<int>);

    static_assert(is_trivially_copyable<int>);
    static_assert(!is_trivially_copyable<std::string>);

    static_assert(is_standard_layout<LayoutA>);
    static_assert(!is_standard_layout<Poly>);

    static_assert(is_empty<Empty>);
    static_assert(!is_empty<NonEmpty>);

    static_assert(is_polymorphic<Poly>);
    static_assert(!is_polymorphic<Empty>);

    static_assert(is_abstract<IAbstract>);
    static_assert(!is_abstract<Empty>);

    static_assert(is_final<Final>);
    static_assert(!is_final<Empty>);

    static_assert(is_aggregate<Aggregate>);
    static_assert(!is_aggregate<NonAggregate>);

    static_assert(is_implicit_lifetime<ImplicitLifetime>);
    static_assert(!is_implicit_lifetime<NonImplicitLifetime>);

    static_assert(is_signed<int>);
    static_assert(!is_signed<unsigned int>);

    static_assert(is_unsigned<unsigned int>);
    static_assert(!is_unsigned<int>);

    static_assert(is_bounded_array<int[3]>);
    static_assert(!is_bounded_array<int[]>);

    static_assert(is_unbounded_array<int[]>);
    static_assert(!is_unbounded_array<int[3]>);

    static_assert(is_copy_assignable<Copyable>);
    static_assert(!is_copy_assignable<NoAssign>);

    static_assert(is_move_assignable<MoveOnly>);
    static_assert(!is_move_assignable<NoMoveAssign>);

    static_assert(is_trivially_constructible<int>);
    static_assert(!is_trivially_constructible<NonTrivial>);

    static_assert(is_trivially_default_constructible<int>);
    static_assert(!is_trivially_default_constructible<NonTrivial>);

    static_assert(is_trivially_copy_constructible<int>);
    static_assert(!is_trivially_copy_constructible<NonTrivialCopy>);

    static_assert(is_trivially_move_constructible<int>);
    static_assert(!is_trivially_move_constructible<NonTrivialMove>);

    static_assert(is_trivially_assignable<int&, int>);
    static_assert(!is_trivially_assignable<const int&, int>);

    static_assert(is_trivially_copy_assignable<int>);
    static_assert(!is_trivially_copy_assignable<NonTrivialCopyAssign>);

    static_assert(is_trivially_move_assignable<int>);
    static_assert(!is_trivially_move_assignable<NonTrivialMoveAssign>);

    static_assert(is_trivially_destructible<int>);
    static_assert(!is_trivially_destructible<NonTrivialDtor>);

    static_assert(is_nothrow_constructible<int, int>);
    static_assert(!is_nothrow_constructible<ThrowMove, ThrowMove&&>);

    static_assert(is_nothrow_default_constructible<int>);
    static_assert(!is_nothrow_default_constructible<ThrowDefault>);

    static_assert(is_nothrow_copy_constructible<int>);
    static_assert(!is_nothrow_copy_constructible<ThrowCopy>);

    static_assert(is_nothrow_move_constructible<NoThrowMove>);
    static_assert(!is_nothrow_move_constructible<ThrowMove>);

    static_assert(is_nothrow_assignable<int&, int>);
    static_assert(!is_nothrow_assignable<ThrowMove&, ThrowMove&&>);

    static_assert(is_nothrow_copy_assignable<int>);
    static_assert(!is_nothrow_copy_assignable<ThrowCopyAssign>);

    static_assert(is_nothrow_move_assignable<NoThrowMove>);
    static_assert(!is_nothrow_move_assignable<ThrowMove>);

    static_assert(is_nothrow_destructible<int>);
    static_assert(!is_nothrow_destructible<ThrowDtor>);

    static_assert(has_virtual_destructor<HasVirtualDtor>);
    static_assert(!has_virtual_destructor<NoVirtualDtor>);

    static_assert(is_nothrow_swappable_with<NoThrowSwap&, NoThrowSwap&>);
    static_assert(!is_nothrow_swappable_with<ThrowSwap&, ThrowSwap&>);

    static_assert(is_nothrow_swappable<NoThrowSwap>);
    static_assert(!is_nothrow_swappable<ThrowSwap>);

    static_assert(has_unique_object_representations<unsigned char>);
    static_assert(!has_unique_object_representations<Padded>);

    static_assert(is_base_of<PBase, PDerived>);
    static_assert(!is_base_of<PDerived, PBase>);

    static_assert(is_nothrow_convertible<int, double>);
    static_assert(!is_nothrow_convertible<ThrowConvert, int>);

    static_assert(ref_constructs_from_tmp<const int&, int>);
    static_assert(!ref_constructs_from_tmp<int&, int>);

    static_assert(ref_converts_from_tmp<const int&, int>);
    static_assert(!ref_converts_from_tmp<int&, int>);

    static_assert(is_invocable_result_convertible<double, decltype(free_func), int>);
    static_assert(!is_invocable_result_convertible<std::string, decltype(free_func), int>);

    static_assert(is_nothrow_invocable<InvocableType, int>);
    static_assert(!is_nothrow_invocable<ThrowInvocableType, int>);

    static_assert(is_nothrow_invocable_result_convertible<double, InvocableType, int>);
    static_assert(!is_nothrow_invocable_result_convertible<double, ThrowInvocableType, int>);

    static_assert(three_way_comparable<ThreeWayTotal>);
    static_assert(!three_way_comparable<ThreeWayNone>);
    static_assert(three_way_comparable_with<ThreeWayType, ThreeWayType>);
    static_assert(!three_way_comparable_with<ThreeWayType, int>);

    return 0;
}
