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

#include "bedrock/core/string/string_hash.h"
#include "bedrock/resources/base_game_version.h"
#include "bedrock/util/new_type.h"

class GameRule {
public:
    enum class Type : uint8_t {
        Invalid = 0,
        Bool = 1,
        Int = 2,
        Float = 3,
    };

    union Value {
        bool bool_val;
        int int_val;
        float float_val;
        Value() = default;
        Value(const bool val) : bool_val(val) {}
        Value(const int val) : int_val(val) {}
        Value(const float val) : float_val(val) {}
        Value &operator=(const bool val)
        {
            bool_val = val;
            return *this;
        }
        Value &operator=(const int val)
        {
            int_val = val;
            return *this;
        }
        Value &operator=(const float val)
        {
            float_val = val;
            return *this;
        }
    };

    using TagDataNotFoundCallback = std::function<void(GameRule &, const BaseGameVersion &)>;
    using ValidateValueCallback = std::function<bool(const Value &, class ValidationError *)>;

    GameRule();
    bool getBool() const
    {
        return value_.bool_val;
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
};
BEDROCK_STATIC_ASSERT_SIZE(GameRule, 176, 144);

struct GameRuleId : NewType<int> {
    GameRuleId() = default;
    explicit GameRuleId(const int id)
    {
        value = id;
    }
};

class GameRules : public Bedrock::EnableNonOwnerReferences {
    using GameRuleMap = std::vector<GameRule>;
    using WorldPolicyMap = std::map<HashedString, GameRule>;

public:
    bool getBool(GameRuleId id, bool default_value) const
    {
        if (id >= 0 && id < game_rules_.size()) {
            return game_rules_[id].getBool();
        }
        return default_value;
    }

    enum GameRulesIndex : int {
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
        SHOW_DAYS_PLAYED = 17,
        DO_NATURAL_REGENERATION = 18,
        DO_TNT_EXPLODE = 19,
        SEND_COMMAND_FEEDBACK = 20,
        MAX_COMMAND_CHAIN_LENGTH = 21,
        DO_INSOMNIA = 22,
        COMMAND_BLOCKS_ENABLED = 23,
        RANDOM_TICK_SPEED = 24,
        DO_IMMEDIATE_RESPAWN = 25,
        SHOW_DEATH_MESSAGES = 26,
        FUNCTION_COMMAND_LIMIT = 27,
        PLAYER_SPAWN_RADIUS = 28,
        SHOW_TAGS = 29,
        FREEZE_DAMAGE = 30,
        RESPAWN_BLOCKS_EXPLODE = 31,
        SHOW_BORDER_EFFECT = 32,
        SHOW_RECIPE_MESSAGES = 33,
        PLAYER_SLEEPING_PERCENTAGE = 34,
        PROJECTILES_CAN_BREAK_BLOCKS = 35,
        TNT_EXPLOSION_DROP_DECAY = 36,
        VANILLA_GAME_RULE_COUNT = 37,
        GLOBAL_MUTE = 37,
        ALLOW_DESTRUCTIVE_OBJECTS = 38,
        ALLOW_MOBS = 39,
        CODE_BUILDER = 40,
        EDU_CLOUD_SAVE = 41,
        EDU_GAME_RULE_COUNT = 42,
        GAME_RULE_COUNT = 42,
    };

private:
    GameRuleMap game_rules_;
    WorldPolicyMap world_policies_;
};
