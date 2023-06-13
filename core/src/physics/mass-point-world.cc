#include "mass-point-world.hh"

#include <functional>
#include <utility>

#include "mass-point.hh"

std::optional<std::reference_wrapper<MassPoint const>>
MassPointWorld::getMassPoint(std::string const& mass_point_name) const noexcept {
  using R = std::optional<std::reference_wrapper<MassPoint const>>;
  auto iter = _mass_points.find(mass_point_name);
  return (iter != _mass_points.end()) ? R{*iter->second} : R{std::nullopt};
}

std::optional<std::reference_wrapper<MassPoint>>
MassPointWorld::getMassPoint(std::string const& mass_point_name) noexcept {
  using R = std::optional<std::reference_wrapper<MassPoint>>;
  auto iter = _mass_points.find(mass_point_name);
  return (iter != _mass_points.end()) ? R{*iter->second} : R{std::nullopt};
}

std::optional<std::reference_wrapper<MassPointWorld::MassPointPtr const>>
MassPointWorld::getMassPointPtr(std::string const& mass_point_name) noexcept {
  using R = std::optional<std::reference_wrapper<MassPointPtr const>>;
  auto iter = _mass_points.find(mass_point_name);
  return (iter != _mass_points.end()) ? R{iter->second} : R{std::nullopt};
}

std::pair<bool, MassPoint&>
MassPointWorld::addMassPoint(std::string_view const& mass_point_name, std::shared_ptr<MassPoint> const& p_mass_point) {
  auto const& [iter, is_inserted] = _mass_points.try_emplace(std::string{mass_point_name}, p_mass_point);
  return {is_inserted, *iter->second};
}

std::pair<bool, MassPoint&>
MassPointWorld::addMassPoint(std::string_view const& mass_point_name, MassPoint* p_mass_point) {
  return addMassPoint(mass_point_name, std::shared_ptr<MassPoint>{p_mass_point});
}

bool MassPointWorld::removeMassPoint(std::string const& mass_point_name) noexcept {
  return _mass_points.erase(mass_point_name) == 1;
}

void MassPointWorld::updateMassPoint(MassPoint& mass_point, f32 dt) {
  constexpr auto DUST = std::numeric_limits<f32>::min();

  auto displacement = MassPoint::VectorType{};
  auto velocity = mass_point.getVelocity();
  auto const& accel = mass_point.getForce() / (mass_point.getMass() + DUST);
  auto const throttle = mass_point.getThrottle();

  do {
    if (0 < throttle) {
      if (std::pow(throttle, 2) < velocity.dot(velocity)) {
        velocity.normalize_().scale_(throttle);
      }

      if (accel != 0) {
        // calculate required time to accelerate mass point's speed up to throttle speed.
        // define variables to use the quadratic formula x=(-b+-sqrt(b^2-4ac))/2a.
        auto const a = accel.dot(accel);
        auto const b = 2 * velocity.dot(accel);
        auto const c = velocity.dot(velocity) - throttle * throttle;
        auto const t = (-b + std::sqrt(b * b - 4 * a * c)) / (2 * a + DUST);
        if (t < dt) {
          // only change direction of the velocity vector.
          velocity.add_(accel * dt).normalize_().scale_(throttle);
          displacement += (velocity * (dt - t)) + (mass_point.getVelocity() * t) + (accel * (std::pow(t, 2) / 2));
          break;
        }
      }
    }

    if (accel != 0) {
      auto const& delta_v = accel * dt;
      velocity.add_(delta_v);
      displacement += (velocity * dt) - (delta_v * (dt / 2));
    } else {
      displacement += velocity * dt;
    }
  } while (false);

  mass_point.setPosition(mass_point.getPosition() + displacement);
  mass_point.setVelocity(velocity);
  mass_point.resetForce();
}

void MassPointWorld::update(f32 dt) {
  for (auto& [_, p_mass_point] : _mass_points) {
    updateMassPoint(*p_mass_point, dt);
  }
}

void MassPointWorld::clear() {
  _mass_points.clear();
}
