#pragma once
#ifndef _VEC2_HH_INL
#define _VEC2_HH_INL

#include <cmath>
#include <stdexcept>

#ifndef _VEC2_HH
#include "vec2.hh"
#endif

namespace linalg {

template <typename T>
Vec2<T>::Vec2(T const x_, T const y_)
    : x{x_}
    , y{y_} {}

template <typename T>
template <typename U>
Vec2<T>::operator Vec2<U>() const {
  return {static_cast<U>(x), static_cast<U>(y)};
}

template <typename T>
Vec2<T> Vec2<T>::operator-() const {
  if constexpr (std::is_const_v<T>) {
    throw std::overflow_error{"unsigned type have no negation."};
  }
  return {-x, -y};
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator=(U const v_) {
  auto const& v = static_cast<T>(v_);
  x = v;
  y = v;
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator+=(U const v) {
  x += v;
  y += v;
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator-=(U const v) {
  x -= v;
  y -= v;
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator+=(Vec2<U> const& other) {
  x += other.x;
  y += other.y;
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator-=(Vec2<U> const& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator*=(U const v) {
  x *= static_cast<T>(v);
  y *= static_cast<T>(v);
  return *this;
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::operator/=(U const v) {
  x /= v;
  y /= v;
  return *this;
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator+(U const rhs) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator+=(rhs));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator-(U const rhs) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator-=(rhs));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator+(Vec2<U> const& rhs) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator+=(rhs));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator-(Vec2<U> const& rhs) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator-=(rhs));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator*(U const v) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator*=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::operator/(U const v) const {
  return const_cast<Vec2<R>&&>(Vec2<R>{*this}.operator/=(v));
}

template <typename T>
bool Vec2<T>::operator==(Vec2 const& rhs) const noexcept {
  return isclose(x, rhs.x) && isclose(y, rhs.y);
}

template <typename T>
bool Vec2<T>::operator==(T const rhs) const noexcept {
  return isclose(x, rhs) && isclose(y, rhs);
}

template <typename T>
bool Vec2<T>::operator!=(Vec2 const& rhs) const noexcept {
  return !isclose(x, rhs.x) || !isclose(y, rhs.y);
}

template <typename T>
bool Vec2<T>::operator!=(T const rhs) const noexcept {
  return !isclose(x, rhs) || !isclose(y, rhs);
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::add(U const v) const {
  return operator+<U, R>(v);
}

template <typename T>
template <typename U>
auto Vec2<T>::add_(U const v) -> Vec2& {
  return const_cast<Vec2&>(operator+=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::add(Vec2<U> const& rhs) const {
  return operator+<U, R>(rhs);
}

template <typename T>
template <typename U>
auto Vec2<T>::add_(Vec2<U> const& v) -> Vec2& {
  return const_cast<Vec2&>(operator+=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::sub(U const v) const {
  return operator-<U, R>(v);
}

template <typename T>
template <typename U>
auto Vec2<T>::sub_(U const v) -> Vec2& {
  return const_cast<Vec2&>(operator-=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::sub(Vec2<U> const& other) const {
  return operator-<U, R>(other);
}

template <typename T>
template <typename U>
auto Vec2<T>::sub_(Vec2<U> const& v) -> Vec2& {
  return const_cast<Vec2&>(operator-=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::scale(U const v) const {
  return operator*<U, R>(v);
}

template <typename T>
template <typename U>
auto Vec2<T>::scale_(U const v) -> Vec2& {
  return const_cast<Vec2&>(operator*=(v));
}

template <typename T>
template <typename U, typename R>
Vec2<R> Vec2<T>::div(U const v) const {
  return operator/<U, R>(v);
}

template <typename T>
template <typename U>
Vec2<T>& Vec2<T>::div_(U const rhs) {
  return (*this /= rhs);
}

template <typename T>
template <typename U, typename R>
R Vec2<T>::dot(Vec2<U> const& rhs) const {
  return (x * rhs.x) + (y * rhs.y);
}

template <typename T>
T Vec2<T>::dotSelf() const noexcept {
  return dot(*this);
}

template <typename T>
std::conditional_t<std::is_same_v<T, f32>, f32, f64> Vec2<T>::norm(u8 p) const {
  auto const& v = std::pow(x, p) + std::pow(y, p);
  return (1 < p) ? std::pow(v, 1. / p) : v;
}

template <typename T>
Vec2<T> Vec2<T>::normalize() const {
  return Vec2{*this}.normalize_();
}

template <typename T>
Vec2<T>& Vec2<T>::normalize_() {
  auto const norm_ = norm();
  x /= norm_;
  y /= norm_;
  return *this;
}

template <typename T>
auto Vec2<T>::xyz(T const z) const -> Vec3<T> {
  return {x, y, z};
}

template <typename T>
auto Vec2<T>::full(T const fill_value) noexcept -> Vec2 {
  return {fill_value, fill_value};
}

template <typename T>
auto Vec2<T>::zero() noexcept -> Vec2 {
  return full(0);
}

}  // namespace linalg

#endif
