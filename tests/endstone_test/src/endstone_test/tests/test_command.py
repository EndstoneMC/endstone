import pytest
from endstone import Server
from endstone.command import (
    Command,
    CommandExecutor,
    CommandSender,
    CommandSenderWrapper,
    ConsoleCommandSender,
)
from endstone.permissions import PermissionLevel
from endstone.plugin import Plugin

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture
def console_sender(server: Server) -> ConsoleCommandSender:
    """Get console command sender from server."""
    return server.command_sender


# =============================================================================
# Section 1: Command Construction Tests
# =============================================================================


class TestCommandConstruction:
    """Tests for Command construction and initialization."""

    def test_construct_with_name_only(self) -> None:
        """Test creating a command with just a name."""
        cmd = Command("mycommand")
        assert cmd.name == "mycommand"
        assert cmd.description == ""
        assert cmd.usages == ["/mycommand"]
        assert cmd.aliases == []
        assert cmd.permissions == []

    def test_name_is_lowercased(self) -> None:
        """Test that command names are converted to lowercase."""
        cmd = Command("TestCmd")
        assert cmd.name == "testcmd"

    def test_construct_with_description(self) -> None:
        """Test creating a command with a description."""
        cmd = Command("mycmd", description="A test command")
        assert cmd.name == "mycmd"
        assert cmd.description == "A test command"

    def test_construct_with_usages(self) -> None:
        """Test creating a command with usage strings."""
        usages = ["/mycmd <arg1>", "/mycmd <arg1> <arg2>"]
        cmd = Command("mycmd", usages=usages)
        assert cmd.usages == usages

    def test_construct_with_aliases(self) -> None:
        """Test creating a command with aliases."""
        aliases = ["mc", "myc"]
        cmd = Command("mycmd", aliases=aliases)
        assert cmd.aliases == aliases

    def test_aliases_are_lowercased(self) -> None:
        """Test that command aliases are converted to lowercase."""
        cmd = Command("mycmd", aliases=["MyAlias", "ANOTHER"])
        assert cmd.aliases == ["myalias", "another"]

    def test_construct_with_permissions(self) -> None:
        """Test creating a command with required permissions."""
        permissions = ["myplugin.command.mycmd", "myplugin.admin"]
        cmd = Command("mycmd", permissions=permissions)
        assert cmd.permissions == permissions

    def test_construct_with_all_parameters(self) -> None:
        """Test creating a command with all parameters."""
        cmd = Command(
            name="fullcmd",
            description="A fully configured command",
            usages=["/fullcmd <action>"],
            aliases=["fc", "full"],
            permissions=["test.command.full"],
        )
        assert cmd.name == "fullcmd"
        assert cmd.description == "A fully configured command"
        assert cmd.usages == ["/fullcmd <action>"]
        assert cmd.aliases == ["fc", "full"]
        assert cmd.permissions == ["test.command.full"]


# =============================================================================
# Section 2: Command Property Tests
# =============================================================================


class TestCommandProperties:
    """Tests for Command property getters and setters."""

    def test_get_name(self) -> None:
        """Test the name property getter."""
        cmd = Command("testname")
        assert cmd.name == "testname"

    def test_set_name(self) -> None:
        """Test the name property setter on unregistered command."""
        cmd = Command("original")
        cmd.name = "newname"
        assert cmd.name == "newname"

    def test_set_name_lowercases(self) -> None:
        """Test that setting name also lowercases it."""
        cmd = Command("original")
        cmd.name = "NewName"
        assert cmd.name == "newname"

    def test_get_description(self) -> None:
        """Test the description property getter."""
        cmd = Command("test", description="Test description")
        assert cmd.description == "Test description"

    def test_set_description(self) -> None:
        """Test the description property setter."""
        cmd = Command("test")
        cmd.description = "New description"
        assert cmd.description == "New description"

    def test_get_aliases(self) -> None:
        """Test the aliases property getter."""
        aliases = ["a1", "a2"]
        cmd = Command("test", aliases=aliases)
        assert cmd.aliases == aliases

    def test_set_aliases(self) -> None:
        """Test the aliases property setter."""
        cmd = Command("test")
        cmd.aliases = ["new1", "new2"]
        assert cmd.aliases == ["new1", "new2"]

    def test_get_usages(self) -> None:
        """Test the usages property getter."""
        usages = ["/test <arg>"]
        cmd = Command("test", usages=usages)
        assert cmd.usages == usages

    def test_set_usages(self) -> None:
        """Test the usages property setter."""
        cmd = Command("test")
        cmd.usages = ["/test new"]
        assert cmd.usages == ["/test new"]

    def test_get_permissions(self) -> None:
        """Test the permissions property getter."""
        permissions = ["perm1", "perm2"]
        cmd = Command("test", permissions=permissions)
        assert cmd.permissions == permissions

    def test_set_permissions(self) -> None:
        """Test the permissions property setter."""
        cmd = Command("test")
        cmd.permissions = ["new.perm"]
        assert cmd.permissions == ["new.perm"]

    def test_is_registered_false_for_new_command(self) -> None:
        """Test that is_registered returns False for newly created commands."""
        cmd = Command("unregistered")
        assert cmd.is_registered is False


# =============================================================================
# Section 3: Command Registration Immutability Tests
# =============================================================================


class TestCommandRegistration:
    """Tests for registered command behavior."""

    def test_registered_command_is_registered(self, server: Server) -> None:
        """Test that registered commands have is_registered=True."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert cmd.is_registered is True

    def test_registered_command_has_properties(self, server: Server) -> None:
        """Test that registered commands have their properties set."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert cmd.name == "test"
        assert cmd.description != ""
        assert len(cmd.usages) > 0
        assert len(cmd.permissions) > 0


# =============================================================================
# Section 4: Command Permission Testing
# =============================================================================


class TestCommandPermissions:
    """Tests for command permission checking."""

    def test_permission_silently_with_no_permissions(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test test_permission_silently with command having no required permissions."""
        cmd = Command("noperm")
        assert cmd.test_permission_silently(console_sender) is True

    def test_permission_silently_console_has_all(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that console sender has permission for commands with permissions."""
        cmd = Command("withperm", permissions=["some.permission"])
        # Console sender should have high permission level
        result = cmd.test_permission_silently(console_sender)
        assert result is True

    def test_permission_with_no_permissions(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test test_permission with command having no required permissions."""
        cmd = Command("noperm")
        assert cmd.test_permission(console_sender) is True

    def test_permission_console_sender(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test test_permission for console sender."""
        cmd = Command("test", permissions=["test.permission"])
        result = cmd.test_permission(console_sender)
        assert result is True

    def test_console_sender_permission_level(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that console sender has CONSOLE permission level."""
        assert console_sender.permission_level == PermissionLevel.CONSOLE


# =============================================================================
# Section 5: Command Execution Tests
# =============================================================================


class TestCommandExecution:
    """Tests for command execution."""

    def test_base_execute_returns_false(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that base Command.execute() returns False."""
        cmd = Command("basecmd")
        result = cmd.execute(console_sender, [])
        assert result is False

    def test_dispatch_known_command(self, server: Server) -> None:
        """Test dispatching a known command."""
        result = server.dispatch_command(
            server.command_sender, "scriptevent endstone:test hello"
        )
        assert result is True

    def test_dispatch_unknown_command(self, server: Server) -> None:
        """Test dispatching an unknown command returns False."""
        result = server.dispatch_command(
            server.command_sender, "nonexistent_command_xyz"
        )
        assert result is False

    def test_dispatch_with_arguments(self, server: Server) -> None:
        """Test dispatching a command with arguments."""
        result = server.dispatch_command(
            server.command_sender, "scriptevent endstone:test arg1 arg2 arg3"
        )
        assert result is True


# =============================================================================
# Section 6: CommandSender Tests (via ConsoleCommandSender)
# =============================================================================


class TestCommandSender:
    """Tests for CommandSender functionality via ConsoleCommandSender."""

    def test_name_is_server(self, console_sender: ConsoleCommandSender) -> None:
        """Test that console sender name is 'Server'."""
        assert console_sender.name == "Server"

    def test_server_property(
        self, server: Server, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that server property returns a valid Server."""
        assert console_sender.server is server

    def test_send_message(self, console_sender: ConsoleCommandSender) -> None:
        """Test send_message does not raise errors."""
        # Should not raise
        console_sender.send_message("Test message from test_command.py")

    def test_send_error_message(self, console_sender: ConsoleCommandSender) -> None:
        """Test send_error_message does not raise errors."""
        # Should not raise
        console_sender.send_error_message("Test error message from test_command.py")

    def test_permission_level(self, console_sender: ConsoleCommandSender) -> None:
        """Test console sender has CONSOLE permission level."""
        assert console_sender.permission_level == PermissionLevel.CONSOLE

    def test_has_permission(self, console_sender: ConsoleCommandSender) -> None:
        """Test has_permission returns True for console sender."""
        # Console should have broad permissions
        assert console_sender.has_permission("endstone.command.test") is True

    def test_is_permission_set(self, console_sender: ConsoleCommandSender) -> None:
        """Test is_permission_set method."""
        # This method should work without error
        result = console_sender.is_permission_set("some.permission")
        assert isinstance(result, bool)

    def test_effective_permissions(self, console_sender: ConsoleCommandSender) -> None:
        """Test effective_permissions returns a set."""
        perms = console_sender.effective_permissions
        assert isinstance(perms, set)


# =============================================================================
# Section 7: ConsoleCommandSender Type Tests
# =============================================================================


class TestConsoleCommandSenderType:
    """Tests for ConsoleCommandSender type verification."""

    def test_is_command_sender(self, console_sender: ConsoleCommandSender) -> None:
        """Test that ConsoleCommandSender is a CommandSender."""
        assert isinstance(console_sender, CommandSender)

    def test_is_console_command_sender(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that console_sender is a ConsoleCommandSender instance."""
        assert isinstance(console_sender, ConsoleCommandSender)


# =============================================================================
# Section 8: CommandSenderWrapper Tests
# =============================================================================


class TestCommandSenderWrapper:
    """Tests for CommandSenderWrapper functionality."""

    def test_wrapper_captures_messages(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper captures messages via on_message callback."""
        messages = []

        def on_message(msg):
            messages.append(str(msg))

        wrapper = CommandSenderWrapper(console_sender, on_message=on_message)
        wrapper.send_message("Test captured message")

        assert len(messages) == 1
        assert "Test captured message" in messages[0]

    def test_wrapper_captures_errors(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper captures errors via on_error callback."""
        errors = []

        def on_error(msg):
            errors.append(str(msg))

        wrapper = CommandSenderWrapper(console_sender, on_error=on_error)
        wrapper.send_error_message("Test captured error")

        assert len(errors) == 1
        assert "Test captured error" in errors[0]

    def test_wrapper_without_callbacks(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper works without callbacks (no crash)."""
        wrapper = CommandSenderWrapper(console_sender)
        # Should not raise
        wrapper.send_message("Message without callback")
        wrapper.send_error_message("Error without callback")

    def test_wrapper_delegates_name(self, console_sender: ConsoleCommandSender) -> None:
        """Test that wrapper delegates name property to wrapped sender."""
        wrapper = CommandSenderWrapper(console_sender)
        assert wrapper.name == console_sender.name

    def test_wrapper_delegates_server(
        self, server: Server, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper delegates server property to wrapped sender."""
        wrapper = CommandSenderWrapper(console_sender)
        assert wrapper.server is server

    def test_wrapper_delegates_permission_level(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper delegates permission_level to wrapped sender."""
        wrapper = CommandSenderWrapper(console_sender)
        assert wrapper.permission_level == console_sender.permission_level

    def test_wrapper_delegates_has_permission(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper delegates has_permission to wrapped sender."""
        wrapper = CommandSenderWrapper(console_sender)
        perm = "endstone.command.test"
        assert wrapper.has_permission(perm) == console_sender.has_permission(perm)

    def test_wrapper_is_command_sender(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that wrapper is a CommandSender instance."""
        wrapper = CommandSenderWrapper(console_sender)
        assert isinstance(wrapper, CommandSender)


# =============================================================================
# Section 9: CommandExecutor Tests
# =============================================================================


class TestExecutor(CommandExecutor):
    """Test executor that records invocations."""

    __test__ = False

    def __init__(self):
        super().__init__()
        self.invocations = []

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        self.invocations.append(
            {
                "command_name": command.name,
                "args": list(args),
                "sender_name": sender.name,
            }
        )
        return True


class TestCommandExecutor:
    """Tests for CommandExecutor functionality."""

    def test_base_executor_returns_false(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that base CommandExecutor.on_command returns False."""
        executor = CommandExecutor()
        cmd = Command("testcmd")
        result = executor.on_command(console_sender, cmd, [])
        assert result is False

    def test_custom_executor_is_called(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that custom executor's on_command is called."""
        executor = TestExecutor()
        cmd = Command("testcmd")
        result = executor.on_command(console_sender, cmd, ["arg1", "arg2"])
        assert result is True
        assert len(executor.invocations) == 1

    def test_executor_receives_correct_command(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that executor receives the correct command name."""
        executor = TestExecutor()
        cmd = Command("mycommand")
        executor.on_command(console_sender, cmd, [])
        assert executor.invocations[0]["command_name"] == "mycommand"

    def test_executor_receives_correct_args(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that executor receives the correct arguments."""
        executor = TestExecutor()
        cmd = Command("testcmd")
        args = ["first", "second", "third"]
        executor.on_command(console_sender, cmd, args)
        assert executor.invocations[0]["args"] == args

    def test_executor_receives_correct_sender(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that executor receives the correct sender."""
        executor = TestExecutor()
        cmd = Command("testcmd")
        executor.on_command(console_sender, cmd, [])
        assert executor.invocations[0]["sender_name"] == "Server"


class FailingExecutor(CommandExecutor):
    """Executor that always returns False."""

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        return False


class TestCommandExecutorReturnValue:
    """Tests for CommandExecutor return value propagation."""

    def test_executor_true_return(self, console_sender: ConsoleCommandSender) -> None:
        """Test that True return value propagates."""
        executor = TestExecutor()
        cmd = Command("testcmd")
        result = executor.on_command(console_sender, cmd, [])
        assert result is True

    def test_executor_false_return(self, console_sender: ConsoleCommandSender) -> None:
        """Test that False return value propagates."""
        executor = FailingExecutor()
        cmd = Command("testcmd")
        result = executor.on_command(console_sender, cmd, [])
        assert result is False


# =============================================================================
# Section 10: PluginCommand Tests (via server.get_plugin_command)
# =============================================================================


class TestPluginCommand:
    """Tests for PluginCommand functionality via server.get_plugin_command."""

    def test_get_plugin_command_existing(self, server: Server) -> None:
        """Test getting an existing plugin command."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None

    def test_get_plugin_command_nonexistent(self, server: Server) -> None:
        """Test getting a non-existent plugin command returns None."""
        cmd = server.get_plugin_command("nonexistent_command_xyz")
        assert cmd is None

    def test_plugin_command_name(self, server: Server) -> None:
        """Test plugin command has correct name."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert cmd.name == "test"

    def test_plugin_command_description(self, server: Server) -> None:
        """Test plugin command has a description."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert isinstance(cmd.description, str)
        assert cmd.description == "Run the tests"

    def test_plugin_command_usages(self, server: Server) -> None:
        """Test plugin command has usages."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert isinstance(cmd.usages, list)
        assert len(cmd.usages) > 0

    def test_plugin_command_permissions(self, server: Server) -> None:
        """Test plugin command has permissions."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert isinstance(cmd.permissions, list)
        assert len(cmd.permissions) > 0
        assert "endstone_test.command.test" in cmd.permissions

    def test_plugin_command_is_registered(self, server: Server) -> None:
        """Test plugin command is registered."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert cmd.is_registered is True

    def test_plugin_command_aliases(self, server: Server) -> None:
        """Test plugin command aliases property."""
        cmd = server.get_plugin_command("test")
        assert cmd is not None
        assert isinstance(cmd.aliases, list)


# =============================================================================
# Section 11: Integration Tests
# =============================================================================


class TestCommandIntegration:
    """Integration tests for command system."""

    def test_server_dispatch_command(self, server: Server) -> None:
        """Test server.dispatch_command works correctly."""
        sender = server.command_sender
        result = server.dispatch_command(
            sender, "scriptevent endstone:integration_test test"
        )
        assert result is True

    def test_dispatch_with_wrapper(self, server: Server) -> None:
        """Test dispatch_command with CommandSenderWrapper captures output."""
        messages = []

        def on_message(msg):
            messages.append(str(msg))

        wrapper = CommandSenderWrapper(server.command_sender, on_message=on_message)
        server.dispatch_command(wrapper, "list")
        # The list command should produce output
        # Note: we just verify no error occurs

    def test_dispatch_help_command(self, server: Server) -> None:
        """Test dispatching the help command."""
        result = server.dispatch_command(server.command_sender, "help test")
        assert result is True

    def test_command_sender_from_server(self, server: Server) -> None:
        """Test that server.command_sender can be used for dispatch."""
        sender = server.command_sender
        assert sender is not None
        result = server.dispatch_command(sender, "time query daytime")
        assert result is True


# =============================================================================
# Section 12: Edge Cases Tests
# =============================================================================


class TestCommandEdgeCases:
    """Tests for edge cases and boundary conditions."""

    def test_empty_name_command(self) -> None:
        """Test creating a command with empty name."""
        # Empty string should be allowed (though not useful)
        cmd = Command("")
        assert cmd.name == ""

    def test_special_characters_in_description(self) -> None:
        """Test command with special characters in description."""
        desc = "Test <arg> & 'quotes' \"double\" :emoji:"
        cmd = Command("test", description=desc)
        assert cmd.description == desc

    def test_many_aliases(self) -> None:
        """Test command with many aliases."""
        aliases = [f"alias{i}" for i in range(20)]
        cmd = Command("test", aliases=aliases)
        assert len(cmd.aliases) == 20

    def test_duplicate_aliases(self) -> None:
        """Test command with duplicate aliases."""
        cmd = Command("test", aliases=["dup", "dup", "other"])
        # Behavior may vary - just ensure no crash
        assert isinstance(cmd.aliases, list)

    def test_empty_permissions_list(self) -> None:
        """Test command with empty permissions list."""
        cmd = Command("test", permissions=[])
        assert cmd.permissions == []

    def test_empty_command_line_dispatch(self, server: Server) -> None:
        """Test dispatching an empty command line."""
        result = server.dispatch_command(server.command_sender, "")
        # Empty command should return False
        assert result is False

    def test_whitespace_command_line_dispatch(self, server: Server) -> None:
        """Test dispatching a whitespace-only command line."""
        result = server.dispatch_command(server.command_sender, "   ")
        # Whitespace-only should return False
        assert result is False

    def test_command_with_newlines_in_description(self) -> None:
        """Test command with newlines in description."""
        desc = "Line 1\nLine 2\nLine 3"
        cmd = Command("test", description=desc)
        assert "\n" in cmd.description

    def test_very_long_command_name(self) -> None:
        """Test command with a very long name."""
        long_name = "a" * 100
        cmd = Command(long_name)
        assert cmd.name == long_name


# =============================================================================
# Section 13: Wrapper Message Capture Tests
# =============================================================================


class TestWrapperMessageCapture:
    """Additional tests for CommandSenderWrapper message capture."""

    def test_multiple_messages_captured(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that multiple messages are all captured."""
        messages = []

        def on_message(msg):
            messages.append(str(msg))

        wrapper = CommandSenderWrapper(console_sender, on_message=on_message)
        wrapper.send_message("Message 1")
        wrapper.send_message("Message 2")
        wrapper.send_message("Message 3")

        assert len(messages) == 3

    def test_both_callbacks_work(self, console_sender: ConsoleCommandSender) -> None:
        """Test that both message and error callbacks work together."""
        messages = []
        errors = []

        def on_message(msg):
            messages.append(str(msg))

        def on_error(msg):
            errors.append(str(msg))

        wrapper = CommandSenderWrapper(
            console_sender, on_message=on_message, on_error=on_error
        )
        wrapper.send_message("Normal message")
        wrapper.send_error_message("Error message")

        assert len(messages) == 1
        assert len(errors) == 1
        assert "Normal message" in messages[0]
        assert "Error message" in errors[0]

    def test_only_message_callback(self, console_sender: ConsoleCommandSender) -> None:
        """Test wrapper with only message callback."""
        messages = []

        def on_message(msg):
            messages.append(str(msg))

        wrapper = CommandSenderWrapper(console_sender, on_message=on_message)
        wrapper.send_message("Test")
        wrapper.send_error_message("Error")  # Should not crash

        assert len(messages) == 1

    def test_only_error_callback(self, console_sender: ConsoleCommandSender) -> None:
        """Test wrapper with only error callback."""
        errors = []

        def on_error(msg):
            errors.append(str(msg))

        wrapper = CommandSenderWrapper(console_sender, on_error=on_error)
        wrapper.send_message("Test")  # Should not crash
        wrapper.send_error_message("Error")

        assert len(errors) == 1


# =============================================================================
# Section 14: Permission Attachment Tests
# =============================================================================


class TestPermissionAttachment:
    """Tests for permission attachment on command senders."""

    def test_add_attachment(
        self, plugin: Plugin, console_sender: ConsoleCommandSender
    ) -> None:
        """Test adding a permission attachment to console sender."""
        attachment = console_sender.add_attachment(plugin)
        assert attachment is not None
        assert attachment.plugin is plugin
        # Clean up
        console_sender.remove_attachment(attachment)

    def test_add_attachment_with_permission(
        self, plugin: Plugin, console_sender: ConsoleCommandSender
    ) -> None:
        """Test adding a permission attachment with specific permission."""
        attachment = console_sender.add_attachment(
            plugin, "test.custom.permission", True
        )
        assert attachment is not None
        # Verify permission is set
        assert "test.custom.permission" in attachment.permissions
        # Clean up
        console_sender.remove_attachment(attachment)

    def test_remove_attachment(
        self, plugin: Plugin, console_sender: ConsoleCommandSender
    ) -> None:
        """Test removing a permission attachment."""
        attachment = console_sender.add_attachment(plugin)
        result = console_sender.remove_attachment(attachment)
        assert result is True


# =============================================================================
# Section 15: Command Subclass Tests
# =============================================================================


class CustomCommand(Command):
    """Custom command subclass for testing."""

    def __init__(self, name: str):
        super().__init__(name, description="A custom command")
        self.execution_count = 0

    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        self.execution_count += 1
        return True


class TestCommandSubclass:
    """Tests for Command subclasses."""

    def test_subclass_execute_called(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that subclass execute() is called."""
        cmd = CustomCommand("custom")
        result = cmd.execute(console_sender, [])
        assert result is True
        assert cmd.execution_count == 1

    def test_subclass_inherits_properties(self) -> None:
        """Test that subclass inherits property behavior."""
        cmd = CustomCommand("custom")
        assert cmd.name == "custom"
        assert cmd.description == "A custom command"
        assert cmd.is_registered is False

    def test_subclass_multiple_executions(
        self, console_sender: ConsoleCommandSender
    ) -> None:
        """Test that subclass tracks multiple executions."""
        cmd = CustomCommand("custom")
        cmd.execute(console_sender, [])
        cmd.execute(console_sender, [])
        cmd.execute(console_sender, [])
        assert cmd.execution_count == 3
