"use strict";

const TARGET_GAME_VERSION = "1.26.45";
const TARGET_PROTOCOL_VERSION = 2169;
const BASE_GAME_VERSION = "1.26.40";

const minecraftDataPath = require.resolve("minecraft-data");
const baseMinecraftData = require(minecraftDataPath);

// Protocol 2168 is shared by 1.26.40 through 1.26.44. The 1.26.44 release
// changed RemoveScore, while the r26_u4 protocol branch also converted a set
// of packet payloads (including InventoryTransaction) to Cereal. Keep these
// compatibility patches local and fail closed if the installed baseline moves
// instead of silently connecting with an older dialect.
if (
  !baseMinecraftData.versions.bedrock.some(
    (version) => version.minecraftVersion === TARGET_GAME_VERSION,
  )
) {
  baseMinecraftData.versions.bedrock.push({
    minecraftVersion: TARGET_GAME_VERSION,
    majorVersion: BASE_GAME_VERSION,
    version: TARGET_PROTOCOL_VERSION,
    releaseType: "release",
  });
}

function exactMinecraftData(version, preNetty) {
  if (version !== `bedrock_${TARGET_GAME_VERSION}`) {
    return baseMinecraftData(version, preNetty);
  }

  const base = baseMinecraftData(`bedrock_${BASE_GAME_VERSION}`, preNetty);
  const protocol = JSON.parse(JSON.stringify(base.protocol));
  const removeObjective =
    protocol.types?.packet_set_score?.[1]?.[0]?.type?.[1]?.type?.[1]?.[3]
      ?.type?.[1]?.fields?.remove?.[1]?.[0];

  if (
    !removeObjective ||
    JSON.stringify(removeObjective.type) !== JSON.stringify(["option", "string"])
  ) {
    throw new Error(
      "The installed minecraft-data schema is not the expected 1.26.40 baseline; refusing the 1.26.44 alias.",
    );
  }

  removeObjective.type = ["option", ["option", "string"]];

  // The local r26_u4 protocol branch adds ItemInteract=2 to the actor
  // transaction enum. The installed minecraft-data baseline predates that
  // enum entry, although the wire type itself is unchanged (zigzag32).
  const actorActionType =
    protocol.types?.Transaction?.[1]?.[3]?.type?.[1]?.fields
      ?.item_use_on_entity?.[1]?.[1]?.type?.[1];
  if (
    !actorActionType ||
    actorActionType.type !== "zigzag32" ||
    actorActionType.mappings?.["0"] !== "interact" ||
    actorActionType.mappings?.["1"] !== "attack" ||
    actorActionType.mappings?.["2"] !== undefined
  ) {
    throw new Error(
      "The installed minecraft-data actor transaction schema is not the expected 1.26.40 baseline.",
    );
  }
  actorActionType.mappings["2"] = "item_interact";

  protocol.types.DebugMarkerData = ["container", [
    { name: "text", type: "string" },
    { name: "position", type: "vec3f" },
    { name: "color", type: "li32" },
    { name: "duration", type: "lu64" },
  ]];
  protocol.types.DebugRendererData = ["container", [
    { name: "type", type: "string" },
    { name: "debug_marker_data", type: ["option", "DebugMarkerData"] },
  ]];
  protocol.types.packet_debug_renderer = protocol.types.DebugRendererData;
  const packetNames = protocol.types.mcpe_packet?.[1]?.[0]?.type?.[1]?.mappings;
  if (!packetNames || packetNames["164"] !== "primitive_shapes") {
    throw new Error(
      "The installed minecraft-data schema is not the expected 1.26.40 packet map.",
    );
  }
  packetNames["164"] = "debug_renderer";

  // Protocol 2168 uses the cereal NetworkItemStackDescriptor for every
  // network ItemStack field. The installed 1.26.40 protodef still calls that
  // field ItemV4, whose ItemExtraData tail is not the 1.26.44 descriptor. The
  // local r26_u4 schema uses a little-endian item ID, fixed count, unsigned
  // variable-length aux/block IDs, a signed ZigZag VarInt stack-net-id variant, and
  // a raw user-data byte buffer.
  protocol.types.NetworkItemStackDescriptor = ["container", [
      { name: "network_id", type: "li16" },
      { name: "count", type: "lu16" },
      { name: "metadata", type: "varint" },
    { name: "has_stack_id", type: "bool" },
    {
      name: "stack_id",
      type: [
        "switch",
        {
          compareTo: "has_stack_id",
          // Bedrock's varint32 is the signed ZigZag form.  The server's
          // BinaryStream::writeVarInt uses (value >> 31) ^ (value << 1), so
          // a server net ID of 1 is encoded as 02 rather than 01.
          fields: { true: "zigzag32" },
          default: "void",
        },
      ],
    },
    { name: "block_runtime_id", type: "varint" },
    // The field is documented as a string because Bedrock's BinaryStream
    // string is a length-prefixed byte buffer. `buffer` is intentional here:
    // protodef's pstring writer always converts through UTF-8 and would
    // corrupt arbitrary NBT/user-data bytes.
    {
      name: "user_data",
      type: ["buffer", { countType: "varint" }],
    },
  ]];

  // MineBlockActionData is the one ItemStackRequest action whose stack
  // network ID is a raw little-endian int32. Keep the guard here because a
  // future minecraft-data update must not silently switch this field back to
  // zigzag32; with a server stack ID of 2 that would put 4 on the wire.
  const requestActionFields = protocol.types?.ItemStackRequest?.[1]
    ?.find((field) => field && field.name === "actions")
    ?.type?.[1]?.type?.[1];
  const requestActionPayload = requestActionFields?.find(
    (field) => field && field.anon === true,
  );
  const mineBlockAction = requestActionPayload?.type?.[1]?.fields?.mine_block;
  if (!mineBlockAction || mineBlockAction[0] !== "container") {
    throw new Error(
      "The installed item stack request schema has no expected mine_block action.",
    );
  }
  const mineBlockNetworkId = mineBlockAction[1]?.find(
    (field) => field && field.name === "network_id",
  );
  if (!mineBlockNetworkId || mineBlockNetworkId.type !== "li32") {
    throw new Error(
      "The installed mine_block action schema does not use raw int32 network IDs.",
    );
  }

  const transaction = protocol.types?.Transaction?.[1];
  const transactionData = transaction?.find(
    (field) => field && field.name === "transaction_data",
  )?.type?.[1]?.fields;
  const actorTransaction = transactionData?.item_use_on_entity?.[1];
  const actorItem = actorTransaction?.find(
    (field) => field && field.name === "held_item",
  );
  const useItem = protocol.types?.TransactionUseItem?.[1]?.find(
    (field) => field && field.name === "held_item",
  );
  const releaseItem = transactionData?.item_release?.[1]?.find(
    (field) => field && field.name === "held_item",
  );
  if (
    !transaction || !transactionData || !actorItem || !useItem || !releaseItem ||
    actorItem.type !== "ItemV4" ||
    useItem.type !== "ItemV4" ||
    releaseItem.type !== "ItemV4"
  ) {
    throw new Error(
      "The installed transaction schema is not the expected 1.26.40 baseline.",
    );
  }
  actorItem.type = "NetworkItemStackDescriptor";
  useItem.type = "NetworkItemStackDescriptor";
  releaseItem.type = "NetworkItemStackDescriptor";

  // ItemV4 is a named type used transitively by the rest of the installed
  // protocol: inventory content/slot, equipment, dropped-item actors,
  // crafting events, and TransactionActions. Alias it once so those fields
  // all follow the 2168 descriptor instead of silently retaining the old
  // ItemExtraData encoding.
  protocol.types.ItemV4 = protocol.types.NetworkItemStackDescriptor;

  // InventoryTransactionPacket was converted to Cereal in protocol 2168.
  // The 1.26.40 protodef still describes the pre-Cereal packet as:
  //
  //   legacy + transaction-type-presence + transaction-type + actions
  //
  // whereas r26_u4 writes:
  //
  //   legacy request id + optional legacy slots + transaction presence +
  //   transaction variant
  //
  // and each transaction variant contains its own InventoryTransaction
  // action list.  The two layouts happen to decode cleanly as packet 30, so
  // using the old schema silently selects InventoryMismatch for item-use
  // packets (the old type-presence byte is read as the new variant selector).
  // Keep the old Transaction type for the packed PlayerAuthInput dialect and
  // install a separate exact packet type here.
  protocol.types.LegacySetSlot = ["container", [
    { name: "container_id", type: "u8" },
    {
      name: "slots",
      type: ["array", { countType: "varint", type: "u8" }],
    },
  ]];

  protocol.types.CerealInventorySource = ["container", [
    {
      name: "source_type",
      type: [
        "mapper",
        {
          type: "varint",
          mappings: {
            "0": "container",
            "1": "global",
            "2": "world_interaction",
            "3": "creative",
            "100": "craft_slot",
            "99999": "craft",
          },
        },
      ],
    },
    { name: "container_presence", type: "bool" },
    {
      // The C++ serializer uses DoubleOptionalFunc here: the first bool says
      // whether the optional field exists, and the inner option bool/value is
      // written only when that outer bool is true.  A plain protodef option
      // would always emit the inner presence byte and shift every following
      // action field for world interactions.
      name: "container_id",
      type: [
        "switch",
        {
          compareTo: "container_presence",
          fields: { true: ["option", "i8"], default: "void" },
        },
      ],
    },
    { name: "flag_presence", type: "bool" },
    {
      name: "flags",
      type: [
        "switch",
        {
          compareTo: "flag_presence",
          fields: { true: ["option", "varint"], default: "void" },
        },
      ],
    },
  ]];

  protocol.types.CerealInventoryAction = ["container", [
    { name: "source", type: "CerealInventorySource" },
    { name: "slot", type: "varint" },
    { name: "from_item", type: "NetworkItemStackDescriptor" },
    { name: "to_item", type: "NetworkItemStackDescriptor" },
  ]];

  protocol.types.CerealInventoryActionList = ["array", {
    countType: "varint",
    type: "CerealInventoryAction",
  }];

  const cerealInventoryTransactionFields = [
    { name: "actions_presence", type: "bool" },
    { name: "actions", type: "CerealInventoryActionList" },
  ];
  protocol.types.CerealInventoryTransaction = [
    "container",
    cerealInventoryTransactionFields,
  ];

  const cerealItemUseFields = protocol.types.TransactionUseItem[1];
  const cerealActionType = cerealItemUseFields.find(
    (field) => field && field.name === "action_type",
  )?.type?.[1];
  if (
    !cerealActionType ||
    cerealActionType.type !== "zigzag32" ||
    cerealActionType.mappings?.["0"] !== "click_block" ||
    cerealActionType.mappings?.["1"] !== "click_air"
  ) {
    throw new Error(
      "The installed item-use transaction schema is not the expected 1.26.40 baseline.",
    );
  }
  // r26_u4 renamed the ItemUseInventoryTransaction actions. Keep the
  // canonical names from the local protocol reference so Use is encoded as
  // value 1 (the old click_air alias also occupied value 1).
  cerealActionType.mappings = {
    "0": "place",
    "1": "use",
    "2": "destroy",
    "3": "use_as_attack",
  };
  protocol.types.CerealItemUseTransaction = [
    "container",
    [
      ...cerealInventoryTransactionFields,
      ...cerealItemUseFields,
    ],
  ];

  const cerealActorActionType = {
    type: "zigzag32",
    mappings: {
      "0": "interact",
      "1": "attack",
      "2": "item_interact",
    },
  };
  protocol.types.CerealItemUseOnActorTransaction = ["container", [
    ...cerealInventoryTransactionFields,
    { name: "entity_runtime_id", type: "varint64" },
    { name: "action_type", type: ["mapper", cerealActorActionType] },
    { name: "hotbar_slot", type: "zigzag32" },
    { name: "held_item", type: "NetworkItemStackDescriptor" },
    { name: "player_pos", type: "vec3f" },
    { name: "click_pos", type: "vec3f" },
  ]];

  const oldTransactionData = protocol.types.Transaction[1].find(
    (field) => field && field.name === "transaction_data",
  )?.type?.[1]?.fields;
  const oldReleaseFields = oldTransactionData?.item_release?.[1];
  if (!oldReleaseFields) {
    throw new Error(
      "The installed transaction schema has no expected item_release fields.",
    );
  }
  protocol.types.CerealItemReleaseTransaction = ["container", [
    ...cerealInventoryTransactionFields,
    ...oldReleaseFields,
  ]];

  protocol.types.CerealNormalTransaction = [
    "container",
    cerealInventoryTransactionFields,
  ];

  protocol.types.CerealInventoryTransactionPacket = ["container", [
    { name: "legacy_request_id", type: "zigzag32" },
    {
      name: "legacy_set_item_slots",
      type: [
        "option",
        ["array", { countType: "varint", type: "LegacySetSlot" }],
      ],
    },
    {
      name: "transaction_presence",
      type: "bool",
    },
    {
      name: "transaction_type",
      type: [
        "mapper",
        {
          type: "varint",
          mappings: {
            "0": "normal",
            "1": "inventory_mismatch",
            "2": "item_use",
            "3": "item_use_on_entity",
            "4": "item_release",
          },
        },
      ],
    },
    {
      name: "transaction_data",
      type: [
        "switch",
        {
          compareTo: "transaction_type",
          fields: {
            normal: "CerealNormalTransaction",
            inventory_mismatch: "CerealNormalTransaction",
            item_use: "CerealItemUseTransaction",
            item_use_on_entity: "CerealItemUseOnActorTransaction",
            item_release: "CerealItemReleaseTransaction",
          },
        },
      ],
    },
  ]];

  protocol.types.packet_inventory_transaction = ["container", [
    { name: "transaction", type: "CerealInventoryTransactionPacket" },
  ]];

  // r26_u4 uses the same packed ItemUse shape inside PlayerAuthInput as the
  // standalone packet, but with one additional fixed presence byte between
  // TransactionLegacy and ItemUseInventoryTransaction. The installed
  // minecraft-data baseline predates that packed wrapper and models the
  // nested action list as a second optional layer. That old shape shifts the
  // ItemUse fields after the action list and lets the packet serialize without
  // making it a valid server-authoritative interaction.
  const playerAuthInput = protocol.types?.packet_player_auth_input;
  const playerAuthTransaction = playerAuthInput?.[1]?.find(
    (field) => field && field.name === "transaction",
  );
  const oldPlayerAuthTransactionFields = playerAuthTransaction?.type?.[1]?.[1];
  if (
    !playerAuthTransaction ||
    !Array.isArray(oldPlayerAuthTransactionFields) ||
    oldPlayerAuthTransactionFields[0]?.name !== "legacy" ||
    oldPlayerAuthTransactionFields[1]?.name !== "actions_presence" ||
    oldPlayerAuthTransactionFields[3]?.name !== "data"
  ) {
    throw new Error(
      "The installed PlayerAuthInput transaction schema is not the expected 1.26.40 baseline.",
    );
  }
  protocol.types.CerealPackedItemUseLegacyInventoryTransaction = ["container", [
    { name: "legacy", type: "TransactionLegacy" },
    { name: "item_use_transaction_presence", type: "bool" },
    { name: "item_use_transaction", type: "CerealItemUseTransaction" },
  ]];
  playerAuthTransaction.type = [
    "option",
    "CerealPackedItemUseLegacyInventoryTransaction",
  ];

  // ItemStackRequestCereal::RequestData serializes each action as the
  // variant selector followed by the legacy action-type byte and then that
  // action's payload.  The local r26_u4 dump and gophertunnel both retain this
  // byte; it is not an obsolete padding field.
  const requestActions = protocol.types?.ItemStackRequest?.[1]?.find(
    (field) => field && field.name === "actions",
  );
  const actionFields = requestActions?.type?.[1]?.type?.[1];
  if (
    !requestActions ||
    !Array.isArray(actionFields) ||
    actionFields[0]?.name !== "type_id" ||
    actionFields[1]?.name !== "legacy_type_id" ||
    actionFields[2]?.anon !== true
  ) {
    throw new Error(
      "The installed minecraft-data item stack request schema is not the expected 1.26.40 baseline.",
    );
  }

  return Object.assign(Object.create(Object.getPrototypeOf(base)), base, {
    protocol,
  });
}

Object.assign(exactMinecraftData, baseMinecraftData);
require.cache[minecraftDataPath].exports = exactMinecraftData;

const bedrock = require("bedrock-protocol");

module.exports = {
  BASE_GAME_VERSION,
  TARGET_GAME_VERSION,
  TARGET_PROTOCOL_VERSION,
  bedrock,
};
