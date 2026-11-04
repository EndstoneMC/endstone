import { system, world } from "@minecraft/server";

const handlers = {
  // scriptevent endstone_test:setblock <x> <y> <z> <block>
  "endstone_test:setblock": ([x, y, z, block]) => {
    world.getDimension("overworld").getBlock({ x: Number(x), y: Number(y), z: Number(z) })?.setType(block);
  },
  // scriptevent endstone_test:explode <x> <y> <z> <radius>
  "endstone_test:explode": ([x, y, z, radius]) => {
    world.getDimension("overworld").createExplosion({ x: Number(x), y: Number(y), z: Number(z) }, Number(radius));
  },
};

system.afterEvents.scriptEventReceive.subscribe((event) => {
  handlers[event.id]?.(event.message.split(" "));
});
