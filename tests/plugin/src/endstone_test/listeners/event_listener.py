import inspect
import typing

from endstone import Server
from endstone.event import Event

from endstone_test.checks import CANCEL, MUTATE

if typing.TYPE_CHECKING:
    from endstone_test.plugin import EndstoneTest


class EventListener:
    def __init__(self, plugin: "EndstoneTest"):
        self.plugin = plugin
        self.server: Server = plugin.server
        self.recorder = plugin.recorder
        self.track_events()

    def track_events(self) -> None:
        self.handled: set[str] = set()
        for attr_name in dir(self):
            func = getattr(self, attr_name)
            if not callable(func) or not getattr(func, "_is_event_handler", False):
                continue

            params = list(inspect.signature(func).parameters.values())
            assert len(params) == 1, "Event handlers must have a single parameter"
            assert inspect.isclass(params[0].annotation)
            assert issubclass(params[0].annotation, Event)

            self.recorder.expect(params[0].annotation)
            self.handled.add(params[0].annotation.__name__)

    def record(
        self, event: Event, summary: str = "", *, always_log: bool = False, **fields
    ) -> None:
        self.recorder.record(event, summary, always_log=always_log, **fields)

    def due(self, event: Event, action: str) -> bool:
        key = f"{type(event).__name__}/{action}"
        # The automated targeted runner intentionally skips the embedded
        # pytest suite. In that mode no interactive check is registered, so a
        # first event must not be cancelled or mutated merely because its
        # check counter is zero. Full runs still register every check and keep
        # the original one-time check behavior.
        return self.recorder.has_check(key) and not self.recorder.checked(key)

    def cancelled(self, event: Event, **fields) -> None:
        event.cancel()
        self.recorder.pass_check(f"{type(event).__name__}/{CANCEL}", **fields)

    def mutated(self, event: Event, **fields) -> None:
        self.recorder.pass_check(f"{type(event).__name__}/{MUTATE}", **fields)
