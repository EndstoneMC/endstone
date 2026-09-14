"use strict";

const fs = require("fs");
const { TARGET_GAME_VERSION, TARGET_PROTOCOL_VERSION } = require("../exact_protocol");
const { driverAckPath, parseArgs } = require("./config");
const { vector2, vector3 } = require("./protocol");
const connection = require("./connection");
const report = require("./report");
const basic = require("./actions/basic");
const crafting = require("./actions/crafting");
const block = require("./actions/block");
const container = require("./actions/container");
const playerActions = require("./actions/player_actions");
const inventory = require("./actions/inventory");
const actor = require("./actions/actor");
const respawn = require("./actions/respawn");
const { runAction } = require("./actions");

class EventBot {
  constructor(options) {
    this.options = options;
    this.client = null;
    this.connected = false;
    this.spawned = false;
    this.closed = false;
    this.connecting = false;
    this.retryTimer = null;
    this.heartbeatTimer = null;
    this.respawnTimer = null;
    this.reportTimer = null;
    this.deadline = Date.now() + options.timeoutMs;
    this.lastScenario = null;
    this.actionInFlight = null;
    this.actionInFlightScenario = null;
    this.entityId = 0n;
    this.position = vector3(0, 80, 0);
    this.rotation = vector2(0, 0);
    // Echo the PlayerList UUID in PlayerSkinPacket; Bedrock UUID wire order differs from the login identity
    // string.
    this.serverUuid = null;
    this.serverSkin = null;
    this.startGameReceived = false;
    this.serverAuthoritativeBlockBreaking = false;
    this.earlySkinSent = false;
    this.respawnRequested = false;
    this.deathInfoReceived = false;
    this.pendingRespawnPacket = null;
    this.respawnActionSent = false;
    this.respawnAckKey = null;
    this.respawnHandshakeAcked = false;
    this.respawnFallbackTimer = null;
    this.deathRespawnSent = false;
    this.respawnFinalTimer = null;
    this.tick = 0n;
    this.pendingHandledTeleport = false;
    this.actors = new Map();
    this.actorPositions = new Map();
    this.blockRuntimeIds = new Map();
    this.inventoryItems = new Map();
    this.craftingRecipes = [];
    this.selectedSlot = 0;
    this.itemStackRequestId = -1;
    // Start the legacy inventory request counter at -10 to match the captured client sequence.
    this.legacyRequestId = -10;
    this.containerWindowId = null;
    this.containerType = null;
    this.actorEntities = new Map();
    this.failed = false;
    this.authInputCount = 0;
    this.actorPacketLogCount = 0;
  }


  start() {
    console.log(
      `Starting exact Bedrock bot ${this.options.username} for game ${TARGET_GAME_VERSION} (protocol ${TARGET_PROTOCOL_VERSION}).`,
    );
    try {
      fs.unlinkSync(driverAckPath(this.options.resultFile));
    } catch (error) {
      if (error && error.code !== "ENOENT") throw error;
    }
    this.connect();
    this.reportTimer = setInterval(() => this.pollReport(), 100);
  }

  fail(message) {
    if (this.failed) return;
    this.failed = true;
    console.error(`[bedrock] driver failure: ${message}`);
  }

  close() {
    this.closed = true;
    if (this.reportTimer) clearInterval(this.reportTimer);
    if (this.heartbeatTimer) clearInterval(this.heartbeatTimer);
    if (this.retryTimer) clearTimeout(this.retryTimer);
    if (this.respawnTimer) clearTimeout(this.respawnTimer);
    if (this.respawnFinalTimer) clearTimeout(this.respawnFinalTimer);
    if (this.respawnFallbackTimer) clearTimeout(this.respawnFallbackTimer);
    if (this.client) this.client.close();
  }
}

Object.assign(
  EventBot.prototype,
  connection,
  report,
  basic,
  crafting,
  block,
  container,
  playerActions,
  inventory,
  actor,
  respawn,
  { runAction },
);

module.exports = { EventBot, parseArgs };
