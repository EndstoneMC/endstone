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
void init_attribute(py::module_ &m)
{
    py::class_<Attribute>(m, "Attribute", "All attribute types.")
#define HANDLE_MC_ATTRIBUTE(ID, NAME, DESC) \
    .def_property_readonly_static(#NAME, [](const py::object &) { return Attribute::NAME; }, DESC)
#include "endstone/attribute/attribute.def"

#undef HANDLE_MC_ATTRIBUTE
        ;
}
}  // namespace endstone::python
