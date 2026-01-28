#pragma once

#include <cstddef>
#include <cstdint>

namespace mayo {
namespace types::core {
    /**
     * 符号付き8bit整数型
     */
    using i8 = std::int8_t;

    /**
     * 符号付き16bit整数型
     */
    using i16 = std::int16_t;

    /**
     * 符号付き32bit整数型
     */
    using i32 = std::int32_t;

    /**
     * 符号付き64bit整数型
     */
    using i64 = std::int64_t;

    /**
     * 符号なし8bit整数型
     */
    using u8 = std::uint8_t;

    /**
     * 符号なし16bit整数型
     */
    using u16 = std::uint16_t;

    /**
     * 符号なし32bit整数型
     */
    using u32 = std::uint32_t;

    /**
     * 符号なし64bit整数型
     */
    using u64 = std::uint64_t;

    /**
     * 符号付きサイズ型
     */
    using isize = std::ptrdiff_t;

    /**
     * 符号なしサイズ型
     */
    using usize = std::size_t;

    /**
     * 32bit浮動小数点型
     */
    using f32 = float;

    /**
     * 64bit浮動小数点型
     */
    using f64 = double;
} // namespace types::core

using types::core::i8;
using types::core::i16;
using types::core::i32;
using types::core::i64;

using types::core::u8;
using types::core::u16;
using types::core::u32;
using types::core::u64;

using types::core::isize;
using types::core::usize;

using types::core::f32;
using types::core::f64;
} // namespace mayo
