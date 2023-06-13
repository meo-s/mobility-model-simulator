#pragma once
#ifndef _SIMUL_GRID_MAP_HH_INL
#define _SIMUL_GRID_MAP_HH_INL

#ifndef _SIMUL_GRID_MAP_HH
#include "grid-map.hh"
#endif

#include <cmath>

template <typename T>
GridMap<T>::~GridMap() {
  destroy();
}

template <typename T>
GridMap<T>::GridMap(f64 map_height, f64 map_width, f64 cell_size)
    : GridMap{} {
  create(map_height, map_width, cell_size);
}

template <typename T>
void GridMap<T>::create(f64 map_height, f64 map_width, f64 cell_size) {
  destroy();

  _map_height = map_height;
  _map_width = map_width;
  _cell_size = cell_size;

  sz n_rows = std::ceil(_map_height / _cell_size);
  sz n_cols = std::ceil(_map_width / _cell_size);
  sz n_cells = n_cols * n_rows;
  _p_cells = new T[n_cells];
  _own_cells = true;
}

template <typename T>
void GridMap<T>::share(T* p_cells, f64 map_height, f64 map_width, f64 cell_size) {
  _p_cells = p_cells;
  _own_cells = true; 
  _map_height = map_height;
  _map_width = map_width;
  _cell_size = cell_size;
}

template <typename T>
void GridMap<T>::destroy() noexcept {
  if (_own_cells) {
    delete[] _p_cells;
  }
  _p_cells = nullptr;
  _own_cells = false;
  _cell_size = 0;
  _map_height = 0;
  _map_width = 0;
}

template <typename T>
f64 GridMap<T>::getMapWidth() const {
  return _map_width;
}

template <typename T>
f64 GridMap<T>::getMapHeight() const {
  return _map_height;
}

template <typename T>
f64 GridMap<T>::getCellSize() const {
  return _cell_size;
}

template <typename T>
u32 GridMap<T>::getRows() const {
  return static_cast<u32>(std::ceil(_map_height / _cell_size));
}

template <typename T>
u32 GridMap<T>::getCols() const {
  return static_cast<u32>(std::ceil(_map_width / _cell_size));
}

template <typename T>
Rect2d GridMap<T>::getCellRect(u32 row, u32 col) const {
  return {static_cast<double>(col), static_cast<double>(row), _cell_size, _cell_size};
}

template <typename T>
Rect2d GridMap<T>::getCellRect(Vec2u const& index) const {
  return {static_cast<double>(index.x), static_cast<double>(index.y), _cell_size, _cell_size};
}

template <typename T>
T const& GridMap<T>::getCell(sz index) const {
  return _p_cells[index];
}

template <typename T>
T& GridMap<T>::getCell(sz index) {
  return _p_cells[index];
}

template <typename T>
T const& GridMap<T>::getCell(u32 row, u32 col) const {
  return getCell(row * getCols() + col);
}

template <typename T>
T& GridMap<T>::getCell(u32 row, u32 col) {
  return getCell(row * getCols() + col);
}

template <typename T>
T const& GridMap<T>::getCell(Vec2u const& index) const {
  return getCell(index.y, index.x);
}

template <typename T>
T& GridMap<T>::getCell(Vec2u const& index) {
  return getCell(index.y, index.x);
}

template <typename T>
std::tuple<u32, u32, u32, u32> GridMap<T>::computeBounds(Rect2d const& query) const {
  auto const row_lbound = std::max(static_cast<u32>(query.y / _cell_size), 0_u32);
  auto const row_ubound = std::min(static_cast<u32>(std::ceil((query.y + query.h) / _cell_size)), getRows());
  auto const col_lbound = std::max(static_cast<u32>(query.x / _cell_size), 0_u32);
  auto const col_ubound = std::min(static_cast<u32>(std::ceil((query.x + query.w) / _cell_size)), getCols());
  return {row_lbound, row_ubound, col_lbound, col_ubound};
}

template <typename T>
GridMapView<T const> GridMap<T>::grabCells(Rect2d const& query) const {
  auto const [row_lbound, row_ubound, col_lbound, col_ubound] = computeBounds(query);
  return {*this, row_lbound, row_ubound, col_lbound, col_ubound};
}

template <typename T>
GridMapView<T> GridMap<T>::grabCells(Rect2d const& query) {
  auto const [row_lbound, row_ubound, col_lbound, col_ubound] = computeBounds(query);
  return {*this, row_lbound, row_ubound, col_lbound, col_ubound};
}

#endif
