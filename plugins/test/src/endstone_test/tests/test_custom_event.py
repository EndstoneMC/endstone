from endstone import Server
from endstone.event import Cancellable, Event, EventPriority, event_handler
from endstone.plugin import Plugin


class CustomEvent(Event):
    """A simple non-cancellable custom event."""

    pass


class CancellableCustomEvent(Event, Cancellable):
    """A cancellable custom event."""

    pass


class TestCustomEvent:
    def test_fire_and_handle(self, server: Server, plugin: Plugin):
        handled = []

        class Listener:
            @event_handler
            def on_custom(self, event: CustomEvent):
                handled.append(True)

        plugin.register_events(Listener())
        server.plugin_manager.call_event(CustomEvent())
        assert len(handled) == 1

    def test_event_name(self):
        event = CustomEvent()
        assert event.event_name.endswith("test_custom_event.CustomEvent")

    def test_handler_receives_event_instance(self, server: Server, plugin: Plugin):
        received = []

        class Listener:
            @event_handler
            def on_custom(self, event: CustomEvent):
                received.append(event)

        plugin.register_events(Listener())
        event = CustomEvent()
        server.plugin_manager.call_event(event)
        assert len(received) == 1
        assert received[0] is event


class TestCancellableCustomEvent:
    def test_fire_and_handle(self, server: Server, plugin: Plugin):
        handled = []

        class Listener:
            @event_handler
            def on_custom(self, event: CancellableCustomEvent):
                handled.append(True)

        plugin.register_events(Listener())
        server.plugin_manager.call_event(CancellableCustomEvent())
        assert len(handled) == 1

    def test_cancel_in_handler(self, server: Server, plugin: Plugin):
        class Listener:
            @event_handler
            def on_custom(self, event: CancellableCustomEvent):
                event.cancel()

        plugin.register_events(Listener())
        event = CancellableCustomEvent()
        server.plugin_manager.call_event(event)
        assert event.is_cancelled

    def test_ignore_cancelled_skips_handler(self, server: Server, plugin: Plugin):
        results = []

        class CancellingListener:
            @event_handler(priority=EventPriority.LOW)
            def on_custom(self, event: CancellableCustomEvent):
                event.cancel()
                results.append("low")

        class IgnoringListener:
            @event_handler(priority=EventPriority.HIGH, ignore_cancelled=True)
            def on_custom(self, event: CancellableCustomEvent):
                results.append("high_ignore")

        plugin.register_events(CancellingListener())
        plugin.register_events(IgnoringListener())
        event = CancellableCustomEvent()
        server.plugin_manager.call_event(event)
        assert "low" in results
        assert "high_ignore" not in results

    def test_default_handler_still_runs_when_cancelled(self, server: Server, plugin: Plugin):
        results = []

        class CancellingListener:
            @event_handler(priority=EventPriority.LOW)
            def on_custom(self, event: CancellableCustomEvent):
                event.cancel()
                results.append("low")

        class DefaultListener:
            @event_handler(priority=EventPriority.HIGH)
            def on_custom(self, event: CancellableCustomEvent):
                results.append("high_default")

        plugin.register_events(CancellingListener())
        plugin.register_events(DefaultListener())
        event = CancellableCustomEvent()
        server.plugin_manager.call_event(event)
        assert "low" in results
        assert "high_default" in results
        assert event.is_cancelled
