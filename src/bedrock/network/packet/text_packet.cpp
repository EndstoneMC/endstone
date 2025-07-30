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

TextPacket TextPacket::createRaw(const std::string &message)
{
    return {TextPacketType::Raw, "", message, std::nullopt, {}, false, "", ""};
}

TextPacket TextPacket::createChat(const std::string &author, const std::string &message,
                                  std::optional<std::string> filtered_message, const std::string &xuid,
                                  const std::string &platform_id)
{
    return {TextPacketType::Chat, author, message, std::move(filtered_message), {}, false, xuid, platform_id};
}

TextPacket TextPacket::createTranslated(const std::string &message, const std::vector<std::string> &params)
{
    return {TextPacketType::Translate, "", message, std::nullopt, params, true, "", ""};
}

MinecraftPacketIds TextPacket::getId() const
{
    return MinecraftPacketIds::Text;
}

std::string TextPacket::getName() const
{
    return "TextPacket";
}

void TextPacket::write(BinaryStream &stream) const
{
    stream.writeByte(static_cast<std::uint8_t>(type), "Message Type", nullptr);
    stream.writeBool(localize, "Localize?", nullptr);
    stream.writeConditional("Message Type",
                            {
                                {type == TextPacketType::Raw,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Raw"},
                                {type == TextPacketType::Chat,
                                 [&](BinaryStream &s) {
                                     s.writeString(author, "Player Name", nullptr);
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Chat"},
                                {type == TextPacketType::Translate,
                                 [&](BinaryStream &s) {
                                     s.writeString(message, "Message", nullptr);
                                     s.writeVectorList(params, "Parameter", nullptr, "Parameter List", nullptr);
                                 },
                                 "Translate"},
                                {type == TextPacketType::Popup,
                                 [&](BinaryStream &s) {
                                     s.writeString(message, "Message", nullptr);
                                     s.writeVectorList(params, "Parameter", nullptr, "Parameter List", nullptr);
                                 },
                                 "Popup"},
                                {type == TextPacketType::JukeboxPopup,
                                 [&](BinaryStream &s) {
                                     s.writeString(message, "Message", nullptr);
                                     s.writeVectorList(params, "Parameter", nullptr, "Parameter List", nullptr);
                                 },
                                 "Jukebox Popup"},
                                {type == TextPacketType::Tip,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Tip"},
                                {type == TextPacketType::SystemMessage,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "System Message"},
                                {type == TextPacketType::Whisper,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(author, "Player Name", nullptr);
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Whisper"},
                                {type == TextPacketType::Announcement,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(author, "Player Name", nullptr);
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Announcement"},
                                {type == TextPacketType::TextObjectWhisper,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Text Object Whisper"},
                                {type == TextPacketType::TextObject,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Text Object"},
                                {type == TextPacketType::TextObjectAnnouncement,
                                 [&](BinaryStream &s) {  //
                                     s.writeString(message, "Message", nullptr);
                                 },
                                 "Text Object Announcement"},
                            },
                            std::nullopt);
    stream.writeString(xuid, "Sender's XUID", nullptr);
    stream.writeString(platform_id, "Platform Id", nullptr);
    stream.writeString(filtered_message.value_or(""), "Filtered Message", nullptr);
}

Bedrock::Result<void> TextPacket::_read(ReadOnlyBinaryStream &)
{
    throw std::runtime_error("Not implemented");
}

TextPacket::TextPacket(TextPacketType type, const std::string &author, const std::string &message,
                       std::optional<std::string> filtered_message, const std::vector<std::string> &params,
                       const bool localize, const std::string &xuid, const std::string &platform_id)
    : type(type), author(author), message(message), filtered_message(std::move(filtered_message)), params(params),
      localize(localize), xuid(xuid), platform_id(platform_id)
{
}
