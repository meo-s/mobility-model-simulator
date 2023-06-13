from collections import defaultdict
from typing import Dict, Tuple

import numpy as np
import pygame
import pygame_gui

import simul
import rsc


# https://codereview.stackexchange.com/questions/70143/drawing-a-dashed-line-with-pygame
def draw_line_dashed(
    surface,
    color,
    start_pos,
    end_pos,
    line_width=1,
    dash_length=10,
    exclude_corners=True,
):

    # convert tuples to numpy arrays
    start_pos = np.array(start_pos)
    end_pos = np.array(end_pos)

    # get euclidian distance between start_pos and end_pos
    length = np.linalg.norm(end_pos - start_pos)

    # get amount of pieces that line will be split up in (half of it are amount of dashes)
    dash_amount = int(length / dash_length)

    # x-y-value-pairs of where dashes start (and on next, will end)
    dash_knots = np.array([np.linspace(start_pos[i], end_pos[i], dash_amount) for i in range(2)]).T

    return [
        pygame.draw.line(surface, color, tuple(dash_knots[n]), tuple(dash_knots[n + 1]), line_width)
        for n in range(int(exclude_corners), dash_amount - int(exclude_corners), 2)
    ]


class SimulationMapWindow(pygame_gui.elements.UIWindow):

    CLIENT_RECT = (900, 900)

    def __init__(
            self,
            position: Tuple[int, int],
            ui_manager: pygame_gui.UIManager,
            coverage_map_size: Tuple[int, int],
            uav_trace_size: Tuple[int, int] = (32, 32),
    ):
        super().__init__(rect=pygame.Rect(position, SimulationMapWindow.CLIENT_RECT),
                         manager=ui_manager,
                         window_display_title='Simulation Map',
                         object_id='#simulation-map')

        w, h = SimulationMapWindow.CLIENT_RECT
        shrinked_w, shrinked_h = self.get_container().get_size()
        self.set_dimensions((2 * w - shrinked_w, 2 * h - shrinked_h))

        self._img_window = pygame_gui.elements.UIImage(
            relative_rect=pygame.Rect((0, 0), SimulationMapWindow.CLIENT_RECT),
            image_surface=pygame.Surface(SimulationMapWindow.CLIENT_RECT).convert(),
            manager=ui_manager,
            container=self,
            parent_element=self,
        )

        self._surf_bg = pygame.image.load(rsc.IMG_SIMULATION_MAP).convert()
        self._surf_uav = pygame.image.load(rsc.IMG_UAV).convert_alpha()
        self._surf_uav_trace = pygame.Surface(SimulationMapWindow.CLIENT_RECT).convert_alpha()
        self._coverage_map_size = coverage_map_size
        self._uav_trace_size = uav_trace_size
        self._uavs: Dict[int, simul.UAV] = defaultdict(simul.UAV)
        self.init()

    def init(self):
        self._surf_uav_trace.fill((0, 0, 0, 0))
        self._uavs.clear()

    def uav(self, index: int) -> simul.UAV:
        return self._uavs[index]

    def process_event(self, event: pygame.event.Event) -> bool:
        consumed_event = False
        if hasattr(event, 'ui_element') and event.ui_element == self.close_window_button:
            consumed_event = True

        return consumed_event or super().process_event(event)

    def update(self, time_delta: float):
        super().update(time_delta=time_delta)

        cw, ch = SimulationMapWindow.CLIENT_RECT
        mw, mh = self._coverage_map_size
        rw, rh = cw / mw, ch / mh

        self._img_window.image.blit(self._surf_bg, (0, 0))

        for uav in self._uavs.values():
            w, h = self._uav_trace_size
            w /= 2
            h /= 2
            x = rw * uav.pos.x - w / 2
            y = rh * uav.pos.y - h / 2
            pygame.draw.ellipse(self._surf_uav_trace, (128, 128, 255, 96), [x, y, w, h])

        self._img_window.image.blit(self._surf_uav_trace, (0, 0))

        for uav in self._uavs.values():
            guide_color = (128, 128, 64, 64)
            uav_x, uav_y = uav.pos.x * rw, uav.pos.y * rh
            dest_x, dest_y = uav.dest.x * rw, uav.dest.y * rh
            draw_line_dashed(
                self._img_window.image,
                guide_color,
                (uav_x, uav_y),
                (dest_x, dest_y),
                line_width=3,
                dash_length=5,
            )
            pygame.draw.ellipse(self._img_window.image, guide_color, [dest_x - 3, dest_y - 3, 7, 7])

        for uav in self._uavs.values():
            uav_x, uav_y = uav.pos.x * rw, uav.pos.y * rh
            surf_w, surf_h = self._surf_uav.get_size()
            blit_x = uav_x - surf_w / 2
            blit_y = uav_y - surf_h / 2
            self._img_window.image.blit(self._surf_uav, (blit_x, blit_y))
