"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  itemForSlot(slot) {
    const item = this.inventoryItems.get(slot);
    if (isNonEmptyItem(item)) return item;
    return emptyItem();
  }
,

  hasItemInSlot(slot) {
    return isNonEmptyItem(this.inventoryItems.get(slot));
  }
,

  async waitForItem(slot, expectedNetworkId, attempts = 60, expectedCount = undefined) {
    for (let attempt = 0; attempt < attempts; attempt += 1) {
      const item = this.inventoryItems.get(slot);
      if (isNonEmptyItem(item) &&
          (expectedNetworkId === undefined || Number(item.network_id) === Number(expectedNetworkId)) &&
          (expectedCount === undefined || Number(item.count) === Number(expectedCount))) {
        return item;
      }
      await wait(100);
    }
    return null;
  }
,

  firstPopulatedSlot(preferredSlot) {
    if (this.hasItemInSlot(preferredSlot)) return preferredSlot;
    for (const [slot, item] of this.inventoryItems) {
      if (isNonEmptyItem(item)) {
        return slot;
      }
    }
    return null;
  }
,

  nextItemStackRequestId() {
    const requestId = this.itemStackRequestId;
    this.itemStackRequestId -= 2;
    return requestId;
  }
,

  nextLegacyRequestId() {
    const requestId = this.legacyRequestId;
    this.legacyRequestId -= 2;
    return requestId;
  }
,

  async sendItemHeld(data) {
    await wait(100);
    const slot = Number(data && data.slot !== undefined ? data.slot : 1);
    const expectedNetworkId = data && data.expected_network_id;
    let item = data && data.item ? data.item : this.itemForSlot(slot);
    if (expectedNetworkId !== undefined) {
      item = await this.waitForItem(slot, expectedNetworkId, 60);
      if (!item) {
        throw new Error(
          `held item did not synchronize in slot ${slot} ` +
          `(expected network id ${expectedNetworkId})`,
        );
      }
    }
    if (data && data.requires_item === true && !isNonEmptyItem(item)) {
      throw new Error(`held item is empty in slot ${slot}`);
    }
    this.client.queue("mob_equipment", {
      runtime_entity_id: this.entityId,
      item: descriptorItem(item),
      slot,
      selected_slot: slot,
      window_id: "inventory",
    });
    this.selectedSlot = slot;
  }
,

  async sendDropItem(data) {
    const requestedSlot = data && data.slot !== undefined ? Number(data.slot) : null;
    const preferredSlot = requestedSlot === null ? this.selectedSlot : requestedSlot;
    const expectedNetworkId = data && data.expected_network_id;
    const expectedCount = data && data.expected_count;
    let synchronizedItem = null;
    if (requestedSlot !== null && expectedNetworkId !== undefined) {
      // Wait for the newly prepared item count, not only the item ID.
      synchronizedItem = await this.waitForItem(
        requestedSlot,
        expectedNetworkId,
        60,
        expectedCount,
      );
      if (!synchronizedItem) {
        throw new Error(`inventory slot ${requestedSlot} did not synchronize for drop`);
      }
    }
    for (let attempt = 0; this.firstPopulatedSlot(preferredSlot) === null && attempt < 60; attempt += 1) {
      await wait(100);
    }
    const synchronizedSlot = this.firstPopulatedSlot(preferredSlot);
    // Use the exact prepared stick descriptor as a bounded fallback when no InventorySlot packet arrives.
    const slot = requestedSlot === null
      ? (synchronizedSlot === null ? this.selectedSlot : synchronizedSlot)
      : requestedSlot;
    if (!synchronizedItem) synchronizedItem = this.itemForSlot(slot);
    if (!isNonEmptyItem(synchronizedItem) && expectedNetworkId !== undefined) {
      synchronizedItem = fallbackItem(expectedNetworkId, 1);
    }
    if (!isNonEmptyItem(synchronizedItem)) {
      throw new Error(`inventory slot ${slot} is empty for drop`);
    }
    if (slot !== this.selectedSlot) {
      await this.sendItemHeld({ slot });
      await wait(100);
    }
    console.log(
      `[bedrock] drop slot=${slot} requested=${requestedSlot} ` +
        `selected=${this.selectedSlot}`,
    );

    // Hotbar Q/drop uses a normal InventoryTransaction; PlayerAction::DropItem is only its lifecycle marker.
    this.client.queue("inventory_transaction", {
      transaction: {
        legacy_request_id: this.nextLegacyRequestId(),
        legacy_set_item_slots: [{ container_id: 29, slots: [slot] }],
        transaction_presence: true,
        transaction_type: "normal",
        transaction_data: {
          actions_presence: true,
          actions: [
            worldInteractionAction(synchronizedItem),
            containerInventoryAction(slot, synchronizedItem),
          ],
        },
      },
    });

    // Keep the action packet as the client-like lifecycle marker; it does not perform the drop itself.
    const position = vector3(0, 0, 0);
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "drop_item",
      position,
      result_position: position,
      face: -1,
    });
    const moveAfterDrop = data && data.move_after_drop;
    if (moveAfterDrop && typeof moveAfterDrop === "object") {
      // Move the dropped stack away so the player cannot reclaim it before the mob pickup goal runs.
      this.position = vector3(
        this.position.x + Number(moveAfterDrop.x || 0),
        this.position.y + Number(moveAfterDrop.y || 0),
        this.position.z + Number(moveAfterDrop.z || 0),
      );
      await wait(50);
      this.sendAuthInput([], vector2(0, 0), this.position);
    } else {
      this.sendAuthInput([], vector2(0, 0), this.position);
    }
  }
,
};
