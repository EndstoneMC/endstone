from __future__ import annotations

from ..models import EventScenario


def build_server_scenarios(ctx, namespace) -> list[EventScenario]:
    command = namespace['command']
    initialize_map = namespace['initialize_map']
    load_chunk = namespace['load_chunk']
    load_dimension = namespace['load_dimension']
    server = namespace['server']
    thunder_change = namespace['thunder_change']
    toggle_event_logger_plugin = namespace['toggle_event_logger_plugin']
    unknown_command = namespace['unknown_command']
    unload_chunk = namespace['unload_chunk']
    verify_broadcast_message = namespace['verify_broadcast_message']
    verify_chunk_event = namespace['verify_chunk_event']
    verify_dimension_load = namespace['verify_dimension_load']
    verify_map_initialize = namespace['verify_map_initialize']
    verify_packet_event = namespace['verify_packet_event']
    verify_player_connect = namespace['verify_player_connect']
    verify_player_login_cancel = namespace['verify_player_login_cancel']
    verify_player_skin_change = namespace['verify_player_skin_change']
    verify_plugin_lifecycle = namespace['verify_plugin_lifecycle']
    verify_script_message = namespace['verify_script_message']
    verify_server_command = namespace['verify_server_command']
    verify_server_list_ping = namespace['verify_server_list_ping']
    verify_thunder_change = namespace['verify_thunder_change']
    verify_unknown_command = namespace['verify_unknown_command']
    verify_weather_change = namespace['verify_weather_change']
    weather_change = namespace['weather_change']
    return [
        EventScenario(
                    "server-command",
                    ("ServerCommandEvent",),
                    lambda: None,
                    driver_action="console-command",
                    driver_data={"command": "say endstone automated event test"},
                    verify=lambda snapshots: verify_server_command(snapshots, False),
                ),
        EventScenario(
                    "server-command-cancel",
                    ("ServerCommandEvent",),
                    lambda: None,
                    driver_action="console-command",
                    driver_data={"command": "say endstone automated event test"},
                    cancel_events=("ServerCommandEvent",),
                    verify=lambda snapshots: verify_server_command(snapshots, True),
                ),
        EventScenario(
                    "broadcast-message",
                    ("BroadcastMessageEvent",),
                    lambda: server.broadcast_message("Endstone automated event test"),
                    verify=lambda snapshots: verify_broadcast_message(snapshots, False),
                ),
        EventScenario(
                    "broadcast-message-cancel",
                    ("BroadcastMessageEvent",),
                    lambda: server.broadcast_message("Endstone automated event test"),
                    cancel_events=("BroadcastMessageEvent",),
                    verify=lambda snapshots: verify_broadcast_message(snapshots, True),
                ),
        EventScenario(
                    "script-message",
                    ("ScriptMessageEvent",),
                    lambda: command("scriptevent endstone:auto_events smoke"),
                    verify=lambda snapshots: verify_script_message(snapshots, False),
                ),
        EventScenario(
                    "script-message-cancel",
                    ("ScriptMessageEvent",),
                    lambda: command("scriptevent endstone:auto_events smoke"),
                    cancel_events=("ScriptMessageEvent",),
                    verify=lambda snapshots: verify_script_message(snapshots, True),
                ),
        EventScenario(
                    "unknown-command",
                    ("UnknownCommandEvent",),
                    unknown_command,
                    verify=verify_unknown_command,
                ),
        EventScenario(
                    "thunder-change",
                    ("ThunderChangeEvent",),
                    thunder_change,
                    verify=lambda snapshots: verify_thunder_change(snapshots, False),
                ),
        EventScenario(
                    "thunder-change-cancel",
                    ("ThunderChangeEvent",),
                    thunder_change,
                    cancel_events=("ThunderChangeEvent",),
                    verify=lambda snapshots: verify_thunder_change(snapshots, True),
                ),
        EventScenario(
                    "weather-change",
                    ("WeatherChangeEvent",),
                    weather_change,
                    verify=lambda snapshots: verify_weather_change(snapshots, False),
                ),
        EventScenario(
                    "weather-change-cancel",
                    ("WeatherChangeEvent",),
                    weather_change,
                    cancel_events=("WeatherChangeEvent",),
                    verify=lambda snapshots: verify_weather_change(snapshots, True),
                ),
        EventScenario(
                    "dimension-load",
                    ("DimensionLoadEvent",),
                    load_dimension,
                    verify=verify_dimension_load,
                ),
        EventScenario(
                    "map-initialize",
                    ("MapInitializeEvent",),
                    initialize_map,
                    verify=verify_map_initialize,
                ),
        EventScenario(
                    "plugin-lifecycle",
                    ("PluginDisableEvent", "PluginEnableEvent"),
                    toggle_event_logger_plugin,
                    verify=verify_plugin_lifecycle,
                ),
        EventScenario(
                    "chunk-load",
                    ("ChunkLoadEvent",),
                    load_chunk,
                    timeout_ticks=200,
                    verify=lambda snapshots: verify_chunk_event(
                        snapshots, "ChunkLoadEvent"
                    ),
                ),
        EventScenario(
                    "chunk-unload",
                    ("ChunkUnloadEvent",),
                    unload_chunk,
                    timeout_ticks=200,
                    verify=lambda snapshots: verify_chunk_event(
                        snapshots, "ChunkUnloadEvent"
                    ),
                ),
        EventScenario(
                    "player-connect",
                    ("PlayerLoginEvent", "PlayerJoinEvent", "PlayerSkinChangeEvent"),
                    lambda: None,
                    timeout_ticks=240,
                    driver_action="connect",
                    verify=verify_player_connect,
                ),
        EventScenario(
                    "player-login-cancel",
                    ("PlayerLoginEvent",),
                    lambda: None,
                    timeout_ticks=120,
                    driver_action="connect",
                    cancel_events=("PlayerLoginEvent",),
                    isolated=True,
                    verify=verify_player_login_cancel,
                ),
        EventScenario(
                    "player-skin-change-cancel",
                    ("PlayerSkinChangeEvent",),
                    lambda: None,
                    timeout_ticks=180,
                    driver_action="connect",
                    cancel_events=("PlayerSkinChangeEvent",),
                    isolated=True,
                    verify=lambda snapshots: verify_player_skin_change(snapshots, True),
                ),
        EventScenario(
                    "server-list-ping",
                    ("ServerListPingEvent",),
                    lambda: None,
                    timeout_ticks=120,
                    driver_action="server-list-ping",
                    requires_player=True,
                    verify=lambda snapshots: verify_server_list_ping(snapshots, False),
                ),
        EventScenario(
                    "server-list-ping-cancel",
                    ("ServerListPingEvent",),
                    lambda: None,
                    timeout_ticks=180,
                    driver_action="server-list-ping",
                    driver_data={"expect_cancelled": True},
                    requires_player=True,
                    cancel_events=("ServerListPingEvent",),
                    isolated=True,
                    verify=lambda snapshots: verify_server_list_ping(snapshots, True),
                ),
        EventScenario(
                    "packet-receive",
                    ("PacketReceiveEvent",),
                    lambda: None,
                    driver_action="input",
                    requires_player=True,
                    verify=lambda snapshots: verify_packet_event(
                        snapshots, "PacketReceiveEvent", False, 144
                    ),
                ),
        EventScenario(
                    "packet-receive-cancel",
                    ("PacketReceiveEvent",),
                    lambda: None,
                    driver_action="input",
                    requires_player=True,
                    cancel_events=("PacketReceiveEvent",),
                    isolated=True,
                    verify=lambda snapshots: verify_packet_event(
                        snapshots, "PacketReceiveEvent", True, 144
                    ),
                ),
        EventScenario(
                    "packet-send",
                    ("PacketSendEvent",),
                    lambda: None,
                    driver_action="input",
                    requires_player=True,
                    verify=lambda snapshots: verify_packet_event(
                        snapshots, "PacketSendEvent", False
                    ),
                ),
        EventScenario(
                    "packet-send-cancel",
                    ("PacketSendEvent",),
                    lambda: None,
                    driver_action="input",
                    requires_player=True,
                    cancel_events=("PacketSendEvent",),
                    isolated=True,
                    verify=lambda snapshots: verify_packet_event(
                        snapshots, "PacketSendEvent", True
                    ),
                ),
    ]
