//
// Created by Vincent on 29/08/2023.
//

#include "endstone/command/command.h"
#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"
#include "endstone/common.h"
#include "endstone/server.h"
#include "pybind.h"

class PyCommandExecutor : public CommandExecutor {
    using CommandExecutor::CommandExecutor;

public:
    bool onCommand(CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) const noexcept override
    {
        PYBIND11_OVERRIDE_PURE_NAME(bool, CommandExecutor, "on_command", onCommand);
    }
};

PYBIND11_MODULE(_command, m)
{
    py::class_<Command, std::shared_ptr<Command>>(m, "Command")
        .def("execute", &Command::execute)
        .def_property("name", &Command::getName, &Command::setName)
        .def_property("label", &Command::getLabel, &Command::setLabel)
        .def_property("aliases", &Command::getAliases, &Command::setAliases)
        .def_property("description", &Command::getDescription, &Command::setDescription)
        .def_property("usages", &Command::getUsages, &Command::setUsages)
        .def_property_readonly("registered", &Command::isRegistered)
        .def("register", &Command::registerTo, py::arg("command_map"))
        .def("unregister", &Command::unregisterFrom, py::arg("command_map"))
        .def("__repr__", [](const Command &c) {
            return "<Command name='" + c.getName() + "' label='" + c.getLabel() + "'>";
        });

    py::class_<CommandSender, std::shared_ptr<CommandSender>>(m, "CommandSender")
        .def("send_message", (void(CommandSender::*)(const std::string &) const) & CommandSender::sendMessage)
        .def_property_readonly("server", &CommandSender::getServer)
        .def_property_readonly("name", &CommandSender::getName)
        .def("__repr__", [](const CommandSender &cs) {
            return "<CommandSender name='" + std::string(cs.getName()) + "'>";
        });

    py::class_<CommandExecutor, PyCommandExecutor, std::shared_ptr<CommandExecutor>>(m, "CommandExecutor")
        .def(py::init<>())
        .def("on_command", &CommandExecutor::onCommand, py::arg("sender"), py::arg("command"), py::arg("label"),
             py::arg("args"));
}
