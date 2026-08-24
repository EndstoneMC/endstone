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
#include <utility>

#include "endstone/command/command_sender.h"
#include "endstone/event/event.h"
#include "endstone/message.h"

namespace endstone {

/**
 * Called when a command sender executes a command that is not defined.
 */
class UnknownCommandEvent final : public Event {
public:
    ENDSTONE_EVENT(UnknownCommandEvent);

    UnknownCommandEvent(const NotNull<CommandSender> &sender, std::string command_line, std::optional<Message> message)
        : sender_(sender), command_line_(std::move(command_line)), message_(std::move(message))
    {
    }

    /**
     * Gets the command sender.
     *
     * @return sender of the command
     */
    [[nodiscard]] const NotNull<CommandSender> &getSender() const { return sender_; }

    /**
     * Gets the command that was sent.
     *
     * @return command sent
     */
    [[nodiscard]] std::string getCommandLine() const { return command_line_; }

    /**
     * Gets the message that will be returned.
     *
     * @return unknown command message, or no value if no message will be sent
     */
    [[nodiscard]] std::optional<Message> getMessage() const { return message_; }

    /**
     * Sets the message that will be returned.
     *
     * @param message the message to be returned, or no value to avoid sending a message
     */
    void setMessage(std::optional<Message> message) { message_ = std::move(message); }

private:
    NotNull<CommandSender> sender_;
    std::string command_line_;
    std::optional<Message> message_;
};

}  // namespace endstone
