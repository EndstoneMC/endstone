import {
  Direction,
  GameMode,
  ItemStack,
  system,
  world,
} from "@minecraft/server";
import * as GameTest from "@minecraft/server-gametest";

const MESSAGE_ID = "endstone:gametest";
const SUITE = "endstone_events";
const STRUCTURE = "endstone_events:basic";
const MAX_TICKS = 160;
const register = GameTest.register;

if (typeof register !== "function") {
  throw new Error("@minecraft/server-gametest.register is unavailable");
}

export function overworld() {
  return world.getDimension("overworld");
}

function send(message) {
  // Use the command bridge so Endstone consistently observes ScriptMessageEvent across Script API versions.
  const result = overworld().runCommand(`scriptevent ${MESSAGE_ID} ${message}`);
  if (!result || result.successCount < 1) {
    throw new Error(`scriptevent command failed: ${message}`);
  }
}

export function worldPositionFor(test, relative) {
  // Support both `worldPosition` and `worldLocation` across Script API versions.
  const converter =
    typeof test.worldLocation === "function"
      ? test.worldLocation
      : test.worldPosition;
  if (typeof converter !== "function") {
    throw new Error("GameTest helper has no world position conversion method");
  }
  return converter.call(test, relative);
}

export function after(test, ticks, callback) {
  if (typeof test.runAfterDelay === "function") {
    return test.runAfterDelay(ticks, callback);
  }
  return system.runTimeout(callback, ticks);
}

export function registerScenario(name, action) {
  register(SUITE, name, (test) => {
    let completed = false;
    let succeededAfterCleanup = false;
    const reportsAfterCleanup = typeof test.runOnFinish === "function";

    if (reportsAfterCleanup) {
      test.runOnFinish(() => {
        if (succeededAfterCleanup) send(`pass ${name}`);
      });
    }

    const finish = (succeeded, error) => {
      if (completed) return;
      completed = true;
      const detail = String(error || "GameTest scenario failed")
        .replace(/[\r\n|]/g, " ")
        .slice(0, 160);

      if (succeeded) {
        if (reportsAfterCleanup) succeededAfterCleanup = true;
        else send(`pass ${name}`);
        test.succeed();
      } else {
        send(`fail ${name} ${detail}`);
        test.fail(detail);
      }
    };

    const done = (succeeded = true, error = undefined) => {
      try {
        finish(succeeded, error);
      } catch (finishError) {
        // Fail the GameTest when the bridge command fails so the runner does not only report a timeout.
        if (!completed) {
          completed = true;
          test.fail(String(finishError).replace(/[\r\n|]/g, " ").slice(0, 160));
        }
      }
    };

    try {
      send(`start ${name}`);
      action(test, overworld(), done);
    } catch (error) {
      done(false, error);
    }
  })
    .maxTicks(MAX_TICKS)
    .structureName(STRUCTURE);
}

export function simulatedPlayer(test, name, position = { x: 3, y: 2, z: 2 }) {
  if (typeof test.spawnSimulatedPlayer !== "function") {
    throw new Error("GameTest helper has no spawnSimulatedPlayer method");
  }
  const support = worldPositionFor(test, {
    x: position.x,
    y: position.y - 1,
    z: position.z,
  });
  overworld().runCommand(
    `setblock ${support.x} ${support.y} ${support.z} stone`,
  );
  const player = test.spawnSimulatedPlayer(
    position,
    name,
    GameMode.survival,
  );
  if (!player) throw new Error("spawnSimulatedPlayer returned no player");
  return player;
}

export { Direction, GameMode, ItemStack };
