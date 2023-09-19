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

#include <string>

#include "bedrock/internal.h"

enum class CommandOriginType : char {
    Player = 0,
    Block = 1,
    MinecartBlock = 2,
    AutomationPlayer = 5,
    ClientAutomation = 6,
    Server = 7,
    Actor = 8,
    Virtual = 9,
    ActorServer = 11,
    Precompiled = 12,
    GameDirectoryEntityServerCommand = 13,
    Script = 14,
    ExecuteContext = 15,
};

class CommandOrigin {
public:
    [[nodiscard]] CommandOriginType getOriginType() const;
    [[nodiscard]] std::string getName() const;
};
