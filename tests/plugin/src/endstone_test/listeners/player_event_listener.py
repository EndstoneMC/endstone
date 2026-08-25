from endstone import ColorFormat
from endstone.event import (
    PlayerArmorStandManipulateEvent,
    PlayerArmSwingEvent,
    PlayerBedEnterEvent,
    PlayerBedLeaveEvent,
    PlayerBucketActorEvent,
    PlayerChatEvent,
    PlayerCommandEvent,
    PlayerCraftItemEvent,
    PlayerDeathEvent,
    PlayerDimensionChangeEvent,
    PlayerDropItemEvent,
    PlayerEditBookEvent,
    PlayerEmoteEvent,
    PlayerExpChangeEvent,
    PlayerGameModeChangeEvent,
    PlayerInputEvent,
    PlayerInteractActorEvent,
    PlayerInteractEvent,
    PlayerItemConsumeEvent,
    PlayerItemHeldEvent,
    PlayerJoinEvent,
    PlayerJumpEvent,
    PlayerKickEvent,
    PlayerLevelChangeEvent,
    PlayerLoginEvent,
    PlayerMoveEvent,
    PlayerPickupArrowEvent,
    PlayerPickupExperienceEvent,
    PlayerPickupItemEvent,
    PlayerPortalEvent,
    PlayerQuitEvent,
    PlayerRecipeBookSettingsChangeEvent,
    PlayerRespawnEvent,
    PlayerRiptideEvent,
    PlayerSetSpawnEvent,
    PlayerShearActorEvent,
    PlayerSkinChangeEvent,
    PlayerTeleportEvent,
    PlayerToggleCrawlEvent,
    PlayerToggleFlightEvent,
    PlayerToggleSneakEvent,
    PlayerToggleSprintEvent,
    event_handler,
)
from endstone.lang import Translatable

from .event_listener import EventListener


def _xyz(location) -> tuple[float, float, float]:
    return (location.x, location.y, location.z)


def _rot(location) -> tuple[float, float]:
    return (location.pitch, location.yaw)


def _as_text(server, message) -> str:
    if isinstance(message, Translatable):
        return server.language.translate(message)
    return "" if message is None else str(message)


class PlayerEventListener(EventListener):
    @event_handler
    def on_player_login(self, event: PlayerLoginEvent):
        skin = event.player.skin
        self.record(
            event,
            ColorFormat.YELLOW + f"{event.player.name} logged in.",
            player=event.player.name,
            skin_id=skin.id,
            cape_id=skin.cape_id,
            skin_shape=tuple(skin.image.shape),
            has_cape=skin.cape_image is not None,
        )

    @event_handler
    def on_player_join(self, event: PlayerJoinEvent):
        player = event.player
        self.record(
            event,
            _as_text(self.server, event.join_message),
            player=player.name,
            unique_id=str(player.unique_id),
            xuid=player.xuid,
            runtime_id=player.runtime_id,
            game_mode=str(player.game_mode),
            locale=player.locale,
            device_os=str(player.device_os),
            ping=player.ping,
            is_op=player.is_op,
        )

        for line in (
            "===========================",
            f"Name: {player.name}",
            f"UUID: {player.unique_id}",
            f"XUID: {player.xuid}",
            f"Entity Id: {player.runtime_id}",
            f"Address: {player.address}",
            f"Game mode: {player.game_mode}",
            f"Location: {player.location}",
            f"Velocity: {player.velocity}",
            f"Op status: {player.is_op}",
            f"Ping: {player.ping}ms",
            f"Locale: {player.locale}",
            f"Device: {player.device_os} {player.device_id}",
            f"Game version: {player.game_version}",
            "===========================",
        ):
            self.plugin.logger.info(line)

        if self.recorder.boss_bar is not None:
            self.recorder.boss_bar.add_player(player)

    @event_handler
    def on_player_quit(self, event: PlayerQuitEvent):
        self.record(
            event,
            _as_text(self.server, event.quit_message),
            player=event.player.name,
        )

    @event_handler
    def on_player_chat(self, event: PlayerChatEvent):
        self.record(
            event,
            f"{event.player.name} says: {event.message}",
            player=event.player.name,
            message=event.message,
            recipient_count=len(event.recipients),
        )

    @event_handler
    def on_player_command(self, event: PlayerCommandEvent):
        self.record(
            event,
            f"{event.player.name} runs command: {event.command}",
            player=event.player.name,
            command=event.command,
        )

    @event_handler
    def on_player_emote(self, event: PlayerEmoteEvent):
        self.record(
            event,
            f"{event.player.name} sends an emote: {event.emote_id}",
            player=event.player.name,
            emote_id=event.emote_id,
            is_muted=event.is_muted,
        )

    @event_handler
    def on_player_interact(self, event: PlayerInteractEvent):
        self.record(
            event,
            f"{event.player.name} interact ({event.action}) with {event.block} "
            f"(face={event.block_face}) using {event.item} item",
            always_log=True,
            player=event.player.name,
            action=str(event.action),
            has_block=event.has_block,
            has_item=event.has_item,
            block_type=str(event.block.type) if event.has_block else None,
            block_face=str(event.block_face),
        )

    @event_handler
    def on_player_interact_actor(self, event: PlayerInteractActorEvent):
        self.record(
            event,
            f"{event.player.name} interacts with actor {event.actor.name}",
            player=event.player.name,
            actor_type=str(event.actor.type),
        )

    @event_handler
    def on_player_kick(self, event: PlayerKickEvent):
        self.record(
            event,
            f"{event.player.name} has been kicked due to {event.reason}",
            player=event.player.name,
            reason=event.reason,
        )
        event.reason = ColorFormat.BOLD + event.reason

    @event_handler
    def on_player_game_mode_change(self, event: PlayerGameModeChangeEvent):
        self.record(
            event,
            f"{event.player.name} changed game mode to {event.new_game_mode}",
            player=event.player.name,
            new_game_mode=str(event.new_game_mode),
        )

    @event_handler
    def on_player_jump(self, event: PlayerJumpEvent):
        self.record(
            event,
            f"{event.player.name} {ColorFormat.YELLOW}jumps{ColorFormat.RESET} from "
            f"{event.from_location} to {event.to_location}",
            player=event.player.name,
            from_xyz=_xyz(event.from_location),
            to_xyz=_xyz(event.to_location),
            from_rot=_rot(event.from_location),
            to_rot=_rot(event.to_location),
        )

    @event_handler
    def on_player_move(self, event: PlayerMoveEvent):
        self.record(
            event,
            f"{event.player.name} {ColorFormat.GREEN}moves{ColorFormat.RESET} from "
            f"{event.from_location} to {event.to_location}",
            player=event.player.name,
            from_xyz=_xyz(event.from_location),
            to_xyz=_xyz(event.to_location),
            from_rot=_rot(event.from_location),
            to_rot=_rot(event.to_location),
        )

    @event_handler
    def on_player_teleport(self, event: PlayerTeleportEvent):
        self.record(
            event,
            f"{event.player.name} teleported from {event.from_location} to "
            f"{event.to_location}",
            player=event.player.name,
            from_xyz=_xyz(event.from_location),
            to_xyz=_xyz(event.to_location),
            from_rot=_rot(event.from_location),
            to_rot=_rot(event.to_location),
        )

    @event_handler
    def on_player_portal(self, event: PlayerPortalEvent):
        self.record(
            event,
            f"{event.player.name} teleported from {event.from_location} to "
            f"{event.to_location} {ColorFormat.GREEN}via portal",
            always_log=True,
            player=event.player.name,
            from_xyz=_xyz(event.from_location),
            to_xyz=_xyz(event.to_location),
            from_rot=_rot(event.from_location),
            to_rot=_rot(event.to_location),
        )

    @event_handler
    def on_player_dimension_change(self, event: PlayerDimensionChangeEvent):
        self.record(
            event,
            f"{event.player.name} has changed dimension from "
            f"{event.from_dimension.id} to {event.to_dimension.id}.",
            always_log=True,
            player=event.player.name,
            from_dimension=str(event.from_dimension.id),
            to_dimension=str(event.to_dimension.id),
        )

    @event_handler
    def on_player_death(self, event: PlayerDeathEvent):
        self.record(
            event,
            f"{_as_text(self.server, event.death_message)} "
            f"(source: {event.damage_source}).",
            player=event.player.name,
            damage_type=event.damage_source.type,
        )

    @event_handler
    def on_player_respawn(self, event: PlayerRespawnEvent):
        self.record(
            event,
            f"{event.player.name} respawned ({event.respawn_reason}).",
            player=event.player.name,
            respawn_reason=event.respawn_reason.name,
        )

    @event_handler
    def on_player_item_consume(self, event: PlayerItemConsumeEvent):
        self.record(
            event,
            f"{event.player.name} consumes {event.item}.",
            player=event.player.name,
            item_type=str(event.item.type),
            hand=str(event.hand),
        )

    @event_handler
    def on_player_item_held(self, event: PlayerItemHeldEvent):
        self.record(
            event,
            f"{event.player.name} changes slot from {event.previous_slot} to "
            f"{event.new_slot}.",
            player=event.player.name,
            previous_slot=event.previous_slot,
            new_slot=event.new_slot,
        )

    @event_handler
    def on_player_drop_item(self, event: PlayerDropItemEvent):
        self.record(
            event,
            f"{event.player.name} drops {event.item}.",
            player=event.player.name,
            item_type=str(event.item.type),
        )

    @event_handler
    def on_player_pickup_item(self, event: PlayerPickupItemEvent):
        self.record(
            event,
            f"{event.player.name} picks up {event.item} ({event.item.item_stack}).",
            player=event.player.name,
            item_type=str(event.item.item_stack.type),
        )

    @event_handler
    def on_player_bed_enter(self, event: PlayerBedEnterEvent):
        self.record(
            event,
            f"{event.player.name} enters bed ({event.bed}).",
            always_log=True,
            player=event.player.name,
            bed_type=str(event.bed.type),
        )

    @event_handler
    def on_player_bed_leave(self, event: PlayerBedLeaveEvent):
        self.record(
            event,
            f"{event.player.name} leaves bed ({event.bed}).",
            always_log=True,
            player=event.player.name,
            bed_type=str(event.bed.type),
        )

    @event_handler
    def on_player_skin_change(self, event: PlayerSkinChangeEvent):
        self.record(
            event,
            f"{event.player.name} changes skin to {event.new_skin.id}.",
            player=event.player.name,
            new_skin_id=event.new_skin.id,
        )

    @event_handler
    def on_player_toggle_sneak(self, event: PlayerToggleSneakEvent):
        self.record(
            event,
            f"{event.player.name} sneaking -> {event.is_sneaking}",
            player=event.player.name,
            is_sneaking=event.is_sneaking,
            player_is_sneaking=event.player.is_sneaking,
        )

    @event_handler
    def on_player_toggle_sprint(self, event: PlayerToggleSprintEvent):
        self.record(
            event,
            f"{event.player.name} sprinting -> {event.is_sprinting}",
            player=event.player.name,
            is_sprinting=event.is_sprinting,
            player_is_sprinting=event.player.is_sprinting,
        )

    @event_handler
    def on_player_arm_swing(self, event: PlayerArmSwingEvent):
        self.record(
            event,
            f"{event.player.name} swings their arm holding {event.item}",
            player=event.player.name,
            has_item=event.item is not None,
            item_type=str(event.item.type) if event.item is not None else None,
        )

    @event_handler
    def on_player_armor_stand_manipulate(self, event: PlayerArmorStandManipulateEvent):
        self.record(
            event,
            f"{event.player.name} swaps {event.player_item} for "
            f"{event.armor_stand_item} in the {event.slot} slot of "
            f"{event.actor.name}",
            always_log=True,
            player=event.player.name,
            actor_type=str(event.actor.type),
            slot=str(event.slot),
            armor_stand_item_type=str(event.armor_stand_item.type),
            player_item_type=str(event.player_item.type),
        )

    @event_handler
    def on_player_bucket_actor(self, event: PlayerBucketActorEvent):
        self.record(
            event,
            f"{event.player.name} captures {event.actor.name} "
            f"({event.actor.type}) with {event.original_bucket}",
            always_log=True,
            player=event.player.name,
            actor_type=str(event.actor.type),
            original_bucket_type=str(event.original_bucket.type),
            hand=str(event.hand),
        )

    @event_handler
    def on_player_exp_change(self, event: PlayerExpChangeEvent):
        self.record(
            event,
            f"{event.player.name} gains {event.amount} experience",
            player=event.player.name,
            amount=event.amount,
        )

    @event_handler
    def on_player_input(self, event: PlayerInputEvent):
        player_input = event.input
        self.record(
            event,
            f"{event.player.name} input: {player_input}",
            player=event.player.name,
            forward=player_input.is_forward,
            backward=player_input.is_backward,
            left=player_input.is_left,
            right=player_input.is_right,
            jump=player_input.is_jump,
            sneak=player_input.is_sneak,
            sprint=player_input.is_sprint,
        )

    @event_handler
    def on_player_level_change(self, event: PlayerLevelChangeEvent):
        self.record(
            event,
            f"{event.player.name} level {event.old_level} -> {event.new_level}",
            always_log=True,
            player=event.player.name,
            old_level=event.old_level,
            new_level=event.new_level,
        )

    @event_handler
    def on_player_pickup_arrow(self, event: PlayerPickupArrowEvent):
        self.record(
            event,
            f"{event.player.name} picks up an arrow ({event.arrow.type})",
            player=event.player.name,
            arrow_type=str(event.arrow.type),
        )

    @event_handler
    def on_player_pickup_experience(self, event: PlayerPickupExperienceEvent):
        self.record(
            event,
            f"{event.player.name} picks up an orb worth {event.amount} experience",
            player=event.player.name,
            amount=event.amount,
        )

    @event_handler
    def on_player_recipe_book_settings_change(
        self, event: PlayerRecipeBookSettingsChangeEvent
    ):
        self.record(
            event,
            f"{event.player.name} recipe book {event.recipe_book_type} "
            f"(open={event.is_open}, filtering={event.is_filtering})",
            always_log=True,
            player=event.player.name,
            recipe_book_type=event.recipe_book_type.name,
            is_open=event.is_open,
            is_filtering=event.is_filtering,
        )

    @event_handler
    def on_player_riptide(self, event: PlayerRiptideEvent):
        self.record(
            event,
            f"{event.player.name} riptides with {event.item}",
            always_log=True,
            player=event.player.name,
            item_type=str(event.item.type),
        )

    @event_handler
    def on_player_shear_actor(self, event: PlayerShearActorEvent):
        self.record(
            event,
            f"{event.player.name} shears {event.actor.name} ({event.actor.type}) "
            f"with {event.item}",
            always_log=True,
            player=event.player.name,
            actor_type=str(event.actor.type),
            item_type=str(event.item.type),
            hand=str(event.hand),
        )

    @event_handler
    def on_player_toggle_crawl(self, event: PlayerToggleCrawlEvent):
        self.record(
            event,
            f"{event.player.name} crawling -> {event.is_crawling}",
            player=event.player.name,
            is_crawling=event.is_crawling,
            player_is_crawling=event.player.is_crawling,
        )

    @event_handler
    def on_player_toggle_flight(self, event: PlayerToggleFlightEvent):
        self.record(
            event,
            f"{event.player.name} flying -> {event.is_flying}",
            player=event.player.name,
            is_flying=event.is_flying,
            player_is_flying=event.player.is_flying,
            allow_flight=event.player.allow_flight,
        )

    @event_handler
    def on_player_craft_item(self, event: PlayerCraftItemEvent):
        self.record(
            event,
            f"{event.player.name} crafts {event.recipe.id} x{event.repetitions}",
            always_log=True,
            player=event.player.name,
            recipe_id=str(event.recipe.id),
            repetitions=event.repetitions,
            ingredients=[str(i.type) for i in event.ingredients],
            results=[str(r.type) for r in event.results],
        )

    @event_handler
    def on_player_edit_book(self, event: PlayerEditBookEvent):
        self.record(
            event,
            f"{event.player.name} edits a book in slot {event.slot} (signing: {event.is_signing})",
            always_log=True,
            player=event.player.name,
            slot=event.slot,
            is_signing=event.is_signing,
            previous_pages=len(event.previous_book_meta.pages),
            new_pages=len(event.new_book_meta.pages),
        )

    @event_handler
    def on_player_set_spawn(self, event: PlayerSetSpawnEvent):
        self.record(
            event,
            f"{event.player.name} sets spawn to {event.location} ({event.cause})",
            always_log=True,
            player=event.player.name,
            cause=str(event.cause),
            xyz=(event.location.x, event.location.y, event.location.z),
            dimension=str(event.location.dimension.id),
        )
