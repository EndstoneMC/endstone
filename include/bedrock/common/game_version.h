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

#include <string>

#include "bedrock/bedrock.h"

namespace Common {
ENDSTONE_HOOK std::string getGameVersionString();
}

class SemVersion {
public:
private:
    std::uint16_t major_;
    std::uint16_t minor_;
    std::uint16_t patch_;
    std::string pre_release_;
    std::string build_meta_;
    std::string full_version_string_;
    bool valid_version_;
    bool any_version_;
};

class BaseGameVersion {
public:
private:
    SemVersion sem_version_;
    bool never_compatible_;
};
