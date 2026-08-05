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
#include <variant>

#include "bedrock/bedrock.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/network/packet/serialize/serialized_packet.h"

namespace BookEditAction {
struct ReplacePage {
    int page_index;
    std::string page_text;
    std::string photo_name;
};

struct AddPage {
    int page_index;
    std::string page_text;
    std::string photo_name;
};

struct DeletePage {
    int page_index;
};

struct SwapPages {
    int page_index;
    int swap_with_index;
};

struct Finalize {
    std::string title;
    std::string author;
    std::string xuid;
};
}  // namespace BookEditAction

struct BookEditPacketPayload {
    using Operation = std::variant<BookEditAction::ReplacePage, BookEditAction::AddPage, BookEditAction::DeletePage,
                                    BookEditAction::SwapPages, BookEditAction::Finalize>;

    int book_slot;
    Operation operation;
};
BEDROCK_STATIC_ASSERT_SIZE(BookEditPacketPayload, 112, 88);

struct BookEditPacketInfo {
    static constexpr auto PACKET_NAME = "BookEditPacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::BookEdit;
    static constexpr auto DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr auto COMPRESSIBILITY = Compressibility::Compressible;
};

class BookEditPacket : public SerializedPayloadPacket<BookEditPacketInfo, BookEditPacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
BEDROCK_STATIC_ASSERT_SIZE(BookEditPacket, 168, 144);
