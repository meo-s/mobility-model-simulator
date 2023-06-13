#include "random-waypoint-planner.hh"

#include <random>

#include "simul/mobility-model-simulator.hh"
#include "simul/uav-mission.hh"
#include "simul/uav.hh"

RandomWaypointPlanner::RandomWaypointPlanner(MobilityModelSimulator& simulator)
    : MissionPlanner{simulator}
    , _random_engine{} {
  auto random_device = std::random_device{};
  _random_engine.seed(random_device());
}

void RandomWaypointPlanner::planMissionOf(UAV const& uav) {
  auto& mission = _p_simulator->getUAVControlTower().getMissionOf(uav);

  auto uniform_dist = std::uniform_real_distribution<double>{0.f, 1.f};
  auto const x = uniform_dist(_random_engine) * _p_simulator->getCoverageMap().getMapWidth();
  auto const y = uniform_dist(_random_engine) * _p_simulator->getCoverageMap().getMapHeight();
  mission.addDestination({static_cast<float>(x), static_cast<float>(y)});
}
