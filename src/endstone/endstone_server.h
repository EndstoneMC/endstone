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

#include "bedrock/minecraft_commands.h"
#include "endstone/command/simple_command_map.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/plugin/simple_plugin_manager.h"
#include "endstone/server.h"
#include "pybind/pybind.h"
#include "versioning.h"

class EndstoneServer : public Server {
public:
    static EndstoneServer &getInstance()
    {
        static EndstoneServer instance;
        return instance;
    }

    // Delete copy and move constructors and assignment operators
    ~EndstoneServer() override = default;
    EndstoneServer(EndstoneServer const &) = delete;
    EndstoneServer(EndstoneServer &&) = delete;
    EndstoneServer &operator=(EndstoneServer const &) = delete;
    EndstoneServer &operator=(EndstoneServer &&) = delete;

    [[nodiscard]] Logger &getLogger() const noexcept override;
    PluginCommand *getPluginCommand(const std::string &name) const noexcept override;
    bool dispatchCommand(CommandSender &sender, const std::string &command_line) const noexcept override;
    CommandSender &getConsoleSender() const noexcept override;
    PluginManager &getPluginManager() const noexcept override;

    void loadPlugins() noexcept;
    void enablePlugins() const noexcept;
    void disablePlugins() const noexcept;
    [[maybe_unused]] [[nodiscard]] SimpleCommandMap &getCommandMap() const noexcept;

    [[nodiscard]] const std::string &getVersion() const noexcept override
    {
        static std::string version = Versioning::getEndstoneVersion();
        return version;
    }

    [[nodiscard]] const std::string &getMinecraftVersion() const noexcept override
    {
        static std::string minecraft_version = Versioning::getMinecraftVersion();
        return minecraft_version;
    }

private:
    EndstoneServer() noexcept;
    void setBedrockCommands() const noexcept;
    void enablePlugin(Plugin &plugin) const noexcept;

    Logger &logger_;
    std::unique_ptr<SimpleCommandMap> command_map_;
    std::unique_ptr<SimplePluginManager> plugin_manager_;
    std::unique_ptr<CommandSender> console_;
};
