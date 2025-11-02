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

#include <string_view>

#include <pybind11/pybind11.h>

#include "endstone/plugin/plugin_loader.h"

namespace endstone::core {

class PythonPluginLoader : public PluginLoader {
public:
    explicit PythonPluginLoader(Server &server);
    ~PythonPluginLoader() override;

    [[nodiscard]] Plugin *loadPlugin(std::string file) override;
    [[nodiscard]] std::vector<Plugin *> loadPlugins(std::string directory) override;
    [[nodiscard]] std::vector<std::string> getPluginFileFilters() const override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;

private:
    [[nodiscard]] PluginLoader *pimpl() const;

    pybind11::object obj_;
};

}  // namespace endstone::core
