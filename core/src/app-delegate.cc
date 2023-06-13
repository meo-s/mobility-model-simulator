#include "app-delegate.hh"

#include <google/protobuf/repeated_field.h>
#include <grpc++/grpc++.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include "fundamental/vec3.hh"
#include "physics/mass-point.hh"
#include "protos/mobmodel-simulator.grpc.pb.h"
#include "simul/coverage-map.hh"
#include "simul/mobility-model-simulator-service.hh"
#include "simul/mobility-model-simulator.hh"
#include "simul/uav-builder.hh"
#include "simul/uav-control-tower.hh"
#include "simul/uav.hh"

constexpr auto SIMULATOR_FPS = 60_u32;

std::unique_ptr<AppDelegate> AppDelegate::_s_instance{};

MobilityModelSimulator const& AppDelegate::getSimulator() const noexcept {
  return _simulator;
}

MobilityModelSimulator& AppDelegate::getSimulator() noexcept {
  return _simulator;
}

std::mutex& AppDelegate::getSimulatorControlLock() noexcept {
  return _simul_control_lock;
}

std::mutex const& AppDelegate::getSimulatorControlLock() const noexcept {
  return _simul_control_lock;
}

bool AppDelegate::isSimulationOnRunning() const noexcept {
  return _simulating;
}

bool AppDelegate::tryStartingSimulation(pb::MobilityModelSimulatorConfig const& pb_simul_cfg) {
  auto ret = false;

  if (!isSimulationOnRunning()) {
    auto const scoped_lock = std::scoped_lock{getSimulatorControlLock()};
    if (!isSimulationOnRunning()) {
      _simulator.init(pb_simul_cfg);
      _simulating = true;
      ret = true;
    }
  }

  return ret;
}

bool AppDelegate::tryStoppingSimulation() {
  auto ret = false;

  if (isSimulationOnRunning()) {
    auto const scoped_lock = std::scoped_lock{getSimulatorControlLock()};
    if (isSimulationOnRunning()) {
      _simulating = false;
      ret = true;
    }
  }

  return ret;
}

int AppDelegate::run() {
  auto worker_threads = std::array{
      std::thread{std::bind(&AppDelegate::simulationWorker, this)},
      std::thread{std::bind(&AppDelegate::networkWorker, this)},
  };

  for (auto& worker_thread : worker_threads) {
    worker_thread.join();
  }

  return 0;
}

AppDelegate& AppDelegate::getInstance() {
  if (_s_instance == nullptr) {
    _s_instance.reset(new AppDelegate{});
  }
  return *_s_instance;
}

void AppDelegate::destroyInstance() {
  _s_instance.reset();
}

void AppDelegate::simulationWorker() {
  constexpr auto dt = 1.f / SIMULATOR_FPS;
  while (!_terminate) {
    if (!_simulating) {
      std::this_thread::yield();
      continue;
    }

    using Clock = std::chrono::steady_clock;

    auto const tps = Clock::now();
    _simulator.update(dt);
    auto const tpe = Clock::now();

    auto const frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(tpe - tps);
    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(dt * 1e+3) - frame_time);
  }
}

void AppDelegate::networkWorker() {
  auto service = MobilityModelSimulatorService{};

  auto server_builder = grpc::ServerBuilder{};
  server_builder.AddListeningPort("0.0.0.0:22022", grpc::InsecureServerCredentials());
  server_builder.RegisterService(&service);

  auto server = server_builder.BuildAndStart();
  server->Wait();
}
