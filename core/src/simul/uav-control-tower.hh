#pragma once
#ifndef _SIMUL_UAV_CONTROL_TOWER_HH
#define _SIMUL_UAV_CONTROL_TOWER_HH

#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

#include "fundamental/optref.hh"
#include "fundamental/vec2.hh"
#include "simul/mobmodels/mission-planner.hh"
#include "simul/uav-mission.hh"

class UAV;

class UAVControlTower {
 public:
  UAVControlTower() = default;

  UAVControlTower(UAVControlTower&) = delete;

  UAVControlTower(UAVControlTower&&) = default;

  ~UAVControlTower() = default;

  UAVControlTower& operator=(UAVControlTower&) = delete;

  UAVControlTower& operator=(UAVControlTower&&) = default;

  void init(MissionPlanner* = nullptr);

  void setMissionPlanner(MissionPlanner*);

  void setMissionPlanner(std::unique_ptr<MissionPlanner>);

  OptRef<UAVMission const> getMissionOf(UAV const&) const;

  UAVMission& getMissionOf(UAV const&);

  void planMissionOf(UAV const&);

 private:
  std::unique_ptr<MissionPlanner> _p_mission_planner;

  std::unordered_map<UAV const*, std::unique_ptr<UAVMission>> _missions;
};

#endif
