#pragma once
#ifndef _SIMUL_UAV_MISSION_HH
#define _SIMUL_UAV_MISSION_HH

#include <list>

#include "fundamental/optref.hh"
#include "fundamental/vec2.hh"

class UAVMission {
 public:
  UAVMission();

  UAVMission(UAVMission&) = delete;

  UAVMission(UAVMission&&) = default;

  ~UAVMission() = default;

  UAVMission& operator=(UAVMission&) = delete;

  UAVMission& operator=(UAVMission&&) = default;

  void init();

  bool hasPreviousDestination() const noexcept;

  bool hasDestination() const noexcept;

  bool hasNextDestination() const noexcept;

  Vec2f const& getPreviousDestination() const noexcept;

  Vec2f const& getCurrentDestination() const noexcept;

  void addDestination(Vec2f const&);

  bool proceed();

 private:
  std::list<Vec2f> _dests;

  std::list<Vec2f>::const_iterator _cur_dest_iter;
};

#endif
