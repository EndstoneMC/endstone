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

#include "bedrock/world/item/writable_book_item.h"

#include "bedrock/nbt/list_tag.h"
#include "bedrock/world/item/written_book_item.h"

namespace {
bool isBookAndQuill(const ItemStackBase &book)
{
    return book && book.getItem()->getFullItemName() == "minecraft:writable_book";
}

ListTag &getOrCreatePages(CompoundTag &user_data)
{
    if (!user_data.contains(WrittenBookItem::TAG_PAGES, Tag::Type::List)) {
        user_data.put(WrittenBookItem::TAG_PAGES, std::make_unique<ListTag>());
    }
    return *user_data.getList(WrittenBookItem::TAG_PAGES);
}
}  // namespace

void WritableBookItem::signBook(ItemStack &book, std::string title, std::optional<std::string> filtered_title,
                                std::string author, std::optional<std::string> filtered_author, std::string xuid)
{
    if (!isBookAndQuill(book) || book.getUserData() == nullptr) {
        book = ItemStack();
        return;
    }
    auto &user_data = *book.getUserData();
    _signBookUserData(user_data, std::move(title), std::move(filtered_title), std::move(author),
                      std::move(filtered_author), std::move(xuid));
    book = ItemStack("minecraft:written_book", 1, 0, &user_data);
}

void WritableBookItem::replacePage(ItemStack &book, int index, const PageContent &page)
{
    if (isBookAndQuill(book)) {
        _replacePage(book, index, page);
    }
}

void WritableBookItem::addPageAt(ItemStack &book, int index, const PageContent &page)
{
    if (isBookAndQuill(book)) {
        _addPageAt(book, index, page);
    }
}

PageContent WritableBookItem::deletePage(ItemStack &book, int index)
{
    if (isBookAndQuill(book)) {
        return _deletePage(book, index);
    }
    return {};
}

void WritableBookItem::swapPages(ItemStack &book, int page1, int page2)
{
    if (!isBookAndQuill(book)) {
        return;
    }
    auto *user_data = book.getUserData();
    if (user_data == nullptr || page1 < 0 || page1 >= WrittenBookItem::MAX_PAGES ||
        static_cast<unsigned>(page2) >= WrittenBookItem::MAX_PAGES) {
        return;
    }
    auto &pages = getOrCreatePages(*user_data);
    if (page1 >= pages.size() || page2 >= pages.size()) {
        return;
    }
    auto *first = pages.getCompound(page1);
    auto *second = pages.getCompound(page2);
    if (first == nullptr || second == nullptr) {
        return;
    }
    const auto first_page = PageContent::read(*first);
    const auto second_page = PageContent::read(*second);
    PageContent::write(first_page, *second);
    PageContent::write(second_page, *first);
}

void WritableBookItem::_replacePage(ItemStackBase &book, int index, const PageContent &page)
{
    auto *user_data = book.getUserData();
    if (user_data == nullptr || index >= WrittenBookItem::MAX_PAGES) {
        return;
    }
    auto &pages = getOrCreatePages(*user_data);
    if (index >= pages.size()) {
        while (pages.size() < index) {
            pages.add(PageContent::BLANK_PAGE.createTag());
        }
        pages.add(page.createTag());
    }
    else if (auto *tag = pages.getCompound(index)) {
        PageContent::write(page, *tag);
    }
}

void WritableBookItem::_addPageAt(ItemStackBase &book, int index, const PageContent &page)
{
    auto *user_data = book.getUserData();
    if (user_data == nullptr || index >= WrittenBookItem::MAX_PAGES) {
        return;
    }
    auto &pages = getOrCreatePages(*user_data);
    if (pages.size() >= WrittenBookItem::MAX_PAGES) {
        return;
    }
    if (index >= pages.size()) {
        while (pages.size() < index) {
            pages.add(PageContent::BLANK_PAGE.createTag());
        }
        pages.add(page.createTag());
        return;
    }
    auto *tag = pages.getCompound(index);
    if (tag == nullptr) {
        return;
    }
    auto carried = PageContent::read(*tag);
    PageContent::write(page, *tag);
    for (auto i = index + 1; i < pages.size(); ++i) {
        if (auto *next = pages.getCompound(i)) {
            auto current = PageContent::read(*next);
            PageContent::write(carried, *next);
            carried = std::move(current);
        }
    }
    pages.add(carried.createTag());
}

PageContent WritableBookItem::_deletePage(ItemStackBase &book, int index)
{
    auto *user_data = book.getUserData();
    const auto pages = WrittenBookItem::getPages(book);
    if (user_data == nullptr || index < 0 || index >= static_cast<int>(pages.size())) {
        return {};
    }
    auto deleted = pages[index];
    auto list = std::make_unique<ListTag>();
    for (auto i = 0; i < static_cast<int>(pages.size()); ++i) {
        if (i != index) {
            list->add(pages[i].createTag());
        }
    }
    user_data->put(WrittenBookItem::TAG_PAGES, std::move(list));
    return deleted;
}

void WritableBookItem::_signBookUserData(CompoundTag &user_data, std::string title,
                                         std::optional<std::string> filtered_title, std::string author,
                                         std::optional<std::string> filtered_author, std::string xuid)
{
    user_data.putString("author", std::move(author));
    if (filtered_author.has_value()) {
        user_data.putString("filteredAuthor", std::move(*filtered_author));
    }
    else {
        user_data.remove("filteredAuthor");
    }
    user_data.putString("xuid", std::move(xuid));
    user_data.putString("title", std::move(title));
    if (filtered_title.has_value()) {
        user_data.putString("filteredTitle", std::move(*filtered_title));
    }
    else {
        user_data.remove("filteredTitle");
    }
    user_data.putInt("generation", 0);
}
