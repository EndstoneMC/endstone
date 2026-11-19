// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone_python.h"

namespace endstone::python {

class PyCommandExecutor : public CommandExecutor, public py::trampoline_self_life_support {
public:
    using CommandExecutor::CommandExecutor;

    bool onCommand(const NotNull<CommandSender> &sender, const Command &command,
                   const std::vector<std::string> &args) override
    {
        PYBIND11_OVERRIDE_NAME(bool, CommandExecutor, "on_command", onCommand, sender, std::ref(command),
                               std::ref(args));
    }
};

class PyCommand : public Command, public py::trampoline_self_life_support {
public:
    using Command::Command;

    bool execute(const NotNull<CommandSender> &sender, const std::vector<std::string> &args) const override
    {
        PYBIND11_OVERRIDE(bool, Command, execute, sender, std::ref(args));
    }
};

class PyBasicCommand : public BasicCommand, public py::trampoline_self_life_support {
public:
    using BasicCommand::BasicCommand;

    void execute(const CommandSource &source, const std::vector<std::string> &args) override
    {
        try {
            PYBIND11_OVERRIDE_PURE(void, BasicCommand, execute, source, args);
        }
        catch (py::error_already_set &e) {
            py::gil_scoped_acquire gil;
            if (!e.matches(py::module_::import("endstone._python").attr("brigadier").attr("CommandSyntaxException"))) {
                throw;
            }
            static const brigadier::SimpleCommandExceptionType type{""};
            throw brigadier::CommandSyntaxException(type, py::str(e.value()).cast<std::string>());
        }
    }

    [[nodiscard]] bool canUse(const NotNull<CommandSender> &sender) const override
    {
        PYBIND11_OVERRIDE_NAME(bool, BasicCommand, "can_use", canUse, sender);
    }

    [[nodiscard]] std::optional<std::string> permission() const override
    {
        PYBIND11_OVERRIDE(std::optional<std::string>, BasicCommand, permission);
    }
};

namespace {
Command createCommand(const std::string &name, const std::optional<std::string> &description,
                      const std::optional<std::vector<std::string>> &usages,
                      const std::optional<std::vector<std::string>> &aliases,
                      const std::optional<std::vector<std::string>> &permissions, const py::args & /*args*/,
                      const py::kwargs & /*kwargs*/)
{
    return Command(std::move(name), description.value_or(""), usages.value_or(std::vector<std::string>{}),
                   aliases.value_or(std::vector<std::string>{}), permissions.value_or(std::vector<std::string>{}));
}

PyCommand createCommandAlias(const std::string &name, const std::optional<std::string> &description,
                             const std::optional<std::vector<std::string>> &usages,
                             const std::optional<std::vector<std::string>> &aliases,
                             const std::optional<std::vector<std::string>> &permissions, const py::args & /*args*/,
                             const py::kwargs & /*kwargs*/)
{
    return PyCommand(std::move(name), description.value_or(""), usages.value_or(std::vector<std::string>{}),
                     aliases.value_or(std::vector<std::string>{}), permissions.value_or(std::vector<std::string>{}));
}
}  // namespace

void init_brigadier(py::module_ &m);

void init_command(py::module &m, py::module_ &m_brigadier, py_class<CommandSender> &command_sender)
{
    py::classh<CommandSourceStack>(m, "CommandSourceStack",
                                   "The source of a command run through a Brigadier command tree, carrying who ran "
                                   "it and where.")
        .def_property_readonly("sender", &CommandSourceStack::getSender,
                               "The command sender that triggered the command.")
        .def_property_readonly(
            "executor", &CommandSourceStack::getExecutor,
            "The actor that executes the command, which differs from the sender after `/execute as`, or `None`.")
        .def_property_readonly("location", &CommandSourceStack::getLocation,
                               "A copy of the location where the command is executed.");

    py::class_<PlayerSelectorArgumentResolver>(m, "PlayerSelectorArgumentResolver",
                                               "Resolves a player selector argument to the players it selects.")
        .def("resolve", &PlayerSelectorArgumentResolver::resolve, py::arg("source"), R"doc(
    Resolves the argument with the given command source stack.

    Args:
        source: The source of the command.

    Returns:
        The players the selector selects.

    Raises:
        CommandSyntaxException: If the argument takes one player and the selector selects none or more than one.
        ValueError: If the source was not created by the server.
)doc");

    py::class_<ActorSelectorArgumentResolver>(m, "ActorSelectorArgumentResolver",
                                              "Resolves an actor selector argument to the actors it selects.")
        .def("resolve", &ActorSelectorArgumentResolver::resolve, py::arg("source"), R"doc(
    Resolves the argument with the given command source stack.

    Args:
        source: The source of the command.

    Returns:
        The actors the selector selects.

    Raises:
        CommandSyntaxException: If the argument takes one actor and the selector selects none or more than one.
        ValueError: If the source was not created by the server.
)doc");

    py::class_<BlockPositionResolver>(
        m, "BlockPositionResolver",
        "Resolves a block position argument, with relative and local coordinates taken from the source of the command.")
        .def("resolve", &BlockPositionResolver::resolve, py::arg("source"), R"doc(
    Resolves the argument with the given command source stack.

    Args:
        source: The source of the command.

    Returns:
        The block position.

    Raises:
        ValueError: If the source was not created by the server.
)doc");

    py::class_<FinePositionResolver>(
        m, "FinePositionResolver",
        "Resolves a fine position argument, with relative and local coordinates taken from the source of the command.")
        .def("resolve", &FinePositionResolver::resolve, py::arg("source"), R"doc(
    Resolves the argument with the given command source stack.

    Args:
        source: The source of the command.

    Returns:
        The position.

    Raises:
        ValueError: If the source was not created by the server.
)doc");

    init_brigadier(m_brigadier);

    bind_set<std::unordered_set<NotNull<CommandSender>>>(
        m, "CommandSenderSet", "A mutable set of command senders, backed by the event that owns it.");

    command_sender
        .def(
            "send_message", [](const CommandSender &self, const Message &message) { self.sendMessage(message); },
            py::arg("message"), R"doc(
    Sends this sender a message.

    Args:
        message: Message to be displayed.
)doc")
        .def(
            "send_error_message",
            [](const CommandSender &self, const Message &message) { self.sendErrorMessage(message); },
            py::arg("message"), R"doc(
    Sends this sender a error message.

    Args:
        message: Error message to be displayed.
)doc")
        .def_property_readonly("server", &CommandSender::getServer, py::return_value_policy::reference,
                               "The server instance that this command is running on.")
        .def_property_readonly("name", &CommandSender::getName, "The name of this command sender.");

    py_class<BlockCommandSender>(m, "BlockCommandSender", "Represents a block command sender.")
        .def_property_readonly("block", &BlockCommandSender::getBlock, "The block this command sender belongs to.");

    py_class<CommandSenderWrapper>(
        m, "CommandSenderWrapper",
        "Represents a wrapper that forwards commands to the wrapped `CommandSender` and captures its output.")
        .def(py::init<const NotNull<CommandSender> &, CommandSenderWrapper::Callback, CommandSenderWrapper::Callback>(),
             py::arg("sender"), py::arg("on_message") = CommandSenderWrapper::Callback{},
             py::arg("on_error") = CommandSenderWrapper::Callback{});

    py_class<ConsoleCommandSender>(m, "ConsoleCommandSender", "Represents a console command sender.");

    py_class<ProxiedCommandSender>(m, "ProxiedCommandSender", R"doc(
    Represents a command sender that is running a command on behalf of another one, as `/execute` does.

    Output and permissions belong to the caller, while the name and the execution context belong to the callee.
)doc")
        .def_property_readonly("caller", &ProxiedCommandSender::getCaller,
                               "The `CommandSender` which triggered this proxied command.")
        .def_property_readonly("callee", &ProxiedCommandSender::getCallee,
                               "The `CommandSender` which is being used to call the command.");

    auto command =
        py::class_<Command, PyCommand, py::smart_holder>(
            m, "Command", "Represents a Command, which executes various tasks upon user input.")
            .def(py::init(&createCommand, &createCommandAlias), py::arg("name"), py::arg("description") = py::none(),
                 py::arg("usages") = py::none(), py::arg("aliases") = py::none(), py::arg("permissions") = py::none())
            .def("execute", &Command::execute, py::arg("sender"), py::arg("args"), R"doc(
    Executes the command, returning its success.

    Args:
        sender: Source of the command.
        args: Arguments passed to the command.

    Returns:
        `True` if the execution was successful, `False` otherwise.
)doc")
            .def("test_permission", &Command::testPermission, py::arg("target"), R"doc(
    Tests the given `CommandSender` to see if they can perform this command.

    If they do not have permission, they will be informed that they cannot do this.

    Args:
        target: User to test.

    Returns:
        `True` if they can use it, `False` otherwise.
)doc")
            .def("test_permission_silently", &Command::testPermissionSilently, py::arg("target"), R"doc(
    Tests the given `CommandSender` to see if they can perform this command.

    No error is sent to the sender.

    Args:
        target: User to test.

    Returns:
        `True` if they can use it, `False` otherwise.
)doc")
            .def_property("name", &Command::getName, &Command::setName, "The name of this command.")
            .def_property("description", &Command::getDescription, &Command::setDescription,
                          "A brief description of this command.")
            .def_property(
                "aliases", &Command::getAliases,
                [](Command &self, const std::vector<std::string> &aliases) { self.setAliases(aliases); },
                "A list of aliases of this command.")
            .def_property(
                "usages", &Command::getUsages,
                [](Command &self, const std::vector<std::string> &usages) { self.setUsages(usages); },
                "A list of usages of this command.")
            .def_property(
                "permissions", &Command::getPermissions,
                [](Command &self, const std::vector<std::string> &permissions) { self.setPermissions(permissions); },
                "The permissions required by users to be able to perform this command.")
            .def_property_readonly("is_registered", &Command::isRegistered,
                                   "The current registered state of this command.");

    py::classh<SoftEnum>(m, "SoftEnum",
                         "Represents a soft enum, a set of command completions that can change while the server runs.")
        .def_property_readonly("name", &SoftEnum::getName, "The identifier the client sees, unique across the server.")
        .def_property_readonly("values", &SoftEnum::getValues, "The completions this soft enum offers.")
        .def("set_values", &SoftEnum::setValues, py::arg("values"), R"doc(
    Replaces the values of this soft enum and updates every client's completion at once.

    Args:
        values: The new values.

    Raises:
        ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
        RuntimeError: If the plugin that owned this soft enum has removed it.
)doc")
        .def("add_values", &SoftEnum::addValues, py::arg("values"), R"doc(
    Adds values to this soft enum and updates every client's completion at once.

    Args:
        values: The values to add.

    Raises:
        ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
        RuntimeError: If the plugin that owned this soft enum has removed it.
)doc")
        .def("remove_values", &SoftEnum::removeValues, py::arg("values"), R"doc(
    Removes values from this soft enum and updates every client's completion at once.

    Args:
        values: The values to remove.

    Raises:
        ValueError: If this soft enum is read-only or a value is empty or holds whitespace.
        RuntimeError: If the plugin that owned this soft enum has removed it.
)doc");

    py::classh<BasicCommand, PyBasicCommand>(m, "BasicCommand",
                                             "A command that takes its arguments as words, registered through "
                                             "`Commands.register_command`.")
        .def(py::init<>())
        .def("execute", &BasicCommand::execute, py::arg("source"), py::arg("args"), R"doc(
    Executes the command with the given source and arguments.

    A `CommandSyntaxException` raised here is sent to the sender as an error.

    Args:
        source: The source of the command.
        args: The words typed after the label.
)doc")
        .def("can_use", &BasicCommand::canUse, py::arg("sender"), R"doc(
    Checks whether a command sender can receive and run the root command.

    Args:
        sender: The command sender.

    Returns:
        `True` if the sender has `permission()` or no permission is set.
)doc")
        .def("permission", &BasicCommand::permission, R"doc(
    Returns the permission for the root command used in `can_use` by default.

    Returns:
        The permission, or `None` if none is required.
)doc");

    py::classh<SoftEnumArgumentType, brigadier::ArgumentType>(
        m, "SoftEnumArgumentType", "An argument completed from a soft enum, whose value is the word typed.")
        .def_property_readonly("name", &SoftEnumArgumentType::getName,
                               "The name of the soft enum this argument completes from.");

    py::classh<PlayerArgumentType, brigadier::ArgumentType>(
        m, "PlayerArgumentType",
        "A player selector or a player name, resolved through a `PlayerSelectorArgumentResolver`.");

    py::classh<ActorArgumentType, brigadier::ArgumentType>(
        m, "ActorArgumentType",
        "An actor selector or a player name, resolved through an `ActorSelectorArgumentResolver`.");

    py::classh<BlockPositionArgumentType, brigadier::ArgumentType>(
        m, "BlockPositionArgumentType",
        "A block position of three integer, relative or local coordinates, resolved through a "
        "`BlockPositionResolver`.");

    py::classh<FinePositionArgumentType, brigadier::ArgumentType>(
        m, "FinePositionArgumentType",
        "A position of three decimal, relative or local coordinates, resolved through a `FinePositionResolver`.");

    py::classh<DimensionArgumentType, brigadier::ArgumentType>(
        m, "DimensionArgumentType",
        "A dimension of the level, completed from the server's dimension names, including those plugins create.");

    py::classh<ResourceArgumentType, brigadier::ArgumentType>(
        m, "ResourceArgumentType", "An entry of a registry, completed from the server's names for it.");

    py::class_<ArgumentTypes>(m, "ArgumentTypes", "The argument types Endstone adds to Brigadier's own.")
        .def_static("player", &ArgumentTypes::player, R"doc(
    A selector or player name for exactly one player, resolved through a `PlayerSelectorArgumentResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("players", &ArgumentTypes::players, R"doc(
    A selector or player name for any number of players, resolved through a `PlayerSelectorArgumentResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("actor", &ArgumentTypes::actor, R"doc(
    A selector or player name for exactly one actor, resolved through an `ActorSelectorArgumentResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("actors", &ArgumentTypes::actors, R"doc(
    A selector or player name for any number of actors, resolved through an `ActorSelectorArgumentResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("block_position", &ArgumentTypes::blockPosition, R"doc(
    A block position, resolved through a `BlockPositionResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("fine_position", &ArgumentTypes::finePosition, R"doc(
    A fine position, resolved through a `FinePositionResolver`.

    Returns:
        The argument type.
)doc")
        .def_static("dimension", &ArgumentTypes::dimension, R"doc(
    A dimension, whose value is a `Dimension`.

    Returns:
        The argument type.
)doc")
        .def_static(
            "resource",
            [](const py::type &type) -> std::shared_ptr<const ResourceArgumentType> {
                if (type.is(py::type::of<BlockType>())) {
                    return ArgumentTypes::resource<BlockType>();
                }
                if (type.is(py::type::of<ItemType>())) {
                    return ArgumentTypes::resource<ItemType>();
                }
                if (type.is(py::type::of<ActorType>())) {
                    return ArgumentTypes::resource<ActorType>();
                }
                throw std::invalid_argument("The registry type must be BlockType, ItemType or ActorType.");
            },
            py::arg("type"), R"doc(
    An entry of a registry, whose value is an instance of the registry type.

    Args:
        type: The registry type, which is `BlockType`, `ItemType` or `ActorType`.

    Returns:
        The argument type.

    Raises:
        ValueError: If the type is not `BlockType`, `ItemType` or `ActorType`.
)doc")
        .def_static("soft_enum", &ArgumentTypes::softEnum, py::arg("name"), R"doc(
    An argument completed from a soft enum, whose value is the word typed as a string.

    The server accepts any word, so the command checks the value itself. The name must be a registered soft enum.
    Soft enums are named for their content in CamelCase, as the server names its own.

    Args:
        name: The name of a soft enum a plugin, Endstone or the server owns.

    Returns:
        The argument type.
)doc");

    py::classh<Commands>(m, "Commands", "The registrar for a plugin's commands and soft enums.")
        .def_static("literal", &Commands::literal, py::arg("name"), R"doc(
    Creates a literal node builder, the keyword a command or a subcommand starts with.

    Args:
        name: The keyword.

    Returns:
        The builder.
)doc")
        .def_static("argument", &Commands::argument<const brigadier::ArgumentType>, py::arg("name"), py::arg("type"),
                    R"doc(
    Creates an argument node builder.

    Args:
        name: The name the command reads the argument back by.
        type: The type of the argument.

    Returns:
        The builder.
)doc");

    py::classh<CommandMap>(m, "CommandMap", "Represents a command map that manages all commands of the Server.")
        .def("register_command", static_cast<bool (CommandMap::*)(NotNull<Command>)>(&CommandMap::registerCommand),
             py::arg("command"), R"doc(
    Registers a command, using the lowercased plugin name as the fallback prefix for a `PluginCommand` and `endstone` otherwise.

    Args:
        command: The command to register.

    Returns:
        `True` if the command was registered under its own name, `False` if the name was already taken.
)doc")
        .def("register_command",
             static_cast<bool (CommandMap::*)(std::string, NotNull<Command>)>(&CommandMap::registerCommand),
             py::arg("fallback_prefix"), py::arg("command"), R"doc(
    Registers a command under its name and under `fallback_prefix:name`.

    If the name is already taken, the command is only reachable as `fallback_prefix:name` and its name changes to that. Its aliases are registered the same way, and an alias that is already taken is dropped. A `fallback_prefix:name` that another command already holds is taken over. Built-in Endstone commands use the fallback prefix `endstone`.

    Args:
        fallback_prefix: A prefix which is prepended to the command name for a unique address.
        command: The command to register.

    Returns:
        `True` if the command was registered under its own name, `False` if the name was already taken.
)doc")
        .def("dispatch", &CommandMap::dispatch, py::arg("sender"), py::arg("command_line"), R"doc(
    Looks for the requested command and executes it if found.

    Args:
        sender: The command's sender.
        command_line: The command and its arguments, e.g. "/test abc 123".

    Returns:
        `True` if the command was found and run, `False` if no such command exists.
)doc")
        .def("clear_commands", &CommandMap::clearCommands, "Clears all registered commands.")
        .def("get_command", &CommandMap::getCommand, py::arg("name"), R"doc(
    Gets the command registered to the specified name.

    Args:
        name: The name of the command to retrieve.

    Returns:
        The command with the specified name, `None` if a command with that label doesn't exist.
)doc")
        .def("get_soft_enum", &CommandMap::getSoftEnum, py::arg("name"), R"doc(
    Gets the soft enum registered to the specified name, whether a plugin, Endstone or the server owns it.

    The mutators of a soft enum the server or Endstone owns raise `ValueError`.

    Args:
        name: The name of the soft enum to retrieve.

    Returns:
        The soft enum with the specified name, `None` if no soft enum has that name.
)doc");

    command
        .def("register_to", &Command::registerTo, py::arg("command_map"), R"doc(
    Registers this command to a `CommandMap`.

    Args:
        command_map: The `CommandMap` to register to.

    Returns:
        `True` if the registration was successful, `False` otherwise.
)doc")
        .def("unregister_from", &Command::unregisterFrom, py::arg("command_map"), R"doc(
    Unregisters this command from a `CommandMap`.

    Args:
        command_map: The `CommandMap` to unregister from.

    Returns:
        `True` if the unregistration was successful, `False` otherwise.
)doc");

    py::class_<CommandExecutor, PyCommandExecutor, py::smart_holder>(
        m, "CommandExecutor", "Represents a class which contains a single method for executing commands.")
        .def(py::init<>())
        .def("on_command", &CommandExecutor::onCommand, py::arg("sender"), py::arg("command"), py::arg("args"), R"doc(
    Executes the given command, returning its success.

    Args:
        sender: Source of the command.
        command: Command which was executed.
        args: Passed command arguments.

    Returns:
        `True` if the execution is successful, `False` otherwise.
)doc");
}

}  // namespace endstone::python
