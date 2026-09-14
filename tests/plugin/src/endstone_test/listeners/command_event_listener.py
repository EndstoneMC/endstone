from endstone.event import UnknownCommandEvent, event_handler

from endstone_test.checks import MUTATE

from .event_listener import EventListener


class CommandEventListener(EventListener):
    @event_handler
    def on_unknown_command(self, event: UnknownCommandEvent):
        if event.command_line == "endstone_test_unknown_command_custom":
            event.message = "Endstone Test replaced the unknown-command message"
        elif event.command_line == "endstone_test_unknown_command_silent":
            event.message = None

        self.record(
            event,
            f"{event.sender.name} sent unknown command: {event.command_line}",
            sender=event.sender.name,
            command_line=event.command_line,
            message=None if event.message is None else str(event.message),
            message_present=event.message is not None,
        )
        if self.due(event, MUTATE):
            before = None if event.message is None else str(event.message)
            event.message = "Rewritten by endstone-test."
            self.mutated(event, before=before, after=str(event.message))
