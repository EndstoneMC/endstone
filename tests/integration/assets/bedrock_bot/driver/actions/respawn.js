"use strict";

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  prepareRespawn() {
    this.respawnRequested = true;
    this.deathInfoReceived = false;
    this.pendingRespawnPacket = null;
    this.respawnActionSent = false;
    this.respawnAckKey = null;
    this.respawnHandshakeAcked = false;
    this.deathRespawnSent = false;
    if (this.respawnFinalTimer) clearTimeout(this.respawnFinalTimer);
    this.respawnFinalTimer = null;
    if (this.respawnFallbackTimer) clearTimeout(this.respawnFallbackTimer);
    this.respawnFallbackTimer = null;
    return Promise.resolve();
  },

  respawn() {
    if (!this.client || !this.entityId || this.respawnActionSent) return;
    this.respawnActionSent = true;
    console.log(
      `[bedrock] sending PlayerAction(respawn) runtime=${this.entityId} ` +
        `position=${JSON.stringify(vector3(0, 0, 0))}`,
    );
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "respawn",
      position: blockPosition(vector3(0, 0, 0)),
      result_position: blockPosition(vector3(0, 0, 0)),
      face: -1,
    });
  }
,

  scheduleDeathRespawn(delayMs) {
    if (this.respawnTimer) clearTimeout(this.respawnTimer);
    this.respawnTimer = setTimeout(() => {
      this.respawnTimer = null;
      this.sendDeathRespawn();
    }, delayMs);
  }
,

  scheduleFinalRespawn(delayMs) {
    if (this.respawnFinalTimer) clearTimeout(this.respawnFinalTimer);
    this.respawnFinalTimer = setTimeout(() => {
      this.respawnFinalTimer = null;
      this.respawn();
    }, delayMs);
  },
};
