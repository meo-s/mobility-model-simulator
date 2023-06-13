from typing import NamedTuple, Iterable

import numpy as np

from protos import mobmodel_simulator_pb2


class Vec2(NamedTuple):

    x: float = 0
    y: float = 0

    def numpy(self) -> np.ndarray:
        return np.array([self.x, self.y])

    def norm(self, ord_: int = 2) -> float:
        return np.linalg.norm([self.x, self.y], ord=ord_)

    @staticmethod
    def copy_from(v) -> 'Vec2':
        if isinstance(v, Iterable):
            return Vec2(*(*v,)[:3])
        elif isinstance(v, (Vec2, mobmodel_simulator_pb2.Vec2f)):
            return Vec2(v.x, v.y)
        else:
            raise ValueError(f'Can\'t construct Vec2 from {type(v)}.')


class Vec3(NamedTuple):

    x: float = 0
    y: float = 0
    z: float = 0

    def numpy(self) -> np.ndarray:
        return np.array([self.x, self.y, self.z])

    def norm(self, ord_: int = 2) -> float:
        return np.linalg.norm([self.x, self.y, self.z], ord=ord_)

    @staticmethod
    def copy_from(v) -> 'Vec3':
        if isinstance(v, Iterable):
            return Vec3(*(*v,)[:3])
        elif isinstance(v, (Vec3, mobmodel_simulator_pb2.Vec3f)):
            return Vec3(v.x, v.y, v.z)
        else:
            raise ValueError(f'Can\'t construct Vec3 from {type(v)}.')
