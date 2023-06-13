import abc
from typing import Optional, Tuple

import pygame
import pygame_gui

from .director import Director


class Scene(abc.ABC):

    def __init__(self, scene_size: Optional[Tuple[int, int]] = None):
        super().__init__()

        if scene_size is None:
            scene_size = Director.get_instance().get_window_size()

        self._ui_manager = pygame_gui.UIManager(scene_size, theme_path='rsc/theme.json')

    @property
    def ui_manager(self) -> pygame_gui.UIManager:
        return self._ui_manager

    @abc.abstractmethod
    def update(self, dt: float):
        pass

    def on_push(self):
        pass

    def on_pop(self):
        pass
