"use strict";

const ACTIONS = {
  "server-list-ping": (bot, data) => bot.sendServerListPing(data),
  "set-spawn": (bot) => bot.sendCommand("spawnpoint"),
  "toggle-crawl": (bot) => bot.toggleInput("crawling", "start_crawling", "stop_crawling"),
  jump: (bot) => bot.sendJump(),
  "block-place": (bot, data) => bot.sendBlockPlace(data),
  "inventory-open-close": (bot, data) => bot.sendContainerOpenClose(data),
  "bed-cycle": (bot, data) => bot.sendBedCycle(data),
  "edit-book": (bot, data) => bot.sendBookEdit(data),
  "consume-item": (bot, data) => bot.sendConsumeItem(data),
  riptide: (bot, data) => bot.sendRiptide(data),
  chat: (bot) => bot.sendChat("endstone automated player chat"),
  command: (bot) => bot.sendCommand("list"),
  "arm-swing": (bot) => bot.sendArmSwing(),
  move: (bot) => bot.sendMove(),
  input: (bot) => bot.sendInput(),
  "toggle-sneak-start": (bot) => bot.sendToggleInput("start_sneaking"),
  "toggle-sneak-stop": (bot) => bot.sendToggleInput("stop_sneaking"),
  "toggle-sprint-start": (bot) => bot.sendToggleInput("start_sprinting"),
  "toggle-sprint-stop": (bot) => bot.sendToggleInput("stop_sprinting"),
  "toggle-swim": (bot) => bot.toggleInput(null, "start_swimming", "stop_swimming"),
  "toggle-glide": (bot) => bot.toggleInput(null, "start_gliding", "stop_gliding"),
  "toggle-flight-start": (bot) => bot.sendFlightInput("start_flying"),
  "toggle-flight-stop": (bot) => bot.sendFlightInput("stop_flying"),
  "recipe-book": (bot) => bot.sendRecipeBookSettings(),
  craft: (bot, data) => bot.sendCraft(data),
  emote: (bot) => bot.sendEmote(),
  skin: () => Promise.resolve(),
  interact: (bot, data) => bot.sendBlockInteraction(data),
  "block-interact": (bot, data) => bot.sendBlockInteraction(data),
  "block-break": (bot, data) => bot.sendBlockBreak(data),
  "interact-actor": (bot, data) => bot.sendActorInteraction(data),
  "item-held": (bot, data) => bot.sendItemHeld(data),
  "drop-item": (bot, data) => bot.sendDropItem(data),
  respawn: (bot) => bot.prepareRespawn(),
  "await-kick": () => Promise.resolve(),
  "console-command": () => Promise.resolve(),
};

function runAction(action, data) {
  const handler = ACTIONS[action];
  if (!handler) throw new Error(`unknown driver action ${action}`);
  return handler(this, data);
}

module.exports = { runAction };
