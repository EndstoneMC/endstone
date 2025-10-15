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
#include <vector>

#include "endstone/plugin/plugin_loader.h"

namespace endstone::core {

class CppPluginLoader : public PluginLoader {
public:
    explicit CppPluginLoader(Server &server);
    [[nodiscard]] Plugin *loadPlugin(std::string file) override;
    [[nodiscard]] std::vector<std::string> getPluginFileFilters() const override;

private:
    std::filesystem::path prefix_;
    std::vector<std::unique_ptr<Plugin, std::function<void(Plugin *)>>> plugins_;
};

}  // namespace endstone::core
