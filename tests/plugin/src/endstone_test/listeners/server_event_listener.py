import textwrap

from endstone import ColorFormat
from endstone.event import (
    BroadcastMessageEvent,
    MapInitializeEvent,
    PacketReceiveEvent,
    PacketSendEvent,
    PluginDisableEvent,
    PluginEnableEvent,
    ScriptMessageEvent,
    ServerCommandEvent,
    ServerListPingEvent,
    ServerLoadEvent,
    event_handler,
)

from .event_listener import EventListener


def _hexdump(payload: bytes, limit: int = 30) -> str:
    return textwrap.shorten(" ".join(f"{byte:02x}" for byte in payload[:limit]), 90)


class ServerEventListener(EventListener):
    @event_handler
    def on_server_load(self, event: ServerLoadEvent):
        self.record(
            event,
            f"ServerLoadEvent is called ({event.type})",
            always_log=True,
            load_type=event.type.name,
        )

    @event_handler
    def on_server_command(self, event: ServerCommandEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("ServerCommandEvent"):
            event.cancel()
        self.record(
            event,
            f"{event.sender.name} executed command: {event.command}",
            sender=event.sender.name,
            command=event.command,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_server_list_ping(self, event: ServerListPingEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("ServerListPingEvent"):
            event.cancel()
        event.motd = ColorFormat.BOLD + event.motd
        event.level_name = ColorFormat.GREEN + "Endstone Test" + ColorFormat.RESET
        self.record(
            event,
            "ServerListPingEvent is called",
            motd=event.motd,
            level_name=event.level_name,
            num_players=event.num_players,
            max_players=event.max_players,
            network_protocol_version=event.network_protocol_version,
            minecraft_version_network=event.minecraft_version_network,
            server_guid=event.server_guid,
            local_port=event.local_port,
            game_mode=str(event.game_mode),
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_broadcast_message(self, event: BroadcastMessageEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("BroadcastMessageEvent"):
            event.cancel()
        self.record(
            event,
            f"Broadcast: {event.message}",
            message=str(event.message),
            recipient_count=len(event.recipients),
            is_asynchronous=event.is_asynchronous,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_plugin_enable(self, event: PluginEnableEvent):
        self.record(
            event,
            f"{event.plugin.name} is enabled",
            plugin=event.plugin.name,
        )

    @event_handler
    def on_plugin_disable(self, event: PluginDisableEvent):
        self.record(
            event,
            f"{event.plugin.name} is disabled",
            plugin=event.plugin.name,
        )

    @event_handler
    def on_script_message(self, event: ScriptMessageEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("ScriptMessageEvent"):
            event.cancel()
        self.record(
            event,
            f"Script message received from {event.sender.name}: "
            f"message_id={event.message_id}, message={event.message}",
            sender=event.sender.name,
            message_id=event.message_id,
            message=event.message,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )
        self.plugin.handle_script_message(event.message_id, event.message)

    @event_handler
    def on_packet_receive(self, event: PacketReceiveEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("PacketReceiveEvent"):
            event.cancel()
        payload = (
            event.payload.hex(" ")
            if event.packet_id in {30, 36, 147}
            else _hexdump(event.payload, 31)
        )
        self.record(
            event,
            f"IN ({event.packet_id}) >> "
            f"{event.player.name if event.player else None} {event.address} "
            f"{payload}",
            always_log=event.packet_id in {30, 36, 49, 50, 147},
            packet_id=event.packet_id,
            payload_size=len(event.payload),
            player=event.player.name if event.player is not None else None,
            address=str(event.address),
            sub_client_id=event.sub_client_id,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_packet_send(self, event: PacketSendEvent):
        cancelled_before = event.is_cancelled
        if self.plugin.should_cancel_event("PacketSendEvent"):
            event.cancel()
        self.record(
            event,
            f"OUT({event.packet_id}) >> "
            f"{event.player.name if event.player else None} {event.address} "
            f"{_hexdump(event.payload)}",
            packet_id=event.packet_id,
            payload_size=len(event.payload),
            player=event.player.name if event.player is not None else None,
            address=str(event.address),
            sub_client_id=event.sub_client_id,
            cancelled_before=cancelled_before,
            cancelled=event.is_cancelled,
        )

    @event_handler
    def on_map_initialize(self, event: MapInitializeEvent):
        self.record(
            event,
            f"MapInitializeEvent is called. MapView(id={event.map.id}, "
            f"scale={event.map.scale}, center_x={event.map.center_x}, "
            f"center_z={event.map.center_z}).",
            always_log=True,
            map_id=event.map.id,
            scale=str(event.map.scale),
            center_x=event.map.center_x,
            center_z=event.map.center_z,
            dimension=(
                str(event.map.dimension.id)
                if event.map.dimension is not None
                else None
            ),
        )
