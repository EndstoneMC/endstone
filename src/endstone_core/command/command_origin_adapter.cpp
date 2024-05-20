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

#include "endstone/detail/command/command_origin_adapter.h"

#include "bedrock/command/command_origin_identity.h"
#include "bedrock/command/command_utils.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/math/block_pos.h"

namespace endstone::detail {

CommandOriginAdapter::CommandOriginAdapter(std::unique_ptr<CommandOrigin> pimpl) : pimpl_(std::move(pimpl)) {}

const std::string &CommandOriginAdapter::getRequestId() const
{
    return pimpl_->getRequestId();
}

std::string CommandOriginAdapter::getName() const
{
    return pimpl_->getName();
}

BlockPos CommandOriginAdapter::getBlockPosition() const
{
    return pimpl_->getBlockPosition();
}

Vec3 CommandOriginAdapter::getWorldPosition() const
{
    return pimpl_->getWorldPosition();
}

std::optional<Vec2> CommandOriginAdapter::getRotation() const
{
    return pimpl_->getRotation();
}

::Level *CommandOriginAdapter::getLevel() const
{
    return pimpl_->getLevel();
}

::Dimension *CommandOriginAdapter::getDimension() const
{
    return pimpl_->getDimension();
}

::Actor *CommandOriginAdapter::getEntity() const
{
    return pimpl_->getEntity();
}

CommandPermissionLevel CommandOriginAdapter::getPermissionsLevel() const
{
    return pimpl_->getPermissionsLevel();
}

std::unique_ptr<CommandOrigin> CommandOriginAdapter::clone() const
{
    return std::make_unique<CommandOriginAdapter>(pimpl_->clone());
}

std::optional<BlockPos> CommandOriginAdapter::getCursorHitBlockPos() const
{
    return pimpl_->getCursorHitBlockPos();
}

std::optional<Vec3> CommandOriginAdapter::getCursorHitPos() const
{
    return pimpl_->getCursorHitPos();
}

bool CommandOriginAdapter::hasChatPerms() const
{
    return pimpl_->hasChatPerms();
}

bool CommandOriginAdapter::hasTellPerms() const
{
    return pimpl_->hasTellPerms();
}

bool CommandOriginAdapter::canUseAbility(AbilitiesIndex ability) const
{
    return pimpl_->canUseAbility(ability);
}

bool CommandOriginAdapter::isWorldBuilder() const
{
    return pimpl_->isWorldBuilder();
}

bool CommandOriginAdapter::canUseCommandsWithoutCheatsEnabled() const
{
    return pimpl_->canUseCommandsWithoutCheatsEnabled();
}

bool CommandOriginAdapter::isSelectorExpansionAllowed() const
{
    return pimpl_->isSelectorExpansionAllowed();
}

const NetworkIdentifier &CommandOriginAdapter::getSourceId() const
{
    return pimpl_->getSourceId();
}

SubClientId CommandOriginAdapter::getSourceSubId() const
{
    return pimpl_->getSourceSubId();
}

CommandOrigin *CommandOriginAdapter::getOutputReceiver() const
{
    return pimpl_->getOutputReceiver();
}

CommandOriginIdentity CommandOriginAdapter::getIdentity() const
{
    return pimpl_->getIdentity();
}

CommandOriginType CommandOriginAdapter::getOriginType() const
{
    return pimpl_->getOriginType();
}

CommandOriginData CommandOriginAdapter::toCommandOriginData() const
{
    return pimpl_->toCommandOriginData();
}

const mce::UUID &CommandOriginAdapter::getUUID() const
{
    return pimpl_->getUUID();
}

void CommandOriginAdapter::handleCommandOutputCallback(int i, std::string &&str, Json::Value &&json) const
{
    pimpl_->handleCommandOutputCallback(i, std::move(str), std::move(json));
}

void CommandOriginAdapter::updateValues()
{
    pimpl_->updateValues();
}

Vec3 CommandOriginAdapter::getExecutePosition(int i, const CommandPositionFloat &pos) const
{
    return pimpl_->getExecutePosition(i, pos);
}

CompoundTag CommandOriginAdapter::serialize() const
{
    // TODO(nbt): implement this
    throw std::runtime_error("Not implemented");
}

bool CommandOriginAdapter::isValid() const
{
    return pimpl_->isValid();
}

void CommandOriginAdapter::setUUID(const mce::UUID &uuid)
{
    pimpl_->setUUID(uuid);
}

}  // namespace endstone::detail
