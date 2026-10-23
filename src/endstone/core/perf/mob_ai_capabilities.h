#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <string_view>

#include "bedrock/world/actor/actor_types.h"
#include "endstone/core/perf/mob_ai.h"

namespace endstone::core {

inline bool supportsMobAiIdle(std::string_view identifier, ActorType type)
{
    constexpr std::array excluded{"minecraft:player",         "minecraft:armor_stand",
                                  "minecraft:tripod_camera",  "minecraft:npc",
                                  "minecraft:agent",          "minecraft:villager",
                                  "minecraft:villager_v2",    "minecraft:wandering_trader",
                                  "minecraft:trader_llama",   "minecraft:ender_dragon",
                                  "minecraft:wither",         "minecraft:warden",
                                  "minecraft:creaking",       "minecraft:allay",
                                  "minecraft:copper_golem",   "minecraft:iron_golem",
                                  "minecraft:snow_golem",     "minecraft:guardian",
                                  "minecraft:elder_guardian", "minecraft:elder_guardian_ghost",
                                  "minecraft:shulker"};
    return identifier.starts_with("minecraft:") && (type & ActorType::Mob) == ActorType::Mob &&
           type != ActorType::Player && std::find(excluded.begin(), excluded.end(), identifier) == excluded.end();
}

inline bool supportsMobAiSearch(std::string_view identifier, ActorType type, const MobAiDecision &decision)
{
    if (!decision.target_search_interval_ticks || !decision.target_search_player_range_blocks ||
        !std::isfinite(*decision.target_search_player_range_blocks) ||
        *decision.target_search_player_range_blocks <= 0 || (type & ActorType::Monster) != ActorType::Monster) {
        return false;
    }
    constexpr std::array allowed{"minecraft:zombie",
                                 "minecraft:husk",
                                 "minecraft:drowned",
                                 "minecraft:zombie_villager",
                                 "minecraft:zombie_villager_v2",
                                 "minecraft:skeleton",
                                 "minecraft:stray",
                                 "minecraft:wither_skeleton",
                                 "minecraft:bogged",
                                 "minecraft:parched",
                                 "minecraft:creeper",
                                 "minecraft:witch"};
    return std::find(allowed.begin(), allowed.end(), identifier) != allowed.end();
}

inline bool hasMobAiActivityCapability(std::string_view identifier, ActorType type, const MobAiDecision &decision)
{
    return (decision.animal && (decision.animal->random_stroll || decision.animal->random_look_around) &&
            supportsMobAiIdle(identifier, type)) ||
           supportsMobAiSearch(identifier, type, decision);
}

inline std::optional<bool> mobAiWithinRange(const std::array<double, 3> &position,
                                               const std::array<double, 3> &other, double range)
{
    if (!std::isfinite(range) || range <= 0 ||
        !std::all_of(position.begin(), position.end(), [](double value) { return std::isfinite(value); }) ||
        !std::all_of(other.begin(), other.end(), [](double value) { return std::isfinite(value); })) {
        return std::nullopt;
    }
    return std::hypot(other[0] - position[0], other[1] - position[1], other[2] - position[2]) <= range;
}

}  // namespace endstone::core
