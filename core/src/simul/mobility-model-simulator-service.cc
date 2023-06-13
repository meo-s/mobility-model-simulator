#include "mobility-model-simulator-service.hh"

#include <mutex>

#include "app-delegate.hh"
#include "simul/uav-mission.hh"
#include "simul/uav.hh"

grpc::Status MobilityModelSimulatorService::isSimulatorIdle(
    grpc::ServerContext*, google::protobuf::Empty const*, google::protobuf::BoolValue* pb_bool) {
  pb_bool->set_value(!AppDelegate::getInstance().isSimulationOnRunning());
  return grpc::Status::OK;
}

grpc::Status MobilityModelSimulatorService::tryStartingSimulation(
    grpc::ServerContext*, pb::MobilityModelSimulatorConfig const* pb_simul_cfg,
    google::protobuf::BoolValue* pb_req_success) {
  pb_req_success->set_value(AppDelegate::getInstance().tryStartingSimulation(*pb_simul_cfg));
  return grpc::Status::OK;
}

grpc::Status MobilityModelSimulatorService::tryStoppingSimulation(
    grpc::ServerContext*, google::protobuf::Empty const*, google::protobuf::BoolValue* pb_req_success) {
  pb_req_success->set_value(AppDelegate::getInstance().tryStoppingSimulation());
  return grpc::Status::OK;
}

grpc::Status MobilityModelSimulatorService::getSimulatorFrameInfo(
    grpc::ServerContext*, google::protobuf::Empty const*, pb::MobilityModelSimulatorFrameInfo* pb_frame_info) {
  auto& app = AppDelegate::getInstance();
  if (app.isSimulationOnRunning()) {
    pb_frame_info->set_state(pb::MobilityModelSimulatorState::RUNNING);

    auto const& simulator = app.getSimulator();
    pb_frame_info->set_simulation_time(simulator.getSimulationTime());
    pb_frame_info->set_coverage_ratio(simulator.getCoverageMap().getCoverageRatio());

    for (auto i = 0; i < simulator.getUAVCount(); ++i) {
      auto const& uav = static_cast<UAV const&>(simulator.getUAV(i).value());

      auto pb_uav = pb_frame_info->add_uavs();
      pb_uav->set_index(i);

      auto const& pos = uav.getPosition();
      pb_uav->mutable_position()->set_x(pos.x);
      pb_uav->mutable_position()->set_y(pos.y);
      pb_uav->mutable_position()->set_z(pos.z);

      auto dest = pos.xy();
      auto const& control_tower = simulator.getUAVControlTower();
      if (auto const& opt_mission = control_tower.getMissionOf(uav); opt_mission.has_value()) {
        if (auto const& mission = getValue(opt_mission); mission.hasDestination()) {
          dest = mission.getCurrentDestination();
        }
      }
      pb_uav->mutable_destination()->set_x(dest.x);
      pb_uav->mutable_destination()->set_y(dest.y);
    }
  }

  return grpc::Status::OK;
}

grpc::Status MobilityModelSimulatorService::getSimulatorApproxMap(
    grpc::ServerContext*, google::protobuf::Empty const*, pb::MobilityModelSimulatorApproxMap* pb_approx_map) {
  auto const& app = AppDelegate::getInstance();
  if (app.isSimulationOnRunning()) {
    auto const& approx_map = app.getSimulator().getCoverageMap().getApproxGridMap();
    pb_approx_map->set_cols(approx_map.getCols());
    pb_approx_map->set_rows(approx_map.getRows());
    for (auto i = 0_sz; i < (approx_map.getRows() * approx_map.getCols()); ++i) {
      pb_approx_map->add_zones(approx_map.getCell(i).getVisitCount());
    }
  }
  return grpc::Status::OK;
}