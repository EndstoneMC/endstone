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

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace endstone {
/**
 * @brief Represents the metadata of a generic item.
 */
class ItemMeta : public std::enable_shared_from_this<ItemMeta> {
public:
    static ItemMeta EMPTY;

    enum class Type {
        Item = 0,
        Map = 1,
        Count = 2,
        None = Item,
    };

    ItemMeta() = default;
    explicit ItemMeta(const ItemMeta *meta)
    {
        if (meta == nullptr) {
            return;
        }
        *this = *meta;
    }

    virtual ~ItemMeta() = default;

    /**
     * @brief Gets the type of this item meta
     *
     * @return type of this item meta
     */
    [[nodiscard]] virtual Type getType() const
    {
        return Type::Item;
    }

    /**
     * @brief Checks if the item metadata is empty.
     *
     * @return true if the metadata is empty, false otherwise.
     */
    [[nodiscard]] virtual bool isEmpty() const
    {
        // TODO(item): more checks here
        return !hasLore();
    }

    /**
     * @brief Creates a clone of the current metadata.
     *
     * @return A copy of the metadata containing the same state as the original.
     */
    [[nodiscard]] virtual std::shared_ptr<ItemMeta> clone() const
    {
        return std::make_shared<ItemMeta>(*this);
    }

    /**
     * @brief Checks for existence of lore.
     *
     * @return true if this has lore
     */
    [[nodiscard]] virtual bool hasLore() const
    {
        return lore_.has_value() && !lore_.value().empty();
    }

    /**
     * @brief Gets the lore that is set.
     *
     * @return a list of lore that is set
     */
    [[nodiscard]] virtual std::optional<std::vector<std::string>> getLore() const
    {
        if (!hasLore()) {
            return std::nullopt;
        }
        return lore_;
    }

    /**
     * @brief Sets the lore for this item or removes lore when given std::nullopt.
     *
     * @param lore the lore that will be set
     */
    virtual void setLore(std::optional<std::vector<std::string>> lore)
    {
        if (!lore.has_value() || !lore_.value().empty()) {
            lore_ = std::nullopt;
        }
        else {
            lore_ = std::move(lore);
        }
    }

private:
    std::optional<std::vector<std::string>> lore_;
};

inline ItemMeta ItemMeta::EMPTY;
}  // namespace endstone
