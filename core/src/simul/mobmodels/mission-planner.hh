#pragma once
#ifndef _MOBMODELS_MISSION_PLANNER_HH
#define _MOBMODELS_MISSION_PLANNER_HH

#include "fundamental/vec2.hh"

class UAV;

class MobilityModelSimulator;

class MissionPlanner {
 public:
  MissionPlanner() = delete;

  MissionPlanner(MobilityModelSimulator& simulator)
      : _p_simulator{&simulator} {}

  MissionPlanner(MissionPlanner&) = delete;

  MissionPlanner(MissionPlanner&&) = default;

  virtual ~MissionPlanner() = default;

  MissionPlanner& operator=(MissionPlanner&) = delete;

  MissionPlanner& operator=(MissionPlanner&&) = default;

  virtual void planMissionOf(UAV const&) = 0;

 protected:
  MobilityModelSimulator* _p_simulator;
};

#endif
