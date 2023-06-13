#pragma once
#ifndef _FUNDAMENTAL_TYPES_HH
#define _FUNDAMENTAL_TYPES_HH

#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#define _TYPES_DEFINE_NUM_UDL(IN_TYPE, OUT_TYPE)                                                                       \
  inline constexpr auto operator"" _##OUT_TYPE(IN_TYPE n)->OUT_TYPE { return static_cast<OUT_TYPE>(n); }

using i8 = std::int8_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, i8);

using u8 = std::uint8_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, u8);

using i16 = std::int16_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, i16);

using u16 = std::int32_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, u16);

using i32 = std::int32_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, i32);

using u32 = std::uint32_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, u32);

using i64 = std::int64_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, i64)

using u64 = std::uint64_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, u64)

using f32 = float;
_TYPES_DEFINE_NUM_UDL(long double, f32)

using f64 = double;
_TYPES_DEFINE_NUM_UDL(long double, f64)

using f128 = long double;
_TYPES_DEFINE_NUM_UDL(long double, f128)

using sz = std::size_t;
_TYPES_DEFINE_NUM_UDL(unsigned long long, sz)

using ssz = std::conditional_t<std::is_same_v<sz, u32>, i32, i64>;
_TYPES_DEFINE_NUM_UDL(unsigned long long, ssz)

#undef _TYPESL_DEFINE_NUM_UDL

template <typename T, typename U>
inline bool isclose(T const& lhs, U const& rhs) {
  if constexpr (std::is_floating_point_v<std::common_type_t<T, U>>) {
    constexpr T eps = std::numeric_limits<T>::epsilon();
    return std::abs(lhs - rhs) <= (eps * (1 + std::abs(lhs + rhs)));
  } else {
    return lhs == rhs;
  }
}

#endif
