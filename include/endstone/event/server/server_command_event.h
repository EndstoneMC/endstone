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

#include <string>
#include <utility>

#include "endstone/command/command_sender.h"
#include "endstone/event/event.h"
#include "endstone/event/handler_list.h"
namespace endstone {

class ServerCommandEvent : public Event {
public:
    // TODO: shall we use endstone::Command instead of std::string?
    ServerCommandEvent(CommandSender &sender, std::string command) : sender_(sender), command_(std::move(command)) {}

    void dispatch() override
    {
        handler_.callEvent(*this);
    }

    [[nodiscard]] std::string getEventName() const override
    {
        return "ServerCommandEvent";
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    /**
     * Gets the command that the user is attempting to execute from the console
     *
     * @return Command the user is attempting to execute
     */
    [[nodiscard]] const std::string &getCommand() const
    {
        return command_;
    }

    /**
     * Sets the command that the server will execute
     *
     * @param message New message that the server will execute
     */
    void setCommand(const std::string &command)
    {
        command_ = command;
    }

    /**
     * Get the command sender.
     *
     * @return The sender
     */
    [[nodiscard]] CommandSender &getSender() const
    {
        return sender_;
    }

private:
    CommandSender &sender_;
    std::string command_;
    HandlerList<ServerCommandEvent> handler_;
};

}  // namespace endstone
