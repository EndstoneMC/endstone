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

#include <memory>

#include "bedrock/network/packet/book_edit_packet.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/meta/book_meta.h"

namespace endstone::core::book_edit {

inline constexpr auto WRITABLE_BOOK = ItemTypeId::minecraft("writable_book");
inline constexpr auto WRITTEN_BOOK = ItemTypeId::minecraft("written_book");

[[nodiscard]] bool isBook(const ItemStack &item);

[[nodiscard]] std::unique_ptr<BookMeta> createBookMeta(const ItemStack &item);

[[nodiscard]] std::unique_ptr<ItemMeta> createBookItemMeta(const BookMeta &meta, ItemTypeId type);

void applyBookEditOperation(BookMeta &meta, const ::BookEditPacketPayload::Operation &operation);

}  // namespace endstone::core::book_edit
