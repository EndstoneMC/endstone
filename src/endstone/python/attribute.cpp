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
        .def_property_readonly_static(
            "HEALTH", [](const py::object &) { return Attribute::Health; }, "Health of an entity.")
        .def_property_readonly_static(
            "FOLLOW_RANGE", [](const py::object &) { return Attribute::FollowRange; },
            "Range at which an entity will follow others.")
        .def_property_readonly_static(
            "KNOCKBACK_RESISTANCE", [](const py::object &) { return Attribute::KnockbackResistance; },
            "Resistance of an entity to knockback.")
        .def_property_readonly_static(
            "MOVEMENT_SPEED", [](const py::object &) { return Attribute::MovementSpeed; },
            "Movement speed of an entity.")
        .def_property_readonly_static(
            "UNDERWATER_MOVEMENT_SPEED", [](const py::object &) { return Attribute::UnderwaterMovementSpeed; },
            "Movement speed of an entity underwater.")
        .def_property_readonly_static(
            "LAVA_MOVEMENT_SPEED", [](const py::object &) { return Attribute::LavaMovementSpeed; },
            "Movement speed of an entity in lava.")
        .def_property_readonly_static(
            "ATTACK_DAMAGE", [](const py::object &) { return Attribute::AttackDamage; }, "Attack damage of an entity.")
        .def_property_readonly_static(
            "ABSORPTION", [](const py::object &) { return Attribute::Absorption; }, "Absorption of an entity.")
        .def_property_readonly_static(
            "LUCK", [](const py::object &) { return Attribute::Luck; }, "Luck bonus of an entity.")
        .def_property_readonly_static(
            "JUMP_STRENGTH", [](const py::object &) { return Attribute::JumpStrength; },
            "Strength with which an entity will jump.")
        .def_property_readonly_static(
            "PLAYER_HUNGER", [](const py::object &) { return Attribute::PlayerHunger; }, "Hunger level of a player.")
        .def_property_readonly_static(
            "PLAYER_SATURATION", [](const py::object &) { return Attribute::PlayerSaturation; },
            "Saturation level of a player.")
        .def_property_readonly_static(
            "PLAYER_EXHAUSTION", [](const py::object &) { return Attribute::PlayerExhaustion; },
            "Exhaustion level of a player.")
        .def_property_readonly_static(
            "PLAYER_LEVEL", [](const py::object &) { return Attribute::PlayerLevel; }, "Experience level of a player.")
        .def_property_readonly_static(
            "PLAYER_EXPERIENCE", [](const py::object &) { return Attribute::PlayerExperience; },
            "Progress toward the next experience level of a player.")
        .def_property_readonly_static(
            "ZOMBIE_SPAWN_REINFORCEMENTS", [](const py::object &) { return Attribute::ZombieSpawnReinforcements; },
            "Chance of a zombie to spawn reinforcements.");

    auto modifier = py::class_<AttributeModifier>(m, "AttributeModifier", "Represents an attribute modifier.");

    py::native_enum<AttributeModifier::Operand>(modifier, "Operand", "enum.Enum",
                                                "Value on which operation to be applied.")
        .value("VALUE", AttributeModifier::Operand::Value)
        .value("MAX_VALUE", AttributeModifier::Operand::MaxValue)
        .value("MIN_VALUE", AttributeModifier::Operand::MinValue)
        .export_values()
        .finalize();

    py::native_enum<AttributeModifier::Operation>(modifier, "Operation", "enum.Enum", "Operation to be applied.")
        .value("ADD", AttributeModifier::Operation::Add, "Adds (or subtracts) the specified amount to the base value.")
        .value("MULTIPLY_BASE", AttributeModifier::Operation::MultiplyBase,
               "Multiplies the current value of the attribute by (1 + x), "
               "where x is the sum of the modifiers' amounts.")
        .value("MULTIPLY", AttributeModifier::Operation::Multiply,
               "For every modifier, multiplies the current value of the attribute by (1 + x), "
               "where x is the amount of the particular modifier.")
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
