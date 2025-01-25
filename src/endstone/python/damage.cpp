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
    py::class_<DamageSource>(m, "DamageSource")
        .def_property_readonly("type", &DamageSource::getType, "Get the damage type.")
        .def_property_readonly("actor", &DamageSource::getActor, py::return_value_policy::reference,
                               "Get the actor that caused the damage to occur.")
        .def_property_readonly("damaging_actor", &DamageSource::getDamagingActor, py::return_value_policy::reference,
                               "Get the actor that directly caused the damage.")
        .def_property_readonly("is_indirect", &DamageSource::isIndirect, "Get if this damage is indirect.");
}

}  // namespace endstone::python
