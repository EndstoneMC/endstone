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

#include <memory>
#include <string>
#include <string_view>

#include "endstone/logger.h"

class PluginManager;

/**
 * @brief Represents a server implementation.
 */
class Server {
public:
    Server() = default;
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    virtual ~Server() = default;

    /**
     * Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    [[nodiscard]] virtual Logger &getLogger() const = 0;

    /**
     * Gets the plugin manager for interfacing with plugins.
     *
     * @return a plugin manager for this Server instance
     */
    [[nodiscard]] virtual PluginManager &getPluginManager() const = 0;

    /**
     * Gets the version string of this server implementation.
     *
     * @return version of this server implementation
     */
    [[nodiscard]] virtual std::string_view getVersion() const = 0;

    /**
     * Gets the Minecraft version that this server is running.
     *
     * @return version of Minecraft
     */
    [[nodiscard]] virtual std::string getMinecraftVersion() const = 0;
};
