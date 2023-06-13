#pragma once
#ifndef _SIMUL_UAV_HH
#define _SIMUL_UAV_HH

#include <memory>

#include "camera.hh"
#include "physics/mass-point.hh"

class UAV {
 public:
  UAV(UAV&) = delete;

  UAV(UAV&&) = default;

  ~UAV() = default;

  UAV& operator=(UAV&) = delete;

  UAV& operator=(UAV&&) = default;

  MassPoint const& getMassPoint() const noexcept;

  MassPoint& getMassPoint() noexcept;

  std::shared_ptr<MassPoint> const& getMassPointPtr() noexcept;

  linalg::Vec2<MassPoint::VectorType::ValueType> getPositionXY() const;

  void setPositionXY(linalg::Vec2<MassPoint::VectorType::ValueType> const& xy_pos);

  MassPoint::VectorType const& getPosition() const;

  void setPosition(MassPoint::VectorType const&);

  MassPoint::VectorType const& getVelocity() const;

  void setVelocity(MassPoint::VectorType const&);

  f32 getAltitude() const;

  void setAltitude(f32);

  f32 getMaxSpeed() const;

  void setMaxSpeed(f32);

  f32 getMass() const;

  void setMass(f32);

  void applyForce(MassPoint::VectorType const&);

  f32 getMaxThrust() const noexcept;

  void setMaxThrust(f32) noexcept;

  Camera const& getCamera() const noexcept;

  Camera& getCamera() noexcept;

  Rect2d getCameraFilm() const noexcept;

 protected:
  UAV() = default;

  void setMassPointPtr(std::shared_ptr<MassPoint> const&) noexcept;

  void setMassPointPtr(MassPoint*) noexcept;

 private:
  std::shared_ptr<MassPoint> _p_mass_point;

  f32 _max_thrust;

  Camera _camera;

  friend class UAVBuilder;
};

#endif
