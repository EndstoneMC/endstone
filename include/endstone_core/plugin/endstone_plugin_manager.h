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
#include <unordered_map>
#include <vector>

#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

class EndstonePluginManager : public PluginManager {
public:
    explicit EndstonePluginManager(Server &server);

    void registerLoader(std::unique_ptr<PluginLoader> loader) override;
    [[nodiscard]] Plugin *getPlugin(const std::string &name) const override;
    [[nodiscard]] std::vector<Plugin *> getPlugins() const override;
    [[nodiscard]] bool isPluginEnabled(const std::string &name) const override;
    bool isPluginEnabled(Plugin *plugin) const override;
    std::vector<Plugin *> loadPlugins(const std::string &directory) override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;
    void disablePlugins() const override;
    void clearPlugins() override;

private:
    Server &server_;
    std::vector<std::unique_ptr<PluginLoader>> plugin_loaders_;
    std::vector<Plugin *> plugins_;
    std::unordered_map<std::string, Plugin *> lookup_names_;
};
