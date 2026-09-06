"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  targetBlock(data) {
    if (data && data.position) {
      return {
        x: Number(data.position.x),
        y: Number(data.position.y),
        z: Number(data.position.z),
      };
    }
    const base = blockPosition(this.position);
    base.y = Math.floor(this.position.y - 1.62);
    const offset = data && data.offset ? data.offset : {};
    return {
      x: base.x + Number(offset.x || 0),
      y: base.y + Number(offset.y || 0),
      z: base.z + Number(offset.z || 0),
    };
  }
,

  aimAt(position) {
    const center = vector3(position.x + 0.5, position.y + 0.5, position.z + 0.5);
    return this.aimAtPoint(center);
  }
,

  aimAtFace(position, face) {
    const points = {
      0: vector3(position.x + 0.5, position.y, position.z + 0.5),
      1: vector3(position.x + 0.5, position.y + 1, position.z + 0.5),
      2: vector3(position.x + 0.5, position.y + 0.5, position.z),
      3: vector3(position.x + 0.5, position.y + 0.5, position.z + 1),
      4: vector3(position.x, position.y + 0.5, position.z + 0.5),
      5: vector3(position.x + 1, position.y + 0.5, position.z + 0.5),
    };
    return this.aimAtPoint(points[Number(face)] || vector3(
      position.x + 0.5,
      position.y + 0.5,
      position.z + 0.5,
    ));
  }
,

  aimAtPoint(point) {
    const dx = point.x - this.position.x;
    const dy = point.y - this.position.y;
    const dz = point.z - this.position.z;
    const horizontalDistance = Math.hypot(dx, dz);
    return vector2(
      Math.atan2(-dx, dz) * 180 / Math.PI,
      Math.atan2(-dy, horizontalDistance) * 180 / Math.PI,
    );
  }
,

  clickPositionForFace(position, face) {
    const points = {
      0: vector3(0.5, 0, 0.5),
      1: vector3(0.5, 1, 0.5),
      2: vector3(0.5, 0.5, 0),
      3: vector3(0.5, 0.5, 1),
      4: vector3(0, 0.5, 0.5),
      5: vector3(1, 0.5, 0.5),
    };
    return points[Number(face)] || vector3(0.5, 0.5, 0.5);
  }
,

  buildPositionForFace(position, face) {
    const result = { x: Number(position.x), y: Number(position.y), z: Number(position.z) };
    switch (Number(face)) {
      case 0:
        result.y -= 1;
        break;
      case 1:
        result.y += 1;
        break;
      case 2:
        result.z -= 1;
        break;
      case 3:
        result.z += 1;
        break;
      case 4:
        result.x -= 1;
        break;
      case 5:
        result.x += 1;
        break;
      default:
        break;
    }
    return result;
  }
,

  async waitForBlockRuntimeId(position, attempts = 30) {
    for (let attempt = 0; attempt < attempts; attempt += 1) {
      const runtimeId = this.blockRuntimeIdAt(position);
      if (runtimeId !== 0) return runtimeId;
      await wait(100);
    }
    return 0;
  }
,

  legacyItemUseTransaction(
    position,
    face,
    heldItem,
    actionType = "place",
    blockRuntimeId = 0,
    includeStackId = true,
    triggerType = "player_input",
    actions = [],
    legacyRequestId = 0,
    legacySetItemSlots = undefined,
  ) {
    return {
      // Standalone InventoryTransactionPacket uses r26_u4 Cereal; packed PlayerAuthInput keeps its legacy dialect.
      legacy_request_id: legacyRequestId,
      legacy_set_item_slots: legacySetItemSlots,
      transaction_presence: true,
      transaction_type: "item_use",
      transaction_data: {
        actions_presence: true,
        actions,
        action_type: actionType,
        trigger_type: triggerType,
        block_position: position,
        face,
        hotbar_slot: Number(this.selectedSlot),
        held_item: descriptorItem(heldItem, includeStackId),
        player_pos: vector3(this.position.x, this.position.y, this.position.z),
        click_pos: this.clickPositionForFace(position, face),
        block_runtime_id: Number(blockRuntimeId),
        client_prediction: "success",
        client_cooldown_state: "off",
      },
    };
  }
,

  packedItemUseTransaction(
    position,
    face,
    heldItem,
    actionType = "place",
    blockRuntimeId = 0,
    hotbarSlot = this.selectedSlot,
    includeStackId = true,
    legacyRequestId = undefined,
    includeInventoryAction = true,
  ) {
    // PlayerAuthInput writes the legacy item-use payload as TransactionLegacy, a presence byte,
    // ItemUseInventoryTransaction, and its action list.
    const slot = Number(hotbarSlot);
    const fromItem = cloneItem(heldItem);
    const remainingItem = Number(fromItem.count) > 1
      ? cloneItem(fromItem)
      : emptyItem();
    if (Number(fromItem.count) > 1) remainingItem.count = Number(fromItem.count) - 1;
    const action = {
      source: {
        source_type: "container",
        container_presence: true,
        container_id: 0,
        flag_presence: true,
        flags: undefined,
      },
      slot,
      from_item: descriptorItem(fromItem, includeStackId),
      to_item: descriptorItem(remainingItem, includeStackId),
    };
    return {
      legacy: {
        legacy_request_id: legacyRequestId === undefined && includeInventoryAction
          ? this.nextLegacyRequestId()
          : (legacyRequestId === undefined ? 0 : legacyRequestId),
        legacy_transactions: includeInventoryAction ? [{
          container_id: 29,
          changed_slots: [{ slot_id: slot }],
        }] : undefined,
      },
      item_use_transaction_presence: true,
      item_use_transaction: {
        actions_presence: true,
        actions: includeInventoryAction ? [action] : [],
        action_type: actionType,
        trigger_type: "player_input",
        block_position: position,
        face,
        hotbar_slot: Number(hotbarSlot),
        held_item: descriptorItem(heldItem, includeStackId),
        player_pos: vector3(this.position.x, this.position.y, this.position.z),
        click_pos: this.clickPositionForFace(position, face),
        block_runtime_id: Number(blockRuntimeId),
        client_prediction: "success",
        client_cooldown_state: "off",
      },
    };
  }
,

  async sendBlockInteraction(data) {
    const position = this.targetBlock(data);
    this.sendAuthInput(["block_action"], vector2(0, 0), this.position, {
      action: "start_break",
      position,
      face: 1,
    });
    // End predicted mining before the next block action; an outstanding StartDestroyBlock invalidates later
    // creative-destroy packets.
    await wait(100);
    this.sendAuthInput(["block_action"], vector2(0, 0), this.position, {
      action: "abort_break",
      position,
      face: 1,
    });
  }
,

  async sendBlockPlace(data) {
    const position = this.targetBlock(data);
    const face = data && data.face !== undefined ? Number(data.face) : 1;
    await wait(700);

    const expectedNetworkId = data && data.expected_network_id;
    let heldItem = await this.waitForItem(
      Number(data && data.slot !== undefined ? data.slot : 0),
      expectedNetworkId,
      20,
    );
    if (!heldItem && data && data.fallback_network_id !== undefined) {
      heldItem = fallbackItem(data.fallback_network_id, 1);
    }
    if (!heldItem) throw new Error("block-place item did not synchronize");

    const savedRotation = this.rotation;
    this.rotation = this.aimAtFace(position, face);
    this.sendAuthInput([], vector2(0, 0), this.position);
    await wait(100);
    const observedRuntimeId = await this.waitForBlockRuntimeId(position);
    const blockRuntimeId = data && data.block_runtime_id !== undefined
      ? Number(data.block_runtime_id)
      : observedRuntimeId;
    if (!Number.isInteger(blockRuntimeId) || blockRuntimeId === 0) {
      throw new Error("block-place target did not provide a valid block runtime ID");
    }
    console.log(
      `[bedrock] block place target=${JSON.stringify(position)} face=${face} ` +
        `runtime=${blockRuntimeId} item=${JSON.stringify(heldItem)}`,
    );
    const hadHeartbeat = this.heartbeatTimer !== null;
    if (hadHeartbeat) {
      clearInterval(this.heartbeatTimer);
      this.heartbeatTimer = null;
    }
    try {
      const immediate = data && data.transport === "standalone-write";
      const send = (name, packet) => {
        if (immediate) {
          this.client.write(name, packet);
        } else {
          this.client.queue(name, packet);
        }
      };
      // Send the item-use lifecycle action before the transaction so BDS associates the click with the active
      // use-on-block interaction.
      send("player_action", {
        runtime_entity_id: this.entityId,
        action: "start_item_use_on",
        position,
        result_position: this.buildPositionForFace(position, face),
        face,
      });
      if (data && (data.transport === "standalone" || data.transport === "standalone-write" || data.transport === "combined")) {
        // Block placement requires the packet-30 inventory delta; an empty action list is rejected before
        // GameMode::useItemOn.
        const slot = Number(data && data.slot !== undefined ? data.slot : this.selectedSlot);
        const fromItem = cloneItem(heldItem);
        const remainingItem = Number(fromItem.count) > 1
          ? cloneItem(fromItem)
          : emptyItem();
        if (Number(fromItem.count) > 1) remainingItem.count = Number(fromItem.count) - 1;
        const includeStackId = data && data.include_stack_id !== false;
        const legacyRequestId = this.nextLegacyRequestId();
        send("inventory_transaction", {
          transaction: this.legacyItemUseTransaction(
            position,
            face,
            heldItem,
            "place",
            blockRuntimeId,
            includeStackId,
            "player_input",
            [{
              source: {
                source_type: "container",
                container_presence: true,
                container_id: 0,
                flag_presence: true,
                flags: undefined,
              },
              slot,
              from_item: descriptorItem(fromItem, includeStackId),
              to_item: descriptorItem(remainingItem, includeStackId),
            }],
            legacyRequestId,
            [{ container_id: 29, slots: [slot] }],
          ),
        });
        // Optional A/B path: also attach the transaction to PlayerAuthInput with the item_interact flag.
        if (data.transport === "standalone") {
          const packedTransaction = this.packedItemUseTransaction(
            position,
            face,
            heldItem,
            "place",
            blockRuntimeId,
            slot,
            includeStackId,
            legacyRequestId,
          );
          this.sendAuthInput(
            ["item_interact"],
            vector2(0, 0),
            this.position,
            undefined,
            undefined,
            packedTransaction,
          );
        }
        // Follow block use with the client-like click-air transaction carrying the remaining stack.
        if (Number(remainingItem.count) > 0) {
          const clickAir = this.legacyItemUseTransaction(
            vector3(0, 0, 0),
            255,
            remainingItem,
            "use",
            0,
            false,
            "unknown",
          );
          clickAir.transaction_data.click_pos = vector3(0, 0, 0);
          clickAir.transaction_data.client_prediction = "failure";
          send("inventory_transaction", { transaction: clickAir });
        }
        if (data.transport === "combined") {
          const transaction = this.packedItemUseTransaction(
            position,
            face,
            heldItem,
            "place",
            blockRuntimeId,
            slot,
            includeStackId,
            legacyRequestId,
          );
          this.sendAuthInput(
            ["item_interact"],
            vector2(0, 0),
            this.position,
            undefined,
            undefined,
            transaction,
          );
        }
      } else {
        const transaction = this.packedItemUseTransaction(
          position,
          face,
          heldItem,
          "place",
          blockRuntimeId,
          Number(data && data.slot !== undefined ? data.slot : this.selectedSlot),
          data && data.include_stack_id !== false,
        );
        this.sendAuthInput(
          ["item_interact"],
          vector2(0, 0),
          this.position,
          undefined,
          undefined,
          transaction,
        );
      }
      // Close the use-on-block lifecycle with the captured StopItemUseOn shape after the movement frame consumes
      // the transaction.
      if (data && (data.transport === "standalone" || data.transport === "standalone-write" || data.transport === "combined")) {
        await wait(50);
        send("player_action", {
          runtime_entity_id: this.entityId,
          action: "stop_item_use_on",
          position: this.buildPositionForFace(position, face),
          result_position: vector3(0, 0, 0),
          face: 0,
        });
      }
      await wait(50);
      this.sendAuthInput([], vector2(0, 0), this.position);
    } finally {
      this.rotation = savedRotation;
      if (hadHeartbeat && !this.closed && this.spawned) {
        this.heartbeatTimer = setInterval(() => this.sendAuthInput(), 50);
      }
    }
  }
,

  async sendBlockBreak(data) {
    const position = this.targetBlock(data);
    let synchronizedItem = null;
    if (data && data.requires_item === true) {
      synchronizedItem = await this.waitForItem(this.selectedSlot, undefined, 30);
      if (!synchronizedItem && data.fallback_network_id !== undefined) {
        synchronizedItem = fallbackItem(data.fallback_network_id, 1);
      }
      if (!synchronizedItem) throw new Error("block-break item did not synchronize");
    }
    if (data && data.mode === "creative") {
      await wait(500);
      console.log(`[bedrock] creative break target=${JSON.stringify(position)} runtime=${this.entityId}`);
      // CreativeDestroyBlock uses PlayerActionPacket; PlayerAuthInput start/stop actions cover the authoritative
      // mining path.
      const savedRotation = this.rotation;
      const center = vector3(position.x + 0.5, position.y + 0.5, position.z + 0.5);
      const dx = center.x - this.position.x;
      const dy = center.y - this.position.y;
      const dz = center.z - this.position.z;
      const horizontalDistance = Math.hypot(dx, dz);
      this.rotation = vector2(
        Math.atan2(-dx, dz) * 180 / Math.PI,
        Math.atan2(-dy, horizontalDistance) * 180 / Math.PI,
      );
      console.log(`[bedrock] creative break aim=${JSON.stringify(this.rotation)}`);
      this.sendAuthInput([], vector2(0, 0), this.position);
      await wait(100);
      const face = data && data.face !== undefined ? Number(data.face) : 1;
      this.client.queue("player_action", {
        runtime_entity_id: this.entityId,
        action: "creative_player_destroy_block",
        position,
        // Creative destruction uses the clicked block for both target and result positions.
        result_position: position,
        face,
      });
      this.rotation = savedRotation;
      return;
    }
    // Allow setup changes to reach BDS before block-action validation.
    await wait(500);
    console.log(`[bedrock] break target=${JSON.stringify(position)} runtime=${this.entityId}`);
    const savedRotation = this.rotation;
    const center = vector3(position.x + 0.5, position.y + 0.5, position.z + 0.5);
    const dx = center.x - this.position.x;
    const dy = center.y - this.position.y;
    const dz = center.z - this.position.z;
    const horizontalDistance = Math.hypot(dx, dz);
    this.rotation = vector2(
      Math.atan2(-dx, dz) * 180 / Math.PI,
      Math.atan2(-dy, horizontalDistance) * 180 / Math.PI,
    );
    console.log(`[bedrock] break aim=${JSON.stringify(this.rotation)}`);
    this.sendAuthInput([], vector2(0, 0), this.position);
    await wait(100);
    const face = data && data.face !== undefined ? Number(data.face) : 1;
    this.sendAuthInput(["block_action"], vector2(0, 0), this.position, {
      action: "start_break",
      position,
      face,
    });
    // Send the prediction in the same frame that GameMode finishes the block.
    const predictionFrame = Math.max(
      0,
      Number(data && data.prediction_frame !== undefined ? data.prediction_frame : 3),
    );
    const hadHeartbeat = this.heartbeatTimer !== null;
    if (hadHeartbeat) {
      clearInterval(this.heartbeatTimer);
      this.heartbeatTimer = null;
    }
    try {
      let predicted = false;
      for (let index = 0; index < 20; index += 1) {
        await wait(50);
        if (
          this.serverAuthoritativeBlockBreaking &&
          !predicted &&
          index >= predictionFrame
        ) {
          // Server-authoritative breaking sends continue/predict actions with MineBlock in one auth-input frame.
          predicted = true;
          const actions = [
            { action: "continue_break", position, face },
            { action: "predict_break", position, face },
          ];
          this.sendAuthInput(
            ["block_action"],
            vector2(0, 0),
            this.position,
            actions,
            this.mineBlockRequest(synchronizedItem),
          );
          break;
        }
        if (index < predictionFrame || !this.serverAuthoritativeBlockBreaking) {
          if (!this.serverAuthoritativeBlockBreaking && index === 19) {
            // Without server-authoritative breaking, Destroy is sent in standalone InventoryTransaction.
            this.client.queue("inventory_transaction", {
              transaction: this.legacyItemUseTransaction(
                position,
                face,
                synchronizedItem || this.itemForSlot(this.selectedSlot),
                "destroy",
                this.blockRuntimeIdAt(position),
                true,
                "simulation_tick",
              ),
            });
          } else {
            this.sendAuthInput(
              ["block_action"],
              vector2(0, 0),
              this.position,
              { action: "continue_break", position, face },
            );
          }
        }
      }
      await wait(100);
      this.sendAuthInput(["block_action"], vector2(0, 0), this.position, {
        action: "stop_break",
        position,
        face,
      });
      this.rotation = savedRotation;
    } finally {
      if (hadHeartbeat && !this.closed && this.spawned) {
        this.heartbeatTimer = setInterval(() => this.sendAuthInput(), 50);
      }
    }
  }
,

  legacyBlockBreakTransaction(position, face, heldItem = this.itemForSlot(this.selectedSlot)) {
    return {
      legacy: { legacy_request_id: 0, legacy_transactions: undefined },
      item_use_transaction_presence: true,
      item_use_transaction: {
        actions_presence: true,
        actions: [],
        action_type: "destroy",
        trigger_type: "simulation_tick",
        block_position: position,
        face,
        hotbar_slot: Number(this.selectedSlot),
        held_item: descriptorItem(heldItem),
        player_pos: vector3(this.position.x, this.position.y, this.position.z),
        click_pos: this.clickPositionForFace(position, face),
        // Use the runtime ID observed in UpdateBlock for this target BDS.
        block_runtime_id: this.blockRuntimeIdAt(position),
        client_prediction: "success",
        client_cooldown_state: "off",
      },
    };
  }
,

  mineBlockRequest(item = this.itemForSlot(this.selectedSlot)) {
    return {
      request_id: this.nextItemStackRequestId(),
      actions: [{
        // MineBlock uses variant selector 9 followed by legacy action ID 11.
        type_id: "mine_block",
        legacy_type_id: 11,
        hotbar_slot: Number(this.selectedSlot),
        predicted_durability: 0,
        network_id: item.has_stack_id ? Number(item.stack_id) : 0,
      }],
      custom_names: [],
      cause: "chat_public",
    };
  }
,
};
