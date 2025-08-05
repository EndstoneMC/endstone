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

#include "endstone/command/console_command_sender.h"
#include "endstone/core/command/server_command_sender.h"

namespace endstone::core {

class EndstoneConsoleCommandSender : public ServerCommandSender, public ConsoleCommandSender {
public:
    EndstoneConsoleCommandSender() = default;

    [[nodiscard]] ConsoleCommandSender *asConsole() const override;
    void sendMessage(const Message &message) const override;
    void sendErrorMessage(const Message &message) const override;
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] PermissionLevel getPermissionLevel() const override;

    // forward
    ENDSTONE_FORWARD_IMPL_PERMISSIBLE(ServerCommandSender);
};

}  // namespace endstone::core
