"use strict";

const { wait } = require("../config");

const { randomUUID } = require("crypto");
const { bedrock, TARGET_GAME_VERSION } = require("../../exact_protocol");
const gameData = require("minecraft-data")(`bedrock_${TARGET_GAME_VERSION}`);
const defaultSkin = gameData.defaultSkin;
const {
  vector2, vector3, blockPosition, emptyItem, worldInteractionAction,
  containerInventoryAction, encodeItemUserData, fallbackStickItem,
  fallbackDiamondItem, fallbackItem, isNonEmptyItem, descriptorItem, cloneItem,
} = require("../protocol");

module.exports = {
  sendCommand(command) {
    this.client.queue("command_request", {
      command: command.startsWith("/") ? command : `/${command}`,
      origin: {
        type: "player",
        uuid: randomUUID(),
        request_id: randomUUID(),
        player_entity_id: this.entityId,
      },
      internal: false,
      version: "latest",
    });
  }
,

  async sendServerListPing(data) {
    const ping = bedrock.ping({
      host: this.options.host,
      port: this.options.port,
    });
    if (data && data.expect_cancelled) {
      const outcome = await Promise.race([
        ping.then(
          (advertisement) => ({ advertisement }),
          (error) => ({ error }),
        ),
        wait(1500).then(() => ({ timedOut: true })),
      ]);
      if (outcome.advertisement) {
        throw new Error("cancelled server-list ping unexpectedly received an advertisement");
      }
      if (outcome.error) {
        console.log(`[bedrock] cancelled server list ping rejected: ${outcome.error}`);
      } else {
        console.log("[bedrock] cancelled server list ping produced no advertisement");
      }
      return;
    }
    const advertisement = await ping;
    console.log(`[bedrock] server list ping=${JSON.stringify(advertisement)}`);
  }
,

  sendChat(message) {
    this.client.queue("text", {
      needs_translation: false,
      category: "authored",
      type: "chat",
      source_name: this.options.username,
      message,
      xuid: "",
      platform_chat_id: "",
      has_filtered_message: false,
      filtered_message: undefined,
    });
  }
,

  sendArmSwing() {
    this.client.queue("animate", {
      action_id: "swing_arm",
      runtime_entity_id: this.entityId,
      data: 0,
      has_swing_source: false,
      swing_source: undefined,
    });
  }
,

  sendAuthInput(
    flags = [],
    move = vector2(0, 0),
    position = this.position,
    blockAction,
    itemStackRequest,
    transaction,
  ) {
    this.authInputCount += 1;
    if (this.authInputCount <= 3) {
      console.log(`[bedrock] auth input ${this.authInputCount}: ${JSON.stringify(position)}`);
    }
    const blockActions = blockAction === undefined
      ? undefined
      : (Array.isArray(blockAction) ? blockAction : [blockAction]);
    const inputFlags = new Set(["received_server_data", ...flags]);
    if (this.pendingHandledTeleport) {
      inputFlags.add("handled_teleport");
      this.pendingHandledTeleport = false;
    }
    if (itemStackRequest !== undefined) inputFlags.add("item_stack_request");
    if (blockActions !== undefined) inputFlags.add("block_action");
    this.client.queue("player_auth_input", {
      pitch: this.rotation.y || 0,
      yaw: this.rotation.x || 0,
      position,
      move_vector: move,
      head_yaw: this.rotation.x || 0,
      input_data: [...inputFlags],
      input_mode: "mouse",
      play_mode: "normal",
      interaction_model: "crosshair",
      // Use the camera rotation for the interaction aim; PlayerAuthInput stores this Vec2 as pitch, yaw.
      interact_rotation: vector2(this.rotation.y || 0, this.rotation.x || 0),
      tick: this.tick++,
      delta: vector3(0, 0, 0),
      // These protocol-2168 fields use DoubleOptional; writing the wrong presence byte shifts the remaining packet.
      transaction_presence: true,
      transaction,
      item_stack_request_presence: true,
      item_stack_request: itemStackRequest,
      block_action_presence: true,
      block_action: blockActions,
      vehicle_rotation_presence: true,
      vehicle_rotation: undefined,
      predicted_vehicle_presence: true,
      predicted_vehicle: undefined,
      analogue_move_vector: vector2(0, 0),
      camera_orientation: this.cameraOrientation(),
      raw_move_vector: move,
    });
  }
,

  cameraOrientation() {
    // Bedrock yaw 0 points along +Z; normalize the camera vector.
    const yaw = (Number(this.rotation.x) || 0) * Math.PI / 180;
    const pitch = (Number(this.rotation.y) || 0) * Math.PI / 180;
    const horizontal = Math.cos(pitch);
    return vector3(
      -Math.sin(yaw) * horizontal,
      -Math.sin(pitch),
      Math.cos(yaw) * horizontal,
    );
  }
,

  sendMove() {
    this.position = vector3(this.position.x + 1, this.position.y, this.position.z);
    this.sendAuthInput([], vector2(1, 0), this.position);
  }
,

  async sendInput() {
    this.sendAuthInput(["right"], vector2(0, 1), this.position);
    await wait(100);
    this.sendAuthInput(["left"], vector2(0, -1), this.position);
    await wait(100);
  }
,

  async toggleInput(state, start, stop) {
    const startFlags = state ? [start, state] : [start];
    this.sendAuthInput(startFlags, vector2(0, 0), this.position);
    await wait(500);
    this.sendAuthInput([stop], vector2(0, 0), this.position);
  }
,

  sendToggleInput(action) {
    this.sendAuthInput([action], vector2(0, 0), this.position);
  }
,

  sendFlightInput(action) {
    this.sendAuthInput([action], vector2(0, 0), this.position);
  }
,

  async sendJump() {
    await wait(500);
    const basePosition = vector3(this.position.x, this.position.y, this.position.z);
    const hadHeartbeat = this.heartbeatTimer !== null;
    if (hadHeartbeat) {
      clearInterval(this.heartbeatTimer);
      this.heartbeatTimer = null;
    }
    try {
      // Confirm the player is standing on the fixture before sending the rising frame.
      this.position = basePosition;
      this.sendAuthInput(["vertical_collision"], vector2(0, 0), basePosition);
      await wait(100);

      const jumpPosition = vector3(
        basePosition.x,
        basePosition.y + 0.1,
        basePosition.z,
      );
      this.position = jumpPosition;
      this.sendAuthInput(
        ["vertical_collision", "start_jumping", "jump_down", "jumping", "jump_pressed_raw", "jump_current_raw"],
        vector2(0, 0),
        jumpPosition,
      );
      await wait(100);
      this.sendAuthInput(["jump_released_raw"], vector2(0, 0), jumpPosition);
      await wait(100);
    } finally {
      if (hadHeartbeat && !this.closed && this.spawned) {
        this.heartbeatTimer = setInterval(() => this.sendAuthInput(), 50);
      }
    }
  }
,

  sendEmote() {
    this.client.queue("emote", {
      entity_id: this.entityId,
      emote_id: "anim.wave",
      emote_length_ticks: 1,
      xuid: "",
      platform_id: "",
      flags: 0,
    });
  }
,

  sendSkin() {
    const image = (data, width, height) => ({
      width,
      height,
      data: Buffer.from(data, "base64"),
    });
    const color = (value) => parseInt(value.slice(1), 16) | 0;
    const imageData = image(
      defaultSkin.SkinData,
      defaultSkin.SkinImageWidth,
      defaultSkin.SkinImageHeight,
    );
    const animations = defaultSkin.AnimatedImageData.map((animation) => ({
      skin_image: image(animation.Image, animation.ImageWidth, animation.ImageHeight),
      animation_type: animation.Type,
      animation_frames: animation.Frames,
      expression_type: animation.AnimationExpression,
    }));
    const personalPieces = defaultSkin.PersonaPieces.map((piece) => ({
      piece_id: piece.PieceId,
      piece_type: piece.PieceType.replace(/^persona_/, ""),
      pack_id: piece.PackId,
      is_default_piece: piece.IsDefault,
      product_id: piece.ProductId,
    }));
    const pieceTintColors = defaultSkin.PieceTintColors.map((tint) => ({
      piece_type: tint.PieceType.replace(/^persona_/, ""),
      colors: tint.Colors.map(color),
    }));
    const generatedSkin = {
      skin_id: defaultSkin.SkinId,
      play_fab_id: "",
      skin_resource_pack: defaultSkin.SkinResourcePatch,
      skin_data: imageData,
      animations,
      cape_data: image(
        defaultSkin.CapeData,
        defaultSkin.CapeImageWidth,
        defaultSkin.CapeImageHeight,
      ),
      geometry_data: defaultSkin.SkinGeometryData,
      geometry_data_version: defaultSkin.SkinGeometryDataEngineVersion,
      animation_data: defaultSkin.SkinAnimationData,
      cape_id: defaultSkin.CapeId,
      full_skin_id: defaultSkin.SkinId,
      arm_size: defaultSkin.ArmSize,
      skin_color: color(defaultSkin.SkinColor),
      personal_pieces: personalPieces,
      piece_tint_colors: pieceTintColors,
      premium: defaultSkin.PremiumSkin,
      persona: defaultSkin.PersonaSkin,
      cape_on_classic: defaultSkin.CapeOnClassicSkin,
      primary_user: true,
      overriding_player_appearance: false,
      trusted: "true",
      profile_hash: "",
    };
    const skin = this.serverSkin
      ? JSON.parse(JSON.stringify(this.serverSkin))
      : generatedSkin;
    const packetUuid = this.serverUuid || this.client.profile.uuid;
    console.log(
      `[bedrock] skin packet uuid=${packetUuid} source=${this.serverSkin ? "player-list" : "generated"}`,
    );
    this.client.queue("player_skin", {
      uuid: packetUuid,
      skin,
      skin_name: "endstone-automated-skin",
      old_skin_name: "",
    });
  }
,

  sendEarlySkin() {
    this.earlySkinSent = true;
    this.sendSkin();
  }
,
};
