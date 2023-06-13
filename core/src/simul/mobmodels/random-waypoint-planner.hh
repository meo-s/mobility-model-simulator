#pragma once
#ifndef _MOBMODELS_RANDOM_WAYPOINT_PLANNER_HH
#define _MOBMODELS_RANDOM_WAYPOINT_PLANNER_HH

#include <random>

#include "mission-planner.hh"

class MobilityModelSimulator;

class RandomWaypointPlanner : public MissionPlanner {
 public:
  RandomWaypointPlanner(MobilityModelSimulator&);

  RandomWaypointPlanner(RandomWaypointPlanner&) = delete;

  RandomWaypointPlanner(RandomWaypointPlanner&&) = default;

  virtual ~RandomWaypointPlanner() = default;

  RandomWaypointPlanner& operator=(RandomWaypointPlanner&) = delete;

  RandomWaypointPlanner& operator=(RandomWaypointPlanner&&) = default;

  void planMissionOf(UAV const&) override;

 private:
  std::mt19937 _random_engine;
};

#endif
