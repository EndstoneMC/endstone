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

#include <filesystem>
#include <regex>

#include "bedrock/resources/pack.h"
#include "bedrock/resources/pack_source.h"

namespace endstone::core {

struct EndstonePackSourceOptions {
    PackSourceOptions base;
    std::filesystem::path path;
    PackType pack_type;
};

class EndstonePackSource : public PackSource {
public:
    EndstonePackSource(EndstonePackSourceOptions options);
    ~EndstonePackSource() override = default;
    [[nodiscard]] PackOrigin getPackOrigin() const override;
    [[nodiscard]] PackType getPackType() const override;

protected:
    PackSourceLoadResult _loadImpl(PackSourceLoadOptions &&) override;

public:
    [[nodiscard]] std::unordered_map<PackIdVersion, std::string> getContentKeys() const;

private:
    std::filesystem::path path_;
    ::PackType pack_type_;
    bool discovered_{false};
    std::unordered_map<PackIdVersion, std::string> content_keys_;
};

}  // namespace endstone::core
