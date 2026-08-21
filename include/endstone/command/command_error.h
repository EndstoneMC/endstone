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

#include <exception>
#include <string>
#include <utility>
#include <variant>

#include "endstone/message.h"

namespace endstone {

/**
 * Thrown from a command handler to report a failure to the sender.
 *
 * The message is sent to the sender as an error message and the command is reported as
 * unsuccessful, so it does not count towards a `/execute` success or a command block's
 * comparator output.
 */
class CommandError : public std::exception {
public:
    explicit CommandError(Message message) : message_(std::move(message))
    {
        if (const auto *text = std::get_if<std::string>(&message_)) {
            what_ = *text;
        }
        else {
            what_ = std::get<Translatable>(message_).getText();
        }
    }

    /**
     * Returns the message sent to the command sender.
     *
     * @return Message to be displayed
     */
    [[nodiscard]] const Message &getMessage() const { return message_; }

    [[nodiscard]] const char *what() const noexcept override { return what_.c_str(); }

private:
    Message message_;
    std::string what_;
};

}  // namespace endstone
