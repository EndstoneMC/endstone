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

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Called when a player changes recipe book settings.
 */
class PlayerRecipeBookSettingsChangeEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerRecipeBookSettingsChangeEvent);

    /**
     * Represents the type of recipe book whose settings changed.
     */
    enum class RecipeBookType {
        /**
         * The player's crafting recipe book.
         */
        Crafting,
        /** The player's furnace recipe book. */
        Furnace,
        /** The player's blast furnace recipe book. */
        BlastFurnace,
        /** The player's smoker recipe book. */
        Smoker,
    };

    PlayerRecipeBookSettingsChangeEvent(const NotNull<Player> &player, RecipeBookType recipe_book_type, bool is_open,
                                        bool is_filtering)
        : PlayerEvent(player), recipe_book_type_(recipe_book_type), is_open_(is_open), is_filtering_(is_filtering)
    {
    }

    /**
     * Gets the type of recipe book whose settings changed.
     *
     * @return the recipe book type
     */
    [[nodiscard]] RecipeBookType getRecipeBookType() const { return recipe_book_type_; }

    /**
     * Gets whether recipe filtering is enabled.
     *
     * @return true if recipe filtering is enabled
     */
    [[nodiscard]] bool isFiltering() const { return is_filtering_; }

    /**
     * Gets whether the recipe book is open.
     *
     * @return true if the recipe book is open
     */
    [[nodiscard]] bool isOpen() const { return is_open_; }

private:
    RecipeBookType recipe_book_type_;
    bool is_open_;
    bool is_filtering_;
};

}  // namespace endstone
