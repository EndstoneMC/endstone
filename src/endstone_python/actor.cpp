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

#include "endstone/actor/actor.h"

#include <pybind11/pybind11.h>

#include "endstone/level/level.h"

namespace py = pybind11;

namespace endstone::detail {

void init_actor(py::module_ &m, py::class_<Actor, CommandSender> &actor)
{
    actor.def_property_readonly("runtime_id", &Actor::getRuntimeId, "Returns the runtime id for this actor.")
        .def_property_readonly("location", &Actor::getLocation, "Gets the actor's current position.")
        .def_property_readonly("velocity", &Actor::getVelocity, "Gets this actor's current velocity.")
        .def_property_readonly("is_on_ground", &Actor::isOnGround,
                               "Returns true if the actor is supported by a block, i.e. on ground.")
        .def_property_readonly("is_in_water", &Actor::isInWater, "Returns true if the actor is in water.")
        .def_property_readonly("is_in_lava", &Actor::isInLava, "Returns true if the actor is in lava.")
        .def_property_readonly("level", &Actor::getLevel, "Gets the current Level this actor resides in.",
                               py::return_value_policy::reference);
}

}  // namespace endstone::detail
