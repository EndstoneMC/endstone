"""
Classes relating to plugin handling of map displays.
"""

from __future__ import annotations

import enum
import typing

import numpy

from endstone import Player
from endstone.level import Dimension

__all__: list[str] = ["MapCanvas", "MapRenderer", "MapView"]

class MapCanvas:
    """
    Represents a canvas for drawing to a map. Each canvas is associated with a specific MapRenderer and represents that renderer's layer on the map.
    """

    def draw_image(self, x: int, y: int, image: numpy.ndarray[numpy.uint8]) -> None:
        """
        Draw an image to the map. The image will be clipped if necessary.
        """

    def get_base_pixel_color(self, x: int, y: int) -> tuple[int, ...]:
        """
        Get a pixel from the layers below this canvas.
        """

    def get_pixel_color(self, x: int, y: int) -> tuple[int, ...] | None:
        """
        Get a pixel from the canvas.

        If no color is set at the given position for this canvas, then None is returned and the color returned by get_base_pixel_color() is shown on the map
        """

    def set_pixel_color(self, x: int, y: int, color: tuple[int, ...] | None) -> None:
        """
        Draw a pixel to the canvas.

        If None is used as color, then the color returned by get_base_pixel_color() is shown on the map.
        """

    @property
    def map_view(self) -> MapView:
        """
        Get the map this canvas is attached to.
        """

class MapRenderer:
    """
    Represents a renderer for a map.
    """

    def __init__(self, is_contextual: bool = False) -> None:
        """
        Initialize the map renderer base with the given contextual status.
        """

    def initialize(self, view: MapView) -> None:
        """
        Initialize this MapRenderer for the given map.
        """

    def render(self, map: MapView, canvas: MapCanvas, player: Player) -> None:
        """
        Render to the given map.
        """

class MapView:
    """
    Represents a map item.
    """

    class Scale(enum.IntEnum):
        """
        An enum representing all possible scales a map can be set to.
        """

        CLOSE: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSE: 1>
        CLOSEST: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSEST: 0>
        FAR: typing.ClassVar[MapView.Scale]  # value = <Scale.FAR: 3>
        FARTHEST: typing.ClassVar[MapView.Scale]  # value = <Scale.FARTHEST: 4>
        NORMAL: typing.ClassVar[MapView.Scale]  # value = <Scale.NORMAL: 2>

        @classmethod
        def __new__(cls, value): ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """

    CLOSE: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSE: 1>
    CLOSEST: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSEST: 0>
    FAR: typing.ClassVar[MapView.Scale]  # value = <Scale.FAR: 3>
    FARTHEST: typing.ClassVar[MapView.Scale]  # value = <Scale.FARTHEST: 4>
    NORMAL: typing.ClassVar[MapView.Scale]  # value = <Scale.NORMAL: 2>

    def add_renderer(self, renderer: MapRenderer) -> None:
        """
        Add a renderer to this map.
        """

    def remove_renderer(self, renderer: MapRenderer) -> bool:
        """
        Remove a renderer from this map.
        """

    @property
    def center_x(self) -> int:
        """
        Get or set the center X position of this map.
        """

    @center_x.setter
    def center_x(self, arg1: int) -> None: ...
    @property
    def center_z(self) -> int:
        """
        Get or set the center Z position of this map.
        """

    @center_z.setter
    def center_z(self, arg1: int) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        Get or set the dimension that this map is associated with.
        """

    @dimension.setter
    def dimension(self, arg1: Dimension) -> None: ...
    @property
    def id(self) -> int:
        """
        Get the ID of this map item for use with MapMeta.
        """

    @property
    def is_unlimited_tracking(self) -> bool:
        """
        Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range.
        """

    @is_unlimited_tracking.setter
    def is_unlimited_tracking(self, arg1: bool) -> None: ...
    @property
    def is_virtual(self) -> bool:
        """
        Check whether this map is virtual.
        """

    @property
    def locked(self) -> bool:
        """
        Whether the map is locked or not. A locked map may not be explored further.
        """

    @locked.setter
    def locked(self, arg1: bool) -> None: ...
    @property
    def renderers(self) -> list[MapRenderer]:
        """
        Get a copied list of MapRenderers currently in effect.
        """

    @property
    def scale(self) -> MapView.Scale:
        """
        Get or set the scale of this map.
        """

    @scale.setter
    def scale(self, arg1: MapView.Scale) -> None: ...
