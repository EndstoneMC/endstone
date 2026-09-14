from collections import Counter

from endstone import Server
from endstone.inventory import (
    BlastingRecipe,
    CampfireRecipe,
    ExactIngredient,
    FurnaceRecipe,
    ItemStack,
    ItemTagIngredient,
    ItemTypeIngredient,
    RecipeIngredient,
    SmokingRecipe,
)

COOKING_RECIPE_TYPES = {
    "blast_furnace": BlastingRecipe,
    "campfire": CampfireRecipe,
    "furnace": FurnaceRecipe,
    "smoker": SmokingRecipe,
    "soul_campfire": CampfireRecipe,
}


def test_recipes_are_reported_once(server: Server) -> None:
    """Verify each loaded recipe appears exactly once."""
    counts = Counter((recipe.tag, recipe.id) for recipe in server.recipes)

    assert counts
    assert [key for key, count in counts.items() if count > 1] == []


def test_cooking_recipes(server: Server) -> None:
    """Verify cooking recipes expose their station, input choice and result."""
    recipes = [recipe for recipe in server.recipes if recipe.tag in COOKING_RECIPE_TYPES]

    assert recipes
    assert {recipe.tag for recipe in recipes} == set(COOKING_RECIPE_TYPES)

    for recipe in recipes:
        assert isinstance(recipe, COOKING_RECIPE_TYPES[recipe.tag])
        assert recipe.id
        assert recipe.result.amount > 0
        assert len(recipe.ingredients) == 1

        ingredient = recipe.input_choice
        assert ingredient is not None
        assert isinstance(ingredient, (ExactIngredient, ItemTypeIngredient, ItemTagIngredient))
        if isinstance(ingredient, ExactIngredient):
            assert ingredient.test(ingredient.item_stack)
        elif isinstance(ingredient, ItemTypeIngredient):
            assert ingredient.test(ItemStack(ingredient.item_type))
        else:
            assert isinstance(ingredient, ItemTagIngredient)
            assert ingredient.item_tag
