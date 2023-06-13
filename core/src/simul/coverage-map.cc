#include "coverage-map.hh"

#include <limits>

void CoverageMap::init(pb::CoverageMapConfig const& config) {
  _map_height = config.map_height();
  _map_width = config.map_width();
  _n_visit_cells = 0;
  _fine_grid_map.create(_map_height, _map_width, config.cell_size());
  _approx_grid_map.create(APPROX_MAP_HEIGHT, APPROX_MAP_WIDTH, 1.);
  for (auto i = 0; i < (_approx_grid_map.getRows() * _approx_grid_map.getCols()); ++i) {
    _approx_grid_map.getCell(i).setVisitCount(0);
  }
}

GridMap<CoverageMapCell<u8>> const& CoverageMap::getFineGridMap() const {
  return _fine_grid_map;
}

GridMap<CoverageMapCell<u32>> const& CoverageMap::getApproxGridMap() const {
  return _approx_grid_map;
}

f64 CoverageMap::getCoverageRatio() const noexcept {
  auto const n_total_cells = _fine_grid_map.getRows() * _fine_grid_map.getCols();
  return static_cast<f64>(_n_visit_cells) / n_total_cells;
}

f64 CoverageMap::getMapHeight() const noexcept {
  return _map_height;
}

f64 CoverageMap::getMapWidth() const noexcept {
  return _map_width;
}

void CoverageMap::visit(Rect2d const& rect, Rect2d const* p_except_rect) noexcept {
  for (auto const [index, cell] : _fine_grid_map.grabCells(rect)) {
    auto const cell_rect = _fine_grid_map.getCellRect(index);
    auto const cell_area = cell_rect.getArea();

    if (!isclose(cell_rect.getIntersectionArea(rect), cell_area)) {
      continue;
    }

    if (p_except_rect != nullptr) {
      if (isclose(cell_rect.getIntersectionArea(*p_except_rect), cell_area)) {
        continue;
      }
    }

    if (cell.increaseVisitCount() == 1) {
      ++_n_visit_cells;
    }

    auto const ct = cell_rect.getCenter();
    auto const zone_row = static_cast<u32>(ct.y / _map_height * APPROX_MAP_HEIGHT);
    auto const zone_col = static_cast<u32>(ct.x / _map_width * APPROX_MAP_WIDTH);
    _approx_grid_map.getCell(zone_row, zone_col).increaseVisitCount();
  }
}
