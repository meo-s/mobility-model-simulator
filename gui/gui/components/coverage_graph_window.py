from typing import Tuple

import pygame
import pygame_gui
import numpy as np

from .matplotlib_surface import MatplotlibSurface


class CoverageGraphWindow(pygame_gui.elements.UIWindow):

    CLIENT_RECT = (500, 350)

    def __init__(self, position: Tuple[int, int], ui_manager: pygame_gui.UIManager, max_simulation_time: float = 30):
        super().__init__(rect=pygame.Rect(position, CoverageGraphWindow.CLIENT_RECT),
                         manager=ui_manager,
                         window_display_title='Coverage Ratio Graph',
                         object_id='#coverage-ratio-graph')

        w, h = CoverageGraphWindow.CLIENT_RECT
        shrinked_w, shrinked_h = self.get_container().get_size()
        self.set_dimensions((2 * w - shrinked_w, 2 * h - shrinked_h))

        self._img_window = pygame_gui.elements.UIImage(
            relative_rect=pygame.Rect((0, 0), CoverageGraphWindow.CLIENT_RECT),
            image_surface=pygame.Surface(CoverageGraphWindow.CLIENT_RECT).convert(),
            manager=ui_manager,
            container=self,
            parent_element=self,
        )

        dpi = 84
        w, h = CoverageGraphWindow.CLIENT_RECT
        self._graph = MatplotlibSurface(surf_size=(w, h), dpi=dpi, facecolor='none')
        self._graph.ax.set_facecolor('none')
        self._graph.ax.spines['left'].set_color('green')
        self._graph.ax.spines['bottom'].set_color('green')
        self._graph.ax.spines['right'].set_color('black')
        self._graph.ax.spines['top'].set_color('black')
        self._graph.ax.set_xlim([0, max_simulation_time])
        self._graph.ax.set_ylim([0, 100])
        self._graph.ax.set_yticks(np.linspace(0, 100, 11))
        self._graph.ax.set_xlabel('Simulation Time (min)')
        self._graph.ax.set_ylabel('Coverage Ratio (%)')

        self._times = [0]
        self._coverage_ratios = [0]
        self._n_points = 500

    def add_coverage_ratio(self, time: float, coverage_ratio: float):
        self._times.append(time / 60)
        self._coverage_ratios.append(coverage_ratio * 1e+2)

        if self._n_points < len(self._times):
            x = np.linspace(0, self._times[-1], self._n_points)
            y = np.interp(x, self._times, self._coverage_ratios)
            self._times = [*x]
            self._coverage_ratios = [*y]

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

        if self._graph.idle():
            if (graph_surf := self._graph.get_surface()) is not None:
                self._img_window.image.fill((0, 0, 0))
                self._img_window.image.blit(graph_surf, (0, 0))

            if 0 < len(self._graph.ax.lines):
                self._graph.ax.lines.pop(0)

            self._graph.call(self._graph.ax.plot, self._times, self._coverage_ratios, color='red')
            self._graph.draw()
