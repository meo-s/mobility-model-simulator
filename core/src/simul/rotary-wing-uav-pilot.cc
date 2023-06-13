#include "rotary-wing-uav-pilot.hh"

#include <iostream>  // TODO(ME) 

#include "simul/uav-control-tower.hh"
#include "simul/uav-mission.hh"
#include "simul/uav.hh"

RotaryWingUAVPilot::RotaryWingUAVPilot(UAV& uav, UAVControlTower& uav_control_tower)
    : UAVPilot{uav, uav_control_tower}
    , _uav_status{UAVStatus::FLYING} {}

void RotaryWingUAVPilot::update(f32 dt) {
  switch (_uav_status) {
  case UAVStatus::FLYING: {
    switchToHovering(dt);
    break;
  }

  case UAVStatus::HOVERING: {
    auto& mission = getUAVMission();
    if (!mission.hasNextDestination()) {
      _p_control_tower->planMissionOf(getUAV());
    }
    if (mission.proceed()) {
      _uav_status = UAVStatus::ON_MISSION;
    }
    break;
  }

  case UAVStatus::ON_MISSION: {
    flyToDestination(dt);
    break;
  }
  }
}

void RotaryWingUAVPilot::switchToHovering(f32 dt) {
  auto const& vel = _p_uav->getVelocity();
  if (vel != vel.zero()) {
    stopFlying(dt);
  } else {
    _uav_status = UAVStatus::HOVERING;
  }
}

void RotaryWingUAVPilot::flyToDestination(f32 dt) {
  auto const& mission = getUAVMission();

  auto const& dest = mission.getCurrentDestination();
  auto const& cur_pos = _p_uav->getPositionXY();
  if ((dest - cur_pos).dotSelf() < 1.f) {
    _uav_status = UAVStatus::FLYING;
    return;
  }

  auto const spd = _p_uav->getVelocity().norm();
  auto const max_accel = _p_uav->getMaxThrust() / _p_uav->getMass();
  auto const req_stop_dist = .5f * max_accel * std::pow(spd / max_accel, 2);
  auto const dist = (dest - cur_pos).norm();
  if (req_stop_dist < dist) {
    if (spd != _p_uav->getMaxSpeed()) {
      auto const force = (dest - cur_pos).normalize() * (max_accel * _p_uav->getMass());
      _p_uav->applyForce({force.x, force.y, 0});
    }
  } else {
    stopFlying(dt);
  }
}

void RotaryWingUAVPilot::stopFlying(f32 dt) {
  auto const mass = _p_uav->getMass();
  auto const& vel = _p_uav->getVelocity();
  auto const spd = vel.norm();
  if (!isclose(spd, 0)) {
    auto const max_accel = _p_uav->getMaxThrust() / mass;
    auto const alpha = -std::min(spd / (max_accel * dt), 1.f);
    _p_uav->getMassPoint().applyForce(vel * ((1 / spd) * max_accel * alpha * mass));
  }
}
