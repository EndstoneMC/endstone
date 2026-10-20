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

#include "bedrock/world/item/page_content.h"

#include "bedrock/world/item/written_book_item.h"

const PageContent PageContent::BLANK_PAGE{};

PageContent PageContent::read(const CompoundTag &tag)
{
    PageContent page;
    page.text = tag.getString(WrittenBookItem::TAG_PAGE_TEXT);
    if (tag.contains(WrittenBookItem::TAG_PAGE_FILTERED_TEXT, Tag::Type::String)) {
        page.filtered_text = tag.getString(WrittenBookItem::TAG_PAGE_FILTERED_TEXT);
    }
    page.photo_name = tag.getString(WrittenBookItem::TAG_PAGE_PHOTO_NAME);
    if (tag.contains(WrittenBookItem::TAG_PAGE_FILTERED_PHOTO_NAME, Tag::Type::String)) {
        page.filtered_photo_name = tag.getString(WrittenBookItem::TAG_PAGE_FILTERED_PHOTO_NAME);
    }
    return page;
}

void PageContent::write(const PageContent &page, CompoundTag &tag)
{
    tag.putString(WrittenBookItem::TAG_PAGE_TEXT, page.text);
    if (page.filtered_text.has_value()) {
        tag.putString(WrittenBookItem::TAG_PAGE_FILTERED_TEXT, *page.filtered_text);
    }
    else {
        tag.remove(WrittenBookItem::TAG_PAGE_FILTERED_TEXT);
    }
    tag.putString(WrittenBookItem::TAG_PAGE_PHOTO_NAME, page.photo_name);
    if (page.filtered_photo_name.has_value()) {
        tag.putString(WrittenBookItem::TAG_PAGE_FILTERED_PHOTO_NAME, *page.filtered_photo_name);
    }
    else {
        tag.remove(WrittenBookItem::TAG_PAGE_FILTERED_PHOTO_NAME);
    }
}

std::unique_ptr<CompoundTag> PageContent::createTag() const
{
    auto tag = std::make_unique<CompoundTag>();
    write(*this, *tag);
    return tag;
}
