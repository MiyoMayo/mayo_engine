#pragma once

#include <type_traits>

namespace mayo::concepts {

// void 型判定
template <class T>
concept is_void = (std::is_void_v<T>);

// nullptr 型判定
template <class T>
concept is_nullptr = (std::is_null_pointer_v<T>);

// 整数型判定
template <class T>
concept is_int = (std::is_integral_v<T>);

// 浮動小数点型判定
template <class T>
concept is_float = (std::is_floating_point_v<T>);

// 配列型判定
template <class T>
concept is_array = (std::is_array_v<T>);

// ポインタ型判定
template <class T>
concept is_ptr = (std::is_pointer_v<T>);

// 左辺値参照型判定
template <class T>
concept is_lvalue_ref = (std::is_lvalue_reference_v<T>);

// 右辺値参照型判定
template <class T>
concept is_rvalue_ref = (std::is_rvalue_reference_v<T>);

// メンバオブジェクトポインタ型判定
template <class T>
concept is_member_object_ptr = (std::is_member_object_pointer_v<T>);

// メンバ関数ポインタ型判定
template <class T>
concept is_member_func_ptr = (std::is_member_function_pointer_v<T>);

// 列挙型判定
template <class T>
concept is_enum = (std::is_enum_v<T>);

// union 型判定
template <class T>
concept is_union = (std::is_union_v<T>);

// class 型判定
template <class T>
concept is_class = (std::is_class_v<T>);

// 関数型判定
template <class T>
concept is_function = (std::is_function_v<T>);

// 参照型判定
template <class T>
concept is_ref = (std::is_reference_v<T>);

// 算術型判定
template <class T>
concept is_arithmetic = (std::is_arithmetic_v<T>);

// 基本型判定
template <class T>
concept is_fundamental = (std::is_fundamental_v<T>);

// オブジェクト型判定
template <class T>
concept is_object = (std::is_object_v<T>);

// スカラー型判定
template <class T>
concept is_scalar = (std::is_scalar_v<T>);

// 複合型判定
template <class T>
concept is_compound = (std::is_compound_v<T>);

// メンバポインタ型判定
template <class T>
concept is_member_ptr = (std::is_member_pointer_v<T>);

// scoped enum 判定
template <class T>
concept is_scoped_enum = (std::is_scoped_enum_v<T>);

// const 修飾判定
template <class T>
concept is_const = (std::is_const_v<T>);

// volatile 修飾判定
template <class T>
concept is_volatile = (std::is_volatile_v<T>);

// トリビアルコピー可能判定
template <class T>
concept is_trivially_copyable = (std::is_trivially_copyable_v<T>);

// 標準レイアウト判定
template <class T>
concept is_standard_layout = (std::is_standard_layout_v<T>);

// 空クラス判定
template <class T>
concept is_empty = (std::is_empty_v<T>);

// 多態型判定
template <class T>
concept is_polymorphic = (std::is_polymorphic_v<T>);

// 抽象型判定
template <class T>
concept is_abstract = (std::is_abstract_v<T>);

// final 判定
template <class T>
concept is_final = (std::is_final_v<T>);

// 集成体判定
template <class T>
concept is_aggregate = (std::is_aggregate_v<T>);

// 暗黙ライフタイム型判定
template <class T>
concept is_implicit_lifetime = (std::is_implicit_lifetime_v<T>);

// 符号付き判定
template <class T>
concept is_signed = (std::is_signed_v<T>);

// 符号なし判定
template <class T>
concept is_unsigned = (std::is_unsigned_v<T>);

// 有界配列判定
template <class T>
concept is_bounded_array = (std::is_bounded_array_v<T>);

// 無界配列判定
template <class T>
concept is_unbounded_array = (std::is_unbounded_array_v<T>);

// コピー代入可能判定
template <class T>
concept is_copy_assignable = (std::is_copy_assignable_v<T>);

// ムーブ代入可能判定
template <class T>
concept is_move_assignable = (std::is_move_assignable_v<T>);

// トリビアル構築可能判定
template <class T, class... Args>
concept is_trivially_constructible = (std::is_trivially_constructible_v<T, Args...>);

// トリビアルデフォルト構築可能判定
template <class T>
concept is_trivially_default_constructible = (std::is_trivially_default_constructible_v<T>);

// トリビアルコピー構築可能判定
template <class T>
concept is_trivially_copy_constructible = (std::is_trivially_copy_constructible_v<T>);

// トリビアルムーブ構築可能判定
template <class T>
concept is_trivially_move_constructible = (std::is_trivially_move_constructible_v<T>);

// トリビアル代入可能判定
template <class Target, class Source>
concept is_trivially_assignable = (std::is_trivially_assignable_v<Target, Source>);

// トリビアルコピー代入可能判定
template <class T>
concept is_trivially_copy_assignable = (std::is_trivially_copy_assignable_v<T>);

// トリビアルムーブ代入可能判定
template <class T>
concept is_trivially_move_assignable = (std::is_trivially_move_assignable_v<T>);

// トリビアル破棄可能判定
template <class T>
concept is_trivially_destructible = (std::is_trivially_destructible_v<T>);

// noexcept 構築可能判定
template <class T, class... Args>
concept is_nothrow_constructible = (std::is_nothrow_constructible_v<T, Args...>);

// noexcept デフォルト構築可能判定
template <class T>
concept is_nothrow_default_constructible = (std::is_nothrow_default_constructible_v<T>);

// noexcept コピー構築可能判定
template <class T>
concept is_nothrow_copy_constructible = (std::is_nothrow_copy_constructible_v<T>);

// noexcept ムーブ構築可能判定
template <class T>
concept is_nothrow_move_constructible = (std::is_nothrow_move_constructible_v<T>);

// noexcept 代入可能判定
template <class Target, class Source>
concept is_nothrow_assignable = (std::is_nothrow_assignable_v<Target, Source>);

// noexcept コピー代入可能判定
template <class T>
concept is_nothrow_copy_assignable = (std::is_nothrow_copy_assignable_v<T>);

// noexcept ムーブ代入可能判定
template <class T>
concept is_nothrow_move_assignable = (std::is_nothrow_move_assignable_v<T>);

// noexcept 破棄可能判定
template <class T>
concept is_nothrow_destructible = (std::is_nothrow_destructible_v<T>);

// 仮想デストラクタ有無判定
template <class T>
concept has_virtual_destructor = (std::has_virtual_destructor_v<T>);

// noexcept swap 可能判定（2型）
template <class T, class U>
concept is_nothrow_swappable_with = (std::is_nothrow_swappable_with_v<T, U>);

// noexcept swap 可能判定
template <class T>
concept is_nothrow_swappable = (std::is_nothrow_swappable_v<T>);

// ユニーク表現判定
template <class T>
concept has_unique_object_representations = (std::has_unique_object_representations_v<T>);

// 基底関係判定
template <class Base, class Derived>
concept is_base_of = (std::is_base_of_v<Base, Derived>);

// noexcept 変換可能判定
template <class From, class To>
concept is_nothrow_convertible = (std::is_nothrow_convertible_v<From, To>);

// 一時オブジェクトからの参照構築可否
template <class Ref, class Source>
concept ref_constructs_from_tmp = (std::reference_constructs_from_temporary_v<Ref, Source>);

// 一時オブジェクトからの参照変換可否
template <class Ref, class Source>
concept ref_converts_from_tmp = (std::reference_converts_from_temporary_v<Ref, Source>);

// 呼び出し結果の変換可能判定
template <class Result, class Func, class... Args>
concept is_invocable_result_convertible = (std::is_invocable_r_v<Result, Func, Args...>);

// noexcept 呼び出し可能判定
template <class Func, class... Args>
concept is_nothrow_invocable = (std::is_nothrow_invocable_v<Func, Args...>);

// noexcept 呼び出し結果の変換可能判定
template <class Result, class Func, class... Args>
concept is_nothrow_invocable_result_convertible =
    (std::is_nothrow_invocable_r_v<Result, Func, Args...>);
} // namespace mayo::concepts
