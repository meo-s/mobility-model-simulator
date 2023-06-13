#pragma once
#ifndef _SIMUL_UAV_BUILDER_HH
#define _SIMUL_UAV_BUILDER_HH

#include <optional>
#include <utility>

#include "fundamental/types.hh"
#include "protos/mobmodel-simulator.pb.h"

class UAV;

class UAVBuilder {
public:
  UAVBuilder() = default;

  UAVBuilder(UAVBuilder const&) = default;

  UAVBuilder& operator=(UAVBuilder const&) = default;

  f32 getCameraHFOV() const;

  UAVBuilder& setCameraHFOV(f32) noexcept;

  f32 getCameraVFOV() const;

  UAVBuilder& setCameraVFOV(f32) noexcept;

  UAVBuilder& setCamera(f32 hfov, f32 vfov) noexcept;

  UAVBuilder& setCamera(pb::CameraConfig const&) noexcept;

  f32 getMass() const;

  UAVBuilder& setMass(f32) noexcept;

  f32 getMaxThrust() const;

  void setMaxThrust(f32) noexcept;

  f32 getMaxSpeed() const noexcept;

  UAVBuilder& setMaxSpeed(f32) noexcept;

  f32 getAltitude() const;

  UAVBuilder& setAltitude(f32) noexcept;

  UAVBuilder& useConfig(pb::UAVConfig const&) noexcept;

  UAV* build();

private:
  std::optional<f32> _opt_mass;

  std::optional<f32> _opt_max_speed;

  std::optional<f32> _opt_max_thrust;

  std::optional<f32> _opt_altitude;

  std::optional<f32> _opt_camera_hfov;

  std::optional<f32> _opt_camera_vfov;
};

#endif
