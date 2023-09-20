// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include "endstone/command/command_executor.h"
#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/plugin_command.h"
#include "fmt/color.h"
#include "pybind/pybind.h"

class PyCommandExecutor : public CommandExecutor {
    using CommandExecutor::CommandExecutor;

public:
    bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) noexcept override
    {
        try {
            PYBIND11_OVERRIDE_PURE_NAME(bool, CommandExecutor, "on_command", onCommand, std::ref(sender),
                                        std::ref(command), std::ref(label), std::ref(args));
        }
        catch (std::exception &e) {
            fmt::print(fg(fmt::color::red), "Unhandled exception executing command '{}'.\n", label);
            fmt::print(fg(fmt::color::red), "{}\n", e.what());
            return true;
        }
    }
};

void def_command(py::module &m)
{
    py::class_<Command, py::smart_holder>(m, "Command")
        .def(py::init<const std::string &>(), py::arg("name"))
        .def(py::init<const std::string &, const std::string &, const std::vector<std::string> &,
                      const std::vector<std::string> &>(),
             py::arg("name"), py::arg("description"), py::arg("usages"), py::arg("aliases"))
        //        .def("execute", &Command::execute)
        .def_property("name", &Command::getName, &Command::setName)
        .def_property("label", &Command::getLabel, &Command::setLabel)
        .def_property("aliases", &Command::getAliases, &Command::setAliases)
        .def_property("description", &Command::getDescription, &Command::setDescription)
        .def_property("usages", &Command::getUsages, &Command::setUsages)
        .def_property("permission", &Command::getPermission, &Command::setPermission)
        .def_property_readonly("registered", &Command::isRegistered)
        .def("register", &Command::registerTo, py::arg("command_map"))
        .def("unregister", &Command::unregisterFrom, py::arg("command_map"));

    py::class_<PluginCommand, Command, py::smart_holder>(m, "PluginCommand")
        .def_property("executor", &PluginCommand::getExecutor, &PluginCommand::setExecutor);

    py::class_<CommandSender>(m, "CommandSender")
        .def("send_message",
             static_cast<void (CommandSender::*)(const std::string &) const>(&CommandSender::sendMessage),
             py::arg("msg"))
        .def_property_readonly("server", &CommandSender::getServer)
        .def_property_readonly("name", &CommandSender::getName);

    py::class_<CommandExecutor, PyCommandExecutor, py::smart_holder>(m, "CommandExecutorBase")
        .def(py::init<>())
        .def("on_command", &CommandExecutor::onCommand, py::arg("sender"), py::arg("command"), py::arg("label"),
             py::arg("args"));
}
