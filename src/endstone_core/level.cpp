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

#include "endstone/detail/virtual_table.h"

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(BedrockLevel &level, BedrockDimension &dimension)
    : server_(entt::locator<EndstoneServer>::value()), level_(level), dimension_(dimension)
{
}

std::string EndstoneLevel::getName() const
{
    if (dimension_.getDimensionId() == Dimension::OVERWORLD) {
        return level_.getLevelId();
    }
    if (dimension_.getDimensionId() == Dimension::NETHER) {
        return level_.getLevelId() + " (Nether)";
    }
    if (dimension_.getDimensionId() == Dimension::THE_END) {
        return level_.getLevelId() + " (TheEnd)";
    }
    throw std::runtime_error("Unknown dimension");
}

std::vector<Actor *> EndstoneLevel::getActors() const
{
    std::vector<Actor *> result;
    result.reserve(actors_.size());
    for (const auto &pair : actors_) {
        result.push_back(pair.second.get());
    }
    return result;
}

Actor *EndstoneLevel::getActor(std::uint64_t runtime_id) const
{
    auto it = actors_.find(runtime_id);
    if (it != actors_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void EndstoneLevel::addActor(std::unique_ptr<Actor> actor)
{
    auto id = actor->getRuntimeId();
    if (getActor(id) != nullptr) {
        server_.getLogger().error("Unable to add actor with runtime id {}. Actor already exists.", id);
        return;
    }
    actors_[id] = std::move(actor);
}

void EndstoneLevel::removeActor(std::uint64_t runtime_id)
{
    if (getActor(runtime_id) == nullptr) {
        server_.getLogger().error("Unable to remove actor with runtime id {}. Actor does not exist.", runtime_id);
        return;
    }
    actors_.erase(runtime_id);
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
