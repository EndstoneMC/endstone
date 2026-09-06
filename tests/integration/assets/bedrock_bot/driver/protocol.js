"use strict";

function vector2(x = 0, y = 0) {
  // Bedrock's protocol Vec2 datatype names its second component `z`, while the driver's internal rotation/move
  // model uses the conventional `y`. Keep both aliases so the same helper is safe for internal calculations
  // and for protodef serialization; omitting `z` writes NaN on the wire.
  return { x, y, z: y };
}

function vector3(x = 0, y = 0, z = 0) {
  return { x, y, z };
}

function blockPosition(position) {
  return {
    x: Math.floor(position.x),
    y: Math.floor(position.y),
    z: Math.floor(position.z),
  };
}

function emptyItem() {
  return {
    network_id: 0,
    count: 0,
    metadata: 0,
    has_stack_id: false,
    stack_id: undefined,
    block_runtime_id: 0,
    user_data: Buffer.alloc(0),
    extra: {
      has_nbt: "false",
      can_place_on: [],
      can_destroy: [],
    },
  };
}

function worldInteractionAction(toItem = emptyItem(), slot = 0) {
  return {
    source: {
      source_type: "world_interaction",
      // InventoryAction encodes WindowID as a double optional.  World interactions do not carry a window/container
      // ID at all; setting the outer presence bit here makes BDS read the following flag bytes as an i8
      // window ID and reject the transaction before the world hook runs.
      container_presence: false,
      container_id: undefined,
      flag_presence: true,
      flags: undefined,
    },
    slot,
    from_item: emptyItem(),
    to_item: descriptorItem(toItem, Boolean(toItem && toItem.has_stack_id)),
  };
}

function containerInventoryAction(slot, fromItem, toItem = emptyItem()) {
  return {
    source: {
      source_type: "container",
      // Container actions carry a present WindowID optional containing the inventory window (0).  SourceFlags is
      // absent for container sources.
      container_presence: true,
      container_id: 0,
      flag_presence: false,
      flags: undefined,
    },
    slot,
    from_item: descriptorItem(fromItem),
    to_item: descriptorItem(toItem),
  };
}

function encodeItemUserData(item) {
  const userData = item && item.user_data;
  if (Buffer.isBuffer(userData)) return Buffer.from(userData);
  if (userData instanceof Uint8Array) return Buffer.from(userData);
  if (userData && userData.type === "Buffer" && Array.isArray(userData.data)) {
    return Buffer.from(userData.data);
  }
  if (typeof userData === "string" && userData.length > 0) {
    return Buffer.from(userData, "binary");
  }
  if (!isNonEmptyItem(item)) return Buffer.alloc(0);
  const extra = item.extra || {};
  // ItemExtraDataWithoutBlockingTick, matching the local gophertunnel writer: int16 NBT length followed by the
  // can-place-on and can-destroy string-list lengths. The list count is a fixed little-endian uint32 and each
  // string uses a fixed little-endian int16 byte length; these are not Bedrock varints (the outer descriptor
  // still wraps this buffer in a varint-length binary string).
  const nbtLength = extra.has_nbt === true ? -1 : 0;
  const bytes = [Buffer.alloc(2)];
  bytes[0].writeInt16LE(nbtLength, 0);
  const canPlaceOn = Array.isArray(extra.can_place_on) ? extra.can_place_on : [];
  const canDestroy = Array.isArray(extra.can_destroy) ? extra.can_destroy : [];
  const writeListLength = (length) => {
    const result = Buffer.alloc(4);
    result.writeUInt32LE(Number(length) >>> 0, 0);
    return result;
  };
  const writeStringUTF = (value) => {
    const encoded = Buffer.from(String(value), "utf8");
    const length = Buffer.alloc(2);
    length.writeInt16LE(encoded.length, 0);
    return Buffer.concat([length, encoded]);
  };
  bytes.push(writeListLength(canPlaceOn.length));
  for (const value of canPlaceOn) {
    bytes.push(writeStringUTF(value));
  }
  bytes.push(writeListLength(canDestroy.length));
  for (const value of canDestroy) {
    bytes.push(writeStringUTF(value));
  }
  return Buffer.concat(bytes);
}

// The actor scenario places a stick in slot 0 before the driver action.  Keep the observed ItemV4 as a last-
// resort prediction for the short interval before BDS sends InventoryContent/InventorySlot back to the
// client. Once a server item packet arrives, the synchronized value always wins.

function fallbackStickItem(count = 1) {
  return {
    // BDS 1.26.44 reports stick as network id 323 in its item table. This fallback is only used when a direct
    // server-side inventory mutation has not produced an InventoryContent packet yet.
    network_id: 323,
    count,
    metadata: 0,
    // This is only a last-resort descriptor.  The actor path should normally wait for the server's
    // InventoryContent so it can carry the authoritative stack net ID.
    has_stack_id: false,
    stack_id: undefined,
    block_runtime_id: 0,
    user_data: Buffer.alloc(10),
    extra: {
      has_nbt: "false",
      can_place_on: [],
      can_destroy: [],
    },
  };
}

function fallbackDiamondItem(count = 1) {
  return {
    network_id: 306,
    count,
    metadata: 0,
    has_stack_id: false,
    stack_id: undefined,
    block_runtime_id: 0,
    user_data: Buffer.alloc(10),
    extra: {
      has_nbt: "false",
      can_place_on: [],
      can_destroy: [],
    },
  };
}

function fallbackItem(networkId, count = 1) {
  return {
    network_id: Number(networkId),
    count,
    metadata: 0,
    has_stack_id: false,
    stack_id: undefined,
    block_runtime_id: 0,
    user_data: Buffer.alloc(10),
    extra: {
      has_nbt: "false",
      can_place_on: [],
      can_destroy: [],
    },
  };
}

function isNonEmptyItem(item) {
  return item && Number(item.network_id) > 0 && Number(item.count) > 0;
}

function descriptorItem(item, includeStackId = true) {
  return {
    ...item,
    has_stack_id: includeStackId && Boolean(item.has_stack_id),
    stack_id: includeStackId ? item.stack_id : undefined,
    user_data: encodeItemUserData(item),
  };
}

function cloneItem(item) {
  if (!item || typeof item !== "object") return item;
  const clone = { ...item };
  if (Buffer.isBuffer(item.user_data)) {
    clone.user_data = Buffer.from(item.user_data);
  } else if (item.user_data instanceof Uint8Array) {
    clone.user_data = Buffer.from(item.user_data);
  } else if (typeof item.user_data === "string") {
    clone.user_data = Buffer.from(item.user_data, "binary");
  }
  return clone;
}

module.exports = {
  vector2,
  vector3,
  blockPosition,
  emptyItem,
  worldInteractionAction,
  containerInventoryAction,
  encodeItemUserData,
  fallbackStickItem,
  fallbackDiamondItem,
  fallbackItem,
  isNonEmptyItem,
  descriptorItem,
  cloneItem
};
