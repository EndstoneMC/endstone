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
#include <string_view>
#include <vector>

#include "bedrock/world/item/item.h"
#include "bedrock/world/item/page_content.h"

class WrittenBookItem : public Item {
public:
    static const int MAX_TITLE_LENGTH = 16;
    static const int MAX_PAGE_LENGTH = 256;
    static const int MAX_PAGES = 50;
    static const int MAX_GENERATION;
    static const std::string TAG_TITLE;
    static const std::string TAG_AUTHOR;
    static const std::string TAG_XUID;
    static const std::string TAG_PAGES;
    static const std::string TAG_PAGE_TEXT;
    static const std::string TAG_GENERATION;
    static const std::string TAG_PAGE_PHOTO_NAME;
    static const std::string TAG_PAGE_FILTERED_TEXT;
    static const std::string TAG_PAGE_FILTERED_PHOTO_NAME;

    static int getPageCount(const ItemStackBase &book);
    static std::vector<PageContent> getPages(const ItemStackBase &book);

    using Item::Item;
};
