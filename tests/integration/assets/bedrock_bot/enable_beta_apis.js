"use strict";

const fs = require("fs");
const nbt = require("prismarine-nbt");

const levelDat = process.argv[2];
if (!levelDat) {
  console.error("usage: node enable_beta_apis.js <world>/level.dat");
  process.exit(2);
}

const original = fs.readFileSync(levelDat);
const parsed = nbt.protos.little.read(original, 8, "nbt");
const root = parsed.value;
if (!root || root.type !== "compound" || !root.value) {
  throw new Error(`unexpected Bedrock level.dat root: ${root?.type}`);
}

const experiments = root.value.experiments || {
  type: "compound",
  value: {},
};
if (experiments.type !== "compound" || !experiments.value) {
  throw new Error("level.dat experiments tag is not a compound");
}

const byte = (value) => ({ type: "byte", value });
// Bedrock persists the Beta APIs toggle under the `gametest` experiment key.
experiments.value.gametest = byte(1);
experiments.value.experiments_ever_used = byte(1);
experiments.value.saved_with_toggled_experiments = byte(1);
root.value.experiments = experiments;

const payload = nbt.writeUncompressed(root, true);
// Update the level.dat payload length when appending NBT tags.
const header = Buffer.from(original.subarray(0, 8));
header.writeUInt32LE(payload.length, 4);
const rewritten = Buffer.concat([header, payload]);
fs.writeFileSync(levelDat, rewritten);
console.log(`Enabled Beta APIs in ${levelDat}`);
