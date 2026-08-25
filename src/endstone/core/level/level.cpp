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

#include "endstone/core/level/level.h"

#include <cstdlib>
#include <ranges>
#include <stdexcept>

#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/dimension_manager.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/storage/game_rule_utils.h"
#include "bedrock/world/level/storage/game_rules.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/game_rule.h"
#include "endstone/core/inventory/recipe_data.h"
#include "endstone/core/level/dimension.h"
#include "endstone/event/level/dimension_load_event.h"
#include "endstone/level/dimension.h"
#include "endstone/plugin/plugin_manager.h"

namespace endstone::core {

EndstoneLevel::EndstoneLevel(::Level &level) : server_(EndstoneServer::getInstance()), level_(level) {}

void EndstoneLevel::loadDimensions()
{
    level_.getDimensionManager().getOnNewDimensionCreatedConnector().connect(
        [&](::Dimension &dimension) { addDimension(dimension); }, Bedrock::PubSub::ConnectPosition::AtBack, nullptr);
    level_.forEachDimension([&](::Dimension &dimension) {
        addDimension(dimension);
        return true;
    });
    // Load all vanilla dimensions on start up
    level_.getOrCreateDimension(VanillaDimensions::Overworld);
    level_.getOrCreateDimension(VanillaDimensions::Nether);
    level_.getOrCreateDimension(VanillaDimensions::TheEnd);
}

void EndstoneLevel::addDimension(::Dimension &dimension)
{
    const auto handle = std::make_shared<EndstoneDimension>(dimension.getWeakRef(), *this);
    dimensions_[dimension.getDimensionId().value] = handle;
    DimensionLoadEvent event{handle};
    server_.getPluginManager().callEvent(event);
}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
}

std::vector<NotNull<Actor>> EndstoneLevel::getActors() const
{
    std::vector<NotNull<Actor>> result;
    for (const auto *entities : {&level_.getUsers(), &level_.getEntities()}) {
        for (const auto &entity : *entities) {
            if (!entity.hasValue()) {
                continue;
            }
            const auto *actor = ::Actor::tryGetFromEntity(*entity, false);
            if (!actor) {
                continue;
            }
            if (&actor->getLevel() != &level_) {
                continue;
            }
            result.push_back(actor->getEndstoneActor());
        }
    }
    return result;
}

int EndstoneLevel::getTime() const
{
    return level_.getTime();
}

void EndstoneLevel::setTime(int time)
{
    level_.setTime(time);
}

std::vector<NotNull<Dimension>> EndstoneLevel::getDimensions() const
{
    std::vector<NotNull<Dimension>> dimensions;
    dimensions.reserve(dimensions_.size());
    for (const auto &val : dimensions_ | std::views::values) {
        dimensions.emplace_back(val);
    }
    return dimensions;
}

std::vector<Recipe> EndstoneLevel::getRecipes() const
{
    std::vector<Recipe> recipes;
    for (const auto &by_tag : level_.getRecipes().getRecipesAllTags()) {
        for (const auto &by_id : by_tag.second) {
            if (by_id.second) {
                recipes.push_back(EndstoneRecipe::fromMinecraft(by_id.second));
            }
        }
    }
    return recipes;
}

Nullable<Dimension> EndstoneLevel::getDimension(DimensionId id) const
{
    for (const auto &dimension : dimensions_ | std::views::values) {
        if (dimension->getId() == id) {
            return dimension;
        }
    }
    return nullptr;
}

Nullable<Dimension> EndstoneLevel::getDimension(DimensionType type) const
{
    if (const auto it = dimensions_.find(type.value); it != dimensions_.end()) {
        return it->second;
    }
    return nullptr;
}

Nullable<Dimension> EndstoneLevel::createDimension(const DimensionCreator &creator)
{
    if (auto existing = getDimension(creator.getId())) {
        return existing;
    }
    const std::string name{creator.getId()};
    auto &dimension_manager = level_.getDimensionManager();
    if (const auto type = dimension_manager.serverRegisterCustomDimension(name, mce::UUID::EMPTY)) {
        level_.getOrCreateDimension(type.value());
        return getDimension(type.value());
    }

    const auto dimension = dimension_manager.getOrCreateDimension(name);
    if (!dimension.isSet()) {
        return nullptr;
    }
    return getDimension(dimension.unwrap()->getDimensionId());
}

std::int64_t EndstoneLevel::getSeed() const
{
    return level_.getLevelSeed64();
}

bool EndstoneLevel::_hasGameRule(Identifier<GameRule> rule) const
{
    return server_.getRegistry<GameRule>().get(rule) != nullptr;
}

GameRuleValue EndstoneLevel::_getGameRule(Identifier<GameRule> rule) const
{
    const auto *entry = server_.getRegistry<GameRule>().get(rule);
    if (entry == nullptr) {
        throw std::out_of_range("Game rule is not available.");
    }

    const auto *game_rule = &static_cast<const EndstoneGameRule *>(entry)->getHandle();
    const auto &value = game_rule->getValue();
    switch (game_rule->getType()) {
    case ::GameRule::Type::Bool:
        return std::get<bool>(value);
    case ::GameRule::Type::Int:
        return std::get<int>(value);
    case ::GameRule::Type::Float:
        return std::get<float>(value);
    case ::GameRule::Type::Invalid:
        break;
    }
    throw std::runtime_error("Game rule holds no value type.");
}

bool EndstoneLevel::_setGameRule(Identifier<GameRule> rule, GameRuleValue value)
{
    const auto *entry = server_.getRegistry<GameRule>().get(rule);
    if (entry == nullptr) {
        return false;
    }

    auto &game_rules = level_.getGameRules();
    const auto *game_rule = &static_cast<const EndstoneGameRule *>(entry)->getHandle();
    const ::GameRuleId id{static_cast<int>(game_rule - game_rules.getRules().data())};

    const auto native_value = std::visit([](auto &&v) -> std::variant<int, float, bool> { return v; }, value);
    try {
        GameRuleUtils::setGameRule(level_, id, native_value, nullptr, false);
    }
    catch (const std::bad_variant_access &) {
        return false;
    }
    return true;
}

EndstoneServer &EndstoneLevel::getServer() const
{
    return server_;
}

::Level &EndstoneLevel::getHandle() const
{
    return level_;
}

};  // namespace endstone::core
