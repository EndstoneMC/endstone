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

#include <bitset>
#include <cstdint>

#include "bedrock/core/utility/enable_non_owner_references.h"

enum class ServerTextEvent : std::uint8_t {
    PlayerSleeping = 0,
    PlayerConnection = 1,
    PlayerChangedSkin = 2,
    _Count = 3,
};

using ServerTextSettingsBitset = std::bitset<3UL>;

class ServerTextSettings : public Bedrock::EnableNonOwnerReferences {
public:
    ServerTextSettings() = default;
    ServerTextSettings(const ServerTextSettingsBitset &enabled_text_events) : enabled_text_events_{enabled_text_events}
    {
    }
    [[nodiscard]] const ServerTextSettingsBitset &getEnabledServerTextEvents() const
    {
        return enabled_text_events_;
    }

private:
    ServerTextSettingsBitset enabled_text_events_;
};
static_assert(sizeof(ServerTextSettings) == 32);
