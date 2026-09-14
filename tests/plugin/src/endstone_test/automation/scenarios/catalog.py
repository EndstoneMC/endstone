from __future__ import annotations

from math import floor

from endstone import GameMode
from endstone.block import Sign
from endstone.command import CommandSenderWrapper
from endstone.inventory import ItemStack
from endstone.level import Dimension, DimensionCreator, Location
from endstone.potion import Effect

from ..manifest import EXCLUDED_AUTOMATED_SCENARIOS
from ..models import EventScenario
from . import build_scenario_groups


def build_scenarios(ctx) -> list[EventScenario]:
    server = ctx.server
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    if overworld is None:
        raise RuntimeError("The overworld is not available")

    dispatch_command = ctx.dispatch_command
    command = ctx.command
    set_block_type = ctx.set_block_type
    set_region_type = ctx.set_region_type
    remove_actors = ctx.remove_actors
    clear_player_inventory = ctx.clear_player_inventory
    give_player_item = ctx.give_player_item
    run_gametest = ctx.run_gametest
    auto_tag = ctx.auto_tag
    player_for_action = ctx.player_for_action
    block_target = ctx.block_target
    
    quiet_command_sender = CommandSenderWrapper(server.command_sender)
    
    
    
    
    
    
    
    
    
    def unknown_command() -> None:
        dispatch_command("endstone_auto_events_unknown")
    
    def weather_change() -> None:
        cancelled = ctx.plugin.should_cancel_event("WeatherChangeEvent")
        ctx.plugin.set_automated_cancel_events(())
        command("weather clear")
        if cancelled:
            ctx.plugin.set_automated_cancel_events(("WeatherChangeEvent",))
        command("weather rain")
    
    def thunder_change() -> None:
        cancelled = ctx.plugin.should_cancel_event("ThunderChangeEvent")
        ctx.plugin.set_automated_cancel_events(())
        command("weather clear")
        if cancelled:
            ctx.plugin.set_automated_cancel_events(("ThunderChangeEvent",))
        command("weather thunder")
    
    chunk_x, chunk_z = 30_000, 30_000
    dimension_load_id = f"endstone_auto:dimension_load_{ctx.run_number}"
    map_expectation: dict[str, object] = {}
    
    def load_dimension() -> None:
        dimension = server.level.create_dimension(DimensionCreator(dimension_load_id))
        if dimension is None:
            raise RuntimeError("could not create the smoke-test dimension")
    
    def verify_dimension_load(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["DimensionLoadEvent"]
        assert event_snapshots, "DimensionLoadEvent did not produce a snapshot"
        assert any(
            snapshot["dimension"] == dimension_load_id
            for snapshot in event_snapshots
        )
    def initialize_map() -> None:
        view = server.create_map(overworld)
        map_expectation.update(
            {
                "id": view.id,
                "scale": str(view.scale),
                "center_x": view.center_x,
                "center_z": view.center_z,
                "dimension": (
                    str(view.dimension.id) if view.dimension is not None else None
                ),
            }
        )
    def verify_map_initialize(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["MapInitializeEvent"]
        assert event_snapshots, "MapInitializeEvent did not produce a snapshot"
        assert map_expectation
        matching_snapshots = [
            snapshot
            for snapshot in event_snapshots
            if snapshot["map_id"] == map_expectation["id"]
        ]
        assert matching_snapshots, (
            "created map was not observed in MapInitializeEvent: "
            f"expected={map_expectation['id']!r}, snapshots={event_snapshots!r}"
        )
        for snapshot in matching_snapshots:
            assert snapshot["map_id"] == map_expectation["id"]
            assert snapshot["scale"] == map_expectation["scale"]
            assert snapshot["center_x"] == map_expectation["center_x"]
            assert snapshot["center_z"] == map_expectation["center_z"]
            assert snapshot["dimension"] == map_expectation["dimension"]
    
    def toggle_event_logger_plugin() -> None:
        plugin = server.plugin_manager.get_plugin("plugin_lifecycle_fixture")
        if plugin is None:
            raise RuntimeError("plugin_lifecycle_fixture plugin is not loaded")
        server.plugin_manager.disable_plugin(plugin)
        server.plugin_manager.enable_plugin(plugin)
    
    def restore_item_held_logger() -> None:
        if not item_held_logger_state["disabled"]:
            return
        plugin = server.plugin_manager.get_plugin("toggle_event_logger")
        if plugin is not None:
            server.plugin_manager.enable_plugin(plugin)
        item_held_logger_state["disabled"] = False
    
    def verify_plugin_lifecycle(snapshots: dict[str, list[dict]]) -> None:
        enabled = snapshots["PluginEnableEvent"]
        disabled = snapshots["PluginDisableEvent"]
        assert len(enabled) == 1
        assert len(disabled) == 1
        assert enabled[0]["plugin"] == "plugin_lifecycle_fixture"
        assert disabled[0]["plugin"] == "plugin_lifecycle_fixture"
    
    def load_chunk() -> None:
        if not overworld.load_chunk(chunk_x, chunk_z):
            raise RuntimeError("load_chunk rejected the smoke-test chunk")
    
    def unload_chunk() -> None:
        if not overworld.is_chunk_loaded(chunk_x, chunk_z) and not overworld.load_chunk(
            chunk_x, chunk_z
        ):
            raise RuntimeError("the smoke-test chunk could not be loaded")
        def release_chunk() -> None:
            # The ticket is released after the asynchronous load has had
            # time to complete; the unload event itself may still be later.
            overworld.unload_chunk_request(chunk_x, chunk_z)
    
        if server.scheduler.run_task(ctx.plugin, release_chunk, delay=2) is None:
            raise RuntimeError("could not schedule the smoke-test chunk release")
    
    def verify_chunk_event(
        snapshots: dict[str, list[dict]], event_name: str
    ) -> None:
        event_snapshots = snapshots[event_name]
        assert event_snapshots, f"{event_name} did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["dimension"] == "minecraft:overworld"
            chunk_xz = snapshot["chunk_xz"]
            assert len(chunk_xz) == 2
            assert all(isinstance(coordinate, int) for coordinate in chunk_xz)
    
    def verify_player_connect(snapshots: dict[str, list[dict]]) -> None:
        login_snapshots = snapshots["PlayerLoginEvent"]
        join_snapshots = snapshots["PlayerJoinEvent"]
        skin_snapshots = snapshots["PlayerSkinChangeEvent"]
        assert len(login_snapshots) == 1
        assert len(join_snapshots) == 1
        assert len(skin_snapshots) == 1
    
        login = login_snapshots[0]
        assert login["player"] == ctx.bot_name
        assert isinstance(login["kick_message"], str)
        assert login["cancelled_before"] is False
        assert login["cancelled"] is False
    
        join = join_snapshots[0]
        assert join["player"] == ctx.bot_name
        assert ctx.bot_name in join["join_message"]
    
        skin = skin_snapshots[0]
        assert skin["player"] == ctx.bot_name
        assert skin["new_skin_id"]
        assert isinstance(skin["skin_change_message"], str)
        assert skin["cancelled_before"] is False
        assert skin["cancelled"] is False
    
    def verify_server_list_ping(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ServerListPingEvent"]
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["motd"]
        assert "Endstone Test" in snapshot["level_name"]
        assert snapshot["network_protocol_version"] == ctx.server.protocol_version
        assert snapshot["minecraft_version_network"]
        assert snapshot["server_guid"]
        assert snapshot["local_port"] == 19132
        assert snapshot["max_players"] >= snapshot["num_players"] >= 0
        assert snapshot["game_mode"].rsplit(".", 1)[-1] == "SURVIVAL"
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_packet_event(
        snapshots: dict[str, list[dict]],
        event_name: str,
        expected_cancelled: bool,
        expected_packet_id: int | None = None,
    ) -> None:
        event_snapshots = snapshots[event_name]
        assert event_snapshots, f"{event_name} did not produce a snapshot"
        matching = [
            snapshot
            for snapshot in event_snapshots
            if snapshot["player"] == ctx.bot_name
            and (expected_packet_id is None or snapshot["packet_id"] == expected_packet_id)
        ]
        assert matching, f"{event_name} has no matching bot packet: {event_snapshots!r}"
        for snapshot in matching:
            assert snapshot["packet_id"] >= 0
            assert snapshot["payload_size"] > 0
            assert snapshot["address"]
            assert snapshot["sub_client_id"] == 0
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_login_cancel(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerLoginEvent"]
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert isinstance(snapshot["kick_message"], str)
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is True
    
    def verify_player_skin_change(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerSkinChangeEvent"]
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert snapshot["new_skin_id"]
        assert isinstance(snapshot["skin_change_message"], str)
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_kick_cancel(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerKickEvent"]
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert snapshot["reason"] == "automated Endstone event test"
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is True
    
    def verify_player_kick_quit(
        snapshots: dict[str, list[dict]]
    ) -> None:
        kick_snapshots = snapshots["PlayerKickEvent"]
        quit_snapshots = snapshots["PlayerQuitEvent"]
        assert len(kick_snapshots) == 1
        assert len(quit_snapshots) == 1
        kick = kick_snapshots[0]
        assert kick["player"] == ctx.bot_name
        assert kick["reason"] == "automated Endstone event test"
        assert kick["cancelled_before"] is False
        assert kick["cancelled"] is False
        quit_event = quit_snapshots[0]
        assert quit_event["player"] == ctx.bot_name
        assert ctx.bot_name in quit_event["quit_message"]
    
    actor_ref: dict[str, object] = {}
    mob_refs: dict[str, object] = {}
    block_grow_random_tick_speed: dict[str, int] = {}
    actor_pickup_fixture: dict[str, object] = {}
    leaves_random_tick_speed: dict[str, int] = {}
    block_from_to_target: dict[str, tuple[int, int, int]] = {}
    item_held_logger_state = {"disabled": False}
    piston_fixture_index = {"value": -1}
    dismount_fixture_index = {"value": -1}
    dismount_fixture: dict[str, str] = {}
    piston_fixture: dict[str, int] = {}
    tag_prefix = ctx.tag_prefix
    
    
    def actor_spawn_location() -> Location:
        player = server.get_player(ctx.bot_name)
        if player is not None:
            location = player.location
            return Location(
                location.dimension,
                location.x,
                location.y,
                location.z,
                location.pitch,
                location.yaw,
            )
    
        # Keep the actor scenario independently diagnosable if a player
        # driver fails.  The origin chunk must be explicitly held before
        # querying terrain or creating the actor.
        overworld.load_chunk(0, 0)
        surface = overworld.get_highest_block_at(0, 0)
        return Location(
            overworld, surface.x + 0.5, surface.y + 1.0, surface.z + 0.5
        )
    def spawn_actor() -> None:
        # The API spawn path does not pass through LevelAddedActorEvent on
        # this build. Reuse the existing command-based summon action so the
        # native actor-added event path is exercised.
        dispatch_command(
            f"execute at {ctx.bot_name} run summon minecraft:chicken ~ ~1 ~"
        )
    def actor_for_action():
        actor = actor_ref.get("actor")
        if actor is None or not actor.is_valid:
            actor = overworld.spawn_actor(actor_spawn_location(), "minecraft:chicken")
            if actor is None:
                raise RuntimeError("could not create the smoke-test actor")
            actor_ref["actor"] = actor
        return actor
    
    def visibility_actor():
        actor = overworld.spawn_actor(actor_spawn_location(), "minecraft:chicken")
        if actor is None:
            raise RuntimeError("could not create the visibility-test actor")
        return actor
    
    def hide_player_actor() -> None:
        player = player_for_action()
        player.hide_actor(ctx.plugin, visibility_actor())
    
    def show_player_actor() -> None:
        player = player_for_action()
        actor = visibility_actor()
        player.hide_actor(ctx.plugin, actor)
        player.show_actor(ctx.plugin, actor)
    
    def verify_visibility_event(
        snapshots: dict[str, list[dict]], event_name: str
    ) -> None:
        event_snapshots = snapshots[event_name]
        assert event_snapshots, f"{event_name} did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["actor_type"] == "minecraft:chicken"
    
    def teleport_actor() -> None:
        actor = actor_for_action()
        location = actor.location
        moved_location = Location(
            location.dimension,
            location.x + 2.0,
            location.y,
            location.z,
            location.pitch,
            location.yaw,
        )
        if not actor.teleport(moved_location) and not ctx.plugin.should_cancel_event(
            "ActorTeleportEvent"
        ):
            raise RuntimeError("actor.teleport returned False")
    
    def verify_actor_spawn(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorSpawnEvent"]
        assert event_snapshots, "ActorSpawnEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:chicken"
            assert snapshot["python_type"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_actor_teleport(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorTeleportEvent"]
        assert event_snapshots, "ActorTeleportEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:chicken"
            from_x, from_y, from_z = snapshot["from_xyz"]
            to_x, to_y, to_z = snapshot["to_xyz"]
            assert to_x - from_x == 2.0
            assert (to_y, to_z) == (from_y, from_z)
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def remove_actor() -> None:
        actor_for_action().remove()
    
    def verify_actor_remove(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["ActorRemoveEvent"]
        assert event_snapshots, "ActorRemoveEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:chicken"
    
    def verify_player_death_respawn(
        snapshots: dict[str, list[dict]]
    ) -> None:
        death_snapshots = snapshots["PlayerDeathEvent"]
        respawn_snapshots = snapshots["PlayerRespawnEvent"]
        assert death_snapshots, "PlayerDeathEvent did not produce a snapshot"
        assert respawn_snapshots, "PlayerRespawnEvent did not produce a snapshot"
        for snapshot in death_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert isinstance(snapshot["damage_type"], str)
            assert snapshot["damage_type"]
        for snapshot in respawn_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["respawn_reason"] == "DEATH"
    
    def verify_player_dimension_change(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerDimensionChangeEvent"]
        assert event_snapshots, "PlayerDimensionChangeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["from_dimension"] == "minecraft:overworld"
            assert snapshot["to_dimension"] == "minecraft:nether"
            assert snapshot["from_dimension"] != snapshot["to_dimension"]
    
    def verify_player_exp_change(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerExpChangeEvent"]
        assert event_snapshots, "PlayerExpChangeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["amount"] == 5
    
    def verify_player_game_mode_change(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerGameModeChangeEvent"]
        assert event_snapshots, "PlayerGameModeChangeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["new_game_mode"] in {
                "GameMode.CREATIVE",
                "GameMode.SURVIVAL",
            }
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_level_change(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerLevelChangeEvent"]
        assert event_snapshots, "PlayerLevelChangeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["new_level"] == snapshot["old_level"] + 1
    
    def verify_player_pickup_experience(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerPickupExperienceEvent"]
        assert event_snapshots, "PlayerPickupExperienceEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert isinstance(snapshot["amount"], int)
            assert snapshot["amount"] > 0
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_pickup_item(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerPickupItemEvent"]
        assert event_snapshots, "PlayerPickupItemEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["item_type"] == "minecraft:diamond"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_set_spawn(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerSetSpawnEvent"]
        assert event_snapshots, "PlayerSetSpawnEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["cause"].endswith("PLUGIN")
            assert snapshot["dimension"] == "minecraft:overworld"
            assert snapshot["xyz"] is not None
            assert len(snapshot["xyz"]) == 3
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_teleport(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerTeleportEvent"]
        assert event_snapshots, "PlayerTeleportEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            from_x, from_y, from_z = snapshot["from_xyz"]
            to_x, to_y, to_z = snapshot["to_xyz"]
            assert to_x - from_x == 8.0
            assert (to_y, to_z) == (from_y, from_z)
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_pickup_arrow(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerPickupArrowEvent"]
        assert event_snapshots, "PlayerPickupArrowEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["arrow_type"] == "minecraft:arrow"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_riptide(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["PlayerRiptideEvent"]
        assert event_snapshots, "PlayerRiptideEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["item_type"] == "minecraft:trident"
    
    def verify_player_recipe_book(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["PlayerRecipeBookSettingsChangeEvent"]
        assert event_snapshots, (
            "PlayerRecipeBookSettingsChangeEvent did not produce a snapshot"
        )
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["recipe_book_type"] == "CRAFTING"
            assert isinstance(snapshot["is_open"], bool)
            assert isinstance(snapshot["is_filtering"], bool)
    
    def verify_player_craft_item(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerCraftItemEvent"]
        assert event_snapshots, "PlayerCraftItemEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["recipe_id"]
            assert snapshot["repetitions"] == 1
            assert snapshot["ingredients"] == ["minecraft:oak_log"]
            assert snapshot["results"] == ["minecraft:oak_planks"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_edit_book(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerEditBookEvent"]
        assert event_snapshots, "PlayerEditBookEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["slot"] == 0
            assert snapshot["is_signing"] is False
            assert snapshot["previous_pages"] == ("",)
            assert snapshot["new_pages"] == ("Endstone automated book edit",)
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_emote(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerEmoteEvent"]
        assert event_snapshots, "PlayerEmoteEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["emote_id"] == "anim.wave"
            assert snapshot["is_muted"] is False
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_drop_item(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerDropItemEvent"]
        assert event_snapshots, "PlayerDropItemEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["item_type"] == "minecraft:stick"
            assert snapshot["item_amount"] == 2
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_item_consume(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_amount: int,
        expected_player: str,
    ) -> None:
        event_snapshots = snapshots["PlayerItemConsumeEvent"]
        assert event_snapshots, "PlayerItemConsumeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["item_type"] == "minecraft:apple"
            assert snapshot["item_amount"] == expected_amount
            assert snapshot["hand"].rsplit(".", 1)[-1] == "HAND"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_shear_actor(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimShear",
    ) -> None:
        event_snapshots = snapshots["PlayerShearActorEvent"]
        assert event_snapshots, "PlayerShearActorEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["actor_type"] == "minecraft:sheep"
            assert snapshot["item_type"] == "minecraft:shears"
            assert snapshot["hand"].rsplit(".", 1)[-1] == "HAND"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_bucket_actor(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimBucket",
    ) -> None:
        event_snapshots = snapshots["PlayerBucketActorEvent"]
        assert event_snapshots, "PlayerBucketActorEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["actor_type"] == "minecraft:cod"
            assert snapshot["original_bucket_type"] == "minecraft:water_bucket"
            assert snapshot["hand"].rsplit(".", 1)[-1] == "HAND"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_bucket(
        snapshots: dict[str, list[dict]],
        event_name: str,
        expected_cancelled: bool,
        expected_player: str,
        expected_bucket: str,
        expected_clicked: str,
    ) -> None:
        event_snapshots = snapshots[event_name]
        assert event_snapshots, f"{event_name} did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["block_clicked_type"] == expected_clicked
            assert snapshot["bucket_type"] == expected_bucket
            assert snapshot["hand"].rsplit(".", 1)[-1] == "HAND"
            assert snapshot["item_stack_type"]
            assert snapshot["item_stack_amount"] == 1
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_cauldron_level_change(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["CauldronLevelChangeEvent"]
        assert event_snapshots, (
            "CauldronLevelChangeEvent did not produce a snapshot"
        )
        for snapshot in event_snapshots:
            assert snapshot["block_type"] == "minecraft:cauldron"
            assert snapshot["actor_type"] == "minecraft:player"
            assert snapshot["reason"].rsplit(".", 1)[-1] == "BUCKET_EMPTY"
            assert snapshot["new_type"] == "minecraft:cauldron"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_block_place(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimPlace",
    ) -> None:
        event_snapshots = snapshots["BlockPlaceEvent"]
        assert event_snapshots, "BlockPlaceEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["block_type"] == "minecraft:dirt"
            assert snapshot["block_placed_type"] == "minecraft:dirt"
            assert snapshot["block_replaced_type"] == "minecraft:air"
            assert snapshot["block_against_type"] == "minecraft:stone"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_block_break(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimBreak",
    ) -> None:
        event_snapshots = snapshots["BlockBreakEvent"]
        assert event_snapshots, "BlockBreakEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["block_type"] == "minecraft:stone"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_armor_stand_manipulate(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimArmor",
    ) -> None:
        event_snapshots = snapshots["PlayerArmorStandManipulateEvent"]
        assert event_snapshots, (
            "PlayerArmorStandManipulateEvent did not produce a snapshot"
        )
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["actor_type"] == "minecraft:armor_stand"
            assert snapshot["slot"].rsplit(".", 1)[-1] == "HEAD"
            assert snapshot["armor_stand_item_type"] == "minecraft:air"
            assert snapshot["armor_stand_item_amount"] == 0
            assert snapshot["player_item_type"] == "minecraft:leather_helmet"
            assert snapshot["player_item_amount"] == 1
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_interact_actor(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str | None = None,
        expected_actor: str = "minecraft:villager",
    ) -> None:
        event_snapshots = snapshots["PlayerInteractActorEvent"]
        assert event_snapshots, (
            "PlayerInteractActorEvent did not produce a snapshot"
        )
        for snapshot in event_snapshots:
            assert snapshot["player"] == (expected_player or ctx.bot_name)
            assert snapshot["actor_type"] == expected_actor
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_gametest_player_interact(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimInteract",
    ) -> None:
        event_snapshots = snapshots["PlayerInteractEvent"]
        assert event_snapshots, "PlayerInteractEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["action"].rsplit(".", 1)[-1] == "RIGHT_CLICK_BLOCK"
            assert snapshot["has_item"] is True
            assert snapshot["item_type"] == "minecraft:stick"
            assert snapshot["has_block"] is True
            assert snapshot["block_type"] == "minecraft:stone"
            assert snapshot["block_face"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_arm_swing(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerArmSwingEvent"]
        assert event_snapshots, "PlayerArmSwingEvent did not produce a snapshot"
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert snapshot["has_item"] is False
        assert snapshot["item_type"] is None
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_chat(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerChatEvent"]
        assert event_snapshots, "PlayerChatEvent did not produce a snapshot"
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert snapshot["message"] == "endstone automated player chat"
        assert snapshot["format"]
        assert snapshot["recipient_count"] >= 1
        assert ctx.bot_name in snapshot["recipient_names"]
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_command(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerCommandEvent"]
        assert event_snapshots, "PlayerCommandEvent did not produce a snapshot"
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert snapshot["command"].strip().lstrip("/") == "list"
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_input(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["PlayerInputEvent"]
        assert event_snapshots, "PlayerInputEvent did not produce a snapshot"
        states = {
            tuple(snapshot[key] for key in (
                "forward", "backward", "left", "right", "jump", "sneak", "sprint"
            ))
            for snapshot in event_snapshots
        }
        assert (False, False, False, True, False, False, False) in states, states
        assert (False, False, True, False, False, False, False) in states, states
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert all(
                isinstance(snapshot[key], bool)
                for key in (
                    "forward", "backward", "left", "right", "jump", "sneak", "sprint"
                )
            )
    
    def verify_toggle_event(
        snapshots: dict[str, list[dict]],
        event_name: str,
        field: str,
        expected_state: bool,
    ) -> None:
        event_snapshots = snapshots[event_name]
        assert event_snapshots, f"{event_name} did not produce a snapshot"
        assert len(event_snapshots) == 1
        for snapshot in event_snapshots:
            if "player" in snapshot:
                assert snapshot["player"] == ctx.bot_name
            else:
                assert snapshot["actor_type"] == "minecraft:player"
            assert snapshot[field] is expected_state
    
    def verify_player_move(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerMoveEvent"]
        assert event_snapshots, "PlayerMoveEvent did not produce a snapshot"
        moved_forward = False
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name, snapshot
            assert snapshot["from_dimension"] == "minecraft:overworld", snapshot
            assert snapshot["to_dimension"] == "minecraft:overworld", snapshot
            from_x, from_y, from_z = snapshot["from_xyz"]
            to_x, to_y, to_z = snapshot["to_xyz"]
            moved_forward |= to_x > from_x and (to_y, to_z) == (from_y, from_z)
            assert snapshot["from_xyz"] != snapshot["to_xyz"], snapshot
            assert snapshot["cancelled_before"] is False, snapshot
            assert snapshot["cancelled"] is expected_cancelled, snapshot
        assert moved_forward, event_snapshots
    
    def verify_player_jump(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerJumpEvent"]
        assert event_snapshots, "PlayerJumpEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["from_dimension"] == "minecraft:overworld"
            assert snapshot["to_dimension"] == "minecraft:overworld"
            assert snapshot["to_xyz"][1] > snapshot["from_xyz"][1]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_open_sign(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["PlayerOpenSignEvent"]
        assert event_snapshots, "PlayerOpenSignEvent did not produce a snapshot"
        assert len(event_snapshots) == 1
        snapshot = event_snapshots[0]
        assert snapshot["player"] == ctx.bot_name
        assert "sign" in snapshot["sign_type"].lower()
        assert snapshot["side"].rsplit(".", 1)[-1] == "FRONT"
        assert snapshot["cause"].rsplit(".", 1)[-1] == "PLUGIN"
        assert snapshot["cancelled_before"] is False
        assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_bed(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str | None = None,
    ) -> None:
        player_name = expected_player or ctx.bot_name
        enter_snapshots = snapshots["PlayerBedEnterEvent"]
        assert enter_snapshots, "PlayerBedEnterEvent did not produce a snapshot"
        for snapshot in enter_snapshots:
            assert snapshot["player"] == player_name
            assert "bed" in snapshot["bed_type"].lower()
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
        if not expected_cancelled:
            leave_snapshots = snapshots["PlayerBedLeaveEvent"]
            assert leave_snapshots, "PlayerBedLeaveEvent did not produce a snapshot"
            for snapshot in leave_snapshots:
                assert snapshot["player"] == player_name
                assert "bed" in snapshot["bed_type"].lower()
    
    def verify_player_item_held(
        snapshots: dict[str, list[dict]],
        expected_slot: int,
        expected_cancelled: bool = False,
    ) -> None:
        event_snapshots = snapshots["PlayerItemHeldEvent"]
        assert event_snapshots, "PlayerItemHeldEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert 0 <= snapshot["previous_slot"] <= 8
            assert 0 <= snapshot["new_slot"] <= 8
            assert snapshot["previous_slot"] != snapshot["new_slot"]
            assert snapshot["new_slot"] == expected_slot
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_player_toggle_flight(
        snapshots: dict[str, list[dict]], expected_state: bool
    ) -> None:
        event_snapshots = snapshots["PlayerToggleFlightEvent"]
        assert event_snapshots, "PlayerToggleFlightEvent did not produce a snapshot"
        assert len(event_snapshots) == 1
        for snapshot in event_snapshots:
            assert snapshot["player"] == ctx.bot_name
            assert snapshot["is_flying"] is expected_state
    
    def verify_server_command(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ServerCommandEvent"]
        assert event_snapshots, "ServerCommandEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["sender"]
            assert snapshot["command"].strip().lstrip("/") == (
                "say endstone automated event test"
            )
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_unknown_command(
        snapshots: dict[str, list[dict]]
    ) -> None:
        event_snapshots = snapshots["UnknownCommandEvent"]
        assert event_snapshots, "UnknownCommandEvent did not produce a snapshot"
        matching_snapshots = [
            snapshot
            for snapshot in event_snapshots
            if snapshot["command_line"].strip().lstrip("/")
            == "endstone_auto_events_unknown"
        ]
        assert matching_snapshots, (
            "automated unknown command was not observed: "
            f"{event_snapshots!r}"
        )
        for snapshot in matching_snapshots:
            assert snapshot["sender"], f"missing sender: {snapshot!r}"
            assert snapshot["message_present"] is True, (
                f"unknown command message is absent: {snapshot!r}"
            )
    
    def verify_script_message(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ScriptMessageEvent"]
        assert event_snapshots, "ScriptMessageEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["sender"]
            assert snapshot["message_id"] == "endstone:auto_events"
            assert snapshot["message"] == "smoke"
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_broadcast_message(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BroadcastMessageEvent"]
        assert event_snapshots, "BroadcastMessageEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["message"] == "Endstone automated event test"
            assert isinstance(snapshot["recipient_count"], int)
            assert snapshot["recipient_count"] >= 0
            assert snapshot["is_asynchronous"] is False
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_thunder_change(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ThunderChangeEvent"]
        assert event_snapshots, "ThunderChangeEvent did not produce a snapshot"
        target_snapshots = [
            snapshot for snapshot in event_snapshots if snapshot["to_thunder_state"] is True
        ]
        assert target_snapshots, "ThunderChangeEvent did not report the target state"
        for snapshot in target_snapshots:
            assert snapshot["to_thunder_state"] is True
            assert snapshot["level"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_weather_change(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["WeatherChangeEvent"]
        assert event_snapshots, "WeatherChangeEvent did not produce a snapshot"
        target_snapshots = [
            snapshot for snapshot in event_snapshots if snapshot["to_weather_state"] is True
        ]
        assert target_snapshots, "WeatherChangeEvent did not report the target state"
        for snapshot in target_snapshots:
            assert snapshot["to_weather_state"] is True
            assert snapshot["level"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def spawn_tagged_mob(name: str, actor_type: str = "minecraft:zombie"):
        tag = auto_tag(name)
        location = actor_spawn_location()
        mob_location = Location(
            location.dimension,
            location.x + 4.0,
            location.y,
            location.z,
            location.pitch,
            location.yaw,
        )
        mob = overworld.spawn_actor(mob_location, actor_type)
        if mob is None:
            raise RuntimeError(f"could not spawn {actor_type}")
        if not mob.add_scoreboard_tag(tag):
            raise RuntimeError(f"could not tag {actor_type} as {tag}")
        mob_refs[tag] = mob
        return mob
    
    def prepare_actor_pickup_item() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        clear_player_inventory()
        remove_actors("minecraft:item")
        remove_actors("minecraft:villager_v2")
        # The preceding actor-damage scenario leaves its zombie alive.
        # Remove it as well so it cannot win the race for the carrot in
        # the cancellation case; this fixture is specifically for a
        # villager pickup.
        remove_actors("minecraft:zombie")
    
        previous_mob_griefing = bool(
            server.level.get_game_rule("mobgriefing")
        )
        actor_pickup_fixture["previous_mob_griefing"] = previous_mob_griefing
        server.level.set_game_rule("mobgriefing", True)
    
        tag = auto_tag("pickup")
        actor_pickup_fixture["tag"] = tag
        command(
            # Put the villager directly in front of the player, where the
            # dropped stack lands with the driver's default rotation.
            f"execute at {ctx.bot_name} run summon minecraft:villager ~ ~ ~1"
        )
        command(
            f"execute at {ctx.bot_name} run tag "
            f"@e[type=minecraft:villager,r=4,c=1] add {tag}"
        )
        # A summoned villager is not guaranteed to have the peasant
        # component group yet.  Make it a farmer so the vanilla
        # shareables table explicitly accepts carrots.
        command(f"event entity @e[tag={tag}] minecraft:become_farmer")
        # Use a server-side item actor. This scenario must remain
        # server-automatable and must not depend on the protocol driver.
        location = player.location
        fixture_item = overworld.drop_item(
            Location(
                location.dimension,
                location.x,
                location.y,
                location.z + 1.0,
            ),
            ItemStack("minecraft:carrot"),
        )
        fixture_item.pickup_delay = 0
    
    def cleanup_actor_pickup_item() -> None:
        tag = actor_pickup_fixture.pop("tag", None)
        if tag is not None:
            remove_actors(tag=tag)
        remove_actors("minecraft:item")
        previous_mob_griefing = actor_pickup_fixture.pop(
            "previous_mob_griefing", None
        )
        if previous_mob_griefing is not None:
            server.level.set_game_rule(
                "mobgriefing", bool(previous_mob_griefing)
            )
    
    def verify_actor_pickup_item(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorPickupItemEvent"]
        assert event_snapshots, "ActorPickupItemEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:villager_v2"
            assert snapshot["item_type"] == "minecraft:carrot"
            assert 0 < snapshot["amount"] <= snapshot["stack_amount"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def actor_damage() -> None:
        tag = auto_tag("damage")
        spawn_tagged_mob("damage")
        command(f"damage @e[tag={tag}] 1 magic")
    
    def verify_actor_damage(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorDamageEvent"]
        assert event_snapshots, "ActorDamageEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:zombie"
            assert snapshot["damage"] == 1
            assert str(snapshot["damage_type"]).lower() == "magic"
            assert snapshot["is_cancellable"] is True
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_actor_explode(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorExplodeEvent"]
        assert event_snapshots, "ActorExplodeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:chicken"
            assert snapshot["block_count"] == 0
            assert all(isinstance(snapshot[key], (int, float)) for key in ("x", "y", "z"))
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_actor_knockback(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorKnockbackEvent"]
        assert event_snapshots, "ActorKnockbackEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:zombie"
            assert snapshot["has_source"] is True
            assert snapshot["source_type"] == "minecraft:zombie"
            knockback = snapshot["knockback"]
            assert len(knockback) == 3
            assert all(isinstance(component, (int, float)) for component in knockback)
            assert any(component != 0 for component in knockback)
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_block_explode(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BlockExplodeEvent"]
        assert event_snapshots, "BlockExplodeEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["block_type"] == "minecraft:stone"
            assert isinstance(snapshot["block_count"], int)
            assert snapshot["block_count"] >= 0
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def verify_inventory_open_close(
        snapshots: dict[str, list[dict]],
        expected_cancelled: bool,
        expected_player: str = "EndstoneSimContainer",
        *,
        expect_close: bool = True,
    ) -> None:
        open_snapshots = snapshots["InventoryOpenEvent"]
        assert open_snapshots, "InventoryOpenEvent did not produce a snapshot"
        for snapshot in open_snapshots:
            assert snapshot["player"] == expected_player
            assert snapshot["size"] == 27
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
        if not expected_cancelled and expect_close:
            close_snapshots = snapshots["InventoryCloseEvent"]
            assert close_snapshots, "InventoryCloseEvent did not produce a snapshot"
            for snapshot in close_snapshots:
                assert snapshot["player"] == expected_player
                assert snapshot["size"] == 27
    
    def actor_death() -> None:
        tag = auto_tag("death")
        spawn_tagged_mob("death")
        command(f"damage @e[tag={tag}] 100 magic")
    
    def verify_actor_death(snapshots: dict[str, list[dict]]) -> None:
        event_snapshots = snapshots["ActorDeathEvent"]
        assert event_snapshots, "ActorDeathEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:zombie"
            assert str(snapshot["damage_type"]).lower() == "magic"
    
    def actor_effect() -> None:
        mob = spawn_tagged_mob("endstone_auto_effect")
        mob.add_effect(Effect("minecraft:speed", 200, 1))
    
    def verify_actor_effect(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorEffectEvent"]
        assert event_snapshots, "ActorEffectEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:zombie"
            assert snapshot["action"] == "ADDED"
            assert snapshot["effect_type"] == "minecraft:speed"
            assert snapshot["duration"] == 200
            assert snapshot["amplifier"] == 1
            assert snapshot["infinite"] is False
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def actor_collision() -> None:
        location = actor_spawn_location()
        first = overworld.spawn_actor(
            Location(location.dimension, location.x + 2.0, location.y, location.z),
            "minecraft:cow",
        )
        second = overworld.spawn_actor(
            Location(location.dimension, location.x + 2.0, location.y, location.z),
            "minecraft:sheep",
        )
        if first is None or second is None:
            raise RuntimeError("could not spawn the collision pair")
    
    def verify_actor_collision(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorCollideWithActorEvent"]
        assert event_snapshots, "ActorCollideWithActorEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def actor_dismount() -> None:
        location = actor_spawn_location()
        rider = overworld.spawn_actor(
            Location(location.dimension, location.x + 3.0, location.y, location.z),
            "minecraft:zombie",
        )
        vehicle = overworld.spawn_actor(
            Location(location.dimension, location.x + 3.0, location.y, location.z),
            "minecraft:boat",
        )
        if rider is None or vehicle is None:
            raise RuntimeError("could not spawn the ride pair")
        dismount_fixture_index["value"] += 1
        fixture = dismount_fixture_index["value"]
        rider_tag = auto_tag(f"rider_{fixture}")
        vehicle_tag = auto_tag(f"vehicle_{fixture}")
        rider.add_scoreboard_tag(rider_tag)
        vehicle.add_scoreboard_tag(vehicle_tag)
        dismount_fixture.clear()
        dismount_fixture.update(rider_tag=rider_tag, vehicle_tag=vehicle_tag)
        command(
            f"ride @e[tag={rider_tag},c=1] start_riding "
            f"@e[tag={vehicle_tag},c=1]"
        )
        # A cancelled dismount is reported by BDS as a failed ride command;
        # the event snapshot is the assertion for this scenario.
        dispatch_command(
            f"ride @e[tag={rider_tag},c=1] stop_riding"
        )
    def cleanup_actor_dismount() -> None:
        # Remove only the uniquely tagged rider and vehicle created by this
        # fixture. Cleanup runs after the settle window, so delayed ride
        # callbacks cannot leak into the next scenario.
        for key in ("rider_tag", "vehicle_tag"):
            tag = dismount_fixture.get(key)
            if tag:
                remove_actors(tag=tag)
        dismount_fixture.clear()
    
    def verify_actor_dismount(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["ActorDismountEvent"]
        assert event_snapshots, "ActorDismountEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert snapshot["actor_type"] == "minecraft:zombie"
            assert snapshot["vehicle_type"] == "minecraft:boat"
            assert snapshot["is_self_ride"] is False
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    
    def set_player_spawn() -> None:
        player = player_for_action()
        location = player.location
        player.respawn_location = Location(
            location.dimension,
            location.block_x,
            location.block_y,
            location.block_z,
        )
    def prepare_piston() -> None:
        piston_fixture_index["value"] += 1
        x, y, z = block_target((1 + 8 * piston_fixture_index["value"], 0, 0))
        piston_fixture.clear()
        piston_fixture.update(x=x, y=y, z=z)
        set_region_type(x - 2, y, z - 1, x + 3, y + 2, z + 1, "minecraft:air")
        # facing_direction=4 is east. Put the power source above the
        # piston, leaving its east-facing push path empty and the player
        # outside the fixture. Removing the source on a later server tick
        # exercises both piston transitions without a player transaction.
        piston = server.create_block_data(
            "minecraft:piston", {"facing_direction": 4}
        )
        overworld.get_block_at(x, y, z).set_data(piston)
        power = server.create_block_data("minecraft:redstone_block")
        overworld.get_block_at(x, y + 1, z).set_data(power)
    
        def retract() -> None:
            set_block_type(x, y + 1, z, "minecraft:air", apply_physics=True)
    
        if server.scheduler.run_task(ctx.plugin, retract, delay=12) is None:
            raise RuntimeError("could not schedule piston retraction")
    
    def cleanup_piston() -> None:
        # The scheduled power removal and both piston transitions have had
        # time to run during the settle window. Clear the complete bounded
        # fixture, including the piston, power source, and push path.
        if not piston_fixture:
            return
        x, y, z = (
            piston_fixture["x"],
            piston_fixture["y"],
            piston_fixture["z"],
        )
        set_region_type(x - 2, y, z - 1, x + 3, y + 2, z + 1, "minecraft:air")
        piston_fixture.clear()
    
    def verify_piston(
        snapshots: dict[str, list[dict]],
        expected: dict[str, tuple[str, bool]],
    ) -> None:
        for event_name, (direction, expected_cancelled) in expected.items():
            event_snapshots = snapshots[event_name]
            assert event_snapshots, f"{event_name} did not produce a snapshot"
            for snapshot in event_snapshots:
                assert "piston" in snapshot["block_type"]
                assert snapshot["direction"].rsplit(".", 1)[-1].lower() == direction
                assert snapshot["cancelled_before"] is False
                assert snapshot["cancelled"] is expected_cancelled
    
    def prepare_campfire() -> None:
        x, y, z = block_target((1, 0, 0))
        set_block_type(x, y, z, "minecraft:air")
        command(f'setblock {x} {y} {z} campfire ["extinguished"=false]')
        state = overworld.get_block_at(x, y, z).capture_state(False)
        state.set_item(0, ItemStack("minecraft:cod"))
        # CampfireBlockActor finishes at 599 ticks. Starting at 598 keeps
        # this scenario deterministic without waiting for a full cook.
        state.set_cook_time(0, 598)
    
    def verify_block_cook(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BlockCookEvent"]
        assert event_snapshots, "BlockCookEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert "campfire" in snapshot["block_type"]
            assert snapshot["source_type"] == "minecraft:cod"
            assert snapshot["recipe_id"]
            assert snapshot["recipe_tag"] in {"campfire", "soul_campfire"}
            assert snapshot["recipe_input_matches"] is True
            assert snapshot["recipe_result_type"] == snapshot["result_type"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def prepare_block_from_to() -> None:
        # LiquidBlock::_trySpreadTo deliberately skips instant-ticking
        # GameTest regions. Use the already-loaded spawn chunk, then
        # select this fixture by its source coordinates in the verifier.
        if not overworld.load_chunk(0, 0):
            raise RuntimeError("could not load the overworld liquid fixture")
        x, z = 5, 1
        # The clean template's flat-world floor is y=75. Do not derive
        # this from the highest block: earlier GameTest structures can
        # raise that value in the same column.
        y = 76
        block_from_to_target["xyz"] = (x, y, z)
        for fixture_x in range(x - 1, x + 2):
            for fixture_z in range(z - 1, z + 2):
                set_block_type(fixture_x, y - 1, fixture_z, "minecraft:stone")
                for fixture_y in range(y, y + 2):
                    set_block_type(fixture_x, fixture_y, fixture_z, "minecraft:air")
        # Direct block writes do not enqueue a liquid tick on this BDS
        # build. Place a lava source through the command path, then toggle
        # one adjacent block to issue the native neighbor update that
        # schedules LiquidBlock::_trySpreadTo.
        command(f"setblock {x} {y} {z} lava")
        command(f"setblock {x + 1} {y} {z} stone")
        command(f"setblock {x + 1} {y} {z} air")
    
    def ready_block_from_to(snapshots: dict[str, list[dict]]) -> bool:
        target = block_from_to_target.get("xyz")
        if target is None:
            return False
        return any(
            tuple(snapshot.get("xyz", ())) == target
            and "lava" in snapshot.get("block_type", "")
            for snapshot in snapshots["BlockFromToEvent"]
        )

    def cleanup_block_from_to() -> None:
        # Keep the target in memory until verification finishes. The runner
        # performs cleanup before calling the scenario verifier.
        target = block_from_to_target.get("xyz")
        if target is None:
            return
        x, y, z = target
        set_region_type(x - 1, y - 1, z - 1, x + 2, y + 1, z + 1, "minecraft:air")

    def verify_block_from_to(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BlockFromToEvent"]
        assert event_snapshots, "BlockFromToEvent did not produce a snapshot"
        matching_snapshots = [
            snapshot
            for snapshot in event_snapshots
            if tuple(snapshot["xyz"]) == block_from_to_target["xyz"]
        ]
        assert matching_snapshots, event_snapshots
        for snapshot in matching_snapshots:
            assert "lava" in snapshot["block_type"], snapshot
            assert snapshot["to_block_type"], snapshot
            assert snapshot["cancelled_before"] is False, snapshot
            assert snapshot["cancelled"] is expected_cancelled, snapshot
    
    def prepare_block_form() -> None:
        # Use the supplied local-coordinate fixture.  The water and lava
        # sources flow toward one another and form a solid block at their
        # contact point through the native liquid simulation.
        location = player_for_action().location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        set_region_type(x - 1, y - 1, z + 1, x + 1, y - 1, z + 4, "minecraft:air")
        # Use the block API for the two sources as well. It calls the
        # same native setBlock path without emitting an error when the
        # server rejects a redundant command placement.
        set_block_type(x, y - 1, z + 1, "minecraft:flowing_water", apply_physics=True)
        set_block_type(x, y - 1, z + 4, "minecraft:flowing_lava", apply_physics=True)
    
    def verify_block_form(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BlockFormEvent"]
        assert event_snapshots, "BlockFormEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert "lava" in snapshot["block_type"]
            assert snapshot["new_type"] in {
                "minecraft:basalt",
                "minecraft:cobblestone",
                "minecraft:obsidian",
            }
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def prepare_block_grow() -> None:
        player_for_action()
        block_grow_random_tick_speed["previous"] = int(
            server.level.get_game_rule("randomtickspeed")
        )
        server.level.set_game_rule("randomtickspeed", 1000)
    
        prefix = f"execute at {ctx.bot_name} rotated ~ 0 run"
        # Use single-block fill ranges. The dirt support makes the
        # sapling fixture independent of the template's surface block.
        location = player_for_action().location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        set_block_type(x, y - 1, z + 1, "minecraft:dirt")
        set_block_type(x, y, z + 1, "minecraft:air")
        set_block_type(x, y, z + 1, "minecraft:oak_sapling", apply_physics=True)
    
    def cleanup_block_grow() -> None:
        previous = block_grow_random_tick_speed.pop("previous", None)
        if previous is not None:
            server.level.set_game_rule("randomtickspeed", previous)
    
    def verify_block_grow(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["BlockGrowEvent"]
        assert event_snapshots, "BlockGrowEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert "sapling" in snapshot["block_type"]
            assert ":" in snapshot["new_type"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    def prepare_leaves_decay() -> None:
        player = player_for_action()
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        # Keep the fixture in the bot's loaded chunk. A leaf cluster with
        # no log within the native radius-4 scan is eligible for the real
        # LeavesBlock random-tick decay path.
        set_region_type(x - 5, y + 1, z - 5, x + 5, y + 6, z + 5, "minecraft:air")
        set_region_type(x - 2, y - 1, z - 2, x + 2, y - 1, z + 2, "minecraft:stone")
        leaves = server.create_block_data(
            "minecraft:oak_leaves",
            {"persistent_bit": False, "update_bit": True},
        )
        overworld.get_block_at(x, y + 2, z).set_data(
            leaves, apply_physics=False
        )
        # Random ticking is the native trigger for this event. Increase
        # it only for this fixture; the scenario cleanup restores the
        # value from the copied template even when the scenario times out.
        leaves_random_tick_speed["previous"] = int(
            server.level.get_game_rule("randomtickspeed")
        )
        server.level.set_game_rule("randomtickspeed", 1000)
    
    def cleanup_leaves_decay() -> None:
        previous = leaves_random_tick_speed.pop("previous", None)
        if previous is not None:
            server.level.set_game_rule("randomtickspeed", previous)
    
    def verify_leaves_decay(
        snapshots: dict[str, list[dict]], expected_cancelled: bool
    ) -> None:
        event_snapshots = snapshots["LeavesDecayEvent"]
        assert event_snapshots, "LeavesDecayEvent did not produce a snapshot"
        for snapshot in event_snapshots:
            assert "leaves" in snapshot["block_type"]
            assert snapshot["cancelled_before"] is False
            assert snapshot["cancelled"] is expected_cancelled
    
    armor_stand_driver_data: dict[str, object] = {
        "actor_type": "minecraft:armor_stand",
        "action_type": "interact",
        "requires_item": True,
        "include_stack_id": True,
        "reassert_selected_slot": True,
        "hit_y": 1.7,
    }
    book_driver_data: dict[str, object] = {"slot": 0}
    craft_driver_data: dict[str, object] = {
        "input_name": "oak_log",
        "output_network_id": 5,
    }
    def prepare_sign() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        clear_player_inventory()
        x, y, z = block_target((1, 0, 0))
        set_block_type(x, y, z, "minecraft:air")
        command(f'setblock {x} {y} {z} standing_sign ["ground_sign_direction"=0]')
        sign = overworld.get_block_at(x, y, z).capture_state()
        if not isinstance(sign, Sign):
            raise TypeError(f"prepared block is not a sign: {sign}")
        player.open_sign(sign, Sign.Side.FRONT)
    
    def prepare_armor_stand_interaction() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        give_player_item("minecraft:leather_helmet")
        remove_actors("minecraft:armor_stand")
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        set_region_type(x - 1, y, z + 1, x + 1, y, z + 3, "minecraft:stone")
        command(f"summon minecraft:armor_stand {x + 0.5} {y + 1} {z + 1.5}")
    
    def prepare_jump() -> None:
        player = player_for_action()
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        # Endstone reports the player's feet location while the protocol
        # position includes the actor height offset.  A solid platform one
        # block below the feet makes PlayerAuthInput's on-ground check
        # deterministic for PlayerJumpEvent.
        set_region_type(x - 1, y - 1, z - 1, x + 1, y - 1, z + 1, "minecraft:stone")
        set_region_type(x - 1, y, z - 1, x + 1, y + 3, z + 1, "minecraft:air")
    
    def prepare_player_move() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        # Earlier world-mechanics scenarios can leave liquids, dropped
        # items, or partial fixtures near the bot. Give the protocol
        # move exactly one clear, level step without teleporting it.
        set_region_type(x - 1, y - 1, z - 1, x + 2, y - 1, z + 1, "minecraft:stone")
        set_region_type(x - 1, y, z - 1, x + 2, y + 3, z + 1, "minecraft:air")
    
    def prepare_book() -> None:
        player = player_for_action()
        clear_player_inventory()
        book = ItemStack("minecraft:writable_book")
        meta = book.item_meta
        meta.pages = [""]
        book.set_item_meta(meta)
        # Force an inventory delta so the protocol driver receives the
        # writable-book descriptor before it sends BookEdit.
        player.inventory.set_item(0, None)
        player.inventory.set_item(0, book)
    
    def prepare_consume_item() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        give_player_item("minecraft:apple", 2)
    
    def prepare_craft_item() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        give_player_item("minecraft:oak_log")
        command(f"recipe give {ctx.bot_name} *")
    
    def prepare_hotbar_item() -> None:
        player = player_for_action()
        if player.inventory.held_item_slot != 0:
            player.inventory.held_item_slot = 0
        # The optional diagnostic plugin unconditionally cancels this
        # event. Disable it so the scenario exercises only this test's
        # normal and cancellation behavior.
        plugin = server.plugin_manager.get_plugin("toggle_event_logger")
        if plugin is not None and plugin.is_enabled:
            server.plugin_manager.disable_plugin(plugin)
            item_held_logger_state["disabled"] = True
        # Use the native command path so BDS publishes both authoritative
        # ItemV4 descriptors to the protocol client before it changes the
        # selected hotbar slot.
        clear_player_inventory()
        command(
            f"replaceitem entity {ctx.bot_name} slot.hotbar 1 "
            "minecraft:diamond 1 0"
        )
    def prepare_drop_item() -> None:
        player_for_action()
        # Keep the item in slot 0 so the driver can correlate the legacy
        # changed-slot list with the server's selected hotbar slot.
        give_player_item("minecraft:stick", 2)
    
    def prepare_pickup_arrow() -> None:
        # The arrow is summoned at the bot's current position, which is
        # the native pickup trigger requested by this event's contract.
        player_for_action().game_mode = GameMode.SURVIVAL
        remove_actors("minecraft:arrow")
        command(
            f"execute as {ctx.bot_name} at @s run summon arrow ~ ~ ~"
        )
    def prepare_riptide() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        # Use the same deterministic setup as a player: give a trident,
        # enchant the held item, and make the weather rainy so Riptide is
        # valid without depending on generated terrain.
        give_player_item("minecraft:trident")
        command(f"enchant {ctx.bot_name} riptide 1")
        # The command path changes the authoritative item immediately but
        # this BDS build does not always publish that change to a client
        # that is not holding a UI. Re-set the same enchanted stack
        # through the existing inventory API so the protocol driver gets
        # the exact descriptor it must echo in packet 30.
        trident = player.inventory.get_item(0)
        if trident is None:
            raise RuntimeError("the trident was not placed in hotbar slot 0")
        meta = trident.item_meta
        if not meta.has_enchant("minecraft:riptide"):
            if not meta.add_enchant("minecraft:riptide", 1, True):
                raise RuntimeError("could not apply Riptide to the test trident")
            trident.set_item_meta(meta)
        # Force an inventory delta even when /enchant already left the
        # same metadata in place; otherwise the server may optimize away
        # the packet and the offline client cannot learn the stack data.
        player.inventory.set_item(0, None)
        player.inventory.set_item(0, trident)
        command("weather rain")
        command(f"execute at {ctx.bot_name} run setblock ~ ~ ~ water")
    
    def prepare_bucket_actor() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        give_player_item("minecraft:water_bucket")
        remove_actors("minecraft:cod")
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        # A fish outside water is not bucketable: the vanilla interaction
        # returns failure before BucketableComponent::getInteraction is
        # reached. Put the fresh target in a source block while keeping it
        # in the bot's line of sight.
        set_region_type(x - 1, y, z + 1, x + 1, y, z + 3, "minecraft:stone")
        command(f"fill {x - 1} {y + 1} {z + 1} {x + 1} {y + 1} {z + 3} water")
        command(
            f"summon minecraft:cod {x + 0.5} {y + 1} {z + 1.5}"
        )
    def prepare_shear_actor() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        command(f"gamemode survival {ctx.bot_name}")
        give_player_item("minecraft:shears")
        remove_actors("minecraft:sheep")
        location = player.location
        x, y, z = floor(location.x), floor(location.y), floor(location.z)
        set_region_type(x - 1, y, z + 1, x + 1, y, z + 3, "minecraft:stone")
        command(
            f"summon minecraft:sheep {x + 0.5} {y + 1} {z + 1.5}"
        )
    def teleport_player() -> None:
        player = player_for_action()
        location = player.location
        if not player.teleport(
            Location(
                location.dimension,
                location.x + 8.0,
                location.y,
                location.z,
                location.pitch,
                location.yaw,
            )
        ) and not ctx.plugin.should_cancel_event("PlayerTeleportEvent"):
            raise RuntimeError("player.teleport returned False")
    
    def change_player_dimension() -> None:
        player = player_for_action()
        nether = server.level.get_dimension(Dimension.NETHER)
        if nether is None:
            raise RuntimeError("the nether is not available")
        if not player.teleport(Location(nether, 0.0, 80.0, 0.0)):
            raise RuntimeError("player dimension teleport returned False")
    
    def change_player_game_mode() -> None:
        player = player_for_action()
        player.game_mode = (
            GameMode.SURVIVAL
            if player.game_mode == GameMode.CREATIVE
            else GameMode.CREATIVE
        )
    def allow_player_flight() -> None:
        player = player_for_action()
        player.allow_flight = True
    
    def prepare_arm_swing() -> None:
        player_for_action()
        clear_player_inventory()
    
    def prepare_player_flight_stop() -> None:
        player = player_for_action()
        player.allow_flight = True
        player.is_flying = True
    
    def prepare_player_sneak_stop() -> None:
        player = player_for_action()
        player.is_sneaking = True
    
    def prepare_player_sprint_stop() -> None:
        player = player_for_action()
        player.is_sprinting = True
    
    def give_player_exp() -> None:
        player_for_action().give_exp(5)
    
    def give_player_exp_level() -> None:
        player_for_action().give_exp_levels(1)
    
    def drop_item_for_player() -> None:
        player = player_for_action()
        player.game_mode = GameMode.SURVIVAL
        clear_player_inventory()
        remove_actors("minecraft:item")
        location = player.location
        item = player.dimension.drop_item(
            Location(location.dimension, location.x, location.y + 0.75, location.z),
            ItemStack("minecraft:diamond"),
        )
        item.pickup_delay = 0
    
    def spawn_experience_orb() -> None:
        player = player_for_action()
        location = player.location
        orb = player.dimension.spawn_actor(
            Location(location.dimension, location.x, location.y, location.z),
            "minecraft:xp_orb",
        )
        if orb is None:
            raise RuntimeError("could not spawn an experience orb")
    
    def kill_player() -> None:
        player_for_action().health = 0
    
    def kick_player() -> None:
        player_for_action().kick("automated Endstone event test")
    
    # GameTest scenario declarations are provided by scenarios/gametest.py.

    scenarios = build_scenario_groups(ctx, locals())
    
    if scenarios[-1].name != "player-kick-quit":
        raise AssertionError(
            "player-kick-quit must remain the final automated event scenario"
        )
    if "PlayerQuitEvent" not in scenarios[-1].expected_events:
        raise AssertionError(
            "PlayerQuitEvent must be asserted by the final automated scenario"
        )
    scenarios = [
        scenario
        for scenario in scenarios
        if not scenario.isolated
        and scenario.name not in EXCLUDED_AUTOMATED_SCENARIOS
    ]
    
    if ctx.selected_scenario_names is None:
        return scenarios
    
    known_names = {scenario.name for scenario in scenarios}
    unknown = sorted(ctx.selected_scenario_names - known_names)
    if unknown:
        raise ValueError(
            "unknown automated event scenario(s): " + ", ".join(unknown)
        )
    selected = [
        scenario for scenario in scenarios
        if scenario.name in ctx.selected_scenario_names
    ]
    # Player-driven scenarios need the initial protocol connection. Include
    # it automatically so a targeted run remains independently runnable.
    if any(scenario.requires_player for scenario in selected):
        connect = next(
            (scenario for scenario in scenarios if scenario.name == "player-connect"),
            None,
        )
        if connect is not None and connect not in selected:
            selected.insert(0, connect)
    return selected

