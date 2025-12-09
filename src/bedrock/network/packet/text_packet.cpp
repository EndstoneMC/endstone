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

std::string TextPacket::getName() const
{
    return "TextPacket";
}

Bedrock::Result<void> TextPacket::_read(ReadOnlyBinaryStream &)
{
    throw std::runtime_error("Not implemented");
}

TextPacket::TextPacket(TextPacketPayload payload) : payload(std::move(payload)) {}
