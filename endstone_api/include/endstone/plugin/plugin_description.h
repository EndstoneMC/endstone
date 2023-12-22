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
#include <utility>
#include <vector>

#include <fmt/format.h>

class PluginDescription {
public:
    PluginDescription(std::string name, std::string version, std::optional<std::string> description = std::nullopt,
                      std::optional<std::vector<std::string>> authors = std::nullopt,
                      std::optional<std::string> prefix = std::nullopt)
    {
        name_ = std::move(name);
        std::replace(name_.begin(), name_.end(), ' ', '_');
        version_ = std::move(version);
        full_name_ = fmt::format("{} v{}", name_, version_);
        description_ = std::move(description);
        authors_ = std::move(authors);
        prefix_ = std::move(prefix);
    }

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

    [[nodiscard]] std::optional<std::string> getDescription() const
    {
        return description_;
    }

    [[nodiscard]] std::optional<std::vector<std::string>> getAuthors() const
    {
        return authors_;
    }

    [[nodiscard]] std::optional<std::string> getPrefix() const
    {
        return prefix_;
    }

    inline const static std::regex ValidName{"^[A-Za-z0-9 _.-]+$"};

private:
    std::string name_;
    std::string version_;
    std::string full_name_;
    std::optional<std::string> description_;
    std::optional<std::vector<std::string>> authors_;
    std::optional<std::string> prefix_;
};
