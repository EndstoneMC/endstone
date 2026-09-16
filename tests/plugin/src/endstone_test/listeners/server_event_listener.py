import textwrap

from endstone import ColorFormat
from endstone.event import (
    MapInitializeEvent,
    PacketReceiveEvent,
    PacketSendEvent,
    ScriptMessageEvent,
    ServerCommandEvent,
    ServerListPingEvent,
    event_handler,
)

from .event_listener import EventListener


class ServerEventListener(EventListener):
    @event_handler
    def on_server_command(self, event: ServerCommandEvent):
        self.plugin.on_event_triggered(
            event, f"{event.sender.name} executed command: {event.command}"
        )

    @event_handler
    def on_server_list_ping(self, event: ServerListPingEvent):
        event.motd = ColorFormat.BOLD + event.motd
        event.level_name = ColorFormat.GREEN + "Endstone Test" + ColorFormat.RESET
        self.plugin.on_event_triggered(event, "ServerListPingEvent is called")

    @event_handler
    def on_script_message(self, event: ScriptMessageEvent):
        self.plugin.on_event_triggered(
            event,
            f"Script message received from {event.sender}: message_id={event.message_id}, message={event.message}",
        )

    @event_handler
    def on_packet_receive(self, event: PacketReceiveEvent):
        payload = event.payload
        payload = payload[:31] if len(payload) > 31 else payload
        player_name = event.player.name if event.player else None
        self.plugin.on_event_triggered(
            event,
            f"IN ({event.packet_id}) >> {player_name} {event.address} "
            + textwrap.shorten(" ".join(f"{byte:02x}" for byte in payload), 90),
        )

    @event_handler
    def on_packet_send(self, event: PacketSendEvent):
        payload = event.payload
        payload = payload[:30] if len(payload) > 30 else payload
        player_name = event.player.name if event.player else None
        self.plugin.on_event_triggered(
            event,
            f"OUT({event.packet_id}) >> {player_name} {event.address} "
            + textwrap.shorten(" ".join(f"{byte:02x}" for byte in payload), 90),
        )

    @event_handler
    def on_map_init(self, event: MapInitializeEvent):
        self.plugin.on_event_triggered(
            event,
            f"MapInitializeEvent is called. MapView(id={event.map.id}, scale={event.map.scale}, center_x={event.map.center_x}, center_z={event.map.center_z}).",
            always_log=True,
        )
