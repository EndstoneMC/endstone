
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

struct CommandFlag {
    std::uint16_t value;
    // bitwise OR
    CommandFlag operator|(const CommandFlag &flag) const
    {
        return CommandFlag{static_cast<std::uint16_t>(value | flag.value)};
    }
    // bitwise AND
    CommandFlag operator&(const CommandFlag &flag) const
    {
        return CommandFlag{static_cast<std::uint16_t>(value & flag.value)};
    }
    // bitwise XOR
    CommandFlag operator^(const CommandFlag &flag) const
    {
        return CommandFlag{static_cast<std::uint16_t>(value ^ flag.value)};
    }
    // bitwise NOT
    CommandFlag operator~() const
    {
        return CommandFlag{static_cast<std::uint16_t>(~value)};
    }
    // assignment OR
    CommandFlag &operator|=(const CommandFlag &flag)
    {
        value |= flag.value;
        return *this;
    }
    // assignment AND
    CommandFlag &operator&=(const CommandFlag &flag)
    {
        value &= flag.value;
        return *this;
    }
    // assignment XOR
    CommandFlag &operator^=(const CommandFlag &flag)
    {
        value ^= flag.value;
        return *this;
    }

    static const CommandFlag None;
    static const CommandFlag HiddenFromBlock;
    static const CommandFlag HiddenFromPlayer;
    static const CommandFlag HiddenFromAutomation;
    static const CommandFlag LocalOnly;
    static const CommandFlag Communication;
    static const CommandFlag WithoutCheatEnabled;
    static const CommandFlag DisabledInEditor;
};
