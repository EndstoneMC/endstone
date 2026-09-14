"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  async sendBedCycle(data) {
    const position = this.targetBlock(data);
    // Bed use is an ItemUse transaction; the start_sleeping PlayerAction is only a state marker.
    const face = data && data.face !== undefined ? Number(data.face) : 1;
    const blockRuntimeId = data && data.block_runtime_id !== undefined
      ? Number(data.block_runtime_id)
      : await this.waitForBlockRuntimeId(position);
    if (!Number.isInteger(blockRuntimeId) || blockRuntimeId === 0) {
      throw new Error("bed target did not provide a valid block runtime ID");
    }
    await wait(700);
    const savedRotation = this.rotation;
    this.rotation = this.aimAtFace(position, face);
    this.sendAuthInput([], vector2(0, 0), this.position);
    await wait(100);
    console.log(
      `[bedrock] bed interaction target=${JSON.stringify(position)} face=${face} ` +
        `runtime=${blockRuntimeId}`,
    );

    if (!data || data.send_use_lifecycle !== false) {
      this.client.queue("player_action", {
        runtime_entity_id: this.entityId,
        action: "start_item_use_on",
        position,
        result_position: position,
        face,
      });
    }
    const actionType = data && data.action_type ? String(data.action_type) : "use";
    const transport = data && data.transport ? String(data.transport) : "standalone";
    if (transport === "packed") {
      this.sendAuthInput(
        ["item_interact"],
        vector2(0, 0),
        this.position,
        undefined,
        undefined,
        this.packedItemUseTransaction(
          position,
          face,
          emptyItem(),
          actionType,
          blockRuntimeId,
          this.selectedSlot,
          false,
          0,
          false,
        ),
      );
    } else if (transport === "standalone") {
      this.client.queue("inventory_transaction", {
        transaction: this.legacyItemUseTransaction(
          position,
          face,
          emptyItem(),
          actionType,
          blockRuntimeId,
          false,
          "player_input",
          [],
        ),
      });
    } else {
      throw new Error(`unsupported bed interaction transport: ${transport}`);
    }
    await wait(100);
    this.sendAuthInput([], vector2(0, 0), this.position);

    // Wait for the sleep state before sending the stop-use action.
    await wait(1000);
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "stop_item_use_on",
      position,
      result_position: vector3(0, 0, 0),
      face: 0,
    });
    await wait(100);
    this.rotation = savedRotation;
  }
,

  async sendBookEdit(data) {
    const slot = Number(data && data.slot !== undefined ? data.slot : 0);
    await wait(700);
    // BookEdit identifies the slot and edit operation; BDS validates the authoritative item.
    console.log(`[bedrock] book edit slot=${slot}`);
    this.client.queue("book_edit", {
      inventory_slot: slot,
      type: "replace_page",
      page_number: 0,
      text: "Endstone automated book edit",
      photo_name: "",
    });
  }
,

  async sendConsumeItem(data) {
    const slot = Number(data && data.slot !== undefined ? data.slot : 0);
    await wait(700);
    const food = await this.waitForItem(slot, undefined, 30);
    if (!food) throw new Error(`consumable item did not synchronize in slot ${slot}`);
    if (slot !== this.selectedSlot) {
      await this.sendItemHeld({ slot, item: food });
      await wait(100);
    }

    const position = blockPosition(this.position);
    const transaction = {
      legacy: { legacy_request_id: 0, legacy_transactions: undefined },
      transaction_type: "item_use",
      actions: [],
      transaction_data: {
        // Use starts the right-click-and-hold action and shares the PlayerAuthInput frame with StartUsingItem.
        action_type: "use",
        trigger_type: "player_input",
        block_position: position,
        // Face is uint8; click-air ignores it, so use the neutral value.
        face: 0,
        hotbar_slot: slot,
        held_item: descriptorItem(food),
        player_pos: vector3(this.position.x, this.position.y, this.position.z),
        click_pos: vector3(0, 0, 0),
        block_runtime_id: 0,
        client_prediction: "success",
        client_cooldown_state: "off",
      },
    };
    console.log(`[bedrock] consume slot=${slot} item=${JSON.stringify(food)}`);
    // Food use uses the packed ItemUse path in the same PlayerAuthInput frame as StartUsingItem.
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "start_item_use_on",
      position,
      result_position: position,
      face: -1,
    });
    this.sendAuthInput(
      ["item_interact", "start_using_item"],
      vector2(0, 0),
      this.position,
      undefined,
      undefined,
      this.packedItemUseTransaction(
        position,
        0,
        food,
        "use",
        0,
        slot,
        true,
      ),
    );
    await wait(1900);
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "stop_item_use_on",
      position,
      result_position: position,
      face: -1,
    });
    this.sendAuthInput([], vector2(0, 0), this.position);
  }
,

  async sendRiptide(data) {
    const slot = Number(data && data.slot !== undefined ? data.slot : 0);
    await wait(700);
    // Wait for /enchant to publish the updated descriptor before starting the use-and-cancel sequence.
    let trident = await this.waitForItem(slot, undefined, 30);
    if (!trident) throw new Error(`trident did not synchronize in slot ${slot}`);
    await this.sendItemHeld({ slot, item: trident });
    const position = blockPosition(this.position);
    const queueItemUse = (inputFlags, playerAction = "start_item_use_on") => {
      // Held use uses ItemUse; Riptide uses the packed PlayerAuthInput representation.
      if (playerAction !== null) {
        this.client.queue("player_action", {
          runtime_entity_id: this.entityId,
          action: playerAction,
          position,
          result_position: position,
          face: -1,
        });
      }
      this.sendAuthInput(
        ["item_interact", ...inputFlags],
        vector2(0, 0),
        this.position,
        undefined,
        undefined,
        this.packedItemUseTransaction(
          position,
          0,
          trident,
          "use",
          0,
          slot,
          true,
          0,
          false,
        ),
      );
    };
    const queueStandaloneItemUseProbe = () => {
      // This packet-30 ItemUse only reconciles the inventory descriptor; the action is canceled and is not Riptide
      // release.
      this.client.queue("player_action", {
        runtime_entity_id: this.entityId,
        action: "start_item_use_on",
        position,
        result_position: position,
        face: -1,
      });
      this.client.queue("inventory_transaction", {
        transaction: this.legacyItemUseTransaction(
          position,
          0,
          trident,
          "use",
          0,
          true,
          "player_input",
          [],
          0,
          undefined,
        ),
      });
      this.sendAuthInput(["start_using_item"], vector2(0, 0), this.position);
    };
    console.log(`[bedrock] riptide slot=${slot} item=${JSON.stringify(trident)}`);
    queueStandaloneItemUseProbe();
    await wait(300);
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "stop_item_use_on",
      position,
      result_position: position,
      face: -1,
    });
    this.sendAuthInput([], vector2(0, 0), this.position);

    let enchantedTrident = null;
    for (let attempt = 0; attempt < 50; attempt += 1) {
      const candidate = this.inventoryItems.get(slot);
      const rawUserData = candidate && candidate.user_data;
      const userData = Buffer.isBuffer(rawUserData)
        ? rawUserData
        : (rawUserData && Array.isArray(rawUserData.data)
          ? Buffer.from(rawUserData.data)
          : null);
      if (isNonEmptyItem(candidate) && userData && userData.includes(Buffer.from("ench"))) {
        enchantedTrident = candidate;
        break;
      }
      await wait(100);
    }
    trident = enchantedTrident;
    if (!trident) throw new Error(`enchanted trident did not synchronize in slot ${slot}`);
    await this.sendItemHeld({ slot, item: trident });
    await wait(100);

    // Start charging with PlayerAuthInput, then finish with packet-30 ItemRelease.
    // BDS invokes TridentItem::releaseUsing for the matching release transaction.
    queueItemUse(["start_using_item"]);
    await wait(1200);
    queueItemUse(["start_spin_attack"], null);
    await wait(100);
    this.client.queue("inventory_transaction", {
      transaction: {
        legacy_request_id: 0,
        legacy_set_item_slots: undefined,
        transaction_presence: true,
        transaction_type: "item_release",
        transaction_data: {
          actions_presence: true,
          actions: [],
          action_type: "release",
          hotbar_slot: slot,
          held_item: descriptorItem(trident, true),
          head_pos: vector3(this.position.x, this.position.y, this.position.z),
        },
      },
    });
    await wait(100);
    this.sendAuthInput([], vector2(0, 0), this.position);
  }
,
};
