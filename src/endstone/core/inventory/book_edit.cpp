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

#include <utility>
#include <variant>

#include "endstone/core/inventory/item_factory.h"
#include "endstone/variant.h"

namespace endstone::core::book_edit {

bool isWritableBook(const ItemStack &item)
{
    return item.getType().getId() == WRITABLE_BOOK;
}

Nullable<BookMeta> createBookMeta(const ItemStack &item)
{
    const auto meta = item.getItemMeta();
    return EndstoneItemFactory::instance().asMetaFor(meta.get().get(), WRITTEN_BOOK).as<BookMeta>();
}

void applyBookEditOperation(BookMeta &meta, const ::BookEditPacketPayload::Operation &operation)
{
    std::visit(overloaded{
                   [&](const BookEditAction::ReplacePage &action) {
                       if (action.page_index < 0 || action.page_index >= MAX_PAGE_COUNT) {
                           return;
                       }
                       auto pages = meta.getPages();
                       if (action.page_index >= static_cast<int>(pages.size())) {
                           pages.resize(action.page_index);
                           pages.emplace_back(action.page_text);
                       }
                       else {
                           pages[action.page_index] = action.page_text;
                       }
                       meta.setPages(std::move(pages));
                   },
                   [&](const BookEditAction::AddPage &action) {
                       if (action.page_index < 0 || action.page_index >= MAX_PAGE_COUNT) {
                           return;
                       }
                       auto pages = meta.getPages();
                       if (static_cast<int>(pages.size()) >= MAX_PAGE_COUNT) {
                           return;
                       }
                       if (action.page_index >= static_cast<int>(pages.size())) {
                           pages.resize(action.page_index);
                           pages.emplace_back(action.page_text);
                       }
                       else {
                           pages.insert(pages.begin() + action.page_index, action.page_text);
                       }
                       meta.setPages(std::move(pages));
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
                       meta.setTitle(action.title);
                       meta.setAuthor(action.author);
                       meta.setGeneration(BookMeta::Generation::Original);
                   },
               },
               operation);
}

}  // namespace endstone::core::book_edit
