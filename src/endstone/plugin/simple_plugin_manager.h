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

#include "endstone/command/simple_command_map.h"
#include "endstone/plugin/plugin_manager.h"

class SimplePluginManager : public PluginManager {

public:
    explicit SimplePluginManager(Server &server, SimpleCommandMap &command_map);

    void registerLoader(std::unique_ptr<PluginLoader> loader) override;
    Plugin *getPlugin(const std::string &name) const override;
    std::vector<Plugin *> getPlugins() const override;
    bool isPluginEnabled(const std::string &name) const override;
    bool isPluginEnabled(Plugin *plugin) const override;
    Plugin *loadPlugin(const std::filesystem::path &file) override;
    std::vector<Plugin *> loadPlugins(const std::filesystem::path &directory) override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;
    void disablePlugins() override;
    void clearPlugins() override;

private:
    Server &server_;
    std::map<std::string, std::unique_ptr<PluginLoader>> file_associations_;
    std::vector<std::unique_ptr<Plugin>> plugins_;
    std::map<std::string, Plugin *> lookup_names_;
    SimpleCommandMap &command_map_;
};
