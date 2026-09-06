"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  async sendContainerOpenClose(data) {
    const position = this.targetBlock(data);
    const face = data && data.face !== undefined ? Number(data.face) : 1;
    await wait(700);
    const savedRotation = this.rotation;
    this.rotation = this.aimAtFace(position, face);
    this.sendAuthInput([], vector2(0, 0), this.position);
    await wait(100);
    const blockRuntimeId = data && data.block_runtime_id !== undefined
      ? Number(data.block_runtime_id)
      : await this.waitForBlockRuntimeId(position);
    if (!Number.isInteger(blockRuntimeId) || blockRuntimeId === 0) {
      this.rotation = savedRotation;
      throw new Error("container target did not provide a valid block runtime ID");
    }
    console.log(
      `[bedrock] container open target=${JSON.stringify(position)} face=${face} ` +
        `runtime=${blockRuntimeId}`,
    );
    this.containerWindowId = null;
    // Use standalone InventoryTransaction by default; the packed PlayerAuthInput form remains available for A/B
    // testing.
    this.client.queue("player_action", {
      runtime_entity_id: this.entityId,
      action: "start_item_use_on",
      position,
      // ResultPos is the clicked block; the adjacent position is only used for placement.
      result_position: position,
      face,
    });
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
          "place",
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
          "place",
          blockRuntimeId,
          false,
          "player_input",
          // An empty-hand container click has no inventory delta; adding one makes the transaction unbalanced.
          [],
        ),
      });
    } else {
      throw new Error(`unsupported container interaction transport: ${transport}`);
    }
    await wait(100);
    this.sendAuthInput([], vector2(0, 0), this.position);
    for (let attempt = 0; attempt < 30 && this.containerWindowId === null; attempt += 1) {
      await wait(100);
    }
    if (this.containerWindowId === null) {
      this.rotation = savedRotation;
      throw new Error("server did not open the prepared container");
    }
    this.client.queue("container_close", {
      window_id: this.containerWindowId,
      window_type: this.containerType || "container",
      server: false,
    });
    await wait(100);
    this.containerWindowId = null;
    this.containerType = null;
    this.rotation = savedRotation;
  }
,
};
