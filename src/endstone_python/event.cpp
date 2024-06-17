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

#include "endstone/event/event.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/event/actor/actor_spawn_event.h"
#include "endstone/event/event_priority.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/player/player_command_event.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_login_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/event/server/broadcast_message_event.h"
#include "endstone/event/server/plugin_disable_event.h"
#include "endstone/event/server/plugin_enable_event.h"
#include "endstone/event/server/server_command_event.h"
#include "endstone/event/server/server_list_ping_event.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"

namespace py = pybind11;

namespace endstone::detail {

void init_event(py::module_ &m, py::class_<Event> &event, py::enum_<EventPriority> &event_priority)
{
    event.def_property_readonly("event_name", &Event::getEventName, "Gets a user-friendly identifier for this event.")
        .def_property_readonly("cancellable", &Event::isCancellable,
                               "Whether the event can be cancelled by a plugin or the server.")
        .def_property("cancelled", &Event::isCancelled, &Event::setCancelled,
                      "Gets or sets the cancellation state of this event. A cancelled event will not be executed in "
                      "the server, but will still pass to other plugins")
        .def_property_readonly("asynchronous", &Event::isAsynchronous, "Whether the event fires asynchronously.");

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

    py::class_<ActorEvent, Event>(m, "ActorEvent", "Represents an Actor-related event.")
        .def_property_readonly("actor", &ActorRemoveEvent::getActor, py::return_value_policy::reference,
                               "Returns the Actor involved in this event");
    py::class_<ActorDeathEvent, ActorEvent>(m, "ActorDeathEvent", "Called when an Actor dies.");
    py::class_<ActorRemoveEvent, ActorEvent>(m, "ActorRemoveEvent", "Called when an Actor is removed.");
    py::class_<ActorSpawnEvent, ActorEvent>(m, "ActorSpawnEvent", "Called when an Actor is spawned into a world.");

    py::class_<PlayerEvent, Event>(m, "PlayerEvent", "Represents a player related event")
        .def_property_readonly("player", &PlayerEvent::getPlayer, py::return_value_policy::reference,
                               "Returns the player involved in this event.");
    py::class_<PlayerChatEvent, PlayerEvent>(m, "PlayerChatEvent", "Called when a player sends a chat message.")
        .def_property("message", &PlayerChatEvent::getMessage, &PlayerChatEvent::setMessage,
                      "Gets or sets the message that the player will send.");
    py::class_<PlayerCommandEvent, PlayerEvent>(m, "PlayerCommandEvent", "Called whenever a player runs a command.")
        .def_property("command", &PlayerCommandEvent::getCommand, &PlayerCommandEvent::setCommand,
                      "Gets or sets the command that the player will send.");
    py::class_<PlayerDeathEvent, ActorDeathEvent, PlayerEvent>(m, "PlayerDeathEvent", "Called when a player dies")
        .def_property("death_message", &PlayerDeathEvent::getDeathMessage, &PlayerDeathEvent::setDeathMessage,
                      "Gets or sets the death message that will appear to everyone on the server.");
    py::class_<PlayerJoinEvent, PlayerEvent>(m, "PlayerJoinEvent", "Called when a player joins a server");
    py::class_<PlayerLoginEvent, PlayerEvent>(m, "PlayerLoginEvent", "Called when a player attempts to login in.")
        .def_property("kick_message", &PlayerLoginEvent::getKickMessage, &PlayerLoginEvent::setKickMessage,
                      "Gets or sets kick message to display if event is cancelled");
    py::class_<PlayerQuitEvent, PlayerEvent>(m, "PlayerQuitEvent", "Called when a player leaves a server.");

    py::class_<BroadcastMessageEvent, Event>(
        m, "BroadcastMessageEvent", "Event triggered for server broadcast messages such as from Server.broadcast")
        .def_property("message", &BroadcastMessageEvent::getMessage, &BroadcastMessageEvent::setMessage,
                      "Gets or sets the message to broadcast.")
        .def_property_readonly("recipients", &BroadcastMessageEvent::getRecipients,
                               py::return_value_policy::reference_internal,
                               "Gets a set of recipients that this broadcast message will be displayed to.");

    py::class_<PluginEnableEvent, Event>(m, "PluginEnableEvent", "Called when a plugin is enabled.")
        .def_property_readonly("plugin", &PluginEnableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<PluginDisableEvent, Event>(m, "PluginDisableEvent", "Called when a plugin is disabled.")
        .def_property_readonly("plugin", &PluginDisableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<ServerCommandEvent, Event>(m, "ServerCommandEvent",
                                          "Called when the console runs a command, early in the process.")
        .def_property_readonly("sender", &ServerCommandEvent::getSender, "Get the command sender.")
        .def_property("command", &ServerCommandEvent::getCommand, &ServerCommandEvent::setCommand,
                      "Gets or sets the command that the server will execute");

    py::class_<ServerListPingEvent, Event>(m, "ServerListPingEvent", "Called when a server ping is coming in.")
        .def_property_readonly("remote_host", &ServerListPingEvent::getRemoteHost,
                               "Get the host the ping is coming from.")
        .def_property_readonly("remote_port", &ServerListPingEvent::getRemotePort,
                               "Get the port the ping is coming from.")
        .def_property_readonly("server_guid", &ServerListPingEvent::getServerGuid,
                               "Get the unique identifier of the server.")
        .def_property_readonly("local_port", &ServerListPingEvent::getLocalPort, "Get the local port of the server.")
        .def_property_readonly("local_port_v6", &ServerListPingEvent::getLocalPortV6,
                               "Get the local port of the server for IPv6 support")
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

    py::class_<ThunderChangeEvent, Event>(m, "ThunderChangeEvent",
                                          "Called when the thunder state in a world is changing.")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "Returns the Level where this event is occurring")
        .def_property_readonly("to_thunder_state", &ThunderChangeEvent::toThunderState,
                               "Gets the state of thunder that the world is being set to");

    py::class_<WeatherChangeEvent, Event>(m, "WeatherChangeEvent",
                                          "Called when the weather (rain) state in a world is changing.")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "Returns the Level where this event is occurring")
        .def_property_readonly("to_weather_state", &WeatherChangeEvent::toWeatherState,
                               "Gets the state of weather that the world is being set to");
}

}  // namespace endstone::detail
