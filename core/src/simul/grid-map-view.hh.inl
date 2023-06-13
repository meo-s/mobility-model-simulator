#pragma once
#ifndef _SIMUL_GRID_MAP_VIEW_HH_INL
#define _SIMUL_GRID_MAP_VIEW_HH_INL

#ifndef _SIMUL_GRID_MAP_VIEW_HH
#include "grid-map-view.hh"
#endif

#include <cstring>

template <typename T>
GridMapView<T>::GridMapView(GridMapType& grid_map, u32 row_lbound, u32 row_ubound, u32 col_lbound, u32 col_ubound)
    : _p_grid_map{&grid_map}
    , _row_lbound{row_lbound}
    , _row_ubound{row_ubound}
    , _col_lbound{col_lbound}
    , _col_ubound{col_ubound} {}

template <typename T>
bool GridMapView<T>::operator==(GridMapView const& other) const {
  return std::memcmp(this, &other, sizeof(*this)) == 0;
}

template <typename T>
bool GridMapView<T>::operator!=(GridMapView const& other) const {
  return std::memcmp(this, &other, sizeof(*this)) != 0;
}

template <typename T>
auto GridMapView<T>::getGridMap() const -> GridMapType const& {
  return _p_grid_map;
}

template <typename T>
auto GridMapView<T>::getGridMap() -> GridMapType& {
  return _p_grid_map;
}

template <typename T>
u32 GridMapView<T>::getRowUpperBound() const {
  return _row_ubound;
}

template <typename T>
u32 GridMapView<T>::getRowLowerBound() const {
  return _row_lbound;
}

template <typename T>
u32 GridMapView<T>::getColUpperBound() const {
  return _col_ubound;
}

template <typename T>
u32 GridMapView<T>::getColLowerBound() const {
  return _col_lbound;
}

template <typename T>
u32 GridMapView<T>::getRows() const {
  return _row_ubound - _row_lbound;
}

template <typename T>
u32 GridMapView<T>::getCols() const {
  return _col_ubound - _col_lbound;
}

template <typename T>
Vec2u GridMapView<T>::getAbsoluteIndex(u32 row, u32 col) const {
  return {_col_lbound + col, _row_lbound + row};
}

template <typename T>
Vec2u GridMapView<T>::getAbsoluteIndex(sz index) const {
  return getAbsoluteIndex(index / getCols(), index % getCols());
}

template <typename T>
Vec2u GridMapView<T>::getAbsoulteIndex(Vec2u const& index) const {
  return getAbsoulteIndex(index.y, index.x);
}

template <typename T>
T const& GridMapView<T>::getCell(u32 row, u32 col) const {
  auto index = getAbsoluteIndex(row, col);
  return _p_grid_map->getCell(index.y, index.x);
}

template <typename T>
T& GridMapView<T>::getCell(u32 row, u32 col) {
  auto index = getAbsoluteIndex(row, col);
  return _p_grid_map->getCell(index.y, index.x);
}

template <typename T>
T const& GridMapView<T>::getCell(sz index) const {
  return getCell(index / getCols(), index % getCols());
}

template <typename T>
T& GridMapView<T>::getCell(sz index) {
  return getCell(index / getCols(), index % getCols());
}

template <typename T>
T const& GridMapView<T>::getCell(Vec2u const& index) const {
  return getCell(index.y, index.x);
}

template <typename T>
T& GridMapView<T>::getCell(Vec2u const& index) {
  return getCell(index.y, index.x);
}

template <typename T>
GridMapViewIterator<T const> GridMapView<T>::cbegin() const {
  return {*this, 0_u32};
}

template <typename T>
GridMapViewIterator<T const> GridMapView<T>::cend() const {
  return {*this, getRows() * getCols()};
}

template <typename T>
GridMapViewIterator<T const> GridMapView<T>::begin() const {
  return cbegin();
}

template <typename T>
GridMapViewIterator<T const> GridMapView<T>::end() const {
  return cend();
}

template <typename T>
GridMapViewIterator<T> GridMapView<T>::end() {
  return {*this, getRows() * getCols()};
}

template <typename T>
GridMapViewIterator<T> GridMapView<T>::begin() {
  return {*this, 0_u32};
}

#endif
