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

namespace BookEditAction {
inline constexpr int MAX_TEXT_LENGTH = 768;

struct ReplacePage {
    bool operator==(const ReplacePage &) const;

    int page_index;
    std::string page_text;
    std::string photo_name;
};

struct AddPage {
    bool operator==(const AddPage &) const;

    int page_index;
    std::string page_text;
    std::string photo_name;
};

struct DeletePage {
    bool operator==(const DeletePage &) const;

    int page_index;
};

struct SwapPages {
    bool operator==(const SwapPages &) const;

    int page_index;
    int swap_with_index;
};

struct Finalize {
    bool operator==(const Finalize &) const;

    std::string title;
    std::string author;
    std::string xuid;
};
}  // namespace BookEditAction

struct BookEditPacketPayload {
    using Operation = std::variant<BookEditAction::ReplacePage, BookEditAction::AddPage, BookEditAction::DeletePage,
                                   BookEditAction::SwapPages, BookEditAction::Finalize>;

    BookEditPacketPayload();
    [[nodiscard]] const Operation &getOperation() const;
    [[nodiscard]] const std::string &getText1() const;
    [[nodiscard]] const std::string &getText2() const;
    [[nodiscard]] PageContent getPage() const;
    void setToReplacePage(int, int, const PageContent &);
    void setToAddPage(int, int, const PageContent &);
    void setToDeletePage(int, int);
    void setToSwapPages(int, int, int);
    void setToFinalize(int, std::string, std::string, std::string);

    int book_slot;
    Operation operation;
};
BEDROCK_STATIC_ASSERT_SIZE(BookEditPacketPayload, 112, 88);

class BookEditPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;

    BookEditPacket();
    BookEditPacket(BookEditPacketPayload);

    BookEditPacketPayload payload;                                        // +48
    SerializationMode serialization_mode{SerializationMode::CerealOnly};  // +160
};
BEDROCK_STATIC_ASSERT_SIZE(BookEditPacket, 168, 144);
