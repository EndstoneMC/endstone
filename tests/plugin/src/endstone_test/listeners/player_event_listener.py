from pathlib import Path

from endstone import ColorFormat
from endstone.event import (
    PlayerBedEnterEvent,
    PlayerBedLeaveEvent,
    PlayerChatEvent,
    PlayerDeathEvent,
    PlayerDimensionChangeEvent,
    PlayerDropItemEvent,
    PlayerEmoteEvent,
    PlayerGameModeChangeEvent,
    PlayerInteractActorEvent,
    PlayerInteractEvent,
    PlayerItemConsumeEvent,
    PlayerItemHeldEvent,
    PlayerJoinEvent,
    PlayerJumpEvent,
    PlayerKickEvent,
    PlayerLoginEvent,
    PlayerMoveEvent,
    PlayerPickupItemEvent,
    PlayerPortalEvent,
    PlayerQuitEvent,
    PlayerRespawnEvent,
    PlayerSkinChangeEvent,
    PlayerTeleportEvent,
    event_handler,
)
from endstone.lang import Translatable
from PIL import Image

from .event_listener import EventListener


class PlayerEventListener(EventListener):
    @event_handler
    def on_player_login(self, event: PlayerLoginEvent) -> None:
        self.plugin.on_event_triggered(
            event, ColorFormat.YELLOW + f"{event.player.name} logged in."
        )
        skin = event.player.skin
        skin_dir = Path(self.plugin.data_folder) / event.player.name
        skin_dir.mkdir(parents=True, exist_ok=True)
        Image.fromarray(skin.image).save(skin_dir / f"{skin.id}.png")
        if skin.cape_image is not None:
            Image.fromarray(skin.cape_image).save(skin_dir / f"{skin.cape_id}.png")

        self.plugin.logger.warning(f"Skin ID: {skin.id}")
        self.plugin.logger.warning(f"Cape ID: {skin.cape_id}")

    @event_handler
    def on_player_join(self, event: PlayerJoinEvent) -> None:
        join_message = event.join_message
        if isinstance(join_message, Translatable):
            join_message = self.plugin.server.language.translate(join_message)

        self.plugin.on_event_triggered(event, join_message)
        # event.join_message = None

        self.plugin.logger.info("===========================")
        self.plugin.logger.info(f"Name: {event.player.name}")
        self.plugin.logger.info(f"UUID: {event.player.unique_id}")
        self.plugin.logger.info(f"XUID: {event.player.xuid}")
        self.plugin.logger.info(f"Entity Id: {event.player.runtime_id}")
        self.plugin.logger.info(f"Address: {event.player.address}")
        self.plugin.logger.info(f"Game mode: {event.player.game_mode}")
        self.plugin.logger.info(f"Location: {event.player.location}")
        self.plugin.logger.info(f"Velocity: {event.player.velocity}")
        self.plugin.logger.info(f"Op status: {event.player.is_op}")
        self.plugin.logger.info(f"Ping: {event.player.ping}ms")
        self.plugin.logger.info(f"Locale: {event.player.locale}")
        self.plugin.logger.info(
            f"Device: {event.player.device_os} {event.player.device_id}"
        )
        self.plugin.logger.info("===========================")

        self.plugin.run_tests("on_player_join", player=event.player)
        self.plugin.boss_bar.add_player(event.player)

    @event_handler
    def on_player_emote(self, event: PlayerEmoteEvent) -> None:
        self.plugin.on_event_triggered(
            event, f"{event.player.name} sends an emote: {event.emote_id}"
        )

    @event_handler
    def on_player_interact(self, event: PlayerInteractEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} interact ({event.action}) with {event.block} (face={event.block_face}) using {event.item} item",
            True,
        )

    @event_handler
    def on_player_interact_actor(self, event: PlayerInteractActorEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} interacts with actor {event.actor.name}"
        )

    @event_handler
    def on_player_kick(self, event: PlayerKickEvent) -> None:
        self.plugin.on_event_triggered(
            event, f"{event.player.name} has been kicked due to {event.reason}"
        )
        event.reason = ColorFormat.BOLD + event.reason

    @event_handler
    def on_player_quit(self, event: PlayerQuitEvent) -> None:
        quit_message = event.quit_message
        if isinstance(quit_message, Translatable):
            quit_message = self.plugin.server.language.translate(quit_message)
        self.plugin.on_event_triggered(event, quit_message)
        # event.quit_message = None

    @event_handler
    def on_player_chat(self, event: PlayerChatEvent) -> None:
        self.plugin.on_event_triggered(
            event, f"{event.player.name} says: {event.message}"
        )

    @event_handler
    def on_player_game_mode_changed(self, event: PlayerGameModeChangeEvent) -> None:
        self.plugin.on_event_triggered(
            event, f"{event.player.name} changed game mode to {event.new_game_mode}"
        )

    @event_handler
    def on_player_jump(self, event: PlayerJumpEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} {ColorFormat.YELLOW}jumps{ColorFormat.RESET} from {event.from_location} to {event.to_location}",
        )

    @event_handler
    def on_player_move(self, event: PlayerMoveEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} {ColorFormat.GREEN}moves{ColorFormat.RESET} from {event.from_location} to {event.to_location}",
        )

    @event_handler
    def on_player_teleport(self, event: PlayerTeleportEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} teleported from {event.from_location} to {event.to_location}",
        )

    @event_handler
    def on_player_death(self, event: PlayerDeathEvent):
        death_message = self.server.language.translate(event.death_message)
        self.plugin.on_event_triggered(
            event, f"{death_message} (source: {event.damage_source})."
        )

    @event_handler
    def on_player_respawn(self, event: PlayerRespawnEvent):
        self.plugin.on_event_triggered(event, f"{event.player.name} respawned.")

    @event_handler
    def on_player_item_consume(self, event: PlayerItemConsumeEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} consumes {event.item}."
        )

    @event_handler
    def on_player_item_held(self, event: PlayerItemHeldEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} changes slot from {event.previous_slot} to {event.new_slot}.",
        )

    @event_handler
    def on_player_drop_item(self, event: PlayerDropItemEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} drops {event.item}."
        )
        if event.item.type == "minecraft:apple":
            event.player.send_message("Please do not throw away the apple :)")
            event.cancel()

    @event_handler
    def on_player_pick_up_item(self, event: PlayerPickupItemEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} picks up {event.item} ({event.item.item_stack}).",
        )
        if event.item.item_stack.type == "minecraft:golden_apple":
            event.cancel()

    @event_handler
    def on_player_enter_bed(self, event: PlayerBedEnterEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} enters bed ({event.bed}).", True
        )

    @event_handler
    def on_player_leave_bed(self, event: PlayerBedLeaveEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} leaves bed ({event.bed}).", True
        )

    @event_handler
    def on_player_change_skin(self, event: PlayerSkinChangeEvent):
        self.plugin.on_event_triggered(
            event, f"{event.player.name} changes skin to {event.new_skin.id}."
        )

    @event_handler
    def on_player_portal(self, event: PlayerPortalEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} teleported from {event.from_location} to {event.to_location} {ColorFormat.GREEN}via portal",
            True,
        )

    @event_handler
    def on_player_dimension_change(self, event: PlayerDimensionChangeEvent):
        self.plugin.on_event_triggered(
            event,
            f"{event.player.name} has changed dimension from {event.from_dimension.name} to {event.to_dimension.name}.",
            True,
        )
