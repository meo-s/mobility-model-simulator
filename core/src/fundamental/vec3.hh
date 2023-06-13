#pragma once
#ifndef _VEC3_HH
#define _VEC3_HH

#include <type_traits>
#include <utility>

#include "types.hh"
#include "vec2.hh"

namespace linalg {

template <typename T>
class Vec3 {
 public:
  using ValueType = T;

  Vec3() = default;

  Vec3(T const x_, T const y_, T const z_);

  Vec3(Vec3 const&) = default;

  ~Vec3() = default;

  template <typename U>
  explicit Vec3(Vec3<U> const&);

  template <typename U>
  explicit operator Vec3<U>() const;

  template <typename = std::enable_if_t<std::negation_v<std::is_unsigned<T>>>>
  Vec3 operator-() const;

  template <typename U>
  Vec3& operator=(U const);

  Vec3& operator=(Vec3 const&) = default;

  template <typename U>
  Vec3& operator+=(U const);

  template <typename U>
  Vec3& operator-=(U const);

  template <typename U>
  Vec3& operator+=(Vec3<U> const&);

  template <typename U>
  Vec3& operator-=(Vec3<U> const&);

  template <typename U>
  Vec3& operator*=(U const);

  template <typename U>
  Vec3& operator/=(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator+(U const) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator-(U const) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator+(Vec3<U> const&) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator-(Vec3<U> const&) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator*(U const) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> operator/(U const) const;

  bool operator==(Vec3 const&) const noexcept;

  bool operator==(T const) const noexcept;

  bool operator!=(Vec3 const&) const noexcept;

  bool operator!=(T const) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> add(U const) const noexcept;

  template <typename U>
  Vec3& add_(U const) noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> add(Vec3<U> const&) const noexcept;

  template <typename U>
  Vec3& add_(Vec3<U> const&) noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> sub(U const) const noexcept;

  template <typename U>
  Vec3& sub_(U const) noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> sub(Vec3<U> const&) const noexcept;

  template <typename U>
  Vec3& sub_(Vec3<U> const&) noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> scale(U const) const noexcept;

  template <typename U>
  Vec3& scale_(U const) noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec3<R> div(U const) const;

  template <typename U>
  Vec3& div_(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  R dot(Vec3<U> const&) const noexcept;

  std::conditional_t<std::is_same_v<T, f32>, f32, f64> norm(u8 p = 2) const;

  Vec3 normalize() const;

  Vec3& normalize_();

  Vec2<T> xy() const noexcept;

  Vec3 clone() const noexcept;

  static Vec3 full(T const) noexcept;

  static Vec3 zero() noexcept;

  T x;

  T y;

  T z;
};

template <typename T, typename U, typename R = std::common_type_t<T, U>>
Vec3<R> cross(Vec3<T> const&, Vec3<U> const&);

}  // namespace linalg

#include "vec3.hh.inl"

using Vec3i = linalg::Vec3<i32>;
using Vec3l = linalg::Vec3<i64>;
using Vec3u = linalg::Vec3<u32>;
using Vec3ul = linalg::Vec3<u64>;
using Vec3f = linalg::Vec3<f32>;
using Vec3d = linalg::Vec3<f64>;

#endif
