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

void init_actor(py::module_ &m, py::class_<Actor, CommandSender> &actor, py::class_<Mob, Actor> &mob)
{
    actor.def_property_readonly("type", &Actor::getType, "Gets the type of the actor.")
        .def_property_readonly("runtime_id", &Actor::getRuntimeId, "Returns the runtime id for this actor.")
        .def_property_readonly("location", &Actor::getLocation, "Gets the actor's current position.")
        .def_property_readonly("velocity", &Actor::getVelocity, "Gets this actor's current velocity.")
        .def_property_readonly("is_on_ground", &Actor::isOnGround,
                               "Returns true if the actor is supported by a block, i.e. on ground.")
        .def_property_readonly("is_in_water", &Actor::isInWater, "Returns true if the actor is in water.")
        .def_property_readonly("is_in_lava", &Actor::isInLava, "Returns true if the actor is in lava.")
        .def_property_readonly("level", &Actor::getLevel, "Gets the current Level this actor resides in.",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Actor::getDimension, "Gets the current Dimension this actor resides in.",
                               py::return_value_policy::reference)
        .def("set_rotation", &Actor::setRotation, "Sets the actor's rotation.", py::arg("yaw"), py::arg("pitch"))
        .def("teleport", py::overload_cast<const Location &>(&Actor::teleport),
             "Teleports this actor to the given location.", py::arg("location"))
        .def("teleport", py::overload_cast<const Actor &>(&Actor::teleport),
             "Teleports this actor to the target Actor.", py::arg("target"))
        .def_property_readonly("id", &Actor::getId, "Returns a unique id for this actor.")
        .def("remove", &Actor::remove, "Remove this actor from the level.")
        .def_property_readonly("is_valid", &Actor::isValid,
                               "Returns false if the entity has died, been despawned for some other reason, or has not "
                               "been added to the level.")
        .def_property_readonly("is_dead", &Actor::isDead, "Returns true if this actor has been marked for removal.")
        .def_property_readonly("scoreboard_tags", &Actor::getScoreboardTags,
                               "Returns a list of scoreboard tags for this actor.")
        .def("add_scoreboard_tag", &Actor::addScoreboardTag, "Adds a tag to this actor.", py::arg("tag"))
        .def("remove_scoreboard_tag", &Actor::removeScoreboardTag, "Removes a given tag from this actor.",
             py::arg("tag"))
        .def_property("is_name_tag_visible", &Actor::isNameTagVisible, &Actor::setNameTagVisible,
                      "Gets or sets if the actor's name tag is visible or not.")
        .def_property("is_name_tag_always_visible", &Actor::isNameTagAlwaysVisible, &Actor::setNameTagAlwaysVisible,
                      "Gets or sets if the actor's name tag is always visible or not.")
        .def_property("name_tag", &Actor::getNameTag, &Actor::setNameTag,
                      "Gets or sets the current name tag of the actor.")
        .def_property("score_tag", &Actor::getScoreTag, &Actor::setScoreTag,
                      "Gets or sets the current score tag of the actor.")
        .def("has_attribute", &Actor::hasAttribute, "Check the attribute is present in the object.",
             py::arg("attribute"))
        .def("get_attribute", &Actor::getAttribute,
             "Gets the specified attribute instance from the object. This instance will be backed directly to the "
             "object and any changes will be visible at once.",
             py::arg("attribute"))
        .def_property_readonly("attributes", &Actor::getAttributes,
                               "Gets all attribute instances from the object. This instance will be backed directly to "
                               "the object and any changes will be visible at once.");

    mob.def_property_readonly("is_gliding", &Mob::isGliding,
                              "Checks to see if an actor is gliding, such as using an Elytra.")
        .def_property("health", &Mob::getHealth, &Mob::setHealth,
                      "Gets or sets the entity's health from 0 to its max possible value, where 0 is dead.")
        .def_property("max_health", &Mob::getMaxHealth, &Mob::setMaxHealth,
                      "Gets or sets the maximum health this entity has.");

    py::class_<Item, Actor>(m, "Item", "Represents a base actor in the level.")
        .def_property("item_stack", &Item::getItemStack, &Item::setItemStack,
                      "Gets or sets the item stack associated with this item drop.")
        .def_property("pickup_delay", &Item::getPickupDelay, &Item::setPickupDelay,
                      "Gets or sets the delay before this Item is available to be picked up by players.")
        .def_property("is_unlimited_lifetime", &Item::isUnlimitedLifetime, &Item::setUnlimitedLifetime,
                      "Gets or sets if this Item lives forever")
        .def_property("thrower", &Item::getThrower, &Item::setThrower, "Gets or sets the thrower of this item.");
}

}  // namespace endstone::python
