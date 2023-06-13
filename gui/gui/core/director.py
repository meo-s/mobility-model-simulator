from __future__ import absolute_import

import sys
from typing import Optional, Tuple

import pygame


class Director:

    _s_instance: Optional['Director'] = None

    def __init__(self):
        super().__init__()

        # self._surf_window = pygame.display.set_mode(flags=pygame.FULLSCREEN)
        self._surf_window = pygame.display.set_mode((1920, 1080))
        self._surf_bg = pygame.Surface(self._surf_window.get_size()).convert()
        self._scenes = []

    def get_window_size(self) -> Tuple[int, int]:
        return self._surf_window.get_size()

    def get_scene(self, index: Optional[int] = None):
        if index is None:
            index = -1
        return self._scenes[index]

    def push_scene(self, scene):
        if scene not in self._scenes:
            self._scenes.append(scene)
            scene.on_push()

    def pop_scene(self):
        if 0 < len(self._scenes):
            self._scenes.pop().on_pop()

    def clear(self):
        while 0 < len(self._scenes):
            self.pop_scene()

    def run(self):
        clock = pygame.time.Clock()
        terminate = False
        while not terminate:
            try:
                dt = clock.tick(60) / 1e+3
                self._surf_window.blit(self._surf_bg, (0, 0))

                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        terminate = True

                    if 0 < len(self._scenes):
                        self._scenes[-1].ui_manager.process_events(event)

                if 0 < len(self._scenes):
                    scene = self._scenes[-1]
                    scene.update(dt)
                    scene.ui_manager.update(time_delta=dt)
                    scene.ui_manager.draw_ui(self._surf_window)

                pygame.display.update()
            except KeyboardInterrupt:
                terminate = True

        while 0 < len(self._scenes):
            self.pop_scene()

        return 0

    @staticmethod
    def get_instance() -> 'Director':
        if Director._s_instance is None:
            Director._s_instance = Director()
        return Director._s_instance

    @staticmethod
    def destroy_instance():
        Director._s_instance = None
