#include "uav-builder.hh"

#include <utility>

#include "camera.hh"
#include "uav.hh"

f32 UAVBuilder::getMass() const {
  if (!_opt_mass.has_value()) {
    throw std::logic_error{"UAVBuilder: Mass of UAV is not configured yet."};
  }
  return _opt_mass.value();
}

UAVBuilder& UAVBuilder::setMass(f32 new_mass) noexcept {
  _opt_mass = new_mass;
  return *this;
}

f32 UAVBuilder::getMaxThrust() const {
  if (!_opt_max_thrust.has_value()) {
    throw std::logic_error{"UAVBuilder: Max thrust of UAV is not configured yet."};
  }
  return _opt_max_thrust.value();
}

void UAVBuilder::setMaxThrust(f32 new_max_thurst) noexcept {
  _opt_max_thrust = new_max_thurst;
}

f32 UAVBuilder::getMaxSpeed() const noexcept { return _opt_max_speed.value_or(0); }

UAVBuilder& UAVBuilder::setMaxSpeed(f32 new_max_speed) noexcept {
  _opt_max_speed = new_max_speed;
  return *this;
}

f32 UAVBuilder::getAltitude() const {
  if (!_opt_altitude.has_value()) {
    throw std::logic_error{"UAVBuilder: Altitude of UAV is not configured yet."};
  }
  return _opt_altitude.value();
}

UAVBuilder& UAVBuilder::setAltitude(f32 altitude) noexcept {
  _opt_altitude = altitude;
  return *this;
}

f32 UAVBuilder::getCameraHFOV() const {
  if (!_opt_camera_hfov.has_value()) {
    throw std::logic_error{"UAVBuilder: HFOV of UAV's camera is not configured yet."};
  }
  return _opt_camera_hfov.value();
}

UAVBuilder& UAVBuilder::setCameraHFOV(f32 hfov) noexcept {
  _opt_camera_hfov = hfov;
  return *this;
}

f32 UAVBuilder::getCameraVFOV() const {
  if (!_opt_camera_vfov.has_value()) {
    throw std::logic_error{"UAVBuilder: VFOV of UAV's camera is not configured yet."};
  }
  return _opt_camera_vfov.value();
}

UAVBuilder& UAVBuilder::setCameraVFOV(f32 vfov) noexcept {
  _opt_camera_vfov = vfov;
  return *this;
}

UAVBuilder& UAVBuilder::setCamera(f32 hfov, f32 vfov) noexcept {
  setCameraHFOV(hfov);
  setCameraVFOV(vfov);
  return *this;
}

UAVBuilder& UAVBuilder::setCamera(pb::CameraConfig const& camera_cfg) noexcept {
  setCameraHFOV(camera_cfg.hfov());
  setCameraVFOV(camera_cfg.vfov());
  return *this;
}

UAVBuilder& UAVBuilder::useConfig(pb::UAVConfig const& uav_cfg) noexcept {
  setMass(uav_cfg.mass());
  setMaxSpeed(uav_cfg.max_speed());
  setMaxThrust(uav_cfg.max_thrust());
  setAltitude(uav_cfg.altitude());
  setCamera(uav_cfg.camera_cfg());
  return *this;
}

UAV* UAVBuilder::build() {
  auto p_mass_point = new MassPoint{};
  p_mass_point->setMass(getMass());
  p_mass_point->setThrottle(getMaxSpeed());

  auto p_uav = new UAV{};
  p_uav->setMassPointPtr(p_mass_point);
  p_uav->setAltitude(getAltitude());
  p_uav->setMaxThrust(getMaxThrust());
  p_uav->getCamera().setFOV(getCameraHFOV(), getCameraVFOV());

  return p_uav;
}