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

#include <boost/algorithm/string.hpp>
#include <entt/entt.hpp>

#include "bedrock/core/utility/automatic_id.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/level/dimension.h"
#include "endstone/level/dimension.h"

namespace endstone::core {

EndstoneLevel::EndstoneLevel(::Level &level) : server_(EndstoneServer::getInstance()), level_(level)
{
    // Load all vanilla dimensions on start up
    level.getDimensionManager().getOnNewDimensionCreatedConnector().connect(
        [this](::Dimension &dimension) {
            // TODO(event): add DimensionLoadEvent here
            server_.getLogger().info("Loading dimension '{}'.", dimension.getName());
            dimensions_[dimension.getDimensionId().runtime_id] =
                std::make_unique<EndstoneDimension>(dimension.getWeakRef(), *this);
        },
        Bedrock::PubSub::ConnectPosition::AtBack, nullptr);
    level.getOrCreateDimension(VanillaDimensions::Overworld);
    level.getOrCreateDimension(VanillaDimensions::Nether);
    level.getOrCreateDimension(VanillaDimensions::TheEnd);
}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
}

std::vector<Actor *> EndstoneLevel::getActors() const
{
    std::vector<Actor *> result;
    for (const auto &entity : level_.getEntities()) {
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

std::vector<Dimension *> EndstoneLevel::getDimensions() const
{
    std::vector<Dimension *> dimensions;
    dimensions.reserve(dimensions_.size());
    for (const auto &val : dimensions_ | std::views::values) {
        dimensions.push_back(val.get());
    }
    return dimensions;
}

Dimension *EndstoneLevel::getDimension(std::string name) const
{
    if (name == "the_end") {
        name = "TheEnd";
    }
    for (const auto &dimension : dimensions_ | std::views::values) {
        if (boost::iequals(dimension->getName(), name)) {
            return dimension.get();
        }
    }
    return nullptr;
}

Dimension *EndstoneLevel::getDimension(int id) const
{
    if (const auto it = dimensions_.find(id); it != dimensions_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::int64_t EndstoneLevel::getSeed() const
{
    return level_.getLevelSeed64();
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
