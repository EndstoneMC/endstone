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

#include "endstone/command/command.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/command/command_executor.h"
#include "endstone/command/command_sender.h"
#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::detail {

class PyCommandExecutor : public CommandExecutor {
public:
    using CommandExecutor::CommandExecutor;

    bool onCommand(const CommandSender &sender, const Command &command, const std::vector<std::string> &args) override
    {
        PYBIND11_OVERRIDE_NAME(bool, CommandExecutor, "on_command", onCommand, std::ref(sender), std::ref(command),
                               std::ref(args));
    }
};

void init_command(py::module &m)
{
    py::class_<CommandSender>(m, "CommandSender")
        .def(
            "send_message",
            [](const CommandSender &sender, const std::string &message) { sender.sendMessage(message); },
            py::arg("message"), "Sends this sender a message")
        .def_property_readonly("server", &CommandSender::getServer, py::return_value_policy::reference,
                               "Returns the server instance that this command is running on")
        .def_property_readonly("name", &CommandSender::getName, "Gets the name of this command sender");

    py::class_<Command, std::shared_ptr<Command>>(m, "Command")
        .def("execute", &Command::execute, py::arg("sender"), py::arg("args"),
             "Executes the command, returning its success")
        .def_property("name", &Command::getName, &Command::setName, "Name of this command.")
        .def_property("description", &Command::getDescription, &Command::setDescription,
                      "Brief description of this command")
        .def_property(
            "aliases", &Command::getAliases,
            [](Command &self, const std::vector<std::string> &aliases) { self.setAliases(aliases); },
            "List of aliases of this command")
        .def_property(
            "usages", &Command::getUsages,
            [](Command &self, const std::vector<std::string> &usages) { self.setUsages(usages); },
            "List of usages of this command")
        .def_property_readonly("registered", &Command::isRegistered,
                               "Returns the current registered state of this command");

    py::class_<CommandExecutor, PyCommandExecutor, std::shared_ptr<CommandExecutor>>(m, "CommandExecutor")
        .def(py::init<>())
        .def("on_command", &CommandExecutor::onCommand, py::arg("sender"), py::arg("command"), py::arg("args"),
             "Executes the given command, returning its success.");
}

}  // namespace endstone::detail
