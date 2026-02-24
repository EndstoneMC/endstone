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

#include "endstone/event/cancellable.h"
#include "endstone/event/server/server_event.h"

namespace endstone {

/**
 * @brief Called when a message is sent by `/scriptevent` command
 */
class ScriptMessageEvent : public Cancellable<ServerEvent> {
public:
    ScriptMessageEvent(std::string message_id, std::string message, const CommandSender &sender)
        : Cancellable(false), message_id_(std::move(message_id)), message_(std::move(message)), sender_(sender)
    {
    }

    inline static const std::string NAME = "ScriptMessageEvent";
    [[nodiscard]] std::string getEventName() const override { return NAME; }

    /**
     * Get the message id to send.
     *
     * @return Message id to send
     */
    [[nodiscard]] const std::string &getMessageId() const { return message_id_; }

    /**
     * Get the message to send.
     *
     * @return Message to send
     */
    [[nodiscard]] const std::string &getMessage() const { return message_; }

    /**
     * Gets the command sender who initiated the command
     *
     * @return Command sender who initiated the command
     */
    [[nodiscard]] const CommandSender &getSender() const { return sender_; }

private:
    std::string message_id_;
    std::string message_;
    const CommandSender &sender_;
};

};  // namespace endstone
