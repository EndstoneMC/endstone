from endstone.event import (
    ThunderChangeEvent,
    WeatherChangeEvent,
    event_handler,
)

from .event_listener import EventListener


class WeatherEventListener(EventListener):
    @event_handler
    def on_thunder_change(self, event: ThunderChangeEvent):
        self.plugin.on_event_triggered(
            event, f"Thunder state changed to {event.to_thunder_state}"
        )

    @event_handler
    def on_weather_change(self, event: WeatherChangeEvent):
        self.plugin.on_event_triggered(
            event, f"Weather state changed to {event.to_weather_state}"
        )
