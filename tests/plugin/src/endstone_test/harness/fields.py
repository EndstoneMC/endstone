"""Invariants every captured event must hold, whichever scenario captured it.

Each check in CHECKS gets the live event, its snapshot and the earlier snapshots of the same capture. The checks in
JOURNAL_CHECKS relate events of different classes: the hub journals every JOURNALED event whether or not a scenario
captures it, and each check runs over the events of one scenario when it ends.
"""

import math
from collections.abc import Callable

CHECKS: dict[str, Callable[[object, dict, list[dict]], None]] = {}
SERVER = None


def check(*names: str):
    def decorator(fn):
        for name in names:
            CHECKS[name] = fn
        return fn

    return decorator


def namespaced(value) -> bool:
    return ":" in str(value)


def xyz(location) -> tuple[float, float, float]:
    return location.x, location.y, location.z


def rotation(location) -> tuple[float, float]:
    return location.pitch, location.yaw


def real_location(location) -> None:
    assert all(math.isfinite(v) for v in xyz(location)), xyz(location)
    assert location.dimension is not None and namespaced(location.dimension.id), (
        location
    )


def hand(event) -> None:
    assert event.hand is not None and event.hand.name in ("HAND", "OFF_HAND"), (
        event.hand
    )


# =============================================================================
# Actor events
# =============================================================================


@check("ActorSpawnEvent", "ActorRemoveEvent")
def _actor_type(event, fields, history):
    assert namespaced(event.actor.type), event.actor.type


@check("ActorDamageEvent")
def _damage(event, fields, history):
    assert event.damage >= 0, event.damage
    assert event.damage_source.type


@check("ActorDeathEvent")
def _death(event, fields, history):
    assert namespaced(event.actor.type)
    assert event.damage_source.type


@check("ActorKnockbackEvent")
def _knockback(event, fields, history):
    assert len(fields["knockback"]) == 3, fields["knockback"]


@check("ActorTeleportEvent")
def _teleport(event, fields, history):
    assert xyz(event.from_location) != xyz(event.to_location)


@check("ActorExplodeEvent", "BlockExplodeEvent")
def _explode(event, fields, history):
    assert isinstance(fields["block_list"], list), fields["block_list"]


@check("ActorChangeBlockEvent")
def _change_block(event, fields, history):
    assert namespaced(event.actor.type)
    assert namespaced(event.block.type)


@check("ActorCollideWithActorEvent")
def _collide(event, fields, history):
    actors = event.actors
    assert len(actors) == 2
    assert actors[0].runtime_id != actors[1].runtime_id
    assert all(namespaced(actor.type) for actor in actors)


@check("ActorDismountEvent")
def _dismount(event, fields, history):
    assert namespaced(event.dismounted.type)
    assert event.actor.runtime_id != event.dismounted.runtime_id


@check("ActorEffectEvent")
def _effect(event, fields, history):
    effect = event.new_effect
    assert namespaced(effect.type)
    assert str(effect.type) == str(event.modified_type)
    assert (event.old_effect is None) == (event.action.name == "ADDED")
    assert effect.amplifier >= 0
    assert (effect.duration == __import__('endstone.potion', fromlist=['Effect']).Effect.INFINITE_DURATION) == effect.infinite
    assert event.action.name in ("ADDED", "CHANGED")


@check("ActorPickupItemEvent")
def _pickup(event, fields, history):
    assert namespaced(event.item.item_stack.type)
    assert 0 <= event.remaining <= event.item.item_stack.amount
    assert str(event.actor.type) != "minecraft:player", (
        "players pick up through PlayerPickupItemEvent"
    )


def _toggle(state: str, actor_state: str):
    def run(event, fields, history):
        assert isinstance(fields[state], bool)
        assert getattr(event.actor, actor_state) is not fields[state], (
            "fired after the state changed"
        )
        mine = [
            h[state]
            for h in history
            if h["actor"]["runtime_id"] == fields["actor"]["runtime_id"]
        ]
        assert not mine or mine[-1] is not fields[state], (
            "fired twice for the same state"
        )

    return run


check("ActorToggleGlideEvent")(_toggle("is_gliding", "is_gliding"))
check("ActorToggleSwimEvent")(_toggle("is_swimming", "is_swimming"))


# =============================================================================
# Block events
# =============================================================================


@check("BlockBreakEvent")
def _break(event, fields, history):
    assert event.player.name
    assert namespaced(event.block.type)


@check("BlockPlaceEvent")
def _place(event, fields, history):
    assert event.player.name
    assert namespaced(event.block_placed.type)
    assert namespaced(event.block_replaced_state.type)
    assert namespaced(event.block_against.type)
    assert str(event.block.type) == str(event.block_placed.type), (
        "block is not the placed block"
    )


@check("BlockFromToEvent")
def _from_to(event, fields, history):
    assert namespaced(event.block.type)
    assert namespaced(event.to_block.type)


COOKING_BLOCKS = {
    "minecraft:campfire": "campfire",
    "minecraft:soul_campfire": "soul_campfire",
    "minecraft:furnace": "furnace",
    "minecraft:lit_furnace": "furnace",
    "minecraft:blast_furnace": "blast_furnace",
    "minecraft:lit_blast_furnace": "blast_furnace",
    "minecraft:smoker": "smoker",
    "minecraft:lit_smoker": "smoker",
}


@check("BlockCookEvent")
def _cook(event, fields, history):
    station = COOKING_BLOCKS.get(str(event.block.type))
    assert station is not None, f"cooked in {event.block.type}"
    assert namespaced(event.source.type)
    assert namespaced(event.result.type)
    recipe = event.recipe
    if recipe is not None:
        assert recipe.id, "a cooking recipe without an id"
        assert recipe.tag == station, (recipe.tag, station)
        assert recipe.input_choice.test(event.source)
        assert str(recipe.result.type) == str(event.result.type)


@check("BlockFormEvent")
def _form(event, fields, history):
    assert "lava" in str(event.block.type), (
        "BlockFormEvent should name the lava, not what it becomes"
    )
    assert str(event.new_state.type) in {
        "minecraft:basalt",
        "minecraft:cobblestone",
        "minecraft:obsidian",
    }


@check("LeavesDecayEvent")
def _decay(event, fields, history):
    assert "leaves" in str(event.block.type)


@check("BlockPistonExtendEvent", "BlockPistonRetractEvent")
def _piston(event, fields, history):
    assert "piston" in str(event.block.type)
    assert event.direction is not None


@check("CauldronLevelChangeEvent")
def _cauldron(event, fields, history):
    assert "cauldron" in str(event.block.type)


# =============================================================================
# Level, weather and server events
# =============================================================================


@check("ChunkLoadEvent", "ChunkUnloadEvent")
def _chunk(event, fields, history):
    assert namespaced(event.dimension.id)


@check("DimensionLoadEvent")
def _dimension(event, fields, history):
    assert namespaced(event.dimension.id)


@check("WeatherChangeEvent")
def _weather(event, fields, history):
    assert isinstance(event.to_weather_state, bool)
    assert event.level.name


@check("ThunderChangeEvent")
def _thunder(event, fields, history):
    assert isinstance(event.to_thunder_state, bool)
    assert event.level.name


@check("ServerLoadEvent")
def _server_load(event, fields, history):
    assert event.type.name in ("STARTUP", "RELOAD")


@check("ServerCommandEvent")
def _server_command(event, fields, history):
    assert event.sender.name
    assert event.command


@check("PluginEnableEvent", "PluginDisableEvent")
def _plugin(event, fields, history):
    assert event.plugin.name


@check("ScriptMessageEvent")
def _script_message(event, fields, history):
    assert event.message_id
    assert event.sender.name


@check("BroadcastMessageEvent")
def _broadcast(event, fields, history):
    assert isinstance(event.is_asynchronous, bool)


@check("ServerListPingEvent")
def _ping(event, fields, history):
    assert event.max_players > 0
    assert event.network_protocol_version == SERVER.protocol_version


@check("MapInitializeEvent")
def _map(event, fields, history):
    assert event.map.id != -1
    assert event.map.scale is not None


@check("PacketSendEvent", "PacketReceiveEvent")
def _packet(event, fields, history):
    assert 0 <= event.packet_id < 1024, event.packet_id
    assert isinstance(event.payload, bytes) and len(event.payload) >= 0
    assert 0 <= event.sub_client_id <= 3


# =============================================================================
# Player events
# =============================================================================


@check("PlayerLoginEvent")
def _login(event, fields, history):
    skin = event.player.skin
    assert event.player.name
    assert skin.id
    assert len(skin.image.shape) == 3 and skin.image.shape[2] == 4


@check("PlayerJoinEvent")
def _join(event, fields, history):
    player = event.player
    assert player.name and str(player.unique_id)
    assert player.runtime_id > 0
    assert player.ping >= 0
    assert isinstance(player.is_op, bool)


@check("PlayerQuitEvent")
def _quit(event, fields, history):
    assert event.player.name


@check("PlayerKickEvent")
def _kick(event, fields, history):
    assert event.player.name, "a kick that names no player"
    assert isinstance(event.reason, str)


@check("PlayerMoveEvent")
def _move(event, fields, history):
    moved = xyz(event.from_location) != xyz(event.to_location)
    assert moved or rotation(event.from_location) != rotation(event.to_location), (
        "fired without moving or turning"
    )


@check("PlayerJumpEvent")
def _jump(event, fields, history):
    assert event.to_location.y > event.from_location.y


@check("PlayerTeleportEvent")
def _player_teleport(event, fields, history):
    real_location(event.from_location)
    real_location(event.to_location)


@check("PlayerPortalEvent")
def _portal(event, fields, history):
    assert xyz(event.from_location) != xyz(event.to_location)


@check("PlayerDimensionChangeEvent")
def _dimension_change(event, fields, history):
    assert str(event.from_dimension.id) != str(event.to_dimension.id)
    assert namespaced(event.from_dimension.id)


def _player_toggle(state: str):
    def run(event, fields, history):
        assert isinstance(fields[state], bool)
        mine = [h[state] for h in history if h["player"] == fields["player"]]
        assert not mine or mine[-1] is not fields[state], (
            "fired twice for the same state"
        )

    return run


check("PlayerToggleSneakEvent")(_player_toggle("is_sneaking"))
check("PlayerToggleSprintEvent")(_player_toggle("is_sprinting"))


@check("PlayerToggleCrawlEvent")
def _crawl(event, fields, history):
    assert event.player.is_crawling is not event.is_crawling, (
        "fired after the state changed"
    )


@check("PlayerToggleFlightEvent")
def _flight(event, fields, history):
    assert event.player.is_flying is not event.is_flying, (
        "fired after the state changed"
    )
    assert event.player.allow_flight, "fired while flight is not allowed"


INPUT_KEYS = (
    "is_forward",
    "is_backward",
    "is_left",
    "is_right",
    "is_jump",
    "is_sneak",
    "is_sprint",
)


@check("PlayerInputEvent")
def _input(event, fields, history):
    state = [getattr(event.input, key) for key in INPUT_KEYS]
    assert all(isinstance(v, bool) for v in state)
    fields["input_state"] = state
    mine = [h for h in history if h["player"] == fields["player"]]
    if mine:
        assert mine[-1]["input_state"] != state, "fired for an unchanged input"


@check("PlayerInteractEvent")
def _interact(event, fields, history):
    assert "CLICK" in event.action.name
    assert (event.block is not None) == event.has_block


@check("PlayerInteractActorEvent")
def _interact_actor(event, fields, history):
    assert namespaced(event.actor.type)


@check("PlayerItemHeldEvent")
def _held(event, fields, history):
    assert 0 <= event.new_slot <= 8 and 0 <= event.previous_slot <= 8


@check("PlayerDropItemEvent")
def _drop(event, fields, history):
    assert namespaced(event.item.type)


@check("PlayerItemConsumeEvent")
def _consume(event, fields, history):
    assert namespaced(event.item.type)
    hand(event)


@check("PlayerPickupItemEvent")
def _pickup_item(event, fields, history):
    assert namespaced(event.item.item_stack.type)


@check("PlayerChatEvent")
def _chat(event, fields, history):
    from endstone.event import Cancellable

    assert isinstance(event, Cancellable)
    assert event.message
    assert len(event.recipients) >= 1


@check("PlayerCommandEvent")
def _command(event, fields, history):
    assert event.command


@check("PlayerEmoteEvent")
def _emote(event, fields, history):
    assert event.emote_id


@check("PlayerArmSwingEvent")
def _swing(event, fields, history):
    held = event.player.inventory.item_in_main_hand
    assert (event.item is not None) == (held is not None), (event.item, held)
    if event.item is not None:
        assert (
            namespaced(event.item.type) and str(event.item.type) != "minecraft:air"
        ), event.item


@check("PlayerArmorStandManipulateEvent")
def _armor_stand(event, fields, history):
    assert str(event.actor.type) == "minecraft:armor_stand"
    assert event.slot is not None
    assert namespaced(event.armor_stand_item.type) and namespaced(
        event.player_item.type
    )
    items = {str(event.armor_stand_item.type), str(event.player_item.type)}
    assert items != {"minecraft:air"}, "nothing to swap"


@check("PlayerBucketActorEvent")
def _bucket_actor(event, fields, history):
    assert namespaced(event.actor.type)
    assert "bucket" in str(event.original_bucket.type)
    hand(event)


@check("PlayerShearActorEvent")
def _shear(event, fields, history):
    assert namespaced(event.actor.type)
    assert str(event.item.type) == "minecraft:shears"
    hand(event)


@check("PlayerRiptideEvent")
def _riptide(event, fields, history):
    assert "trident" in str(event.item.type)


@check("PlayerPickupArrowEvent")
def _pickup_arrow(event, fields, history):
    kind = str(event.arrow.type)
    assert "arrow" in kind or "trident" in kind


@check("PlayerRecipeBookSettingsChangeEvent")
def _recipe_book(event, fields, history):
    assert isinstance(event.is_open, bool) and isinstance(event.is_filtering, bool)
    assert event.recipe_book_type.name == "CRAFTING", (
        "only the crafting book has a fire site"
    )


@check("PlayerBedEnterEvent")
def _bed_enter(event, fields, history):
    assert "bed" in str(event.bed.type)


@check("PlayerBedLeaveEvent")
def _bed_leave(event, fields, history):
    assert "bed" in str(event.bed.type) or str(event.bed.type) == "minecraft:air"


@check("PlayerDeathEvent")
def _player_death(event, fields, history):
    assert event.player.name
    assert event.damage_source.type


@check("PlayerRespawnEvent")
def _respawn(event, fields, history):
    assert event.respawn_reason.name in ("DEATH", "END_PORTAL")


@check("PlayerGameModeChangeEvent")
def _game_mode(event, fields, history):
    assert event.new_game_mode is not None


@check("PlayerExpChangeEvent")
def _exp(event, fields, history):
    assert isinstance(event.amount, int)


@check("PlayerPickupExperienceEvent")
def _pickup_exp(event, fields, history):
    assert event.amount > 0


@check("PlayerLevelChangeEvent")
def _level(event, fields, history):
    assert event.old_level != event.new_level
    assert event.new_level >= 0


@check("PlayerSkinChangeEvent")
def _skin(event, fields, history):
    assert event.new_skin.id


# =============================================================================
# Checks across event classes, over the journal of one scenario
# =============================================================================

JOURNALED = ("ActorDeathEvent", "PlayerDeathEvent", "PlayerRespawnEvent")
JOURNAL_CHECKS: list[Callable[[list[dict], list[dict]], None]] = []


def journal_check(fn):
    JOURNAL_CHECKS.append(fn)
    return fn


def player_of(entry: dict) -> object:
    fields = entry["fields"]
    who = fields.get("player") or fields.get("actor") or {}
    return who.get("runtime_id")


@journal_check
def _player_death_is_an_actor_death(scenario: list[dict], journal: list[dict]) -> None:
    actor_deaths = {
        player_of(e)
        for e in scenario
        if e["event"] == "ActorDeathEvent"
        and e["fields"]["actor"]["type"] == "minecraft:player"
    }
    for entry in scenario:
        if entry["event"] == "PlayerDeathEvent":
            assert player_of(entry) in actor_deaths, (
                f"{entry['fields']['player']['name']} died without an ActorDeathEvent for minecraft:player"
            )


@journal_check
def _death_respawn_follows_a_death(scenario: list[dict], journal: list[dict]) -> None:
    for entry in scenario:
        if (
            entry["event"] != "PlayerRespawnEvent"
            or entry["fields"]["respawn_reason"] != "DEATH"
        ):
            continue
        before = journal[: entry["seq"]]
        player = player_of(entry)
        last_death = max(
            (
                e["seq"]
                for e in before
                if e["event"] == "PlayerDeathEvent" and player_of(e) == player
            ),
            default=-1,
        )
        last_respawn = max(
            (
                e["seq"]
                for e in before
                if e["event"] == "PlayerRespawnEvent" and player_of(e) == player
            ),
            default=-1,
        )
        assert last_death > last_respawn, (
            f"{entry['fields']['player']['name']} respawned with reason DEATH without dying first"
        )
