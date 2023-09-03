//
// Created by Vincent on 21/08/2023.
//

#ifndef ENDSTONE_PYBIND_H
#define ENDSTONE_PYBIND_H

#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/smart_holder.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Command)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class PluginCommand)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Plugin)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class PluginLoader)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Logger)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class CommandExecutor)

void def_plugin(py::module &m);
void def_logger(py::module &m);
void def_command_api(py::module &m);
void def_chat_color(py::module &m);
void def_server(py::module &m);

#endif // ENDSTONE_PYBIND_H
