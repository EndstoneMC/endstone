"use strict";

const fs = require("fs");
const path = require("path");

const DEFAULT_USERNAME = "EndstoneEventBot";
const TERMINAL_STATUSES = new Set([
  "passed",
  "failed",
  "cancelled",
  "infrastructure_failed",
]);
const RAKNET_BACKEND = process.platform === "win32" ? "raknet-native" : "jsp-raknet";

function parseArgs(argv) {
  const options = {
    host: process.env.ENDSTONE_TEST_BOT_HOST || "127.0.0.1",
    port: Number(process.env.ENDSTONE_TEST_BOT_PORT || 19132),
    username: process.env.ENDSTONE_TEST_BOT_NAME || DEFAULT_USERNAME,
    resultFile: process.env.ENDSTONE_TEST_RESULT_FILE || "endstone-event-test-result.json",
    runId: process.env.ENDSTONE_TEST_RUN_ID || null,
    worldName: process.env.ENDSTONE_TEST_WORLD_NAME || null,
    timeoutMs: Number(process.env.ENDSTONE_TEST_BOT_TIMEOUT_MS || 300000),
  };

  for (let index = 0; index < argv.length; index += 1) {
    const argument = argv[index];
    const next = argv[index + 1];
    if (argument === "--host") options.host = next;
    else if (argument === "--port") options.port = Number(next);
    else if (argument === "--username") options.username = next;
    else if (argument === "--result-file") options.resultFile = next;
    else if (argument === "--timeout") options.timeoutMs = Number(next) * 1000;
    else if (argument === "--help") {
      console.log(
        "Usage: node driver.js --result-file result.json [--host 127.0.0.1] [--port 19132]",
      );
      process.exit(0);
    }
    if (argument.startsWith("--")) index += 1;
  }
  options.resultFile = path.resolve(options.resultFile);
  return options;
}

function readReport(resultFile) {
  try {
    const report = JSON.parse(fs.readFileSync(resultFile, "utf8"));
    return report && typeof report === "object" ? report : null;
  } catch {
    return null;
  }
}

function driverAckPath(resultFile) {
  return `${resultFile}.driver.json`;
}

function wait(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

module.exports = { DEFAULT_USERNAME, TERMINAL_STATUSES, RAKNET_BACKEND, parseArgs, readReport, driverAckPath, wait };
