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

#include "endstone/detail/level/level.h"

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/automatic_id.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"

using BedrockDimension = ::Dimension;

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(BedrockLevel &level) : server_(entt::locator<EndstoneServer>::value()), level_(level)
{
    const static AutomaticID<BedrockDimension, int> dimension_ids[] = {
        VanillaDimensions::Overworld, VanillaDimensions::Nether, VanillaDimensions::TheEnd};
    for (const auto &dimension_id : dimension_ids) {
        // Load all dimensions when the level is loaded
        level.getOrCreateDimension(dimension_id);
    }
}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
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

        if (&actor->getLevel() != &level_) {
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
