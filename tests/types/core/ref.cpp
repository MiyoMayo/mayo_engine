#include "mayo/types/core/ref.hpp"
#include "mayo/types/core/numeric.hpp"

#include <cassert>
#include <compare>
#include <concepts>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

struct Base {
    int value = 0;
};

struct Derived : Base {
    explicit Derived(int v) {
        value = v;
    }
};

auto assign_seven(int& v) -> void {
    v = 7;
}

auto read_value(const int& v) -> int {
    return v;
}

auto main() -> mayo::i32 {
    {
        int value = 1;
        mayo::Ref<int> ref{value};

        assert(std::addressof(ref.get()) == std::addressof(value));
        assert(std::addressof(*ref) == std::addressof(value));

        ref.get() = 3;
        assert(value == 3);
    }

    {
        std::string text = "abc";
        mayo::Ref<std::string> ref{text};

        assert(ref->size() == 3);
        ref->push_back('d');
        assert(text == "abcd");
    }

    {
        int value = 0;
        mayo::Ref<int> ref{value};

        assign_seven(ref);
        assert(value == 7);
        assert(read_value(ref) == 7);
    }

    {
        int value = 10;
        mayo::Ref<int> mut_ref{value};
        mayo::Ref<const int> const_ref = mut_ref;

        assert(const_ref.get() == 10);

        mut_ref.get() = 11;
        assert(const_ref.get() == 11);
    }

    {
        int value = 4;
        mayo::Ref<int> a{value};
        mayo::Ref<int> b{a};
        mayo::Ref<int> c{std::move(a)};
        auto d = mayo::Ref{a};

        b.get() = 9;
        assert(value == 9);
        assert(c.get() == 9);
    }

    {
        Derived derived{42};
        mayo::Ref<Base> base_ref{derived};
        assert(base_ref->value == 42);
    }

    {
        int a = 1;
        int b = 2;
        const int c = 1;

        mayo::Ref<int> ra{a};
        mayo::Ref<int> rb{b};
        mayo::Ref<const int> rc{c};

        assert(ra == 1);
        assert(ra == rc);
        assert(rc == ra);
        assert(std::is_lt(ra <=> rb));
        assert(std::is_eq(ra <=> rc));
        assert(std::is_eq(rc <=> ra));
    }

    {
        static_assert(std::is_constructible_v<mayo::Ref<int>, int&>);
        static_assert(std::is_constructible_v<mayo::Ref<const int>, int&>);
        static_assert(std::is_constructible_v<mayo::Ref<Base>, Derived&>);

        static_assert(!std::is_constructible_v<mayo::Ref<int>, int&&>);
        static_assert(!std::is_constructible_v<mayo::Ref<const int>, int&&>);
        static_assert(!std::is_constructible_v<mayo::Ref<int>, const int&>);
        static_assert(!std::is_constructible_v<mayo::Ref<int>, double&>);

        static_assert(std::is_copy_constructible_v<mayo::Ref<int>>);
        static_assert(std::is_move_constructible_v<mayo::Ref<int>>);

        static_assert(!std::is_convertible_v<int&, mayo::Ref<int>>);
        static_assert(std::is_convertible_v<mayo::Ref<int>, int&>);

        static_assert(mayo::concepts::is_ref_type<mayo::Ref<int>>);
        static_assert(!mayo::concepts::is_ref_type<int>);
    }

    return 0;
}
