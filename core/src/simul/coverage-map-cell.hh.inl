#pragma once
#ifndef _SIMUL_COVERAGE_MAP_CELL_HH_INL
#define _SIMUL_COVERAGE_MAP_CELL_HH_INL

#ifndef _SIMUL_COVERAGE_MAP_CELL_HH
#include "coverage-map-cell.hh"
#endif

template <typename T>
T CoverageMapCell<T>::getVisitCount() const noexcept {
  return _n_visits;
}

template <typename T>
void CoverageMapCell<T>::setVisitCount(T n_visits) noexcept {
  _n_visits = n_visits;
}

template <typename T>
T CoverageMapCell<T>::increaseVisitCount(T amount) noexcept {
  _n_visits += amount;
  return _n_visits;
}

#endif
