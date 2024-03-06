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

#include "endstone/command/command_sender.h"
#include "endstone/detail/python.h"
#include "endstone/logger.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::detail {

void init_command(py::module &m)
{
    py_class<Server>(m, "Server");

    py_class<CommandSender>(m, "CommandSender")
        .def(
            "send_message",
            [](const CommandSender &sender, const std::string &message) { sender.sendMessage(message); },
            py::arg("message"), "Sends this sender a message")
        .def_property_readonly("server", &CommandSender::getServer, py::return_value_policy::reference,
                               "Returns the server instance that this command is running on")
        .def_property_readonly("name", &CommandSender::getName, "Gets the name of this command sender");
}

}  // namespace endstone::detail
