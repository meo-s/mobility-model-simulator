#include "mobility-model-simulator.hh"

#include <array>
#include <cstdio>

#include "camera.hh"
#include "rotary-wing-uav-pilot.hh"
#include "simul/coverage-map.hh"
#include "simul/mobmodels/random-waypoint-planner.hh"
#include "uav-pilot.hh"
#include "uav.hh"

bool MobilityModelSimulator::init(pb::MobilityModelSimulatorConfig const& cfg) {
  MissionPlanner* p_mission_planner = nullptr;
  switch (cfg.mobmodel()) {
  case pb::MobilityModel::RANDOM_WAYPOINT:
    p_mission_planner = new RandomWaypointPlanner{*this};
    break;
  default:
    return false;
  }

  _simulation_time = 0.;
  _mass_point_world.clear();
  _coverage_map.init(cfg.coverage_map_cfg());
  _uav_control_tower.init(p_mission_planner);
  _uav_builder.useConfig(cfg.uav_cfg());
  _uavs.clear();
  for (auto i = 0; i < cfg.n_uavs(); ++i) {
    auto uav_tag = std::array<char, 16>{};
    std::snprintf(uav_tag.data(), uav_tag.size(), "UAV-%03d", i);

    auto p_uav = _uav_builder.build();
    _mass_point_world.addMassPoint(uav_tag.data(), p_uav->getMassPointPtr());

    auto p_uav_pilot = std::make_unique<RotaryWingUAVPilot>(*p_uav, _uav_control_tower);
    _uavs.emplace_back(uav_tag.data(), p_uav, std::move(p_uav_pilot));

    auto const x_initial = static_cast<float>(_coverage_map.getMapWidth() / 2);
    p_uav->setPositionXY({x_initial, 0.f});

    // Set initial destination of UAV as initial position of UAV.
    _uav_control_tower.getMissionOf(*p_uav).addDestination(p_uav->getPositionXY());
  }

  return true;
}

f64 MobilityModelSimulator::getSimulationTime() const noexcept {
  return _simulation_time;
}

MassPointWorld const& MobilityModelSimulator::getMassPointWorld() const {
  return _mass_point_world;
}

MassPointWorld& MobilityModelSimulator::getMassPointWorld() {
  return _mass_point_world;
}

CoverageMap const& MobilityModelSimulator::getCoverageMap() const noexcept {
  return _coverage_map;
}

CoverageMap& MobilityModelSimulator::getCoverageMap() noexcept {
  return _coverage_map;
}

UAVControlTower const& MobilityModelSimulator::getUAVControlTower() const noexcept {
  return _uav_control_tower;
}

UAVControlTower& MobilityModelSimulator::getUAVControlTower() noexcept {
  return _uav_control_tower;
}

UAVBuilder const& MobilityModelSimulator::getUAVBuilder() const {
  return _uav_builder;
}

UAVBuilder& MobilityModelSimulator::getUAVBuilder() {
  return _uav_builder;
}

sz MobilityModelSimulator::getUAVCount() const noexcept {
  return _uavs.size();
}

OptRef<UAV const> MobilityModelSimulator::getUAV(sz index) const {
  if (index < _uavs.size()) {
    return *std::get<1>(_uavs[index]);
  }
  return std::nullopt;
}

OptRef<UAV> MobilityModelSimulator::getUAV(sz index) {
  if (index < _uavs.size()) {
    return *std::get<1>(_uavs[index]);
  }
  return std::nullopt;
}

OptRef<UAV const> MobilityModelSimulator::getUAV(std::string const& uav_tag) const {
  if (auto const iter = findUAVIter(uav_tag); iter != _uavs.cend()) {
    return *std::get<1>(*iter);
  }
  return std::nullopt;
}

OptRef<UAV> MobilityModelSimulator::getUAV(std::string const& uav_tag) {
  if (auto const iter = findUAVIter(uav_tag); iter != _uavs.end()) {
    return *std::get<1>(*iter);
  }
  return std::nullopt;
}

OptRef<std::shared_ptr<UAV> const> MobilityModelSimulator::getUAVPtr(sz index) {
  if (index < _uavs.size()) {
    return std::get<1>(_uavs[index]);
  }
  return std::nullopt;
}

OptRef<std::shared_ptr<UAV> const> MobilityModelSimulator::getUAVPtr(std::string const& uav_tag) {
  if (auto const iter = findUAVIter(uav_tag); iter != _uavs.end()) {
    return std::get<1>(*iter);
  }
  return std::nullopt;
}

OptRef<UAVPilot> MobilityModelSimulator::getUAVPilot(sz index) {
  if (index < _uavs.size()) {
    return *std::get<2>(_uavs[index]);
  }
  return std::nullopt;
}

void MobilityModelSimulator::update(f32 dt) {
  // record previous coverage region of UAV to avoid improper coverage map updating.
  auto uav_coverage_traces = std::vector<Rect2d>{getUAVCount()};
  for (auto i = 0; i < getUAVCount(); ++i) {
    UAV& uav = getUAV(i).value();
    uav_coverage_traces[i] = uav.getCameraFilm();
  }

  for (auto i = 0; i < getUAVCount(); ++i) {
    UAVPilot& uav_pilot = getUAVPilot(i).value();
    uav_pilot.update(dt);
  }

  _mass_point_world.update(dt);

  for (auto i = 0; i < getUAVCount(); ++i) {
    UAV& uav = getUAV(i).value();
    auto p_uav_coverage_trace = (0 < _simulation_time) ? &uav_coverage_traces[i] : nullptr;
    _coverage_map.visit(uav.getCameraFilm(), p_uav_coverage_trace);
  }

  _simulation_time += dt;
}

MobilityModelSimulator::UAVContainer::iterator MobilityModelSimulator::findUAVIter(std::string const& uav_tag) {
  auto const pred = [&uav_tag](auto const& tup) { return std::get<0>(tup) == uav_tag; };
  return std::find_if(_uavs.begin(), _uavs.end(), pred);
}

MobilityModelSimulator::UAVContainer::const_iterator
MobilityModelSimulator::findUAVIter(std::string const& uav_tag) const {
  auto const pred = [&uav_tag](auto const& tup) { return std::get<0>(tup) == uav_tag; };
  return std::find_if(_uavs.cbegin(), _uavs.cend(), pred);
}
