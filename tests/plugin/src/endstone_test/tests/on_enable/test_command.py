import pytest
from endstone import Server
from endstone.command import CommandSenderWrapper
from endstone.lang import Translatable
from endstone.plugin import Plugin, PluginCommand

from endstone_test.command_executor import TestCommandExecutor


@pytest.fixture
def command(plugin: Plugin) -> PluginCommand:
    return plugin.get_command("test")


def test_command(plugin: Plugin, command: PluginCommand) -> None:
    assert command is not None
    assert command.plugin is plugin


def test_command_executor(plugin: Plugin, command: PluginCommand) -> None:
    assert command.executor is plugin
    command.executor = TestCommandExecutor(plugin)
    assert command.executor is not plugin
    assert isinstance(command.executor, TestCommandExecutor)
    command.executor = plugin
    assert command.executor is plugin
    assert not isinstance(command.executor, TestCommandExecutor)


def test_server_command_sender(plugin: Plugin) -> None:
    server = plugin.server
    assert server.command_sender.name == "Server"


@pytest.mark.parametrize(
    "command_line,locale,expected",
    [
        ("listd", "zh_CN", "玩家在线"),
        ("status", None, "Server status"),
    ],
)
def test_command_sender_wrapper(
    server: Server,
    plugin: Plugin,
    command: PluginCommand,
    command_line: str,
    locale: str | None,
    expected: str,
) -> None:
    command.executor = TestCommandExecutor(plugin)
    messages = []

    def on_message(message: Translatable | str):
        if isinstance(message, Translatable):
            messages.append(server.language.translate(message, locale="zh_CN"))
        else:
            messages.append(message)

    def on_error(message: Translatable | str):
        if isinstance(message, Translatable):
            assert False, server.language.translate(message, locale="zh_CN")
        else:
            assert False, message

    sender = CommandSenderWrapper(
        server.command_sender, on_message=on_message, on_error=on_error
    )

    assert server.dispatch_command(sender, command_line)
    assert expected in "\n".join(messages), (
        f"Expected message {expected} not found in {messages}"
    )
    command.executor = plugin
