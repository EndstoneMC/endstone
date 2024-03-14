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

#include <pybind11/pybind11.h>

#include "endstone/permissions/permission_default.h"
#include "endstone/permissions/server_operator.h"

namespace py = pybind11;

namespace endstone::detail {

void init_permissions(py::module &m)
{
    py::class_<ServerOperator, std::shared_ptr<ServerOperator>>(m, "ServerOperator")
        .def_property("op", &ServerOperator::isOp, &ServerOperator::setOp, "The operator status of this object");

    py::enum_<PermissionDefault>(m, "PermissionDefault")
        .value("TRUE", PermissionDefault::True)
        .value("FALSE", PermissionDefault::False)
        .value("OP", PermissionDefault::Operator)
        .value("OPERATOR", PermissionDefault::Operator)
        .value("NOT_OP", PermissionDefault::Operator)
        .value("NOT_OPERATOR", PermissionDefault::NotOperator);
}

}  // namespace endstone::detail
