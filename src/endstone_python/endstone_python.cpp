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

#include <pybind11/pybind11.h>

#include "endstone/color_format.h"
#include "endstone/game_mode.h"
#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::detail {
void init_(py::module_ &);  // NOLINT(*-identifier-naming)
void init_util(py::module_ &);
void init_command(py::module_ &);
void init_logger(py::module_ &);
void init_plugin(py::module_ &);
void init_permissions(py::module_ &, py::class_<Permissible> &permissible, py::class_<Permission> &permission,
                      py::enum_<PermissionDefault> &permission_default);
void init_level(py::module_ &);
void init_server(py::class_<Server> &server);
void init_event(py::module_ &, py::class_<Event> &event, py::enum_<EventPriority> &event_priority);

PYBIND11_MODULE(endstone_python, m)  // NOLINT(*-use-anonymous-namespace)
{
    // Forward declaration, see:
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#avoiding-c-types-in-docstrings
    auto event = py::class_<Event>(m, "Event");
    auto event_priority = py::enum_<EventPriority>(
        m, "EventPriority",
        "Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR");
    auto server = py::class_<Server>(m, "Server");
    auto permissible = py::class_<Permissible>(m, "Permissible");
    auto permission = py::class_<Permission>(m, "Permission");
    auto permission_default = py::enum_<PermissionDefault>(m, "PermissionDefault");

    init_(m);
    init_util(m);
    init_command(m);
    init_logger(m);
    init_plugin(m);
    init_permissions(m, permissible, permission, permission_default);
    init_level(m);
    init_server(server);
    init_event(m, event, event_priority);
}

void init_(py::module_ &m)
{
    py::class_<ColorFormat>(m, "ColorFormat")
        .def_property_readonly_static("BLACK", [](const py::object &) { return ColorFormat::BLACK; })
        .def_property_readonly_static("DARK_BLUE", [](const py::object &) { return ColorFormat::DARK_BLUE; })
        .def_property_readonly_static("DARK_GREEN", [](const py::object &) { return ColorFormat::DARK_GREEN; })
        .def_property_readonly_static("DARK_AQUA", [](const py::object &) { return ColorFormat::DARK_AQUA; })
        .def_property_readonly_static("DARK_RED", [](const py::object &) { return ColorFormat::DARK_RED; })
        .def_property_readonly_static("DARK_PURPLE", [](const py::object &) { return ColorFormat::DARK_PURPLE; })
        .def_property_readonly_static("GOLD", [](const py::object &) { return ColorFormat::GOLD; })
        .def_property_readonly_static("GRAY", [](const py::object &) { return ColorFormat::GRAY; })
        .def_property_readonly_static("DARK_GRAY", [](const py::object &) { return ColorFormat::DARK_GRAY; })
        .def_property_readonly_static("BLUE", [](const py::object &) { return ColorFormat::BLUE; })
        .def_property_readonly_static("GREEN", [](const py::object &) { return ColorFormat::GREEN; })
        .def_property_readonly_static("AQUA", [](const py::object &) { return ColorFormat::AQUA; })
        .def_property_readonly_static("RED", [](const py::object &) { return ColorFormat::RED; })
        .def_property_readonly_static("LIGHT_PURPLE", [](const py::object &) { return ColorFormat::LIGHT_PURPLE; })
        .def_property_readonly_static("YELLOW", [](const py::object &) { return ColorFormat::YELLOW; })
        .def_property_readonly_static("WHITE", [](const py::object &) { return ColorFormat::WHITE; })
        .def_property_readonly_static("MINECOIN_GOLD", [](const py::object &) { return ColorFormat::MINECOIN_GOLD; })
        .def_property_readonly_static("MATERIAL_QUARTZ",
                                      [](const py::object &) { return ColorFormat::MATERIAL_QUARTZ; })
        .def_property_readonly_static("MATERIAL_IRON", [](const py::object &) { return ColorFormat::MATERIAL_IRON; })
        .def_property_readonly_static("MATERIAL_NETHERITE",
                                      [](const py::object &) { return ColorFormat::MATERIAL_NETHERITE; })
        .def_property_readonly_static("MATERIAL_REDSTONE",
                                      [](const py::object &) { return ColorFormat::MATERIAL_REDSTONE; })
        .def_property_readonly_static("MATERIAL_COPPER",
                                      [](const py::object &) { return ColorFormat::MATERIAL_COPPER; })
        .def_property_readonly_static("MATERIAL_GOLD", [](const py::object &) { return ColorFormat::MATERIAL_GOLD; })
        .def_property_readonly_static("MATERIAL_EMERALD",
                                      [](const py::object &) { return ColorFormat::MATERIAL_EMERALD; })
        .def_property_readonly_static("MATERIAL_DIAMOND",
                                      [](const py::object &) { return ColorFormat::MATERIAL_DIAMOND; })
        .def_property_readonly_static("MATERIAL_LAPIS", [](const py::object &) { return ColorFormat::MATERIAL_LAPIS; })
        .def_property_readonly_static("MATERIAL_AMETHYST",
                                      [](const py::object &) { return ColorFormat::MATERIAL_AMETHYST; })
        .def_property_readonly_static("OBFUSCATED", [](const py::object &) { return ColorFormat::OBFUSCATED; })
        .def_property_readonly_static("BOLD", [](const py::object &) { return ColorFormat::BOLD; })
        .def_property_readonly_static("ITALIC", [](const py::object &) { return ColorFormat::ITALIC; })
        .def_property_readonly_static("RESET", [](const py::object &) { return ColorFormat::RESET; });

    py::enum_<GameMode>(m, "GameMode")
        .value("SURVIVAL", GameMode::Survival)
        .value("CREATIVE", GameMode::Creative)
        .value("ADVENTURE", GameMode::Adventure)
        .value("SPECTATOR", GameMode::Spectator);
}

}  // namespace endstone::detail
