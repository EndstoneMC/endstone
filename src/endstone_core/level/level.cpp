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

#include "bedrock/core/utility/automatic_id.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/detail/level/dimension.h"
#include "endstone/level/dimension.h"

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(::Level &level) : server_(entt::locator<EndstoneServer>::value()), level_(level)
{
    // Load all dimensions when the level is loaded
    const static AutomaticID<::Dimension, int> dimension_ids[] = {VanillaDimensions::Overworld,
                                                                  VanillaDimensions::Nether, VanillaDimensions::TheEnd};
    for (const auto &dimension_id : dimension_ids) {

        auto dimension = level.getOrCreateDimension(dimension_id);
        addDimension(std::make_unique<EndstoneDimension>(*dimension, *this));
    }

    loadResourcePacks();
}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
}

std::vector<Actor *> EndstoneLevel::getActors() const
{
    std::vector<Actor *> result;
    for (const auto &e : level_.getEntities()) {
        if (!e.hasValue()) {
            continue;
        }

        // TODO(check): is this the correct usage of OwnerPtr<EntityContext> ?
        const auto *actor = ::Actor::tryGetFromEntity(e.getStackRef(), false);
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
    for (const auto &it : dimensions_) {
        dimensions.push_back(it.second.get());
    }
    return dimensions;
}

Dimension *EndstoneLevel::getDimension(std::string name) const
{
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    auto it = dimensions_.find(name);
    if (it == dimensions_.end()) {
        return nullptr;
    }
    return it->second.get();
}

void EndstoneLevel::addDimension(std::unique_ptr<Dimension> dimension)
{
    auto name = dimension->getName();
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (dimensions_.find(name) != dimensions_.end()) {
        server_.getLogger().error(
            "Dimension {} is a duplicate of another dimension and has been prevented from loading.", name);
        return;
    }
    dimensions_[name] = std::move(dimension);
}

void EndstoneLevel::loadResourcePacks()
{
    auto *manager = level_.getClientResourcePackManager();
    auto &source = server_.getResourcePackSource();

    // Load packs from world_resource_packs.json first
    nlohmann::json json;
    fs::path file_path = fs::current_path() / "worlds" / level_.getLevelId() / "world_resource_packs.json";

    if (exists(file_path)) {
        try {
            std::ifstream file(file_path);
            json = nlohmann::json::parse(file, nullptr, true, true);
        }
        catch (std::exception &e) {
            server_.getLogger().error("Failed to load from {}", file_path);
        }
    }

    // Append loaded packs to the stack
    source.forEachPackConst([&json](auto &pack) {
        auto &identity = pack.getManifest().getIdentity();
        json.push_back({
            {"pack_id", identity.id.asString()},
            {"version", {identity.version.getMajor(), identity.version.getMinor(), identity.version.getPatch()}},
        });
    });

    // Deserialize the pack stack from json
    std::stringstream ss(json.dump());
    auto pack_stack = ResourcePackStack::deserialize(ss, server_.getResourcePackRepository());

    auto content_keys = source.getContentKeys();
    for (const auto &pack_instance : pack_stack->stack) {
        const auto &manifest = pack_instance.getManifest();
        bool encrypted = content_keys.find(manifest.getIdentity()) != content_keys.end();
        server_.getLogger().info("Loaded {} v{} (Pack ID: {}) {}", manifest.getName(),
                                 manifest.getIdentity().version.asString(), manifest.getIdentity().id.asString(),
                                 encrypted ? ColorFormat::Green + "[encrypted]" : "");
    }

    // Replace the level pack stack
    manager->setStack(std::move(pack_stack), ResourcePackStackType::LEVEL, false);
}

EndstoneServer &EndstoneLevel::getServer() const
{
    return server_;
}

::Level &EndstoneLevel::getHandle() const
{
    return level_;
}

};  // namespace endstone::detail
