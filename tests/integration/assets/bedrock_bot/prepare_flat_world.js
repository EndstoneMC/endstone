"use strict";

const fs = require("fs");
const path = require("path");
const nbt = require("prismarine-nbt");

const levelDat = process.argv[2];
if (!levelDat) {
  console.error("usage: node prepare_flat_world.js <world>/level.dat");
  process.exit(2);
}

const resolvedLevelDat = path.resolve(levelDat);
const original = fs.readFileSync(resolvedLevelDat);
const parsed = nbt.protos.little.read(original, 8, "nbt");
const root = parsed.value;
if (!root || root.type !== "compound" || !root.value) {
  throw new Error(`unexpected Bedrock level.dat root: ${root?.type}`);
}

const tags = root.value;
if (!tags.Generator || tags.Generator.type !== "int") {
  throw new Error("level.dat has no integer Generator tag");
}

// Generator value 2 selects the flat world; keep the layer description explicit because BDS ignores level-type.
tags.Generator.value = 2;
tags.FlatWorldLayers = {
  type: "string",
  value:
    JSON.stringify({
      biome_id: 1,
      block_layers: [
        { block_name: "minecraft:bedrock", count: 1 },
        // The flat generator starts at y=-64; the configured dirt depth puts the grass surface at y=75.
        { block_name: "minecraft:dirt", count: 138 },
        { block_name: "minecraft:grass_block", count: 1 },
      ],
      encoding_version: 6,
      preset_id: null,
      structure_options: null,
      world_version: "version.post_1_18",
    }) + "\n",
};

const setByte = (name, value) => {
  if (!tags[name] || tags[name].type !== "byte") {
    throw new Error(`level.dat has no byte ${name} tag`);
  }
  tags[name].value = value;
};
const setInt = (name, value) => {
  if (!tags[name] || tags[name].type !== "int") {
    throw new Error(`level.dat has no int ${name} tag`);
  }
  tags[name].value = value;
};

// Keep the fixture deterministic while retaining /summon, GameTest, /weather, and entity drops.
setByte("spawnMobs", 0);
setByte("domobspawning", 0);
setByte("doweathercycle", 0);
setByte("dofiretick", 0);
setByte("doinsomnia", 0);
setByte("mobgriefing", 0);
setInt("randomtickspeed", 0);

// Pin the spawn area at y=76 so the bot does not fall into the void before the first scenario.
setInt("SpawnX", 0);
setInt("SpawnY", 76);
setInt("SpawnZ", 0);

const payload = nbt.writeUncompressed(root, true);
// Update the level.dat payload length when appending NBT tags.
const header = Buffer.from(original.subarray(0, 8));
header.writeUInt32LE(payload.length, 4);
const rewritten = Buffer.concat([header, payload]);

// Reuse the same minimal NBT format as enable_beta_apis.js; the server is stopped during the update.
fs.writeFileSync(resolvedLevelDat, rewritten);
console.log(`Prepared superflat test world ${resolvedLevelDat}`);
