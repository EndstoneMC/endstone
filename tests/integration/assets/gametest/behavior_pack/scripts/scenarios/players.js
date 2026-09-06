import { Direction, ItemStack } from "@minecraft/server";
import {
  after,
  overworld,
  registerScenario,
  simulatedPlayer,
  worldPositionFor,
} from "../framework.js";

function simulatedBedUse(test, dimension, name, done, stopAfterUse = true) {
  const bed = { x: 2, y: 2, z: 2 };
  const bedWorld = worldPositionFor(test, bed);
  dimension.runCommand("time set night");
  dimension.runCommand(
    `setblock ${bedWorld.x} ${bedWorld.y} ${bedWorld.z} bed ["direction"=3]`,
  );

  const player = simulatedPlayer(test, name, { x: 1, y: 2, z: 2 });
  after(test, 10, () => {
    player.interactWithBlock(bed, Direction.East);
    after(test, 20, () => {
      if (stopAfterUse) {
        player.stopInteracting();
        after(test, 20, done);
      } else {
        done();
      }
    });
  });
}

registerScenario("simulated_bed_enter", (test, dimension, done) => {
  simulatedBedUse(test, dimension, "EndstoneSimBed", done);
});

registerScenario("simulated_bed_enter_cancel", (test, dimension, done) => {
  simulatedBedUse(test, dimension, "EndstoneSimBedCancel", done, false);
});

registerScenario("simulated_inventory_open", (test, dimension, done) => {
  const chest = { x: 2, y: 2, z: 2 };
  const chestWorld = worldPositionFor(test, chest);
  dimension.runCommand(
    `setblock ${chestWorld.x} ${chestWorld.y} ${chestWorld.z} chest`,
  );

  const player = simulatedPlayer(test, "EndstoneSimContainer");
  if (!player.interactWithBlock(chest, Direction.East)) {
    throw new Error("interactWithBlock returned false for chest");
  }
  after(test, 10, done);
});

registerScenario("simulated_inventory_open_cancel", (test, dimension, done) => {
  const chest = { x: 2, y: 2, z: 2 };
  const chestWorld = worldPositionFor(test, chest);
  dimension.runCommand(
    `setblock ${chestWorld.x} ${chestWorld.y} ${chestWorld.z} chest`,
  );

  const player = simulatedPlayer(test, "EndstoneSimContainerCancel");
  player.interactWithBlock(chest, Direction.East);
  after(test, 20, done);
});

function simulatedEntityInteraction(test, entityIdentifier, itemIdentifier, name) {
  const player = simulatedPlayer(test, name);
  const entitySupport = worldPositionFor(test, { x: 2, y: 1, z: 2 });
  overworld().runCommand(
    `setblock ${entitySupport.x} ${entitySupport.y} ${entitySupport.z} stone`,
  );
  const entity = test.spawn(entityIdentifier, { x: 2, y: 2, z: 2 });
  if (!entity) throw new Error(`spawn failed for ${entityIdentifier}`);
  player.setItem(new ItemStack(itemIdentifier, 1), 0, true);
  if (typeof player.lookAtEntity === "function") {
    player.lookAtEntity(entity);
  }
  return { player, entity, entityIdentifier };
}

function interactWithEntityAfterSpawn(test, interaction, done) {
  // Do not retry on false; the event recorder is authoritative and retries duplicate interactions.
  after(test, 1, () => {
    try {
      if (typeof interaction.player.lookAtEntity === "function") {
        interaction.player.lookAtEntity(interaction.entity);
      }
      interaction.player.interactWithEntity(interaction.entity);
      after(test, 2, done);
    } catch (error) {
      done(false, error);
    }
  });
}

registerScenario("simulated_armor_stand_manipulate", (test, dimension, done) => {
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:armor_stand",
    "minecraft:leather_helmet",
    "EndstoneSimArmor",
  );
  interactWithEntityAfterSpawn(test, interaction, done);
});

registerScenario("simulated_armor_stand_manipulate_cancel", (test, dimension, done) => {
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:armor_stand",
    "minecraft:leather_helmet",
    "EndstoneSimArmorCancel",
  );
  interactWithEntityAfterSpawn(test, interaction, done, true);
});

registerScenario("simulated_bucket_actor", (test, dimension, done) => {
  const water = worldPositionFor(test, { x: 2, y: 2, z: 2 });
  dimension.runCommand(`setblock ${water.x} ${water.y} ${water.z} water`);
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:cod",
    "minecraft:water_bucket",
    "EndstoneSimBucket",
  );
  interactWithEntityAfterSpawn(test, interaction, done);
});

registerScenario("simulated_bucket_actor_cancel", (test, dimension, done) => {
  const water = worldPositionFor(test, { x: 2, y: 2, z: 2 });
  dimension.runCommand(`setblock ${water.x} ${water.y} ${water.z} water`);
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:cod",
    "minecraft:water_bucket",
    "EndstoneSimBucketCancel",
  );
  interactWithEntityAfterSpawn(test, interaction, done, true);
});

registerScenario("simulated_shear_actor", (test, dimension, done) => {
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:sheep",
    "minecraft:shears",
    "EndstoneSimShear",
  );
  interactWithEntityAfterSpawn(test, interaction, done);
});

registerScenario("simulated_shear_actor_cancel", (test, dimension, done) => {
  const interaction = simulatedEntityInteraction(
    test,
    "minecraft:sheep",
    "minecraft:shears",
    "EndstoneSimShearCancel",
  );
  interactWithEntityAfterSpawn(test, interaction, done, true);
});

registerScenario("simulated_item_consume", (test, dimension, done) => {
  const player = simulatedPlayer(test, "EndstoneSimConsume");
  player.setItem(new ItemStack("minecraft:apple", 1), 0, true);
  if (!player.useItemInSlot(0)) {
    throw new Error("useItemInSlot returned false for apple");
  }
  after(test, 40, done);
});

registerScenario("simulated_item_consume_cancel", (test, dimension, done) => {
  const player = simulatedPlayer(test, "EndstoneSimConsumeCancel");
  player.setItem(new ItemStack("minecraft:apple", 1), 0, true);
  if (!player.useItemInSlot(0)) {
    throw new Error("useItemInSlot returned false for apple");
  }
  after(test, 40, done);
});
