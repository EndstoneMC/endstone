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

#include "endstone/command/command.h"
#include "endstone/common.h"

class PluginDescription {
public:
    // Constructor for PluginDescription with default behavior from CppPluginDescription
    PluginDescription(const std::string &name, const std::string &version)
    {
        if (!std::regex_match(name, VALID_NAME)) {
            throw std::invalid_argument("Plugin name contains invalid characters.");
        }

        name_ = name;
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = version;
        full_name_ = name_ + " v" + version;
    }

    virtual ~PluginDescription() = default;

    const std::string &getName() const
    {
        return name_;
    }

    const std::string &getVersion() const
    {
        return version_;
    }

    const std::string &getFullName() const
    {
        return full_name_;
    }

    virtual std::optional<std::string> getDescription() const
    {
        return {};
    }

    virtual std::optional<std::vector<std::string>> getAuthors() const
    {
        return {};
    }

    virtual std::optional<std::string> getPrefix() const
    {
        return {};
    }

    virtual std::vector<std::shared_ptr<Command>> getCommands() const
    {
        return {};
    }

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
    inline static const std::regex VALID_NAME{"^[A-Za-z0-9 _.-]+$"};
};
