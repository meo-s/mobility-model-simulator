#pragma once
#ifndef _SIMUL_COVERAGE_MAP_HH
#define _SIMUL_COVERAGE_MAP_HH

#include <unordered_map>
#include <utility>

#include "coverage-map-cell.hh"
#include "fundamental/types.hh"
#include "grid-map.hh"
#include "protos/mobmodel-simulator.pb.h"

class CoverageMap {
 public:
  constexpr static auto APPROX_MAP_HEIGHT = 100;
  constexpr static auto APPROX_MAP_WIDTH = 100;

  CoverageMap() = default;

  CoverageMap(CoverageMap&) = default;

  CoverageMap(CoverageMap&&) = default;

  ~CoverageMap() = default;

  CoverageMap& operator=(CoverageMap&) = default;

  CoverageMap& operator=(CoverageMap&&) = default;

  void init(pb::CoverageMapConfig const&);

  f64 getMapHeight() const noexcept;

  f64 getMapWidth() const noexcept;

  GridMap<CoverageMapCell<u8>> const& getFineGridMap() const;

  GridMap<CoverageMapCell<u32>> const& getApproxGridMap() const;

  f64 getCoverageRatio() const noexcept;

  void visit(Rect2d const&, Rect2d const* = nullptr) noexcept;

 private:
  GridMap<CoverageMapCell<u8>> _fine_grid_map;

  GridMap<CoverageMapCell<u32>> _approx_grid_map;

  f64 _map_height;

  f64 _map_width;

  sz _n_visit_cells;
};

#endif
