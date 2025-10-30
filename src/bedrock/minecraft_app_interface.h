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

#include "bedrock/core/islands.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"

class Minecraft;

class IMinecraftApp {
public:
    virtual ~IMinecraftApp() = default;
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<Minecraft> getPrimaryMinecraft() const = 0;
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<Automation::AutomationClient> getAutomationClient() const = 0;
    [[nodiscard]] virtual bool isEduMode() const = 0;
    [[nodiscard]] virtual bool isDedicatedServer() const = 0;
    virtual void onNetworkMaxPlayersChanged(std::uint32_t) = 0;
    [[nodiscard]] virtual IGameModuleShared &getGameModuleShared() const = 0;
    virtual void requestServerShutdown(std::string const &) = 0;
};
