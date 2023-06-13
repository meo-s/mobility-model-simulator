import time
from typing import Optional, Tuple

import grpc
import numpy as np
import pygame
import pygame_gui
from google.protobuf.empty_pb2 import Empty  # pylint: disable=no-name-in-module

from .matplotlib_surface import MatplotlibSurface
from protos import mobmodel_simulator_pb2_grpc

_APPROX_MAP_ROWS = 100
_APPROX_MAP_COLS = 100
_APPROX_MAP_X, _APPROX_MAP_Y = np.meshgrid(np.arange(1, _APPROX_MAP_COLS + 1),
                                           np.arange(1, _APPROX_MAP_ROWS + 1))


class VisitGraphWindow(pygame_gui.elements.UIWindow):

    CLIENT_RECT = (500, 500)

    def __init__(
        self,
        position: Tuple[int, int],
        ui_manager: pygame_gui.UIManager,
        update_interval: float = .5,
        stub: Optional[mobmodel_simulator_pb2_grpc.MobilityModelSimulatorServiceStub] = None,
    ):
        super().__init__(rect=pygame.Rect(position, VisitGraphWindow.CLIENT_RECT),
                         manager=ui_manager,
                         window_display_title='Revisit Ratio Graph',
                         object_id='#coverage-ratio-graph')

        w, h = VisitGraphWindow.CLIENT_RECT
        shrinked_w, shrinked_h = self.get_container().get_size()
        self.set_dimensions((2 * w - shrinked_w, 2 * h - shrinked_h))

        self._img_window = pygame_gui.elements.UIImage(
            relative_rect=pygame.Rect((0, 0), VisitGraphWindow.CLIENT_RECT),
            image_surface=pygame.Surface(VisitGraphWindow.CLIENT_RECT).convert(),
            manager=ui_manager,
            container=self,
            parent_element=self,
        )

        dpi = 84
        w, h = VisitGraphWindow.CLIENT_RECT
        self._graph = MatplotlibSurface(surf_size=(w + 100, h + 100),
                                        dpi=dpi,
                                        facecolor='none',
                                        subplot_kw={'projection': '3d'})
        self._graph.ax.set_xlim([0, _APPROX_MAP_COLS])
        self._graph.ax.set_xticks(np.linspace(0, _APPROX_MAP_COLS, 11))
        self._graph.ax.set_ylim([0, _APPROX_MAP_ROWS])
        self._graph.ax.set_yticks(np.linspace(0, _APPROX_MAP_ROWS, 11))
        self._graph.ax.set_zlim([0, 1.])
        self._graph.ax.set_xlabel('Zone X Index')
        self._graph.ax.set_ylabel('Zone Y Index')
        self._graph.ax.set_zlabel('Number of Visits Ratio')

        self._graph_poly3d = self._graph.ax.plot_surface(
            _APPROX_MAP_X,
            _APPROX_MAP_Y,
            np.zeros((_APPROX_MAP_COLS, _APPROX_MAP_ROWS)),
            cmap='inferno',
            linewidth=1,
            antialiased=True,
        )

        self._stub = stub
        self._pending_io: Optional[grpc.Future] = None
        self._update_interval = update_interval
        self._last_update = 0

    def set_stub(self, stub: mobmodel_simulator_pb2_grpc.MobilityModelSimulatorServiceStub):
        self._stub = stub

    def process_event(self, event: pygame.event.Event) -> bool:
        consumed_event = False
        if hasattr(event, 'ui_element') and event.ui_element == self.close_window_button:
            consumed_event = True

        if event.type == pygame.QUIT:
            self._graph.discard()
            self._graph.worker.join()

        return consumed_event or super().process_event(event)

    def update(self, time_delta: float):
        super().update(time_delta=time_delta)

        now = time.time()
        if (now - self._last_update) < self._update_interval:
            return

        if self._graph.idle():
            if (graph_surf := self._graph.get_surface()) is not None:
                self._img_window.image.fill((0, 0, 0))
                self._img_window.image.blit(graph_surf, (-70, -60))

            if self._pending_io is None:
                if self._stub is not None:
                    self._pending_io = self._stub.getSimulatorApproxMap.future(Empty())

            approx_map = self._pending_io.result()
            if (approx_map.rows != 0) and (approx_map.cols != 0):
                assert approx_map.rows == _APPROX_MAP_ROWS
                assert approx_map.cols == _APPROX_MAP_COLS

                z = np.array(approx_map.zones).reshape(_APPROX_MAP_ROWS, _APPROX_MAP_COLS)[::-1, :]
                z = z / (np.max(z) + 1e-6)
                self._graph.call(self._graph_poly3d.update_surface, _APPROX_MAP_X, _APPROX_MAP_Y, z)
                self._graph.draw()

                self._last_update = now

            self._pending_io = None
