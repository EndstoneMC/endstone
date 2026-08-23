from endstone import Server
from endstone.inventory import (
    ExactIngredient,
    FurnaceRecipe,
    ItemStack,
    ItemTypeIngredient,
)


def test_furnace_recipes(server: Server) -> None:
    """Verify furnace recipes expose their station, input and result."""
    recipes = [recipe for recipe in server.recipes if isinstance(recipe, FurnaceRecipe)]

    assert recipes
    assert {recipe.tag for recipe in recipes} == {
        "blast_furnace",
        "campfire",
        "furnace",
        "smoker",
        "soul_campfire",
    }

    for recipe in recipes:
        assert recipe.recipe_id == ""
        assert recipe.result.amount > 0
        assert len(recipe.ingredients) == 1
        assert recipe.input is not None
        assert recipe.input.count == 1

        ingredient = recipe.input
        assert isinstance(ingredient, (ExactIngredient, ItemTypeIngredient))
        if isinstance(ingredient, ExactIngredient):
            assert ingredient.test(ingredient.item_stack)
        else:
            assert ingredient.test(ItemStack(ingredient.item_type))
