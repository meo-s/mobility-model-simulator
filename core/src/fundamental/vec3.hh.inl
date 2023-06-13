#pragma once
#include "fundamental/types.hh"
#ifndef _VEC3_HH_INL
#define _VEC3_HH_INL

#include <cmath>

#ifndef _VEC3_HH
#include "vec3.hh"
#endif

namespace linalg {

template <typename T>
Vec3<T>::Vec3(T const x_, T const y_, T const z_)
    : x{x_}
    , y{y_}
    , z{z_} {}

template <typename T>
template <typename U>
Vec3<T>::Vec3(Vec3<U> const& other)
    : x{static_cast<T>(other.x)}
    , y{static_cast<T>(other.y)}
    , z{static_cast<T>(other.z)} {}

template <typename T>
template <typename U>
Vec3<T>::operator Vec3<U>() const {
  return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
}

template <typename T>
template <typename>
Vec3<T> Vec3<T>::operator-() const {
  return {-x, -y, -z};
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator+=(U const v) {
  x += v;
  y += v;
  z += v;
  return *this;
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator-=(U const v) {
  x -= v;
  y -= v;
  z -= v;
  return *this;
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator+=(Vec3<U> const& other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator-=(Vec3<U> const& other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator*=(U const v) {
  x *= v;
  y *= v;
  z *= v;
  return *this;
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::operator/=(U const v) {
  x /= v;
  y /= v;
  z /= v;
  return *this;
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator+(U const rhs) const noexcept {
  return static_cast<Vec3<R>>(clone() += rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator-(U const rhs) const noexcept {
  return static_cast<Vec3<R>>(clone() -= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator+(Vec3<U> const& rhs) const noexcept {
  return static_cast<Vec3<R>>(clone() += rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator-(Vec3<U> const& rhs) const noexcept {
  return static_cast<Vec3<R>>(clone() -= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator*(U const rhs) const noexcept {
  return static_cast<Vec3<R>>(clone() *= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::operator/(U const rhs) const {
  return static_cast<Vec3<R>>(clone() /= rhs);
}

template <typename T>
bool Vec3<T>::operator==(Vec3<T> const& rhs) const noexcept {
  return isclose(x, rhs.x) && isclose(y, rhs.y) && isclose(z, rhs.z);
}

template <typename T>
bool Vec3<T>::operator==(T const rhs) const noexcept {
  return isclose(x, rhs) && isclose(y, rhs) && isclose(z, rhs);
}

template <typename T>
bool Vec3<T>::operator!=(Vec3<T> const& rhs) const noexcept {
  return !(*this == rhs);
}

template <typename T>
bool Vec3<T>::operator!=(T rhs) const noexcept {
  return !(*this == rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::add(U const rhs) const noexcept {
  return static_cast<Vec3<R>&&>(*this + rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::add_(U const rhs) noexcept {
  return (*this += rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::add(Vec3<U> const& rhs) const noexcept {
  return static_cast<Vec3<R>&&>(*this + rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::add_(Vec3<U> const& rhs) noexcept {
  return (*this += rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::sub(U const rhs) const noexcept {
  return operator-<U, R>(rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::sub_(U const rhs) noexcept {
  return (*this -= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::sub(Vec3<U> const& rhs) const noexcept {
  return operator-<U, R>(rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::sub_(Vec3<U> const& rhs) noexcept {
  return (*this -= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::scale(U const rhs) const noexcept {
  return operator*<U, R>(rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::scale_(U const rhs) noexcept {
  return (*this *= rhs);
}

template <typename T>
template <typename U, typename R>
Vec3<R> Vec3<T>::div(U const rhs) const {
  return operator/<U, R>(rhs);
}

template <typename T>
template <typename U>
Vec3<T>& Vec3<T>::div_(U const rhs) {
  return (*this /= rhs);
}

template <typename T>
template <typename U, typename R>
R Vec3<T>::dot(Vec3<U> const& rhs) const noexcept {
  return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

template <typename T>
std::conditional_t<std::is_same_v<T, f32>, f32, f64> Vec3<T>::norm(u8 p) const {
  auto const v = std::pow(x, p) + std::pow(y, p) + std::pow(z, p);
  return (1 < p) ? std::pow(v, 1. / p) : v;
}

template <typename T>
Vec3<T> Vec3<T>::normalize() const {
  return (*this / norm());
}

template <typename T>
Vec3<T>& Vec3<T>::normalize_() {
  return (*this /= norm());
}

template <typename T>
Vec3<T> Vec3<T>::full(T const value) noexcept {
  return {value, value, value};
}

template <typename T>
Vec3<T> Vec3<T>::zero() noexcept {
  return Vec3<T>::full(0);
}

template <typename T, typename U, typename R>
Vec3<R> cross(Vec3<T> const& v1, Vec3<U> const& v2) {
  return {
      static_cast<R>(v1.y * v2.z - v1.z * v2.y), static_cast<R>(-(v1.x * v2.z - v1.z * v2.x)),
      static_cast<R>(v1.x * v2.y - v1.y * v2.x)};
}

template <typename T>
Vec2<T> Vec3<T>::xy() const noexcept {
  return {x, y};
}

template <typename T>
Vec3<T> Vec3<T>::clone() const noexcept {
  return *this;
}

}  // namespace linalg

#endif
