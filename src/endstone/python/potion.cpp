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
void init_potion(py::module_ &m)
{
    // Declared before Effect, whose signatures name Identifier<EffectType>.
    auto effect_type = def_registry_type(py::class_<EffectType>(m, "EffectType", "Represents an effect type."));

    py::class_<Effect>(m, "Effect", "Represents an effect that can be added to a `Mob`.")
        .def(py::init<EffectId, std::optional<int>, int, bool, bool, bool>(), py::arg("type"), py::arg("duration"),
             py::arg("amplifier"), py::arg("ambient") = false, py::arg("particles") = true, py::arg("icon") = true,
             "Creates an effect. A duration of None represents an infinite duration.")
        .def(py::self == py::self)  // NOLINT(misc-redundant-expression)
        .def(py::self != py::self)  // NOLINT(misc-redundant-expression)
        .def("__repr__",
             [](const Effect &self) {
                 const auto duration =
                     self.isInfinite() ? std::string("infinite") : std::to_string(self.getDuration().value());
                 return std::format("Effect(type='{}', duration={}, amplifier={})", self.getType(), duration,
                                    self.getAmplifier());
             })
        .def_property_readonly("type", &Effect::getType, "Gets the type of this effect.")
        .def_property_readonly("duration", &Effect::getDuration,
                               "Gets the duration of this effect in ticks, or None if this effect is infinite.")
        .def_property_readonly("infinite", &Effect::isInfinite, "Whether this effect has an infinite duration.")
        .def_property_readonly("amplifier", &Effect::getAmplifier, "Gets the amplifier of this effect.")
        .def_property_readonly("ambient", &Effect::isAmbient,
                               "Whether this effect produces more, translucent, particles.")
        .def_property_readonly("particles", &Effect::hasParticles, "Whether this effect has particles.")
        .def_property_readonly("icon", &Effect::hasIcon, "Whether this effect has an icon.");

    effect_type.def_property_readonly("id", &EffectType::getId, "The identifier of this effect type.")
        .def_property_readonly("translation_key", &EffectType::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_static("get", &EffectType::get, py::arg("name"), R"doc(
    Attempts to get the `EffectType` with the given name.

    Args:
        name: The identifier of the effect type (e.g. `minecraft:speed`).

    Returns:
        The `EffectType`, or `None` if no effect type with that name exists.
)doc",
                    py::return_value_policy::reference)
        .def_property_readonly_static("SPEED", id(EffectType::Speed), "Increases movement speed.")
        .def_property_readonly_static("SLOWNESS", id(EffectType::Slowness), "Decreases movement speed.")
        .def_property_readonly_static("HASTE", id(EffectType::Haste), "Increases dig speed.")
        .def_property_readonly_static("MINING_FATIGUE", id(EffectType::MiningFatigue), "Decreases dig speed.")
        .def_property_readonly_static("STRENGTH", id(EffectType::Strength), "Increases damage dealt.")
        .def_property_readonly_static("INSTANT_HEALTH", id(EffectType::InstantHealth), "Heals an entity.")
        .def_property_readonly_static("INSTANT_DAMAGE", id(EffectType::InstantDamage), "Hurts an entity.")
        .def_property_readonly_static("JUMP_BOOST", id(EffectType::JumpBoost), "Increases jump height.")
        .def_property_readonly_static("NAUSEA", id(EffectType::Nausea), "Warps vision on the client.")
        .def_property_readonly_static("REGENERATION", id(EffectType::Regeneration), "Regenerates health.")
        .def_property_readonly_static("RESISTANCE", id(EffectType::Resistance), "Decreases damage dealt to an entity.")
        .def_property_readonly_static("FIRE_RESISTANCE", id(EffectType::FireResistance), "Stops fire damage.")
        .def_property_readonly_static("WATER_BREATHING", id(EffectType::WaterBreathing), "Allows breathing underwater.")
        .def_property_readonly_static("INVISIBILITY", id(EffectType::Invisibility), "Grants invisibility.")
        .def_property_readonly_static("BLINDNESS", id(EffectType::Blindness), "Blinds an entity.")
        .def_property_readonly_static("NIGHT_VISION", id(EffectType::NightVision),
                                      "Allows an entity to see in the dark.")
        .def_property_readonly_static("HUNGER", id(EffectType::Hunger), "Increases hunger.")
        .def_property_readonly_static("WEAKNESS", id(EffectType::Weakness), "Decreases damage dealt by an entity.")
        .def_property_readonly_static("POISON", id(EffectType::Poison), "Deals damage to an entity over time.")
        .def_property_readonly_static("WITHER", id(EffectType::Wither),
                                      "Deals damage to an entity over time and gives the health to the shooter.")
        .def_property_readonly_static("HEALTH_BOOST", id(EffectType::HealthBoost),
                                      "Increases the maximum health of an entity.")
        .def_property_readonly_static("ABSORPTION", id(EffectType::Absorption),
                                      "Increases the maximum health of an entity with health that cannot be "
                                      "regenerated, but is refilled every 30 seconds.")
        .def_property_readonly_static("SATURATION", id(EffectType::Saturation),
                                      "Increases the food level of an entity each tick.")
        .def_property_readonly_static("LEVITATION", id(EffectType::Levitation),
                                      "Causes the entity to float into the air.")
        .def_property_readonly_static(
            "FATAL_POISON", id(EffectType::FatalPoison),
            "Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither.")
        .def_property_readonly_static("CONDUIT_POWER", id(EffectType::ConduitPower),
                                      "Effects granted by a nearby conduit. Includes enhanced underwater abilities.")
        .def_property_readonly_static("SLOW_FALLING", id(EffectType::SlowFalling), "Slows entity fall rate.")
        .def_property_readonly_static("BAD_OMEN", id(EffectType::BadOmen),
                                      "Triggers an ominous event when the player enters a village or trial chambers.")
        .def_property_readonly_static("VILLAGE_HERO", id(EffectType::VillageHero),
                                      "Reduces the cost of villager trades.")
        .def_property_readonly_static("DARKNESS", id(EffectType::Darkness),
                                      "Causes the player's vision to dim occasionally.")
        .def_property_readonly_static("TRIAL_OMEN", id(EffectType::TrialOmen),
                                      "Causes trial spawners to become ominous.")
        .def_property_readonly_static("WIND_CHARGED", id(EffectType::WindCharged), "Emits a wind burst upon death.")
        .def_property_readonly_static("WEAVING", id(EffectType::Weaving), "Creates cobwebs upon death.")
        .def_property_readonly_static("OOZING", id(EffectType::Oozing), "Causes slimes to spawn upon death.")
        .def_property_readonly_static("INFESTED", id(EffectType::Infested), "Chance of spawning silverfish when hurt.")
        .def_property_readonly_static("RAID_OMEN", id(EffectType::RaidOmen),
                                      "Triggers a raid when a player enters a village.")
        .def_property_readonly_static(
            "BREATH_OF_THE_NAUTILUS", id(EffectType::BreathOfTheNautilus),
            "Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus.");

    def_registry_type(py::class_<PotionType>(m, "PotionType", "Represents a potion type."))
        .def_property_readonly("id", &PotionType::getId, "The identifier of this potion type.")
        .def_property_readonly("translation_key", &PotionType::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_static("get", &PotionType::get, py::arg("name"), R"doc(
    Attempts to get the `PotionType` with the given name.

    Args:
        name: The identifier of the potion type (e.g. `minecraft:potion_type:water`).

    Returns:
        The `PotionType`, or `None` if no potion type with that name exists.
)doc",
                    py::return_value_policy::reference)
        .def_property_readonly_static("WATER", id(PotionType::Water))
        .def_property_readonly_static("MUNDANE", id(PotionType::Mundane))
        .def_property_readonly_static("LONG_MUNDANE", id(PotionType::LongMundane))
        .def_property_readonly_static("THICK", id(PotionType::Thick))
        .def_property_readonly_static("AWKWARD", id(PotionType::Awkward))
        .def_property_readonly_static("NIGHTVISION", id(PotionType::Nightvision))
        .def_property_readonly_static("LONG_NIGHTVISION", id(PotionType::LongNightvision))
        .def_property_readonly_static("INVISIBILITY", id(PotionType::Invisibility))
        .def_property_readonly_static("LONG_INVISIBILITY", id(PotionType::LongInvisibility))
        .def_property_readonly_static("LEAPING", id(PotionType::Leaping))
        .def_property_readonly_static("LONG_LEAPING", id(PotionType::LongLeaping))
        .def_property_readonly_static("STRONG_LEAPING", id(PotionType::StrongLeaping))
        .def_property_readonly_static("FIRE_RESISTANCE", id(PotionType::FireResistance))
        .def_property_readonly_static("LONG_FIRE_RESISTANCE", id(PotionType::LongFireResistance))
        .def_property_readonly_static("SWIFTNESS", id(PotionType::Swiftness))
        .def_property_readonly_static("LONG_SWIFTNESS", id(PotionType::LongSwiftness))
        .def_property_readonly_static("STRONG_SWIFTNESS", id(PotionType::StrongSwiftness))
        .def_property_readonly_static("SLOWNESS", id(PotionType::Slowness))
        .def_property_readonly_static("LONG_SLOWNESS", id(PotionType::LongSlowness))
        .def_property_readonly_static("STRONG_SLOWNESS", id(PotionType::StrongSlowness))
        .def_property_readonly_static("WATER_BREATHING", id(PotionType::WaterBreathing))
        .def_property_readonly_static("LONG_WATER_BREATHING", id(PotionType::LongWaterBreathing))
        .def_property_readonly_static("HEALING", id(PotionType::Healing))
        .def_property_readonly_static("STRONG_HEALING", id(PotionType::StrongHealing))
        .def_property_readonly_static("HARMING", id(PotionType::Harming))
        .def_property_readonly_static("STRONG_HARMING", id(PotionType::StrongHarming))
        .def_property_readonly_static("POISON", id(PotionType::Poison))
        .def_property_readonly_static("LONG_POISON", id(PotionType::LongPoison))
        .def_property_readonly_static("STRONG_POISON", id(PotionType::StrongPoison))
        .def_property_readonly_static("REGENERATION", id(PotionType::Regeneration))
        .def_property_readonly_static("LONG_REGENERATION", id(PotionType::LongRegeneration))
        .def_property_readonly_static("STRONG_REGENERATION", id(PotionType::StrongRegeneration))
        .def_property_readonly_static("STRENGTH", id(PotionType::Strength))
        .def_property_readonly_static("LONG_STRENGTH", id(PotionType::LongStrength))
        .def_property_readonly_static("STRONG_STRENGTH", id(PotionType::StrongStrength))
        .def_property_readonly_static("WEAKNESS", id(PotionType::Weakness))
        .def_property_readonly_static("LONG_WEAKNESS", id(PotionType::LongWeakness))
        .def_property_readonly_static("WITHER", id(PotionType::Wither))
        .def_property_readonly_static("TURTLE_MASTER", id(PotionType::TurtleMaster))
        .def_property_readonly_static("LONG_TURTLE_MASTER", id(PotionType::LongTurtleMaster))
        .def_property_readonly_static("STRONG_TURTLE_MASTER", id(PotionType::StrongTurtleMaster))
        .def_property_readonly_static("SLOW_FALLING", id(PotionType::SlowFalling))
        .def_property_readonly_static("LONG_SLOW_FALLING", id(PotionType::LongSlowFalling))
        .def_property_readonly_static("WIND_CHARGED", id(PotionType::WindCharged))
        .def_property_readonly_static("WEAVING", id(PotionType::Weaving))
        .def_property_readonly_static("OOZING", id(PotionType::Oozing))
        .def_property_readonly_static("INFESTED", id(PotionType::Infested));
}
}  // namespace endstone::python
