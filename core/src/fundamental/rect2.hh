#pragma once
#ifndef _RECT2_HH
#define _RECT2_HH

#include "types.hh"
#include "vec2.hh"

template <typename T>
class Rect2 {
public:
  Rect2() = default;

  Rect2(T x_, T y_, T w_, T h_);

  Rect2(Rect2 const&) = default;

  template <typename U>
  explicit Rect2(Rect2<U> const&);

  template <typename U>
  explicit operator Rect2<U>() const;

  Rect2& operator=(Rect2 const&) noexcept = default;

  template <typename U>
  Rect2& operator+=(linalg::Vec2<U> const&);

  template <typename U>
  Rect2& operator-=(linalg::Vec2<U> const&);

  template <typename U, typename R = std::common_type_t<T, U>>
  Rect2<R> operator+(linalg::Vec2<U> const&) const;

  template <typename U, typename R = std::common_type_t<T, U>>
  Rect2<R> operator-(linalg::Vec2<U> const&) const;

  template <typename U>
  auto moveTo_(U x_, U y_) -> Rect2&;

  template <typename U>
  auto moveTo_(linalg::Vec2<U> const&) -> Rect2&;

  template <typename U, typename R = std::common_type_t<T, U>>
  auto moveTo(U x_, U y_) const -> Rect2<R>;

  template <typename U, typename R = std::common_type_t<T, U>>
  auto moveTo(linalg::Vec2<U> const&) const -> Rect2<R>;

  template <typename U, typename R = std::common_type_t<T, U>>
  Rect2<R> moveBy(linalg::Vec2<U> const&) const;

  template <typename U>
  Rect2& moveBy_(linalg::Vec2<U> const&);

  linalg::Vec2<T> getCenter() const noexcept;

  template <typename R = T>
  auto getArea() const -> R;

  template <typename U, typename R = std::common_type_t<T, U>>
  auto getIntersectionArea(Rect2<U> const& other) const -> R;

  T x;

  T y;

  T w;

  T h;
};

#include "rect2.hh.inl"

using Rect2i = Rect2<i32>;
using Rect2l = Rect2<i64>;
using Rect2u = Rect2<u32>;
using Rect2ul = Rect2<u64>;
using Rect2f = Rect2<f32>;
using Rect2d = Rect2<f64>;

#endif
