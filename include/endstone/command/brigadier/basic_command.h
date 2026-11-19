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
#include <string>
#include <vector>

#include "endstone/command/brigadier/command_source_stack.h"
#include "endstone/command/command_sender.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * A command that takes its arguments as words, registered through Commands::registerCommand().
 */
class BasicCommand {
public:
    virtual ~BasicCommand() = default;

    /**
     * Executes the command with the given source and arguments.
     *
     * A brigadier::CommandSyntaxException thrown here is sent to the sender as an error.
     *
     * @param source the source of the command
     * @param args the words typed after the label
     */
    virtual void execute(const CommandSource &source, const std::vector<std::string> &args) = 0;

    /**
     * Checks whether a command sender can receive and run the root command.
     *
     * @param sender the command sender
     * @return true if the sender has permission() or no permission is set
     */
    [[nodiscard]] virtual bool canUse(const NotNull<CommandSender> &sender) const
    {
        const auto permission = this->permission();
        return !permission.has_value() || sender->hasPermission(*permission);
    }

    /**
     * Returns the permission for the root command used in canUse() by default.
     *
     * @return the permission, or std::nullopt if none is required
     */
    [[nodiscard]] virtual std::optional<std::string> permission() const { return std::nullopt; }
};

}  // namespace endstone
