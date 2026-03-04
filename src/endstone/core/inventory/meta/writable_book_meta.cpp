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

#include "endstone/core/inventory/meta/writable_book_meta.h"

namespace endstone::core {
EndstoneWritableBookMeta::EndstoneWritableBookMeta(const ItemMeta *meta) : Base(meta)
{
    if (!meta || !meta->as<WritableBookMeta>()) {
        return;
    }
    auto *m = static_cast<const EndstoneWritableBookMeta *>(meta);
    pages_ = m->pages_;
}

EndstoneWritableBookMeta::EndstoneWritableBookMeta(const ::CompoundTag &tag) : Base(tag) {}

bool EndstoneWritableBookMeta::equalsCommon(const ItemMeta &meta) const
{
    if (!EndstoneItemMetaBase<WritableBookMeta>::equalsCommon(meta)) {
        return false;
    }
    if (meta.as<WritableBookMeta>()) {
        auto &that = static_cast<const EndstoneWritableBookMeta &>(meta);
        return (hasPages() ? that.hasPages() && pages_ == that.pages_ : !that.hasPages());
    }
    return true;
}

bool EndstoneWritableBookMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<WritableBookMeta>() || isPagesEmpty());
}
}  // namespace endstone::core
