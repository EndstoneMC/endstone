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

class TextPacket : public Packet {
public:
    TextPacket() = default;
    ~TextPacket() override = default;

    static TextPacket createRaw(const std::string & message);
    static TextPacket createChat(const std::string &author, const std::string &message,
                                 std::optional<std::string> filtered_message, const std::string &xuid,
                                 const std::string &platform_id);
    static TextPacket createTranslatedChat(const std::string &, const std::string &, const std::string &,
                                           const std::string &);
    static TextPacket createTranslated(const std::string &, const std::vector<std::string> &);
    static TextPacket createTextObjectMessage(const ResolvedTextObject &, std::string, std::string);
    static TextPacket createTextObjectWhisperMessage(const ResolvedTextObject &, const std::string &,
                                                     const std::string &);
    static TextPacket createTextObjectWhisperMessage(const std::string &, const std::string &, const std::string &);
    static TextPacket createRawJsonObjectMessage(const std::string &);
    static TextPacket createRawJsonObjectAnnouncement(const std::string &);
    static TextPacket createSystemMessage(const std::string &);
    static TextPacket createWhisper(const std::string &, const std::string &, const std::optional<std::string>,
                                    const std::string &, const std::string &);
    static TextPacket createAnnouncement(const std::string &, const std::string &, const std::optional<std::string>,
                                         const std::string &, const std::string &);
    static TextPacket createTranslatedAnnouncement(const std::string &, const std::string &, const std::string &,
                                                   const std::string &);
    static TextPacket createJukeboxPopup(const std::string &, const std::vector<std::string> &);

    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string getName() const override;
    void write(BinaryStream &) const override;

    TextPacketType type;
    std::string author;
    std::string message;
    std::optional<std::string> filtered_message;
    std::vector<std::string> params;
    bool localize;
    std::string xuid;
    std::string platform_id;

    static constexpr int MAX_CHAT_LENGTH = 512;

private:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &) override;
    TextPacket(TextPacketType type, const std::string &author, const std::string &message,
               std::optional<std::string> filtered_message, const std::vector<std::string> &params, bool localize,
               const std::string &xuid, const std::string &platform_id);
};
// static_assert(sizeof(TextPacket) == 216);
