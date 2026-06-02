"""
Classes relating to plugin handling of map displays.
"""

import enum

import numpy

from endstone import Player
from endstone.level import Dimension

__all__ = [
    "MapCanvas",
    "MapCursor",
    "MapRenderer",
    "MapView",
]

class MapCursor:
    """
    Represents a cursor on a map.
    """
    def __init__(self, x: int, y: int, direction: int, type: Type, visible: bool, caption: str = "") -> None: ...
    class Type(enum.Enum):
        """
        Represents the standard types of map cursors.
        """

        PLAYER = 0
        FRAME = 1
        RED_MARKER = 2
        BLUE_MARKER = 3
        TARGET_X = 4
        TARGET_POINT = 5
        PLAYER_OFF_MAP = 6
        SIGN_MARKER = 7
        PINK_MARKER = 8
        ORANGE_MARKER = 9
        YELLOW_MARKER = 10
        CYAN_MARKER = 11
        GREEN_POINT = 12
        PLAYER_OFF_LIMITS = 13
        MANSION = 14
        MONUMENT = 15
        VILLAGE_DESERT = 17
        VILLAGE_PLAINS = 18
        VILLAGE_SAVANNA = 19
        VILLAGE_SNOWY = 20
        VILLAGE_TAIGA = 21
        JUNGLE_TEMPLE = 22
        SWAMP_HUT = 23
        TRIAL_CHAMBERS = 24

    @property
    def x(self) -> int:
        """
        The X position of this cursor.
        """
        ...
    @x.setter
    def x(self, arg1: int) -> None: ...
    @property
    def y(self) -> int:
        """
        The Y position of this cursor.
        """
        ...
    @y.setter
    def y(self, arg1: int) -> None: ...
    @property
    def direction(self) -> int:
        """
        The facing of the cursor, from 0 to 15.
        """
        ...
    @direction.setter
    def direction(self, arg1: int) -> None: ...
    @property
    def type(self) -> Type:
        """
        The type (color/style) of this map cursor.
        """
        ...
    @type.setter
    def type(self, arg1: Type) -> None: ...
    @property
    def is_visible(self) -> bool:
        """
        The visibility status of this cursor.
        """
        ...
    @is_visible.setter
    def is_visible(self, arg1: bool) -> None: ...
    @property
    def caption(self) -> str:
        """
        The caption on this cursor.
        """
        ...
    @caption.setter
    def caption(self, arg1: str) -> None: ...

class MapView:
    """
    Represents a map item.
    """
    class Scale(enum.IntEnum):
        """
        An enum representing all possible scales a map can be set to.
        """

        CLOSEST = 0
        CLOSE = 1
        NORMAL = 2
        FAR = 3
        FARTHEST = 4

    CLOSEST = Scale.CLOSEST
    CLOSE = Scale.CLOSE
    NORMAL = Scale.NORMAL
    FAR = Scale.FAR
    FARTHEST = Scale.FARTHEST
    @property
    def id(self) -> int:
        """
        The unique ID of this map item for use with `MapMeta`.
        """
        ...
    @property
    def is_virtual(self) -> bool:
        """
        Whether this map is virtual.

        A map is virtual if its lowermost `MapRenderer` is plugin-provided.
        """
        ...
    @property
    def scale(self) -> Scale:
        """
        The scale of this map.
        """
        ...
    @scale.setter
    def scale(self, arg1: Scale) -> None: ...
    @property
    def center_x(self) -> int:
        """
        The center X position of this map.
        """
        ...
    @center_x.setter
    def center_x(self, arg1: int) -> None: ...
    @property
    def center_z(self) -> int:
        """
        The center Z position of this map.
        """
        ...
    @center_z.setter
    def center_z(self, arg1: int) -> None: ...
    @property
    def dimension(self) -> Dimension:
        """
        The dimension that this map is associated with.
        """
        ...
    @dimension.setter
    def dimension(self, arg1: Dimension) -> None: ...
    @property
    def renderers(self) -> list[MapRenderer]:
        """
        A list of `MapRenderer`s currently in effect.
        """
        ...
    def add_renderer(self, renderer: MapRenderer) -> None:
        """
        Add a renderer to this map.

        Args:
            renderer: The `MapRenderer` to add.
        """
        ...
    def remove_renderer(self, renderer: MapRenderer) -> bool:
        """
        Remove a renderer from this map.

        Args:
            renderer: The `MapRenderer` to remove.

        Returns:
            `True` if the renderer was successfully removed.
        """
        ...
    @property
    def is_unlimited_tracking(self) -> bool:
        """
        Whether the map will show a smaller position cursor (`True`), or no position cursor (`False`) when cursor is
        outside of map's range.
        """
        ...
    @is_unlimited_tracking.setter
    def is_unlimited_tracking(self, arg1: bool) -> None: ...
    @property
    def locked(self) -> bool:
        """
        Whether the map is locked or not. A locked map may not be explored further.
        """
        ...
    @locked.setter
    def locked(self, arg1: bool) -> None: ...

class MapCanvas:
    """
    Represents a canvas for drawing to a map.

    Each canvas is associated with a specific `MapRenderer` and represents that
    renderer's layer on the map.
    """
    @property
    def map_view(self) -> MapView:
        """
        The `MapView` this canvas is attached to.
        """
        ...
    @property
    def cursors(self) -> list[MapCursor]:
        """
        The cursors associated with this canvas.
        """
        ...
    @cursors.setter
    def cursors(self, arg1: list[MapCursor]) -> None: ...
    def set_pixel_color(self, x: int, y: int, color: tuple[int, ...]) -> None:
        """
        Draw a pixel to the canvas.

        Args:
            x: The x coordinate, from 0 to 127.
            y: The y coordinate, from 0 to 127.
            color: The color.
        """
        ...
    def get_pixel_color(self, x: int, y: int) -> tuple[int, ...]:
        """
        Get a pixel from the canvas.

        Args:
            x: The x coordinate, from 0 to 127.
            y: The y coordinate, from 0 to 127.

        Returns:
            The color.
        """
        ...
    def set_pixel(self, x: int, y: int, color: int) -> None:
        """
        Draw a pixel to the canvas.

        Args:
            x: The x coordinate, from 0 to 127.
            y: The y coordinate, from 0 to 127.
            color: The color.
        """
        ...
    def get_pixel(self, x: int, y: int) -> int:
        """
        Get a pixel from the canvas.

        Args:
            x: The x coordinate, from 0 to 127.
            y: The y coordinate, from 0 to 127.

        Returns:
            The color.
        """
        ...
    def draw_image(self, x: int, y: int, image: numpy.ndarray[numpy.uint8]) -> None:
        """
        Draw an image to the map. The image will be clipped if necessary.

        Args:
            x: The x coordinate of the image.
            y: The y coordinate of the image.
            image: The `Image` to draw.
        """
        ...

class MapRenderer:
    """
    Represents a renderer for a map.
    """
    def __init__(self, is_contextual: bool = False) -> None:
        """
        Initialize the map renderer base with the given contextual status.
        """
        ...
    def initialize(self, view: MapView) -> None:
        """
        Initialize this `MapRenderer` for the given map.

        Args:
            view: The `MapView` being initialized.
        """
        ...
    def render(self, view: MapView, canvas: MapCanvas, player: Player) -> None:
        """
        Render to the given map.

        Args:
            view: The `MapView` being rendered to.
            canvas: The canvas to use for rendering.
            player: The player who triggered the rendering.
        """
        ...
