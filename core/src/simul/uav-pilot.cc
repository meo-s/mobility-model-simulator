#include "uav-pilot.hh"

#include "simul/uav-control-tower.hh"
#include "simul/uav.hh"

UAVPilot::UAVPilot(UAV& uav, UAVControlTower& control_tower)
    : _p_uav{&uav}
    , _p_control_tower{&control_tower} {}

UAV& UAVPilot::getUAV() {
  return *_p_uav;
}

UAVControlTower& UAVPilot::getUAVControlTower() {
  return *_p_control_tower;
}

UAVMission& UAVPilot::getUAVMission() {
  return _p_control_tower->getMissionOf(*_p_uav);
}
