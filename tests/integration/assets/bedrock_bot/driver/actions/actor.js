"use strict";

const { wait } = require("../config");

const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  async sendActorInteraction(data) {
    const requestedType = data && data.actor_type ? data.actor_type : "minecraft:armor_stand";
    // Wait for the actor add/remove packets to settle before selecting the live runtime ID.
    await wait(750);
    let target = this.latestActorRuntime(requestedType);
    for (let attempt = 0; !target && attempt < 10; attempt += 1) {
      await wait(100);
      target = this.latestActorRuntime(requestedType);
    }
    if (!target) throw new Error(`no visible actor with type ${requestedType}`);
    console.log(`[bedrock] actor interaction target=${requestedType} runtime=${target}`);
    // Wait for the authoritative inventory packet before sending ItemUseOnActor.
    // An empty-hand descriptor does not enter the normal actor path on this BDS build.
    const expectedNetworkId = data && data.expected_network_id;
    let synchronizedItem;
    if (data && data.requires_item === true) {
      synchronizedItem = await this.waitForItem(0, expectedNetworkId, 30);
    } else if (expectedNetworkId === undefined) {
      synchronizedItem = emptyItem();
    } else if (Number(expectedNetworkId) === 306) {
      // Allow a short window for InventoryContent/InventorySlot after direct inventory mutations.
      synchronizedItem = await this.waitForItem(0, expectedNetworkId, 20);
    } else {
      synchronizedItem = await this.waitForItem(0, expectedNetworkId, 20);
    }
    // Use the declared item ID as a fallback when no inventory packet arrives.
    if (!synchronizedItem && Number(expectedNetworkId) === 323) {
      synchronizedItem = fallbackStickItem(1);
    }
    if (!synchronizedItem && Number(expectedNetworkId) === 306) {
      synchronizedItem = fallbackDiamondItem(1);
    }
    if (!synchronizedItem) throw new Error("actor interaction item did not synchronize");
    // Reassert slot 0 because scenario setup may not produce a client equipment packet.
    if (!data || data.reassert_selected_slot !== false) {
      await this.sendItemHeld({ slot: 0, item: synchronizedItem });
      await wait(100);
    }
    // Derive yaw and pitch from the actor position so the camera ray passes BDS validation.
    const savedRotation = this.rotation;
    const actorPosition = this.actorPositions.get(String(target));
    let actorRotation = savedRotation;
    const actorHit = vector3(
      Number(data && data.hit_x !== undefined ? data.hit_x : 0),
      Number(data && data.hit_y !== undefined ? data.hit_y : 0.9),
      Number(data && data.hit_z !== undefined ? data.hit_z : 0),
    );
    if (actorPosition) {
      // Keep the camera ray and actor-relative hit point consistent for BDS line-of-sight validation.
      actorRotation = this.aimAtPoint(vector3(
        actorPosition.x + actorHit.x,
        actorPosition.y + actorHit.y,
        actorPosition.z + actorHit.z,
      ));
      console.log(
        `[bedrock] actor aim target=${JSON.stringify(actorPosition)} ` +
          `from=${JSON.stringify(this.position)} rotation=${JSON.stringify(actorRotation)}`,
      );
    }
    this.rotation = actorRotation;
    this.sendAuthInput([], vector2(0, 0), this.position);
    await wait(100);
    const actionType = data && data.action_type ? data.action_type : "interact";
    // MouseOverEntity is hover state, not the actor click; the transaction below is the complete click sequence.
    if (data && data.send_hover === true) {
      this.client.queue("interact", {
        action_id: "mouse_over_entity",
        target_entity_id: target,
        has_position: true,
        position: actorHit,
      });
    }
    await wait(50);
    const queueInteraction = async (heldItem, clickPosition = actorHit) => {
      console.log(
        `[bedrock] actor action=${actionType} held item=${JSON.stringify(heldItem)}`,
      );
      // Prefer the complete ItemV4 from InventoryContent so the source stack net ID matches server state.
      const transactionItem = { ...heldItem };
      this.client.queue("inventory_transaction", {
        transaction: {
          legacy_request_id: 0,
          legacy_set_item_slots: undefined,
          transaction_presence: true,
          transaction_type: "item_use_on_entity",
          // Keep the actions field empty; adding an inventory delta causes BalanceMismatch before actor dispatch.
          transaction_data: {
            actions_presence: true,
            actions: [],
            entity_runtime_id: target,
            action_type: actionType,
            hotbar_slot: 0,
            // Preserve the synchronized stack net ID required by ItemUseOnActor validation.
            held_item: descriptorItem(transactionItem, data && data.include_stack_id === true),
            // Use the actor position for both PlayerAuthInput and the transaction distance check.
            player_pos: vector3(this.position.x, this.position.y, this.position.z),
            click_pos: clickPosition,
          },
        },
      });
      // Do not advertise PerformItemInteraction without its packed PlayerAuthInput payload.
      await wait(50);
      this.sendAuthInput([], vector2(0, 0), this.position);
    };

    await queueInteraction(synchronizedItem, actorHit);
    this.rotation = savedRotation;
  }
,
};
