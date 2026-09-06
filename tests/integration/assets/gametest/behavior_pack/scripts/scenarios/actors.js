import { after, registerScenario, worldPositionFor } from "../framework.js";

registerScenario("actor_explode", (test, dimension, done) => {
  const relative = { x: 2, y: 2, z: 2 };
  const origin = worldPositionFor(test, relative);
  const actor = test.spawn("minecraft:chicken", relative);
  dimension.createExplosion(origin, 1, {
    source: actor,
    breaksBlocks: false,
  });
  after(test, 5, () => done());
});

registerScenario("actor_explode_cancel", (test, dimension, done) => {
  const relative = { x: 2, y: 2, z: 2 };
  const origin = worldPositionFor(test, relative);
  const actor = test.spawn("minecraft:chicken", relative);
  dimension.createExplosion(origin, 1, {
    source: actor,
    breaksBlocks: false,
  });
  after(test, 5, () => done());
});

registerScenario("actor_knockback", (test, dimension, done) => {
  const target = { x: 2, y: 2, z: 2 };
  const source = { x: 1, y: 2, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  const sourceActor = test.spawn("minecraft:zombie", source);
  test.spawn("minecraft:zombie", target);
  dimension.createExplosion(targetWorld, 1, {
    source: sourceActor,
    breaksBlocks: false,
  });
  after(test, 5, () => done());
});

registerScenario("actor_knockback_cancel", (test, dimension, done) => {
  const target = { x: 2, y: 2, z: 2 };
  const source = { x: 1, y: 2, z: 2 };
  const targetWorld = worldPositionFor(test, target);
  const sourceActor = test.spawn("minecraft:zombie", source);
  test.spawn("minecraft:zombie", target);
  dimension.createExplosion(targetWorld, 1, {
    source: sourceActor,
    breaksBlocks: false,
  });
  after(test, 5, () => done());
});
