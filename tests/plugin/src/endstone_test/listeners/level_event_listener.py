from endstone.event import (
    ChunkLoadEvent,
    ChunkUnloadEvent,
    event_handler,
)

from .event_listener import EventListener


class LevelEventListener(EventListener):
    @event_handler
    def on_chunk_load(self, event: ChunkLoadEvent):
        self.plugin.on_event_triggered(event, f"{event.chunk} is loaded")

    @event_handler
    def on_chunk_unload(self, event: ChunkUnloadEvent):
        self.plugin.on_event_triggered(event, f"{event.chunk} is unloaded")
