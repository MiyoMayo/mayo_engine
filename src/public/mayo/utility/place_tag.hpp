#pragma once

#include "mayo/types/core/basic_types.hpp"

namespace mayo {
namespace utility {
    /**
     * 値なし状態タグ
     */
    struct Valueless {
        explicit constexpr Valueless() noexcept = default;
    };

    /**
     * @brief 値なし状態タグ
     */
    inline constexpr auto VALUELESS = Valueless{};

    /**
     * 構築タグ
     */
    struct InPlace {
        explicit constexpr InPlace() noexcept = default;
    };

    /**
     * @brief 構築タグ
     */
    inline constexpr auto IN_PLACE = InPlace{};

    /**
     * 構築index指定タグ
     */
    template <usize Index>
    struct InPlaceIndex {
        explicit constexpr InPlaceIndex() = default;
    };

    /**
     * @brief 構築index指定タグ
     */
    template <usize Index>
    inline constexpr auto IN_PLACE_INDEX = InPlaceIndex<Index>{};

    /**
     *構築型指定タグ
     */
    template <class T>
    struct InPlaceType {
        explicit constexpr InPlaceType() = default;
    };

    /**
     * @brief 構築型指定タグ
     */
    template <class T>
    inline constexpr auto IN_PLACE_TYPE = InPlaceType<T>{};
} // namespace utility

using utility::Valueless;
using utility::VALUELESS;

using utility::InPlace;
using utility::IN_PLACE;

using utility::InPlaceIndex;
using utility::IN_PLACE_INDEX;

using utility::InPlaceType;
using utility::IN_PLACE_TYPE;
} // namespace mayo
