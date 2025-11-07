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

    auto modifier = py::class_<AttributeModifier>(m, "AttributeModifier", "Represents an attribute modifier.");

    py::native_enum<AttributeModifier::Operand>(modifier, "Operand", "enum.Enum",
                                                "Value on which operation to be applied.")
        .value("VALUE", AttributeModifier::Operand::Value)
        .value("MAX_VALUE", AttributeModifier::Operand::MaxValue)
        .value("MIN_VALUE", AttributeModifier::Operand::MinValue)
        .export_values()
        .finalize();

    py::native_enum<AttributeModifier::Operation>(modifier, "Operation", "enum.Enum", "Operation to be applied.")
        .value("ADD_NUMBER", AttributeModifier::Operation::AddNumber,
               "Adds (or subtracts) the specified amount to the base value.")
        .value("ADD_SCALAR", AttributeModifier::Operation::AddScalar, "Adds this scalar of amount to the base value.")
        .value("MULTIPLY_SCALAR_1", AttributeModifier::Operation::MultiplyScalar1,
               "Multiply amount by this value, after adding 1 to it.")
        .export_values()
        .finalize();

    modifier
        .def(py::init<std::string, float, AttributeModifier::Operation, AttributeModifier::Operand>(), py::arg("name"),
             py::arg("amount"), py::arg("operation"), py::arg("operand") = AttributeModifier::Operand::Value)
        .def_property_readonly("unique_id", &AttributeModifier::getUniqueId, "Get the unique ID for this modifier.")
        .def_property_readonly("name", &AttributeModifier::getName, "Get the name of this modifier.")
        .def_property_readonly("amount", &AttributeModifier::getAmount,
                               "Get the amount by which this modifier will apply the operation.")
        .def_property_readonly("operand", &AttributeModifier::getOperand, "Get the operand this modifier will apply.")
        .def_property_readonly("operation", &AttributeModifier::getOperation,
                               "Get the operation this modifier will apply.");

    py::class_<AttributeInstance>(
        m, "AttributeInstance",
        "Represents a mutable instance of an attribute and its associated modifiers and values.")
        .def_property_readonly("type", &AttributeInstance::getType, "The attribute type pertaining to this instance.")
        .def_property("base_value", &AttributeInstance::getBaseValue, &AttributeInstance::setBaseValue,
                      "Base value of this instance before modifiers are applied.")
        .def_property("base_max_value", &AttributeInstance::getBaseMaxValue, &AttributeInstance::setBaseMaxValue,
                      "Base max value of this instance before modifiers are applied.")
        .def_property("base_min_value", &AttributeInstance::getBaseMinValue, &AttributeInstance::setBaseMinValue,
                      "Base min value of this instance before modifiers are applied.")
        .def_property_readonly("value", &AttributeInstance::getValue,
                               "Get the value of this instance after all associated modifiers have been applied.")
        .def_property_readonly("max_value", &AttributeInstance::getMaxValue,
                               "Get the max value of this instance after all associated modifiers have been applied.")
        .def_property_readonly("min_value", &AttributeInstance::getMinValue,
                               "Get the min value of this instance after all associated modifiers have been applied.")
        .def_property_readonly("modifiers", &AttributeInstance::getModifiers,
                               "Get all modifiers present on this instance.")
        .def("add_modifier", &AttributeInstance::addModifier, py::arg("modifier"), "Add a modifier to this instance.")
        .def("remove_modifier", &AttributeInstance::removeModifier, py::arg("modifier"),
             "Remove a modifier from this instance.");
}
}  // namespace endstone::python
