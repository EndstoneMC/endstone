from endstone._internal.endstone_python import (
    Event,
    EventPriority,
    PluginEnableEvent,
    PluginDisableEvent,
    ServerCommandEvent,
    ServerListPingEvent,
    ServerLoadEvent,
)

__all__ = [
    "event_handler",
    "Event",
    "EventPriority",
    "PluginEnableEvent",
    "PluginDisableEvent",
    "ServerCommandEvent",
    "ServerListPingEvent",
    "ServerLoadEvent",
]


def event_handler(func=None, *, priority: EventPriority = EventPriority.NORMAL, ignore_cancelled: bool = False):
    def decorator(f):
        setattr(f, "_is_event_handler", True)
        setattr(f, "_priority", priority)
        setattr(f, "_ignore_cancelled", ignore_cancelled)
        return f

    if func:
        return decorator(func)

    return decorator
