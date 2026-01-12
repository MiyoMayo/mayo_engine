#pragma once

#include <cstddef>
#include <cstdint>

namespace mayo {
namespace types::core {
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using isize = std::ptrdiff_t;
using usize = std::size_t;

using f32 = float;
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
