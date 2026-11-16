import pytest
from endstone import Server
from endstone.command import (
    Command,
    CommandExecutor,
    CommandSender,
    CommandSenderWrapper,
)
from endstone.lang import Translatable
from endstone.plugin import Plugin


class ArgsRecorder(CommandExecutor):
    def __init__(self):
        CommandExecutor.__init__(self)
        self.seen: list[list[str]] = []

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        self.seen.append(list(args))
        return True


@pytest.fixture
def recorder(plugin: Plugin):
    command = plugin.get_command("testoptmsg2")
    recorder = ArgsRecorder()
    command.executor = recorder
    yield recorder
    command.executor = plugin


@pytest.mark.parametrize(
    "command_line,expected",
    [
        ("testoptmsg2 a", ["a"]),
        ("testoptmsg2 a x", ["a", "x"]),
        ("testoptmsg2 b y hello world", ["b", "y", "hello world"]),
    ],
)
def test_optional_message(
    server: Server, recorder: ArgsRecorder, command_line: str, expected: list[str]
) -> None:
    """Verify an optional argument before a message can be left out."""
    errors = []

    def on_error(message: Translatable | str):
        if isinstance(message, Translatable):
            message = server.language.translate(message)
        errors.append(message)

    sender = CommandSenderWrapper(server.command_sender, on_error=on_error)
    assert server.dispatch_command(sender, command_line), errors
    assert recorder.seen == [expected]
