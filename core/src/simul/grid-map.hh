#pragma once
#ifndef _SIMUL_GRID_MAP_HH
#define _SIMUL_GRID_MAP_HH

#include <memory>
#include <tuple>
#include <utility>

#include "fundamental/rect2.hh"

template <typename T>
class GridMapView;

#ifndef _SIMUL_GRID_MAP_VIEW_HH
#include "grid-map-view.hh"
#endif

template <typename T>
class GridMap {
 public:
  using GridMapCell = T;

  GridMap() = default;

  GridMap(f64 map_height, f64 map_width, f64 cell_size);

  GridMap(GridMap&) = default;

  GridMap(GridMap&&) = default;

  virtual ~GridMap();

  GridMap& operator=(GridMap&) = default;

  GridMap& operator=(GridMap&&) = default;

  void create(f64 map_height, f64 map_width, f64 cell_size);

  void share(T* p_cells, f64 map_height, f64 map_width, f64 cell_size);

  void destroy() noexcept;

  f64 getMapWidth() const;

  f64 getMapHeight() const;

  f64 getCellSize() const;

  u32 getRows() const;

  u32 getCols() const;

  Rect2d getCellRect(u32 row, u32 col) const;

  Rect2d getCellRect(Vec2u const&) const;

  T const& getCell(sz) const;

  T& getCell(sz);

  T const& getCell(u32 row, u32 col) const;

  T& getCell(u32 row, u32 col);

  T const& getCell(Vec2u const&) const;

  T& getCell(Vec2u const&);

  std::tuple<u32, u32, u32, u32> computeBounds(Rect2d const&) const;

  GridMapView<T const> grabCells(Rect2d const&) const;

  GridMapView<T> grabCells(Rect2d const&);

 private:
  f64 _cell_size;

  f64 _map_width;

  f64 _map_height;

  T* _p_cells;

  bool _own_cells;
};

#include "grid-map.hh.inl"

#endif
