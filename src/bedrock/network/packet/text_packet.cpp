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

#include "bedrock/network/packet/text_packet.h"

TextPacket TextPacketPayload::createRaw(const std::string &message)
{
    return TextPacketPayload{
        .body = MessageOnly{TextPacketType::Raw, message},
    };
}

TextPacket TextPacketPayload::createChat(const std::string &author, const std::string &message,
                                         std::optional<std::string> filtered_message, const std::string &xuid,
                                         const std::string &platform_id)
{
    return TextPacketPayload{
        .xuid = xuid,
        .platform_id = platform_id,
        .filtered_message = std::move(filtered_message),
        .body = AuthorAndMessage{TextPacketType::Chat, author, message},
    };
}

TextPacket TextPacketPayload::createTranslated(const std::string &message, const std::vector<std::string> &params)
{
    return TextPacketPayload{
        .localize = true,
        .body = MessageAndParams{TextPacketType::Translate, message, params},
    };
}

MinecraftPacketIds TextPacket::getId() const
{
    return MinecraftPacketIds::Text;
}

std::string_view TextPacket::getName() const
{
    return "TextPacket";
}

void TextPacket::write(BinaryStream &stream) const
{
    // TODO(wtf): this doesn't look right, why would we transmit a list of TextPacketType(s) in string to the client?
    stream.writeBool(payload.localize, "Localize?", nullptr);
    stream.writeUnsignedVarInt(payload.body.index(), "Message content (variant based on message type)", nullptr);
    std::visit(
        [&](auto &&arg) {  //
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, TextPacketPayload::MessageOnly>) {
                stream.writeString("raw", "raw", nullptr);
                stream.writeString("tip", "tip", nullptr);
                stream.writeString("systemMessage", "systemMessage", nullptr);
                stream.writeString("textObjectWhisper", "textObjectWhisper", nullptr);
                stream.writeString("textObjectAnnouncement", "textObjectAnnouncement", nullptr);
                stream.writeString("textObject", "textObject", nullptr);
                stream.writeByte(static_cast<std::uint8_t>(arg.type), "Message Type", nullptr);
                stream.writeString(arg.message, "Message", nullptr);
            }
            else if constexpr (std::is_same_v<T, TextPacketPayload::AuthorAndMessage>) {
                stream.writeString("chat", "chat", nullptr);
                stream.writeString("whisper", "whisper", nullptr);
                stream.writeString("announcement", "announcement", nullptr);
                stream.writeByte(static_cast<std::uint8_t>(arg.type), "Message Type", nullptr);
                stream.writeString(arg.author, "Player Name", nullptr);
                stream.writeString(arg.message, "Message", nullptr);
            }
            else if constexpr (std::is_same_v<T, TextPacketPayload::MessageAndParams>) {
                stream.writeString("translate", "translate", nullptr);
                stream.writeString("popup", "popup", nullptr);
                stream.writeString("jukeboxPopup", "jukeboxPopup", nullptr);
                stream.writeByte(static_cast<std::uint8_t>(arg.type), "Message Type", nullptr);
                stream.writeString(arg.message, "Message", nullptr);
                stream.writeVectorList(arg.params, "Parameter", nullptr, "Parameter List", nullptr);
            }
        },
        payload.body);
    stream.writeString(payload.xuid, "Sender's XUID", nullptr);
    stream.writeString(payload.platform_id, "Platform Id", nullptr);
    if (payload.filtered_message.has_value()) {
        stream.writeBool(true, "Has Filtered Message", nullptr);
        stream.writeString(payload.filtered_message.value(), "Filtered Message", nullptr);
    }
    else {
        stream.writeBool(false, "Has Filtered Message", nullptr);
    }
}

Bedrock::Result<void> TextPacket::_read(ReadOnlyBinaryStream &)
{
    throw std::runtime_error("Not implemented");
}

TextPacket TextPacketPayload::createPopup(const std::string &message, const std::vector<std::string> &params)
{
    return TextPacketPayload{
        .body = MessageAndParams{TextPacketType::Popup, message, params},
    };
}

TextPacket TextPacketPayload::createTip(const std::string &message)
{
    return TextPacketPayload{
        .body = MessageOnly{TextPacketType::Tip, message},
    };
}

TextPacket::TextPacket(TextPacketPayload payload) : payload(std::move(payload)) {}
