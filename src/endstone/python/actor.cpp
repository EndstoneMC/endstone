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

#include <fmt/format.h>

#include "endstone_python.h"

namespace endstone::python {

void init_actor(py::module_ &m, py_class<Actor> &actor, py_class<Mob> &mob)
{
    py::class_<ActorType>(m, "ActorType", "Represents an actor type.")
        .def_property_readonly_static(
            "AGENT", [](const py::object &) { return ActorType::Agent; })
        .def_property_readonly_static(
            "ALLAY", [](const py::object &) { return ActorType::Allay; })
        .def_property_readonly_static(
            "AREA_EFFECT_CLOUD", [](const py::object &) { return ActorType::AreaEffectCloud; })
        .def_property_readonly_static(
            "ARMADILLO", [](const py::object &) { return ActorType::Armadillo; })
        .def_property_readonly_static(
            "ARMOR_STAND", [](const py::object &) { return ActorType::ArmorStand; })
        .def_property_readonly_static(
            "ARROW", [](const py::object &) { return ActorType::Arrow; })
        .def_property_readonly_static(
            "AXOLOTL", [](const py::object &) { return ActorType::Axolotl; })
        .def_property_readonly_static(
            "BALLOON", [](const py::object &) { return ActorType::Balloon; })
        .def_property_readonly_static(
            "BAT", [](const py::object &) { return ActorType::Bat; })
        .def_property_readonly_static(
            "BEE", [](const py::object &) { return ActorType::Bee; })
        .def_property_readonly_static(
            "BLAZE", [](const py::object &) { return ActorType::Blaze; })
        .def_property_readonly_static(
            "BOAT", [](const py::object &) { return ActorType::Boat; })
        .def_property_readonly_static(
            "BOGGED", [](const py::object &) { return ActorType::Bogged; })
        .def_property_readonly_static(
            "BREEZE", [](const py::object &) { return ActorType::Breeze; })
        .def_property_readonly_static(
            "BREEZE_WIND_CHARGE_PROJECTILE", [](const py::object &) { return ActorType::BreezeWindChargeProjectile; })
        .def_property_readonly_static(
            "CAMEL", [](const py::object &) { return ActorType::Camel; })
        .def_property_readonly_static(
            "CAMEL_HUSK", [](const py::object &) { return ActorType::CamelHusk; })
        .def_property_readonly_static(
            "CAT", [](const py::object &) { return ActorType::Cat; })
        .def_property_readonly_static(
            "CAVE_SPIDER", [](const py::object &) { return ActorType::CaveSpider; })
        .def_property_readonly_static(
            "CHEST_BOAT", [](const py::object &) { return ActorType::ChestBoat; })
        .def_property_readonly_static(
            "CHEST_MINECART", [](const py::object &) { return ActorType::ChestMinecart; })
        .def_property_readonly_static(
            "CHICKEN", [](const py::object &) { return ActorType::Chicken; })
        .def_property_readonly_static(
            "COD", [](const py::object &) { return ActorType::Cod; })
        .def_property_readonly_static(
            "COMMAND_BLOCK_MINECART", [](const py::object &) { return ActorType::CommandBlockMinecart; })
        .def_property_readonly_static(
            "COPPER_GOLEM", [](const py::object &) { return ActorType::CopperGolem; })
        .def_property_readonly_static(
            "COW", [](const py::object &) { return ActorType::Cow; })
        .def_property_readonly_static(
            "CREAKING", [](const py::object &) { return ActorType::Creaking; })
        .def_property_readonly_static(
            "CREEPER", [](const py::object &) { return ActorType::Creeper; })
        .def_property_readonly_static(
            "DOLPHIN", [](const py::object &) { return ActorType::Dolphin; })
        .def_property_readonly_static(
            "DONKEY", [](const py::object &) { return ActorType::Donkey; })
        .def_property_readonly_static(
            "DRAGON_FIREBALL", [](const py::object &) { return ActorType::DragonFireball; })
        .def_property_readonly_static(
            "DROWNED", [](const py::object &) { return ActorType::Drowned; })
        .def_property_readonly_static(
            "EGG", [](const py::object &) { return ActorType::Egg; })
        .def_property_readonly_static(
            "ELDER_GUARDIAN", [](const py::object &) { return ActorType::ElderGuardian; })
        .def_property_readonly_static(
            "ELDER_GUARDIAN_GHOST", [](const py::object &) { return ActorType::ElderGuardianGhost; })
        .def_property_readonly_static(
            "ENDER_CRYSTAL", [](const py::object &) { return ActorType::EnderCrystal; })
        .def_property_readonly_static(
            "ENDER_DRAGON", [](const py::object &) { return ActorType::EnderDragon; })
        .def_property_readonly_static(
            "ENDER_PEARL", [](const py::object &) { return ActorType::EnderPearl; })
        .def_property_readonly_static(
            "ENDERMAN", [](const py::object &) { return ActorType::Enderman; })
        .def_property_readonly_static(
            "ENDERMITE", [](const py::object &) { return ActorType::Endermite; })
        .def_property_readonly_static(
            "EVOCATION_FANG", [](const py::object &) { return ActorType::EvocationFang; })
        .def_property_readonly_static(
            "EVOCATION_ILLAGER", [](const py::object &) { return ActorType::EvocationIllager; })
        .def_property_readonly_static(
            "EYE_OF_ENDER_SIGNAL", [](const py::object &) { return ActorType::EyeOfEnderSignal; })
        .def_property_readonly_static(
            "FALLING_BLOCK", [](const py::object &) { return ActorType::FallingBlock; })
        .def_property_readonly_static(
            "FIREBALL", [](const py::object &) { return ActorType::Fireball; })
        .def_property_readonly_static(
            "FIREWORKS_ROCKET", [](const py::object &) { return ActorType::FireworksRocket; })
        .def_property_readonly_static(
            "FISHING_HOOK", [](const py::object &) { return ActorType::FishingHook; })
        .def_property_readonly_static(
            "FOX", [](const py::object &) { return ActorType::Fox; })
        .def_property_readonly_static(
            "FROG", [](const py::object &) { return ActorType::Frog; })
        .def_property_readonly_static(
            "GHAST", [](const py::object &) { return ActorType::Ghast; })
        .def_property_readonly_static(
            "GLOW_SQUID", [](const py::object &) { return ActorType::GlowSquid; })
        .def_property_readonly_static(
            "GOAT", [](const py::object &) { return ActorType::Goat; })
        .def_property_readonly_static(
            "GUARDIAN", [](const py::object &) { return ActorType::Guardian; })
        .def_property_readonly_static(
            "HAPPY_GHAST", [](const py::object &) { return ActorType::HappyGhast; })
        .def_property_readonly_static(
            "HOGLIN", [](const py::object &) { return ActorType::Hoglin; })
        .def_property_readonly_static(
            "HOPPER_MINECART", [](const py::object &) { return ActorType::HopperMinecart; })
        .def_property_readonly_static(
            "HORSE", [](const py::object &) { return ActorType::Horse; })
        .def_property_readonly_static(
            "HUSK", [](const py::object &) { return ActorType::Husk; })
        .def_property_readonly_static(
            "ICE_BOMB", [](const py::object &) { return ActorType::IceBomb; })
        .def_property_readonly_static(
            "IRON_GOLEM", [](const py::object &) { return ActorType::IronGolem; })
        .def_property_readonly_static(
            "ITEM", [](const py::object &) { return ActorType::Item; })
        .def_property_readonly_static(
            "LEASH_KNOT", [](const py::object &) { return ActorType::LeashKnot; })
        .def_property_readonly_static(
            "LIGHTNING_BOLT", [](const py::object &) { return ActorType::LightningBolt; })
        .def_property_readonly_static(
            "LINGERING_POTION", [](const py::object &) { return ActorType::LingeringPotion; })
        .def_property_readonly_static(
            "LLAMA", [](const py::object &) { return ActorType::Llama; })
        .def_property_readonly_static(
            "LLAMA_SPIT", [](const py::object &) { return ActorType::LlamaSpit; })
        .def_property_readonly_static(
            "MAGMA_CUBE", [](const py::object &) { return ActorType::MagmaCube; })
        .def_property_readonly_static(
            "MINECART", [](const py::object &) { return ActorType::Minecart; })
        .def_property_readonly_static(
            "MOOSHROOM", [](const py::object &) { return ActorType::Mooshroom; })
        .def_property_readonly_static(
            "MULE", [](const py::object &) { return ActorType::Mule; })
        .def_property_readonly_static(
            "NAUTILUS", [](const py::object &) { return ActorType::Nautilus; })
        .def_property_readonly_static(
            "NPC", [](const py::object &) { return ActorType::Npc; })
        .def_property_readonly_static(
            "OCELOT", [](const py::object &) { return ActorType::Ocelot; })
        .def_property_readonly_static(
            "OMINOUS_ITEM_SPAWNER", [](const py::object &) { return ActorType::OminousItemSpawner; })
        .def_property_readonly_static(
            "PAINTING", [](const py::object &) { return ActorType::Painting; })
        .def_property_readonly_static(
            "PANDA", [](const py::object &) { return ActorType::Panda; })
        .def_property_readonly_static(
            "PARCHED", [](const py::object &) { return ActorType::Parched; })
        .def_property_readonly_static(
            "PARROT", [](const py::object &) { return ActorType::Parrot; })
        .def_property_readonly_static(
            "PHANTOM", [](const py::object &) { return ActorType::Phantom; })
        .def_property_readonly_static(
            "PIG", [](const py::object &) { return ActorType::Pig; })
        .def_property_readonly_static(
            "PIGLIN", [](const py::object &) { return ActorType::Piglin; })
        .def_property_readonly_static(
            "PIGLIN_BRUTE", [](const py::object &) { return ActorType::PiglinBrute; })
        .def_property_readonly_static(
            "PILLAGER", [](const py::object &) { return ActorType::Pillager; })
        .def_property_readonly_static(
            "PLAYER", [](const py::object &) { return ActorType::Player; })
        .def_property_readonly_static(
            "POLAR_BEAR", [](const py::object &) { return ActorType::PolarBear; })
        .def_property_readonly_static(
            "PUFFERFISH", [](const py::object &) { return ActorType::Pufferfish; })
        .def_property_readonly_static(
            "RABBIT", [](const py::object &) { return ActorType::Rabbit; })
        .def_property_readonly_static(
            "RAVAGER", [](const py::object &) { return ActorType::Ravager; })
        .def_property_readonly_static(
            "SALMON", [](const py::object &) { return ActorType::Salmon; })
        .def_property_readonly_static(
            "SHEEP", [](const py::object &) { return ActorType::Sheep; })
        .def_property_readonly_static(
            "SHULKER", [](const py::object &) { return ActorType::Shulker; })
        .def_property_readonly_static(
            "SHULKER_BULLET", [](const py::object &) { return ActorType::ShulkerBullet; })
        .def_property_readonly_static(
            "SILVERFISH", [](const py::object &) { return ActorType::Silverfish; })
        .def_property_readonly_static(
            "SKELETON", [](const py::object &) { return ActorType::Skeleton; })
        .def_property_readonly_static(
            "SKELETON_HORSE", [](const py::object &) { return ActorType::SkeletonHorse; })
        .def_property_readonly_static(
            "SLIME", [](const py::object &) { return ActorType::Slime; })
        .def_property_readonly_static(
            "SMALL_FIREBALL", [](const py::object &) { return ActorType::SmallFireball; })
        .def_property_readonly_static(
            "SNIFFER", [](const py::object &) { return ActorType::Sniffer; })
        .def_property_readonly_static(
            "SNOW_GOLEM", [](const py::object &) { return ActorType::SnowGolem; })
        .def_property_readonly_static(
            "SNOWBALL", [](const py::object &) { return ActorType::Snowball; })
        .def_property_readonly_static(
            "SPIDER", [](const py::object &) { return ActorType::Spider; })
        .def_property_readonly_static(
            "SPLASH_POTION", [](const py::object &) { return ActorType::SplashPotion; })
        .def_property_readonly_static(
            "SQUID", [](const py::object &) { return ActorType::Squid; })
        .def_property_readonly_static(
            "STRAY", [](const py::object &) { return ActorType::Stray; })
        .def_property_readonly_static(
            "STRIDER", [](const py::object &) { return ActorType::Strider; })
        .def_property_readonly_static(
            "TADPOLE", [](const py::object &) { return ActorType::Tadpole; })
        .def_property_readonly_static(
            "THROWN_TRIDENT", [](const py::object &) { return ActorType::ThrownTrident; })
        .def_property_readonly_static(
            "TNT", [](const py::object &) { return ActorType::Tnt; })
        .def_property_readonly_static(
            "TNT_MINECART", [](const py::object &) { return ActorType::TntMinecart; })
        .def_property_readonly_static(
            "TRADER_LLAMA", [](const py::object &) { return ActorType::TraderLlama; })
        .def_property_readonly_static(
            "TRIPOD_CAMERA", [](const py::object &) { return ActorType::TripodCamera; })
        .def_property_readonly_static(
            "TROPICALFISH", [](const py::object &) { return ActorType::Tropicalfish; })
        .def_property_readonly_static(
            "TURTLE", [](const py::object &) { return ActorType::Turtle; })
        .def_property_readonly_static(
            "VEX", [](const py::object &) { return ActorType::Vex; })
        .def_property_readonly_static(
            "VILLAGER", [](const py::object &) { return ActorType::Villager; })
        .def_property_readonly_static(
            "VILLAGER_V2", [](const py::object &) { return ActorType::VillagerV2; })
        .def_property_readonly_static(
            "VINDICATOR", [](const py::object &) { return ActorType::Vindicator; })
        .def_property_readonly_static(
            "WANDERING_TRADER", [](const py::object &) { return ActorType::WanderingTrader; })
        .def_property_readonly_static(
            "WARDEN", [](const py::object &) { return ActorType::Warden; })
        .def_property_readonly_static(
            "WIND_CHARGE_PROJECTILE", [](const py::object &) { return ActorType::WindChargeProjectile; })
        .def_property_readonly_static(
            "WITCH", [](const py::object &) { return ActorType::Witch; })
        .def_property_readonly_static(
            "WITHER", [](const py::object &) { return ActorType::Wither; })
        .def_property_readonly_static(
            "WITHER_SKELETON", [](const py::object &) { return ActorType::WitherSkeleton; })
        .def_property_readonly_static(
            "WITHER_SKULL", [](const py::object &) { return ActorType::WitherSkull; })
        .def_property_readonly_static(
            "WITHER_SKULL_DANGEROUS", [](const py::object &) { return ActorType::WitherSkullDangerous; })
        .def_property_readonly_static(
            "WOLF", [](const py::object &) { return ActorType::Wolf; })
        .def_property_readonly_static(
            "XP_BOTTLE", [](const py::object &) { return ActorType::XpBottle; })
        .def_property_readonly_static(
            "XP_ORB", [](const py::object &) { return ActorType::XpOrb; })
        .def_property_readonly_static(
            "ZOGLIN", [](const py::object &) { return ActorType::Zoglin; })
        .def_property_readonly_static(
            "ZOMBIE", [](const py::object &) { return ActorType::Zombie; })
        .def_property_readonly_static(
            "ZOMBIE_HORSE", [](const py::object &) { return ActorType::ZombieHorse; })
        .def_property_readonly_static(
            "ZOMBIE_NAUTILUS", [](const py::object &) { return ActorType::ZombieNautilus; })
        .def_property_readonly_static(
            "ZOMBIE_PIGMAN", [](const py::object &) { return ActorType::ZombiePigman; })
        .def_property_readonly_static(
            "ZOMBIE_VILLAGER", [](const py::object &) { return ActorType::ZombieVillager; })
        .def_property_readonly_static(
            "ZOMBIE_VILLAGER_V2", [](const py::object &) { return ActorType::ZombieVillagerV2; })
        .def_property_readonly("id", &ActorType::getId, "Return the identifier of this actor type.")
        .def_property_readonly("translation_key", &ActorType::getTranslationKey,
                               "Get the translation key, suitable for use in a translation component.")
        .def_static("get", &ActorType::get, py::arg("name"), "Attempts to get the ActorType with the given name.",
                    py::return_value_policy::reference)
        .def("__str__", [](const ActorType &self) { return std::string(self.getId()); })
        .def("__repr__", [](const ActorType &self) { return fmt::format("ActorType({})", self.getId()); })
        .def("__hash__", [](const ActorType &self) { return std::hash<ActorTypeId>{}(self.getId()); })
        .def(py::self == py::self)
        .def(py::self != py::self);

    actor.def_property_readonly("type", &Actor::getType, py::return_value_policy::reference,
                                "Gets the type of the actor.")
        .def_property_readonly("runtime_id", &Actor::getRuntimeId, "Returns the runtime id for this actor.")
        .def_property_readonly("location", &Actor::getLocation, "Gets the actor's current position.")
        .def_property_readonly("velocity", &Actor::getVelocity, "Gets this actor's current velocity.")
        .def_property_readonly("is_on_ground", &Actor::isOnGround,
                               "Returns true if the actor is supported by a block, i.e. on ground.")
        .def_property_readonly("is_in_water", &Actor::isInWater, "Returns true if the actor is in water.")
        .def_property_readonly("is_in_lava", &Actor::isInLava, "Returns true if the actor is in lava.")
        .def_property_readonly("level", &Actor::getLevel, "Gets the current Level this actor resides in.",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Actor::getDimension, "Gets the current Dimension this actor resides in.",
                               py::return_value_policy::reference)
        .def("set_rotation", &Actor::setRotation, "Sets the actor's rotation.", py::arg("yaw"), py::arg("pitch"))
        .def("teleport", py::overload_cast<const Location &>(&Actor::teleport),
             "Teleports this actor to the given location.", py::arg("location"))
        .def("teleport", py::overload_cast<const Actor &>(&Actor::teleport),
             "Teleports this actor to the target Actor.", py::arg("target"))
        .def_property_readonly("id", &Actor::getId, "Returns a unique id for this actor.")
        .def("remove", &Actor::remove, "Remove this actor from the level.")
        .def_property_readonly("is_valid", &Actor::isValid,
                               "Returns false if the entity has died, been despawned for some other reason, or has not "
                               "been added to the level.")
        .def_property_readonly("is_dead", &Actor::isDead, "Returns true if this actor has been marked for removal.")
        .def_property_readonly("scoreboard_tags", &Actor::getScoreboardTags,
                               "Returns a list of scoreboard tags for this actor.")
        .def("add_scoreboard_tag", &Actor::addScoreboardTag, "Adds a tag to this actor.", py::arg("tag"))
        .def("remove_scoreboard_tag", &Actor::removeScoreboardTag, "Removes a given tag from this actor.",
             py::arg("tag"))
        .def_property("is_name_tag_visible", &Actor::isNameTagVisible, &Actor::setNameTagVisible,
                      "Gets or sets if the actor's name tag is visible or not.")
        .def_property("is_name_tag_always_visible", &Actor::isNameTagAlwaysVisible, &Actor::setNameTagAlwaysVisible,
                      "Gets or sets if the actor's name tag is always visible or not.")
        .def_property("name_tag", &Actor::getNameTag, &Actor::setNameTag,
                      "Gets or sets the current name tag of the actor.")
        .def_property("score_tag", &Actor::getScoreTag, &Actor::setScoreTag,
                      "Gets or sets the current score tag of the actor.");

    mob.def_property_readonly("is_gliding", &Mob::isGliding,
                              "Checks to see if an actor is gliding, such as using an Elytra.")
        .def_property("health", &Mob::getHealth, &Mob::setHealth,
                      "Gets or sets the entity's health from 0 to its max possible value, where 0 is dead.")
        .def_property("max_health", &Mob::getMaxHealth, &Mob::setMaxHealth,
                      "Gets or sets the maximum health this entity has.");

    py_class<Item>(m, "Item", "Represents a base actor in the level.")
        .def_property("item_stack", &Item::getItemStack, &Item::setItemStack,
                      "Gets or sets the item stack associated with this item drop.")
        .def_property("pickup_delay", &Item::getPickupDelay, &Item::setPickupDelay,
                      "Gets or sets the delay before this Item is available to be picked up by players.")
        .def_property("is_unlimited_lifetime", &Item::isUnlimitedLifetime, &Item::setUnlimitedLifetime,
                      "Gets or sets if this Item lives forever")
        .def_property("thrower", &Item::getThrower, &Item::setThrower, "Gets or sets the thrower of this item.");
}

}  // namespace endstone::python
