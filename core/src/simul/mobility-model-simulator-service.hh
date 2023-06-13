#pragma once
#ifndef _SIMUL_MOBILITY_MODEL_SIMULATOR_SERVICE_HH
#define _SIMUL_MOBILITY_MODEL_SIMULATOR_SERVICE_HH

#include <google/protobuf/empty.pb.h>
#include <grpc/grpc.h>

#include "protos/mobmodel-simulator.grpc.pb.h"

class MobilityModelSimulatorService : public pb::MobilityModelSimulatorService::Service {
 public:
  grpc::Status
  isSimulatorIdle(grpc::ServerContext*, google::protobuf::Empty const*, google::protobuf::BoolValue*) override;

  grpc::Status tryStartingSimulation(
      grpc::ServerContext*, pb::MobilityModelSimulatorConfig const*, google::protobuf::BoolValue*) override;

  grpc::Status
  tryStoppingSimulation(grpc::ServerContext*, google::protobuf::Empty const*, google::protobuf::BoolValue*) override;

  grpc::Status getSimulatorFrameInfo(
      grpc::ServerContext*, google::protobuf::Empty const*, pb::MobilityModelSimulatorFrameInfo*) override;

  grpc::Status getSimulatorApproxMap(
      grpc::ServerContext*, google::protobuf::Empty const*, pb::MobilityModelSimulatorApproxMap*) override;
};

#endif
