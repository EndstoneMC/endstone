"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  sendRecipeBookSettings() {
    this.client.queue("set_player_inventory_options", {
      left_tab: "construction",
      right_tab: "crafting",
      filtering: true,
      layout: "default",
      crafting_layout: "default",
    });
  }
,

  craftRecipeOutputId(recipe) {
    const output = Array.isArray(recipe && recipe.output) ? recipe.output[0] : null;
    return output && output.network_id !== undefined ? Number(output.network_id) : null;
  }
,

  craftRecipeHasInput(recipe, expectedName) {
    if (!expectedName || !Array.isArray(recipe && recipe.input)) return false;
    const wanted = String(expectedName).replace(/^minecraft:/, "");
    return recipe.input.some((ingredient) => {
      const name = ingredient && ingredient.name;
      return typeof name === "string" && name.replace(/^minecraft:/, "") === wanted;
    });
  }
,

  findCraftRecipe(data) {
    const expectedRecipeId = data && data.recipe_id ? String(data.recipe_id) : null;
    const expectedOutputId = data && data.output_network_id !== undefined
      ? Number(data.output_network_id)
      : 5;
    const expectedInputName = data && data.input_name ? String(data.input_name) : "oak_log";
    return this.craftingRecipes.find((recipe) => {
      if (!recipe || Number(recipe.network_id) <= 0) return false;
      if (expectedRecipeId && String(recipe.recipe_id) !== expectedRecipeId) return false;
      if (expectedOutputId !== null && this.craftRecipeOutputId(recipe) !== expectedOutputId) {
        return false;
      }
      return this.craftRecipeHasInput(recipe, expectedInputName);
    }) || null;
  }
,

  craftRecipeIngredients(recipe) {
    const legacyTypes = {
      invalid: 0,
      name: 1,
      molang: 2,
      item_tag: 3,
    };
    const inputs = Array.isArray(recipe && recipe.input) ? recipe.input : [];
    const selectedItem = this.inventoryItems.get(this.selectedSlot);
    return inputs.map((ingredient) => {
      const type = ingredient && ingredient.descriptor_type
        ? String(ingredient.descriptor_type)
        : "name";
      const result = {
        type,
        legacy_type: legacyTypes[type] ?? legacyTypes.name,
        count: Number(ingredient && ingredient.count) || 1,
      };
      if (type === "name") {
        result.name = String(ingredient.name || "");
        result.metadata = selectedItem && selectedItem.metadata !== undefined
          ? Number(selectedItem.metadata)
          : Number(ingredient.metadata ?? 32767);
      } else if (type === "molang") {
        result.expression = String(ingredient.expression || "");
        result.version = Number(ingredient.version || 0);
      } else if (type === "item_tag") {
        result.tag = String(ingredient.tag || "");
      }
      return result;
    });
  }
,

  async openInventoryForCrafting() {
    if (this.containerWindowId !== null) return;
    // Open the player inventory before sending the recipe-book request; otherwise BDS returns
    // InvalidCraftRequest.
    this.client.queue("interact", {
      action_id: "open_inventory",
      target_entity_id: this.entityId,
      has_position: false,
    });
    for (let attempt = 0; attempt < 30 && this.containerWindowId === null; attempt += 1) {
      await wait(100);
    }
    if (this.containerWindowId === null) {
      throw new Error("server did not open the player inventory for crafting");
    }
  }
,

  async sendCraft(data) {
    await this.openInventoryForCrafting();
    let recipe = this.findCraftRecipe(data);
    for (let attempt = 0; !recipe && attempt < 60; attempt += 1) {
      await wait(100);
      recipe = this.findCraftRecipe(data);
    }
    if (!recipe) {
      throw new Error("oak-log crafting recipe did not synchronize");
    }

    const requestId = this.nextItemStackRequestId();
    const recipeNetworkId = Number(recipe.network_id);
    console.log(
      `[bedrock] craft recipe=${recipe.recipe_id} network=${recipeNetworkId} ` +
        `request=${requestId}`,
    );
    console.log(`[bedrock] craft recipe data=${JSON.stringify(recipe)}`);
    // CraftRecipe is sufficient for PlayerCraftItemEvent because Endstone observes it before BDS applies the
    // following inventory transaction.
    this.client.queue("item_stack_request", {
      requests: [{
        request_id: requestId,
        actions: [{
          // Recipe-book crafting uses CraftRecipeAuto (variant 11, legacy byte 13); CraftRecipe is for a populated
          // grid.
          type_id: "craft_recipe_auto",
          legacy_type_id: 13,
          recipe_network_id: recipeNetworkId,
          times_crafted: 1,
          // Reuse the RecipeIngredient2 descriptor and wildcard metadata from the synchronized recipe.
          ingredients: this.craftRecipeIngredients(recipe),
        }],
        custom_names: [],
        cause: "chat_public",
      }],
    });
    await wait(250);
  }
,
};
