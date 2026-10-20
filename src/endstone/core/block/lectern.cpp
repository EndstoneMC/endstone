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

#include "endstone/core/block/lectern.h"

#include <algorithm>

#include "bedrock/world/item/written_book_item.h"

namespace endstone::core {

EndstoneLectern::EndstoneLectern(const EndstoneBlock &block, ::LecternBlockActor &lectern, bool use_snapshot)
    : EndstoneContainerBase<Lectern>(block, lectern, use_snapshot)
{
}

int EndstoneLectern::getPage() const
{
    return getLectern().getPage();
}

void EndstoneLectern::setPage(int page)
{
    const auto &book = getLectern().getBook();
    const auto *pages = book.hasUserData() ? book.getUserData()->getList(WrittenBookItem::TAG_PAGES) : nullptr;
    const auto page_count = pages != nullptr ? static_cast<int>(pages->size()) : 0;
    getLectern().setPage(std::clamp(page, 0, std::max(page_count - 1, 0)));
    getLectern().setChanged();
}

}  // namespace endstone::core
