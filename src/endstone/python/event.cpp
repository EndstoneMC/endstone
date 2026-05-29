// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "event.h"

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_event(py::module_ &m, py::class_<Event, PyEvent> &event)
{
    py::native_enum<EventResult>(m, "EventResult", "enum.Enum")
        .value("DENY", EventResult::Deny)
        .value("DEFAULT", EventResult::Default)
        .value("ALLOW", EventResult::Allow)
        .finalize();

    event.def(py::init<bool>(), py::arg("is_async") = false)
        .def_property_readonly("event_name", &Event::getEventName, "A user-friendly identifier for this event.")
        .def_property_readonly("is_asynchronous", &Event::isAsynchronous,
                               "False by default, True if the event fires asynchronously.");

    py::class_<ICancellable>(m, "Cancellable",
                             "A type characterizing events that may be cancelled by a plugin or the server.")
        .def_property("is_cancelled", &ICancellable::isCancelled, &ICancellable::setCancelled, R"doc(
    The cancellation state of this event.

    A cancelled event will not be executed in the server, but will still pass to other plugins.
)doc")
        .def("cancel", &ICancellable::cancel, R"doc(
    Cancel this event.

    A cancelled event will not be executed in the server, but will still pass to other plugins.
)doc");

    // Actor events
    py::class_<ActorEvent<Actor>, Event>(m, "ActorEvent", "Represents an Actor-related event.")
        .def_property_readonly("actor", &ActorEvent<Actor>::getActor, py::return_value_policy::reference,
                               "The Actor which is involved in this event.");
    py::class_<ActorEvent<Mob>, Event>(m, "MobEvent", "Represents an Mob-related event.")
        .def_property_readonly("actor", &ActorEvent<Mob>::getActor, py::return_value_policy::reference,
                               "The Mob which is involved in this event.");
    py::class_<ActorDamageEvent, ActorEvent<Mob>, ICancellable>(m, "ActorDamageEvent",
                                                                "Called when an Actor is damaged.")
        .def_property("damage", &ActorDamageEvent::getDamage, &ActorDamageEvent::setDamage,
                      "The raw amount of damage caused by the event.")
        .def_property_readonly("damage_source", &ActorDamageEvent::getDamageSource, py::return_value_policy::reference,
                               "A DamageSource detailing the source of the damage.");
    py::class_<ActorDeathEvent, ActorEvent<Mob>>(m, "ActorDeathEvent", "Called when an Actor dies.")
        .def_property_readonly("damage_source", &ActorDeathEvent::getDamageSource, py::return_value_policy::reference,
                               "A DamageSource detailing the source of the damage for the death.");
    py::class_<PlayerDeathEvent, ActorDeathEvent>(m, "PlayerDeathEvent", "Called when a Player dies.")
        .def_property_readonly("player", &PlayerDeathEvent::getPlayer, py::return_value_policy::reference,
                               "The Player which is involved in this event.")
        .def_property("death_message", &PlayerDeathEvent::getDeathMessage, &PlayerDeathEvent::setDeathMessage,
                      "The death message that will appear to everyone on the server.");
    py::class_<ActorExplodeEvent, ActorEvent<Actor>, ICancellable>(m, "ActorExplodeEvent",
                                                                   "Called when an actor explodes.")
        .def_property_readonly("location", &ActorExplodeEvent::getLocation, R"doc(
    The location where the explosion happened.

    It is not possible to get this value from the Entity as the Entity no longer exists in the
    world.
)doc")
        .def_property(
            "block_list",
            [](const ActorExplodeEvent &self) {
                std::vector<Block *> blocks;
                for (const auto &block : self.getBlockList()) {
                    if (block) {
                        blocks.emplace_back(block.get());
                    }
                }
                return blocks;
            },
            [](ActorExplodeEvent &self, const std::vector<Block *> &blocks) {
                self.getBlockList().clear();
                for (const auto &block : blocks) {
                    if (block) {
                        self.getBlockList().emplace_back(block->clone());
                    }
                }
            },
            py::return_value_policy::reference_internal,
            "The list of blocks that would have been removed or were removed from the explosion event.");
    py::class_<ActorKnockbackEvent, ActorEvent<Mob>, ICancellable>(m, "ActorKnockbackEvent",
                                                                   "Called when a living entity receives knockback.")
        .def_property_readonly("source", &ActorKnockbackEvent::getSource, py::return_value_policy::reference,
                               "The source actor that has caused knockback to the defender, or None if the "
                               "knockback is not caused by an actor.")
        .def_property("knockback", &ActorKnockbackEvent::getKnockback, &ActorKnockbackEvent::setKnockback, R"doc(
    The knockback that will be applied to the entity.

    Note: the getter returns a copy; changes must be applied via the setter.
)doc");
    py::class_<ActorRemoveEvent, ActorEvent<Actor>>(m, "ActorRemoveEvent", R"doc(
    Called when an Actor is removed.

    This event should only be used for monitoring. Modifying the actor during or after this event
    leads to undefined behaviours. This event will not be called for Players.
)doc");
    py::class_<ActorSpawnEvent, ActorEvent<Actor>, ICancellable>(m, "ActorSpawnEvent", R"doc(
    Called when an Actor is spawned into a world.

    If an Actor Spawn event is cancelled, the actor will not spawn.
)doc");
    py::class_<ActorTeleportEvent, ActorEvent<Actor>, ICancellable>(m, "ActorTeleportEvent", R"doc(
    Called when a non-player entity is teleported from one location to another.

    This may be as a result of natural causes (Enderman, Shulker), pathfinding (Wolf), or commands
    (/teleport).
)doc")
        .def_property("from_location", &ActorTeleportEvent::getFrom, &ActorTeleportEvent::setFrom,
                      "The location that this actor moved from.")
        .def_property("to_location", &ActorTeleportEvent::getTo, &ActorTeleportEvent::setTo,
                      "The location that this actor moved to.");

    // Block events
    py::class_<BlockEvent, Event>(m, "BlockEvent", "Represents an Block-related event.")
        .def_property_readonly("block", &BlockEvent::getBlock, py::return_value_policy::reference,
                               "The Block which is involved in this event.");
    py::class_<BlockBreakEvent, BlockEvent, ICancellable>(m, "BlockBreakEvent", R"doc(
    Called when a block is broken by a player.

    If a BlockBreakEvent is cancelled, the block will not break and experience will not drop.
)doc")
        .def_property_readonly("player", &BlockBreakEvent::getPlayer, py::return_value_policy::reference,
                               "The Player that is breaking the block involved in this event.");
    py::class_<BlockExplodeEvent, BlockEvent, ICancellable>(m, "BlockExplodeEvent", R"doc(
    Called when a block explodes (e.g. bed in the Nether, respawn anchor in the Overworld).

    If a BlockExplodeEvent is cancelled, the explosion will not occur.
)doc")
        .def_property(
            "block_list",
            [](const BlockExplodeEvent &self) {
                std::vector<Block *> blocks;
                for (const auto &block : self.getBlockList()) {
                    if (block) {
                        blocks.emplace_back(block.get());
                    }
                }
                return blocks;
            },
            [](BlockExplodeEvent &self, const std::vector<Block *> &blocks) {
                self.getBlockList().clear();
                for (const auto &block : blocks) {
                    if (block) {
                        self.getBlockList().emplace_back(block->clone());
                    }
                }
            },
            py::return_value_policy::reference_internal,
            "The list of blocks that would have been removed or were removed from the explosion event.");
    py::class_<BlockCookEvent, BlockEvent, ICancellable>(m, "BlockCookEvent",
                                                         "Called when an ItemStack is successfully cooked in a block.")
        .def_property_readonly("source", &BlockCookEvent::getSource, py::return_value_policy::reference,
                               "The smelted (source) ItemStack for this event.")
        .def_property("result", &BlockCookEvent::getResult, &BlockCookEvent::setResult,
                      "The resultant ItemStack for this event.");
    py::class_<BlockGrowEvent, BlockEvent, ICancellable>(m, "BlockGrowEvent", R"doc(
    Called when a block grows naturally in the world.

    If a Block Grow event is cancelled, the block will not grow.
)doc")
        .def_property_readonly("new_state", &BlockGrowEvent::getNewState, py::return_value_policy::reference,
                               "The new state of the block after it has grown.");
    py::class_<BlockFormEvent, BlockGrowEvent>(m, "BlockFormEvent", R"doc(
    Called when a block is formed or spreads based on world conditions.

    Use BlockSpreadEvent to catch blocks that actually spread and don't just "randomly" form.

    Examples:
        - Snow forming due to a snow storm.
        - Ice forming in a snowy Biome like Taiga or Tundra.
        - Obsidian / Cobblestone forming due to contact with water.
        - Concrete forming due to mixing of concrete powder and water.

    If a Block Form event is cancelled, the block will not be formed.
)doc");
    py::class_<BlockFromToEvent, BlockEvent, ICancellable>(m, "BlockFromToEvent", R"doc(
    Represents events with a source block and a destination block, currently only applies to liquid
    (lava and water) and teleporting dragon eggs.

    If a Block From To event is cancelled, the block will not move (the liquid will not flow).
)doc")
        .def_property_readonly("to_block", &BlockFromToEvent::getToBlock, py::return_value_policy::reference,
                               "The faced Block.");
    py::class_<BlockPistonEvent, BlockEvent, ICancellable>(m, "BlockPistonEvent",
                                                           "Called when a piston block is triggered.")
        .def_property_readonly("direction", &BlockPistonEvent::getDirection,
                               "The direction in which the piston will operate.");
    py::class_<BlockPistonExtendEvent, BlockPistonEvent>(m, "BlockPistonExtendEvent", "Called when a piston extends.");
    py::class_<BlockPistonRetractEvent, BlockPistonEvent>(m, "BlockPistonRetractEvent",
                                                          "Called when a piston retracts.");
    py::class_<BlockPlaceEvent, BlockEvent, ICancellable>(m, "BlockPlaceEvent", R"doc(
    Called when a block is placed by a player.

    If a BlockPlaceEvent is cancelled, the block will not be placed.
)doc")
        .def_property_readonly("player", &BlockPlaceEvent::getPlayer, py::return_value_policy::reference,
                               "The Player who placed the block involved in this event.")
        .def_property_readonly("block_placed", &BlockPlaceEvent::getBlockPlaced, py::return_value_policy::reference,
                               "The Block that was placed.")
        .def_property_readonly("block_replaced_state", &BlockPlaceEvent::getBlockReplacedState,
                               py::return_value_policy::reference,
                               "The BlockState of the block that was replaced.")
        .def_property_readonly("block_against", &BlockPlaceEvent::getBlockAgainst, py::return_value_policy::reference,
                               "The block that the new block was placed against.");
    py::class_<LeavesDecayEvent, BlockEvent, ICancellable>(m, "LeavesDecayEvent", R"doc(
    Called when leaves are decaying naturally.

    If a Leaves Decay event is cancelled, the leaves will not decay.
)doc");

    // Level events
    py::class_<LevelEvent, Event>(m, "LevelEvent", "Represents events within a level.")
        .def_property_readonly("level", &LevelEvent::getLevel, py::return_value_policy::reference,
                               "The Level primarily involved with this event.");
    py::class_<DimensionEvent, LevelEvent>(m, "DimensionEvent", "Represents events within a dimension.")
        .def_property_readonly("dimension", &DimensionEvent::getDimension, py::return_value_policy::reference,
                               "The Dimension primarily involved with this event.");

    // Chunk events
    py::class_<ChunkEvent, DimensionEvent>(m, "ChunkEvent", "Represents a Chunk related event.")
        .def_property_readonly("chunk", &ChunkEvent::getChunk, py::return_value_policy::reference,
                               "The Chunk being loaded/unloaded.");
    py::class_<ChunkLoadEvent, ChunkEvent>(m, "ChunkLoadEvent", "Called when a chunk is loaded.");
    py::class_<ChunkUnloadEvent, ChunkEvent>(m, "ChunkUnloadEvent", "Called when a chunk is unloaded.");

    // Player events
    py::class_<PlayerEvent, Event>(m, "PlayerEvent", "Represents a player related event.")
        .def_property_readonly("player", &PlayerEvent::getPlayer, py::return_value_policy::reference,
                               "The Player who is involved in this event.");
    auto player_bed_enter_event = py::class_<PlayerBedEnterEvent, PlayerEvent, ICancellable>(
        m, "PlayerBedEnterEvent", "Called when a player is almost about to enter the bed.");
    player_bed_enter_event.def_property_readonly("bed", &PlayerBedEnterEvent::getBed,
                                                 py::return_value_policy::reference,
                                                 "The bed block involved in this event.");

    py::class_<PlayerBedLeaveEvent, PlayerEvent>(m, "PlayerBedLeaveEvent", "Called when a player is leaving a bed.")
        .def_property_readonly("bed", &PlayerBedLeaveEvent::getBed, py::return_value_policy::reference,
                               "The bed block involved in this event.");
    py::class_<PlayerChatEvent, PlayerEvent, ICancellable>(m, "PlayerChatEvent",
                                                           "Called when a player sends a chat message.")
        .def_property("message", &PlayerChatEvent::getMessage, &PlayerChatEvent::setMessage,
                      "The message that the player is attempting to send.")
        .def_property("player", &PlayerChatEvent::getPlayer, &PlayerChatEvent::setPlayer,
                      py::return_value_policy::reference,
                      "The player that this message will be displayed as being sent by.")
        .def_property("format", &PlayerChatEvent::getFormat, &PlayerChatEvent::setFormat, R"doc(
    The format to use to display this chat message.

    See the format string syntax at https://en.cppreference.com/w/cpp/utility/format/spec.html.
)doc")
        .def_property_readonly("recipients", &PlayerChatEvent::getRecipients,
                               py::return_value_policy::reference_internal,
                               "The set of Players who will see this chat message.");
    py::class_<PlayerCommandEvent, PlayerEvent, ICancellable>(m, "PlayerCommandEvent",
                                                              "Called whenever a player runs a command.")
        .def_property("command", &PlayerCommandEvent::getCommand, &PlayerCommandEvent::setCommand,
                      "The command that the player is attempting to send.");
    py::class_<PlayerDimensionChangeEvent, PlayerEvent>(m, "PlayerDimensionChangeEvent",
                                                        "Called when a player switches to another dimension.")
        .def_property_readonly("from_dimension", &PlayerDimensionChangeEvent::getFrom,
                               py::return_value_policy::reference, "The player's previous dimension.")
        .def_property_readonly("to_dimension", &PlayerDimensionChangeEvent::getTo, py::return_value_policy::reference,
                               "The player's new dimension.");
    py::class_<PlayerDropItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerDropItemEvent", "Called when a player drops an item from their inventory.")
        .def_property_readonly("item", &PlayerDropItemEvent::getItem, py::return_value_policy::reference,
                               "The ItemStack dropped by the player.");
    py::class_<PlayerEmoteEvent, PlayerEvent, ICancellable>(m, "PlayerEmoteEvent",
                                                            "Called when a player uses an emote.")
        .def_property_readonly("emote_id", &PlayerEmoteEvent::getEmoteId, "The emote piece ID.")
        .def_property("is_muted", &PlayerEmoteEvent::isMuted, &PlayerEmoteEvent::setMuted, R"doc(
    The muted state for the emote.

    When True, the emote is executed without sending a chat message about the emote.
)doc");
    py::class_<PlayerGameModeChangeEvent, PlayerEvent, ICancellable>(
        m, "PlayerGameModeChangeEvent", "Called when the GameMode of the player is changed.")
        .def_property_readonly("new_game_mode", &PlayerGameModeChangeEvent::getNewGameMode,
                               "The GameMode the player is switched to.");
    auto player_interact_event = py::class_<PlayerInteractEvent, PlayerEvent, ICancellable>(
        m, "PlayerInteractEvent", "Represents an event that is called when a player right-clicks a block.");
    py::native_enum<PlayerInteractEvent::Action>(player_interact_event, "Action", "enum.Enum")
        .value("LEFT_CLICK_BLOCK", PlayerInteractEvent::Action::LeftClickBlock)
        .value("RIGHT_CLICK_BLOCK", PlayerInteractEvent::Action::RightClickBlock)
        .value("LEFT_CLICK_AIR", PlayerInteractEvent::Action::LeftClickAir)
        .value("RIGHT_CLICK_AIR", PlayerInteractEvent::Action::RightClickAir)
        .export_values()
        .finalize();
    player_interact_event
        .def_property_readonly("action", &PlayerInteractEvent::getAction, "The action type of this interaction.")
        .def_property_readonly("has_item", &PlayerInteractEvent::hasItem, "True if this event involved an item.")
        .def_property_readonly("item", &PlayerInteractEvent::getItem,
                               "The item in hand represented by this event, or None if no item.")
        .def_property_readonly("has_block", &PlayerInteractEvent::hasBlock, "True if this event involved a block.")
        .def_property_readonly("block", &PlayerInteractEvent::getBlock, py::return_value_policy::reference,
                               "The block clicked with this item.")
        .def_property_readonly("block_face", &PlayerInteractEvent::getBlockFace,
                               "The face of the block that was clicked.")
        .def_property_readonly("clicked_position", &PlayerInteractEvent::getClickedPosition, R"doc(
    The exact position on the block the player interacted with.

    This will be None outside of Action.RIGHT_CLICK_BLOCK. All vector components are between 0.0 and
    1.0 inclusive.
)doc");
    py::class_<PlayerInteractActorEvent, PlayerEvent, ICancellable>(
        m, "PlayerInteractActorEvent", "Represents an event that is called when a player right-clicks an actor.")
        .def_property_readonly("actor", &PlayerInteractActorEvent::getActor, py::return_value_policy::reference,
                               "The actor that was right-clicked by the player.");
    py::class_<PlayerItemConsumeEvent, PlayerEvent, ICancellable>(m, "PlayerItemConsumeEvent", R"doc(
    Called when a player is finishing consuming an item (food, potion, milk bucket).

    If the ItemStack is modified the server will use the effects of the new item and not remove the
    original one from the player's inventory.

    If the event is cancelled the effect will not be applied and the item will not be removed from
    the player's inventory.
)doc")
        .def_property_readonly("item", &PlayerItemConsumeEvent::getItem,
                               "An ItemStack for the item being consumed.")
        .def_property_readonly("hand", &PlayerItemConsumeEvent::getHand, "The hand used to consume the item.");
    py::class_<PlayerItemHeldEvent, PlayerEvent, ICancellable>(
        m, "PlayerItemHeldEvent", "Called when a player changes their currently held item.")
        .def_property_readonly("new_slot", &PlayerItemHeldEvent::getNewSlot, "The new held slot index.")
        .def_property_readonly("previous_slot", &PlayerItemHeldEvent::getPreviousSlot,
                               "The previous held slot index.");
    py::class_<PlayerJoinEvent, PlayerEvent>(m, "PlayerJoinEvent", "Called when a player joins a server.")
        .def_property("join_message", &PlayerJoinEvent::getJoinMessage, &PlayerJoinEvent::setJoinMessage,
                      "The join message to send to all online players.");
    py::class_<PlayerKickEvent, PlayerEvent, ICancellable>(m, "PlayerKickEvent",
                                                           "Called when a player gets kicked from the server.")
        .def_property("reason", &PlayerKickEvent::getReason, &PlayerKickEvent::setReason,
                      "The reason why the player is getting kicked.");
    py::class_<PlayerLoginEvent, PlayerEvent, ICancellable>(m, "PlayerLoginEvent",
                                                            "Called when a player attempts to login in.")
        .def_property("kick_message", &PlayerLoginEvent::getKickMessage, &PlayerLoginEvent::setKickMessage,
                      "The kick message to display if the event is cancelled.");
    py::class_<PlayerMoveEvent, PlayerEvent, ICancellable>(m, "PlayerMoveEvent", "Called when a player moves.")
        .def_property("from_location", &PlayerMoveEvent::getFrom, &PlayerMoveEvent::setFrom,
                      "The location that this player moved from.")
        .def_property("to_location", &PlayerMoveEvent::getTo, &PlayerMoveEvent::setTo,
                      "The location that this player moved to.");
    py::class_<PlayerJumpEvent, PlayerMoveEvent>(m, "PlayerJumpEvent", "Called when a player jumps.");
    py::class_<PlayerQuitEvent, PlayerEvent>(m, "PlayerQuitEvent", "Called when a player leaves a server.")
        .def_property("quit_message", &PlayerQuitEvent::getQuitMessage, &PlayerQuitEvent::setQuitMessage,
                      "The quit message to send to all online players.");
    py::class_<PlayerRespawnEvent, PlayerEvent>(m, "PlayerRespawnEvent", "Called when a player respawns.");
    py::class_<PlayerSkinChangeEvent, PlayerEvent, ICancellable>(m, "PlayerSkinChangeEvent",
                                                                 "Called when a player changes their skin.")
        .def_property_readonly("new_skin", &PlayerSkinChangeEvent::getNewSkin, "The skin that will be applied.")
        .def_property("skin_change_message", &PlayerSkinChangeEvent::getSkinChangeMessage,
                      &PlayerSkinChangeEvent::setSkinChangeMessage,
                      "The message to send to all online players for this skin change.");
    py::class_<PlayerTeleportEvent, PlayerMoveEvent>(
        m, "PlayerTeleportEvent", "Called when a player is teleported from one location to another.");
    py::class_<PlayerPortalEvent, PlayerTeleportEvent>(
        m, "PlayerPortalEvent", "Called when a player is about to teleport because it is in contact with a portal.");
    py::class_<PlayerPickupItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerPickupItemEvent", "Called when a player picks an item up from the ground.")
        .def_property_readonly("item", &PlayerPickupItemEvent::getItem, py::return_value_policy::reference,
                               "The Item picked up by the entity.");

    // Server events
    py::class_<ServerEvent, Event>(m, "ServerEvent", "Represents a Server-related event.");
    py::class_<BroadcastMessageEvent, ServerEvent, ICancellable>(m, "BroadcastMessageEvent", R"doc(
    Event triggered for server broadcast messages such as from Server.broadcast.

    This event should be async if fired from an async thread.
)doc")
        .def_property("message", &BroadcastMessageEvent::getMessage, &BroadcastMessageEvent::setMessage,
                      "The message to broadcast.")
        .def_property_readonly("recipients", &BroadcastMessageEvent::getRecipients,
                               py::return_value_policy::reference_internal,
                               "The set of CommandSenders who will see this broadcast message.");
    py::class_<MapInitializeEvent, ServerEvent>(m, "MapInitializeEvent", "Called when a map is initialized.")
        .def_property_readonly("map", &MapInitializeEvent::getMap, py::return_value_policy::reference,
                               "The Map initialized in this event.");
    py::class_<PacketReceiveEvent, ServerEvent, ICancellable>(
        m, "PacketReceiveEvent", "Called when the server receives a packet from a connected client.")
        .def_property_readonly("packet_id", &PacketReceiveEvent::getPacketId, "The ID of the packet.")
        .def_property(
            "payload", [](const PacketReceiveEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketReceiveEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "The raw packet data, excluding the header.")
        .def_property_readonly("player", &PacketReceiveEvent::getPlayer, py::return_value_policy::reference, R"doc(
    The Player who is involved in this event.

    This may return None if the packet is sent before the player completes the login process.
)doc")
        .def_property_readonly("address", &PacketReceiveEvent::getAddress,
                               "The network address of the client that sent this packet.")
        .def_property_readonly("sub_client_id", &PacketReceiveEvent::getSubClientId, R"doc(
    The SubClient ID.

    Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
)doc");

    py::class_<PacketSendEvent, ServerEvent, ICancellable>(
        m, "PacketSendEvent", "Called when the server sends a packet to a connected client.")
        .def_property_readonly("packet_id", &PacketSendEvent::getPacketId, "The ID of the packet.")
        .def_property(
            "payload", [](const PacketSendEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketSendEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "The raw packet data, excluding the header.")
        .def_property_readonly("player", &PacketSendEvent::getPlayer, py::return_value_policy::reference, R"doc(
    The Player who is involved in this event.

    This may return None if the packet is sent before the player completes the login process.
)doc")
        .def_property_readonly("address", &PacketSendEvent::getAddress,
                               "The network address to which this packet is being sent.")
        .def_property_readonly("sub_client_id", &PacketSendEvent::getSubClientId, R"doc(
    The SubClient ID.

    Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
)doc");

    py::class_<PluginEnableEvent, ServerEvent>(m, "PluginEnableEvent", "Called when a plugin is enabled.")
        .def_property_readonly("plugin", &PluginEnableEvent::getPlugin, py::return_value_policy::reference,
                               "The Plugin involved in this event.");

    py::class_<PluginDisableEvent, ServerEvent>(m, "PluginDisableEvent", "Called when a plugin is disabled.")
        .def_property_readonly("plugin", &PluginDisableEvent::getPlugin, py::return_value_policy::reference,
                               "The Plugin involved in this event.");

    py::class_<ScriptMessageEvent, ServerEvent, ICancellable>(
        m, "ScriptMessageEvent", "Called when a message is sent by the ``/scriptevent`` command.")
        .def_property_readonly("message_id", &ScriptMessageEvent::getMessageId, "The message id to send.")
        .def_property_readonly("message", &ScriptMessageEvent::getMessage, "The message to send.")
        .def_property_readonly("sender", &ScriptMessageEvent::getSender,
                               "The command sender who sent the script message.");

    py::class_<ServerCommandEvent, ServerEvent, ICancellable>(m, "ServerCommandEvent", R"doc(
    Called when a command is run by a non-player, early in the command handling process.

    You should not use this except for a few cases like logging commands, blocking commands on
    certain places, or applying modifiers.

    The command message contains a slash '/' at the start.
)doc")
        .def_property_readonly("sender", &ServerCommandEvent::getSender, "The command sender.")
        .def_property("command", &ServerCommandEvent::getCommand, &ServerCommandEvent::setCommand,
                      "The command that the server is attempting to execute from the console.");

    py::class_<ServerListPingEvent, ServerEvent, ICancellable>(m, "ServerListPingEvent",
                                                               "Called when a server ping is coming in.")
        .def_property_readonly("address", &ServerListPingEvent::getAddress, "The address the ping is coming from.")
        .def_property("server_guid", &ServerListPingEvent::getServerGuid, &ServerListPingEvent::setServerGuid,
                      "The unique identifier of the server.")
        .def_property("local_port", &ServerListPingEvent::getLocalPort, &ServerListPingEvent::setLocalPort,
                      "The local port of the server.")
        .def_property("local_port_v6", &ServerListPingEvent::getLocalPortV6, &ServerListPingEvent::setLocalPortV6,
                      "The local port of the server for IPv6 support.")
        .def_property("motd", &ServerListPingEvent::getMotd, &ServerListPingEvent::setMotd,
                      "The message of the day.")
        .def_property_readonly("network_protocol_version", &ServerListPingEvent::getNetworkProtocolVersion,
                               "The network protocol version of this server.")
        .def_property("minecraft_version_network", &ServerListPingEvent::getMinecraftVersionNetwork,
                      &ServerListPingEvent::setMinecraftVersionNetwork,
                      "The network version of Minecraft that is supported by this server.")
        .def_property("num_players", &ServerListPingEvent::getNumPlayers, &ServerListPingEvent::setNumPlayers,
                      "The number of players online.")
        .def_property("max_players", &ServerListPingEvent::getMaxPlayers, &ServerListPingEvent::setMaxPlayers,
                      "The maximum number of players allowed.")
        .def_property("level_name", &ServerListPingEvent::getLevelName, &ServerListPingEvent::setLevelName,
                      "The level name.")
        .def_property("game_mode", &ServerListPingEvent::getGameMode, &ServerListPingEvent::setGameMode,
                      "The current game mode.");

    auto server_load_event = py::class_<ServerLoadEvent, Event>(
        m, "ServerLoadEvent", "Called when either the server startup or reload has completed.");
    py::native_enum<ServerLoadEvent::LoadType>(server_load_event, "LoadType", "enum.Enum")
        .value("STARTUP", ServerLoadEvent::LoadType::Startup)
        .export_values()
        .finalize();
    server_load_event.def_property_readonly("type", &ServerLoadEvent::getType, "The load type of this event.");

    py::class_<WeatherEvent, Event>(m, "WeatherEvent", "Represents a Weather-related event.")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "The Level where this event is occurring.");
    py::class_<ThunderChangeEvent, WeatherEvent, ICancellable>(m, "ThunderChangeEvent",
                                                               "Called when the thunder state in a world is changing.")
        .def_property_readonly("to_thunder_state", &ThunderChangeEvent::toThunderState,
                               "True if the weather is being set to thundering, False otherwise.");
    py::class_<WeatherChangeEvent, WeatherEvent, ICancellable>(
        m, "WeatherChangeEvent", "Called when the weather (rain) state in a world is changing.")
        .def_property_readonly("to_weather_state", &WeatherChangeEvent::toWeatherState,
                               "True if the weather is being set to raining, False otherwise.");
}

}  // namespace endstone::python
