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

namespace py = pybind11;

namespace endstone::python {
void init_ability(py::module_ &m)
{
    def_registry_type(py::class_<Ability>(m, "Ability", R"doc(
    All player abilities.

    `ATTACK_MOBS`, `ATTACK_PLAYERS`, `BUILD`, `DOORS_AND_SWITCHES`, `MINE`, `OPEN_CONTAINERS`, `OPERATOR_COMMANDS`
    and `TELEPORT` are the eight member permissions the client shows in its pause menu, and the server enforces
    every one of them.
)doc"))
        .def_property_readonly("id", &Ability::getId, "The identifier of this ability.")
        .def_property_readonly("translation_key", &Ability::getTranslationKey, "The translation key of this ability.")
        .def_static("get", &Ability::get, py::arg("name"), R"doc(
    Attempts to get the `Ability` with the given name.

    Args:
        name: The identifier of the ability (e.g. `minecraft:noclip`).

    Returns:
        The `Ability`, or `None` if no ability with that name exists.
)doc",
                    py::return_value_policy::reference)
        .def_static(
            "__class_getitem__", [](const py::object &) { return py::type::of<Ability>(); }, py::arg("item"))
        .def_property_readonly_static("ATTACK_MOBS", id(Ability::AttackMobs), "Whether the player can attack mobs.")
        .def_property_readonly_static("ATTACK_PLAYERS", id(Ability::AttackPlayers),
                                      "Whether the player can attack other players.")
        .def_property_readonly_static("BUILD", id(Ability::Build), "Whether the player can place blocks.")
        .def_property_readonly_static("DOORS_AND_SWITCHES", id(Ability::DoorsAndSwitches),
                                      "Whether the player can use doors, trapdoors, buttons, levers and other "
                                      "redstone components.")
        .def_property_readonly_static("FLYING", id(Ability::Flying), "Whether the player is currently flying.")
        .def_property_readonly_static("FLY_SPEED", id(Ability::FlySpeed),
                                      "The speed at which the player flies, default is ``0.05``.")
        .def_property_readonly_static("INSTABUILD", id(Ability::Instabuild),
                                      "Whether the player destroys blocks instantly.")
        .def_property_readonly_static("INVULNERABLE", id(Ability::Invulnerable),
                                      "Whether the player is immune to all damage.")
        .def_property_readonly_static("LIGHTNING", id(Ability::Lightning),
                                      "Whether the player was struck by lightning.")
        .def_property_readonly_static("MAY_FLY", id(Ability::MayFly), "Whether the player is allowed to fly.")
        .def_property_readonly_static("MINE", id(Ability::Mine), "Whether the player can destroy blocks.")
        .def_property_readonly_static("MUTED", id(Ability::Muted),
                                      "Whether the player's chat messages are hidden from other players.")
        .def_property_readonly_static("NO_CLIP", id(Ability::NoClip), "Whether the player can move through blocks.")
        .def_property_readonly_static("OPEN_CONTAINERS", id(Ability::OpenContainers),
                                      "Whether the player can open containers.")
        .def_property_readonly_static("OPERATOR_COMMANDS", id(Ability::OperatorCommands),
                                      "Whether the player can use operator commands.")
        .def_property_readonly_static("PRIVILEGED_BUILDER", id(Ability::PrivilegedBuilder),
                                      "Whether the player is a privileged builder.")
        .def_property_readonly_static("TELEPORT", id(Ability::Teleport), "Whether the player can teleport.")
        .def_property_readonly_static("VERTICAL_FLY_SPEED", id(Ability::VerticalFlySpeed),
                                      "The speed at which the player flies up and down, default is ``1``.")
        .def_property_readonly_static("WALK_SPEED", id(Ability::WalkSpeed),
                                      "The speed at which the player walks, default is ``0.1``.")
        .def_property_readonly_static("WORLD_BUILDER", id(Ability::WorldBuilder),
                                      "Whether the player is a world builder.");
}
}  // namespace endstone::python
