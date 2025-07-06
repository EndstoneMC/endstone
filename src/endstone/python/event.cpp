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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

void init_event(py::module_ &m, py::class_<Event> &event, py::enum_<EventPriority> &event_priority)
{
    event.def_property_readonly("event_name", &Event::getEventName, "Gets a user-friendly identifier for this event.")
        .def_property_readonly("is_asynchronous", &Event::isAsynchronous, "Whether the event fires asynchronously.");

    event_priority
        .value("LOWEST", EventPriority::Lowest,
               "Event call is of very low importance and should be run first, to allow other plugins to further "
               "customise the outcome")
        .value("LOW", EventPriority::Low, "Event call is of low importance")
        .value("NORMAL", EventPriority::Normal,
               " Event call is neither important nor unimportant, and may be run normally")
        .value("HIGH", EventPriority::High, "Event call is of high importance")
        .value("HIGHEST", EventPriority::Highest,
               "Event call is critical and must have the final say in what happens to the event")
        .value("MONITOR", EventPriority::Monitor,
               "Event is listened to purely for monitoring the outcome of an event. No modifications to the event "
               "should be made under this priority.");

    py::class_<ICancellable>(m, "Cancellable", "Represents an event that may be cancelled by a plugin or the server.")
        .def_property(
            "cancelled",
            [](const ICancellable &self) {
                PyErr_WarnEx(PyExc_FutureWarning,
                             "The event.cancelled property is deprecated and will be removed from endstone in a future "
                             "version. Use event.is_cancelled instead.",
                             1);
                return self.isCancelled();
            },
            [](ICancellable &self, const bool value) {
                PyErr_WarnEx(PyExc_FutureWarning,
                             "The event.cancelled property is deprecated and will be removed from endstone in a future "
                             "version. Use event.is_cancelled instead.",
                             1);
                self.setCancelled(value);
            },
            "Gets or sets the cancellation state of this event. A cancelled event will not be executed in "
            "the server, but will still pass to other plugins. [Warning] Deprecated: Use is_cancelled instead.")
        .def_property("is_cancelled", &ICancellable::isCancelled, &ICancellable::setCancelled,
                      "Gets or sets the cancellation state of this event. A cancelled event will not be executed in "
                      "the server, but will still pass to other plugins.")
        .def("cancel", &ICancellable::cancel,
             "Cancel this event. A cancelled event will not be executed in the server, but will still pass to other "
             "plugins.");

    // Actor events
    py::class_<ActorEvent<Actor>, Event>(m, "ActorEvent", "Represents an Actor-related event.")
        .def_property_readonly("actor", &ActorEvent<Actor>::getActor, py::return_value_policy::reference,
                               "Returns the Actor involved in this event");
    py::class_<ActorEvent<Mob>, Event>(m, "MobEvent", "Represents an Mob-related event.")
        .def_property_readonly("actor", &ActorEvent<Mob>::getActor, py::return_value_policy::reference,
                               "Returns the Mob involved in this event");
    py::class_<ActorDamageEvent, ActorEvent<Mob>, ICancellable>(m, "ActorDamageEvent",
                                                                "Called when an Actor is damaged.")
        .def_property("damage", &ActorDamageEvent::getDamage, &ActorDamageEvent::setDamage,
                      "Gets or sets the amount of damage caused by the event")
        .def_property_readonly("damage_source", &ActorDamageEvent::getDamageSource, py::return_value_policy::reference,
                               "Gets the source of damage.");
    py::class_<ActorDeathEvent, ActorEvent<Mob>>(m, "ActorDeathEvent", "Called when an Actor dies.")
        .def_property_readonly("damage_source", &ActorDeathEvent::getDamageSource, py::return_value_policy::reference,
                               "Gets the source of damage which caused the death.");
    py::class_<ActorExplodeEvent, ActorEvent<Actor>, ICancellable>(m, "ActorExplodeEvent",
                                                                   "Called when an Actor explodes.")
        .def_property_readonly("location", &ActorExplodeEvent::getLocation,
                               "Returns the location where the explosion happened.")
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
            "Gets or sets the list of blocks that would have been removed or were removed from the explosion event.");
    py::class_<ActorKnockbackEvent, ActorEvent<Mob>, ICancellable>(m, "ActorKnockbackEvent",
                                                                   "Called when a living entity receives knockback.")
        .def_property_readonly("source", &ActorKnockbackEvent::getSource, py::return_value_policy::reference,
                               "Get the source actor that has caused knockback to the defender, if exists.")
        .def_property("knockback", &ActorKnockbackEvent::getKnockback, &ActorKnockbackEvent::setKnockback,
                      "Gets or sets the knockback that will be applied to the entity.");
    py::class_<ActorRemoveEvent, ActorEvent<Actor>>(m, "ActorRemoveEvent", "Called when an Actor is removed.");
    py::class_<ActorSpawnEvent, ActorEvent<Actor>, ICancellable>(m, "ActorSpawnEvent",
                                                                 "Called when an Actor is spawned into a world.");
    py::class_<ActorTeleportEvent, ActorEvent<Actor>, ICancellable>(
        m, "ActorTeleportEvent", "Called when a non-player entity is teleported from one location to another.")
        .def_property("from_location", &ActorTeleportEvent::getFrom, &ActorTeleportEvent::setFrom,
                      "Gets or sets the location that this actor moved from.")
        .def_property("to_location", &ActorTeleportEvent::getTo, &ActorTeleportEvent::setTo,
                      "Gets or sets the location that this actor moved to.");

    // Block events
    py::class_<BlockEvent, Event>(m, "BlockEvent", "Represents an Block-related event")
        .def_property_readonly("block", &BlockEvent::getBlock, py::return_value_policy::reference,
                               "Gets the block involved in this event.");
    py::class_<BlockBreakEvent, BlockEvent, ICancellable>(m, "BlockBreakEvent",
                                                          "Called when a block is broken by a player.")
        .def_property_readonly("player", &BlockBreakEvent::getPlayer, py::return_value_policy::reference,
                               "Gets the Player that is breaking the block involved in this event.");
    py::class_<BlockCookEvent, BlockEvent, ICancellable>(m, "BlockCookEvent",
                                                         "Called when an ItemStack is successfully cooked in a block.")
        .def_property_readonly("source", &BlockCookEvent::getSource, py::return_value_policy::reference,
                               "Gets the smelted ItemStack for this event")
        .def_property(
            "result", &BlockCookEvent::getResult,
            [](BlockCookEvent &self, const ItemStack &result) { self.setResult(result.clone()); },
            py::return_value_policy::reference, "Gets or sets the resultant ItemStack for this event");
    py::class_<BlockPistonEvent, BlockEvent, ICancellable>(m, "BlockPistonEvent",
                                                           "Called when a piston block is triggered")
        .def_property_readonly("direction", &BlockPistonEvent::getDirection,
                               "Return the direction in which the piston will operate.");
    py::class_<BlockPistonExtendEvent, BlockPistonEvent>(m, "BlockPistonExtendEvent", "Called when a piston extends.");
    py::class_<BlockPistonRetractEvent, BlockPistonEvent>(m, "BlockPistonRetractEvent",
                                                          "Called when a piston retracts.");
    py::class_<BlockPlaceEvent, BlockEvent, ICancellable>(m, "BlockPlaceEvent",
                                                          "Called when a block is placed by a player.")
        .def_property_readonly("player", &BlockPlaceEvent::getPlayer, py::return_value_policy::reference,
                               "Gets the player who placed the block involved in this event.")
        .def_property_readonly("block_placed_state", &BlockPlaceEvent::getBlockPlacedState,
                               py::return_value_policy::reference,
                               "Gets the BlockState for the block which was placed.")
        .def_property_readonly("block_replaced", &BlockPlaceEvent::getBlockReplaced, py::return_value_policy::reference,
                               "Gets the block which was replaced.")
        .def_property_readonly("block_against", &BlockPlaceEvent::getBlockAgainst, py::return_value_policy::reference,
                               "Gets the block that this block was placed against");
    py::class_<LeavesDecayEvent, BlockEvent, ICancellable>(
        m, "LeavesDecayEvent",
        "Called when leaves are decaying naturally.\nIf a Leaves Decay event is cancelled, the leaves will not decay.");

    // Level events
    py::class_<LevelEvent, Event>(m, "LevelEvent", "Represents events within a level")
        .def_property_readonly("level", &LevelEvent::getLevel, py::return_value_policy::reference,
                               "Gets the level primarily involved with this event");
    py::class_<DimensionEvent, LevelEvent>(m, "DimensionEvent", "Represents events within a dimension")
        .def_property_readonly("dimension", &DimensionEvent::getDimension, py::return_value_policy::reference,
                               "Gets the dimension primarily involved with this event");

    // Chunk events
    py::class_<ChunkEvent, DimensionEvent>(m, "ChunkEvent", "Represents a Chunk related event")
        .def_property_readonly("chunk", &ChunkEvent::getChunk, py::return_value_policy::reference,
                               "Gets the chunk being loaded/unloaded");
    py::class_<ChunkLoadEvent, ChunkEvent>(m, "ChunkLoadEvent", "Called when a chunk is loaded");
    py::class_<ChunkUnloadEvent, ChunkEvent>(m, "ChunkUnloadEvent", "Called when a chunk is unloaded");

    // Player events
    py::class_<PlayerEvent, Event>(m, "PlayerEvent", "Represents a player related event")
        .def_property_readonly("player", &PlayerEvent::getPlayer, py::return_value_policy::reference,
                               "Returns the player involved in this event.");
    py::class_<PlayerChatEvent, PlayerEvent, ICancellable>(m, "PlayerChatEvent",
                                                           "Called when a player sends a chat message.")
        .def_property("message", &PlayerChatEvent::getMessage, &PlayerChatEvent::setMessage,
                      "Gets or sets the message that the player will send.");
    py::class_<PlayerCommandEvent, PlayerEvent, ICancellable>(m, "PlayerCommandEvent",
                                                              "Called whenever a player runs a command.")
        .def_property("command", &PlayerCommandEvent::getCommand, &PlayerCommandEvent::setCommand,
                      "Gets or sets the command that the player will send.");
    py::class_<PlayerDeathEvent, ActorDeathEvent, PlayerEvent>(m, "PlayerDeathEvent", "Called when a player dies")
        .def_property("death_message", &PlayerDeathEvent::getDeathMessage, &PlayerDeathEvent::setDeathMessage,
                      "Gets or sets the death message that will appear to everyone on the server.");
    py::class_<PlayerDropItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerDropItemEvent", "Called when a player drops an item from their inventory")
        .def_property_readonly("item", &PlayerDropItemEvent::getItem, py::return_value_policy::reference,
                               "Gets the ItemStack dropped by the player");
    py::class_<PlayerEmoteEvent, PlayerEvent>(m, "PlayerEmoteEvent", "Called when a player uses and emote")
        .def_property_readonly("emote_id", &PlayerEmoteEvent::getEmoteId, "Gets the emote ID");
    py::class_<PlayerGameModeChangeEvent, PlayerEvent, ICancellable>(
        m, "PlayerGameModeChangeEvent", "Called when the GameMode of the player is changed.")
        .def_property_readonly("new_game_mode", &PlayerGameModeChangeEvent::getNewGameMode,
                               "Gets the GameMode the player is switched to.");
    auto player_interact_event = py::class_<PlayerInteractEvent, PlayerEvent, ICancellable>(
        m, "PlayerInteractEvent", "Represents an event that is called when a player right-clicks a block.");
    py::enum_<PlayerInteractEvent::Action>(player_interact_event, "Action")
        .value("LEFT_CLICK_BLOCK", PlayerInteractEvent::Action::LeftClickBlock)
        .value("RIGHT_CLICK_BLOCK", PlayerInteractEvent::Action::RightClickBlock)
        .value("LEFT_CLICK_AIR", PlayerInteractEvent::Action::LeftClickAir)
        .value("RIGHT_CLICK_AIR", PlayerInteractEvent::Action::RightClickAir);
    player_interact_event
        .def_property_readonly("action", &PlayerInteractEvent::getAction, "Returns the action type of interaction")
        .def_property_readonly("has_item", &PlayerInteractEvent::hasItem, "Check if this event involved an item")
        .def_property_readonly("item", &PlayerInteractEvent::getItem, py::return_value_policy::reference,
                               "Returns the item in hand represented by this event")
        .def_property_readonly("has_block", &PlayerInteractEvent::hasBlock, "Check if this event involved a block")
        .def_property_readonly("block", &PlayerInteractEvent::getBlock, py::return_value_policy::reference,
                               "Returns the clicked block")
        .def_property_readonly("block_face", &PlayerInteractEvent::getBlockFace,
                               "Returns the face of the block that was clicked")
        .def_property_readonly("clicked_position", &PlayerInteractEvent::getClickedPosition,
                               "Gets the exact position on the block the player interacted with.");
    py::class_<PlayerInteractActorEvent, PlayerEvent, ICancellable>(
        m, "PlayerInteractActorEvent", "Represents an event that is called when a player right-clicks an actor.")
        .def_property_readonly("actor", &PlayerInteractActorEvent::getActor, py::return_value_policy::reference,
                               "Gets the actor that was right-clicked by the player.");
    py::class_<PlayerItemConsumeEvent, PlayerEvent, ICancellable>(
        m, "PlayerItemConsumeEvent", "Called when a player is finishing consuming an item (food, potion, milk bucket).")
        .def_property_readonly("item", &PlayerItemConsumeEvent::getItem, py::return_value_policy::reference,
                               "Gets or sets the item that is being consumed.")
        .def_property_readonly("hand", &PlayerItemConsumeEvent::getHand, "Get the hand used to consume the item.");
    py::class_<PlayerJoinEvent, PlayerEvent>(m, "PlayerJoinEvent", "Called when a player joins a server")
        .def_property("join_message", &PlayerJoinEvent::getJoinMessage, &PlayerJoinEvent::setJoinMessage,
                      "Gets or sets the join message to send to all online players.");
    py::class_<PlayerKickEvent, PlayerEvent, ICancellable>(m, "PlayerKickEvent",
                                                           "Called when a player gets kicked from the server")
        .def_property("reason", &PlayerKickEvent::getReason, &PlayerKickEvent::setReason,
                      "Gets or sets the reason why the player is getting kicked");
    py::class_<PlayerLoginEvent, PlayerEvent, ICancellable>(m, "PlayerLoginEvent",
                                                            "Called when a player attempts to login in.")
        .def_property("kick_message", &PlayerLoginEvent::getKickMessage, &PlayerLoginEvent::setKickMessage,
                      "Gets or sets kick message to display if event is cancelled");
    py::class_<PlayerMoveEvent, PlayerEvent, ICancellable>(m, "PlayerMoveEvent", "Called when a player moves.")
        .def_property("from_location", &PlayerMoveEvent::getFrom, &PlayerMoveEvent::setFrom,
                      "Gets or sets the location that this player moved from.")
        .def_property("to_location", &PlayerMoveEvent::getTo, &PlayerMoveEvent::setTo,
                      "Gets or sets the location that this player moved to.");
    py::class_<PlayerJumpEvent, PlayerMoveEvent>(m, "PlayerJumpEvent", "Called when a player jumps.");
    py::class_<PlayerQuitEvent, PlayerEvent>(m, "PlayerQuitEvent", "Called when a player leaves a server.")
        .def_property("quit_message", &PlayerQuitEvent::getQuitMessage, &PlayerQuitEvent::setQuitMessage,
                      "Gets or sets the quit message to send to all online players.");
    py::class_<PlayerRespawnEvent, PlayerEvent>(m, "PlayerRespawnEvent", "Called when a player respawns.");
    py::class_<PlayerTeleportEvent, PlayerMoveEvent>(
        m, "PlayerTeleportEvent", "Called when a player is teleported from one location to another.");
    py::class_<PlayerPickupItemEvent, PlayerEvent, ICancellable>(
        m, "PlayerPickupItemEvent", "Called when a player picks an item up from the ground.")
        .def_property_readonly("item", &PlayerPickupItemEvent::getItem, py::return_value_policy::reference,
                               "Gets the Item picked up by the entity.");

    // Server events
    py::class_<ServerEvent, Event>(m, "ServerEvent", "Represents a server-related event");
    py::class_<BroadcastMessageEvent, ServerEvent, ICancellable>(
        m, "BroadcastMessageEvent", "Event triggered for server broadcast messages such as from Server.broadcast")
        .def_property("message", &BroadcastMessageEvent::getMessage, &BroadcastMessageEvent::setMessage,
                      "Gets or sets the message to broadcast.")
        .def_property_readonly("recipients", &BroadcastMessageEvent::getRecipients,
                               py::return_value_policy::reference_internal,
                               "Gets a set of recipients that this broadcast message will be displayed to.");
    py::class_<PacketReceiveEvent, ServerEvent, ICancellable>(
        m, "PacketReceiveEvent", "Called when the server receives a packet from a connected client.")
        .def_property_readonly("packet_id", &PacketReceiveEvent::getPacketId, "Gets the ID of the packet.")
        .def_property(
            "payload", [](const PacketReceiveEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketReceiveEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "Gets or sets the raw packet data **excluding** the header.")
        .def_property_readonly(
            "player", &PacketReceiveEvent::getPlayer, py::return_value_policy::reference,
            "Gets the player involved in this event\n"
            "NOTE: This may return None if the packet is sent before the player completes the login process.")
        .def_property_readonly("address", &PacketReceiveEvent::getAddress,
                               "Gets the network address to which this packet is being sent.")
        .def_property_readonly("sub_client_id", &PacketReceiveEvent::getSubClientId,
                               "Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).");

    py::class_<PacketSendEvent, ServerEvent, ICancellable>(
        m, "PacketSendEvent", "Called when the server sends a packet to a connected client.")
        .def_property_readonly("packet_id", &PacketSendEvent::getPacketId, "Gets the ID of the packet.")
        .def_property(
            "payload", [](const PacketSendEvent &self) { return py::bytes(self.getPayload()); },
            [](PacketSendEvent &self, const py::bytes &payload) { self.setPayload(payload); },
            "Gets or sets the raw packet data **excluding** the header.")
        .def_property_readonly(
            "player", &PacketSendEvent::getPlayer, py::return_value_policy::reference,
            "Gets the player involved in this event\n"
            "NOTE: This may return None if the packet is sent before the player completes the login process.")
        .def_property_readonly("address", &PacketSendEvent::getAddress,
                               "Gets the network address to which this packet is being sent.")
        .def_property_readonly("sub_client_id", &PacketSendEvent::getSubClientId,
                               "Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).");

    py::class_<PluginEnableEvent, ServerEvent>(m, "PluginEnableEvent", "Called when a plugin is enabled.")
        .def_property_readonly("plugin", &PluginEnableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<PluginDisableEvent, ServerEvent>(m, "PluginDisableEvent", "Called when a plugin is disabled.")
        .def_property_readonly("plugin", &PluginDisableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<ScriptMessageEvent, ServerEvent, ICancellable>(m, "ScriptMessageEvent",
                                                              "Called when a message is sent by `/scriptevent` command")
        .def_property_readonly("message_id", &ScriptMessageEvent::getMessageId, "Get the message id to send.")
        .def_property_readonly("message", &ScriptMessageEvent::getMessage, "Get the message to send.")
        .def_property_readonly("sender", &ScriptMessageEvent::getSender,
                               "Gets the command sender who initiated the command.");

    py::class_<ServerCommandEvent, ServerEvent, ICancellable>(
        m, "ServerCommandEvent", "Called when the console runs a command, early in the process.")
        .def_property_readonly("sender", &ServerCommandEvent::getSender, "Get the command sender.")
        .def_property("command", &ServerCommandEvent::getCommand, &ServerCommandEvent::setCommand,
                      "Gets or sets the command that the server will execute");

    py::class_<ServerListPingEvent, ServerEvent, ICancellable>(m, "ServerListPingEvent",
                                                               "Called when a server ping is coming in.")
        .def_property_readonly("remote_host", &ServerListPingEvent::getRemoteHost,
                               "Get the host the ping is coming from.")
        .def_property_readonly("remote_port", &ServerListPingEvent::getRemotePort,
                               "Get the port the ping is coming from.")
        .def_property("server_guid", &ServerListPingEvent::getServerGuid, &ServerListPingEvent::setServerGuid,
                      "Get or set the unique identifier of the server.")
        .def_property("local_port", &ServerListPingEvent::getLocalPort, &ServerListPingEvent::setLocalPort,
                      "Get or set the local port of the server.")
        .def_property("local_port_v6", &ServerListPingEvent::getLocalPortV6, &ServerListPingEvent::setLocalPortV6,
                      "Get or set the local port of the server for IPv6 support")
        .def_property("motd", &ServerListPingEvent::getMotd, &ServerListPingEvent::setMotd,
                      "Gets or sets the message of the day message.")
        .def_property_readonly("network_protocol_version", &ServerListPingEvent::getNetworkProtocolVersion,
                               "Get the network protocol version of this server")
        .def_property("minecraft_version_network", &ServerListPingEvent::getMinecraftVersionNetwork,
                      &ServerListPingEvent::setMinecraftVersionNetwork,
                      "Gets or sets the network version of Minecraft that is supported by this server")
        .def_property("num_players", &ServerListPingEvent::getNumPlayers, &ServerListPingEvent::setNumPlayers,
                      "Gets or sets the number of players online.")
        .def_property("max_players", &ServerListPingEvent::getMaxPlayers, &ServerListPingEvent::setMaxPlayers,
                      "Gets or sets the maximum number of players allowed.")
        .def_property("level_name", &ServerListPingEvent::getLevelName, &ServerListPingEvent::setLevelName,
                      "Gets or sets the level name.")
        .def_property("game_mode", &ServerListPingEvent::getGameMode, &ServerListPingEvent::setGameMode,
                      "Gets or sets the current game mode.");

    auto server_load_event = py::class_<ServerLoadEvent, Event>(
        m, "ServerLoadEvent", "Called when either the server startup or reload has completed.");
    py::enum_<ServerLoadEvent::LoadType>(server_load_event, "LoadType")
        .value("STARTUP", ServerLoadEvent::LoadType::Startup)
        .export_values();
    server_load_event.def_property_readonly("type", &ServerLoadEvent::getType);

    py::class_<WeatherEvent, Event>(m, "WeatherEvent", "Represents a weather-related event")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "Returns the Level where this event is occurring");
    py::class_<ThunderChangeEvent, WeatherEvent, ICancellable>(m, "ThunderChangeEvent",
                                                               "Called when the thunder state in a world is changing.")
        .def_property_readonly("to_thunder_state", &ThunderChangeEvent::toThunderState,
                               "Gets the state of thunder that the world is being set to");
    py::class_<WeatherChangeEvent, WeatherEvent, ICancellable>(
        m, "WeatherChangeEvent", "Called when the weather (rain) state in a world is changing.")
        .def_property_readonly("to_weather_state", &WeatherChangeEvent::toWeatherState,
                               "Gets the state of weather that the world is being set to");
}

}  // namespace endstone::python
