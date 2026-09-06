from endstone.event import ThunderChangeEvent, WeatherChangeEvent, event_handler

from .event_listener import EventListener


class WeatherEventListener(EventListener):
    @event_handler
    def on_thunder_change(self, event: ThunderChangeEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("ThunderChangeEvent"):
            event.cancel()
        self.record(
            event,
            f"Thunder state changed to {event.to_thunder_state}",
            to_thunder_state=event.to_thunder_state,
            level=event.level.name,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_weather_change(self, event: WeatherChangeEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("WeatherChangeEvent"):
            event.cancel()
        self.record(
            event,
            f"Weather state changed to {event.to_weather_state}",
            to_weather_state=event.to_weather_state,
            level=event.level.name,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )
