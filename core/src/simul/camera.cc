#include "camera.hh"

#include <cmath>

constexpr auto PI = 3.141592653589793116;
constexpr auto PIf = static_cast<float>(PI);

Camera::Camera(Precision hfov, Precision vfov) noexcept
    : _tan_hfov{std::tan(hfov * PIf / 360)}
    , _tan_vfov{std::tan(vfov * PIf / 360)} {}

Camera::Precision Camera::getHFOV() const noexcept {
  return std::atan(_tan_hfov) / PI * 360;
}

void Camera::setHFOV(Precision hfov) noexcept {
  _tan_hfov = std::tan(hfov * PI / 360);
}

Camera::Precision Camera::getVFOV() const noexcept {
  return std::atan(_tan_vfov) / PI * 360;
}

void Camera::setVFOV(Precision vfov) noexcept {
  _tan_vfov = std::tan(vfov * PI / 360);
}

void Camera::setFOV(Precision hfov, Precision vfov) noexcept {
  setHFOV(hfov);
  setVFOV(vfov);
}

Rect2<Camera::Precision> Camera::getFilm(Precision distance) const noexcept {
  auto const hw = _tan_hfov * distance;
  auto const hh = _tan_vfov * distance;
  return {-hw, -hh, 2 * hw, 2 * hh};
}
