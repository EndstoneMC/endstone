

# File game\_rule.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**game\_rule.h**](game__rule_8h.md)

[Go to the documentation of this file](game__rule_8h.md)


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

#include <type_traits>
#include <variant>

#include "endstone/registry.h"

namespace endstone {

using GameRuleValue = std::variant<bool, int, float>;

class GameRule;

template <typename T>
class GameRuleId : public Identifier<GameRule> {
public:
    using Identifier::Identifier;

    static constexpr GameRuleId minecraft(const std::string_view key) noexcept { return {Minecraft, key}; }
};

class GameRule : public Registry<GameRule>::Type {
public:
    static constexpr auto CommandBlockOutput = GameRuleId<bool>::minecraft("commandblockoutput");
    static constexpr auto CommandBlocksEnabled = GameRuleId<bool>::minecraft("commandblocksenabled");
    static constexpr auto DoDayLightCycle = GameRuleId<bool>::minecraft("dodaylightcycle");
    static constexpr auto DoEntityDrops = GameRuleId<bool>::minecraft("doentitydrops");
    static constexpr auto DoFireTick = GameRuleId<bool>::minecraft("dofiretick");
    static constexpr auto DoImmediateRespawn = GameRuleId<bool>::minecraft("doimmediaterespawn");
    static constexpr auto DoInsomnia = GameRuleId<bool>::minecraft("doinsomnia");
    static constexpr auto DoLimitedCrafting = GameRuleId<bool>::minecraft("dolimitedcrafting");
    static constexpr auto DoMobLoot = GameRuleId<bool>::minecraft("domobloot");
    static constexpr auto DoMobSpawning = GameRuleId<bool>::minecraft("domobspawning");
    static constexpr auto DoTileDrops = GameRuleId<bool>::minecraft("dotiledrops");
    static constexpr auto DoWeatherCycle = GameRuleId<bool>::minecraft("doweathercycle");
    static constexpr auto DrowningDamage = GameRuleId<bool>::minecraft("drowningdamage");
    static constexpr auto FallDamage = GameRuleId<bool>::minecraft("falldamage");
    static constexpr auto FireDamage = GameRuleId<bool>::minecraft("firedamage");
    static constexpr auto FreezeDamage = GameRuleId<bool>::minecraft("freezedamage");
    static constexpr auto FunctionCommandLimit = GameRuleId<int>::minecraft("functioncommandlimit");
    static constexpr auto KeepInventory = GameRuleId<bool>::minecraft("keepinventory");
    static constexpr auto LocatorBar = GameRuleId<bool>::minecraft("locatorbar");
    static constexpr auto MaxCommandChainLength = GameRuleId<int>::minecraft("maxcommandchainlength");
    static constexpr auto MobGriefing = GameRuleId<bool>::minecraft("mobgriefing");
    static constexpr auto NaturalRegeneration = GameRuleId<bool>::minecraft("naturalregeneration");
    static constexpr auto PlayersSleepingPercentage = GameRuleId<int>::minecraft("playerssleepingpercentage");
    static constexpr auto PlayerWaypoints = GameRuleId<int>::minecraft("playerwaypoints");
    static constexpr auto ProjectilesCanBreakBlocks = GameRuleId<bool>::minecraft("projectilescanbreakblocks");
    static constexpr auto Pvp = GameRuleId<bool>::minecraft("pvp");
    static constexpr auto RandomTickSpeed = GameRuleId<int>::minecraft("randomtickspeed");
    static constexpr auto RecipesUnlock = GameRuleId<bool>::minecraft("recipesunlock");
    static constexpr auto RespawnBlocksExplode = GameRuleId<bool>::minecraft("respawnblocksexplode");
    static constexpr auto SendCommandFeedback = GameRuleId<bool>::minecraft("sendcommandfeedback");
    static constexpr auto ShowBorderEffect = GameRuleId<bool>::minecraft("showbordereffect");
    static constexpr auto ShowCoordinates = GameRuleId<bool>::minecraft("showcoordinates");
    static constexpr auto ShowDaysPlayed = GameRuleId<bool>::minecraft("showdaysplayed");
    static constexpr auto ShowDeathMessages = GameRuleId<bool>::minecraft("showdeathmessages");
    static constexpr auto ShowRecipeMessages = GameRuleId<bool>::minecraft("showrecipemessages");
    static constexpr auto ShowTags = GameRuleId<bool>::minecraft("showtags");
    static constexpr auto SpawnRadius = GameRuleId<int>::minecraft("spawnradius");
    static constexpr auto TntExplodes = GameRuleId<bool>::minecraft("tntexplodes");
    static constexpr auto TntExplosionDropDecay = GameRuleId<bool>::minecraft("tntexplosiondropdecay");
};

static_assert(std::is_same_v<decltype(GameRule::DoFireTick), const GameRuleId<bool>>);
static_assert(std::is_same_v<decltype(GameRule::PlayerWaypoints), const GameRuleId<int>>);
static_assert(std::is_same_v<decltype(GameRule::RandomTickSpeed), const GameRuleId<int>>);

}  // namespace endstone
```


