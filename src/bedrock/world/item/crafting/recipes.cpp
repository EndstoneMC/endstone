#include "bedrock/world/item/crafting/recipes.h"

#include "bedrock/symbol.h"

Recipes::RecipesMap &Recipes::getRecipesAllTags()
{
    return *reinterpret_cast<RecipesMap *>(reinterpret_cast<char *>(this) + 16);
}

void Recipes::addShapedRecipe(
    std::string recipe_id,  // NOLINT
    std::vector<::ItemInstance> const &result, std::vector<std::string> const &rows,
    std::vector<::Recipes::Type> const &types, std::vector<::HashedString> const &tags, int priority,
    std::function<std::unique_ptr<ShapedRecipe>(
        std::string, int, int, std::vector<RecipeIngredient> const &, std::vector<ItemInstance> const &, HashedString,
        bool, int, mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
        constructor,  // NOLINT
    RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version, bool assume_symmetry)
{
    void (Recipes::*fp)(
        std::string recipe_id, std::vector<::ItemInstance> const &result, std::vector<std::string> const &rows,
        std::vector<::Recipes::Type> const &types, std::vector<::HashedString> const &tags, int priority,
        std::function<std::unique_ptr<ShapedRecipe>(
            std::string, int, int, std::vector<RecipeIngredient> const &, std::vector<ItemInstance> const &,
            HashedString, bool, int, mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
            constructor,
        RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version, bool assume_symmetry) =
        &Recipes::addShapedRecipe;
    BEDROCK_CALL(fp, this, recipe_id, result, rows, types, tags, priority, constructor, unlocking_req, format_version,
                 assume_symmetry);
}

void Recipes::addShapelessRecipe(
    std::string recipe_id,  // NOLINT
    ::ItemInstance const &result, std::vector<::Recipes::Type> const &types, std::vector<::HashedString> const &tags,
    int priority,
    std::function<std::unique_ptr<::ShapelessRecipe>(
        std::string, std::vector<::RecipeIngredient> const &, std::vector<::ItemInstance> const &, HashedString, int,
        mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
        constructor,  // NOLINT
    RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version)
{
    void (Recipes::*fp)(
        std::string recipe_id, ::ItemInstance const &result, std::vector<::Recipes::Type> const &types,
        std::vector<::HashedString> const &tags, int priority,
        std::function<std::unique_ptr<::ShapelessRecipe>(
            std::string, std::vector<::RecipeIngredient> const &, std::vector<::ItemInstance> const &, HashedString,
            int, mce::UUID const *, RecipeUnlockingRequirement const &, SemVersion const &)>
            constructor,
        RecipeUnlockingRequirement const &unlocking_req, SemVersion const &format_version) =
        &Recipes::addShapelessRecipe;
    BEDROCK_CALL(fp, this, recipe_id, result, types, tags, priority, constructor, unlocking_req, format_version);
}

void Recipes::addFurnaceRecipeAuxData(ItemInstance const &input, ItemInstance const &result,
                                      std::vector<::HashedString> const &tags)
{
    void (Recipes::*fp)(ItemInstance const &input, ItemInstance const &result, std::vector<::HashedString> const &tags);
    BEDROCK_CALL(fp, this, input, result, tags);
}
