import pytest
from endstone import Server
from endstone.command import Command, CommandMap, CommandSender, CommandSenderWrapper
from endstone.lang import Translatable

# =============================================================================
# Fixtures
# =============================================================================


class RecordingCommand(Command):
    def __init__(self, name: str, **kwargs):
        super().__init__(name, **kwargs)
        self.calls: list[list[str]] = []

    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        self.calls.append(list(args))
        return True


@pytest.fixture
def command_map(server: Server) -> CommandMap:
    return server.command_map


@pytest.fixture
def sender(server: Server):
    return server.command_sender


# =============================================================================
# Section 1: Reaching the command map
# =============================================================================


def test_server_exposes_the_command_map(command_map: CommandMap) -> None:
    """Verify Server.command_map hands back a CommandMap."""
    assert isinstance(command_map, CommandMap)


def test_get_a_registered_command(command_map: CommandMap) -> None:
    """Verify a plugin command declared up front is reachable through the map."""
    command = command_map.get_command("test")
    assert command is not None
    assert command.name == "test"


def test_get_an_unknown_command(command_map: CommandMap) -> None:
    """Verify an unknown command name yields None."""
    assert command_map.get_command("definitely_not_a_command") is None


# =============================================================================
# Section 2: Registering at runtime
# =============================================================================


def test_register_a_command_at_runtime(command_map: CommandMap) -> None:
    """Verify a command can be registered after the plugin has started."""
    command = RecordingCommand("endstone_test_runtime")
    assert command.is_registered is False
    assert command_map.register_command(command) is True
    assert command.is_registered is True
    assert command_map.get_command("endstone_test_runtime") is not None


def test_register_a_taken_name(command_map: CommandMap) -> None:
    """Verify registering a name that is already taken is refused."""
    assert command_map.register_command(RecordingCommand("test")) is False


def test_python_subclass_execute_is_called(
    command_map: CommandMap, sender: CommandSender
) -> None:
    """Verify a Python Command subclass has its execute() actually invoked."""
    command = RecordingCommand(
        "endstone_test_execute",
        usages=["/endstone_test_execute <first: string> <second: string>"],
    )
    assert command_map.register_command(command) is True

    assert command_map.dispatch(sender, "endstone_test_execute alpha beta") is True
    assert command.calls == [["alpha", "beta"]]


def test_dispatch_passes_no_arguments(
    command_map: CommandMap, sender: CommandSender
) -> None:
    """Verify a bare dispatch reaches execute() with an empty argument list."""
    command = RecordingCommand("endstone_test_noargs")
    assert command_map.register_command(command) is True

    assert command_map.dispatch(sender, "endstone_test_noargs") is True
    assert command.calls == [[]]


def test_register_with_metadata(command_map: CommandMap) -> None:
    """Verify description, usages and aliases survive runtime registration."""
    command = RecordingCommand(
        "endstone_test_meta",
        description="A runtime command",
        usages=["/endstone_test_meta"],
        aliases=["etm"],
    )
    assert command_map.register_command(command) is True

    registered = command_map.get_command("endstone_test_meta")
    assert registered.description == "A runtime command"
    assert registered.usages == ["/endstone_test_meta"]
    assert registered.aliases == ["etm"]


def test_dispatch_an_unknown_command(
    command_map: CommandMap, sender: CommandSender
) -> None:
    """Verify an unknown command reports failure with the default message."""
    errors = []
    wrapper = CommandSenderWrapper(sender, on_error=errors.append)

    assert command_map.dispatch(wrapper, "definitely_not_a_command value") is False
    assert len(errors) == 1
    assert isinstance(errors[0], Translatable)
    assert errors[0].text == "commands.generic.unknown"
    assert errors[0].params == ["definitely_not_a_command"]


def test_unknown_command_message_can_be_replaced(
    command_map: CommandMap, sender: CommandSender
) -> None:
    """Verify a listener can replace the unknown-command response."""
    errors = []
    wrapper = CommandSenderWrapper(sender, on_error=errors.append)

    assert (
        command_map.dispatch(wrapper, "/endstone_test_unknown_command_custom") is False
    )
    assert errors == ["Endstone Test replaced the unknown-command message"]


def test_unknown_command_message_can_be_suppressed(
    command_map: CommandMap, sender: CommandSender
) -> None:
    """Verify a listener can suppress the unknown-command response."""
    errors = []
    wrapper = CommandSenderWrapper(sender, on_error=errors.append)

    assert (
        command_map.dispatch(wrapper, "endstone_test_unknown_command_silent") is False
    )
    assert errors == []


def test_clear_commands_exists(command_map: CommandMap) -> None:
    """Verify clear_commands is exposed without calling it on the live server."""
    assert callable(command_map.clear_commands)
