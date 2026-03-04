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

#include "bedrock/world/item/written_book_item.h"
#include "endstone/core/inventory/meta/item_meta.h"

namespace endstone::core {

template <typename Derived, typename Interface>
class EndstoneBookMetaBase : public EndstoneItemMetaBase<Interface> {
    using Base = EndstoneItemMetaBase<Interface>;

public:
    explicit EndstoneBookMetaBase(const ItemMeta *meta) : Base(meta) {}
    explicit EndstoneBookMetaBase(const ::CompoundTag &tag) : Base(tag)
    {
        if (const auto *pages_list = tag.getList(WrittenBookItem::TAG_PAGES)) {
            pages_list->forEachCompoundTag([&](const ::CompoundTag &page_tag) {
                pages_.emplace_back(page_tag.getString(WrittenBookItem::TAG_PAGE_TEXT));
            });
        }
    }

    [[nodiscard]] bool hasPages() const override { return !pages_.empty(); }

    [[nodiscard]] std::string getPage(int page) const override
    {
        if (page < 1 || page > static_cast<int>(pages_.size())) {
            throw std::out_of_range("Page index out of range");
        }
        return pages_[page - 1];
    }

    void setPage(int page, std::string data) override
    {
        if (page < 1 || page > static_cast<int>(pages_.size())) {
            throw std::out_of_range("Page index out of range");
        }
        pages_[page - 1] = validatePage(std::move(data));
    }

    [[nodiscard]] std::vector<std::string> getPages() const override { return pages_; }

    void setPages(std::vector<std::string> pages) override
    {
        pages_.clear();
        for (auto &page : pages) {
            internalAddPage(validatePage(std::move(page)));
        }
    }

    void addPages(std::vector<std::string> pages) override
    {
        for (auto &page : pages) {
            internalAddPage(validatePage(std::move(page)));
        }
    }

    [[nodiscard]] int getPageCount() const override { return static_cast<int>(pages_.size()); }

    void applyToItem(::CompoundTag &tag) const override
    {
        Base::applyToItem(tag);
        tag.remove(WrittenBookItem::TAG_PAGES);
        if (hasPages()) {
            auto list = std::make_unique<::ListTag>();
            for (const auto &page : pages_) {
                auto page_tag = std::make_unique<::CompoundTag>();
                page_tag->putString(WrittenBookItem::TAG_PAGE_TEXT, page);
                page_tag->putString(WrittenBookItem::TAG_PAGE_PHOTO_NAME, "");
                list->add(std::move(page_tag));
            }
            tag.put(WrittenBookItem::TAG_PAGES, std::move(list));
        }
    }

    [[nodiscard]] bool isEmpty() const override { return Base::isEmpty() && isPagesEmpty(); }

    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override
    {
        return std::make_unique<Derived>(static_cast<const Derived &>(*this));
    }

protected:
    static std::string validatePage(std::string page)
    {
        if (static_cast<int>(page.size()) > WrittenBookItem::MAX_PAGE_LENGTH) {
            page.resize(WrittenBookItem::MAX_PAGE_LENGTH);
        }
        return page;
    }

    void internalAddPage(std::string page)
    {
        if (static_cast<int>(pages_.size()) >= WrittenBookItem::MAX_PAGES) {
            return;
        }
        pages_.push_back(std::move(page));
    }

    [[nodiscard]] bool isPagesEmpty() const { return !hasPages(); }

    std::vector<std::string> pages_;
};

}  // namespace endstone::core
