#include "uav-control-tower.hh"

#include <optional>
#include <utility>

#include "simul/uav-mission.hh"

void UAVControlTower::init(MissionPlanner* p_mission_planner) {
  _p_mission_planner.reset(p_mission_planner);
  _missions.clear();
}

void UAVControlTower::setMissionPlanner(MissionPlanner* p) {
  _p_mission_planner.reset(p);
}

void UAVControlTower::setMissionPlanner(std::unique_ptr<MissionPlanner> p_new_mission_planner) {
  _p_mission_planner.reset(_p_mission_planner.release());
}

OptRef<UAVMission const> UAVControlTower::getMissionOf(UAV const& uav) const {
  auto const iter = _missions.find(&uav);
  return (iter != _missions.end()) ? OptRef<UAVMission const>(*iter->second) : OptRef<UAVMission const>{std::nullopt};
}

UAVMission& UAVControlTower::getMissionOf(UAV const& uav) {
  auto mission_iter = _missions.find(&uav);
  if (mission_iter == _missions.end()) {
    auto const [new_iter, _] = _missions.try_emplace(&uav, new UAVMission{});
    mission_iter = std::move(new_iter);
  }

  return *mission_iter->second;
}

void UAVControlTower::planMissionOf(UAV const& uav) {
  _p_mission_planner->planMissionOf(uav);
}
