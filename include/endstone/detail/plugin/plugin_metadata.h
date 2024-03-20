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

#include "endstone/command/command.h"
#include "endstone/permissions/permission.h"
#include "endstone/plugin/plugin_load_order.h"

namespace endstone::detail {

struct PluginMetadata {
public:
    std::string description;
    std::vector<std::string> authors;
    std::vector<std::string> contributors;
    std::string website;
    std::string prefix;
    PluginLoadOrder load = PluginLoadOrder::PostWorld;
    std::vector<std::string> depends;
    std::vector<std::string> soft_depends;
    std::vector<std::string> load_before;


private:
    std::vector<Command> commands_;
    std::vector<Permission> permissions_;
};

}  // namespace endstone::detail
