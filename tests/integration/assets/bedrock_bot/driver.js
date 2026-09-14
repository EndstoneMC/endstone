"use strict";

const { EventBot } = require("./driver/event_bot");
const { parseArgs } = require("./driver/config");

const bot = new EventBot(parseArgs(process.argv.slice(2)));
bot.start();

process.on("SIGINT", () => {
  bot.close();
  process.exit(130);
});
process.on("SIGTERM", () => {
  bot.close();
  process.exit(143);
});
