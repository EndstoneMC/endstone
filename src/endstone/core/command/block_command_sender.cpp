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

#include "endstone/core/command/block_command_sender.h"

#include <endstone/core/block/block.h>

#include "endstone/core/message.h"
#include "endstone/core/server.h"

namespace endstone::core {

namespace {
class BlockPermissionBase final : public PermissibleBase {
public:
    BlockPermissionBase() : PermissibleBase(nullptr) {}

    [[nodiscard]] PermissionLevel getPermissionLevel() const override
    {
        return PermissionLevel::Operator;
    }

    static std::shared_ptr<BlockPermissionBase> instance()
    {
        static std::shared_ptr<BlockPermissionBase> instance;
        if (!instance) {
            instance = std::make_unique<BlockPermissionBase>();
            instance->recalculatePermissions();
        }
        return instance;
    }
};
}  // namespace

EndstoneBlockCommandSender::EndstoneBlockCommandSender(const CommandOrigin &origin, CommandOutput &output)
    : ServerCommandSender(BlockPermissionBase::instance()), origin_(origin), output_(output)
{
}

BlockCommandSender *EndstoneBlockCommandSender::asBlock() const
{
    return const_cast<EndstoneBlockCommandSender *>(this);
}

void EndstoneBlockCommandSender::sendMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.forceOutput(tr.getText(), params);
}

void EndstoneBlockCommandSender::sendErrorMessage(const Message &message) const
{
    const auto tr = EndstoneMessage::toTranslatable(message);
    std::vector<CommandOutputParameter> params;
    for (const auto &param : tr.getParameters()) {
        params.emplace_back(param);
    }
    output_.error(tr.getText(), params);
}

std::string EndstoneBlockCommandSender::getName() const
{
    return origin_.getName();
}

PermissionLevel EndstoneBlockCommandSender::getPermissionLevel() const
{
    return BlockPermissionBase::instance()->getPermissionLevel();
}

std::unique_ptr<Block> EndstoneBlockCommandSender::getBlock() const
{
    return EndstoneBlock::at(origin_.getDimension()->getBlockSourceFromMainChunkSource(), origin_.getBlockPosition());
}
}  // namespace endstone::core
