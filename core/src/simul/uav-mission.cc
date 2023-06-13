#include "uav-mission.hh"

#include <limits>

UAVMission::UAVMission()
    : _dests{}
    , _cur_dest_iter{} {
  init();
}

void UAVMission::init() {
  constexpr f32 INF = std::numeric_limits<f32>::infinity();

  _dests.clear();
  _dests.push_back({INF, INF});
  _cur_dest_iter = _dests.cbegin();
}

bool UAVMission::hasPreviousDestination() const noexcept {
  return _cur_dest_iter != _dests.cbegin();
}

bool UAVMission::hasDestination() const noexcept {
  constexpr f32 INF = std::numeric_limits<f32>::infinity();

  auto const& dest = getCurrentDestination();
  return (dest.x != INF) && (dest.y != INF);
}

bool UAVMission::hasNextDestination() const noexcept {
  constexpr f32 INF = std::numeric_limits<f32>::infinity();

  auto const next_dest_iter = std::next(_cur_dest_iter);
  if (next_dest_iter == _dests.cend()) {
    return false;
  }

  return (next_dest_iter->x != INF) && (next_dest_iter->y != INF);
}

Vec2f const& UAVMission::getPreviousDestination() const noexcept {
  return *std::prev(hasPreviousDestination() ? _cur_dest_iter : _dests.cend());
}

Vec2f const& UAVMission::getCurrentDestination() const noexcept {
  return *_cur_dest_iter;
}

void UAVMission::addDestination(Vec2f const& new_dest) {
  _dests.insert(--_dests.cend(), new_dest);
  if (_cur_dest_iter == --_dests.cend()) {
    --_cur_dest_iter;
  }
}

bool UAVMission::proceed() {
  return hasDestination() ? (++_cur_dest_iter, hasDestination()) : false;
}
