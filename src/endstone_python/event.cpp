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

#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/event/actor/actor_spawn_event.h"
#include "endstone/event/event_priority.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"
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

    event.def_property_readonly("event_name", &Event::getEventName, "Gets a user-friendly identifier for this event.")
        .def_property_readonly("cancellable", &Event::isCancellable,
                               "Whether the event can be cancelled by a plugin or the server.")
        .def_property("cancelled", &Event::isCancelled, &Event::setCancelled,
                      "The cancellation state of this event. A cancelled event will not be executed in "
                      "the server, but will still pass to other plugins")
        .def("asynchronous", &Event::isAsynchronous);

    py::enum_<Event::Result>(event, "Result")
        .value("DENY", Event::Result::DENY)
        .value("DEFAULT", Event::Result::DEFAULT)
        .value("ALLOW", Event::Result::ALLOW)
        .export_values();

    py::class_<ActorRemoveEvent, Event>(m, "ActorRemoveEvent")
        .def_property_readonly("actor", &ActorRemoveEvent::getActor, py::return_value_policy::reference,
                               "Returns the Actor being removed");

    py::class_<ActorSpawnEvent, Event>(m, "ActorSpawnEvent")
        .def_property_readonly("actor", &ActorSpawnEvent::getActor, py::return_value_policy::reference,
                               "Returns the Actor being spawned");

    py::class_<PlayerChatEvent, Event>(m, "PlayerChatEvent")
        .def_property_readonly("player", &PlayerChatEvent::getPlayer, py::return_value_policy::reference,
                               "Returns the Player who joins the server")
        .def_property("message", &PlayerChatEvent::getMessage, &PlayerChatEvent::setMessage,
                      "The message that the player will send.");

    py::class_<PlayerJoinEvent, Event>(m, "PlayerJoinEvent")
        .def_property_readonly("player", &PlayerJoinEvent::getPlayer, py::return_value_policy::reference,
                               "Returns the Player who joins the server");

    py::class_<PlayerQuitEvent, Event>(m, "PlayerQuitEvent")
        .def_property_readonly("player", &PlayerQuitEvent::getPlayer, py::return_value_policy::reference,
                               "Returns the Player who leaves the server");

    py::class_<PluginEnableEvent, Event>(m, "PluginEnableEvent")
        .def_property_readonly("plugin", &PluginEnableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<PluginDisableEvent, Event>(m, "PluginDisableEvent")
        .def_property_readonly("plugin", &PluginDisableEvent::getPlugin, py::return_value_policy::reference);

    py::class_<ServerCommandEvent, Event>(m, "ServerCommandEvent")
        .def_property_readonly("sender", &ServerCommandEvent::getSender, "Get the command sender.")
        .def_property("command", &ServerCommandEvent::getCommand, &ServerCommandEvent::setCommand,
                      "The command that the server will execute");

    py::class_<ServerListPingEvent, Event>(m, "ServerListPingEvent")
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
                      "The message of the day message.")
        .def_property_readonly("network_protocol_version", &ServerListPingEvent::getNetworkProtocolVersion,
                               "Get the network protocol version of this server")
        .def_property("minecraft_version_network", &ServerListPingEvent::getMinecraftVersionNetwork,
                      &ServerListPingEvent::setMinecraftVersionNetwork,
                      "The network version of Minecraft that is supported by this server")
        .def_property("num_players", &ServerListPingEvent::getNumPlayers, &ServerListPingEvent::setNumPlayers,
                      "The number of players online.")
        .def_property("max_players", &ServerListPingEvent::getMaxPlayers, &ServerListPingEvent::setMaxPlayers,
                      "The maximum number of players allowed.")
        .def_property("level_name", &ServerListPingEvent::getLevelName, &ServerListPingEvent::setLevelName,
                      "The level name.")
        .def_property("game_mode", &ServerListPingEvent::getGameMode, &ServerListPingEvent::setGameMode,
                      "The current game mode.");

    auto server_load_event = py::class_<ServerLoadEvent, Event>(m, "ServerLoadEvent");
    py::enum_<ServerLoadEvent::LoadType>(server_load_event, "LoadType")
        .value("STARTUP", ServerLoadEvent::LoadType::Startup)
        .export_values();
    server_load_event.def_property_readonly("type", &ServerLoadEvent::getType);

    py::class_<ThunderChangeEvent, Event>(m, "ThunderChangeEvent")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "Returns the Level where this event is occurring")
        .def_property_readonly("to_thunder_state", &ThunderChangeEvent::toThunderState,
                               "Gets the state of thunder that the world is being set to");

    py::class_<WeatherChangeEvent, Event>(m, "WeatherChangeEvent")
        .def_property_readonly("level", &WeatherChangeEvent::getLevel, py::return_value_policy::reference,
                               "Returns the Level where this event is occurring")
        .def_property_readonly("to_weather_state", &WeatherChangeEvent::toWeatherState,
                               "Gets the state of weather that the world is being set to");
}

}  // namespace endstone::detail
