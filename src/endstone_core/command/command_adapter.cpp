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

#include "endstone/detail/command/command_adapter.h"

namespace endstone::detail {
CommandSenderAdapter::CommandSenderAdapter(EndstoneServer &server, const CommandOrigin &origin, CommandOutput &output)
    : server_(server), origin_(origin), output_(output)
{
}

void CommandSenderAdapter::sendMessage(const std::string &message) const
{
    // TODO: addMessage to output
    printf("%s\n", message.c_str());
    printf("Sender name: %s\n", getName().c_str());
}

Server &CommandSenderAdapter::getServer() const
{
    return server_;
}

std::string CommandSenderAdapter::getName() const
{
    return origin_.getName();
}

void CommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    auto &server = Singleton<EndstoneServer>::getInstance();
    auto &command_map = server.getCommandMap();
    auto *command = command_map.getCommand(getCommandName());
    if (command) {
        auto sender = CommandSenderAdapter(server, origin, output);
        bool success = command->execute(sender, {});
        if (!success) {
            // TODO: set output status code
        }
    }
    else {
        // TODO: shouldn't happen, what to do now? be silent or print error message?
    }
}

}  // namespace endstone::detail
