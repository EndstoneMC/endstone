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

#include <pybind11/embed.h>

#include "endstone/plugin/plugin_loader.h"

class PythonPluginLoader : public PluginLoader {
public:
    explicit PythonPluginLoader(Server &server);

    [[nodiscard]] std::vector<Plugin *> loadPlugins(const std::string &directory) noexcept override;
    void enablePlugin(Plugin &plugin) const override;
    void disablePlugin(Plugin &plugin) const override;

private:
    [[nodiscard]] PluginLoader *pimpl() const;

    pybind11::scoped_interpreter interpreter_;
    pybind11::object obj_;
    pybind11::gil_scoped_release release_;
};
