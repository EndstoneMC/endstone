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

#include <memory>
#include <vector>

#include "bedrock/core/resource/pack_id_version.h"
#include "bedrock/core/resource/resource_helper.h"
#include "bedrock/core/sem_ver/sem_version.h"

enum class PackErrorType : int {
    None = 0,
    IncompletePackError = 1,
    PackParseError = 2,
    PackLoadError = 3,
    UIError = 4,
    PackSettingsError = 5,
};

class PackError {
public:
    virtual ~PackError() = default;
    [[nodiscard]] virtual std::string getLocErrorMessage() const;

private:
    std::vector<std::string> error_parameters_;  // +8
    int error_value_;                            // +32
    PackErrorType pack_error_type_;              // +36
};

using PackErrors = std::vector<std::shared_ptr<PackError>>;

class PackReport {
public:
    PackReport() = default;
    [[nodiscard]] ResourceLocation const &getLocation() const;
    [[nodiscard]] bool hasErrors() const;
    [[nodiscard]] PackErrors const &getErrors() const;
     [[nodiscard]] PackIdVersion const &getIdentity() const;
    [[nodiscard]] std::string const &getOriginalName() const;

private:
    ResourceLocation location_;              // +0
    bool was_upgraded_;                      // +56
    bool attempted_upgrade_;                 // +57
    PackErrors errors_;                      // +64
    PackErrors warnings_;                    // +88
    SemVersion required_base_game_version_;  // +112
    std::string original_name_;              // +224
    std::string original_version_;           // +256
    PackIdVersion identity_;                 // +288
    PackType pack_type_;                     // +424
};
