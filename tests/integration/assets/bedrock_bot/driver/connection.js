"use strict";

const { bedrock, TARGET_GAME_VERSION } = require("../exact_protocol");
const { RAKNET_BACKEND, wait } = require("./config");
const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("./protocol");

module.exports = {
  connect() {
    if (this.closed || this.connecting || this.connected) return;
    this.connecting = true;
    try {
      const client = bedrock.createClient({
        host: this.options.host,
        port: this.options.port,
        username: this.options.username,
        offline: true,
        version: TARGET_GAME_VERSION,
        connectTimeout: 5000,
        // The dependency fallback uses an older RakNet handshake and cannot reliably join 1.26.x BDS.
        raknetBackend: RAKNET_BACKEND,
        // The target protocol/version is still fixed explicitly in `version`.
        skipPing: true,
        useRaknetWorkers: false,
        conLog: (message) => console.log(`[bedrock] ${message}`),
      });
      this.client = client;
      client.on("start_game", (packet) => this.onStartGame(packet));
      client.on("move_player", (packet) => this.onMovePlayer(packet));
      client.on("move_entity", (packet) => this.onMoveEntity(packet));
      client.on("move_entity_delta", (packet) => this.onMoveEntityDelta(packet));
      client.on("player_list", (packet) => this.onPlayerList(packet));
      client.on("respawn", (packet) => this.onRespawn(packet));
      client.on("add_entity", (packet) => this.onAddEntity(packet));
      client.on("add_actor", (packet) => this.onAddEntity(packet));
      client.on("add_item_entity", (packet) => this.onAddEntity(packet));
      client.on("remove_entity", (packet) => this.onRemoveEntity(packet));
      client.on("inventory_content", (packet) => this.onInventoryContent(packet));
      client.on("inventory_slot", (packet) => this.onInventorySlot(packet));
      client.on("item_stack_response", (packet) => {
        console.log(`[bedrock] item stack response=${JSON.stringify(packet)}`);
      });
      client.on("crafting_data", (packet) => this.onCraftingData(packet));
      client.on("update_block", (packet) => {
        this.onUpdateBlock(packet);
        if (packet && packet.position && packet.block_runtime_id !== undefined) {
          console.log(
            `[bedrock] update block pos=${JSON.stringify(packet.position)} ` +
              `runtime=${packet.block_runtime_id} layer=${packet.layer}`,
          );
        }
      });
      client.on("container_open", (packet) => this.onContainerOpen(packet));
      client.on("death_screen", () => {
        console.log("[bedrock] death screen received");
      });
      client.on("death_info", (packet) => {
        console.log(`[bedrock] death info received: ${JSON.stringify(packet)}`);
        this.onDeathInfo();
      });
      client.on("set_health", (packet) => {
        if (packet && Number(packet.health) <= 0) {
          console.log("[bedrock] zero health received");
        }
      });
      client.on("spawn", () => this.onSpawn());
      client.on("join", () => console.log("[bedrock] encrypted join complete"));
      client.on("error", (error) => this.onError(error));
      client.on("disconnect", (packet) => {
        console.error(`[bedrock] server disconnect: ${JSON.stringify(packet)}`);
      });
      client.on("kick", (packet) => {
        console.error(`[bedrock] server kick: ${JSON.stringify(packet)}`);
      });
      client.on("close", () => this.onClose());
    } catch (error) {
      this.onError(error);
    }
  }
,

  onStartGame(packet) {
    this.startGameReceived = true;
    this.serverAuthoritativeBlockBreaking = Boolean(packet.server_authoritative_block_breaking);
    console.log(
      `[bedrock] server authoritative block breaking=${packet.server_authoritative_block_breaking} ` +
        `inventory=${packet.server_authoritative_inventory}`,
    );
    if (packet.runtime_entity_id !== undefined) {
      this.entityId = BigInt(packet.runtime_entity_id);
    }
    if (packet.player_position) {
      this.position = {
        x: packet.player_position.x,
        // Ignore the transient out-of-range start_game Y; echoing it in PlayerAuthInput closes the session.
        y: packet.player_position.y >= -64 && packet.player_position.y <= 320
          ? packet.player_position.y
          : 60,
        z: packet.player_position.z,
      };
    }
    if (packet.rotation) this.rotation = packet.rotation;
    console.log(`[bedrock] start_game runtime=${this.entityId} position=${JSON.stringify(this.position)}`);
    if (this.serverUuid && !this.earlySkinSent) this.sendEarlySkin();
  }
,

  onMovePlayer(packet) {
    if (this.entityId && BigInt(packet.runtime_id) === this.entityId && packet.position) {
      if (packet.mode === "teleport") this.pendingHandledTeleport = true;
      this.position = packet.position;
      this.rotation = vector2(packet.yaw, packet.pitch);
    }
  }
,

  onMoveEntity(packet) {
    const runtime = packet && packet.runtime_entity_id;
    if (runtime === undefined || !packet.position) return;
    this.actorPositions.set(String(runtime), vector3(
      packet.position.x,
      packet.position.y,
      packet.position.z,
    ));
  }
,

  onMoveEntityDelta(packet) {
    const runtime = packet && packet.runtime_entity_id;
    if (runtime === undefined) return;
    const key = String(runtime);
    const previous = this.actorPositions.get(key);
    if (!previous) return;
    this.actorPositions.set(key, vector3(
      packet.x === undefined ? previous.x : Number(packet.x),
      packet.y === undefined ? previous.y : Number(packet.y),
      packet.z === undefined ? previous.z : Number(packet.z),
    ));
  }
,

  onRespawn(packet) {
    const state = Number(packet.state);
    console.log(
      `[bedrock] respawn handshake state=${state} runtime=${packet.runtime_entity_id} ` +
        `position=${JSON.stringify(packet.position)}`,
    );
    if (packet.position) {
      this.position = {
        x: packet.position.x,
        y: packet.position.y,
        z: packet.position.z,
      };
    }
    if (state === 1 && this.respawnRequested && this.deathRespawnSent) {
      if (packet.position) this.position = packet.position;
      this.scheduleFinalRespawn(200);
      return;
    }
    if (state === 1) {
      if (packet.runtime_entity_id !== undefined && BigInt(packet.runtime_entity_id) !== 0n) {
        this.entityId = BigInt(packet.runtime_entity_id);
      }
      if (packet.position) this.position = packet.position;
      this.respawnRequested = false;
      this.deathInfoReceived = false;
      this.pendingRespawnPacket = null;
      this.respawnActionSent = false;
      this.respawnAckKey = null;
      this.respawnHandshakeAcked = false;
      if (this.respawnFallbackTimer) clearTimeout(this.respawnFallbackTimer);
      this.respawnFallbackTimer = null;
      this.deathRespawnSent = false;
      return;
    }
    if (state !== 0) return;

    // Do not acknowledge SEARCHING packets around the death screen; the client responds with Respawn(state=2)
    // after it settles.
    if (this.spawned) {
      this.pendingRespawnPacket = packet;
      return;
    }

    if (this.respawnHandshakeAcked) return;
    const key = this.respawnPacketKey(packet);
    if (this.respawnAckKey === key) return;

    // Keep SEARCHING_FOR_SPAWN pending until the explicit respawn request to avoid concurrent handshakes.
    if (this.spawned && !this.respawnActionSent) {
      this.pendingRespawnPacket = packet;
      return;
    }

    this.ackRespawn(packet);
  }
,

  onDeathInfo() {
    this.respawnRequested = true;
    this.deathInfoReceived = true;
    this.scheduleDeathRespawn(200);
  }
,

  sendDeathRespawn() {
    if (!this.client || !this.entityId || this.deathRespawnSent) return;
    this.deathRespawnSent = true;
    this.client.queue("respawn", {
      position: vector3(0, 0, 0),
      state: 2,
      runtime_entity_id: this.entityId,
    });
  }
,

  respawnPacketKey(packet) {
    const position = packet.position || {};
    return [
      String(packet.runtime_entity_id ?? ""),
      Number(position.x),
      Number(position.y),
      Number(position.z),
    ].join(":");
  }
,

  ackRespawn(packet) {
    const position = packet.position || this.position;
    this.respawnAckKey = this.respawnPacketKey(packet);
    this.respawnHandshakeAcked = true;
    console.log(
      `[bedrock] respawn ACK state=2 runtime=${packet.runtime_entity_id} ` +
        `position=${JSON.stringify(position)}`,
    );

    // A respawn handshake requires CLIENT_READY_TO_SPAWN; PlayerAction(respawn) alone is ignored.
    this.client.queue("respawn", {
      position,
      state: 2,
      runtime_entity_id: packet.runtime_entity_id ?? this.entityId,
    });
  }
,

  onPlayerList(packet) {
    const records = packet.records;
    const entries = Array.isArray(records)
      ? records
      : (records && Array.isArray(records.records) ? records.records : []);
    for (const entry of entries) {
      if (entry.username === this.options.username && entry.uuid) {
        this.serverUuid = entry.uuid;
        if (entry.skin_data) this.serverSkin = entry.skin_data;
        console.log(
          `[bedrock] player list uuid=${this.serverUuid} profile uuid=${this.client.profile.uuid} ` +
            `skin=${this.serverSkin ? "present" : "absent"}`,
        );
        if (this.startGameReceived && !this.earlySkinSent) this.sendEarlySkin();
      }
    }
  }
,

  onAddEntity(packet) {
    // Actor packets use inconsistent runtime ID field names; accept both `runtime_id` and `runtime_entity_id`.
    const runtimeId =
      packet.runtime_id ?? packet.runtime_entity_id ?? packet.entity_runtime_id;
    if (runtimeId === undefined) return;
    const type = packet.entity_type || packet.identifier || packet.entity_type_name;
    if (!type && packet.position) {
      console.log(
        `[bedrock] item/entity runtime=${runtimeId} ` +
          `position=${JSON.stringify(packet.position)} ` +
          `item=${JSON.stringify(packet.item)}`,
      );
    }
    if (this.actorPacketLogCount < 12) {
      this.actorPacketLogCount += 1;
      console.log(
        `[bedrock] actor packet id=${runtimeId} type=${JSON.stringify(type)} ` +
          `keys=${Object.keys(packet).join(",")}`,
      );
    }
    if (type) {
      const name = String(type);
      const runtime = BigInt(runtimeId);
      this.actors.set(name, runtime);
      if (!name.includes(":")) this.actors.set(`minecraft:${name}`, BigInt(runtimeId));
      if (packet.position) {
        this.actorPositions.set(String(runtime), vector3(
          packet.position.x,
          packet.position.y,
          packet.position.z,
        ));
      }
      if (packet.unique_id !== undefined) {
        this.actorEntities.set(String(packet.unique_id), { name, runtime });
      }
      if (
        name === "minecraft:chicken" ||
        name === "minecraft:armor_stand" ||
        name === "minecraft:boat" ||
        name === "minecraft:villager_v2"
      ) {
        console.log(
          `[bedrock] ${name} actor id=${runtimeId} unique=${packet.unique_id} ` +
            `position=${JSON.stringify(packet.position)}`,
        );
      }
    }
  }
,

  onRemoveEntity(packet) {
    const key = String(packet && packet.entity_id_self);
    const entity = this.actorEntities.get(key);
    if (!entity) return;
    this.actorEntities.delete(key);
    for (const [name, runtime] of this.actors) {
      if (runtime === entity.runtime) this.actors.delete(name);
    }
    this.actorPositions.delete(String(entity.runtime));
  }
,

  blockKey(position) {
    return `${Number(position.x)},${Number(position.y)},${Number(position.z)}`;
  }
,

  onUpdateBlock(packet) {
    if (!packet || !packet.position || packet.block_runtime_id === undefined) return;
    const rawRuntimeId = Number(packet.block_runtime_id);
    // Restore block runtime IDs above INT32_MAX after protodef reads them as signed values.
    const runtimeId = Number.isInteger(rawRuntimeId) ? rawRuntimeId >>> 0 : rawRuntimeId;
    this.blockRuntimeIds.set(
      this.blockKey(packet.position),
      runtimeId,
    );
  }
,

  blockRuntimeIdAt(position) {
    return this.blockRuntimeIds.get(this.blockKey(position)) ?? 0;
  }
,

  latestActorRuntime(requestedType) {
    const wanted = String(requestedType);
    const canonical = wanted.includes(":") ? wanted : `minecraft:${wanted}`;
    let nearest = null;
    let nearestDistance = Number.POSITIVE_INFINITY;
    let fallback = null;
    for (const entity of this.actorEntities.values()) {
      if (entity.name !== wanted && entity.name !== canonical) continue;
      if (fallback === null || entity.runtime > fallback) fallback = entity.runtime;
      const position = this.actorPositions.get(String(entity.runtime));
      if (!position) continue;
      const distance = Math.hypot(
        position.x - this.position.x,
        position.y - this.position.y,
        position.z - this.position.z,
      );
      if (distance < nearestDistance) {
        nearest = entity.runtime;
        nearestDistance = distance;
      }
    }
    return nearest ?? fallback ?? this.actors.get(wanted) ?? this.actors.get(canonical);
  }
,

  onInventoryContent(packet) {
    if (!packet || !this.isInventoryWindow(packet.window_id)) return;
    const items = Array.isArray(packet.input)
      ? packet.input
      : (Array.isArray(packet.items) ? packet.items : packet.content);
    if (!Array.isArray(items)) return;
    console.log(
      `[bedrock] inventory content window=${packet.window_id} slot0=${JSON.stringify(items[0])}`,
    );
    this.inventoryItems.clear();
    items.forEach((item, slot) => this.inventoryItems.set(slot, item));
  }
,

  onInventorySlot(packet) {
    if (!packet || !this.isInventoryWindow(packet.window_id)) return;
    const slot = Number(packet.slot);
    if (!Number.isInteger(slot) || slot < 0) return;
    if (packet.item !== undefined) {
      if (slot === 0 || slot === 1) {
        console.log(
          `[bedrock] inventory slot window=${packet.window_id} slot=${slot} ` +
            `item=${JSON.stringify(packet.item)}`,
        );
      }
      this.inventoryItems.set(slot, packet.item);
    }
  }
,

  onCraftingData(packet) {
    if (!packet) return;
    const recipeFields = [
      "shaped_recipes",
      "shapeless_recipes",
      "multi_recipes",
      "shulker_box_recipes",
      "shapeless_chemistry_recipes",
      "shaped_chemistry_recipes",
      "smithing_transform_recipes",
      "smithing_trim_recipes",
    ];
    this.craftingRecipes = recipeFields.flatMap((field) =>
      Array.isArray(packet[field]) ? packet[field] : [],
    );
    console.log(
      `[bedrock] crafting data recipes=${this.craftingRecipes.length} ` +
        `shaped=${Array.isArray(packet.shaped_recipes) ? packet.shaped_recipes.length : 0} ` +
        `shapeless=${Array.isArray(packet.shapeless_recipes) ? packet.shapeless_recipes.length : 0}`,
    );
  }
,

  isInventoryWindow(windowId) {
    return windowId === undefined || windowId === null || windowId === 0 || windowId === "inventory";
  }
,

  onContainerOpen(packet) {
    if (!packet) return;
    this.containerWindowId = packet.window_id;
    this.containerType = packet.window_type;
    console.log(
      `[bedrock] container opened window=${this.containerWindowId} ` +
        `type=${this.containerType} position=${JSON.stringify(packet.coordinates)}`,
    );
  }
,

  onSpawn() {
    this.connected = true;
    this.connecting = false;
    this.spawned = true;
    console.log("[bedrock] spawned");
    if (this.heartbeatTimer) clearInterval(this.heartbeatTimer);
    this.heartbeatTimer = setInterval(() => this.sendAuthInput(), 50);
  }
,

  onError(error) {
    console.error(`[bedrock] ${error && error.stack ? error.stack : error}`);
    if (this.spawned) this.fail("protocol error after spawn");
    this.connected = false;
    this.connecting = false;
    this.scheduleRetry();
  }
,

  onClose() {
    console.error("[bedrock] connection closed");
    this.connected = false;
    this.connecting = false;
    if (!this.closed) this.scheduleRetry();
  }
,

  scheduleRetry() {
    if (this.closed || this.failed || this.retryTimer || Date.now() >= this.deadline) return;
    this.retryTimer = setTimeout(() => {
      this.retryTimer = null;
      this.connect();
    }, 500);
  }
,
};
