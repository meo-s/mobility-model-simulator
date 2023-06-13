#include "mass-point.hh"

auto MassPoint::getMass() const noexcept -> NumberType {
  return _mass;
}

auto MassPoint::setMass(NumberType new_mass) noexcept -> NumberType {
  return std::exchange(_mass, new_mass);
}

auto MassPoint::getPosition() const noexcept -> VectorType const& {
  return _position;
}

auto MassPoint::setPosition(VectorType const& new_position) noexcept -> VectorType {
  return std::exchange(_position, new_position);
}

auto MassPoint::getVelocity() const noexcept -> VectorType const& {
  return _velocity;
}

auto MassPoint::setVelocity(VectorType const& new_velocity) noexcept -> VectorType {
  return std::exchange(_velocity, new_velocity);
}

auto MassPoint::getForce() const noexcept -> VectorType const& {
  return _force;
}

auto MassPoint::setForce(VectorType const& new_force) noexcept -> VectorType {
  return std::exchange(_force, new_force);
}

void MassPoint::applyForce(VectorType const& new_force) noexcept {
  _force += new_force;
}

auto MassPoint::resetForce() noexcept -> VectorType {
  return std::exchange(_force, VectorType::zero());
}

auto MassPoint::getThrottle() const noexcept -> NumberType const& {
  return _throttle;
}

auto MassPoint::setThrottle(NumberType new_throttle) noexcept -> NumberType {
  return std::exchange(_throttle, new_throttle);
}
