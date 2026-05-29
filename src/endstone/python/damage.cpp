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

void init_damage(py::module &m)
{
    py::class_<DamageSource>(m, "DamageSource", "Represents a source of damage.")
        .def_property_readonly("type", &DamageSource::getType, "The damage type.")
        .def_property_readonly("actor", &DamageSource::getActor, py::return_value_policy::reference, R"doc(
    The actor that caused the damage to occur.

    Not to be confused with damaging_actor, the returned actor is the actor to which the damage is
    ultimately attributed if the receiver is killed. If, for example, the receiver was damaged by a projectile, the
    shooter/thrower would be returned.
)doc")
        .def_property_readonly("damaging_actor", &DamageSource::getDamagingActor, py::return_value_policy::reference,
                               R"doc(
    The actor that directly caused the damage.

    Not to be confused with actor, the returned actor is the actor that actually inflicted the
    damage. If, for example, the receiver was damaged by a projectile, the projectile would be returned.
)doc")
        .def_property_readonly("is_indirect", &DamageSource::isIndirect, R"doc(
    Whether this damage is indirect.

    Damage is considered indirect if actor is not equal to damaging_actor. This will be the case, for
    example, if a skeleton shot an arrow or a player threw a potion.
)doc")
        .def("__str__", [](const DamageSource &self) { return std::format("{}", self); });
}

}  // namespace endstone::python
