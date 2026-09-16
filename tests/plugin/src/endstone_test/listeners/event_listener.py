import inspect
import typing

from endstone import Server
from endstone.event import Event

if typing.TYPE_CHECKING:
    from endstone_test.plugin import EndstoneTest


class EventListener:
    def __init__(self, plugin: "EndstoneTest"):
        self.plugin = plugin
        self.server: Server = plugin.server
        self.track_events()

    def track_events(self):
        for attr_name in dir(self):
            func = getattr(self, attr_name)
            if not callable(func) or not getattr(func, "_is_event_handler", False):
                continue

            sig = inspect.signature(func)
            params = list(sig.parameters.values())
            assert len(params) == 1, "Event handlers must have a single parameter"
            assert inspect.isclass(params[0].annotation)
            assert issubclass(params[0].annotation, Event)

            event_cls = params[0].annotation
            event_name = getattr(event_cls, "NAME", event_cls.__name__)
            self.plugin.track_event(event_name)
