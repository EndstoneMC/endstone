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
#include <vector>

#include "endstone/inventory/meta/item_meta.h"

namespace endstone {
/**
 * @brief Represents the meta for a writable book that can have pages
 */
class WritableBookMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(WritableBook)

    /**
     * @brief Checks for the existence of pages in the book.
     *
     * @return true if the book has pages
     */
    [[nodiscard]] virtual bool hasPages() const = 0;

    /**
     * @brief Gets the specified page in the book. The given page must exist.
     *
     * @note Pages are 1-indexed.
     *
     * @param page the page number to get, in range [1, getPageCount()]
     * @return the page from the book
     */
    [[nodiscard]] virtual std::string getPage(int page) const = 0;

    /**
     * @brief Sets the specified page in the book. Pages of the book must be contiguous.
     *
     * @note The data can be up to 1024 characters in length, additional characters are truncated.
     *
     * Pages are 1-indexed.
     *
     * @param page the page number to set, in range [1, getPageCount()]
     * @param data the data to set for that page
     */
    virtual void setPage(int page, std::string data) = 0;

    /**
     * @brief Gets all the pages in the book.
     *
     * @return list of all the pages in the book
     */
    [[nodiscard]] virtual std::vector<std::string> getPages() const = 0;

    /**
     * @brief Clears the existing book pages, and sets the book to use the provided pages.
     *
     * @note Maximum 50 pages with 798 characters per page.
     *
     * @param pages A list of pages to set the book to use
     */
    template <std::convertible_to<std::string>... Args>
    void setPages(Args &&...pages)
    {
        setPages(std::vector<std::string>{&pages...});
    }

    /**
     * @note Clears the existing book pages, and sets the book to use the provided pages.
     *
     * @brief Maximum 50 pages with 798 characters per page.
     *
     * @param pages A list of strings, each being a page
     */
    virtual void setPages(std::vector<std::string> pages) = 0;

    /**
     * @brief Adds new pages to the end of the book.
     *
     * @note Up to a maximum of 50 pages with 798 characters per page.
     *
     * @param pages A list of strings, each being a page
     */
    template <std::convertible_to<std::string>... Args>
    void addPage(Args &&...pages)
    {
        addPage(std::vector<std::string>{&pages...});
    }

    /**
     * @brief Adds new pages to the end of the book.
     *
     * @note Up to a maximum of 50 pages with 798 characters per page.
     *
     * @param pages A list of strings, each being a page
     */
    virtual void addPage(std::vector<std::string> pages) = 0;

    /**
     * @brief Gets the number of pages in the book.
     *
     * @return the number of pages in the book
     */
    [[nodiscard]] virtual int getPageCount() const = 0;
};
}  // namespace endstone
