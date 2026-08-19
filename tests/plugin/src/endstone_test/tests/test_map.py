import pytest
from endstone import Server
from endstone.level import Dimension
from endstone.map import MapCanvas, MapRenderer, MapView

from endstone_test.image_renderer import ImageRenderer, make_test_pattern


def test_pattern_shape() -> None:
    """Verify the generated test image is a 128x128 RGBA array."""
    image = make_test_pattern()
    assert image.shape == (128, 128, 4)
    assert image.dtype.name == "uint8"


def test_pattern_is_deterministic() -> None:
    """Verify the generated test image is stable across calls."""
    assert (make_test_pattern() == make_test_pattern()).all()


def test_pattern_ramps_and_checkers() -> None:
    """Verify the test image ramps on red and green and checkers on blue."""
    image = make_test_pattern()
    assert image[0, 0, 0] < image[0, -1, 0]
    assert image[0, 0, 1] < image[-1, 0, 1]
    assert image[0, 0, 2] != image[0, 16, 2]
    assert (image[..., 3] == 255).all()


def test_renderer_is_a_map_renderer() -> None:
    """Verify the image renderer subclasses MapRenderer and starts undrawn."""
    renderer = ImageRenderer()
    assert isinstance(renderer, MapRenderer)
    assert renderer.rendered is False
    assert (renderer.image == make_test_pattern()).all()


def test_add_and_remove_a_renderer(server: Server) -> None:
    """Verify a renderer can be attached to a map view and taken off again."""
    view = server.create_map(server.level.get_dimension(Dimension.OVERWORLD))
    original = list(view.renderers)

    renderer = ImageRenderer()
    view.add_renderer(renderer)
    assert renderer in view.renderers

    view.remove_renderer(renderer)
    assert renderer not in view.renderers
    assert len(view.renderers) == len(original)


def test_created_map_has_a_valid_view(server: Server) -> None:
    """Verify a freshly created map view is usable."""
    view = server.create_map(server.level.get_dimension(Dimension.OVERWORLD))
    assert isinstance(view, MapView)
    assert view.id != -1
    assert server.get_map(view.id) is not None


def test_canvas_is_not_constructible() -> None:
    """Verify a canvas only ever arrives through MapRenderer.render."""
    with pytest.raises(TypeError):
        MapCanvas()
