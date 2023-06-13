#pragma once
#ifndef _PHYSICS_MASS_POINT_HH
#define _PHYSICS_MASS_POINT_HH

#include <type_traits>

#include "fundamental/vec3.hh"

class MassPoint {
 public:
  using NumberType = f32;
  static_assert(std::is_floating_point_v<NumberType>, "NumberType must be floating point.");

  using VectorType = linalg::Vec3<NumberType>;

  MassPoint() noexcept = default;

  MassPoint(MassPoint const&) noexcept = default;

  ~MassPoint() noexcept = default;

  MassPoint& operator=(MassPoint const&) noexcept = default;

  NumberType getMass() const noexcept;

  NumberType setMass(NumberType) noexcept;

  VectorType const& getPosition() const noexcept;

  VectorType setPosition(VectorType const&) noexcept;

  VectorType const& getVelocity() const noexcept;

  VectorType setVelocity(VectorType const&) noexcept;

  VectorType const& getForce() const noexcept;

  VectorType setForce(VectorType const&) noexcept;

  void applyForce(VectorType const&) noexcept;

  VectorType resetForce() noexcept;

  NumberType const& getThrottle() const noexcept;

  NumberType setThrottle(NumberType) noexcept;

 private:
  NumberType _mass;

  VectorType _position;

  VectorType _velocity;

  VectorType _force;

  NumberType _throttle;
};

#endif
