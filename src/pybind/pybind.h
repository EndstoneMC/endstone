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

#pragma once

#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/smart_holder.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Command)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class CommandExecutor)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Permission)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class PluginCommand)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class Plugin)
PYBIND11_SMART_HOLDER_TYPE_CASTERS(class PluginLoader)

void def_chat_color(py::module &m);
void def_command(py::module &m);
void def_logger(py::module &m);
void def_permission(py::module &m);
void def_plugin(py::module &m);
void def_server(py::module &m);
