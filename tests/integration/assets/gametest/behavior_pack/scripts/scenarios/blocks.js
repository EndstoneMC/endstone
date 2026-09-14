import { Direction, ItemStack } from "@minecraft/server";
import {
  after,
  registerScenario,
  simulatedPlayer,
  worldPositionFor,
} from "../framework.js";

registerScenario("block_explode", (test, dimension, done) => {
  const relative = { x: 2, y: 1, z: 2 };
  test.setBlockType("minecraft:stone", relative);
  const origin = worldPositionFor(test, relative);
  dimension.createExplosion(origin, 1, { breaksBlocks: true });
  after(test, 5, () => done());
});

registerScenario("block_explode_cancel", (test, dimension, done) => {
  const relative = { x: 2, y: 1, z: 2 };
  test.setBlockType("minecraft:stone", relative);
  const origin = worldPositionFor(test, relative);
  dimension.createExplosion(origin, 1, { breaksBlocks: true });
  after(test, 5, () => done());
});

registerScenario("simulated_block_interact", (test, dimension, done) => {
  const target = { x: 2, y: 2, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  dimension.runCommand(
    `setblock ${targetWorld.x} ${targetWorld.y} ${targetWorld.z} stone`,
  );

  const player = simulatedPlayer(test, "EndstoneSimInteract");
  player.setItem(new ItemStack("minecraft:stick", 1), 0, true);
  if (!player.useItemInSlotOnBlock(
    0,
    target,
    Direction.East,
    { x: 1, y: 0.5, z: 0.5 },
  )) {
    throw new Error("useItemInSlotOnBlock returned false for block interaction");
  }
  after(test, 20, done);
});

registerScenario("simulated_block_interact_cancel", (test, dimension, done) => {
  const target = { x: 2, y: 2, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  dimension.runCommand(
    `setblock ${targetWorld.x} ${targetWorld.y} ${targetWorld.z} stone`,
  );

  const player = simulatedPlayer(test, "EndstoneSimInteractCancel");
  player.setItem(new ItemStack("minecraft:stick", 1), 0, true);
  player.useItemInSlotOnBlock(
    0,
    target,
    Direction.East,
    { x: 1, y: 0.5, z: 0.5 },
  );
  after(test, 20, done);
});

registerScenario("simulated_block_place", (test, dimension, done) => {
  const support = { x: 1, y: 1, z: 2 };
  const destination = { x: 2, y: 1, z: 2 };
  const supportWorld = worldPositionFor(test, support);
  const destinationWorld = worldPositionFor(test, destination);
  dimension.runCommand(
    `setblock ${supportWorld.x} ${supportWorld.y} ${supportWorld.z} stone`,
  );
  dimension.runCommand(
    `setblock ${destinationWorld.x} ${destinationWorld.y} ${destinationWorld.z} air`,
  );

  const player = simulatedPlayer(test, "EndstoneSimPlace");
  player.setItem(new ItemStack("minecraft:dirt", 1), 0, true);
  if (!player.useItemInSlotOnBlock(
    0,
    support,
    Direction.East,
    { x: 1, y: 0.5, z: 0.5 },
  )) {
    throw new Error("useItemInSlotOnBlock returned false");
  }

  after(test, 20, done);
});

registerScenario("simulated_block_place_cancel", (test, dimension, done) => {
  const support = { x: 1, y: 1, z: 2 };
  const destination = { x: 2, y: 1, z: 2 };
  const supportWorld = worldPositionFor(test, support);
  const destinationWorld = worldPositionFor(test, destination);
  dimension.runCommand(
    `setblock ${supportWorld.x} ${supportWorld.y} ${supportWorld.z} stone`,
  );
  dimension.runCommand(
    `setblock ${destinationWorld.x} ${destinationWorld.y} ${destinationWorld.z} air`,
  );

  const player = simulatedPlayer(test, "EndstoneSimPlaceCancel");
  player.setItem(new ItemStack("minecraft:dirt", 1), 0, true);
  player.useItemInSlotOnBlock(
    0,
    support,
    Direction.East,
    { x: 1, y: 0.5, z: 0.5 },
  );
  after(test, 20, done);
});

registerScenario("simulated_block_break", (test, dimension, done) => {
  const target = { x: 2, y: 1, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  dimension.runCommand(
    `setblock ${targetWorld.x} ${targetWorld.y} ${targetWorld.z} stone`,
  );

  const player = simulatedPlayer(test, "EndstoneSimBreak");
  player.setItem(new ItemStack("minecraft:diamond_pickaxe", 1), 0, true);
  if (!player.breakBlock(target, Direction.East)) {
    throw new Error("breakBlock returned false");
  }

  after(test, 30, done);
});

registerScenario("simulated_block_break_cancel", (test, dimension, done) => {
  const target = { x: 2, y: 1, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  dimension.runCommand(
    `setblock ${targetWorld.x} ${targetWorld.y} ${targetWorld.z} stone`,
  );

  const player = simulatedPlayer(test, "EndstoneSimBreakCancel");
  player.setItem(new ItemStack("minecraft:diamond_pickaxe", 1), 0, true);
  player.breakBlock(target, Direction.East);
  after(test, 30, done);
});

function simulatedBucketUse(test, dimension, blockType, itemType, name, done) {
  const target = { x: 2, y: 2, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  dimension.runCommand(
    `setblock ${targetWorld.x} ${targetWorld.y} ${targetWorld.z} ${blockType}`,
  );
  const player = simulatedPlayer(test, name);
  player.setItem(new ItemStack(itemType, 1), 0, true);
  player.useItemInSlotOnBlock(
    0,
    target,
    Direction.East,
    { x: 1, y: 0.5, z: 0.5 },
  );
  after(test, 30, done);
}

registerScenario("simulated_bucket_fill", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "water", "minecraft:bucket", "EndstoneSimBucketFill", done);
});

registerScenario("simulated_bucket_fill_cancel", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "water", "minecraft:bucket", "EndstoneSimBucketFillCancel", done);
});

registerScenario("simulated_bucket_empty", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "stone", "minecraft:water_bucket", "EndstoneSimBucketEmpty", done);
});

registerScenario("simulated_bucket_empty_cancel", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "stone", "minecraft:water_bucket", "EndstoneSimBucketEmptyCancel", done);
});

registerScenario("simulated_cauldron_bucket_empty", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "cauldron", "minecraft:water_bucket", "EndstoneSimCauldron", done);
});

registerScenario("simulated_cauldron_bucket_empty_cancel", (test, dimension, done) => {
  simulatedBucketUse(test, dimension, "cauldron", "minecraft:water_bucket", "EndstoneSimCauldronCancel", done);
});
