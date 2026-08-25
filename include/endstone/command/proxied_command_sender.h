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

#include "endstone/command/command_sender.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * Represents a command sender that is running a command on behalf of another one, as `/execute` does.
 *
 * Output and permissions belong to the caller, while the name and the execution context belong to the callee.
 */
class ProxiedCommandSender : public CommandSender {
public:
    /**
     * Returns the CommandSender which triggered this proxied command.
     *
     * @return The caller which triggered the command
     */
    [[nodiscard]] virtual NotNull<CommandSender> getCaller() const = 0;

    /**
     * Returns the CommandSender which is being used to call the command.
     *
     * @return The sender which the command is being run as
     */
    [[nodiscard]] virtual NotNull<CommandSender> getCallee() const = 0;
};

}  // namespace endstone
