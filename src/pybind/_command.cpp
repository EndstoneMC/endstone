//
// Created by Vincent on 29/08/2023.
//

#include "endstone/command/command.h"
#include "endstone/command/command_executor.h"
#include "endstone/command/command_map.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/plugin_command.h"
#include "endstone/common.h"
#include "pybind.h"

class PyCommandExecutor : public CommandExecutor {
    using CommandExecutor::CommandExecutor;

public:
    bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) noexcept override
    {
        PYBIND11_OVERRIDE_PURE_NAME(bool, CommandExecutor, "on_command", onCommand, std::ref(sender), std::ref(command),
                                    std::ref(label), std::ref(args));
    }
};

void def_command_api(py::module &m)
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
        .def_property_readonly("registered", &Command::isRegistered)
        .def("register", &Command::registerTo, py::arg("command_map"))
        .def("unregister", &Command::unregisterFrom, py::arg("command_map"));

    py::class_<PluginCommand, Command, py::smart_holder>(m, "PluginCommand")
        .def_property("executor", &PluginCommand::getExecutor, &PluginCommand::setExecutor);

    py::class_<CommandSender>(m, "CommandSender")
        .def("send_message", (void(CommandSender::*)(const std::string &) const) & CommandSender::sendMessage,
             py::arg("msg"))
        .def_property_readonly("server", &CommandSender::getServer)
        .def_property_readonly("name", &CommandSender::getName);

    py::class_<CommandExecutor, PyCommandExecutor, py::smart_holder>(m, "CommandExecutor")
        .def(py::init<>())
        .def("on_command", &CommandExecutor::onCommand, py::arg("sender"), py::arg("command"), py::arg("label"),
             py::arg("args"));
}
