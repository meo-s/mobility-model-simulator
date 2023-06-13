#pragma once
#include <functional>
#ifndef _PHYSICS_MASS_POINT_WORLD_HH
#define _PHYSICS_MASS_POINT_WORLD_HH

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "fundamental/types.hh"

class MassPoint;

class MassPointWorld {
 public:
  using MassPointPtr = std::shared_ptr<MassPoint>;

  MassPointWorld() = default;

  MassPointWorld(MassPointWorld&) = delete;

  MassPointWorld(MassPointWorld&&) = default;

  MassPointWorld& operator=(MassPointWorld&) = delete;

  MassPointWorld& operator=(MassPointWorld&&) = default;

  std::optional<std::reference_wrapper<MassPoint const>> getMassPoint(std::string const&) const noexcept;

  std::optional<std::reference_wrapper<MassPoint>> getMassPoint(std::string const&) noexcept;

  std::optional<std::reference_wrapper<MassPointPtr const>> getMassPointPtr(std::string const&) noexcept;

  std::pair<bool, MassPoint&> addMassPoint(std::string_view const&, MassPointPtr const&);

  std::pair<bool, MassPoint&> addMassPoint(std::string_view const&, MassPoint*);

  bool removeMassPoint(std::string const&) noexcept;

  void update(f32);

  void clear();

 private:
  void updateMassPoint(MassPoint&, f32);

  std::unordered_map<std::string, MassPointPtr> _mass_points;
};

#endif
