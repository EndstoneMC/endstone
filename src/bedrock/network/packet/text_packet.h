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

enum class TextPacketType : std::uint8_t {
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

struct TextPacketPayload {
    static constexpr int MAX_CHAT_LENGTH = 512;
    static constexpr int MAX_MESSAGE_PARAMS = 4;
    struct MessageOnly {
        bool operator==(const MessageOnly &) const;
        TextPacketType type;
        std::string message;
    };
    struct AuthorAndMessage {
        bool operator==(const AuthorAndMessage &) const;
        TextPacketType type;
        std::string author;
        std::string message;
    };
    struct MessageAndParams {
        bool operator==(const MessageAndParams &) const;
        TextPacketType type;
        std::string message;
        std::vector<std::string> params;
    };
    bool localize = false;
    std::string xuid;
    std::string platform_id;
    std::optional<std::string> filtered_message;
    std::variant<MessageOnly, AuthorAndMessage, MessageAndParams> body;
    TextPacketType getType() const;
    const std::string &getMessage() const;
    const std::string &getAuthorOrEmpty() const;
    const std::vector<std::string> &getParams() const;
};

class TextPacket : public Packet {
public:
    TextPacketPayload payload;
    SerializationMode serialization_mode;
};
BEDROCK_STATIC_ASSERT_SIZE(TextPacket, 248, 208);
