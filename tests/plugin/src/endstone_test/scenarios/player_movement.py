import math

from endstone.enchantments import Enchantment
from endstone.event import (
    ActorSpawnEvent,
    ActorToggleGlideEvent,
    ActorToggleSwimEvent,
    PlayerArmSwingEvent,
    PlayerInputEvent,
    PlayerItemHeldEvent,
    PlayerJumpEvent,
    PlayerMoveEvent,
    PlayerPortalEvent,
    PlayerRiptideEvent,
    PlayerToggleCrawlEvent,
    PlayerToggleFlightEvent,
    PlayerToggleSneakEvent,
    PlayerToggleSprintEvent,
)
from endstone.inventory import ItemStack
from endstone.level import Location

from endstone_test.harness import Context, scenario

BUDGET = 900


async def poll(ctx: Context, read, ok, timeout: int = 200, what: str = "a condition"):
    for _ in range(timeout):
        value = await read()
        if ok(value):
            return value
        await ctx.ticks(1)
    raise TimeoutError(f"timed out after {timeout} polls waiting for {what}")


def where(player):
    loc = player.location
    return loc.x, loc.y, loc.z


def vec(v) -> list[float]:
    return [v["x"], v["y"], v["z"]]


def flat(a, b) -> float:
    return math.hypot(a[0] - b[0], a[2] - b[2])


async def bot_position(bot) -> list[float]:
    return await bot.call("local_player.get_position")


async def packets(bot, name: str, since: int) -> list[dict]:
    return await bot.call("get_received_packets", name, since=since)


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def join(ctx: Context, name: str, dx: int = 8, dz: int = 8, mode: str = "survival"):
    bot = await ctx.bot(name)
    player = ctx.server.get_player(name)
    assert player is not None, f"{name} is not online"
    ctx.run(f"gamemode {mode} {name}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    spot = (x + 0.5, y, z + 0.5)
    ctx.run(f"tp {name} {spot[0]} {spot[1]} {spot[2]}")
    await ctx.until(
        lambda: flat(where(player), spot) < 0.1,
        timeout=60,
        what=f"{name} to arrive in the plot",
    )
    await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: flat(p, spot) < 0.1,
        what=f"{name}'s client to see the teleport",
    )
    await ctx.ticks(10)
    return bot, player, spot


def mine(name: str):
    return lambda e: e.player.name == name


def peak_of(ctx: Context, name: str, start_y: float) -> list[float]:
    peak = [start_y]

    def track(event) -> bool:
        peak[0] = max(peak[0], event.player.location.y)
        return False

    ctx.capture(PlayerMoveEvent, track)
    return peak


@scenario("movement.move.normal", covers={PlayerMoveEvent: "normal"}, plot=True, budget=BUDGET)
async def move_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvwalk", dx=4)
    moves = ctx.capture(PlayerMoveEvent, mine("mvwalk"))
    corrections = await bot.call("local_player.get_movement_correction_count")
    goal = (start[0] + 6, start[1], start[2])
    await bot.call("local_player.move_to", list(goal))
    await poll(ctx, lambda: bot.call("local_player.is_moving"), lambda m: m is False, what="the bot to arrive")
    await ctx.ticks(5)
    assert moves.count >= 3, moves.events
    first, last = moves.events[0], moves.events[-1]
    assert abs(first["from_location"]["x"] - start[0]) < 0.05, first["from_location"]
    xs = [e["to_location"]["x"] for e in moves.events]
    assert xs == sorted(xs) and xs[-1] > xs[0], xs
    assert all(abs(e["to_location"]["z"] - start[2]) < 0.05 for e in moves.events)
    assert all(e["from_location"]["x"] <= e["to_location"]["x"] for e in moves.events)
    assert abs(last["to_location"]["x"] - where(player)[0]) < 0.2, (last["to_location"], where(player))
    assert flat(where(player), goal) < 0.1, (where(player), goal)
    seen = await bot_position(bot)
    assert flat(seen, where(player)) < 0.05, (seen, where(player))
    assert await bot.call("local_player.get_movement_correction_count") == corrections


@scenario("movement.move.cancel", covers={PlayerMoveEvent: "cancel"}, plot=True, budget=BUDGET)
async def move_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvblock", dx=3)
    moves = ctx.capture(PlayerMoveEvent, mine("mvblock"), cancel=True)
    index = await mark(bot)
    await bot.call("local_player.move_to", [start[0] + 8, start[1], start[2]])
    await ctx.until(lambda: moves.count >= 5, timeout=100, what="cancelled moves")
    await ctx.ticks(20)
    assert flat(where(player), start) < 0.01, (where(player), start)
    fixes = await packets(bot, "CorrectPlayerMovePredictionPacket", index)
    ctx.info["corrections"] = len(fixes)
    assert fixes, "the client was never told its move was refused"
    eye = start[1] + 1.62
    assert all(
        abs(vec(f["packet"]["pos"])[0] - start[0]) < 0.01 and abs(vec(f["packet"]["pos"])[1] - eye) < 0.01
        for f in fixes
    ), fixes[0]
    seen = await bot_position(bot)
    assert seen[0] - start[0] < 1.5, f"the client walked on to {seen}"
    await bot.call("local_player.stop_moving")


@scenario("movement.move.mutate", covers={PlayerMoveEvent: "mutate"}, plot=True, budget=BUDGET)
async def move_mutate(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvwarp", dx=3, dz=3)
    x, y, z = ctx.plot.pos(12, 0, 12)
    target = (x + 0.5, y, z + 0.5)
    done = []

    def elsewhere(event) -> None:
        if not done:
            done.append(1)
            event.to_location = Location(ctx.overworld, *target)

    moves = ctx.capture(PlayerMoveEvent, mine("mvwarp"), mutate=elsewhere)
    index = await mark(bot)
    await bot.call("local_player.move_to", [start[0] + 6, start[1], start[2]])
    await ctx.until(lambda: moves.count >= 1, timeout=100, what="PlayerMoveEvent")
    await ctx.until(
        lambda: (
            flat(where(player), target) < 0.3
            and abs(where(player)[1] - target[1]) < 0.1
        ),
        timeout=40,
        what="the player to reach the mutated to_location",
    )
    eye = target[1] + 1.62
    moved = await packets(bot, "MovePlayerPacket", index)
    assert any(
        flat(vec(p["packet"]["pos"]), target) < 0.5
        and abs(vec(p["packet"]["pos"])[1] - eye) < 0.1
        for p in moved
    ), moved


@scenario("movement.jump.normal", covers={PlayerJumpEvent: "normal"}, plot=True, budget=BUDGET)
async def jump_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvjump")
    jumps = ctx.capture(PlayerJumpEvent, mine("mvjump"))
    peak = peak_of(ctx, "mvjump", start[1])
    await bot.call("local_player.jump")
    await ctx.until(lambda: jumps.count, timeout=40, what="PlayerJumpEvent")
    await ctx.ticks(30)
    assert jumps.count == 1, jumps.events
    event = jumps.events[0]
    assert abs(event["from_location"]["y"] - start[1]) < 0.01, event["from_location"]
    assert 0.3 < event["to_location"]["y"] - start[1] < 0.6, event["to_location"]
    assert peak[0] - start[1] > 1.0, f"the server never saw the player above {peak[0]}"
    seen = await bot_position(bot)
    assert abs(seen[1] - where(player)[1]) < 0.05, (seen, where(player))
    assert abs(where(player)[1] - start[1]) < 0.01, "the player did not land again"


@scenario("movement.jump.cancel", covers={PlayerJumpEvent: "cancel"}, plot=True, budget=BUDGET)
async def jump_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvnojump")
    jumps = ctx.capture(PlayerJumpEvent, mine("mvnojump"), cancel=True)
    peak = peak_of(ctx, "mvnojump", start[1])
    index = await mark(bot)
    await bot.call("local_player.jump")
    await ctx.until(lambda: jumps.count, timeout=40, what="PlayerJumpEvent")
    await ctx.ticks(30)
    assert peak[0] - start[1] < 0.1, f"a cancelled jump still lifted the player to {peak[0]}"
    assert abs(where(player)[1] - start[1]) < 0.01
    await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: abs(p[1] - start[1]) < 0.05,
        what="the client to be put back on the floor",
    )
    assert await packets(bot, "MovePlayerPacket", index), "the client was not sent back to the start of the jump"


@scenario("movement.jump.mutate", covers={PlayerJumpEvent: "mutate"}, plot=True, budget=BUDGET)
async def jump_mutate(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvjumpto")
    x, y, z = ctx.plot.pos(2, 0, 2)
    away = Location(ctx.overworld, x + 0.5, y, z + 0.5, 0.0, 90.0)
    refused = []

    def back_elsewhere(e) -> None:
        try:
            e.to_location = away
        except AttributeError:
            refused.append(True)
        e.from_location = away
        e.cancel()

    jumps = ctx.capture(PlayerJumpEvent, mine("mvjumpto"), mutate=back_elsewhere)
    await bot.call("local_player.jump")
    await ctx.until(lambda: jumps.count, timeout=40, what="PlayerJumpEvent")
    await ctx.ticks(30)
    assert refused == [True], "PlayerJumpEvent.to_location accepted a write"
    assert jumps.events[0]["after"]["from_location"]["x"] == away.x
    assert flat(where(player), (away.x, away.y, away.z)) < 0.3, (
        f"the cancelled jump did not return the player to the from_location the listener wrote: {where(player)}"
    )
    assert abs(where(player)[1] - away.y) < 0.05, where(player)
    assert abs(player.location.yaw - 90.0) < 1.0, player.location.yaw
    await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: flat(p, (away.x, away.y, away.z)) < 0.3,
        what="the client to be moved to from_location",
    )


@scenario("movement.input.normal", covers={PlayerInputEvent: "normal"}, plot=True, budget=BUDGET)
async def input_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvinput")
    inputs = ctx.capture(PlayerInputEvent, mine("mvinput"))
    keys = ("forward", "backward", "left", "right", "jump", "sneak", "sprint")

    async def step(label: str, method: str, want: set[str], *args) -> None:
        before = inputs.count
        await bot.call(method, *args)
        await ctx.until(lambda: inputs.count > before, timeout=60, what=f"PlayerInputEvent for {label}")
        await ctx.ticks(6)
        assert inputs.count == before + 1, f"{label}: {inputs.events[before:]}"
        got = {k for k, v in zip(keys, inputs.events[before]["input_state"]) if v}
        assert got == want, f"{label}: expected {want}, the server reported {got}"

    await step("forward", "local_player.set_move_vector", {"forward"}, 0.0, 1.0)
    await step("release", "local_player.stop_moving", set())
    await step("left", "local_player.set_move_vector", {"left"}, 1.0, 0.0)
    await step("right", "local_player.set_move_vector", {"right"}, -1.0, 0.0)
    await step("backward", "local_player.set_move_vector", {"backward"}, 0.0, -1.0)
    await step("release again", "local_player.stop_moving", set())
    await step("sneak", "local_player.set_sneaking", {"sneak"}, True)
    await step("unsneak", "local_player.set_sneaking", set(), False)
    await step("forward for sprint", "local_player.set_move_vector", {"forward"}, 0.0, 1.0)
    await step("sprint", "local_player.set_sprinting", {"forward", "sprint"}, True)
    await step("stop", "local_player.stop_moving", {"sprint"})
    await step("release sprint", "local_player.set_sprinting", set(), False)


def before(attr: str):
    return lambda e: {"before": getattr(e.player, attr)}


@scenario("movement.toggle.sneak", covers={PlayerToggleSneakEvent: "normal"}, plot=True, budget=BUDGET)
async def toggle_sneak(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvsneak")
    toggles = ctx.capture(PlayerToggleSneakEvent, mine("mvsneak"), extra=before("is_sneaking"))
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the sneak toggle")
    await ctx.ticks(5)
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_sneaking"] is True and toggles.events[0]["before"] is False
    assert player.is_sneaking
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: toggles.count == 2, timeout=40, what="the unsneak toggle")
    await ctx.ticks(5)
    assert toggles.count == 2, toggles.events
    assert toggles.events[1]["is_sneaking"] is False and toggles.events[1]["before"] is True
    assert not player.is_sneaking


@scenario("movement.toggle.sprint", covers={PlayerToggleSprintEvent: "normal"}, plot=True, budget=BUDGET)
async def toggle_sprint(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvsprint", dx=2)
    toggles = ctx.capture(PlayerToggleSprintEvent, mine("mvsprint"), extra=before("is_sprinting"))
    await bot.call("local_player.set_sprinting", True)
    await bot.call("local_player.set_move_vector", 0.0, 1.0)
    await ctx.until(lambda: toggles.count, timeout=40, what="the sprint toggle")
    await ctx.ticks(3)
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_sprinting"] is True and toggles.events[0]["before"] is False
    assert player.is_sprinting
    await bot.call("local_player.stop_moving")
    await ctx.until(lambda: toggles.count == 2, timeout=40, what="the sprint to stop")
    await ctx.ticks(3)
    assert toggles.count == 2, toggles.events
    assert toggles.events[1]["is_sprinting"] is False and toggles.events[1]["before"] is True
    assert not player.is_sprinting


@scenario("movement.toggle.flight", covers={PlayerToggleFlightEvent: "normal"}, plot=True, budget=BUDGET)
async def toggle_flight(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvfly", mode="creative")
    assert player.allow_flight
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvfly"), extra=before("is_flying"))
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the flight toggle")
    await ctx.ticks(5)
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_flying"] is True and toggles.events[0]["before"] is False


@scenario("movement.toggle.flight_denied", plot=True, budget=BUDGET)
async def toggle_flight_denied(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvnofly")
    assert not player.allow_flight
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvnofly"))
    await bot.call("local_player.set_flying", True)
    await ctx.ticks(20)
    assert toggles.count == 0, f"a player who may not fly raised {toggles.events}"
    assert not player.is_flying


@scenario("movement.toggle.flight_ability", covers={PlayerToggleFlightEvent: "normal"}, plot=True, budget=BUDGET)
async def toggle_flight_ability(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvflyab")
    player.allow_flight = True
    await ctx.ticks(10)
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvflyab"), extra=before("is_flying"))
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the flight toggle")
    await ctx.ticks(5)
    assert toggles.events[0]["is_flying"] is True and toggles.events[0]["before"] is False


async def water_box(ctx: Context) -> None:
    x, y, z = ctx.plot.pos(5, 0, 5)
    ctx.run(f"fill {x} {y} {z} {x + 6} {y + 4} {z + 6} glass")
    ctx.run(f"fill {x + 1} {y} {z + 1} {x + 5} {y + 3} {z + 5} water")
    await ctx.ticks(10)


def before_actor(attr: str):
    return lambda e: {"before": getattr(e.actor, attr)}


@scenario("movement.toggle.crawl", covers={PlayerToggleCrawlEvent: "normal"}, plot=True, budget=BUDGET)
async def toggle_crawl(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvcrawl", dx=3)
    toggles = ctx.capture(PlayerToggleCrawlEvent, mine("mvcrawl"), extra=before("is_crawling"))
    ctx.run("replaceitem entity mvcrawl slot.armor.chest 0 elytra")
    cx, cy, cz = (math.floor(v) for v in start)
    ctx.run(f"setblock {cx - 1} {cy} {cz} stone")
    ctx.run(f"fill {cx + 1} {cy + 1} {cz} {cx + 4} {cy + 1} {cz} stone")
    await ctx.ticks(10)
    ctx.run(f"tp mvcrawl {cx - 0.5} {cy + 1} {cz + 0.5}")
    await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: (
            flat(p, (cx - 0.5, 0.0, cz + 0.5)) < 0.1 and abs(p[1] - (cy + 1)) < 0.05
        ),
        what="the client to see the teleport onto the ledge",
    )
    await ctx.ticks(10)
    await bot.call("local_player.set_rotation", 0.0, -90.0)
    await bot.call("local_player.set_move_vector", 0.0, 1.0)
    await poll(
        ctx,
        lambda: bot_position(bot),
        lambda p: p[1] < cy + 1 - 0.01,
        what="the bot to step off the ledge",
    )
    await bot.call("local_player.set_gliding", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the crawl toggle")
    await bot.call("local_player.stop_moving")
    await ctx.ticks(10)
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_crawling"] is True and toggles.events[0]["before"] is False
    ctx.info["crawling_after"] = player.is_crawling


@scenario("movement.swim.normal", covers={ActorToggleSwimEvent: "normal"}, plot=True, budget=BUDGET)
async def swim_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvswim", dx=6, dz=8)
    await water_box(ctx)
    toggles = ctx.capture(ActorToggleSwimEvent, lambda e: e.actor.name == "mvswim", extra=before_actor("is_swimming"))
    await bot.call("local_player.set_swimming", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the swim toggle")
    assert toggles.events[0]["is_swimming"] is True and toggles.events[0]["before"] is False
    await bot.call("local_player.set_sprinting", True)
    await bot.call("local_player.set_move_vector", 0.0, 1.0)
    await ctx.until(lambda: player.is_swimming, timeout=60, what="the server to put the sprinting player in water to swim")
    await bot.call("local_player.set_swimming", False)
    await ctx.until(lambda: toggles.count == 2, timeout=40, what="the swim to stop")
    assert toggles.events[1]["is_swimming"] is False and toggles.events[1]["before"] is True
    await bot.call("local_player.stop_moving")


@scenario("movement.glide.normal", covers={ActorToggleGlideEvent: "normal"}, plot=True, budget=BUDGET)
async def glide_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvglide")
    ctx.run("replaceitem entity mvglide slot.armor.chest 0 elytra")
    x, y, z = ctx.plot.pos(8, 0, 8)
    ctx.run(f"tp mvglide {x + 0.5} {y + 15} {z + 0.5}")
    await ctx.until(lambda: not player.is_on_ground, timeout=20, what="the player to leave the ground")
    toggles = ctx.capture(ActorToggleGlideEvent, lambda e: e.actor.name == "mvglide", extra=before_actor("is_gliding"))
    await bot.call("local_player.set_gliding", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the glide toggle")
    await ctx.ticks(10)
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_gliding"] is True and toggles.events[0]["before"] is False
    ctx.info["gliding_after"] = player.is_gliding


@scenario("movement.riptide.normal", covers={PlayerRiptideEvent: "normal"}, plot=True, budget=BUDGET)
async def riptide_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvtrident")
    await water_box(ctx)
    trident = ItemStack("minecraft:trident")
    meta = trident.item_meta
    meta.add_enchant(Enchantment.RIPTIDE, 1)
    trident.set_item_meta(meta)
    player.inventory.item_in_main_hand = trident
    await poll(
        ctx,
        lambda: bot.call("local_player.get_inventory_json"),
        lambda s: s["inventory"]["slots"][s["inventory"]["selected_slot"]]["id"] == "minecraft:trident",
        what="the client to hold the riptide trident, which it must send back unchanged",
    )
    await ctx.until(
        lambda: player.is_in_water,
        timeout=60,
        what="the server to see the player in water, without which it refuses to start a riptide",
    )
    riptide = ctx.capture(PlayerRiptideEvent, mine("mvtrident"))
    thrown = ctx.capture(ActorSpawnEvent, lambda e: "trident" in str(e.actor.type))
    index = await mark(bot)
    await bot.call("game_mode.use_item")
    await ctx.ticks(30)
    resent = await packets(bot, "InventoryContentPacket", index)
    assert not resent, "the server rejected the trident use and resent the inventory"
    origin = where(player)
    await bot.call("game_mode.release_using_item")
    moved = 0.0
    for _ in range(30):
        await ctx.ticks(1)
        moved = max(moved, math.dist(origin, where(player)))
    ctx.info["moved"] = round(moved, 2)
    assert thrown.count == 0, "the riptide trident was thrown instead of propelling the player"
    assert player.inventory.item_in_main_hand is not None, "the trident left the player's hand"
    assert riptide.count == 1, f"PlayerRiptideEvent fired {riptide.count} times for one launch"
    assert riptide.events[0]["item"]["type"] == "minecraft:trident"
    assert moved > 1.0, f"the riptide launch moved the player only {moved:.2f} blocks"


@scenario("movement.swing.normal", covers={PlayerArmSwingEvent: "normal"}, plot=True, budget=BUDGET)
async def swing_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvswinger", dx=6)
    watcher, seen_by, _ = await join(ctx, "mvwatcher", dx=10)
    ctx.run("replaceitem entity mvswinger slot.weapon.mainhand 0 diamond_sword")
    await ctx.ticks(10)
    swings = ctx.capture(PlayerArmSwingEvent, mine("mvswinger"))
    index = await mark(watcher)
    await bot.call("local_player.swing")
    await ctx.until(lambda: swings.count, timeout=40, what="PlayerArmSwingEvent")
    assert swings.events[0]["item"]["type"] == "minecraft:diamond_sword", swings.events[0]
    await poll(
        ctx,
        lambda: packets(watcher, "AnimatePacket", index),
        lambda found: found,
        what="the second player to be shown the swing",
    )
    shown = await packets(watcher, "AnimatePacket", index)
    assert len(shown) == 1 and shown[0]["packet"]["runtime_id"] == player.runtime_id, shown
    assert shown[0]["packet"]["action"] == "SWING", shown
    assert swings.count == 1


@scenario("movement.swing.cancel", covers={PlayerArmSwingEvent: "cancel"}, plot=True, budget=BUDGET)
async def swing_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvswinger", dx=6)
    watcher, seen_by, _ = await join(ctx, "mvwatcher", dx=10)
    gate = {"on": False}
    swings = ctx.capture(
        PlayerArmSwingEvent,
        mine("mvswinger"),
        mutate=lambda e: e.cancel() if gate["on"] else None,
    )
    index = await mark(watcher)
    await bot.call("local_player.swing")
    await poll(
        ctx,
        lambda: packets(watcher, "AnimatePacket", index),
        lambda found: found,
        what="the control swing to reach the second player",
    )
    gate["on"] = True
    index = await mark(watcher)
    await bot.call("local_player.swing")
    await ctx.until(lambda: swings.count == 2, timeout=40, what="the cancelled swing")
    await ctx.ticks(30)
    shown = await packets(watcher, "AnimatePacket", index)
    assert not shown, f"the second player was shown a cancelled swing: {shown}"


@scenario("movement.held.normal", covers={PlayerItemHeldEvent: "normal"}, plot=True, budget=BUDGET)
async def held_normal(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvheld")
    held = ctx.capture(PlayerItemHeldEvent, mine("mvheld"))
    await bot.call("local_player.select_slot", 3)
    await ctx.until(lambda: held.count, timeout=40, what="PlayerItemHeldEvent")
    await ctx.until(lambda: player.inventory.held_item_slot == 3, timeout=20, what="the server to hold slot 3")
    assert (held.events[0]["previous_slot"], held.events[0]["new_slot"]) == (0, 3), held.events
    await bot.call("local_player.select_slot", 3)
    await ctx.ticks(10)
    assert held.count == 1, "selecting the slot already held raised an event"
    await bot.call("local_player.select_slot", 7)
    await ctx.until(lambda: held.count == 2, timeout=40, what="the second PlayerItemHeldEvent")
    assert (held.events[1]["previous_slot"], held.events[1]["new_slot"]) == (3, 7), held.events
    await ctx.until(lambda: player.inventory.held_item_slot == 7, timeout=20, what="the server to hold slot 7")


@scenario("movement.held.cancel", covers={PlayerItemHeldEvent: "cancel"}, plot=True, budget=BUDGET)
async def held_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvheldno")
    ctx.run("replaceitem entity mvheldno slot.hotbar 4 stone")
    await ctx.ticks(10)
    held = ctx.capture(PlayerItemHeldEvent, mine("mvheldno"), cancel=True)
    index = await mark(bot)
    await bot.call("local_player.select_slot", 4)
    await ctx.until(lambda: held.count, timeout=40, what="PlayerItemHeldEvent")
    await ctx.ticks(20)
    assert (held.events[0]["previous_slot"], held.events[0]["new_slot"]) == (0, 4), held.events
    assert player.inventory.held_item_slot == 0, "a cancelled selection still moved the server's held slot"
    resync = []
    for name in ("MobEquipmentPacket", "PlayerHotbarPacket"):
        resync += await packets(bot, name, index)
    resync.sort(key=lambda p: p["index"])
    ctx.info["resync"] = [(p["name"], p["packet"].get("selected_slot")) for p in resync]
    assert resync, "the client was never told its slot selection was refused"
    assert resync[-1]["packet"]["selected_slot"] == 0, resync[-1]


def nether_portal(ctx: Context, dx: int, dz: int) -> tuple[int, int, int]:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    ctx.run(f"fill {x} {y - 1} {z} {x} {y + 3} {z + 3} obsidian")
    ctx.run(f"fill {x} {y} {z + 1} {x} {y + 2} {z + 2} air")
    ctx.run(f'fill {x} {y} {z + 1} {x} {y + 2} {z + 2} portal ["portal_axis"="z"]')
    return x, y, z


async def walk_into_portal(ctx: Context, name: str):
    bot, player, start = await join(ctx, name, dx=5, dz=8, mode="creative")
    x, y, z = nether_portal(ctx, 9, 7)
    await ctx.ticks(10)
    return bot, player, (x, y, z)


@scenario("movement.portal.normal", covers={PlayerPortalEvent: "normal"}, plot=True, budget=2000)
async def portal_normal(ctx: Context) -> None:
    bot, player, (x, y, z) = await walk_into_portal(ctx, "mvportal")
    portals = ctx.capture(PlayerPortalEvent, mine("mvportal"))
    await bot.call("local_player.move_to", [x + 3.0, y, z + 1.5])
    await ctx.until(lambda: portals.count, timeout=300, what="PlayerPortalEvent")
    await ctx.until(
        lambda: str(player.location.dimension.id) == "minecraft:nether",
        timeout=300,
        what="the player to reach the nether",
    )
    event = portals.events[0]
    assert event["from_location"]["dimension"] == "minecraft:overworld", event
    assert event["to_location"]["dimension"] == "minecraft:nether", event
    assert abs(event["from_location"]["x"] - x) < 3 and abs(event["from_location"]["z"] - (z + 1.5)) < 3, event
    assert abs(event["to_location"]["x"] - x / 8) < 40, event
    await poll(
        ctx,
        lambda: bot.call("local_player.is_changing_dimension"),
        lambda changing: changing is False,
        timeout=300,
        what="the client to finish the dimension change",
    )


@scenario("movement.portal.cancel", covers={PlayerPortalEvent: "cancel"}, plot=True, budget=2000)
async def portal_cancel(ctx: Context) -> None:
    bot, player, (x, y, z) = await walk_into_portal(ctx, "mvportalno")
    portals = ctx.capture(PlayerPortalEvent, mine("mvportalno"), cancel=True)
    await bot.call("local_player.move_to", [x + 3.0, y, z + 1.5])
    await ctx.until(lambda: portals.count, timeout=300, what="PlayerPortalEvent")
    await ctx.ticks(80)
    assert str(player.location.dimension.id) == "minecraft:overworld", "a cancelled portal still moved the player"
    assert await bot.call("local_player.is_changing_dimension") is False


@scenario("movement.portal.mutate", covers={PlayerPortalEvent: "mutate"}, plot=True, budget=2000)
async def portal_mutate(ctx: Context) -> None:
    bot, player, (x, y, z) = await walk_into_portal(ctx, "mvportalto")
    nether = ctx.server.level.get_dimension("nether")
    target = (900.5, 70.0, 900.5)
    portals = ctx.capture(
        PlayerPortalEvent,
        mine("mvportalto"),
        mutate=lambda e: setattr(e, "to_location", Location(nether, *target)),
    )
    await bot.call("local_player.move_to", [x + 3.0, y, z + 1.5])
    await ctx.until(lambda: portals.count, timeout=300, what="PlayerPortalEvent")
    await ctx.until(
        lambda: str(player.location.dimension.id) == "minecraft:nether",
        timeout=300,
        what="the player to reach the nether",
    )
    ctx.info["arrived"] = where(player)
    assert flat(where(player), target) < 40, (
        f"the player arrived at {where(player)} instead of near the to_location the listener wrote"
    )


def sneak_flag(found: list[dict], runtime_id: int) -> bool | None:
    for entry in reversed(found):
        packet = entry["packet"]
        if packet["id"] != runtime_id:
            continue
        for item in packet["packed_items"]["data"]:
            if item["id"] in (0, "RESERVED_0"):
                return bool(item["payload"]["value"] >> 1 & 1)
    return None


def sprint_flag(found: list[dict], runtime_id: int) -> bool | None:
    for entry in reversed(found):
        packet = entry["packet"]
        if packet["id"] != runtime_id:
            continue
        for item in packet["packed_items"]["data"]:
            if item["id"] in (0, "RESERVED_0"):
                return bool(item["payload"]["value"] >> 3 & 1)
    return None


def flying_bit(found: list[dict]) -> bool | None:
    for entry in reversed(found):
        for layer in entry["packet"]["data"]["layers"]:
            if layer["serialized_layer"] == "BASE":
                return bool(layer["ability_values"] >> 9 & 1)
    return None


async def states(ctx: Context, read, n: int = 20) -> set:
    seen = set()
    for _ in range(n):
        seen.add(read())
        await ctx.ticks(1)
    return seen


@scenario("movement.toggle.sneak_cancel", covers={PlayerToggleSneakEvent: "cancel"}, plot=True, budget=BUDGET)
async def toggle_sneak_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvsnkno")
    toggles = ctx.capture(PlayerToggleSneakEvent, mine("mvsnkno"), cancel=True, extra=before("is_sneaking"))
    index = await mark(bot)
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the sneak toggle")
    held = await states(ctx, lambda: player.is_sneaking)
    assert held == {False}, f"a cancelled sneak still made the server sneak: {held}"
    assert toggles.count == 1, f"holding the sneak key fired again: {toggles.events}"
    assert toggles.events[0]["is_sneaking"] is True and toggles.events[0]["before"] is False
    found = await packets(bot, "SetActorDataPacket", index)
    ctx.info["set_actor_data_after_cancel"] = len(found)
    flag = sneak_flag(found, player.runtime_id)
    assert flag is False, f"the client was not sent SNEAKING clear after the cancel: {found[-3:]}"
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: toggles.count == 2, timeout=40, what="releasing the key to fire")
    assert toggles.events[1]["is_sneaking"] is False and toggles.events[1]["before"] is False, toggles.events
    await ctx.ticks(5)
    assert not player.is_sneaking
    ctx.release(toggles)
    again = ctx.capture(PlayerToggleSneakEvent, mine("mvsnkno"))
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: again.count, timeout=40, what="a second press")
    await ctx.until(lambda: player.is_sneaking, timeout=40, what="the server to sneak once the key was released")
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: not player.is_sneaking, timeout=40, what="the player to stand")


@scenario("movement.toggle.sneak_stop_cancel", covers={PlayerToggleSneakEvent: "cancel"}, plot=True, budget=BUDGET)
async def toggle_sneak_stop_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvsnkstay")
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: player.is_sneaking, timeout=40, what="the player to sneak")
    toggles = ctx.capture(PlayerToggleSneakEvent, mine("mvsnkstay"), cancel=True, extra=before("is_sneaking"))
    index = await mark(bot)
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: toggles.count, timeout=40, what="the unsneak toggle")
    held = await states(ctx, lambda: player.is_sneaking)
    assert held == {True}, f"a cancelled stop still made the server stand: {held}"
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_sneaking"] is False and toggles.events[0]["before"] is True
    found = await packets(bot, "SetActorDataPacket", index)
    ctx.info["set_actor_data_after_cancel"] = len(found)
    flag = sneak_flag(found, player.runtime_id)
    assert flag is True, f"the client was not sent SNEAKING set after the cancel: {found[-3:]}"
    ctx.release(toggles)
    again = ctx.capture(PlayerToggleSneakEvent, mine("mvsnkstay"), extra=before("is_sneaking"))
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: again.count, timeout=40, what="pressing sneak again to fire")
    assert again.events[0]["is_sneaking"] is True and again.events[0]["before"] is True, again.events
    await ctx.ticks(10)
    assert player.is_sneaking
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: again.count == 2, timeout=40, what="the release to fire")
    await ctx.until(lambda: not player.is_sneaking, timeout=40, what="the player to stand once the key was pressed again")


@scenario(
    "movement.toggle.sprint_cancel",
    covers={PlayerToggleSprintEvent: "cancel"},
    plot=True,
    budget=BUDGET,
)
async def toggle_sprint_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvsprno", dx=2)
    toggles = ctx.capture(
        PlayerToggleSprintEvent,
        mine("mvsprno"),
        cancel=True,
        extra=before("is_sprinting"),
    )
    index = await mark(bot)
    await bot.call("local_player.set_sprinting", True)
    await bot.call("local_player.set_move_vector", 0.0, 1.0)
    await ctx.until(lambda: toggles.count, timeout=40, what="the sprint toggle")
    held = await states(ctx, lambda: player.is_sprinting, 30)
    assert held == {False}, f"a cancelled sprint still made the server sprint: {held}"
    assert toggles.count == 1, f"holding the sprint key fired again: {toggles.events}"
    assert (
        toggles.events[0]["is_sprinting"] is True
        and toggles.events[0]["before"] is False
    )
    found = await packets(bot, "SetActorDataPacket", index)
    ctx.info["set_actor_data_after_cancel"] = len(found)
    flag = sprint_flag(found, player.runtime_id)
    assert flag is False, (
        f"the client was not sent SPRINTING clear after the cancel: {found[-3:]}"
    )
    await bot.call("local_player.stop_moving")
    await bot.call("local_player.set_sprinting", False)
    await ctx.ticks(10)
    assert not player.is_sprinting
    ctx.release(toggles)
    again = ctx.capture(PlayerToggleSprintEvent, mine("mvsprno"))
    await bot.call("local_player.set_sprinting", True)
    await bot.call("local_player.set_move_vector", 0.0, 1.0)
    await ctx.until(lambda: again.count, timeout=40, what="a later sprint to fire")
    await ctx.until(
        lambda: player.is_sprinting,
        timeout=40,
        what="the player to sprint once not cancelled",
    )


@scenario("movement.flight.cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET)
async def flight_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvflyno", mode="creative")
    assert player.allow_flight
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvflyno"), cancel=True, extra=before("is_flying"))
    index = await mark(bot)
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the flight toggle")
    flying = await states(ctx, lambda: player.is_flying, 30)
    assert flying == {False}, f"a cancelled flight still started: {flying}"
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_flying"] is True and toggles.events[0]["before"] is False
    told = flying_bit(await packets(bot, "UpdateAbilitiesPacket", index))
    assert told is False, "the client was not sent its abilities with flying clear"
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: player.is_sneaking, timeout=40, what="sneaking to work after a cancelled flight")
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: not player.is_sneaking, timeout=40, what="the player to stand")
    ctx.release(toggles)
    again = ctx.capture(PlayerToggleFlightEvent, mine("mvflyno"), extra=before("is_flying"))
    await bot.call("local_player.set_flying", False)
    await ctx.ticks(10)
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: again.count, timeout=40, what="a later double tap to fire")
    assert again.events[-1]["is_flying"] is True and again.events[-1]["before"] is False, again.events
    await ctx.until(lambda: player.is_flying, timeout=40, what="the player to fly once not cancelled")


@scenario("movement.flight.stop_cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET)
async def flight_stop_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvflystay", mode="creative")
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: player.is_flying, timeout=40, what="the player to fly")
    await ctx.ticks(10)
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvflystay"), cancel=True, extra=before("is_flying"))
    index = await mark(bot)
    await bot.call("local_player.set_flying", False)
    await ctx.until(lambda: toggles.count, timeout=40, what="the flight stop")
    flying = await states(ctx, lambda: player.is_flying)
    assert flying == {True}, f"a cancelled stop still ended the flight: {flying}"
    assert toggles.count == 1, toggles.events
    assert toggles.events[0]["is_flying"] is False and toggles.events[0]["before"] is True
    told = flying_bit(await packets(bot, "UpdateAbilitiesPacket", index))
    assert told is True, "the client was not sent its abilities with flying set"
    await ctx.ticks(40)
    ctx.info["flying_after_client_landed"] = player.is_flying
    ctx.info["stops_fired"] = toggles.count


@scenario("movement.input.pinned_raw", covers={PlayerInputEvent: "normal"}, plot=True, budget=BUDGET)
async def input_pinned_raw(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvrawin")
    ctx.capture(PlayerToggleSneakEvent, mine("mvrawin"), cancel=True)
    inputs = ctx.capture(PlayerInputEvent, mine("mvrawin"))
    keys = ("forward", "backward", "left", "right", "jump", "sneak", "sprint")
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: inputs.count, timeout=60, what="PlayerInputEvent for the sneak press")
    await ctx.ticks(10)
    assert not player.is_sneaking, "the cancelled sneak made the server sneak"
    assert inputs.count == 1, inputs.events
    got = {k for k, v in zip(keys, inputs.events[0]["input_state"]) if v}
    assert got == {"sneak"}, f"PlayerInputEvent did not report the raw sneak key: {got}"
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: inputs.count == 2, timeout=60, what="PlayerInputEvent for the release")
    got = {k for k, v in zip(keys, inputs.events[1]["input_state"]) if v}
    assert got == set(), got


@scenario("movement.flight.triple_tap_cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET)
async def flight_triple_tap_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvfly3", mode="creative")
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvfly3"), cancel=True)
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: toggles.count, timeout=40, what="the cancelled flight toggle")
    await ctx.call("bot.jump_taps", bot="mvfly3", presses=[True])
    flying = await states(ctx, lambda: player.is_flying, 30)
    assert flying == {False}, f"a third jump press started the flight: {flying}"
    assert toggles.count == 1, toggles.events
    await bot.call("local_player.set_sneaking", True)
    await ctx.until(lambda: player.is_sneaking, timeout=40, what="sneaking after a cancelled flight and a third tap")
    await bot.call("local_player.set_sneaking", False)
    await ctx.until(lambda: not player.is_sneaking, timeout=40, what="the player to stand")


@scenario(
    "movement.flight.triple_tap_stop_cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET
)
async def flight_triple_tap_stop_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvfly4", mode="creative")
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: player.is_flying, timeout=40, what="the player to fly")
    await ctx.ticks(10)
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvfly4"), cancel=True)
    await bot.call("local_player.set_flying", False)
    await ctx.until(lambda: toggles.count, timeout=40, what="the cancelled flight stop")
    await ctx.call("bot.jump_taps", bot="mvfly4", presses=[True])
    flying = await states(ctx, lambda: player.is_flying, 30)
    assert flying == {True}, f"a third jump press ended the flight with no event: {flying}"
    assert toggles.count == 1, toggles.events


def portal_blocks_near(ctx: Context, at, radius: int = 2) -> int:
    x, y, z = (int(v // 1) for v in at)
    count = 0
    for dx in range(-radius, radius + 1):
        for dy in range(-1, radius + 2):
            for dz in range(-radius, radius + 1):
                if str(ctx.overworld.get_block_at(x + dx, y + dy, z + dz).type) == "minecraft:portal":
                    count += 1
    return count



def lit_portal(ctx: Context, dx: int, dz: int) -> tuple[int, int, int]:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    ctx.run(f"fill {x} {y - 1} {z} {x} {y + 3} {z + 3} obsidian")
    ctx.run(f"fill {x} {y} {z + 1} {x} {y + 2} {z + 2} air")
    ctx.run(f"setblock {x} {y} {z + 1} fire")
    return x, y, z


def portal_blocks(dimension, x0: int, y0: int, z0: int, x1: int, y1: int, z1: int) -> list:
    return [
        (x, y, z)
        for x in range(x0, x1 + 1)
        for y in range(y0, y1 + 1)
        for z in range(z0, z1 + 1)
        if str(dimension.get_block_at(x, y, z).type) == "minecraft:portal"
    ]


def redirect_once(capture_ref: list, location):
    def mutate(event):
        if capture_ref[0].count > 1:
            event.cancel()
            return
        event.to_location = location

    return mutate


async def settle(ctx: Context, player, ticks: int = 30) -> tuple:
    trail = []
    for _ in range(ticks):
        await ctx.ticks(1)
        trail.append(tuple(round(v, 3) for v in where(player)))
    return trail[-1], trail


@scenario("movement.portal.mutate_same_dimension", covers={PlayerPortalEvent: "mutate"}, plot=True, budget=2000)
async def portal_mutate_same_dimension(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvportalsd", dx=5, dz=8, mode="creative")
    x, y, z = lit_portal(ctx, 9, 7)
    await ctx.until(
        lambda: len(portal_blocks(ctx.overworld, x, y, z + 1, x, y + 2, z + 2)) == 6,
        timeout=40,
        what="the lit portal",
    )
    target = (x - 6.5, float(y), z + 6.5)
    ref = []
    portals = ctx.capture(
        PlayerPortalEvent, mine("mvportalsd"), mutate=redirect_once(ref, Location(ctx.overworld, *target))
    )
    ref.append(portals)
    await bot.call("local_player.move_to", [x + 0.5, y, z + 1.5])
    await ctx.until(lambda: portals.count, timeout=300, what="PlayerPortalEvent")
    await bot.call("local_player.stop_moving")
    entered = portals.events[0]["from_location"]
    landed, trail = await settle(ctx, player)
    ctx.info["entered"] = (entered["x"], entered["y"], entered["z"])
    ctx.info["landed"] = landed
    ctx.info["trail"] = trail[:8]
    assert z + 1 <= landed[2] <= z + 3 and abs(landed[0] - (x + 0.5)) < 0.6, (
        f"the player landed at {landed}, outside the portal frame at x={x}, z={z + 1}..{z + 3}"
    )
    assert abs(landed[2] - entered["z"]) < 0.6, (
        f"the player landed at z={landed[2]} in the frame but entered it at z={entered['z']}"
    )


@scenario("movement.portal.mutate_nearby_portal", covers={PlayerPortalEvent: "mutate"}, plot=True, budget=4000)
async def portal_mutate_nearby_portal(ctx: Context) -> None:
    nether = ctx.server.level.get_dimension("nether")
    first, first_player, start = await join(ctx, "mvportaln1", dx=5, dz=8, mode="creative")
    x, y, z = lit_portal(ctx, 9, 7)
    await ctx.until(
        lambda: len(portal_blocks(ctx.overworld, x, y, z + 1, x, y + 2, z + 2)) == 6,
        timeout=40,
        what="the lit portal",
    )
    ref = []
    trips = ctx.capture(
        PlayerPortalEvent, mine("mvportaln1"), mutate=redirect_once(ref, Location(nether, -1800.5, 70.0, 1800.5))
    )
    ref.append(trips)
    await first.call("local_player.move_to", [x + 0.5, y, z + 1.5])
    await ctx.until(lambda: trips.count, timeout=300, what="the first PlayerPortalEvent")
    await first.call("local_player.stop_moving")
    await ctx.until(
        lambda: str(first_player.location.dimension.id) == "minecraft:nether",
        timeout=600,
        what="the first player to reach the nether",
    )
    await poll(
        ctx,
        lambda: first.call("local_player.is_changing_dimension"),
        lambda changing: changing is False,
        timeout=600,
        what="the first client to finish the dimension change",
    )
    await ctx.ticks(40)
    ax, ay, az = (int(v // 1) for v in where(first_player))
    x2, y2, z2 = ax + 24, ay, az
    ctx.run(f"execute in nether run fill {x2 - 3} {y2 - 2} {z2 - 4} {x2 + 7} {y2 + 5} {z2 + 4} obsidian")
    ctx.run(f"execute in nether run fill {x2 - 2} {y2 - 1} {z2 - 3} {x2 + 6} {y2 + 4} {z2 + 3} air")
    ctx.run(f"execute in nether run fill {x2} {y2 - 1} {z2} {x2 + 4} {y2 + 3} {z2} obsidian")
    ctx.run(f"execute in nether run fill {x2 + 1} {y2} {z2} {x2 + 3} {y2 + 2} {z2} air")
    ctx.run(f"execute in nether run setblock {x2 + 1} {y2} {z2} fire")
    await ctx.until(
        lambda: len(portal_blocks(nether, x2 + 1, y2, z2, x2 + 3, y2 + 2, z2)) == 9,
        timeout=60,
        what="the lit nether portal",
    )
    second, second_player, start = await join(ctx, "mvportaln2", dx=5, dz=8, mode="creative")
    target = (x2 + 10.5, float(y2), z2 + 3.5)
    ref2 = []
    portals = ctx.capture(PlayerPortalEvent, mine("mvportaln2"), mutate=redirect_once(ref2, Location(nether, *target)))
    ref2.append(portals)
    await second.call("local_player.move_to", [x + 0.5, y, z + 1.5])
    await ctx.until(lambda: portals.count, timeout=300, what="the second PlayerPortalEvent")
    await second.call("local_player.stop_moving")
    await ctx.until(
        lambda: str(second_player.location.dimension.id) == "minecraft:nether",
        timeout=600,
        what="the second player to reach the nether",
    )
    await poll(
        ctx,
        lambda: second.call("local_player.is_changing_dimension"),
        lambda changing: changing is False,
        timeout=600,
        what="the second client to finish the dimension change",
    )
    entered = portals.events[0]["from_location"]
    landed, trail = await settle(ctx, second_player, 40)
    offset = (entered["z"] - (z + 1)) / 2
    expected_x = x2 + 1 + 3 * offset
    ctx.info["first_portal"] = (ax, ay, az)
    ctx.info["built_portal"] = (x2 + 1, y2, z2)
    ctx.info["entered"] = (entered["x"], entered["y"], entered["z"])
    ctx.info["landed"] = landed
    ctx.info["expected_x"] = expected_x
    ctx.info["trail"] = trail[:8]
    assert x2 + 1 <= landed[0] <= x2 + 4 and abs(landed[2] - (z2 + 0.5)) < 0.6, (
        f"the player landed at {landed}, outside the nether portal frame at x={x2 + 1}..{x2 + 4}, z={z2}"
    )
    assert abs(landed[0] - expected_x) < 0.6, (
        f"the player landed at x={landed[0]} but entered the overworld portal {offset:.2f} along it, "
        f"which is x={expected_x:.2f} in the nether portal"
    )



async def bunched_flight_taps(ctx: Context, name: str, flying: bool) -> None:
    import time

    await ctx.call("bot.flight_burst", bot=name, flying=flying, after=[False, False, True], delay=0.3)
    time.sleep(1.0)


@scenario(
    "movement.flight.bunched_stop_cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET
)
async def flight_bunched_stop_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvfly5", mode="creative")
    await ctx.ticks(20)
    await bot.call("local_player.set_flying", True)
    await ctx.until(lambda: player.is_flying, timeout=80, what="the player to fly")
    await ctx.ticks(20)
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvfly5"), cancel=True)
    await bunched_flight_taps(ctx, "mvfly5", False)
    flying = await states(ctx, lambda: player.is_flying, 40)
    assert toggles.count == 1, toggles.events
    assert flying == {True}, f"a third press bunched with a cancelled stop ended the flight with no event: {flying}"


@scenario(
    "movement.flight.bunched_start_cancel", covers={PlayerToggleFlightEvent: "cancel"}, plot=True, budget=BUDGET
)
async def flight_bunched_start_cancel(ctx: Context) -> None:
    bot, player, start = await join(ctx, "mvfly6", mode="creative")
    await ctx.ticks(20)
    toggles = ctx.capture(PlayerToggleFlightEvent, mine("mvfly6"), cancel=True)
    await bunched_flight_taps(ctx, "mvfly6", True)
    flying = await states(ctx, lambda: player.is_flying, 40)
    assert toggles.count == 1, toggles.events
    assert flying == {False}, f"a third press bunched with a cancelled start began a flight with no event: {flying}"
