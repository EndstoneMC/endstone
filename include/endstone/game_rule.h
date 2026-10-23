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

#include <type_traits>
#include <variant>

#include "endstone/registry.h"

namespace endstone {

using GameRuleValue = std::variant<bool, int, float>;

class GameRule;

/**
 * Represents the identifier of a game rule, carrying the type of the rule's value.
 */
template <typename T>
class GameRuleId : public Identifier<GameRule> {
public:
    using Identifier::Identifier;

    static constexpr GameRuleId minecraft(const std::string_view key) noexcept { return {Minecraft, key}; }
};

/**
 * Represents a game rule.
 */
class GameRule : public Registry<GameRule>::Type {
public:
    /** Whether command blocks notify administrators when they execute commands. */
    static constexpr auto CommandBlockOutput = GameRuleId<bool>::minecraft("commandblockoutput");
    /** Whether command blocks can execute commands. */
    static constexpr auto CommandBlocksEnabled = GameRuleId<bool>::minecraft("commandblocksenabled");
    /** Whether the day and night cycles progress. */
    static constexpr auto DoDaylightCycle = GameRuleId<bool>::minecraft("dodaylightcycle");
    /** Whether non-mob entities drop items. */
    static constexpr auto DoEntityDrops = GameRuleId<bool>::minecraft("doentitydrops");
    /** Whether fire spreads. */
    static constexpr auto DoFireTick = GameRuleId<bool>::minecraft("dofiretick");
    /** Whether players immediately respawn. */
    static constexpr auto DoImmediateRespawn = GameRuleId<bool>::minecraft("doimmediaterespawn");
    /** Whether players experience insomnia. */
    static constexpr auto DoInsomnia = GameRuleId<bool>::minecraft("doinsomnia");
    /** Whether players can craft only unlocked recipes. */
    static constexpr auto DoLimitedCrafting = GameRuleId<bool>::minecraft("dolimitedcrafting");
    /** Whether mobs drop loot. */
    static constexpr auto DoMobLoot = GameRuleId<bool>::minecraft("domobloot");
    /** Whether mobs spawn naturally. */
    static constexpr auto DoMobSpawning = GameRuleId<bool>::minecraft("domobspawning");
    /** Whether blocks drop items when destroyed. */
    static constexpr auto DoTileDrops = GameRuleId<bool>::minecraft("dotiledrops");
    /** Whether the weather changes naturally. */
    static constexpr auto DoWeatherCycle = GameRuleId<bool>::minecraft("doweathercycle");
    /** Whether entities take drowning damage. */
    static constexpr auto DrowningDamage = GameRuleId<bool>::minecraft("drowningdamage");
    /** Whether entities take fall damage. */
    static constexpr auto FallDamage = GameRuleId<bool>::minecraft("falldamage");
    /** Whether entities take fire damage. */
    static constexpr auto FireDamage = GameRuleId<bool>::minecraft("firedamage");
    /** Whether entities take freezing damage. */
    static constexpr auto FreezeDamage = GameRuleId<bool>::minecraft("freezedamage");
    /** The maximum number of commands executed by a function per tick. */
    static constexpr auto FunctionCommandLimit = GameRuleId<int>::minecraft("functioncommandlimit");
    /** Whether players keep their inventory on death. */
    static constexpr auto KeepInventory = GameRuleId<bool>::minecraft("keepinventory");
    /** Whether the locator bar is displayed. */
    static constexpr auto LocatorBar = GameRuleId<bool>::minecraft("locatorbar");
    /** The maximum number of chained commands executed per tick. */
    static constexpr auto MaxCommandChainLength = GameRuleId<int>::minecraft("maxcommandchainlength");
    /** Whether mobs can modify the world. */
    static constexpr auto MobGriefing = GameRuleId<bool>::minecraft("mobgriefing");
    /** Whether players regenerate health naturally. */
    static constexpr auto NaturalRegeneration = GameRuleId<bool>::minecraft("naturalregeneration");
    /** The percentage of players that must sleep to skip the night. */
    static constexpr auto PlayersSleepingPercentage = GameRuleId<int>::minecraft("playerssleepingpercentage");
    /** Whether player waypoints are added to locator bars. */
    static constexpr auto PlayerWaypoints = GameRuleId<int>::minecraft("playerwaypoints");
    /** Whether projectiles can break supported blocks. */
    static constexpr auto ProjectilesCanBreakBlocks = GameRuleId<bool>::minecraft("projectilescanbreakblocks");
    /** Whether players can damage each other. */
    static constexpr auto Pvp = GameRuleId<bool>::minecraft("pvp");
    /** The frequency of random block ticks. */
    static constexpr auto RandomTickSpeed = GameRuleId<int>::minecraft("randomtickspeed");
    /** Whether built-in recipes unlock as players progress. */
    static constexpr auto RecipesUnlock = GameRuleId<bool>::minecraft("recipesunlock");
    /** Whether respawn blocks explode in other dimensions. */
    static constexpr auto RespawnBlocksExplode = GameRuleId<bool>::minecraft("respawnblocksexplode");
    /** Whether command feedback is displayed to players. */
    static constexpr auto SendCommandFeedback = GameRuleId<bool>::minecraft("sendcommandfeedback");
    /** Whether border block effects are shown. */
    static constexpr auto ShowBorderEffect = GameRuleId<bool>::minecraft("showbordereffect");
    /** Whether player coordinates are displayed. */
    static constexpr auto ShowCoordinates = GameRuleId<bool>::minecraft("showcoordinates");
    /** Whether the number of days played is displayed. */
    static constexpr auto ShowDaysPlayed = GameRuleId<bool>::minecraft("showdaysplayed");
    /** Whether death messages are displayed in chat. */
    static constexpr auto ShowDeathMessages = GameRuleId<bool>::minecraft("showdeathmessages");
    /** Whether recipe unlock notifications are shown. */
    static constexpr auto ShowRecipeMessages = GameRuleId<bool>::minecraft("showrecipemessages");
    /** Whether item tags are shown. */
    static constexpr auto ShowTags = GameRuleId<bool>::minecraft("showtags");
    /** The radius around world spawn in which players can spawn. */
    static constexpr auto SpawnRadius = GameRuleId<int>::minecraft("spawnradius");
    /** Whether TNT blocks can be lit. */
    static constexpr auto TntExplodes = GameRuleId<bool>::minecraft("tntexplodes");
    /** Whether explosion drop decay is enabled for TNT. */
    static constexpr auto TntExplosionDropDecay = GameRuleId<bool>::minecraft("tntexplosiondropdecay");
};

static_assert(std::is_same_v<decltype(GameRule::DoFireTick), const GameRuleId<bool>>);
static_assert(std::is_same_v<decltype(GameRule::PlayerWaypoints), const GameRuleId<int>>);
static_assert(std::is_same_v<decltype(GameRule::RandomTickSpeed), const GameRuleId<int>>);

}  // namespace endstone
