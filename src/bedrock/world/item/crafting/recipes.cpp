#include "bedrock/world/item/crafting/recipes.h"
[[nodiscard]] ItemInstance Recipes::getFurnaceRecipeResult(::ItemStackBase const &item, ::HashedString const &tag) const
{
    if (item) {
        if (FurnaceRecipeKey key = {.id = item.getId() << 16 | item.getAuxValue(), .tag = tag};
            furnace_recipes_.contains(key)) {
            return furnace_recipes_.at(key);
        }
        if (FurnaceRecipeKey key = {.id = item.getId() << 16 | 0x7fff, .tag = tag}; furnace_recipes_.contains(key)) {
            return furnace_recipes_.at(key);
        }
    }
    return {};
}
bool Recipes::FurnaceRecipeKey::operator<(const FurnaceRecipeKey &other) const
{
    if (id != other.id) {
        return id < other.id;
    }
    if (tag.getHash() != other.tag.getHash()) {
        return tag.getHash() < other.tag.getHash();
    }
    return tag.getString() < other.tag.getString();
}
