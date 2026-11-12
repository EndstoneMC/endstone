import pytest
from endstone import Server
from endstone.command import Command, CommandMap, CommandSender, CommandSenderWrapper

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


def test_register_a_mixed_case_command_with_an_enum(command_map: CommandMap) -> None:
    """Verify a mixed-case command with an enum argument registers under its lowercased name."""
    command = RecordingCommand(
        "Endstone_Test_MixedCase",
        usages=["/endstone_test_mixedcase (on|off)<state: EndstoneTestMixedState>"],
    )
    assert command.name == "endstone_test_mixedcase"
    assert command_map.register_command(command) is True
    assert command_map.get_command("ENDSTONE_TEST_MIXEDCASE") is not None


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
    """Verify CommandMap.dispatch reports an unknown command silently."""
    errors = []
    wrapper = CommandSenderWrapper(sender, on_error=errors.append)

    assert command_map.dispatch(wrapper, "definitely_not_a_command value") is False
    assert errors == []


def test_server_dispatch_an_unknown_command(
    server: Server, sender: CommandSender
) -> None:
    """Verify Server.dispatch_command reports an unknown command silently."""
    errors = []
    wrapper = CommandSenderWrapper(sender, on_error=errors.append)

    assert server.dispatch_command(wrapper, "definitely_not_a_command value") is False
    assert errors == []


class FailingCommand(Command):
    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        return False


def test_dispatch_a_failing_command_is_handled(
    command_map: CommandMap, server: Server, sender: CommandSender
) -> None:
    """Verify dispatch returns True for a found command whose execute() fails."""
    command = FailingCommand("endstone_test_failing")
    assert command_map.register_command(command) is True

    assert command_map.dispatch(sender, "endstone_test_failing") is True
    assert server.dispatch_command(sender, "endstone_test_failing") is True
    command.unregister_from(command_map)


def test_get_a_vanilla_command_is_stable(command_map: CommandMap) -> None:
    """Verify get_command hands back the same object for a vanilla command."""
    assert command_map.get_command("say") is command_map.get_command("say")


def test_clear_commands_exists(command_map: CommandMap) -> None:
    """Verify clear_commands is exposed without calling it on the live server."""
    assert callable(command_map.clear_commands)
