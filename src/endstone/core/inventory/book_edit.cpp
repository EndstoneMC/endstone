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

#include "endstone/core/inventory/book_edit.h"

#include <optional>
#include <utility>
#include <variant>

#include "bedrock/nbt/compound_tag.h"
#include "endstone/core/inventory/meta/book_meta.h"
#include "endstone/core/inventory/meta/writable_book_meta.h"
#include "endstone/variant.h"

namespace endstone::core::book_edit {

bool isBook(const ItemStack &item)
{
    const auto type = item.getType().getId();
    return type == WRITABLE_BOOK || type == WRITTEN_BOOK;
}

std::unique_ptr<BookMeta> createBookMeta(const ItemStack &item)
{
    const auto meta = item.getItemMeta();
    return std::make_unique<EndstoneBookMeta>(meta.get());
}

std::unique_ptr<ItemMeta> createBookItemMeta(const BookMeta &meta, ItemTypeId type)
{
    ::CompoundTag tag;
    meta.getExtras().applyToItem(tag);
    if (type == WRITABLE_BOOK) {
        return std::make_unique<EndstoneWritableBookMeta>(tag);
    }
    if (type == WRITTEN_BOOK) {
        return std::make_unique<EndstoneBookMeta>(tag);
    }
    return nullptr;
}

void applyBookEditOperation(BookMeta &meta, const ::BookEditPacketPayload::Operation &operation)
{
    std::visit(
        overloaded{
            [&](const BookEditAction::ReplacePage &action) {
                auto pages = meta.getPages();
                if (action.page_index >= 0 && action.page_index <= static_cast<int>(pages.size())) {
                    if (action.page_index == static_cast<int>(pages.size())) {
                        pages.emplace_back(action.page_text);
                    }
                    else {
                        pages[action.page_index] = action.page_text;
                    }
                    meta.setPages(std::move(pages));
                }
            },
            [&](const BookEditAction::AddPage &action) {
                auto pages = meta.getPages();
                if (action.page_index >= 0 && action.page_index <= static_cast<int>(pages.size())) {
                    pages.insert(pages.begin() + action.page_index, action.page_text);
                    meta.setPages(std::move(pages));
                }
            },
            [&](const BookEditAction::DeletePage &action) {
                auto pages = meta.getPages();
                if (action.page_index >= 0 && action.page_index < static_cast<int>(pages.size())) {
                    pages.erase(pages.begin() + action.page_index);
                    meta.setPages(std::move(pages));
                }
            },
            [&](const BookEditAction::SwapPages &action) {
                auto pages = meta.getPages();
                if (action.page_index >= 0 && action.page_index < static_cast<int>(pages.size()) &&
                    action.swap_with_index >= 0 && action.swap_with_index < static_cast<int>(pages.size())) {
                    std::swap(pages[action.page_index], pages[action.swap_with_index]);
                    meta.setPages(std::move(pages));
                }
            },
            [&](const BookEditAction::Finalize &action) {
                meta.setTitle(action.title.empty() ? std::nullopt : std::optional{action.title});
                meta.setAuthor(action.author.empty() ? std::nullopt : std::optional{action.author});
                if (!meta.hasGeneration()) {
                    meta.setGeneration(BookMeta::Generation::Original);
                }
            },
        },
        operation);
}

}  // namespace endstone::core::book_edit
