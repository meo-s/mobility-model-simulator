#pragma once
#ifndef _SIMUL_GRID_MAP_VIEW_HH
#define _SIMUL_GRID_MAP_VIEW_HH

#include <iterator>
#include <type_traits>
#include <utility>

#include "fundamental/type_traits.hh"
#include "fundamental/types.hh"

template <typename T>
class GridMap;

#ifndef _SIMUL_GRID_MAP_HH
#include "grid-map.hh"
#endif

template <typename T>
class GridMapViewIterator;

#ifndef _SIMUL_GRID_MAP_VIEW_ITERATOR_HH
#include "grid-map-view-iterator.hh"
#endif

template <typename T>
class GridMapView {
 public:
  using GridMapType = propagate_const_t<T, GridMap<std::remove_cv_t<T>>>;

  GridMapView() = delete;

  GridMapView(GridMapType&, u32 row_lbound, u32 row_ubound, u32 col_lbound, u32 col_rbound);

  GridMapView(GridMapView const&) = default;

  bool operator==(GridMapView const&) const;

  bool operator!=(GridMapView const&) const;

  u32 getRows() const;

  u32 getRowLowerBound() const;

  u32 getRowUpperBound() const;

  u32 getCols() const;

  u32 getColLowerBound() const;

  u32 getColUpperBound() const;

  Vec2u getAbsoluteIndex(u32 row, u32 col) const;

  Vec2u getAbsoluteIndex(sz) const;

  Vec2u getAbsoulteIndex(Vec2u const&) const;

  GridMapType const& getGridMap() const;

  GridMapType& getGridMap();

  T const& getCell(u32 row, u32 col) const;

  T& getCell(u32 row, u32 col);

  T const& getCell(sz index) const;

  T& getCell(sz index);

  T const& getCell(Vec2u const&) const;

  T& getCell(Vec2u const&);

  GridMapViewIterator<T const> cbegin() const;

  GridMapViewIterator<T const> cend() const;

  GridMapViewIterator<T const> begin() const;

  GridMapViewIterator<T const> end() const;

  GridMapViewIterator<T> begin();

  GridMapViewIterator<T> end();

 private:
  GridMapType* _p_grid_map;

  u32 _col_lbound;

  u32 _col_ubound;

  u32 _row_lbound;

  u32 _row_ubound;
};

#include "grid-map-view.hh.inl"

#endif
