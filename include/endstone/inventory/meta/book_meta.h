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

#include "endstone/inventory/meta/writable_book_meta.h"

namespace endstone {
/**
 * @brief Represents the meta for a written book that can have a title, an author, and pages.
 */
class BookMeta : public WritableBookMeta {
public:
    ENDSTONE_ITEM_META_TYPE(Book)

    /**
     * @brief Represents the generation (or level of copying) of a written book
     */
    enum class Generation {
        /**
         * @brief Book written into a book-and-quill. Can be copied. (Default value)
         */
        Original,
        /**
         * @brief Book that was copied from an original. Can be copied.
         */
        CopyOfOriginal,
        /**
         * @brief Book that was copied from a copy of an original. Can't be copied.
         */
        CopyOfCopy,
    };

    /**
     * @brief Checks for the existence of a title in the book.
     *
     * @return true if the book has a title
     */
    [[nodiscard]] virtual bool hasTitle() const = 0;

    /**
     * @brief Gets the title of the book.
     *
     * @note Plugins should check that hasTitle() returns true before calling this method.
     *
     * @return the title of the book
     */
    [[nodiscard]] virtual std::string getTitle() const = 0;

    /**
     * @brief Sets the title of the book.
     *
     * @note Limited to 32 characters. Removes title when given std::nullopt.
     *
     * @param title the title to set
     *
     * @return true if the title was successfully set
     */
    virtual void setTitle(std::optional<std::string> title) = 0;

    /**
     * @brief Checks for the existence of an author in the book.
     *
     * @return true if the book has an author
     */
    [[nodiscard]] virtual bool hasAuthor() const = 0;

    /**
     * @brief Gets the author of the book.
     *
     * @note Plugins should check that hasAuthor() returns true before calling this method.
     *
     * @return the author of the book
     */
    [[nodiscard]] virtual std::string getAuthor() const = 0;

    /**
     * @brief Sets the author of the book. Removes author when given std::nullopt.
     *
     * @param author the author to set
     */
    virtual void setAuthor(std::optional<std::string> author) = 0;

    /**
     * @brief Checks for the existence of generation level in the book.
     *
     * @return true if the book has a generation level
     */
    [[nodiscard]] virtual bool hasGeneration() const = 0;

    /**
     * @brief Gets the generation of the book.
     *
     * @note Plugins should check that hasGeneration() returns true before calling this method.
     *
     * @return the generation of the book
     */
    [[nodiscard]] virtual std::optional<Generation> getGeneration() const = 0;

    /**
     * @brief Sets the generation of the book. Removes generation when given std::nullopt.
     *
     * @param generation the generation to set
     */
    virtual void setGeneration(std::optional<Generation> generation) = 0;
};
}  // namespace endstone
