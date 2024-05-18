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

#include "endstone/detail/command/plugin_command_origin.h"

#include "bedrock/command/command_origin_identity.h"
#include "bedrock/world/math/block_pos.h"

namespace endstone::detail {

PluginCommandOrigin::PluginCommandOrigin(::Level &level, ::Dimension &dimension) : level_(level), dimension_(dimension)
{
}

const std::string &PluginCommandOrigin::getRequestId() const
{
    static std::string empty_guid;
    return empty_guid;
}

std::string PluginCommandOrigin::getName() const
{
    return "Plugin";
}

BlockPos PluginCommandOrigin::getBlockPosition() const
{
    return BlockPos::ZERO;
}

Vec3 PluginCommandOrigin::getWorldPosition() const
{
    return Vec3::ZERO;
}

std::optional<Vec2> PluginCommandOrigin::getRotation() const
{
    return std::nullopt;
}

::Level *PluginCommandOrigin::getLevel() const
{
    return &level_;
}

::Dimension *PluginCommandOrigin::getDimension() const
{
    return &dimension_;
}

::Actor *PluginCommandOrigin::getEntity() const
{
    return nullptr;
}

CommandPermissionLevel PluginCommandOrigin::getPermissionsLevel() const
{
    return CommandPermissionLevel::GameDirectors;
}

std::unique_ptr<CommandOrigin> PluginCommandOrigin::clone() const
{
    return std::make_unique<PluginCommandOrigin>(level_, dimension_);
}

std::optional<BlockPos> PluginCommandOrigin::getCursorHitBlockPos() const
{
    return std::nullopt;
}

std::optional<Vec3> PluginCommandOrigin::getCursorHitPos() const
{
    return std::nullopt;
}

bool PluginCommandOrigin::hasChatPerms() const
{
    return true;
}

bool PluginCommandOrigin::hasTellPerms() const
{
    return true;
}

bool PluginCommandOrigin::canUseAbility(AbilitiesIndex ability) const
{
    return true;
}

bool PluginCommandOrigin::isWorldBuilder() const
{
    return false;
}

bool PluginCommandOrigin::canUseCommandsWithoutCheatsEnabled() const
{
    return true;
}

bool PluginCommandOrigin::isSelectorExpansionAllowed() const
{
    return true;
}

const NetworkIdentifier &PluginCommandOrigin::getSourceId() const
{
    static NetworkIdentifier unknown{};
    return unknown;
}

SubClientId PluginCommandOrigin::getSourceSubId() const
{
    return SubClientId::Primary;
}

CommandOrigin *PluginCommandOrigin::getOutputReceiver() const
{
    return const_cast<PluginCommandOrigin *>(this);
}

CommandOriginIdentity PluginCommandOrigin::getIdentity() const
{
    return {};
}

CommandOriginType PluginCommandOrigin::getOriginType() const
{
    return CommandOriginType::Virtual;
}

CommandOriginData PluginCommandOrigin::toCommandOriginData() const
{
    return {getOriginType(), getUUID()};
}

const mce::UUID &PluginCommandOrigin::getUUID() const
{
    return uuid_;
}

void PluginCommandOrigin::handleCommandOutputCallback(int, std::string &&, Json::Value &&) const {}

void PluginCommandOrigin::updateValues() {}

const Vec3 PluginCommandOrigin::getExecutePosition(int, const CommandPositionFloat &) const
{
    // TODO(fixme): correct implementation
    return Vec3::ZERO;
}

CompoundTag PluginCommandOrigin::serialize() const
{
    // TODO(fixme): correct implementation
    throw std::runtime_error("Not implemented");
}

bool PluginCommandOrigin::isValid() const
{
    return true;
}

void PluginCommandOrigin::_setUUID(const mce::UUID &uuid)
{
    uuid_ = uuid;
}

}  // namespace endstone::detail
