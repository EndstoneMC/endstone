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
#include <vector>

#include "bedrock/network/packet.h"

enum class TextPacketType : char {
    Raw = 0,
    Chat = 1,
    Translate = 2,
    Popup = 3,
    JukeboxPopup = 4,
    Tip = 5,
    SystemMessage = 6,
    Whisper = 7,
    Announcement = 8,
    TextObjectWhisper = 9,
    TextObject = 10,
    TextObjectAnnouncement = 11
};

class TextPacket : public Packet {
public:
    TextPacketType type{TextPacketType::Raw};     // +48
    std::string player_name;                      // +56
    std::string message;                          // +88
    std::optional<std::string> filtered_message;  // +120
    std::vector<std::string> params;              // +160
    bool localize;                                // +184
    std::string xuid;                             // +192
    std::string platform_id;                      // +224
};
// static_assert(sizeof(TextPacket) == 216);
