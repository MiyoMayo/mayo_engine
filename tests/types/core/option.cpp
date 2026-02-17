#include "mayo/types/core/option.hpp"
#include "mayo/types/core/numeric.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <ranges>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

struct Counted {
    inline static int destroyed = 0;
    int value = 0;

    explicit Counted(int v)
        : value{v} {}

    Counted(const Counted&) = default;
    Counted(Counted&& other) noexcept
        : value{other.value} {
        other.value = -1;
    }

    ~Counted() {
        ++destroyed;
    }
};

struct NoThrowMove {
    NoThrowMove() = default;
    NoThrowMove(NoThrowMove&&) noexcept = default;
    auto operator=(NoThrowMove&&) noexcept -> NoThrowMove& = default;
};

struct ThrowMove {
    ThrowMove() = default;
    ThrowMove(ThrowMove&&) noexcept(false) {}
};

struct MoveOnly {
    int value = 0;

    MoveOnly() = default;
    explicit MoveOnly(int v)
        : value{v} {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) noexcept = default;
    auto operator=(const MoveOnly&) -> MoveOnly& = delete;
    auto operator=(MoveOnly&&) noexcept -> MoveOnly& = default;
};

struct RebuildOnly {
    int value = 0;

    explicit RebuildOnly(int v)
        : value{v} {}

    RebuildOnly(const RebuildOnly&) = default;
    RebuildOnly(RebuildOnly&&) = default;
    auto operator=(const RebuildOnly&) -> RebuildOnly& = delete;
    auto operator=(RebuildOnly&&) -> RebuildOnly& = delete;
};

auto main() -> mayo::i32 {
    {
        constexpr mayo::Option<mayo::i32> NONE{};
        static_assert(NONE.is_none());
        static_assert(!NONE.is_some());
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        assert(none.is_none());
        assert(!none.is_some());
    }

    {
        mayo::Option<mayo::i32> some{1};
        assert(some.is_some());
        assert(!some.is_none());
        assert(*some == 1);
        assert(some.unwrap() == 1);
        assert(static_cast<bool>(some));

        *some = 5;
        assert(*some == 5);
    }

    Counted::destroyed = 0;
    {
        Counted local{7};
        mayo::Option<Counted> value{std::move(local)};
        assert(value);
        assert(value.unwrap().value == 7);
        assert(value.unwrap().value != 0);
    }
    assert(Counted::destroyed == 2);
    assert(Counted::destroyed != 1);

    {
        mayo::Option<mayo::i32> a{42};
        mayo::Option<mayo::i32>& b{a};
        assert(*b == 42);

        mayo::Option<mayo::i32> c = b;
        assert(c.is_some());
        assert(*c == 42);

        mayo::Option<mayo::i32> d = std::move(a);
        assert(d.is_some());
        assert(*d == 42);

        mayo::Option<mayo::i32> e = mayo::NONE;
        e = *b;
        assert(e.is_some());
        assert(*e == 42);

        mayo::Option<mayo::i32> f{mayo::NONE};
        f = mayo::Option{std::move(b)};
        assert(f.is_some());
        assert(*f == 42);

        f = mayo::NONE;
        assert(f.is_none());

        f = 7;
        assert(f.is_some());
        assert(*f == 7);
    }

    {
        mayo::Option<RebuildOnly> value{RebuildOnly{11}};
        value = *value;
        assert(value.is_some());
        assert(value->value == 11);

        RebuildOnly other{29};
        value = other;
        assert(value.is_some());
        assert(value->value == 29);
    }

    {
        const mayo::Option<mayo::i32> NONE{};
        const mayo::Option<mayo::i32> SOME{10};
        const mayo::Option<mayo::i32> OTHER{10};
        const mayo::Option<mayo::i32> DIFFERENT{20};
        const mayo::Option<long> OTHER_TYPE{10};

        assert(NONE == mayo::NONE);
        assert(!(SOME == mayo::NONE));

        assert(NONE == mayo::Option<mayo::i32>{});
        assert(!(NONE == SOME));
        assert(SOME == OTHER);
        assert(!(SOME == DIFFERENT));

        assert(SOME == 10);
        assert(!(SOME == 5));
        assert(10 == SOME);
        assert(!(5 == SOME));
        assert(!(NONE == 10));

        assert(SOME == OTHER_TYPE);

        auto cmp_none_some = NONE <=> SOME;
        assert(std::is_lt(cmp_none_some));

        auto cmp_some_none = 10 <=> NONE;
        assert(std::is_gt(cmp_some_none));

        auto cmp_some_val = SOME <=> 10;
        assert(std::is_eq(cmp_some_val));

        auto cmp_some_diff = SOME <=> DIFFERENT;
        assert(std::is_lt(cmp_some_diff));

        auto cmp_some_none_tag = SOME <=> mayo::NONE;
        assert(std::is_gt(cmp_some_none_tag));
    }

    {
        mayo::Option<std::string> text{"hello"};
        assert(text->size() == 5);
        assert(text->front() == 'h');

        const mayo::Option<std::string> CONST_TEXT{"world"};
        assert(CONST_TEXT->size() == 5);
        assert(CONST_TEXT->front() == 'w');
    }

    {
        mayo::Option<mayo::i32> none{mayo::NONE};
        mayo::Option<mayo::i32> some{3};

        assert(none.begin() == none.end());
        assert(some.begin() != some.end());
        assert(*some.begin() == 3);

        auto sum = 0;
        for (auto value : some) {
            sum += value;
        }
        assert(sum == 3);

        auto none_sum = 0;
        for (auto value : none) {
            none_sum += value;
        }
        assert(none_sum == 0);
    }

    {
        const mayo::Option<mayo::i32> NONE{mayo::NONE};
        const mayo::Option<mayo::i32> SOME{7};

        assert(NONE.begin() == NONE.end());
        assert(SOME.begin() != SOME.end());
        assert(*SOME.begin() == 7);
    }

    {
        mayo::Option<mayo::i32> none{mayo::NONE};
        auto none_ref = none.as_ref();
        assert(none_ref.is_none());

        const mayo::Option<mayo::i32> const_some{13};
        auto const_ref = const_some.as_ref();
        assert(const_ref.is_some());
        assert(const_ref->get() == 13);
    }

    {
        mayo::Option<mayo::i32> some{21};
        auto value_mut = some.as_mut();
        assert(value_mut.is_some());
        value_mut->get() = 99;
        assert(some.is_some());
        assert(*some == 99);

        mayo::Option<mayo::i32> none{mayo::NONE};
        auto none_mut = none.as_mut();
        assert(none_mut.is_none());
    }

    {
        mayo::Option<mayo::i32> none{mayo::NONE};
        auto none_slice = none.as_slice();
        assert(none_slice.empty());
        assert(none_slice.size() == 0);

        const mayo::Option<mayo::i32> some{31};
        auto some_slice = some.as_slice();
        assert(!some_slice.empty());
        assert(some_slice.size() == 1);
        assert(some_slice[0] == 31);
    }

    {
        mayo::Option<mayo::i32> some{44};
        auto some_slice = some.as_mut_slice();
        assert(!some_slice.empty());
        assert(some_slice.size() == 1);
        some_slice[0] = 66;
        assert(*some == 66);

        mayo::Option<mayo::i32> none{mayo::NONE};
        auto none_slice = none.as_mut_slice();
        assert(none_slice.empty());
        assert(none_slice.size() == 0);
    }

    {
        mayo::Option<std::string> some{"hello"};
        auto& value = some.expect("unused");
        assert(value == "hello");

        value = "updated";
        assert(some.is_some());
        assert(*some == "updated");
    }

    {
        mayo::Option<std::string> none{mayo::NONE};
        auto captured = std::string{};
        auto thrown = false;

        try {
            (void)none.expect("custom message");
        } catch (const std::runtime_error& e) {
            captured = e.what();
            thrown = true;
        }

        assert(thrown);
        assert(captured == "custom message");
    }

    {
        mayo::Option<std::string> none{mayo::NONE};
        auto captured = std::string{};
        auto thrown = false;

        try {
            (void)none->size();
        } catch (const std::runtime_error& e) {
            captured = e.what();
            thrown = true;
        }

        assert(thrown);
        assert(captured == "Optionに値がありません。");
    }

    {
        const mayo::Option<mayo::i32> some{10};
        const mayo::Option<mayo::i32> none{mayo::NONE};
        assert(some.unwrap_or(99) == 10);
        assert(none.unwrap_or(99) == 99);
    }

    {
        auto some = mayo::Option<std::string>{"alive"};
        auto none = mayo::Option<std::string>{mayo::NONE};
        assert(std::move(some).unwrap_or("fallback") == "alive");
        assert(std::move(none).unwrap_or("fallback") == "fallback");
    }

    {
        const mayo::Option<mayo::i32> some{7};
        const mayo::Option<mayo::i32> none{mayo::NONE};
        auto called = 0;

        auto from_some = some.unwrap_or_else([&]() {
            ++called;
            return 100;
        });
        assert(from_some == 7);
        assert(called == 0);

        auto from_none = none.unwrap_or_else([&]() {
            ++called;
            return 100;
        });
        assert(from_none == 100);
        assert(called == 1);
    }

    {
        auto some = mayo::Option<std::string>{"x"};
        auto none = mayo::Option<std::string>{mayo::NONE};
        assert(std::move(some).unwrap_or_else([] { return std::string{"fallback"}; }) == "x");
        assert(std::move(none).unwrap_or_else([] { return std::string{"fallback"}; }) == "fallback");
    }

    {
        const mayo::Option<std::string> none{mayo::NONE};
        const mayo::Option<std::string> some{"value"};
        assert(none.unwrap_or_default().empty());
        assert(some.unwrap_or_default() == "value");
    }

    {
        auto none = mayo::Option<std::string>{mayo::NONE};
        auto some = mayo::Option<std::string>{"value"};
        assert(std::move(none).unwrap_or_default().empty());
        assert(std::move(some).unwrap_or_default() == "value");
    }

    {
        mayo::Option<std::string> some{"unsafe"};
        auto& value = some.unwrap_unchecked();
        value = "updated";
        assert(*some == "updated");

        const mayo::Option<mayo::i32> CONST_SOME{5};
        assert(CONST_SOME.unwrap_unchecked() == 5);
    }

    {
        auto some = mayo::Option<mayo::i32>{5};
        auto mapped = std::move(some).map([](auto v) { return v * 3; });
        assert(mapped.is_some());
        assert(*mapped == 15);
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;
        auto mapped = std::move(none).map([&](mayo::i32&&) {
            ++called;
            return 1;
        });
        assert(mapped.is_none());
        assert(called == 0);
    }

    {
        auto some = mayo::Option<std::string>{"abc"};
        auto mapped = std::move(some).map([](std::string&& s) {
            return static_cast<mayo::i32>(s.size());
        });
        assert(mapped.is_some());
        assert(*mapped == 3);
    }

    {
        auto some = mayo::Option<mayo::i32>{8};
        auto called = 0;
        auto inspected = std::move(some).inspect([&](const mayo::i32& v) {
            ++called;
            assert(v == 8);
        });
        assert(called == 1);
        assert(inspected.is_some());
        assert(*inspected == 8);
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;
        auto inspected = std::move(none).inspect([&](const mayo::i32&) {
            ++called;
        });
        assert(called == 0);
        assert(inspected.is_none());
    }

    {
        auto some = mayo::Option<mayo::i32>{6};
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        assert(std::move(some).map_or(99, [](mayo::i32&& v) { return v * 2; }) == 12);
        assert(std::move(none).map_or(99, [](mayo::i32&& v) { return v * 2; }) == 99);
    }

    {
        auto some = mayo::Option<mayo::i32>{4};
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;

        auto from_some = std::move(some).map_or_else(
            [&]() {
                ++called;
                return 100;
            },
            [](mayo::i32&& v) { return v + 1; });
        assert(from_some == 5);
        assert(called == 0);

        auto from_none = std::move(none).map_or_else(
            [&]() {
                ++called;
                return 100;
            },
            [](mayo::i32&& v) { return v + 1; });
        assert(from_none == 100);
        assert(called == 1);
    }

    {
        auto some = mayo::Option<mayo::i32>{9};
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        assert(std::move(some).map_or_default([](mayo::i32&& v) { return v + 1; }) == 10);
        assert(std::move(none).map_or_default([](mayo::i32&& v) { return v + 1; }) == 0);
    }

    {
        auto some = mayo::Option<std::string>{"abcd"};
        auto none = mayo::Option<std::string>{mayo::NONE};
        assert(std::move(some).map_or_default([](std::string&& s) { return s.size(); }) == 4);
        assert(std::move(none).map_or_default([](std::string&& s) { return s.size(); }) == 0);
    }

    {
        auto lhs_some = mayo::Option<mayo::i32>{1};
        auto rhs_some = mayo::Option<std::string>{"ok"};
        auto result = std::move(lhs_some).and_other(std::move(rhs_some));
        assert(result.is_some());
        assert(*result == "ok");
    }

    {
        auto lhs_none = mayo::Option<mayo::i32>{mayo::NONE};
        auto rhs_some = mayo::Option<std::string>{"ok"};
        auto result = std::move(lhs_none).and_other(std::move(rhs_some));
        assert(result.is_none());
    }

    {
        auto some = mayo::Option<mayo::i32>{3};
        auto called = 0;
        auto result = std::move(some).and_then([&](mayo::i32&& v) {
            ++called;
            return mayo::Option<std::string>{std::to_string(v)};
        });

        assert(called == 1);
        assert(result.is_some());
        assert(*result == "3");
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;
        auto result = std::move(none).and_then([&](mayo::i32&&) {
            ++called;
            return mayo::Option<std::string>{"x"};
        });

        assert(called == 0);
        assert(result.is_none());
    }

    {
        auto some = mayo::Option<mayo::i32>{10};
        auto kept = std::move(some).filter([](const mayo::i32& v) {
            return v == 10;
        });
        assert(kept.is_some());
        assert(*kept == 10);
    }

    {
        auto some = mayo::Option<mayo::i32>{10};
        auto dropped = std::move(some).filter([](const mayo::i32& v) {
            return v == 0;
        });
        assert(dropped.is_none());
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;
        auto result = std::move(none).filter([&](const mayo::i32&) {
            ++called;
            return true;
        });
        assert(result.is_none());
        assert(called == 0);
    }

    {
        auto some = mayo::Option<mayo::i32>{7};
        auto result = std::move(some).or_other(mayo::Option<mayo::i32>{99});
        assert(result.is_some());
        assert(*result == 7);
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto result = std::move(none).or_other(mayo::Option<mayo::i32>{99});
        assert(result.is_some());
        assert(*result == 99);
    }

    {
        auto some = mayo::Option<mayo::i32>{7};
        auto called = 0;
        auto result = std::move(some).or_else([&] {
            ++called;
            return mayo::Option<mayo::i32>{99};
        });
        assert(result.is_some());
        assert(*result == 7);
        assert(called == 0);
    }

    {
        auto none = mayo::Option<mayo::i32>{mayo::NONE};
        auto called = 0;
        auto result = std::move(none).or_else([&] {
            ++called;
            return mayo::Option<mayo::i32>{99};
        });
        assert(result.is_some());
        assert(*result == 99);
        assert(called == 1);
    }

    {
        auto lhs = mayo::Option<mayo::i32>{1};
        auto rhs = mayo::Option<mayo::i32>{mayo::NONE};
        auto result = std::move(lhs).xor_other(std::move(rhs));
        assert(result.is_some());
        assert(*result == 1);
    }

    {
        auto lhs = mayo::Option<mayo::i32>{mayo::NONE};
        auto rhs = mayo::Option<mayo::i32>{2};
        auto result = std::move(lhs).xor_other(std::move(rhs));
        assert(result.is_some());
        assert(*result == 2);
    }

    {
        auto lhs = mayo::Option<mayo::i32>{1};
        auto rhs = mayo::Option<mayo::i32>{2};
        auto result = std::move(lhs).xor_other(std::move(rhs));
        assert(result.is_none());
    }

    {
        auto lhs = mayo::Option<mayo::i32>{mayo::NONE};
        auto rhs = mayo::Option<mayo::i32>{mayo::NONE};
        auto result = std::move(lhs).xor_other(std::move(rhs));
        assert(result.is_none());
    }

    {
        static_assert(std::equality_comparable_with<mayo::i32, mayo::i32>);
        static_assert(std::equality_comparable_with<mayo::Option<mayo::i32>, mayo::Option<mayo::i32>>);
        static_assert(std::equality_comparable_with<mayo::Option<mayo::i32>*, mayo::Option<mayo::i32>*>);
        static_assert(std::equality_comparable_with<mayo::i32*, mayo::i32*>);

        std::vector<mayo::Option<mayo::i32>> values{
            mayo::Option<mayo::i32>{0},
            mayo::Option<mayo::i32>{mayo::NONE},
            mayo::Option<mayo::i32>{3},
        };

        auto joined = values | std::views::join;
        std::vector<mayo::i32> out{};
        for (auto value : joined) {
            out.push_back(value);
        }

        assert(out.size() == 2);
        assert(out[0] == 0);
        assert(out[1] == 3);
    }

    {
        static_assert(std::is_constructible_v<mayo::Option<std::string>, const char*>);
        static_assert(!std::is_convertible_v<const char*, mayo::Option<std::string>>);
        static_assert(!std::is_constructible_v<mayo::Option<mayo::i32>, std::string>);
        static_assert(std::is_assignable_v<mayo::Option<std::string>&, const char*>);
        static_assert(!std::is_assignable_v<mayo::Option<mayo::i32>&, std::string>);
        static_assert(std::is_assignable_v<mayo::Option<mayo::i32>&, mayo::types::core::None>);
        static_assert(!std::is_convertible_v<mayo::Option<mayo::i32>, bool>);
        static_assert(std::is_convertible_v<mayo::Option<mayo::i32>, mayo::Option<mayo::i32>>);

        static_assert(std::is_same_v<decltype(*std::declval<mayo::Option<mayo::i32>&>()), mayo::i32&>);
        static_assert(std::is_same_v<decltype(*std::declval<const mayo::Option<mayo::i32>&>()), const mayo::i32&>);
        static_assert(std::is_same_v<decltype(*std::declval<mayo::Option<mayo::i32>&&>()), mayo::i32&&>);
        static_assert(std::is_same_v<decltype(*std::declval<const mayo::Option<mayo::i32>&&>()), const mayo::i32&&>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<std::string>&>().operator->()), std::string*>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<std::string>&>().operator->()), const std::string*>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().as_ref()), mayo::Option<mayo::Ref<const mayo::i32>>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&>().as_mut()), mayo::Option<mayo::Ref<mayo::i32>>>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().as_slice()), std::span<const mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&>().as_mut_slice()), std::span<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&>().expect(std::declval<std::string_view>())), mayo::i32&>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().expect(std::declval<std::string_view>())), const mayo::i32&>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().expect(std::declval<std::string_view>())), mayo::i32&&>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&&>().expect(std::declval<std::string_view>())), const mayo::i32&&>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().unwrap_or(0)), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().unwrap_or(0)), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().unwrap_or_else([] { return 0; })), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().unwrap_or_else([] { return 0; })), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().unwrap_or_default()), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().unwrap_or_default()), mayo::i32>);
        static_assert(requires(mayo::Option<MoveOnly>&& o) {
            std::move(o).unwrap_or(MoveOnly{});
        });
        static_assert(!mayo::concepts::constructible_from<MoveOnly, const MoveOnly&>);
        static_assert(requires(mayo::Option<MoveOnly>&& o) {
            std::move(o).unwrap_or_else([] { return MoveOnly{}; });
        });
        static_assert(requires(mayo::Option<MoveOnly>&& o) {
            std::move(o).unwrap_or_default();
        });
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&>().unwrap_unchecked()), mayo::i32&>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&>().unwrap_unchecked()), const mayo::i32&>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().unwrap_unchecked()), mayo::i32&&>);
        static_assert(std::is_same_v<decltype(std::declval<const mayo::Option<mayo::i32>&&>().unwrap_unchecked()), const mayo::i32&&>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().map([](mayo::i32&& v) { return v + 1; })), mayo::Option<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<std::string>&&>().map([](std::string&& s) { return s.size(); })), mayo::Option<std::size_t>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().inspect([](const mayo::i32&) {})), mayo::Option<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().map_or(0, [](mayo::i32&& v) { return v + 1; })), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().map_or_else([] { return 0; }, [](mayo::i32&& v) { return v + 1; })), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().map_or_default([](mayo::i32&& v) { return v + 1; })), mayo::i32>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<std::string>&&>().map_or_default([](std::string&& s) { return s.size(); })), std::size_t>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().and_other(std::declval<mayo::Option<std::size_t>>())), mayo::Option<std::size_t>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().and_then([](mayo::i32&& v) { return mayo::Option<std::size_t>{static_cast<std::size_t>(v)}; })), mayo::Option<std::size_t>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().filter([](const mayo::i32&) { return true; })), mayo::Option<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().or_other(std::declval<mayo::Option<mayo::i32>>())), mayo::Option<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().or_else([] { return mayo::Option<mayo::i32>{0}; })), mayo::Option<mayo::i32>>);
        static_assert(std::is_same_v<decltype(std::declval<mayo::Option<mayo::i32>&&>().xor_other(std::declval<mayo::Option<mayo::i32>>())), mayo::Option<mayo::i32>>);
        static_assert(requires(mayo::Option<MoveOnly>&& o) {
            std::move(o).filter([](const MoveOnly&) { return true; });
        });
        static_assert(!mayo::concepts::constructible_from<mayo::Option<MoveOnly>, mayo::Option<MoveOnly>&>);

        static_assert(noexcept(mayo::Option<NoThrowMove>{std::declval<mayo::Option<NoThrowMove>&&>()}));
        static_assert(!noexcept(mayo::Option<ThrowMove>{std::declval<mayo::Option<ThrowMove>&&>()}));
        static_assert(!noexcept(std::declval<mayo::Option<ThrowMove>&&>().unwrap_or(ThrowMove{})));
        static_assert(noexcept(std::declval<mayo::Option<mayo::i32>&>().unwrap_unchecked()));

        static_assert(noexcept(std::declval<mayo::Option<NoThrowMove>&>() = std::declval<mayo::Option<NoThrowMove>&&>()));
        static_assert(!noexcept(std::declval<mayo::Option<ThrowMove>&>() = std::declval<mayo::Option<ThrowMove>&&>()));
    }

    return 0;
}
