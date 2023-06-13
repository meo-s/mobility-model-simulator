from .types import Vec2
from .types import Vec3


class UAV:

    def __init__(self):
        self._position = Vec3()
        self._dest = Vec2()

    @property
    def position(self):
        return self._position

    @position.setter
    def position(self, value: Vec3):
        self._position = Vec3.copy_from(value)

    @property
    def pos(self):
        return self._position

    @pos.setter
    def pos(self, value: Vec3):
        self._position = Vec3.copy_from(value)

    @property
    def dest(self):
        return self._dest

    @dest.setter
    def dest(self, value):
        self._dest = Vec2.copy_from(value)
