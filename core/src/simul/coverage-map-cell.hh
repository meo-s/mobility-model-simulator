#pragma once
#ifndef _SIMUL_COVERAGE_MAP_CELL_HH
#define _SIMUL_COVERAGE_MAP_CELL_HH

#include "fundamental/types.hh"

template <typename T>
class CoverageMapCell {
 public:
  CoverageMapCell() = default;

  CoverageMapCell(CoverageMapCell const&) = default;

  ~CoverageMapCell() = default;

  CoverageMapCell& operator=(CoverageMapCell const&) = default;

  T getVisitCount() const noexcept;

  void setVisitCount(T) noexcept;

  T increaseVisitCount(T = 1) noexcept;

 public:
  T _n_visits;
};

#include "coverage-map-cell.hh.inl"

#endif
