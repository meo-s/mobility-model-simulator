#pragma once
#ifndef _VEC2_HH
#define _VEC2_HH

#include <type_traits>
#include <utility>

#include "type_traits.hh"
#include "types.hh"

namespace linalg {

template <typename T>
class Vec2;

template <typename T>
class Vec3;

template <typename T>
using _is_Vec2_instance = is_template_instance<T, Vec2>;

template <typename T>
constexpr bool _is_Vec2_instance_v = _is_Vec2_instance<T>::value;

template <typename T>
class Vec2 {
 public:
  using ElementType = T;

  Vec2() = default;

  Vec2(T const x_, T const y_);

  Vec2(Vec2 const&) = default;

  template <typename U>
  explicit operator Vec2<U>() const;

  Vec2 operator-() const;

  template <typename U>
  Vec2& operator=(U const);

  Vec2& operator=(Vec2 const&) = default;

  template <typename U>
  Vec2& operator+=(U const);

  template <typename U>
  Vec2& operator-=(U const);

  template <typename U>
  Vec2& operator+=(Vec2<U> const&);

  template <typename U>
  Vec2& operator-=(Vec2<U> const&);

  template <typename U>
  Vec2& operator*=(U const);

  template <typename U>
  Vec2& operator/=(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator+(U const) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator-(U const) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator+(Vec2<U> const&) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator-(Vec2<U> const&) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator*(U const) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> operator/(U const) const;

  bool operator==(Vec2 const&) const noexcept;

  bool operator==(T const) const noexcept;

  bool operator!=(Vec2 const&) const noexcept;

  bool operator!=(T const) const noexcept;

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> add(U const) const;

  template <typename U>
  Vec2& add_(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> add(Vec2<U> const&) const;

  template <typename U>
  Vec2& add_(Vec2<U> const&);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> sub(U const) const;

  template <typename U>
  Vec2& sub_(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> sub(Vec2<U> const&) const;

  template <typename U>
  Vec2& sub_(Vec2<U> const&);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> scale(U const) const;

  template <typename U>
  Vec2& scale_(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  Vec2<R> div(U const) const;

  template <typename U>
  Vec2& div_(U const);

  template <typename U, typename R = std::common_type_t<T, U>>
  R dot(Vec2<U> const&) const;

  T dotSelf() const noexcept;

  std::conditional_t<std::is_same_v<T, f32>, f32, f64> norm(u8 p = 2) const;

  Vec2 normalize() const;

  Vec2& normalize_();

  Vec3<T> xyz(T const z = T{0}) const;

  static Vec2 full(T const) noexcept;

  static Vec2 zero() noexcept;

  T x;

  T y;
};

}  // namespace linalg

#include "vec2.hh.inl"

using Vec2i = linalg::Vec2<i32>;
using Vec2l = linalg::Vec2<i64>;
using Vec2u = linalg::Vec2<u32>;
using Vec2ul = linalg::Vec2<u64>;
using Vec2f = linalg::Vec2<f32>;
using Vec2d = linalg::Vec2<f64>;

#endif
