from endstone.event import Cancellable, Event


class CustomEvent(Event):
    def __init__(self, is_async: bool = False):
        super().__init__(is_async)


class CancellableCustomEvent(Event, Cancellable):
    def __init__(self, is_async: bool = False):
        super().__init__(is_async)


class TestCustomEvent:
    def test_event_name(self):
        event = CustomEvent()
        assert event.event_name.endswith("test_event.CustomEvent")

    def test_not_cancellable(self):
        event = CustomEvent()
        assert not hasattr(event, "cancel")

    def test_not_async(self):
        event = CustomEvent()
        assert not event.is_asynchronous

    def test_async(self):
        event = CustomEvent(is_async=True)
        assert event.is_asynchronous


class TestCancellableCustomEvent:
    def test_event_name(self):
        event = CancellableCustomEvent()
        assert event.event_name.endswith("test_event.CancellableCustomEvent")

    def test_is_cancellable(self):
        event = CancellableCustomEvent()
        assert hasattr(event, "cancel")

    def test_default_not_cancelled(self):
        event = CancellableCustomEvent()
        assert not event.is_cancelled

    def test_set_cancelled(self):
        event = CancellableCustomEvent()
        event.is_cancelled = True
        assert event.is_cancelled

    def test_uncancel(self):
        event = CancellableCustomEvent()
        event.cancel()
        event.is_cancelled = False
        assert not event.is_cancelled

    def test_cancel(self):
        event = CancellableCustomEvent()
        event.cancel()
        assert event.is_cancelled

    def test_async_cancellable(self):
        event = CancellableCustomEvent(is_async=True)
        assert event.is_asynchronous
        event.cancel()
        assert event.is_cancelled
