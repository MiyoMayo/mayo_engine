#pragma once

#include <type_traits>

namespace mayo::concepts {
template <class T>
concept is_void = (std::is_void_v<T>);

template <class T>
concept is_nullptr = (std::is_null_pointer_v<T>);

template <class T>
concept is_int = (std::is_integral_v<T>);

template <class T>
concept is_float = (std::is_floating_point_v<T>);

template <class T>
concept is_array = (std::is_array_v<T>);

template <class T>
concept is_ptr = (std::is_pointer_v<T>);

template <class T>
concept is_lvalue_ref = (std::is_lvalue_reference_v<T>);

template <class T>
concept is_rvalue_ref = (std::is_rvalue_reference_v<T>);

template <class T>
concept is_member_object_ptr = (std::is_member_object_pointer_v<T>);

template <class T>
concept is_member_func_ptr = (std::is_member_function_pointer_v<T>);

template <class T>
concept is_enum = (std::is_enum_v<T>);

template <class T>
concept is_union = (std::is_union_v<T>);

template <class T>
concept is_class = (std::is_class_v<T>);

template <class T>
concept is_function = (std::is_function_v<T>);

template <class T>
concept is_ref = (std::is_reference_v<T>);

template <class T>
concept is_arithmetic = (std::is_arithmetic_v<T>);

template <class T>
concept is_fundamental = (std::is_fundamental_v<T>);

template <class T>
concept is_object = (std::is_object_v<T>);

template <class T>
concept is_scalar = (std::is_scalar_v<T>);

template <class T>
concept is_compound = (std::is_compound_v<T>);

template <class T>
concept is_member_ptr = (std::is_member_pointer_v<T>);

template <class T>
concept is_scoped_enum = (std::is_scoped_enum_v<T>);

template <class T>
concept is_const = (std::is_const_v<T>);

template <class T>
concept is_volatile = (std::is_volatile_v<T>);

template <class T>
concept is_trivially_copyable = (std::is_trivially_copyable_v<T>);

template <class T>
concept is_standard_layout = (std::is_standard_layout_v<T>);

template <class T>
concept is_empty = (std::is_empty_v<T>);

template <class T>
concept is_polymorphic = (std::is_polymorphic_v<T>);

template <class T>
concept is_abstract = (std::is_abstract_v<T>);

template <class T>
concept is_final = (std::is_final_v<T>);

template <class T>
concept is_aggregate = (std::is_aggregate_v<T>);

template <class T>
concept is_implicit_lifetime = (std::is_implicit_lifetime_v<T>);

template <class T>
concept is_signed = (std::is_signed_v<T>);

template <class T>
concept is_unsigned = (std::is_unsigned_v<T>);

template <class T>
concept is_bounded_array = (std::is_bounded_array_v<T>);

template <class T>
concept is_unbounded_array = (std::is_unbounded_array_v<T>);

template <class T, class... Args>
concept is_constructible = (std::is_constructible_v<T, Args...>);

template <class T>
concept is_default_constructible = (std::is_default_constructible_v<T>);

template <class T>
concept is_copy_constructible = (std::is_copy_constructible_v<T>);

template <class T>
concept is_move_constructible = (std::is_move_constructible_v<T>);

template <class T, class U>
concept is_assignable = (std::is_assignable_v<T, U>);

template <class T>
concept is_copy_assignable = (std::is_copy_assignable_v<T>);

template <class T>
concept is_move_assignable = (std::is_move_assignable_v<T>);

template <class T>
concept is_destructible = (std::is_destructible_v<T>);

template <class T, class... Args>
concept is_trivially_constructible = (std::is_trivially_constructible_v<T, Args...>);

template <class T>
concept is_trivially_default_constructible = (std::is_trivially_default_constructible_v<T>);

template <class T>
concept is_trivially_copy_constructible = (std::is_trivially_copy_constructible_v<T>);

template <class T>
concept is_trivially_move_constructible = (std::is_trivially_move_constructible_v<T>);

template <class T, class U>
concept is_trivially_assignable = (std::is_trivially_assignable_v<T, U>);

template <class T>
concept is_trivially_copy_assignable = (std::is_trivially_copy_assignable_v<T>);

template <class T>
concept is_trivially_move_assignable = (std::is_trivially_move_assignable_v<T>);

template <class T>
concept is_trivially_destructible = (std::is_trivially_destructible_v<T>);

template <class T, class... Args>
concept is_nothrow_constructible = (std::is_nothrow_constructible_v<T, Args...>);

template <class T>
concept is_nothrow_default_constructible = (std::is_nothrow_default_constructible_v<T>);

template <class T>
concept is_nothrow_copy_constructible = (std::is_nothrow_copy_constructible_v<T>);

template <class T>
concept is_nothrow_move_constructible = (std::is_nothrow_move_constructible_v<T>);

template <class T, class U>
concept is_nothrow_assignable = (std::is_nothrow_assignable_v<T, U>);

template <class T>
concept is_nothrow_copy_assignable = (std::is_nothrow_copy_assignable_v<T>);

template <class T>
concept is_nothrow_move_assignable = (std::is_nothrow_move_assignable_v<T>);

template <class T>
concept is_nothrow_destructible = (std::is_nothrow_destructible_v<T>);

template <class T>
concept has_virtual_destructor = (std::has_virtual_destructor_v<T>);

template <class T, class U>
concept is_swappable_with = (std::is_swappable_with_v<T, U>);

template <class T>
concept is_swappable = (std::is_swappable_v<T>);

template <class T, class U>
concept is_nothrow_swappable_with = (std::is_nothrow_swappable_with_v<T, U>);

template <class T>
concept is_nothrow_swappable = (std::is_nothrow_swappable_v<T>);

template <class T>
concept has_unique_object_representations = (std::has_unique_object_representations_v<T>);

template <class T, class U>
concept is_same = (std::is_same_v<T, U>);

template <class Base, class Derived>
concept is_base_of = (std::is_base_of_v<Base, Derived>);

template <class Base, class Derived>
concept is_virtual_base_of = (std::is_virtual_base_of_v<Base, Derived>);

template <class From, class To>
concept is_convertible = (std::is_convertible_v<From, To>);

template <class From, class To>
concept is_nothrow_convertible = (std::is_nothrow_convertible_v<From, To>);

template <class T, class U>
concept is_layout_compatible = (__is_layout_compatible(T, U));

template <class Base, class Derived>
concept is_ptr_interconvertible_base_of = (__is_pointer_interconvertible_base_of(Base, Derived));

template <class Ref, class T>
concept ref_constructs_from_tmp = (std::reference_constructs_from_temporary_v<Ref, T>);

template <class Ref, class T>
concept ref_converts_from_tmp = (std::reference_converts_from_temporary_v<Ref, T>);

template <class Fn, class... Args>
concept is_invocable = (std::is_invocable_v<Fn, Args...>);

template <class R, class Fn, class... Args>
concept is_invocable_result_convertible = (std::is_invocable_r_v<R, Fn, Args...>);

template <class Fn, class... Args>
concept is_nothrow_invocable = (std::is_nothrow_invocable_v<Fn, Args...>);

template <class R, class Fn, class... Args>
concept is_nothrow_invocable_result_convertible = (std::is_nothrow_invocable_r_v<R, Fn, Args...>);

template <class = void>
concept is_constant_evaluated = (std::is_constant_evaluated());
} // namespace mayo::concepts
