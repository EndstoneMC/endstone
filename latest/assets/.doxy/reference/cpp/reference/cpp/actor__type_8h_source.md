

# File actor\_type.h

[**File List**](files.md) **>** [**actor**](dir_dd7779a583e02d88c9a89a2c881c3946.md) **>** [**actor\_type.h**](actor__type_8h.md)

[Go to the documentation of this file](actor__type_8h.md)


```C++
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

#pragma once

#include "endstone/registry.h"

namespace endstone {
class ActorType;
using ActorTypeId = Identifier<ActorType>;
class ActorType : public Registry<ActorType>::Type {
public:
    // Passive mobs
    static constexpr auto Allay = ActorTypeId::minecraft("allay");
    static constexpr auto Armadillo = ActorTypeId::minecraft("armadillo");
    static constexpr auto Axolotl = ActorTypeId::minecraft("axolotl");
    static constexpr auto Bat = ActorTypeId::minecraft("bat");
    static constexpr auto Bee = ActorTypeId::minecraft("bee");
    static constexpr auto Camel = ActorTypeId::minecraft("camel");
    static constexpr auto CamelHusk = ActorTypeId::minecraft("camel_husk");
    static constexpr auto Cat = ActorTypeId::minecraft("cat");
    static constexpr auto Chicken = ActorTypeId::minecraft("chicken");
    static constexpr auto Cod = ActorTypeId::minecraft("cod");
    static constexpr auto CopperGolem = ActorTypeId::minecraft("copper_golem");
    static constexpr auto Cow = ActorTypeId::minecraft("cow");
    static constexpr auto Dolphin = ActorTypeId::minecraft("dolphin");
    static constexpr auto Donkey = ActorTypeId::minecraft("donkey");
    static constexpr auto Fox = ActorTypeId::minecraft("fox");
    static constexpr auto Frog = ActorTypeId::minecraft("frog");
    static constexpr auto GlowSquid = ActorTypeId::minecraft("glow_squid");
    static constexpr auto Goat = ActorTypeId::minecraft("goat");
    static constexpr auto HappyGhast = ActorTypeId::minecraft("happy_ghast");
    static constexpr auto Hoglin = ActorTypeId::minecraft("hoglin");
    static constexpr auto Horse = ActorTypeId::minecraft("horse");
    static constexpr auto IronGolem = ActorTypeId::minecraft("iron_golem");
    static constexpr auto Llama = ActorTypeId::minecraft("llama");
    static constexpr auto Mooshroom = ActorTypeId::minecraft("mooshroom");
    static constexpr auto Mule = ActorTypeId::minecraft("mule");
    static constexpr auto Nautilus = ActorTypeId::minecraft("nautilus");
    static constexpr auto Ocelot = ActorTypeId::minecraft("ocelot");
    static constexpr auto Panda = ActorTypeId::minecraft("panda");
    static constexpr auto Parrot = ActorTypeId::minecraft("parrot");
    static constexpr auto Pig = ActorTypeId::minecraft("pig");
    static constexpr auto PolarBear = ActorTypeId::minecraft("polar_bear");
    static constexpr auto Pufferfish = ActorTypeId::minecraft("pufferfish");
    static constexpr auto Rabbit = ActorTypeId::minecraft("rabbit");
    static constexpr auto Salmon = ActorTypeId::minecraft("salmon");
    static constexpr auto Sheep = ActorTypeId::minecraft("sheep");
    static constexpr auto SkeletonHorse = ActorTypeId::minecraft("skeleton_horse");
    static constexpr auto Sniffer = ActorTypeId::minecraft("sniffer");
    static constexpr auto SnowGolem = ActorTypeId::minecraft("snow_golem");
    static constexpr auto Squid = ActorTypeId::minecraft("squid");
    static constexpr auto Strider = ActorTypeId::minecraft("strider");
    static constexpr auto Tadpole = ActorTypeId::minecraft("tadpole");
    static constexpr auto TraderLlama = ActorTypeId::minecraft("trader_llama");
    static constexpr auto Tropicalfish = ActorTypeId::minecraft("tropicalfish");
    static constexpr auto Turtle = ActorTypeId::minecraft("turtle");
    static constexpr auto WanderingTrader = ActorTypeId::minecraft("wandering_trader");
    static constexpr auto Wolf = ActorTypeId::minecraft("wolf");
    static constexpr auto ZombieHorse = ActorTypeId::minecraft("zombie_horse");

    // Hostile mobs
    static constexpr auto Blaze = ActorTypeId::minecraft("blaze");
    static constexpr auto Bogged = ActorTypeId::minecraft("bogged");
    static constexpr auto Breeze = ActorTypeId::minecraft("breeze");
    static constexpr auto CaveSpider = ActorTypeId::minecraft("cave_spider");
    static constexpr auto Creaking = ActorTypeId::minecraft("creaking");
    static constexpr auto Creeper = ActorTypeId::minecraft("creeper");
    static constexpr auto Drowned = ActorTypeId::minecraft("drowned");
    static constexpr auto ElderGuardian = ActorTypeId::minecraft("elder_guardian");
    static constexpr auto ElderGuardianGhost = ActorTypeId::minecraft("elder_guardian_ghost");
    static constexpr auto Enderman = ActorTypeId::minecraft("enderman");
    static constexpr auto Endermite = ActorTypeId::minecraft("endermite");
    static constexpr auto EvocationIllager = ActorTypeId::minecraft("evocation_illager");
    static constexpr auto Ghast = ActorTypeId::minecraft("ghast");
    static constexpr auto Guardian = ActorTypeId::minecraft("guardian");
    static constexpr auto Husk = ActorTypeId::minecraft("husk");
    static constexpr auto MagmaCube = ActorTypeId::minecraft("magma_cube");
    static constexpr auto Parched = ActorTypeId::minecraft("parched");
    static constexpr auto Phantom = ActorTypeId::minecraft("phantom");
    static constexpr auto Piglin = ActorTypeId::minecraft("piglin");
    static constexpr auto PiglinBrute = ActorTypeId::minecraft("piglin_brute");
    static constexpr auto Pillager = ActorTypeId::minecraft("pillager");
    static constexpr auto Ravager = ActorTypeId::minecraft("ravager");
    static constexpr auto Shulker = ActorTypeId::minecraft("shulker");
    static constexpr auto Silverfish = ActorTypeId::minecraft("silverfish");
    static constexpr auto Skeleton = ActorTypeId::minecraft("skeleton");
    static constexpr auto Slime = ActorTypeId::minecraft("slime");
    static constexpr auto Spider = ActorTypeId::minecraft("spider");
    static constexpr auto Stray = ActorTypeId::minecraft("stray");
    static constexpr auto Vex = ActorTypeId::minecraft("vex");
    static constexpr auto Vindicator = ActorTypeId::minecraft("vindicator");
    static constexpr auto Warden = ActorTypeId::minecraft("warden");
    static constexpr auto Witch = ActorTypeId::minecraft("witch");
    static constexpr auto Wither = ActorTypeId::minecraft("wither");
    static constexpr auto WitherSkeleton = ActorTypeId::minecraft("wither_skeleton");
    static constexpr auto Zoglin = ActorTypeId::minecraft("zoglin");
    static constexpr auto Zombie = ActorTypeId::minecraft("zombie");
    static constexpr auto ZombieNautilus = ActorTypeId::minecraft("zombie_nautilus");
    static constexpr auto ZombiePigman = ActorTypeId::minecraft("zombie_pigman");
    static constexpr auto ZombieVillager = ActorTypeId::minecraft("zombie_villager");
    static constexpr auto ZombieVillagerV2 = ActorTypeId::minecraft("zombie_villager_v2");

    // Villagers
    static constexpr auto Villager = ActorTypeId::minecraft("villager");
    static constexpr auto VillagerV2 = ActorTypeId::minecraft("villager_v2");

    // Special mobs
    static constexpr auto Agent = ActorTypeId::minecraft("agent");
    static constexpr auto ArmorStand = ActorTypeId::minecraft("armor_stand");
    static constexpr auto EnderDragon = ActorTypeId::minecraft("ender_dragon");
    static constexpr auto Npc = ActorTypeId::minecraft("npc");
    static constexpr auto Player = ActorTypeId::minecraft("player");
    static constexpr auto TripodCamera = ActorTypeId::minecraft("tripod_camera");

    // Non-living entities
    static constexpr auto AreaEffectCloud = ActorTypeId::minecraft("area_effect_cloud");
    static constexpr auto Arrow = ActorTypeId::minecraft("arrow");
    static constexpr auto Balloon = ActorTypeId::minecraft("balloon");
    static constexpr auto Boat = ActorTypeId::minecraft("boat");
    static constexpr auto BreezeWindChargeProjectile = ActorTypeId::minecraft("breeze_wind_charge_projectile");
    static constexpr auto ChestBoat = ActorTypeId::minecraft("chest_boat");
    static constexpr auto ChestMinecart = ActorTypeId::minecraft("chest_minecart");
    static constexpr auto CommandBlockMinecart = ActorTypeId::minecraft("command_block_minecart");
    static constexpr auto DragonFireball = ActorTypeId::minecraft("dragon_fireball");
    static constexpr auto Egg = ActorTypeId::minecraft("egg");
    static constexpr auto EnderCrystal = ActorTypeId::minecraft("ender_crystal");
    static constexpr auto EnderPearl = ActorTypeId::minecraft("ender_pearl");
    static constexpr auto EvocationFang = ActorTypeId::minecraft("evocation_fang");
    static constexpr auto EyeOfEnderSignal = ActorTypeId::minecraft("eye_of_ender_signal");
    static constexpr auto FallingBlock = ActorTypeId::minecraft("falling_block");
    static constexpr auto Fireball = ActorTypeId::minecraft("fireball");
    static constexpr auto FireworksRocket = ActorTypeId::minecraft("fireworks_rocket");
    static constexpr auto FishingHook = ActorTypeId::minecraft("fishing_hook");
    static constexpr auto HopperMinecart = ActorTypeId::minecraft("hopper_minecart");
    static constexpr auto IceBomb = ActorTypeId::minecraft("ice_bomb");
    static constexpr auto Item = ActorTypeId::minecraft("item");
    static constexpr auto LeashKnot = ActorTypeId::minecraft("leash_knot");
    static constexpr auto LightningBolt = ActorTypeId::minecraft("lightning_bolt");
    static constexpr auto LingeringPotion = ActorTypeId::minecraft("lingering_potion");
    static constexpr auto LlamaSpit = ActorTypeId::minecraft("llama_spit");
    static constexpr auto Minecart = ActorTypeId::minecraft("minecart");
    static constexpr auto OminousItemSpawner = ActorTypeId::minecraft("ominous_item_spawner");
    static constexpr auto Painting = ActorTypeId::minecraft("painting");
    static constexpr auto ShulkerBullet = ActorTypeId::minecraft("shulker_bullet");
    static constexpr auto SmallFireball = ActorTypeId::minecraft("small_fireball");
    static constexpr auto Snowball = ActorTypeId::minecraft("snowball");
    static constexpr auto SplashPotion = ActorTypeId::minecraft("splash_potion");
    static constexpr auto ThrownTrident = ActorTypeId::minecraft("thrown_trident");
    static constexpr auto Tnt = ActorTypeId::minecraft("tnt");
    static constexpr auto TntMinecart = ActorTypeId::minecraft("tnt_minecart");
    static constexpr auto WindChargeProjectile = ActorTypeId::minecraft("wind_charge_projectile");
    static constexpr auto WitherSkull = ActorTypeId::minecraft("wither_skull");
    static constexpr auto WitherSkullDangerous = ActorTypeId::minecraft("wither_skull_dangerous");
    static constexpr auto XpBottle = ActorTypeId::minecraft("xp_bottle");
    static constexpr auto XpOrb = ActorTypeId::minecraft("xp_orb");
};
}  // namespace endstone
```


