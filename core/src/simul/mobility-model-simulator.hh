#pragma once
#ifndef _SIMUL_MOBILITY_MODEL_SIMULATOR_HH
#define _SIMUL_MOBILITY_MODEL_SIMULATOR_HH

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>

#include "fundamental/optref.hh"
#include "fundamental/rect2.hh"
#include "fundamental/types.hh"
#include "physics/mass-point-world.hh"
#include "protos/mobmodel-simulator.pb.h"
#include "simul/coverage-map.hh"
#include "simul/uav-builder.hh"
#include "simul/uav-control-tower.hh"
#include "simul/uav-pilot.hh"


class UAV;
class UAVControlTower;

class MobilityModelSimulator {
 public:
  using UAVContainer = std::vector<std::tuple<std::string, std::shared_ptr<UAV>, std::unique_ptr<UAVPilot>>>;

  MobilityModelSimulator() = default;

  MobilityModelSimulator(MobilityModelSimulator&) = delete;

  MobilityModelSimulator(MobilityModelSimulator&&) = default;

  ~MobilityModelSimulator() = default;

  MobilityModelSimulator& operator=(MobilityModelSimulator&) = delete;

  MobilityModelSimulator& operator=(MobilityModelSimulator&&) = default;

  bool init(pb::MobilityModelSimulatorConfig const&);

  f64 getSimulationTime() const noexcept;

  MassPointWorld const& getMassPointWorld() const;

  MassPointWorld& getMassPointWorld();

  CoverageMap const& getCoverageMap() const noexcept;

  CoverageMap& getCoverageMap() noexcept;

  UAVControlTower const& getUAVControlTower() const noexcept;

  UAVControlTower& getUAVControlTower() noexcept;

  UAVBuilder const& getUAVBuilder() const;

  UAVBuilder& getUAVBuilder();

  sz getUAVCount() const noexcept;

  OptRef<UAV const> getUAV(sz) const;

  OptRef<UAV> getUAV(sz);

  OptRef<UAV const> getUAV(std::string const&) const;

  OptRef<UAV> getUAV(std::string const&);

  OptRef<std::shared_ptr<UAV> const> getUAVPtr(sz);

  OptRef<std::shared_ptr<UAV> const> getUAVPtr(std::string const&);

  OptRef<UAVPilot> getUAVPilot(sz);

  void update(f32);

 private:
  UAVContainer::const_iterator findUAVIter(std::string const&) const;

  UAVContainer::iterator findUAVIter(std::string const&);

 private:
  f64 _simulation_time;

  MassPointWorld _mass_point_world;

  CoverageMap _coverage_map;

  UAVControlTower _uav_control_tower;

  UAVContainer _uavs;

  UAVBuilder _uav_builder;
};

#endif
