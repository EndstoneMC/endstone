import enum
import typing
from collections.abc import Callable

from endstone import event as events
from endstone.actor import Actor
from endstone.block import Block, BlockState
from endstone.command import CommandSender
from endstone.event import event_handler
from endstone.inventory import ItemStack, Recipe
from endstone.lang import Translatable
from endstone.level import Chunk, Dimension, Level, Location
from endstone.map import MapView
from endstone.plugin import Plugin
from endstone.util import Vector

from endstone_test.harness import fields as field_checks

if typing.TYPE_CHECKING:
    from endstone_test.plugin import EndstoneTest


def event_classes() -> list[type]:
    """Every event class the bindings export, abstract bases included."""
    classes = []
    for name in events.__all__:
        cls = getattr(events, name)
        if (
            isinstance(cls, type)
            and issubclass(cls, events.Event)
            and cls is not events.Event
        ):
            classes.append(cls)
    return classes


def properties(cls: type) -> dict[str, property]:
    found: dict[str, property] = {}
    for klass in cls.__mro__:
        for name, attr in vars(klass).items():
            if (
                isinstance(attr, property)
                and not name.startswith("_")
                and name not in found
            ):
                found[name] = attr
    return found


def writable(cls: type) -> list[str]:
    return sorted(
        name
        for name, prop in properties(cls).items()
        if prop.fset is not None and name != "is_cancelled"
    )


def plain(value, depth: int = 0):
    """A JSON-friendly view of an event field."""
    if value is None or isinstance(value, (bool, int, float, str)):
        return value
    if isinstance(value, bytes):
        return {"bytes": len(value)}
    if isinstance(value, enum.Enum):
        return value.name
    if isinstance(value, Block):
        return {
            "type": str(value.type),
            "x": value.x,
            "y": value.y,
            "z": value.z,
            "dimension": str(value.dimension.id),
        }
    if isinstance(value, BlockState):
        return {
            "type": str(value.type),
            "x": value.x,
            "y": value.y,
            "z": value.z,
            "data": str(value.data),
        }
    if isinstance(value, Actor):
        return {
            "type": str(value.type),
            "name": value.name,
            "runtime_id": value.runtime_id,
        }
    if isinstance(value, Location):
        dimension = value.dimension
        return {
            "x": value.x,
            "y": value.y,
            "z": value.z,
            "pitch": value.pitch,
            "yaw": value.yaw,
            "dimension": None if dimension is None else str(dimension.id),
        }
    if isinstance(value, Vector):
        return [value.x, value.y, value.z]
    if isinstance(value, Chunk):
        return {"x": value.x, "z": value.z, "dimension": str(value.dimension.id)}
    if isinstance(value, Dimension):
        return {"id": str(value.id)}
    if isinstance(value, Level):
        return {"name": value.name}
    if isinstance(value, ItemStack):
        return {"type": str(value.type), "amount": value.amount}
    if isinstance(value, MapView):
        return {
            "id": value.id,
            "scale": str(value.scale),
            "center_x": value.center_x,
            "center_z": value.center_z,
        }
    if isinstance(value, Recipe):
        return {
            "class": type(value).__name__,
            "id": value.id,
            "tag": value.tag,
            "result": plain(value.result, depth + 1),
        }
    if isinstance(value, Translatable):
        return {"translate": value.text, "params": list(value.params)}
    if isinstance(value, Plugin):
        return {"plugin": value.name}
    if isinstance(value, CommandSender):
        return {"sender": value.name, "class": type(value).__name__}
    if depth < 2 and isinstance(value, (list, tuple, set, frozenset)):
        return [plain(v, depth + 1) for v in value]
    if depth < 2 and isinstance(value, dict):
        return {str(plain(k, depth + 1)): plain(v, depth + 1) for k, v in value.items()}
    if depth < 2 and hasattr(value, "__len__") and (hasattr(value, "__getitem__") or hasattr(value, "__iter__")):
        return [plain(v, depth + 1) for v in value]
    return repr(value)


def snapshot(event) -> dict:
    fields = {}
    for name in properties(type(event)):
        try:
            fields[name] = plain(getattr(event, name))
        except Exception as e:  # noqa: BLE001
            fields[name] = {"error": f"{type(e).__name__}: {e}"}
    return fields


class Capture:
    """Records the matching events of one class while open, cancelling or mutating each inside the handler."""

    def __init__(
        self,
        name: str,
        where: Callable | None = None,
        cancel: bool = False,
        mutate: Callable | None = None,
        extra: Callable | None = None,
    ):
        self.name = name
        self.where = where
        self.cancel = cancel
        self.mutate = mutate
        self.extra = extra
        self.events: list[dict] = []
        self.error: BaseException | None = None
        self.open = True

    @property
    def count(self) -> int:
        return len(self.events)

    def offer(self, event) -> None:
        try:
            if self.where is not None and not self.where(event):
                return
            fields = snapshot(event)
            if self.extra is not None:
                fields.update(self.extra(event))
            field_check = field_checks.CHECKS.get(self.name)
            if field_check is not None:
                try:
                    field_check(event, fields, self.events)
                except AssertionError as e:
                    raise AssertionError(
                        f"{self.name} broke an invariant: {e or 'see fields.py'}"
                    ) from e
            self.events.append(fields)
            if self.cancel:
                event.is_cancelled = True
                event.cancel()
                assert event.is_cancelled, f"{self.name}.is_cancelled is False after cancelling it"
            if self.mutate is not None:
                self.mutate(event)
                fields["after"] = snapshot(event)
        except Exception as e:  # noqa: BLE001
            if self.error is None:
                self.error = e

    def summary(self) -> dict:
        return {"event": self.name, "count": self.count}


class Hub:
    """One handler per event class, fanning out to whichever captures are open."""

    STARTUP = ("ServerLoadEvent", "PluginEnableEvent", "DimensionLoadEvent")

    def __init__(self, plugin: "EndstoneTest"):
        self.plugin = plugin
        field_checks.SERVER = plugin.server
        self.captures: dict[str, list[Capture]] = {}
        self.journal: list[dict] = []
        self.startup = {name: self.open(name) for name in self.STARTUP}

    def register(self, classes: list[type]) -> None:
        def handler_for(cls: type):
            name = cls.__name__

            def handler(listener, event):
                listener.hub.dispatch(name, event)

            handler.__annotations__ = {"event": cls}
            return event_handler(handler)

        namespace = {f"on_{cls.__name__}": handler_for(cls) for cls in classes}
        listener = type("HubListener", (), namespace)()
        listener.hub = self
        self.plugin.register_events(listener)

    def open(self, name: str, **options) -> Capture:
        capture = Capture(name, **options)
        self.captures.setdefault(name, []).append(capture)
        return capture

    def close(self, capture: Capture) -> None:
        capture.open = False
        listeners = self.captures.get(capture.name, [])
        if capture in listeners:
            listeners.remove(capture)

    def dispatch(self, name: str, event) -> None:
        if name in field_checks.JOURNALED:
            self.journal.append(
                {"seq": len(self.journal), "event": name, "fields": snapshot(event)}
            )
        for capture in tuple(self.captures.get(name, ())):
            capture.offer(event)

    def journal_violations(self, since: int) -> list[str]:
        scenario = self.journal[since:]
        violations = []
        for check in field_checks.JOURNAL_CHECKS:
            try:
                check(scenario, self.journal)
            except AssertionError as e:
                violations.append(f"{check.__name__.strip('_')}: {e}")
        return violations
