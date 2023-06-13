# pylint: disable=wrong-import-position

import queue
import threading
from typing import Optional, Tuple

import matplotlib
import matplotlib.pyplot as plt
import matplotlib.style
matplotlib.use('Agg')
matplotlib.style.use(['dark_background', 'fast'])
matplotlib.rcParams['path.simplify'] = True
matplotlib.rcParams['path.simplify_threshold'] = 1.
matplotlib.rcParams['agg.path.chunksize'] = 10000
matplotlib.rcParams['axes.edgecolor'] = 'green'
matplotlib.rcParams['text.color'] = 'white'
matplotlib.rcParams['axes.labelcolor'] = 'yellow'
matplotlib.rcParams['xtick.color'] = 'yellow'
matplotlib.rcParams['ytick.color'] = 'yellow'

import matplotlib.backends.backend_agg as agg  # noqa: E402
import pygame  # noqa: E402


class MatplotlibSurface:

    def __init__(self, surf_size: Tuple[int, int], *args, **kargs):
        kargs['figsize'] = (surf_size[0] / kargs['dpi'], surf_size[1] / kargs['dpi'])
        self._fig, self._ax = plt.subplots(*args, **kargs)
        self._fig_lock = threading.Lock()
        self._canvas = agg.FigureCanvasAgg(self._fig)
        self._surf: Optional[pygame.Surface] = None
        self._surf_size = surf_size

        self._requests = queue.Queue()
        self._discard = False
        self._on_job = False
        self._worker = threading.Thread(target=self._worker_entry)
        self._worker.start()

    @property
    def fig(self):
        return self._fig

    @property
    def fig_lock(self):
        return self._fig_lock

    @property
    def ax(self):
        return self._ax

    @property
    def worker(self) -> threading.Thread:
        return self._worker

    def get_surface(self) -> Optional[pygame.Surface]:
        return self._surf

    def idle(self) -> bool:
        return (not self._on_job) and self._requests.empty()

    def call(self, callee, *args, **kargs):
        self._requests.put({'type': 'call', 'callee': callee, 'args': args, 'kargs': kargs})

    def draw(self):
        self._requests.put({'type': 'draw'})

    def discard(self):
        self._discard = True
        self._requests.put(None)

    def _draw(self):
        with self._fig_lock:
            self._canvas.draw()
            buffer = self._canvas.get_renderer().buffer_rgba()
            self._surf = pygame.image.frombuffer(buffer, self._surf_size, 'RGBA')

    def _worker_entry(self):
        while True:
            self._on_job = False
            req = self._requests.get(block=True)
            if self._discard:
                break

            self._on_job = True
            if (req_type := req['type']) == 'call':
                req['callee'](*req['args'], **req['kargs'])
            elif req_type == 'draw':
                self._draw()
            else:
                raise ValueError('')
