#pragma once
#ifndef _RECT2_HH_INL
#define _RECT2_HH_INL

#include <cmath>

#ifndef _RECT2_HH
#include "rect2.hh"
#endif

template <typename T>
Rect2<T>::Rect2(T x_, T y_, T w_, T h_)
    : x{x_}
    , y{y_}
    , w{w_}
    , h{h_} {}

template <typename T>
template <typename U>
Rect2<T>::Rect2(Rect2<U> const& other)
    : x{other.x}
    , y{other.y}
    , w{other.w}
    , h{other.h} {}

template <typename T>
template <typename U>
Rect2<T>::operator Rect2<U>() const {
  return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h)};
}

template <typename T>
template <typename U>
Rect2<T>& Rect2<T>::operator+=(linalg::Vec2<U> const& v) {
  x += v.x;
  y += v.y;
  return *this;
}

template <typename T>
template <typename U>
Rect2<T>& Rect2<T>::operator-=(linalg::Vec2<U> const& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template <typename T>
template <typename U, typename R>
auto Rect2<T>::operator+(linalg::Vec2<U> const& v) const -> Rect2<R> {

  return const_cast<Rect2<R>&&>(Rect2<R>{*this}.operator+=(v));
}

template <typename T>
template <typename U, typename R>
auto Rect2<T>::operator-(linalg::Vec2<U> const& v) const -> Rect2<R> {
  return const_cast<Rect2<R>&&>(Rect2<R>{*this}.operator-=(v));
}

template <typename T>
template <typename U>
auto Rect2<T>::moveTo_(U x_, U y_) -> Rect2& {
  x = x_;
  y = y_;
  return *this;
}

template <typename T>
template <typename U>
auto Rect2<T>::moveTo_(linalg::Vec2<U> const& v) -> Rect2<T>& {
  x = v.x;
  y = v.y;
  return *this;
}

template <typename T>
template <typename U, typename R>
auto Rect2<T>::moveTo(U x_, U y_) const -> Rect2<R> {
  return Rect2<R>{*this}.moveTo_(x, y);
}

template <typename T>
template <typename U, typename R>
auto Rect2<T>::moveTo(linalg::Vec2<U> const& v) const -> Rect2<R> {
  return Rect2<R>{*this}.moveTo_(v);
}

template <typename T>
template <typename U, typename R>
Rect2<R> Rect2<T>::moveBy(linalg::Vec2<U> const& v) const {
  return (Rect2<R>{*this} += v);
}

template <typename T>
template <typename U>
Rect2<T>& Rect2<T>::moveBy_(linalg::Vec2<U> const& v) {
  return (*this += v);
}

template <typename T>
template <typename R>
R Rect2<T>::getArea() const {
  return static_cast<R>(w * h);
}

template <typename T>
linalg::Vec2<T> Rect2<T>::getCenter() const noexcept {
  return {x + w / 2, y + h / 2};
}

template <typename T>
template <typename U, typename R>
R Rect2<T>::getIntersectionArea(Rect2<U> const& other) const {
  auto const lxmax = std::min(static_cast<R>(x + w), static_cast<R>(other.x + other.w));
  auto const rxmin = std::max(static_cast<R>(x), static_cast<R>(other.x));
  auto const bymax = std::min(static_cast<R>(y + h), static_cast<R>(other.y + other.h));
  auto const tymin = std::max(static_cast<R>(y), static_cast<R>(other.y));

  // R could be unsigned type.
  auto const x_overlap = (rxmin < lxmax) ? lxmax - rxmin : R{0};
  auto const y_overlap = (tymin < bymax) ? bymax - tymin : R{0};

  return x_overlap * y_overlap;
}

#endif
