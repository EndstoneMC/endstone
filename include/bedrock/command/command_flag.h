
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

    static const CommandFlag NONE;
    static const CommandFlag HIDDEN_FROM_BLOCK;
    static const CommandFlag HIDDEN_FROM_PLAYER;
    static const CommandFlag HIDDEN_FROM_AUTOMATION;
    static const CommandFlag LOCAL_ONLY;              // Will not send the command to clients
    static const CommandFlag COMMUNICATION;           // Will check if the executor has permission to chat
    static const CommandFlag WITHOUT_CHEATS_ENABLED;  // Will be usable in a level with require-cheats enabled
    static const CommandFlag DISABLED_IN_EDITOR;      // Will be disabled if inside a level editor
};
inline const CommandFlag CommandFlag::NONE = {0};
inline const CommandFlag CommandFlag::HIDDEN_FROM_BLOCK = {1 << 1};
inline const CommandFlag CommandFlag::HIDDEN_FROM_PLAYER = {1 << 2};
inline const CommandFlag CommandFlag::HIDDEN_FROM_AUTOMATION = {1 << 3};
inline const CommandFlag CommandFlag::LOCAL_ONLY = {1 << 4};
inline const CommandFlag CommandFlag::COMMUNICATION = {1 << 6};
inline const CommandFlag CommandFlag::WITHOUT_CHEATS_ENABLED = {1 << 7};
inline const CommandFlag CommandFlag::DISABLED_IN_EDITOR = {1 << 9};
