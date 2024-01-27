
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

#include <cstdint>

class Command {
public:
    Command() = default;
    virtual ~Command() = default;
    virtual bool collectOptionalArguments()
    {
        return true;
    }
    virtual void execute(class CommandOrigin const &, class CommandOutput &) const = 0;

private:
    int32_t unk1_ = 0;
    int64_t unk2_ = 0;
    int32_t unk3_ = -1;
    int8_t unk4_ = 5;
    int16_t unk5_ = -1;
};
static_assert(sizeof(Command) == 32);
