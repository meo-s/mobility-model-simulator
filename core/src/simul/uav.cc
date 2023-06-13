#include "uav.hh"

#include <memory>

#include "camera.hh"

MassPoint const& UAV::getMassPoint() const noexcept {
  return *_p_mass_point;
}

MassPoint& UAV::getMassPoint() noexcept {
  return *_p_mass_point;
}

std::shared_ptr<MassPoint> const& UAV::getMassPointPtr() noexcept {
  return _p_mass_point;
}

void UAV::setMassPointPtr(std::shared_ptr<MassPoint> const& p_new_mass_point) noexcept {
  auto p_old_mass_point = _p_mass_point.get();
  _p_mass_point = p_new_mass_point;
}

void UAV::setMassPointPtr(MassPoint* p_new_mass_point) noexcept {
  return setMassPointPtr(std::shared_ptr<MassPoint>{p_new_mass_point});
}

linalg::Vec2<MassPoint::VectorType::ValueType> UAV::getPositionXY() const {
  return getPosition().xy();
}

void UAV::setPositionXY(linalg::Vec2<MassPoint::VectorType::ValueType> const& xy_pos) {
  setPosition({xy_pos.x, xy_pos.y, getAltitude()});
}

MassPoint::VectorType const& UAV::getPosition() const {
  return _p_mass_point->getPosition();
}

void UAV::setPosition(MassPoint::VectorType const& position) {
  _p_mass_point->setPosition(position);
}

MassPoint::VectorType const& UAV::getVelocity() const {
  return _p_mass_point->getVelocity();
}

void UAV::setVelocity(MassPoint::VectorType const& velocity) {
  _p_mass_point->setVelocity(velocity);
}

f32 UAV::getAltitude() const {
  return getPosition().z;
}

void UAV::setAltitude(f32 altitude) {
  auto position = getPosition();
  position.z = altitude;
  setPosition(position);
}

f32 UAV::getMaxSpeed() const {
  return _p_mass_point->getThrottle();
}

void UAV::setMaxSpeed(f32 new_max_speed) {
  _p_mass_point->setThrottle(new_max_speed);
}

f32 UAV::getMaxThrust() const noexcept {
  return _max_thrust;
}

void UAV::setMaxThrust(f32 new_max_thurst) noexcept {
  _max_thrust = new_max_thurst;
}

f32 UAV::getMass() const {
  return _p_mass_point->getMass();
}

void UAV::setMass(f32 mass) {
  _p_mass_point->setMass(mass);
}

Camera const& UAV::getCamera() const noexcept {
  return _camera;
}

Camera& UAV::getCamera() noexcept {
  return _camera;
}

Rect2d UAV::getCameraFilm() const noexcept {
  return _camera.getFilm(getAltitude()) + getPositionXY();
}

void UAV::applyForce(MassPoint::VectorType const& force) {
  _p_mass_point->applyForce(force);
}
