#pragma once
#ifndef _SIMUL_GRID_MAP_VIEW_ITERATOR_HH
#define _SIMUL_GRID_MAP_VIEW_ITERATOR_HH

template <typename T>
class GridMapView;

#ifndef _SIMUL_GRID_MAP_VIEW_HH
#include "grid-map-view.hh"
#endif

template <typename T>
class GridMapViewIterator {
 public:
  using GridMapViewType = propagate_const_t<T, GridMapView<T>>;

  using iterator_category = std::bidirectional_iterator_tag;

  using difference_type = std::ptrdiff_t;

  using value_type = std::pair<Vec2u, T&>;

  using pointer = value_type*;

  using reference = value_type&;

  GridMapViewIterator() = default;

  GridMapViewIterator(GridMapViewType& grid_map_view, sz offset)
      : _p_grid_map_view{&grid_map_view}
      , _offset{offset} {}

  GridMapViewIterator(GridMapViewIterator const& other) = default;

  ~GridMapViewIterator() = default;

  bool operator==(GridMapViewIterator const& other) const {
    if (_p_grid_map_view != other._p_grid_map_view) {
      throw "It is not allowed to compare iterators which indicates different GridMapView object.";
    }
    return _offset == other._offset;
  }

  bool operator!=(GridMapViewIterator const& other) const {
    if (_p_grid_map_view != other._p_grid_map_view) {
      throw "It is not allowed to compare iterators which indicates different GridMapView object.";
    }
    return _offset != other._offset;
  }

  GridMapViewIterator& operator=(GridMapViewIterator const& other) = default;

  GridMapViewIterator& operator++() {
    ++_offset;
    return *this;
  }

  GridMapViewIterator operator++(int) {
    auto iterator = GridMapViewIterator{*this};
    ++(*this);
    return iterator;
  }

  GridMapViewIterator& operator--() {
    if (_offset == 0) {
      throw "GridMapView::GridMapViewIterator cannot indicate negative offset.";
    }
    --_offset;
    return *this;
  }

  GridMapViewIterator operator--(int) {
    auto iterator = GridMapViewIterator{*this};
    --(*this);
    return iterator;
  }

  value_type const operator*() const {
    return {_p_grid_map_view->getAbsoluteIndex(_offset), _p_grid_map_view->getCell(_offset)};
  }

  value_type operator*() {
    return {_p_grid_map_view->getAbsoluteIndex(_offset), _p_grid_map_view->getCell(_offset)};
  }

 private:
  GridMapViewType* _p_grid_map_view;

  sz _offset;
};

#endif
