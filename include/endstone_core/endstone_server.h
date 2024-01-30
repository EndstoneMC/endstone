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



#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"
#include "endstone_core/plugin/endstone_plugin_manager.h"

class EndstoneServer : public Server {
public:
    static EndstoneServer &getInstance()
    {
        static EndstoneServer instance;
        return instance;
    }

    EndstoneServer(EndstoneServer const &) = delete;
    EndstoneServer(EndstoneServer &&) = delete;
    EndstoneServer &operator=(EndstoneServer const &) = delete;
    EndstoneServer &operator=(EndstoneServer &&) = delete;
    ~EndstoneServer() override = default;

    [[nodiscard]] Logger &getLogger() const override;
    [[nodiscard]] PluginManager &getPluginManager() const override;

    void loadPlugins();
    void enablePlugins() const;
    void disablePlugins() const;

    [[nodiscard]] std::string_view getVersion() const override;
    [[nodiscard]] std::string getMinecraftVersion() const override;

private:
    EndstoneServer();
    void enablePlugin(Plugin &plugin) const;

    Logger &logger_;
    std::unique_ptr<EndstonePluginManager> plugin_manager_;
};
