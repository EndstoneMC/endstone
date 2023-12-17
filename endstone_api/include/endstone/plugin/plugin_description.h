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
#include <mutex>
#include <optional>
#include <regex>
#include <string>
#include <vector>

class PluginDescription {
public:
    PluginDescription(const std::string &name, const std::string &version)
    {
        name_ = name;
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = version;
        full_name_ = name_ + " v" + version;
    }
    PluginDescription(const PluginDescription &) = delete;
    PluginDescription &operator=(const PluginDescription &) = delete;

    virtual ~PluginDescription() = default;

    [[nodiscard]] const std::string &getName() const
    {
        return name_;
    }

    [[nodiscard]] const std::string &getVersion() const
    {
        return version_;
    }

    [[nodiscard]] const std::string &getFullName() const
    {
        return full_name_;
    }

    [[nodiscard]] virtual std::optional<std::string> getDescription() const
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual std::optional<std::vector<std::string>> getAuthors() const
    {
        return std::nullopt;
    }

    [[nodiscard]] virtual std::optional<std::string> getPrefix() const
    {
        return std::nullopt;
    }

    inline const static std::regex ValidName{"^[A-Za-z0-9 _.-]+$"};

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
};
