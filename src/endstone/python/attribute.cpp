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
    def_registry_type(
        py::class_<Attribute>(m, "Attribute", "Represents a type of attribute which may be present on a `Mob`."))
        .def_property_readonly("id", &Attribute::getId, "The identifier of this attribute.")
        .def_property_readonly("translation_key", &Attribute::getTranslationKey,
                               "Raises, as Bedrock attributes carry no translation key.")
        .def_static("get", &Attribute::get, py::arg("name"), R"doc(
    Attempts to get the `Attribute` with the given name.

    Args:
        name: The identifier of the attribute (e.g. `minecraft:health`).

    Returns:
        The `Attribute`, or `None` if no attribute with that name exists.
)doc",
                    py::return_value_policy::reference)
        .def_property_readonly_static("HEALTH", id(Attribute::Health), "Health of an entity.")
        .def_property_readonly_static("FOLLOW_RANGE", id(Attribute::FollowRange),
                                      "Range at which an entity will follow others.")
        .def_property_readonly_static("KNOCKBACK_RESISTANCE", id(Attribute::KnockbackResistance),
                                      "Resistance of an entity to knockback.")
        .def_property_readonly_static("MOVEMENT_SPEED", id(Attribute::MovementSpeed), "Movement speed of an entity.")
        .def_property_readonly_static("UNDERWATER_MOVEMENT_SPEED", id(Attribute::UnderwaterMovementSpeed),
                                      "Movement speed of an entity underwater.")
        .def_property_readonly_static("LAVA_MOVEMENT_SPEED", id(Attribute::LavaMovementSpeed),
                                      "Movement speed of an entity in lava.")
        .def_property_readonly_static("ATTACK_DAMAGE", id(Attribute::AttackDamage), "Attack damage of an entity.")
        .def_property_readonly_static("ABSORPTION", id(Attribute::Absorption), "Absorption of an entity.")
        .def_property_readonly_static("LUCK", id(Attribute::Luck), "Luck bonus of an entity.")
        .def_property_readonly_static("JUMP_STRENGTH", id(Attribute::JumpStrength),
                                      "Strength with which an entity will jump.")
        .def_property_readonly_static("PLAYER_HUNGER", id(Attribute::PlayerHunger), "Hunger level of a player.")
        .def_property_readonly_static("PLAYER_SATURATION", id(Attribute::PlayerSaturation),
                                      "Saturation level of a player.")
        .def_property_readonly_static("PLAYER_EXHAUSTION", id(Attribute::PlayerExhaustion),
                                      "Exhaustion level of a player.")
        .def_property_readonly_static("PLAYER_LEVEL", id(Attribute::PlayerLevel), "Experience level of a player.")
        .def_property_readonly_static("PLAYER_EXPERIENCE", id(Attribute::PlayerExperience),
                                      "Progress toward the next experience level of a player.")
        .def_property_readonly_static("ZOMBIE_SPAWN_REINFORCEMENTS", id(Attribute::ZombieSpawnReinforcements),
                                      "Chance of a zombie to spawn reinforcements.");

    auto modifier = py::class_<AttributeModifier>(m, "AttributeModifier", "Represents an attribute modifier.");

    py::native_enum<AttributeModifier::Operation>(modifier, "Operation", "enum.Enum", "Operation to be applied.")
        .value("ADD", AttributeModifier::Operation::Add, "Adds (or subtracts) the specified amount to the base value.")
        .value("MULTIPLY_BASE", AttributeModifier::Operation::MultiplyBase,
               "Multiplies the current value of the attribute by (1 + x), "
               "where x is the sum of the modifiers' amounts.")
        .value("MULTIPLY", AttributeModifier::Operation::Multiply,
               "For every modifier, multiplies the current value of the attribute by (1 + x), "
               "where x is the amount of the particular modifier.")
        .value("CAP", AttributeModifier::Operation::Cap,
               "Caps the value of the attribute at the modifier's amount. Bedrock-specific operation with no Bukkit "
               "equivalent.")
        .export_values()
        .finalize();

    modifier
        .def(py::init<AttributeModifierId, float, AttributeModifier::Operation>(), py::arg("key"), py::arg("amount"),
             py::arg("operation"))
        .def_property_readonly("key", &AttributeModifier::getKey, "The key of this modifier.")
        .def_property_readonly("amount", &AttributeModifier::getAmount,
                               "The amount by which this modifier will apply its operation.")
        .def_property_readonly("operation", &AttributeModifier::getOperation,
                               "The operation this modifier will apply.");

    py::classh<AttributeInstance>(
        m, "AttributeInstance",
        "Represents a mutable instance of an attribute and its associated modifiers and values.")
        .def_property_readonly("attribute", &AttributeInstance::getAttribute, py::return_value_policy::reference,
                               "The attribute pertaining to this instance.")
        .def_property("base_value", &AttributeInstance::getBaseValue, &AttributeInstance::setBaseValue,
                      "Base value of this instance before modifiers are applied.")
        .def_property("min_value", &AttributeInstance::getMinValue, &AttributeInstance::setMinValue,
                      "The minimum value this instance is allowed to take. Bedrock-specific.")
        .def_property("max_value", &AttributeInstance::getMaxValue, &AttributeInstance::setMaxValue,
                      "The maximum value this instance is allowed to take. Bedrock-specific.")
        .def_property_readonly("value", &AttributeInstance::getValue,
                               "The value of this instance after all associated modifiers have been applied.")
        .def_property_readonly("modifiers", &AttributeInstance::getModifiers, "All modifiers present on this instance.")
        .def("get_modifier", &AttributeInstance::getModifier, py::arg("key"), R"doc(
    Gets the modifier with the corresponding key.

    Args:
        key: The key of the modifier.

    Returns:
        The modifier, or ``None`` if no modifier with the given key is present.
)doc")
        .def("add_modifier", &AttributeInstance::addModifier, py::arg("modifier"), R"doc(
    Add a modifier to this instance.

    Args:
        modifier: Modifier to add.
)doc")
        .def("add_transient_modifier", &AttributeInstance::addTransientModifier, py::arg("modifier"), R"doc(
    Add a transient modifier to this instance. Transient modifiers are not persisted (saved with the NBT data).

    Args:
        modifier: Modifier to add.
)doc")
        .def("remove_modifier", py::overload_cast<const AttributeModifier &>(&AttributeInstance::removeModifier),
             py::arg("modifier"), R"doc(
    Remove a modifier from this instance.

    Args:
        modifier: Modifier to remove.
)doc")
        .def("remove_modifier", py::overload_cast<AttributeModifierId>(&AttributeInstance::removeModifier),
             py::arg("key"), R"doc(
    Remove a modifier with the corresponding key from this instance.

    Args:
        key: The key of the modifier to remove.
)doc");
}
}  // namespace endstone::python
