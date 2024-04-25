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
#include <magic_enum/magic_enum.hpp>

#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/detail/virtual_table.h"

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(BedrockLevel &level, BedrockDimension &dimension)
    : server_(entt::locator<EndstoneServer>::value()), level_(level), handle_(dimension)
{
    dimension_ = []() {
        auto dim = magic_enum::enum_cast<Dimension>(handle_.getDimensionId().id);
        if (!dim.has_value()) {
            throw std::runtime_error("Invalid dimension");
        }
        return dim.value();
    }();
}

std::string EndstoneLevel::getName() const
{
    if (getDimension() == Dimension::Overworld) {
        return level_.getLevelId();
    }
    return fmt::format("{} ({})", level_.getLevelId(), magic_enum::enum_name(getDimension()));
}

Dimension EndstoneLevel::getDimension() const
{
    return dimension_;
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

        if (&actor->getDimension() != &handle_) {
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
