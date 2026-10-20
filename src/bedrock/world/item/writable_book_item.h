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

#include <optional>
#include <string>

#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/page_content.h"

class WritableBookItem : public Item {
public:
    static void signBook(ItemStack &book, std::string title, std::optional<std::string> filtered_title,
                         std::string author, std::optional<std::string> filtered_author, std::string xuid);
    static void replacePage(ItemStack &book, int index, const PageContent &page);
    static void addPageAt(ItemStack &book, int index, const PageContent &page);
    static PageContent deletePage(ItemStack &book, int index);
    static void swapPages(ItemStack &book, int page1, int page2);
    static void _replacePage(ItemStackBase &book, int index, const PageContent &page);
    static void _addPageAt(ItemStackBase &book, int index, const PageContent &page);
    static PageContent _deletePage(ItemStackBase &book, int index);
    static void _signBookUserData(CompoundTag &user_data, std::string title, std::optional<std::string> filtered_title,
                                  std::string author, std::optional<std::string> filtered_author, std::string xuid);
};
