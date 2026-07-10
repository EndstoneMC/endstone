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

#include <functional>
#include <map>
#include <optional>
#include <variant>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/util/new_type.h"

class CompoundTag;

class GameRule {
public:
    enum class Type : uint8_t {
        Invalid = 0,
        Bool = 1,
        Int = 2,
        Float = 3,
    };

    using Value = std::variant<std::monostate, bool, int, float>;

    using TagDataNotFoundCallback = std::function<void(GameRule &, const BaseGameVersion &, const CompoundTag &)>;
    using ValidateValueCallback = std::function<bool(const Value &, class ValidationError *)>;
    using CommandValueRedirectCallback = std::function<Value(const Value &)>;

    GameRule();
    [[nodiscard]] bool getBool() const
    {
        const auto *value = std::get_if<bool>(&value_);
        return value != nullptr && *value;
    }

private:
    bool should_save_;
    Type type_;
    Value value_;
    std::string name_;
    bool allow_use_in_command_;
    bool allow_use_in_scripting_;
    bool is_default_set_;
    bool requires_cheat_;
    bool can_be_modified_by_player_;
    TagDataNotFoundCallback tag_not_found_callback_;
    ValidateValueCallback validate_value_callback_;
    std::map<std::string, int> command_enum_names_;
    int min_command_version_;
    int max_command_version_;
    CommandValueRedirectCallback command_value_redirect_converter_;
    std::optional<int> command_value_redirect_target_;
};
BEDROCK_STATIC_ASSERT_SIZE(GameRule, 280, 240);

struct GameRuleId : NewType<int> {
    GameRuleId() = default;
    explicit GameRuleId(const int id) { value = id; }
};

class GameRules : public Bedrock::EnableNonOwnerReferences {
    using GameRuleMap = std::vector<GameRule>;
    using WorldPolicyMap = std::map<HashedString, GameRule>;

public:
    [[nodiscard]] bool getBool(GameRuleId id, bool default_value) const
    {
        if (id >= 0 && id < game_rules_.size()) {
            return game_rules_[id].getBool();
        }
        return default_value;
    }

    enum GameRulesIndex : int {  // NOLINTBEGIN
        INVALID_GAME_RULE = -1,
        COMMAND_BLOCK_OUTPUT = 0,
        DO_DAYLIGHT_CYCLE = 1,
        DO_ENTITY_DROPS = 2,
        DO_FIRE_TICK = 3,
        RECIPES_UNLOCK = 4,
        DO_LIMITED_CRAFTING = 5,
        DO_MOB_LOOT = 6,
        DO_MOB_SPAWNING = 7,
        DO_TILE_DROPS = 8,
        DO_WEATHER_CYCLE = 9,
        DROWNING_DAMAGE = 10,
        FALL_DAMAGE = 11,
        FIRE_DAMAGE = 12,
        KEEP_INVENTORY = 13,
        MOB_GRIEFING = 14,
        PVP = 15,
        SHOW_COORDINATES = 16,
        PLAYER_WAYPOINTS = 17,
        LOCATOR_BAR_DEPRECATED = 18,
        SHOW_DAYS_PLAYED = 19,
        DO_NATURAL_REGENERATION = 20,
        DO_TNT_EXPLODE = 21,
        SEND_COMMAND_FEEDBACK = 22,
        MAX_COMMAND_CHAIN_LENGTH = 23,
        DO_INSOMNIA = 24,
        COMMAND_BLOCKS_ENABLED = 25,
        RANDOM_TICK_SPEED = 26,
        DO_IMMEDIATE_RESPAWN = 27,
        SHOW_DEATH_MESSAGES = 28,
        FUNCTION_COMMAND_LIMIT = 29,
        PLAYER_SPAWN_RADIUS = 30,
        SHOW_TAGS = 31,
        FREEZE_DAMAGE = 32,
        RESPAWN_BLOCKS_EXPLODE = 33,
        SHOW_BORDER_EFFECT = 34,
        SHOW_RECIPE_MESSAGES = 35,
        PLAYER_SLEEPING_PERCENTAGE = 36,
        PROJECTILES_CAN_BREAK_BLOCKS = 37,
        TNT_EXPLOSION_DROP_DECAY = 38,
        VANILLA_GAME_RULE_COUNT = 39,
        GLOBAL_MUTE = 39,
        ALLOW_DESTRUCTIVE_OBJECTS = 40,
        ALLOW_MOBS = 41,
        CODE_BUILDER = 42,
        EDU_CLOUD_SAVE = 43,
        EDU_GAME_RULE_COUNT = 44,
        GAME_RULE_COUNT = 44,
    };  // NOLINTEND

private:
    GameRuleMap game_rules_;
    WorldPolicyMap world_policies_;
    Bedrock::PubSub::Publisher<void(const GameRules &, const GameRuleId &), Bedrock::PubSub::ThreadModel::MultiThreaded>
        game_rule_change_publisher_;
};
