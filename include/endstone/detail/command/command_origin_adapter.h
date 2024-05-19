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

#pragma once

#include "bedrock/command/command_origin.h"

namespace endstone::detail {

class CommandOriginAdapter : public CommandOrigin {
public:
    explicit CommandOriginAdapter(std::unique_ptr<CommandOrigin> pimpl);

    [[nodiscard]] const std::string &getRequestId() const override;
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] BlockPos getBlockPosition() const override;
    [[nodiscard]] Vec3 getWorldPosition() const override;
    [[nodiscard]] std::optional<Vec2> getRotation() const override;
    [[nodiscard]] ::Level *getLevel() const override;
    [[nodiscard]] ::Dimension *getDimension() const override;
    [[nodiscard]] ::Actor *getEntity() const override;
    [[nodiscard]] CommandPermissionLevel getPermissionsLevel() const override;
    [[nodiscard]] std::unique_ptr<CommandOrigin> clone() const override;
    [[nodiscard]] std::optional<BlockPos> getCursorHitBlockPos() const override;
    [[nodiscard]] std::optional<Vec3> getCursorHitPos() const override;
    [[nodiscard]] bool hasChatPerms() const override;
    [[nodiscard]] bool hasTellPerms() const override;
    [[nodiscard]] bool canUseAbility(AbilitiesIndex ability) const override;
    [[nodiscard]] bool isWorldBuilder() const override;
    [[nodiscard]] bool canUseCommandsWithoutCheatsEnabled() const override;
    [[nodiscard]] bool isSelectorExpansionAllowed() const override;
    [[nodiscard]] const NetworkIdentifier &getSourceId() const override;
    [[nodiscard]] SubClientId getSourceSubId() const override;
    [[nodiscard]] CommandOrigin *getOutputReceiver() const override;
    [[nodiscard]] CommandOriginIdentity getIdentity() const override;
    [[nodiscard]] CommandOriginType getOriginType() const override;
    [[nodiscard]] CommandOriginData toCommandOriginData() const override;
    [[nodiscard]] const mce::UUID &getUUID() const override;
    void handleCommandOutputCallback(int, std::string &&, Json::Value &&) const override;
    void updateValues() override;
    [[nodiscard]] Vec3 getExecutePosition(int i, const CommandPositionFloat &) const override;
    [[nodiscard]] CompoundTag serialize() const override;
    [[nodiscard]] bool isValid() const override;
    void setUUID(const mce::UUID &uuid) override;

protected:
    std::unique_ptr<CommandOrigin> pimpl_;
};

}  // namespace endstone::detail
