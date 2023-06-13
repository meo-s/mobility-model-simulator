#pragma once
#ifndef _APP_DELEGATE_HH
#define _APP_DELEGATE_HH

#include <memory>
#include <mutex>

#include "protos/mobmodel-simulator.pb.h"
#include "simul/mobility-model-simulator.hh"


class AppDelegate {
 public:
  MobilityModelSimulator const& getSimulator() const noexcept;

  MobilityModelSimulator& getSimulator() noexcept;

  std::mutex& getSimulatorControlLock() noexcept;

  std::mutex const& getSimulatorControlLock() const noexcept;

  bool isSimulationOnRunning() const noexcept;

  bool tryStartingSimulation(pb::MobilityModelSimulatorConfig const&);

  bool tryStoppingSimulation();

  int run();

  static AppDelegate& getInstance();

  static void destroyInstance();

 private:
  AppDelegate() = default;

  void simulationWorker();

  void networkWorker();

 private:
  bool _terminate;

  bool _simulating;

  std::mutex _simul_control_lock;

  MobilityModelSimulator _simulator;

  static std::unique_ptr<AppDelegate> _s_instance;
};

#endif
