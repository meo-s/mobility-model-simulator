#pragma once
#ifndef _SIMUL_ROTARY_WING_UAV_PILOT_HH
#define _SIMUL_ROTARY_WING_UAV_PILOT_HH

#include "physics/mass-point.hh"
#include "uav-pilot.hh"

class RotaryWingUAVPilot : public UAVPilot {
 public:
  enum class UAVStatus : u8 {
    FLYING = 0,
    HOVERING = 1,
    ON_MISSION = 2,
  };

  RotaryWingUAVPilot(UAV&, UAVControlTower&);

  void update(f32) override;

 private:
  void switchToHovering(f32);

  void flyToDestination(f32);

  void stopFlying(f32);

 private:
  UAVStatus _uav_status;
};

#endif
