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

#include "endstone/detail/level.h"

#include <entt/entt.hpp>

#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/detail/virtual_table.h"

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(BedrockLevel &level, BedrockDimension &dimension)
    : server_(entt::locator<EndstoneServer>::value()), level_(level), dimension_(dimension)
{
}

std::string EndstoneLevel::getName() const
{
    if (dimension_.getDimensionId() == VanillaDimensions::Overworld) {
        return level_.getLevelId();
    }
    return fmt::format("{} ({})", level_.getLevelId(), VanillaDimensions::toString(dimension_.getDimensionId()));
}

std::vector<Actor *> EndstoneLevel::getActors() const
{
    std::vector<Actor *> result;
    for (const auto &e : level_.getEntities()) {
        const auto &ctx = e.storage.context;
        if (!ctx.has_value()) {
            continue;
        }

        auto *actor = ::Actor::tryGetFromEntity(ctx.value(), false);
        if (!actor) {
            continue;
        }

        if (&actor->getDimension() != &dimension_) {
            continue;
        }

        result.push_back(&actor->getEndstoneActor());
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

BedrockLevel &EndstoneLevel::getBedrockLevel() const
{
    return level_;
}

};  // namespace endstone::detail
