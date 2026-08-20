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

namespace endstone::python {

void init_actor(py::module_ &m, py_class<Actor> &actor, py_class<Mob> &mob)
{
    def_registry_type(py::class_<ActorType>(m, "ActorType", "Represents an actor type."))
        .def_property_readonly_static("AGENT", id(ActorType::Agent))
        .def_property_readonly_static("ALLAY", id(ActorType::Allay))
        .def_property_readonly_static("AREA_EFFECT_CLOUD", id(ActorType::AreaEffectCloud))
        .def_property_readonly_static("ARMADILLO", id(ActorType::Armadillo))
        .def_property_readonly_static("ARMOR_STAND", id(ActorType::ArmorStand))
        .def_property_readonly_static("ARROW", id(ActorType::Arrow))
        .def_property_readonly_static("AXOLOTL", id(ActorType::Axolotl))
        .def_property_readonly_static("BALLOON", id(ActorType::Balloon))
        .def_property_readonly_static("BAT", id(ActorType::Bat))
        .def_property_readonly_static("BEE", id(ActorType::Bee))
        .def_property_readonly_static("BLAZE", id(ActorType::Blaze))
        .def_property_readonly_static("BOAT", id(ActorType::Boat))
        .def_property_readonly_static("BOGGED", id(ActorType::Bogged))
        .def_property_readonly_static("BREEZE", id(ActorType::Breeze))
        .def_property_readonly_static("BREEZE_WIND_CHARGE_PROJECTILE", id(ActorType::BreezeWindChargeProjectile))
        .def_property_readonly_static("CAMEL", id(ActorType::Camel))
        .def_property_readonly_static("CAMEL_HUSK", id(ActorType::CamelHusk))
        .def_property_readonly_static("CAT", id(ActorType::Cat))
        .def_property_readonly_static("CAVE_SPIDER", id(ActorType::CaveSpider))
        .def_property_readonly_static("CHEST_BOAT", id(ActorType::ChestBoat))
        .def_property_readonly_static("CHEST_MINECART", id(ActorType::ChestMinecart))
        .def_property_readonly_static("CHICKEN", id(ActorType::Chicken))
        .def_property_readonly_static("COD", id(ActorType::Cod))
        .def_property_readonly_static("COMMAND_BLOCK_MINECART", id(ActorType::CommandBlockMinecart))
        .def_property_readonly_static("COPPER_GOLEM", id(ActorType::CopperGolem))
        .def_property_readonly_static("COW", id(ActorType::Cow))
        .def_property_readonly_static("CREAKING", id(ActorType::Creaking))
        .def_property_readonly_static("CREEPER", id(ActorType::Creeper))
        .def_property_readonly_static("DOLPHIN", id(ActorType::Dolphin))
        .def_property_readonly_static("DONKEY", id(ActorType::Donkey))
        .def_property_readonly_static("DRAGON_FIREBALL", id(ActorType::DragonFireball))
        .def_property_readonly_static("DROWNED", id(ActorType::Drowned))
        .def_property_readonly_static("EGG", id(ActorType::Egg))
        .def_property_readonly_static("ELDER_GUARDIAN", id(ActorType::ElderGuardian))
        .def_property_readonly_static("ELDER_GUARDIAN_GHOST", id(ActorType::ElderGuardianGhost))
        .def_property_readonly_static("ENDER_CRYSTAL", id(ActorType::EnderCrystal))
        .def_property_readonly_static("ENDER_DRAGON", id(ActorType::EnderDragon))
        .def_property_readonly_static("ENDER_PEARL", id(ActorType::EnderPearl))
        .def_property_readonly_static("ENDERMAN", id(ActorType::Enderman))
        .def_property_readonly_static("ENDERMITE", id(ActorType::Endermite))
        .def_property_readonly_static("EVOCATION_FANG", id(ActorType::EvocationFang))
        .def_property_readonly_static("EVOCATION_ILLAGER", id(ActorType::EvocationIllager))
        .def_property_readonly_static("EYE_OF_ENDER_SIGNAL", id(ActorType::EyeOfEnderSignal))
        .def_property_readonly_static("FALLING_BLOCK", id(ActorType::FallingBlock))
        .def_property_readonly_static("FIREBALL", id(ActorType::Fireball))
        .def_property_readonly_static("FIREWORKS_ROCKET", id(ActorType::FireworksRocket))
        .def_property_readonly_static("FISHING_HOOK", id(ActorType::FishingHook))
        .def_property_readonly_static("FOX", id(ActorType::Fox))
        .def_property_readonly_static("FROG", id(ActorType::Frog))
        .def_property_readonly_static("GHAST", id(ActorType::Ghast))
        .def_property_readonly_static("GLOW_SQUID", id(ActorType::GlowSquid))
        .def_property_readonly_static("GOAT", id(ActorType::Goat))
        .def_property_readonly_static("GUARDIAN", id(ActorType::Guardian))
        .def_property_readonly_static("HAPPY_GHAST", id(ActorType::HappyGhast))
        .def_property_readonly_static("HOGLIN", id(ActorType::Hoglin))
        .def_property_readonly_static("HOPPER_MINECART", id(ActorType::HopperMinecart))
        .def_property_readonly_static("HORSE", id(ActorType::Horse))
        .def_property_readonly_static("HUSK", id(ActorType::Husk))
        .def_property_readonly_static("ICE_BOMB", id(ActorType::IceBomb))
        .def_property_readonly_static("IRON_GOLEM", id(ActorType::IronGolem))
        .def_property_readonly_static("ITEM", id(ActorType::Item))
        .def_property_readonly_static("LEASH_KNOT", id(ActorType::LeashKnot))
        .def_property_readonly_static("LIGHTNING_BOLT", id(ActorType::LightningBolt))
        .def_property_readonly_static("LINGERING_POTION", id(ActorType::LingeringPotion))
        .def_property_readonly_static("LLAMA", id(ActorType::Llama))
        .def_property_readonly_static("LLAMA_SPIT", id(ActorType::LlamaSpit))
        .def_property_readonly_static("MAGMA_CUBE", id(ActorType::MagmaCube))
        .def_property_readonly_static("MINECART", id(ActorType::Minecart))
        .def_property_readonly_static("MOOSHROOM", id(ActorType::Mooshroom))
        .def_property_readonly_static("MULE", id(ActorType::Mule))
        .def_property_readonly_static("NAUTILUS", id(ActorType::Nautilus))
        .def_property_readonly_static("NPC", id(ActorType::Npc))
        .def_property_readonly_static("OCELOT", id(ActorType::Ocelot))
        .def_property_readonly_static("OMINOUS_ITEM_SPAWNER", id(ActorType::OminousItemSpawner))
        .def_property_readonly_static("PAINTING", id(ActorType::Painting))
        .def_property_readonly_static("PANDA", id(ActorType::Panda))
        .def_property_readonly_static("PARCHED", id(ActorType::Parched))
        .def_property_readonly_static("PARROT", id(ActorType::Parrot))
        .def_property_readonly_static("PHANTOM", id(ActorType::Phantom))
        .def_property_readonly_static("PIG", id(ActorType::Pig))
        .def_property_readonly_static("PIGLIN", id(ActorType::Piglin))
        .def_property_readonly_static("PIGLIN_BRUTE", id(ActorType::PiglinBrute))
        .def_property_readonly_static("PILLAGER", id(ActorType::Pillager))
        .def_property_readonly_static("PLAYER", id(ActorType::Player))
        .def_property_readonly_static("POLAR_BEAR", id(ActorType::PolarBear))
        .def_property_readonly_static("PUFFERFISH", id(ActorType::Pufferfish))
        .def_property_readonly_static("RABBIT", id(ActorType::Rabbit))
        .def_property_readonly_static("RAVAGER", id(ActorType::Ravager))
        .def_property_readonly_static("SALMON", id(ActorType::Salmon))
        .def_property_readonly_static("SHEEP", id(ActorType::Sheep))
        .def_property_readonly_static("SHULKER", id(ActorType::Shulker))
        .def_property_readonly_static("SHULKER_BULLET", id(ActorType::ShulkerBullet))
        .def_property_readonly_static("SILVERFISH", id(ActorType::Silverfish))
        .def_property_readonly_static("SKELETON", id(ActorType::Skeleton))
        .def_property_readonly_static("SKELETON_HORSE", id(ActorType::SkeletonHorse))
        .def_property_readonly_static("SLIME", id(ActorType::Slime))
        .def_property_readonly_static("SMALL_FIREBALL", id(ActorType::SmallFireball))
        .def_property_readonly_static("SNIFFER", id(ActorType::Sniffer))
        .def_property_readonly_static("SNOW_GOLEM", id(ActorType::SnowGolem))
        .def_property_readonly_static("SNOWBALL", id(ActorType::Snowball))
        .def_property_readonly_static("SPIDER", id(ActorType::Spider))
        .def_property_readonly_static("SPLASH_POTION", id(ActorType::SplashPotion))
        .def_property_readonly_static("SQUID", id(ActorType::Squid))
        .def_property_readonly_static("STRAY", id(ActorType::Stray))
        .def_property_readonly_static("STRIDER", id(ActorType::Strider))
        .def_property_readonly_static("SULFUR_CUBE", id(ActorType::SulfurCube))
        .def_property_readonly_static("TADPOLE", id(ActorType::Tadpole))
        .def_property_readonly_static("THROWN_TRIDENT", id(ActorType::ThrownTrident))
        .def_property_readonly_static("TNT", id(ActorType::Tnt))
        .def_property_readonly_static("TNT_MINECART", id(ActorType::TntMinecart))
        .def_property_readonly_static("TRADER_LLAMA", id(ActorType::TraderLlama))
        .def_property_readonly_static("TRIPOD_CAMERA", id(ActorType::TripodCamera))
        .def_property_readonly_static("TROPICALFISH", id(ActorType::Tropicalfish))
        .def_property_readonly_static("TURTLE", id(ActorType::Turtle))
        .def_property_readonly_static("VEX", id(ActorType::Vex))
        .def_property_readonly_static("VILLAGER", id(ActorType::Villager))
        .def_property_readonly_static("VILLAGER_V2", id(ActorType::VillagerV2))
        .def_property_readonly_static("VINDICATOR", id(ActorType::Vindicator))
        .def_property_readonly_static("WANDERING_TRADER", id(ActorType::WanderingTrader))
        .def_property_readonly_static("WARDEN", id(ActorType::Warden))
        .def_property_readonly_static("WIND_CHARGE_PROJECTILE", id(ActorType::WindChargeProjectile))
        .def_property_readonly_static("WITCH", id(ActorType::Witch))
        .def_property_readonly_static("WITHER", id(ActorType::Wither))
        .def_property_readonly_static("WITHER_SKELETON", id(ActorType::WitherSkeleton))
        .def_property_readonly_static("WITHER_SKULL", id(ActorType::WitherSkull))
        .def_property_readonly_static("WITHER_SKULL_DANGEROUS", id(ActorType::WitherSkullDangerous))
        .def_property_readonly_static("WOLF", id(ActorType::Wolf))
        .def_property_readonly_static("XP_BOTTLE", id(ActorType::XpBottle))
        .def_property_readonly_static("XP_ORB", id(ActorType::XpOrb))
        .def_property_readonly_static("ZOGLIN", id(ActorType::Zoglin))
        .def_property_readonly_static("ZOMBIE", id(ActorType::Zombie))
        .def_property_readonly_static("ZOMBIE_HORSE", id(ActorType::ZombieHorse))
        .def_property_readonly_static("ZOMBIE_NAUTILUS", id(ActorType::ZombieNautilus))
        .def_property_readonly_static("ZOMBIE_PIGMAN", id(ActorType::ZombiePigman))
        .def_property_readonly_static("ZOMBIE_VILLAGER", id(ActorType::ZombieVillager))
        .def_property_readonly_static("ZOMBIE_VILLAGER_V2", id(ActorType::ZombieVillagerV2))
        .def_property_readonly("id", &ActorType::getId, "The identifier of this actor type.")
        .def_property_readonly("translation_key", &ActorType::getTranslationKey,
                               "The translation key, suitable for use in a translation component.")
        .def_static("get", &ActorType::get, py::arg("name"), "Attempts to get the `ActorType` with the given name.",
                    py::return_value_policy::reference);

    actor.def_property_readonly("type", &Actor::getType, py::return_value_policy::reference, "The type of the actor.")
        .def_property_readonly("runtime_id", &Actor::getRuntimeId, "The runtime id for this actor.")
        .def_property_readonly("location", &Actor::getLocation,
                               "A new copy of `Location` containing the position of this actor.")
        .def_property_readonly("velocity", &Actor::getVelocity, "The current traveling velocity of this actor.")
        .def_property_readonly("is_on_ground", &Actor::isOnGround,
                               "`True` if the actor is supported by a block, i.e. on ground.")
        .def_property_readonly("is_in_water", &Actor::isInWater, "`True` if the actor is in water.")
        .def_property_readonly("is_in_lava", &Actor::isInLava, "`True` if the actor is in lava.")
        .def_property_readonly("level", &Actor::getLevel, "The current `Level` this actor resides in.",
                               py::return_value_policy::reference)
        .def_property_readonly("dimension", &Actor::getDimension, "The current `Dimension` this actor resides in.")
        .def("set_rotation", &Actor::setRotation, py::arg("yaw"), py::arg("pitch"), R"doc(
    Sets the actor's rotation.

    Note that if the actor is affected by AI, it may override this rotation.

    Args:
        yaw: Rotation around the up axis (Y axis).
        pitch: Rotation around the right axis (X axis).
)doc")
        .def("teleport", py::overload_cast<const Location &>(&Actor::teleport), py::arg("location"), R"doc(
    Teleports this actor to the given location.

    Args:
        location: New location to teleport this actor to.

    Returns:
        `True` if the teleport was successful.
)doc")
        .def("teleport", py::overload_cast<const NotNull<Actor> &>(&Actor::teleport), py::arg("target"), R"doc(
    Teleports this actor to the target `Actor`.

    Args:
        target: `Actor` to teleport this actor to.

    Returns:
        `True` if the teleport was successful.
)doc")
        .def_property_readonly("id", &Actor::getId, "A unique id for this actor.")
        .def("remove", &Actor::remove, R"doc(
    Remove this actor from the level.

    If you are trying to remove a `Player`, use `Player.kick` instead.
)doc")
        .def_property_readonly("is_valid", &Actor::isValid,
                               "`False` if the entity has died, been despawned for some other reason, or has not "
                               "been added to the level.")
        .def_property_readonly("is_dead", &Actor::isDead, "`True` if this actor has been marked for removal.")
        .def_property_readonly("scoreboard_tags", &Actor::getScoreboardTags,
                               "A list of scoreboard tags for this actor.")
        .def("add_scoreboard_tag", &Actor::addScoreboardTag, py::arg("tag"), R"doc(
    Adds a tag to this actor.

    Args:
        tag: The tag to add.

    Returns:
        `True` if the tag was successfully added, `False` if the tag already exists.
)doc")
        .def("remove_scoreboard_tag", &Actor::removeScoreboardTag, py::arg("tag"), R"doc(
    Removes a given tag from this actor.

    Args:
        tag: The tag to remove.

    Returns:
        `True` if the tag was successfully removed, `False` if the tag does not exist.
)doc")
        .def_property("is_name_tag_visible", &Actor::isNameTagVisible, &Actor::setNameTagVisible,
                      "Whether the actor's name tag is currently visible.")
        .def_property("is_name_tag_always_visible", &Actor::isNameTagAlwaysVisible, &Actor::setNameTagAlwaysVisible,
                      "Whether the actor's name tag is always visible.")
        .def_property("name_tag", &Actor::getNameTag, &Actor::setNameTag, "The current name tag of the actor.")
        .def_property("score_tag", &Actor::getScoreTag, &Actor::setScoreTag, "The current score tag of the actor.");

    mob.def_property_readonly("is_gliding", &Mob::isGliding,
                              "`True` if this actor is gliding, such as using an Elytra.")
        .def_property("health", &Mob::getHealth, &Mob::setHealth,
                      "The entity's health from 0 to its max possible value, where 0 is dead.")
        .def_property("max_health", &Mob::getMaxHealth, &Mob::setMaxHealth, R"doc(
    The maximum health this entity has.

    If the health of the entity is above the value provided, it will be set to that value. An entity
    with a health bar (e.g. `Player`, `EnderDragon`, `Wither`, etc.) will have their bar scaled
    accordingly.
)doc")
        .def("has_attribute", &Mob::hasAttribute, py::arg("attribute"),
             "Checks whether the given attribute is present on the object.")
        .def("get_attribute", &Mob::getAttribute, py::arg("attribute"),
             "Gets the specified attribute instance from the object. This instance will be backed directly to the "
             "object and any changes will be visible at once.")
        .def_property_readonly("attributes", &Mob::getAttributes,
                               "Gets all attribute instances from the object. This instance will be backed directly to "
                               "the object and any changes will be visible at once.")
        .def("add_effect", &Mob::addEffect, py::arg("effect"),
             "Adds the given potion effect to this entity. Only one effect of any given type may be active at any one "
             "time; an existing effect of the same type will be overwritten.")
        .def("remove_effect", &Mob::removeEffect, py::arg("type"),
             "Removes any effects of the given type that are present on this entity.")
        .def("has_effect", &Mob::hasEffect, py::arg("type"),
             "Returns whether the entity already has an existing effect of the given type applied to it.")
        .def("get_effect", &Mob::getEffect, py::arg("type"),
             "Returns the active potion effect of the specified type, or None if the effect is not present.")
        .def_property_readonly("active_effects", &Mob::getActiveEffects,
                               "Returns all currently active potion effects on this entity.");

    py_class<Item>(m, "Item", "Represents a dropped item that can be picked up by players.")
        .def_property("item_stack", &Item::getItemStack, &Item::setItemStack,
                      "The item stack associated with this item drop.")
        .def_property("pickup_delay", &Item::getPickupDelay, &Item::setPickupDelay,
                      "The delay before this `Item` is available to be picked up by players.")
        .def_property("is_unlimited_lifetime", &Item::isUnlimitedLifetime, &Item::setUnlimitedLifetime,
                      "Whether this `Item` lives forever.")
        .def_property("thrower", &Item::getThrower, &Item::setThrower,
                      "The thrower of this item (the entity which dropped the item), as a unique id.");
}

}  // namespace endstone::python
