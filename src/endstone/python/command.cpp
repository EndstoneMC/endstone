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

    bool onCommand(CommandSender &sender, const Command &command, const std::vector<std::string> &args) override
    {
        PYBIND11_OVERRIDE_NAME(bool, CommandExecutor, "on_command", onCommand, std::ref(sender), std::ref(command),
                               std::ref(args));
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
}  // namespace

void init_command(py::module &m, py_class<CommandSender> &command_sender)
{
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
        .def_property_readonly("block", &BlockCommandSender::getBlock,
                               "The block this command sender belongs to.");

    py_class<CommandSenderWrapper>(
        m, "CommandSenderWrapper",
        "Represents a wrapper that forwards commands to the wrapped CommandSender and captures its output.")
        .def(py::init<CommandSender &, CommandSenderWrapper::Callback, CommandSenderWrapper::Callback>(),
             py::arg("sender"), py::arg("on_message") = CommandSenderWrapper::Callback{},
             py::arg("on_error") = CommandSenderWrapper::Callback{});

    py_class<ConsoleCommandSender>(m, "ConsoleCommandSender", "Represents a console command sender.");

    py::class_<Command, std::shared_ptr<Command>>(m, "Command",
                                                  "Represents a Command, which executes various tasks upon user input.")
        .def(py::init(&createCommand), py::arg("name"), py::arg("description") = py::none(),
             py::arg("usages") = py::none(), py::arg("aliases") = py::none(), py::arg("permissions") = py::none())
        .def("execute", &Command::execute, py::arg("sender"), py::arg("args"), R"doc(
    Executes the command, returning its success.

    Args:
        sender: Source of the command.
        args: Arguments passed to the command.

    Returns:
        True if the execution was successful, False otherwise.
)doc")
        .def("test_permission", &Command::testPermission, py::arg("target"), R"doc(
    Tests the given CommandSender to see if they can perform this command.

    If they do not have permission, they will be informed that they cannot do this.

    Args:
        target: User to test.

    Returns:
        True if they can use it, False otherwise.
)doc")
        .def("test_permission_silently", &Command::testPermissionSilently, py::arg("target"), R"doc(
    Tests the given CommandSender to see if they can perform this command.

    No error is sent to the sender.

    Args:
        target: User to test.

    Returns:
        True if they can use it, False otherwise.
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
        True if the execution is successful, False otherwise.
)doc");
}

}  // namespace endstone::python
