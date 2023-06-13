#ifndef _SIMUL_UAV_PILOT_HH
#define _SIMUL_UAV_PILOT_HH

#include "fundamental/types.hh"

class UAV;
class UAVControlTower;
class UAVMission;

class UAVPilot {
 public:
  UAVPilot() = delete;

  UAVPilot(UAV&, UAVControlTower&);

  UAVPilot(UAVPilot&) = delete;

  UAVPilot(UAVPilot&&) = default;

  virtual ~UAVPilot() = default;

  UAVPilot& operator=(UAVPilot&) = delete;

  UAVPilot& operator=(UAVPilot&&) = default;

  virtual void update(f32 dt) = 0;

 protected:
  UAV& getUAV();

  UAVControlTower& getUAVControlTower();

  UAVMission& getUAVMission();

 protected:
  UAV* _p_uav;

  UAVControlTower* _p_control_tower;
};

#endif
