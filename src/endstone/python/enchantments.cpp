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

#include <format>

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_enchantments(py::module_ &m)
{
    def_registry_type(py::class_<Enchantment, std::shared_ptr<Enchantment>>(
        m, "Enchantment", "Represents an enchantment that may be added to armor, weapons, tools or other items."))
        .def_property_readonly_static("PROTECTION", id(Enchantment::Protection),
                                      "Provides protection against environmental damage.")
        .def_property_readonly_static("FIRE_PROTECTION", id(Enchantment::FireProtection),
                                      "Provides protection against fire damage.")
        .def_property_readonly_static("FEATHER_FALLING", id(Enchantment::FeatherFalling),
                                      "Provides protection against fall damage.")
        .def_property_readonly_static("BLAST_PROTECTION", id(Enchantment::BlastProtection),
                                      "Provides protection against explosive damage.")
        .def_property_readonly_static("PROJECTILE_PROTECTION", id(Enchantment::ProjectileProtection),
                                      "Provides protection against projectile damage.")
        .def_property_readonly_static("THORNS", id(Enchantment::Thorns), "Damages the attacker.")
        .def_property_readonly_static("RESPIRATION", id(Enchantment::Respiration),
                                      "Decreases the rate of air loss whilst underwater.")
        .def_property_readonly_static("DEPTH_STRIDER", id(Enchantment::DepthStrider),
                                      "Increases walking speed while in water.")
        .def_property_readonly_static("AQUA_AFFINITY", id(Enchantment::AquaAffinity),
                                      "Increases the speed at which a player may mine underwater.")
        .def_property_readonly_static("SHARPNESS", id(Enchantment::Sharpness), "Increases damage against all targets.")
        .def_property_readonly_static("SMITE", id(Enchantment::Smite), "Increases damage against undead targets.")
        .def_property_readonly_static("BANE_OF_ARTHROPODS", id(Enchantment::BaneOfArthropods),
                                      "Increases damage against arthropod targets.")
        .def_property_readonly_static("KNOCKBACK", id(Enchantment::Knockback),
                                      "All damage to other targets will knock them back when hit.")
        .def_property_readonly_static("FIRE_ASPECT", id(Enchantment::FireAspect),
                                      "When attacking a target, has a chance to set them on fire.")
        .def_property_readonly_static("LOOTING", id(Enchantment::Looting),
                                      "Provides a chance of gaining extra loot when killing monsters.")
        .def_property_readonly_static("EFFICIENCY", id(Enchantment::Efficiency),
                                      "Increases the rate at which you mine/dig.")
        .def_property_readonly_static(
            "SILK_TOUCH", id(Enchantment::SilkTouch),
            "Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone).")
        .def_property_readonly_static("UNBREAKING", id(Enchantment::Unbreaking),
                                      "Decreases the rate at which a tool looses durability.")
        .def_property_readonly_static("FORTUNE", id(Enchantment::Fortune),
                                      "Provides a chance of gaining extra loot when destroying blocks.")
        .def_property_readonly_static("POWER", id(Enchantment::Power),
                                      "Provides extra damage when shooting arrows from bows.")
        .def_property_readonly_static("PUNCH", id(Enchantment::Punch),
                                      "Provides a knockback when an entity is hit by an arrow from a bow.")
        .def_property_readonly_static("FLAME", id(Enchantment::Flame),
                                      "Sets entities on fire when hit by arrows shot from a bow.")
        .def_property_readonly_static("INFINITY", id(Enchantment::Infinity),
                                      "Provides infinite arrows when shooting a bow.")
        .def_property_readonly_static("LUCK_OF_THE_SEA", id(Enchantment::LuckOfTheSea),
                                      "Decreases odds of catching worthless junk.")
        .def_property_readonly_static("LURE", id(Enchantment::Lure), "Increases rate of fish biting your hook.")
        .def_property_readonly_static("FROST_WALKER", id(Enchantment::FrostWalker),
                                      "Freezes any still water adjacent to ice / frost which player is walking on.")
        .def_property_readonly_static("MENDING", id(Enchantment::Mending),
                                      "Allows mending the item using experience orbs.")
        .def_property_readonly_static("CURSE_OF_BINDING", id(Enchantment::CurseOfBinding), "Item cannot be removed.")
        .def_property_readonly_static("CURSE_OF_VANISHING", id(Enchantment::CurseOfVanishing),
                                      "Item disappears instead of dropping.")
        .def_property_readonly_static("IMPALING", id(Enchantment::Impaling),
                                      "Deals more damage to mobs that live in the ocean.")
        .def_property_readonly_static("RIPTIDE", id(Enchantment::Riptide),
                                      "When it is rainy, launches the player in the direction their trident is thrown.")
        .def_property_readonly_static("LOYALTY", id(Enchantment::Loyalty),
                                      "Causes a thrown trident to return to the player who threw it.")
        .def_property_readonly_static("CHANNELING", id(Enchantment::Channeling),
                                      "Strikes lightning when a mob is hit with a trident if conditions are stormy.")
        .def_property_readonly_static("MULTISHOT", id(Enchantment::Multishot), "Shoot multiple arrows from crossbows.")
        .def_property_readonly_static("PIERCING", id(Enchantment::Piercing), "Crossbow projectiles pierce entities.")
        .def_property_readonly_static("QUICK_CHARGE", id(Enchantment::QuickCharge), "Charges crossbows quickly.")
        .def_property_readonly_static("SOUL_SPEED", id(Enchantment::SoulSpeed), "Walk quicker on soul blocks.")
        .def_property_readonly_static("SWIFT_SNEAK", id(Enchantment::SwiftSneak), "Walk quicker while sneaking.")
        .def_property_readonly_static("WIND_BURST", id(Enchantment::WindBurst), "Emits wind burst upon hitting enemy.")
        .def_property_readonly_static("DENSITY", id(Enchantment::Density), "Increases fall damage of maces.")
        .def_property_readonly_static("BREACH", id(Enchantment::Breach), "Reduces armor effectiveness against maces.")
        .def_property_readonly_static(
            "LUNGE", id(Enchantment::Lunge),
            "Applied to spears, causes jab attacks to propel the wielder forward horizontally.")

        .def_property_readonly("id", &Enchantment::getId, "The identifier of this enchantment.")
        .def_property_readonly("translation_key", &Enchantment::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_property_readonly("max_level", &Enchantment::getMaxLevel,
                               "The maximum level that this `Enchantment` may become.")
        .def_property_readonly("start_level", &Enchantment::getStartLevel,
                               "The level that this `Enchantment` should start at (also known as minimum level).")
        .def("conflicts_with", &Enchantment::conflictsWith, py::arg("other"), R"doc(
    Check if this enchantment conflicts with another enchantment.

    Args:
        other: The enchantment to check against.

    Returns:
        `True` if there is a conflict.
)doc")
        .def("can_enchant_item", &Enchantment::canEnchantItem, py::arg("item"), R"doc(
    Checks if this `Enchantment` may be applied to the given `ItemStack`.

    This does not check if it conflicts with any enchantments already applied to the item.

    Args:
        item: Item to test.

    Returns:
        `True` if the enchantment may be applied, `False` otherwise.
)doc")
        .def_static("get", &Enchantment::get, py::arg("name"), R"doc(
    Look up a registry entry by identifier.

    Args:
        name: Identifier to look up.

    Returns:
        The `Enchantment`, or `None` if not found.
)doc",
                    py::return_value_policy::reference);
}

}  // namespace endstone::python
