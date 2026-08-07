// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <string>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/recipe.h"

namespace endstone {

/**
 * Represents an event that is called when a player clicks a recipe in the recipe book.
 */
class PlayerRecipeBookClickEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerRecipeBookClickEvent);

    PlayerRecipeBookClickEvent(Player &player, RecipeId recipe, bool make_all)
        : Cancellable(player), recipe_(static_cast<std::string>(recipe)), make_all_(make_all)
    {
    }

    /**
     * Gets the recipe identifier clicked by the player.
     *
     * @return the recipe identifier
     */
    [[nodiscard]] RecipeId getRecipe() const { return RecipeId{recipe_}; }

    /**
     * Gets whether the player requested crafting as many copies as possible.
     *
     * @return true if the player requested crafting all possible copies
     */
    [[nodiscard]] bool isMakeAll() const { return make_all_; }

    /**
     * Sets whether the player requests crafting as many copies as possible.
     *
     * @param make_all true to request crafting all possible copies
     */
    void setMakeAll(bool make_all) { make_all_ = make_all; }

private:
    std::string recipe_;
    bool make_all_;
};

}  // namespace endstone
