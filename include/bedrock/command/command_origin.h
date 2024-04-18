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

#include <optional>

#include "bedrock/command/command_permission_level.h"
#include "bedrock/forward.h"
#include "bedrock/mce.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/protocol/sub_client_id.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/math/vec3.h"

class Actor;
class Level;

enum class CommandOriginType : char {
    Player = 0,
    CommandBlock = 1,
    MinecartCommandBlock = 2,
    DevConsole = 3,
    Test = 4,
    AutomationPlayer = 5,
    ClientAutomation = 6,
    DedicatedServer = 7,
    Entity = 8,
    Virtual = 9,
    GameArgument = 10,
    EntityServer = 11,
    Precompiled = 12,
    GameDirectorEntityServer = 13,
    Script = 14,
    ExecuteContext = 15,
};

class CommandOrigin {
public:
    CommandOrigin() = default;
    virtual ~CommandOrigin() = default;                                                                // 0
    [[nodiscard]] virtual const std::string &getRequestId() const = 0;                                 // 1
    [[nodiscard]] virtual std::string getName() const = 0;                                             // 2
    [[nodiscard]] virtual BlockPos getBlockPosition() const = 0;                                       // 3
    [[nodiscard]] virtual Vec3 getWorldPosition() const = 0;                                           // 4
    [[nodiscard]] virtual std::optional<Vec2> getRotation() const = 0;                                 // 5
    [[nodiscard]] virtual Level *getLevel() const = 0;                                                 // 6
    [[nodiscard]] virtual Dimension *getDimension() const = 0;                                         // 7
    [[nodiscard]] virtual Actor *getEntity() const = 0;                                                // 8
    [[nodiscard]] virtual CommandPermissionLevel getPermissionsLevel() const = 0;                      // 9
    [[nodiscard]] virtual std::unique_ptr<CommandOrigin> clone() const = 0;                            // 10
    [[nodiscard]] virtual std::optional<BlockPos> getCursorHitBlockPos() const = 0;                    // 11
    [[nodiscard]] virtual std::optional<Vec3> getCursorHitPos() const = 0;                             // 12
    [[nodiscard]] virtual bool hasChatPerms() const = 0;                                               // 13
    [[nodiscard]] virtual bool hasTellPerms() const = 0;                                               // 14
    [[nodiscard]] virtual bool canUseAbility(AbilitiesIndex ability) const = 0;                        // 15
    [[nodiscard]] virtual bool isWorldBuilder() const = 0;                                             // 16
    [[nodiscard]] virtual bool canUseCommandsWithoutCheatsEnabled() const = 0;                         // 17
    [[nodiscard]] virtual bool isSelectorExpansionAllowed() const = 0;                                 // 18
    [[nodiscard]] virtual const NetworkIdentifier &getSourceId() const = 0;                            // 19
    [[nodiscard]] virtual SubClientId getSourceSubId() const = 0;                                      // 20
    [[nodiscard]] virtual CommandOrigin *getOutputReceiver() const = 0;                                // 21
    [[nodiscard]] virtual CommandOriginIdentity getIdentity() const = 0;                               // 22
    [[nodiscard]] virtual CommandOriginType getOriginType() const = 0;                                 // 23
    [[nodiscard]] virtual CommandOriginData toCommandOriginData() const = 0;                           // 24
    [[nodiscard]] virtual const mce::UUID &getUUID() const = 0;                                        // 25
    virtual void handleCommandOutputCallback(int, std::string &&, void *) const = 0;                   // 26
    virtual void updateValues() = 0;                                                                   // 27
    [[nodiscard]] virtual const Vec3 getExecutePosition(int, const CommandPositionFloat &) const = 0;  // 28
    [[nodiscard]] virtual CompoundTag serialize() const = 0;                                           // 29
    [[nodiscard]] virtual bool isValid() const = 0;                                                    // 30

protected:
    virtual void _setUUID(const mce::UUID &uuid) = 0;  // 31

private:
    mce::UUID uuid_;
};
