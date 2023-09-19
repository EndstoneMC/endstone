// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <fmt/format.h>

#include "endstone/permission/permissible.h"

class Server;

class CommandSender : public Permissible {
public:
    CommandSender() noexcept = default;
    ~CommandSender() noexcept override = default;

    /**
     * Sends this sender a message
     *
     * @param message Message to be displayed
     */
    virtual void sendMessage(const std::string &message) const noexcept = 0;

    template <typename... Args>
    void sendMessage(const fmt::format_string<Args...> format, Args &&...args) const noexcept
    {
        sendMessage(fmt::format(format, std::forward<Args>(args)...));
    }

    /**
     * Returns the server instance that this command is running on
     *
     * @return Server instance
     */
    [[nodiscard]] virtual Server &getServer() const noexcept = 0;

    /**
     * Gets the name of this command sender
     *
     * @return Name of the sender
     */
    [[nodiscard]] virtual std::string getName() const noexcept = 0;
};
