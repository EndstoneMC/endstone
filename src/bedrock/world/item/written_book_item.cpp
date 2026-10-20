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

#include "bedrock/world/item/written_book_item.h"

#include "bedrock/nbt/list_tag.h"
#include "bedrock/world/item/item_stack_base.h"

const std::string WrittenBookItem::TAG_TITLE = "title";
const std::string WrittenBookItem::TAG_AUTHOR = "author";
const std::string WrittenBookItem::TAG_XUID = "xuid";
const std::string WrittenBookItem::TAG_PAGES = "pages";
const std::string WrittenBookItem::TAG_PAGE_TEXT = "text";
const std::string WrittenBookItem::TAG_GENERATION = "generation";
const std::string WrittenBookItem::TAG_PAGE_PHOTO_NAME = "photoname";
const std::string WrittenBookItem::TAG_PAGE_FILTERED_TEXT = "filteredText";
const std::string WrittenBookItem::TAG_PAGE_FILTERED_PHOTO_NAME = "filteredPhotoname";

namespace {
bool isBook(const ItemStackBase &book)
{
    if (!book) {
        return false;
    }
    const auto &name = book.getItem()->getFullItemName();
    return name == "minecraft:writable_book" || name == "minecraft:written_book";
}
}  // namespace

int WrittenBookItem::getPageCount(const ItemStackBase &book)
{
    if (!isBook(book)) {
        return 0;
    }
    const auto *user_data = book.getUserData();
    if (user_data == nullptr) {
        return 0;
    }
    const auto *pages = user_data->getList(TAG_PAGES);
    return pages != nullptr ? pages->size() : 0;
}

std::vector<PageContent> WrittenBookItem::getPages(const ItemStackBase &book)
{
    std::vector<PageContent> result;
    if (!isBook(book)) {
        return result;
    }
    const auto *user_data = book.getUserData();
    if (user_data == nullptr) {
        return result;
    }
    const auto *pages = user_data->getList(TAG_PAGES);
    if (pages == nullptr) {
        return result;
    }
    for (auto i = 0; i < pages->size(); ++i) {
        if (const auto *page = pages->getCompound(i)) {
            result.push_back(PageContent::read(*page));
        }
    }
    return result;
}
