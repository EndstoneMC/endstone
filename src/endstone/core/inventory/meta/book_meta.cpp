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

#include "endstone/core/inventory/meta/book_meta.h"

#include "bedrock/world/item/written_book_item.h"

namespace endstone::core {
EndstoneBookMeta::EndstoneBookMeta(const ItemMeta *meta) : Base(meta)
{
    if (!meta) {
        return;
    }
    if (meta->as<BookMeta>()) {
        auto *m = static_cast<const EndstoneBookMeta *>(meta);
        pages_ = m->pages_;
        title_ = m->title_;
        author_ = m->author_;
        generation_ = m->generation_;
    }
    else if (meta->as<WritableBookMeta>()) {
        pages_ = static_cast<const WritableBookMeta *>(meta)->getPages();
    }
}

EndstoneBookMeta::EndstoneBookMeta(const ::CompoundTag &tag) : Base(tag)
{
    if (const auto &title = tag.getString(WrittenBookItem::TAG_TITLE); !title.empty()) {
        title_ = title;
    }
    if (const auto &author = tag.getString(WrittenBookItem::TAG_AUTHOR); !author.empty()) {
        author_ = author;
    }
    if (const auto generation = tag.getInt(WrittenBookItem::TAG_GENERATION)) {
        generation_ = static_cast<BookMeta::Generation>(generation);
    }
}

bool EndstoneBookMeta::hasTitle() const
{
    return !title_.empty();
}

std::string EndstoneBookMeta::getTitle() const
{
    return title_;
}

void EndstoneBookMeta::setTitle(std::optional<std::string> title)
{
    if (!title.has_value()) {
        title_.clear();
        return;
    }
    if (static_cast<int>(title->size()) > WrittenBookItem::MAX_TITLE_LENGTH) {
        title->resize(WrittenBookItem::MAX_TITLE_LENGTH);
    }
    title_ = std::move(*title);
}

bool EndstoneBookMeta::hasAuthor() const
{
    return !author_.empty();
}

std::string EndstoneBookMeta::getAuthor() const
{
    return author_;
}

void EndstoneBookMeta::setAuthor(std::optional<std::string> author)
{
    author_ = std::move(author.value_or(""));
}

bool EndstoneBookMeta::hasGeneration() const
{
    return generation_.has_value();
}

std::optional<BookMeta::Generation> EndstoneBookMeta::getGeneration() const
{
    return generation_;
}

void EndstoneBookMeta::setGeneration(std::optional<BookMeta::Generation> generation)
{
    generation_ = generation;
}

void EndstoneBookMeta::applyToItem(::CompoundTag &tag) const
{
    Base::applyToItem(tag);

    tag.remove(WrittenBookItem::TAG_TITLE);
    if (hasTitle()) {
        tag.putString(WrittenBookItem::TAG_TITLE, title_);
    }

    tag.remove(WrittenBookItem::TAG_AUTHOR);
    if (hasAuthor()) {
        tag.putString(WrittenBookItem::TAG_AUTHOR, author_);
    }

    tag.remove(WrittenBookItem::TAG_GENERATION);
    if (hasGeneration()) {
        tag.putInt(WrittenBookItem::TAG_GENERATION, static_cast<int>(*generation_));
    }
}

bool EndstoneBookMeta::isEmpty() const
{
    return Base::isEmpty() && isBookEmpty();
}

bool EndstoneBookMeta::equalsCommon(const ItemMeta &meta) const
{
    if (!EndstoneItemMetaBase<BookMeta>::equalsCommon(meta)) {
        return false;
    }
    if (meta.as<BookMeta>()) {
        auto &that = static_cast<const EndstoneBookMeta &>(meta);
        return (hasPages() ? that.hasPages() && pages_ == that.pages_ : !that.hasPages())
            && (hasTitle() ? that.hasTitle() && title_ == that.title_ : !that.hasTitle())
            && (hasAuthor() ? that.hasAuthor() && author_ == that.author_ : !that.hasAuthor())
            && (hasGeneration() ? that.hasGeneration() && generation_ == that.generation_ : !that.hasGeneration());
    }
    if (meta.as<WritableBookMeta>()) {
        auto *writable = static_cast<const WritableBookMeta *>(&meta);
        return (hasPages() ? writable->hasPages() && pages_ == writable->getPages() : !writable->hasPages());
    }
    return true;
}

bool EndstoneBookMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<BookMeta>() || isBookEmpty());
}

bool EndstoneBookMeta::isBookEmpty() const
{
    return isPagesEmpty() && !hasTitle() && !hasAuthor() && !hasGeneration();
}
}  // namespace endstone::core
