import numpy as np
from endstone import Player
from endstone.map import MapCanvas, MapRenderer, MapView

SIZE = 128
TILE = 16


def make_test_pattern(size: int = SIZE) -> np.ndarray:
    y, x = np.mgrid[0:size, 0:size]
    image = np.zeros((size, size, 4), np.uint8)
    image[..., 0] = x * (255 // (size - 1))
    image[..., 1] = y * (255 // (size - 1))
    image[..., 2] = ((x // TILE + y // TILE) % 2) * 255
    image[..., 3] = 255
    return image


class ImageRenderer(MapRenderer):
    def __init__(self, image: np.ndarray | None = None):
        MapRenderer.__init__(self, is_contextual=False)
        self.image = make_test_pattern() if image is None else image
        self.rendered = False

    def render(self, view: MapView, canvas: MapCanvas, player: Player) -> None:
        if self.rendered:
            return

        canvas.draw_image(0, 0, self.image)
        self.rendered = True
