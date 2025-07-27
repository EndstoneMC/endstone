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

#include "bedrock/network/packet.h"

struct SetTitlePacketInfo {
    static constexpr auto PACKET_NAME = "SetTitlePacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::SetTitle;
    static constexpr auto DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr auto COMPRESSIBILITY = Compressibility::Compressible;
};

struct SetTitlePacketPayload {
    enum class TitleType : int {
        Clear = 0,
        Reset = 1,
        Title = 2,
        Subtitle = 3,
        Actionbar = 4,
        Times = 5,
        TitleTextObject = 6,
        SubtitleTextObject = 7,
        ActionbarTextObject = 8,
    };

    SetTitlePacketPayload();
    SetTitlePacketPayload(TitleType);
    SetTitlePacketPayload(TitleType, const std::string &, std::optional<std::string>);
    SetTitlePacketPayload(TitleType, const ResolvedTextObject &);
    SetTitlePacketPayload(int, int, int);

    TitleType type;
    std::string title_text;
    std::string filtered_title_text;
    int fade_in_time;
    int stay_time;
    int fade_out_time;
    std::string xuid;
    std::string platform_online_id;
};

class SetTitlePacket : public SerializedPayloadPacket<SetTitlePacketInfo, SetTitlePacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
