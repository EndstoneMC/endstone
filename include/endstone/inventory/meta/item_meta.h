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
 * @brief Represents the storage mechanism for auxiliary item data.
 */
class ItemMeta : public std::enable_shared_from_this<ItemMeta> {
public:
    enum class Type {
        Base = 0,
        Map = 1,
        Count = 2,
        None = Base,
    };

    virtual ~ItemMeta() = default;

    /**
     * @brief Gets the type of this item meta
     *
     * @return type of this item meta
     */
    [[nodiscard]] virtual Type getType() const
    {
        return Type::Base;
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
}  // namespace endstone
