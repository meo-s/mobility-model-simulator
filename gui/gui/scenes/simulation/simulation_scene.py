from typing import Optional

import grpc
from grpc._channel import _InactiveRpcError
from google.protobuf.empty_pb2 import Empty  # pylint: disable=no-name-in-module

import gui.components
import gui.core
from protos import mobmodel_simulator_pb2
from protos import mobmodel_simulator_pb2_grpc


class SimulationScene(gui.core.Scene):

    def __init__(self, server_addr: str):
        super().__init__()

        self._cfg_wnd = gui.components.ConfigWindow(
            position=(0, 50),
            ui_manager=self.ui_manager,
            listeners={
                '#btn-start-simul': self._try_starting_simulation,
                '#btn-stop-simul': self._try_stopping_simulation,
            }
        )
        self._cvge_wnd = gui.components.CoverageGraphWindow(
            position=(470, 50),
            ui_manager=self.ui_manager,
        )
        self._visit_graph_wnd = gui.components.VisitGraphWindow(
            position=(470, 450),
            ui_manager=self.ui_manager,
        )
        self._simul_map_wnd = gui.components.SimulationMapWindow(
            position=(980, 50),
            ui_manager=self.ui_manager,
            coverage_map_size=(1000, 1000),
        )

        self._server_addr = server_addr
        self._channel: Optional[grpc.Channel] = None
        self._stub: Optional[mobmodel_simulator_pb2_grpc.MobilityModelSimulatorServiceStub] = None

    def on_push(self):
        self._channel = grpc.insecure_channel(self._server_addr)
        self._stub = mobmodel_simulator_pb2_grpc.MobilityModelSimulatorServiceStub(self._channel)
        self._visit_graph_wnd.set_stub(self._stub)

    def on_pop(self):
        self._channel.close()
        self._channel = None
        self._stub = None

    def update(self, dt: float):
        try:
            frame_info = self._stub.getSimulatorFrameInfo(Empty())
        except _InactiveRpcError:
            return

        self._cvge_wnd.add_coverage_ratio(frame_info.simulation_time, frame_info.coverage_ratio)
        for i, uav in enumerate(frame_info.uavs):
            self._simul_map_wnd.uav(i).pos = uav.position
            self._simul_map_wnd.uav(i).dest = uav.destination

    def _try_starting_simulation(self, cfg: mobmodel_simulator_pb2.MobilityModelSimulatorConfig):
        if self._stub.tryStartingSimulation(cfg).value:
            self._simul_map_wnd.init()

    def _try_stopping_simulation(self):
        print(self._stub.tryStoppingSimulation(Empty()).value)
