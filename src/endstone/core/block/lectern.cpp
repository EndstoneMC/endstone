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

#include "endstone/check.h"

namespace endstone::core {

EndstoneLectern::EndstoneLectern(const EndstoneBlock &block, ::LecternBlockActor &lectern, bool use_snapshot)
    : EndstoneContainerBase<Lectern>(block, lectern, use_snapshot),
      lectern_(static_cast<::LecternBlockActor &>(*getBlockActor()))
{
}

int EndstoneLectern::getPage() const
{
    return lectern_.getPage();
}

void EndstoneLectern::setPage(int page)
{
    Preconditions::checkArgument(page >= 0, "Page must not be negative, got {}.", page);
    // BDS keeps the page within the book it holds; a lectern with no book has no pages to turn to
    const auto total_pages = lectern_.getTotalPages();
    lectern_.setPage(total_pages > 0 ? std::min(page, total_pages - 1) : 0);
    lectern_.setChanged();
}

}  // namespace endstone::core
