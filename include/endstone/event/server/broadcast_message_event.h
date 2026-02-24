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
#include <unordered_set>
#include <utility>

#include "endstone/command/command_sender.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/server/server_event.h"

namespace endstone {

/**
 * @brief Event triggered for server broadcast messages such as from Server::broadcast
 *
 * This event should be async if fired from an async thread.
 */
class BroadcastMessageEvent : public Cancellable<ServerEvent> {
public:
    BroadcastMessageEvent(bool async, Message message, std::unordered_set<const CommandSender *> recipients)
        : Cancellable(async), message_(std::move(message)), recipients_(std::move(recipients))
    {
    }

    inline static const std::string NAME = "BroadcastMessageEvent";
    [[nodiscard]] std::string getEventName() const override { return NAME; }

    /**
     * Get the message to broadcast.
     *
     * @return Message to broadcast
     */
    [[nodiscard]] const Message &getMessage() const { return message_; }

    /**
     * Set the message to broadcast.
     *
     * @param message New message to broadcast
     */
    void setMessage(Message message) { message_ = std::move(message); }

    /**
     * Gets a set of recipients that this broadcast message will be displayed to.
     *
     * @return All CommandSenders who will see this broadcast message
     */
    [[nodiscard]] const std::unordered_set<const CommandSender *> &getRecipients() const { return recipients_; }

private:
    Message message_;
    std::unordered_set<const CommandSender *> recipients_;
};

}  // namespace endstone
