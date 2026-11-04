import json
import math
import uuid

from endstone.actor import Actor
from endstone.event import (
    ActorCollideWithActorEvent,
    ActorDamageEvent,
    ActorDeathEvent,
    ActorDismountEvent,
    ActorKnockbackEvent,
    ActorPickupItemEvent,
    PlayerArmorStandManipulateEvent,
    PlayerBucketActorEvent,
    PlayerBucketFillEvent,
    PlayerDropItemEvent,
    PlayerInteractActorEvent,
    PlayerPickupItemEvent,
    PlayerShearActorEvent,
)
from endstone.inventory import ItemStack
from endstone.level import Location
from endstone.util import Vector

from endstone_test.harness import Context, Skip, scenario
from endstone_test.harness.engine import HarnessError
from endstone_test.harness.plot import HEIGHT, SIZE

BUDGET = 900
NO_DISMOUNT = "endstone-client has no local_player.leave_vehicle, the InteractPacket LEAVE_VEHICLE a real client sends to leave a boat (board todo scen.actors-2)"


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def packets(bot, name: str, since: int) -> list[dict]:
    return await bot.call("get_received_packets", name, since=since)


async def poll(ctx: Context, read, ok, timeout: int = 100, what: str = "a condition"):
    for _ in range(timeout):
        value = await read()
        if ok(value):
            return value
        await ctx.ticks(1)
    raise TimeoutError(f"timed out after {timeout} polls waiting for {what}")


async def barrier(ctx: Context, *bots) -> None:
    token = f"@@barrier {uuid.uuid4().hex[:8]}"
    since = {bot.name: await mark(bot) for bot in bots}
    ctx.server.broadcast_message(token)
    for bot in bots:
        await poll(
            ctx,
            lambda: packets(bot, "TextPacket", since[bot.name]),
            lambda found: any(token in json.dumps(p["packet"]) for p in found),
            what=f"{bot.name} to hear the barrier",
        )


def in_plot(ctx: Context, actor: Actor) -> bool:
    location = actor.location
    plot = ctx.plot
    return (
        plot.x <= location.x < plot.x + SIZE
        and plot.y - 1 <= location.y < plot.y + HEIGHT
        and plot.z <= location.z < plot.z + SIZE
    )


def mine(name: str):
    return lambda e: e.player.name == name


def hit(kind: str):
    return lambda e: str(e.actor.type) == kind


def where(actor) -> tuple[float, float, float]:
    loc = actor.location
    return loc.x, loc.y, loc.z


def flat(a, b) -> float:
    return math.hypot(a[0] - b[0], a[2] - b[2])


def source_of(event) -> dict:
    source = event.damage_source
    return {
        "cause": source.type,
        "by": getattr(source.actor, "name", None),
        "via": getattr(source.damaging_actor, "name", None),
    }


async def join(ctx: Context, name: str, dx: int = 8, dz: int = 8, mode: str = "survival"):
    bot = await ctx.bot(name)
    player = ctx.server.get_player(name)
    assert player is not None, f"{name} is not online"
    ctx.run(f"gamemode {mode} {name}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    spot = (x + 0.5, y, z + 0.5)
    ctx.run(f"tp {name} {spot[0]} {spot[1]} {spot[2]}")
    await ctx.until(
        lambda: flat(where(player), spot) < 0.1, timeout=60, what=f"{name} to arrive in the plot"
    )
    await poll(
        ctx,
        lambda: bot.call("local_player.get_position"),
        lambda p: flat(p, spot) < 0.1,
        what=f"{name}'s client to see the teleport",
    )
    await ctx.ticks(10)
    return bot, player, spot


async def give(ctx: Context, bot, name: str, item: str, slot: str = "slot.weapon.mainhand") -> None:
    ctx.run(f"replaceitem entity {name} {slot} 0 {item}")
    wanted = f"minecraft:{item}"
    if slot == "slot.weapon.mainhand":
        await poll(
            ctx,
            lambda: bot.call("local_player.get_inventory_json"),
            lambda inv: inv["inventory"]["slots"][inv["inventory"]["selected_slot"]]["id"] == wanted,
            what=f"{name}'s client to hold {item}",
        )


async def spawn(ctx: Context, bot, kind: str, dx: float, dz: float, dy: float = 0, slow: bool = True, extra: str = ""):
    known = {a.runtime_id for a in ctx.level.actors}
    x, y, z = ctx.plot.pos(0, 0, 0)
    ctx.run(f"summon {kind} {x + dx} {y + dy} {z + dz} {extra}".rstrip())
    full = f"minecraft:{kind}"
    actor = await ctx.until(
        lambda: next(
            (a for a in ctx.level.actors if a.runtime_id not in known and str(a.type) == full and in_plot(ctx, a)),
            None,
        ),
        timeout=30,
        what=f"the summoned {kind}",
    )
    if slow:
        ctx.run(f"effect @e[type={kind},{ctx.plot.volume}] slowness 600 255 true")
    await poll(
        ctx,
        lambda: bot.call("get_actors", full),
        lambda found: any(a["runtime_id"] == actor.runtime_id for a in found),
        what=f"the client to see the {kind}",
    )
    return actor


async def punch(ctx: Context, name: str):
    bot, player, spot = await join(ctx, name)
    pig = await spawn(ctx, bot, "pig", spot[0] - ctx.plot.pos(0, 0, 0)[0], 10.5)
    return bot, player, pig


def pig_health_seen(packets_: list[dict], runtime_id: int):
    seen = [
        a["current_value"]
        for p in packets_
        if p["packet"]["runtime_id"] == runtime_id
        for a in p["packet"]["attribute_data"]
        if a["name"] == "minecraft:health"
    ]
    return seen[-1] if seen else None


def hurt_animations(found: list[dict], runtime_id: int) -> list[dict]:
    return [p for p in found if p["packet"]["runtime_id"] == runtime_id and p["packet"]["event_id"] == "HURT"]


def pushed(found: list[dict], runtime_id: int) -> list[dict]:
    return [p for p in found if p["packet"]["runtime_id"] == runtime_id]


@scenario(
    "pact.attack.normal",
    covers={ActorDamageEvent: "normal", ActorKnockbackEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def attack_normal(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "patk")
    damage = ctx.capture(ActorDamageEvent, hit("minecraft:pig"), extra=source_of)
    knock = ctx.capture(ActorKnockbackEvent, hit("minecraft:pig"))
    before = pig.health
    start = where(pig)
    seen_start = (await client_actor(bot, "minecraft:pig", pig.runtime_id))["position"]
    index = await mark(bot)
    await bot.call("game_mode.attack", pig.runtime_id)
    await ctx.until(lambda: damage.count and knock.count, timeout=40, what="the player's hit")
    await barrier(ctx, bot)
    assert damage.count == 1 and knock.count == 1, (damage.events, knock.events)
    assert damage.events[0]["damage"] == 1.0, damage.events[0]
    assert damage.events[0]["by"] == "patk" and damage.events[0]["cause"] == "entity_attack", damage.events[0]
    assert pig.health == before - 1.0, (before, pig.health)
    source = knock.events[0]["source"]
    assert source is not None and source["name"] == "patk", knock.events[0]
    end = where(pig)
    assert end[2] - start[2] > 0.1, f"the pig was not knocked away from the attacker: {start} -> {end}"
    assert len(hurt_animations(await packets(bot, "ActorEventPacket", index), pig.runtime_id)) == 1
    health = pig_health_seen(await packets(bot, "UpdateAttributesPacket", index), pig.runtime_id)
    assert health == before - 1.0, f"the client was told the pig has {health} health"
    seen_end = (await client_actor(bot, "minecraft:pig", pig.runtime_id))["position"]
    assert seen_end[2] - seen_start[2] > 0.1, f"the client never saw the knockback: {seen_start} -> {seen_end}"


@scenario("pact.attack.damage_cancel", covers={ActorDamageEvent: "cancel"}, plot=True, budget=BUDGET)
async def attack_damage_cancel(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "patkno")
    damage = ctx.capture(ActorDamageEvent, hit("minecraft:pig"), cancel=True)
    knock = ctx.capture(ActorKnockbackEvent, hit("minecraft:pig"))
    before = pig.health
    start = where(pig)
    index = await mark(bot)
    await bot.call("game_mode.attack", pig.runtime_id)
    await ctx.until(lambda: damage.count, timeout=40, what="ActorDamageEvent")
    await barrier(ctx, bot)
    assert pig.health == before, f"a cancelled hit still took {before - pig.health} health"
    assert flat(where(pig), start) < 0.05, f"a cancelled hit still moved the pig {start} -> {where(pig)}"
    assert knock.count == 0, f"a cancelled hit still knocked the pig back: {knock.events}"
    assert not hurt_animations(await packets(bot, "ActorEventPacket", index), pig.runtime_id)
    assert not pushed(await packets(bot, "SetActorMotionPacket", index), pig.runtime_id)
    assert pig_health_seen(await packets(bot, "UpdateAttributesPacket", index), pig.runtime_id) is None


@scenario("pact.attack.damage_mutate", covers={ActorDamageEvent: "mutate"}, plot=True, budget=BUDGET)
async def attack_damage_mutate(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "patkmut")
    damage = ctx.capture(
        ActorDamageEvent, hit("minecraft:pig"), mutate=lambda e: setattr(e, "damage", 3.0), extra=source_of
    )
    before = pig.health
    index = await mark(bot)
    await bot.call("game_mode.attack", pig.runtime_id)
    await ctx.until(lambda: damage.count, timeout=40, what="ActorDamageEvent")
    await barrier(ctx, bot)
    assert damage.events[0]["damage"] == 1.0 and damage.events[0]["after"]["damage"] == 3.0
    assert pig.health == before - 3.0, (before, pig.health)
    health = pig_health_seen(await packets(bot, "UpdateAttributesPacket", index), pig.runtime_id)
    assert health == before - 3.0, f"the client was told the pig has {health} health"


@scenario("pact.attack.knockback_cancel", covers={ActorKnockbackEvent: "cancel"}, plot=True, budget=BUDGET)
async def attack_knockback_cancel(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "pknno")
    knock = ctx.capture(ActorKnockbackEvent, hit("minecraft:pig"), cancel=True)
    before = pig.health
    start = where(pig)
    index = await mark(bot)
    await bot.call("game_mode.attack", pig.runtime_id)
    await ctx.until(lambda: knock.count, timeout=40, what="ActorKnockbackEvent")
    await barrier(ctx, bot)
    assert pig.health == before - 1.0, "the hit itself must still land"
    assert flat(where(pig), start) < 0.05, f"a cancelled knockback still moved the pig {start} -> {where(pig)}"
    assert len(hurt_animations(await packets(bot, "ActorEventPacket", index), pig.runtime_id)) == 1
    for p in pushed(await packets(bot, "SetActorMotionPacket", index), pig.runtime_id):
        motion = p["packet"]["motion"]
        assert abs(motion["x"]) < 0.01 and abs(motion["z"]) < 0.01, f"the client was sent a knockback: {motion}"


@scenario("pact.attack.knockback_mutate", covers={ActorKnockbackEvent: "mutate"}, plot=True, budget=BUDGET)
async def attack_knockback_mutate(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "pknmut")
    knock = ctx.capture(
        ActorKnockbackEvent, hit("minecraft:pig"), mutate=lambda e: setattr(e, "knockback", Vector(1.0, 0.4, 0.0))
    )
    start = where(pig)
    seen_start = (await client_actor(bot, "minecraft:pig", pig.runtime_id))["position"]
    await mark(bot)
    await bot.call("game_mode.attack", pig.runtime_id)
    await ctx.until(lambda: knock.count, timeout=40, what="ActorKnockbackEvent")
    await ctx.until(lambda: where(pig)[0] - start[0] > 0.3, timeout=40, what="the pig to follow the written knockback")
    await barrier(ctx, bot)
    assert abs(where(pig)[2] - start[2]) < 0.1, f"the pig still went along the attack direction: {start} -> {where(pig)}"
    seen_end = (await client_actor(bot, "minecraft:pig", pig.runtime_id))["position"]
    assert seen_end[0] - seen_start[0] > 0.1, f"the client never saw the knockback: {seen_start} -> {seen_end}"


@scenario("pact.attack.kill", covers={ActorDeathEvent: "normal"}, plot=True, budget=BUDGET)
async def attack_kill(ctx: Context) -> None:
    bot, player, pig = await punch(ctx, "pkill")
    await give(ctx, bot, "pkill", "diamond_sword")
    death = ctx.capture(ActorDeathEvent, hit("minecraft:pig"), extra=source_of)
    index = await mark(bot)
    for _ in range(8):
        await bot.call("game_mode.attack", pig.runtime_id)
        try:
            await ctx.until(lambda: death.count, timeout=15, what="the pig to die")
            break
        except TimeoutError:
            continue
    assert death.count == 1, death.events
    assert death.events[0]["by"] == "pkill" and death.events[0]["cause"] == "entity_attack", death.events[0]
    await poll(
        ctx,
        lambda: packets(bot, "RemoveActorPacket", index),
        lambda found: found,
        what="the client to lose the pig",
    )
    assert death.count == 1



def ground_items(ctx: Context) -> list:
    return [a for a in ctx.level.actors if str(a.type) == "minecraft:item" and in_plot(ctx, a)]


def held(player) -> str | None:
    stack = player.inventory.item_in_main_hand
    return None if stack is None else str(stack.type)


async def held_by_client(bot) -> str:
    inv = await bot.call("local_player.get_inventory_json")
    return inv["inventory"]["slots"][inv["inventory"]["selected_slot"]]["id"]


async def client_actor(bot, kind: str, runtime_id: int) -> dict:
    found = await bot.call("get_actors", kind)
    return next(a for a in found if a["runtime_id"] == runtime_id)


def links(found: list[dict], vehicle_uid: int) -> list[dict]:
    return [p for p in found if p["packet"]["link"]["a"] == vehicle_uid]


async def stage(ctx: Context, name: str, kind: str, item: str | None, dz: float = 6.5, dy: float = 0, **options):
    bot, player, spot = await join(ctx, name, dx=8, dz=4)
    ox = spot[0] - ctx.plot.pos(0, 0, 0)[0]
    if item:
        await give(ctx, bot, name, item)
    mob = await spawn(ctx, bot, kind, ox, dz, dy=dy, **options)
    if kind in ("sheep", "cow"):
        ctx.command(f"event entity @e[type={kind},{ctx.plot.volume}] minecraft:ageable_grow_up")
    return bot, player, mob, ox


def seated(ctx: Context) -> bool:
    return ctx.command(f"ride @e[type=boat,{ctx.plot.volume}] evict_riders").success


@scenario("pact.interact.boat", covers={PlayerInteractActorEvent: "normal"}, plot=True, budget=BUDGET)
async def interact_boat(ctx: Context) -> None:
    bot, player, boat, _ = await stage(ctx, "pibt", "boat", None, slow=False)
    uid = (await client_actor(bot, "minecraft:boat", boat.runtime_id))["unique_id"]
    touched = ctx.capture(PlayerInteractActorEvent, mine("pibt"))
    index = await mark(bot)
    await bot.call("game_mode.interact", boat.runtime_id)
    await ctx.until(lambda: touched.count, timeout=40, what="PlayerInteractActorEvent")
    found = await poll(
        ctx, lambda: packets(bot, "SetActorLinkPacket", index), lambda f: links(f, uid), what="the client to be seated"
    )
    await barrier(ctx, bot)
    assert touched.count == 1 and touched.events[0]["actor"]["type"] == "minecraft:boat", touched.events
    assert links(found, uid)[0]["packet"]["link"]["type"] == "RIDING", found
    ctx.release(touched)
    assert seated(ctx), "the server did not seat the player in the boat"


@scenario("pact.interact.boat_cancel", covers={PlayerInteractActorEvent: "cancel"}, plot=True, budget=BUDGET)
async def interact_boat_cancel(ctx: Context) -> None:
    bot, player, boat, _ = await stage(ctx, "pibtno", "boat", None, slow=False)
    uid = (await client_actor(bot, "minecraft:boat", boat.runtime_id))["unique_id"]
    touched = ctx.capture(PlayerInteractActorEvent, mine("pibtno"), cancel=True)
    index = await mark(bot)
    await bot.call("game_mode.interact", boat.runtime_id)
    await ctx.until(lambda: touched.count, timeout=40, what="PlayerInteractActorEvent")
    await barrier(ctx, bot)
    assert not links(await packets(bot, "SetActorLinkPacket", index), uid), "the client was seated in the boat"
    ctx.release(touched)
    assert not seated(ctx), "a cancelled interaction still seated the player"


@scenario("pact.interact.trader", covers={PlayerInteractActorEvent: "normal"}, plot=True, budget=BUDGET)
async def interact_trader(ctx: Context) -> None:
    bot, player, trader, _ = await stage(ctx, "pitr", "wandering_trader", None)
    touched = ctx.capture(PlayerInteractActorEvent, mine("pitr"))
    index = await mark(bot)
    await bot.call("game_mode.interact", trader.runtime_id)
    shown = await poll(
        ctx,
        lambda: packets(bot, "UpdateTradePacket", index),
        lambda f: f,
        what="the trade screen",
    )
    assert touched.count == 1 and touched.events[0]["actor"]["runtime_id"] == trader.runtime_id, touched.events
    assert shown[0]["packet"]["type"] == "TRADE"


@scenario("pact.interact.trader_cancel", covers={PlayerInteractActorEvent: "cancel"}, plot=True, budget=BUDGET)
async def interact_trader_cancel(ctx: Context) -> None:
    bot, player, trader, _ = await stage(ctx, "pitrno", "wandering_trader", None)
    touched = ctx.capture(PlayerInteractActorEvent, mine("pitrno"), cancel=True)
    index = await mark(bot)
    await bot.call("game_mode.interact", trader.runtime_id)
    await ctx.until(lambda: touched.count, timeout=40, what="PlayerInteractActorEvent")
    await barrier(ctx, bot)
    assert not await packets(bot, "UpdateTradePacket", index), "a cancelled interaction still opened the trade screen"
    assert not await packets(bot, "ContainerOpenPacket", index)


async def shear_sheep(ctx: Context, name: str, cancel: bool):
    shears = ctx.capture(PlayerShearActorEvent, mine(name), cancel=cancel)
    bot, player, sheep, _ = await stage(ctx, name, "sheep", "shears")
    ctx.command(f"kill @e[type=item,{ctx.plot.volume}]")
    await ctx.until(lambda: not ground_items(ctx), timeout=20, what="the plot to hold no items")
    await barrier(ctx, bot)
    assert shears.count == 0, "PlayerShearActorEvent fired before anyone sheared"
    await bot.call("game_mode.interact", sheep.runtime_id, 0.5)
    await ctx.until(lambda: shears.count, timeout=60, what="PlayerShearActorEvent")
    await barrier(ctx, bot)
    return bot, player, sheep, shears


def wool(ctx: Context) -> list:
    return [a for a in ground_items(ctx) if str(a.item_stack.type).endswith("_wool")]


@scenario("pact.shear.normal", covers={PlayerShearActorEvent: "normal"}, plot=True, budget=BUDGET)
async def shear_normal(ctx: Context) -> None:
    bot, player, sheep, shears = await shear_sheep(ctx, "pshear", cancel=False)
    assert shears.count == 1, shears.events
    assert shears.events[0]["actor"]["runtime_id"] == sheep.runtime_id and shears.events[0]["hand"] == "HAND"
    assert shears.events[0]["item"]["type"] == "minecraft:shears"
    await ctx.until(lambda: wool(ctx), timeout=40, what="wool to drop")
    await bot.call("game_mode.interact", sheep.runtime_id, 0.5)
    await barrier(ctx, bot)
    assert shears.count == 1, "a sheep that was already sheared raised another PlayerShearActorEvent"


@scenario("pact.shear.cancel", covers={PlayerShearActorEvent: "cancel"}, plot=True, budget=BUDGET)
async def shear_cancel(ctx: Context) -> None:
    bot, player, sheep, shears = await shear_sheep(ctx, "pshearno", cancel=True)
    assert shears.count == 1, shears.events
    assert not wool(ctx), "a cancelled shear still dropped wool"
    ctx.release(shears)
    again = ctx.capture(PlayerShearActorEvent, mine("pshearno"))
    await bot.call("game_mode.interact", sheep.runtime_id, 0.5)
    await ctx.until(lambda: again.count, timeout=60, what="the sheep to be shearable after the cancelled shear")
    await ctx.until(lambda: wool(ctx), timeout=40, what="wool from the second shear")


async def milk_cow(ctx: Context, name: str, **options):
    fills = ctx.capture(PlayerBucketFillEvent, mine(name), **options)
    bot, player, cow, _ = await stage(ctx, name, "cow", "bucket")
    await barrier(ctx, bot)
    assert fills.count == 0, "PlayerBucketFillEvent fired before anyone milked"
    await bot.call("game_mode.interact", cow.runtime_id, 0.5)
    await ctx.until(lambda: fills.count, timeout=60, what="PlayerBucketFillEvent")
    await barrier(ctx, bot)
    return bot, player, cow, fills


@scenario("pact.milk.normal", covers={PlayerBucketFillEvent: "normal"}, plot=True, budget=BUDGET)
async def milk_normal(ctx: Context) -> None:
    bot, player, cow, fills = await milk_cow(ctx, "pmilk")
    event = fills.events[0]
    assert fills.count == 1 and event["block_face"] == "SELF" and event["hand"] == "HAND", fills.events
    assert "minecraft:bucket" in event["bucket"], event
    await ctx.until(lambda: held(player) == "minecraft:milk_bucket", timeout=40, what="the cow to be milked")
    await poll(
        ctx,
        lambda: held_by_client(bot),
        lambda item: item == "minecraft:milk_bucket",
        what="the client to hold the milk bucket",
    )


@scenario("pact.milk.cancel", covers={PlayerBucketFillEvent: "cancel"}, plot=True, budget=BUDGET)
async def milk_cancel(ctx: Context) -> None:
    bot, player, cow, fills = await milk_cow(ctx, "pmilkno", cancel=True)
    assert fills.count == 1, fills.events
    assert held(player) == "minecraft:bucket", f"a cancelled milking left the player holding {held(player)}"
    assert await held_by_client(bot) == "minecraft:bucket"
    ctx.release(fills)
    again = ctx.capture(PlayerBucketFillEvent, mine("pmilkno"))
    await bot.call("game_mode.interact", cow.runtime_id, 0.5)
    await ctx.until(lambda: again.count, timeout=60, what="the cow to be milkable after the cancelled milking")
    await ctx.until(lambda: held(player) == "minecraft:milk_bucket", timeout=40, what="the second milking to succeed")


@scenario("pact.milk.mutate", covers={PlayerBucketFillEvent: "mutate"}, plot=True, budget=BUDGET)
async def milk_mutate(ctx: Context) -> None:
    bot, player, cow, fills = await milk_cow(
        ctx, "pmilkmut", mutate=lambda e: setattr(e, "item_stack", ItemStack("minecraft:diamond", 1))
    )
    assert fills.events[0]["after"]["item_stack"]["type"] == "minecraft:diamond"
    await ctx.until(lambda: held(player) == "minecraft:diamond", timeout=40, what="the written item_stack to be handed over")
    await poll(
        ctx, lambda: held_by_client(bot), lambda item: item == "minecraft:diamond", what="the client to hold the diamond"
    )


async def water_pool(ctx: Context) -> None:
    x, y, z = ctx.plot.pos(8, 0, 4)
    ctx.run(f"fill {x - 1} {y} {z + 2} {x + 1} {y + 2} {z + 4} water")
    await ctx.ticks(10)


async def bucket_cod(ctx: Context, name: str, cancel: bool):
    buckets = ctx.capture(PlayerBucketActorEvent, mine(name), cancel=cancel)
    bot, player, spot = await join(ctx, name, dx=8, dz=4)
    ox = spot[0] - ctx.plot.pos(0, 0, 0)[0]
    await water_pool(ctx)
    await give(ctx, bot, name, "water_bucket")
    cod = await spawn(ctx, bot, "cod", ox, 7.5, dy=1)
    uid = (await client_actor(bot, "minecraft:cod", cod.runtime_id))["unique_id"]
    ctx.info["cod_id"] = cod.runtime_id
    await barrier(ctx, bot)
    assert buckets.count == 0, "PlayerBucketActorEvent fired before anyone used the bucket"
    index = await mark(bot)
    await bot.call("game_mode.interact", cod.runtime_id, 0.5)
    await ctx.until(lambda: buckets.count, timeout=60, what="PlayerBucketActorEvent")
    await barrier(ctx, bot)
    return bot, player, cod, uid, buckets, index


@scenario("pact.bucket.cod", covers={PlayerBucketActorEvent: "normal"}, plot=True, budget=BUDGET)
async def bucket_cod_normal(ctx: Context) -> None:
    bot, player, cod, uid, buckets, index = await bucket_cod(ctx, "pcod", cancel=False)
    event = buckets.events[0]
    assert buckets.count == 1 and event["actor"]["runtime_id"] == ctx.info["cod_id"], buckets.events
    assert event["original_bucket"]["type"] == "minecraft:water_bucket" and event["hand"] == "HAND", event
    await ctx.until(lambda: held(player) == "minecraft:cod_bucket", timeout=40, what="the player to hold a cod bucket")
    assert not cod.is_valid, "the cod is still in the world"
    await poll(
        ctx,
        lambda: packets(bot, "RemoveActorPacket", index),
        lambda f: any(p["packet"]["entity_id"] == uid for p in f),
        what="the client to lose the cod",
    )
    assert await held_by_client(bot) == "minecraft:cod_bucket"


@scenario("pact.bucket.cod_cancel", covers={PlayerBucketActorEvent: "cancel"}, plot=True, budget=BUDGET)
async def bucket_cod_cancel(ctx: Context) -> None:
    bot, player, cod, uid, buckets, index = await bucket_cod(ctx, "pcodno", cancel=True)
    assert buckets.count == 1, buckets.events
    assert cod.is_valid, "a cancelled bucketing still removed the cod"
    assert held(player) == "minecraft:water_bucket", f"the player holds {held(player)}"
    assert await held_by_client(bot) == "minecraft:water_bucket"
    assert not any(p["packet"]["entity_id"] == uid for p in await packets(bot, "RemoveActorPacket", index))


STAND_SLOT = "slot.armor.head"


def stand_wears(ctx: Context, item: str, slot: str = STAND_SLOT) -> bool:
    return ctx.command(
        f"testfor @e[type=armor_stand,{ctx.plot.volume},hasitem={{item={item},location={slot}}}]"
    ).success


async def stage_stand(ctx: Context, name: str, item: str | None, worn: str | None = None):
    bot, player, stand, _ = await stage(ctx, name, "armor_stand", item, slow=False)
    if worn:
        ctx.run(f"replaceitem entity @e[type=armor_stand,{ctx.plot.volume}] {STAND_SLOT} 0 {worn}")
        await ctx.until(lambda: stand_wears(ctx, worn), timeout=20, what="the stand to wear the helmet")
    await barrier(ctx, bot)
    return bot, player, stand


@scenario("pact.stand.place", covers={PlayerArmorStandManipulateEvent: "normal"}, plot=True, budget=BUDGET)
async def stand_place(ctx: Context) -> None:
    manipulate = ctx.capture(PlayerArmorStandManipulateEvent, mine("pstand"))
    bot, player, stand = await stage_stand(ctx, "pstand", "diamond_helmet")
    assert manipulate.count == 0, "PlayerArmorStandManipulateEvent fired before anyone touched the stand"
    index = await mark(bot)
    await bot.call("game_mode.interact", stand.runtime_id, 1.8)
    await ctx.until(lambda: manipulate.count, timeout=60, what="PlayerArmorStandManipulateEvent")
    await ctx.until(lambda: stand_wears(ctx, "diamond_helmet"), timeout=40, what="the helmet to land on the stand")
    await barrier(ctx, bot)
    event = manipulate.events[0]
    assert manipulate.count == 1, manipulate.events
    assert event["slot"] == "HEAD" and event["player_item"]["type"] == "minecraft:diamond_helmet", event
    assert event["armor_stand_item"]["type"] == "minecraft:air", event
    assert held(player) in (None, "minecraft:air"), f"the player still holds {held(player)}"
    worn = await packets(bot, "MobArmorEquipmentPacket", index)
    assert any(p["packet"]["runtime_id"] == stand.runtime_id and p["packet"]["head"]["id"] != 0 for p in worn), worn


@scenario("pact.stand.cancel", covers={PlayerArmorStandManipulateEvent: "cancel"}, plot=True, budget=BUDGET)
async def stand_cancel(ctx: Context) -> None:
    manipulate = ctx.capture(PlayerArmorStandManipulateEvent, mine("pstandno"), cancel=True)
    bot, player, stand = await stage_stand(ctx, "pstandno", "diamond_helmet")
    index = await mark(bot)
    await bot.call("game_mode.interact", stand.runtime_id, 1.8)
    await ctx.until(lambda: manipulate.count, timeout=60, what="PlayerArmorStandManipulateEvent")
    await barrier(ctx, bot)
    assert manipulate.count == 1, manipulate.events
    assert not stand_wears(ctx, "diamond_helmet"), "a cancelled manipulation still dressed the stand"
    assert held(player) == "minecraft:diamond_helmet"
    assert await held_by_client(bot) == "minecraft:diamond_helmet"
    worn = await packets(bot, "MobArmorEquipmentPacket", index)
    assert not any(p["packet"]["runtime_id"] == stand.runtime_id and p["packet"]["head"]["id"] != 0 for p in worn), worn


@scenario("pact.stand.swap", covers={PlayerArmorStandManipulateEvent: "normal"}, plot=True, budget=BUDGET)
async def stand_swap(ctx: Context) -> None:
    manipulate = ctx.capture(PlayerArmorStandManipulateEvent, mine("pstandsw"))
    bot, player, stand = await stage_stand(ctx, "pstandsw", "iron_helmet", worn="diamond_helmet")
    await bot.call("game_mode.interact", stand.runtime_id, 1.8)
    await ctx.until(lambda: manipulate.count, timeout=60, what="PlayerArmorStandManipulateEvent")
    await ctx.until(lambda: stand_wears(ctx, "iron_helmet"), timeout=40, what="the helmets to swap")
    await barrier(ctx, bot)
    event = manipulate.events[0]
    assert event["slot"] == "HEAD", event
    assert event["armor_stand_item"]["type"] == "minecraft:diamond_helmet", event
    assert event["player_item"]["type"] == "minecraft:iron_helmet", event
    assert held(player) == "minecraft:diamond_helmet", f"the player holds {held(player)} after the swap"
    assert await held_by_client(bot) == "minecraft:diamond_helmet"


@scenario("pact.stand.take", covers={PlayerArmorStandManipulateEvent: "normal"}, plot=True, budget=BUDGET)
async def stand_take(ctx: Context) -> None:
    manipulate = ctx.capture(PlayerArmorStandManipulateEvent, mine("pstandtk"))
    bot, player, stand = await stage_stand(ctx, "pstandtk", None, worn="diamond_helmet")
    await bot.call("game_mode.interact", stand.runtime_id, 1.8)
    await ctx.until(lambda: manipulate.count, timeout=60, what="PlayerArmorStandManipulateEvent")
    await ctx.until(lambda: not stand_wears(ctx, "diamond_helmet"), timeout=40, what="the helmet to leave the stand")
    await barrier(ctx, bot)
    event = manipulate.events[0]
    assert event["slot"] == "HEAD" and event["armor_stand_item"]["type"] == "minecraft:diamond_helmet", event
    assert event["player_item"]["type"] == "minecraft:air", event
    assert held(player) == "minecraft:diamond_helmet", f"the player holds {held(player)} after taking the helmet"


def with_player(name: str):
    return lambda e: any(a.name == name for a in e.actors)


async def shove_pig(ctx: Context, name: str, **options):
    bot, player, spot = await join(ctx, name, dx=5, dz=8)
    ox = spot[0] - ctx.plot.pos(0, 0, 0)[0]
    pig = await spawn(ctx, bot, "pig", ox + 3, 8.5)
    bumps = ctx.capture(ActorCollideWithActorEvent, with_player(name), **options)
    start = where(pig)
    await bot.call("local_player.move_to", [start[0] - 0.4, start[1], start[2]])
    await poll(ctx, lambda: bot.call("local_player.is_moving"), lambda m: m is False, timeout=120, what="the bot to arrive")
    await barrier(ctx, bot)
    return bot, player, pig, start, bumps


@scenario("pact.collide.walk", covers={ActorCollideWithActorEvent: "normal"}, plot=True, budget=BUDGET)
async def collide_walk(ctx: Context) -> None:
    bot, player, pig, start, bumps = await shove_pig(ctx, "pcolw")
    assert bumps.count >= 1, "walking into a pig raised no ActorCollideWithActorEvent"
    assert {a["type"] for a in bumps.events[0]["actors"]} == {"minecraft:player", "minecraft:pig"}, bumps.events[0]
    assert where(pig)[0] - start[0] > 0.2, f"the pig was not pushed along: {start} -> {where(pig)}"


@scenario("pact.collide.walk_cancel", covers={ActorCollideWithActorEvent: "cancel"}, plot=True, budget=BUDGET)
async def collide_walk_cancel(ctx: Context) -> None:
    bot, player, pig, start, bumps = await shove_pig(ctx, "pcolwno", cancel=True)
    assert bumps.count >= 1, "walking into a pig raised no ActorCollideWithActorEvent"
    assert flat(where(pig), start) < 0.1, f"a cancelled collision still pushed the pig: {start} -> {where(pig)}"


async def sit_in_boat(ctx: Context, name: str):
    bot, player, boat, _ = await stage(ctx, name, "boat", None, slow=False)
    uid = (await client_actor(bot, "minecraft:boat", boat.runtime_id))["unique_id"]
    index = await mark(bot)
    await bot.call("game_mode.interact", boat.runtime_id)
    await poll(
        ctx, lambda: packets(bot, "SetActorLinkPacket", index), lambda f: links(f, uid), what="the client to be seated"
    )
    return bot, player, boat, uid


async def leave_boat(bot) -> None:
    try:
        await bot.call("local_player.leave_vehicle")
    except HarnessError as e:
        if "leave_vehicle" in str(e):
            raise Skip(NO_DISMOUNT) from e
        raise


def left(found: list[dict], uid: int) -> list[dict]:
    return [p for p in links(found, uid) if p["packet"]["link"]["type"] != "RIDING"]


@scenario("pact.dismount.sneak", covers={ActorDismountEvent: "normal"}, plot=True, budget=BUDGET)
async def dismount_sneak(ctx: Context) -> None:
    bot, player, boat, uid = await sit_in_boat(ctx, "pdis")
    leaves = ctx.capture(ActorDismountEvent, lambda e: e.actor.name == "pdis")
    index = await mark(bot)
    await leave_boat(bot)
    await ctx.until(lambda: leaves.count, timeout=60, what="ActorDismountEvent")
    await poll(
        ctx, lambda: packets(bot, "SetActorLinkPacket", index), lambda f: left(f, uid), what="the client to be told it left"
    )
    assert leaves.count == 1 and leaves.events[0]["dismounted"]["type"] == "minecraft:boat", leaves.events
    assert not seated(ctx), "the player is still in the boat"


@scenario("pact.dismount.sneak_cancel", covers={ActorDismountEvent: "cancel"}, plot=True, budget=BUDGET)
async def dismount_sneak_cancel(ctx: Context) -> None:
    bot, player, boat, uid = await sit_in_boat(ctx, "pdisno")
    leaves = ctx.capture(ActorDismountEvent, lambda e: e.actor.name == "pdisno", cancel=True)
    index = await mark(bot)
    await leave_boat(bot)
    await ctx.until(lambda: leaves.count, timeout=60, what="ActorDismountEvent")
    await barrier(ctx, bot)
    assert not left(await packets(bot, "SetActorLinkPacket", index), uid), "the client was told it left the boat"
    ctx.release(leaves)
    assert seated(ctx), "a cancelled dismount still took the player out of the boat"


def pickups(name: str):
    return lambda e: e.player.name == name


@scenario(
    "pact.pickup.player",
    covers={PlayerPickupItemEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def pickup_player(ctx: Context) -> None:
    bot, player, spot = await join(ctx, "ppick", dx=8, dz=8)
    by_player = ctx.capture(PlayerPickupItemEvent, pickups("ppick"))
    by_actor = ctx.capture(ActorPickupItemEvent, lambda e: str(e.actor.type) == "minecraft:player")
    ground = Location(ctx.overworld, spot[0], spot[1], spot[2] + 3)
    drop = ctx.overworld.drop_item(ground, ItemStack("minecraft:gold_ingot", 3))
    dropped = drop.runtime_id
    ctx.run(f"tp ppick {spot[0]} {spot[1]} {spot[2] + 3}")
    await ctx.until(lambda: by_player.count, timeout=120, what="PlayerPickupItemEvent")
    await barrier(ctx, bot)
    assert by_player.count == 1, by_player.events
    assert by_player.events[0]["item"]["runtime_id"] == dropped, by_player.events[0]
    assert by_actor.count == 0, f"a player's pickup also raised ActorPickupItemEvent: {by_actor.events}"
    inventory = await bot.call("local_player.get_inventory_json")
    total = sum(s["count"] for s in inventory["inventory"]["slots"] if s["id"] == "minecraft:gold_ingot")
    assert total == 3, f"the client holds {total} gold ingots"


@scenario("pact.pickup.player_cancel", covers={PlayerPickupItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def pickup_player_cancel(ctx: Context) -> None:
    bot, player, spot = await join(ctx, "ppickno", dx=8, dz=8)
    by_player = ctx.capture(PlayerPickupItemEvent, pickups("ppickno"), cancel=True)
    drop = ctx.overworld.drop_item(Location(ctx.overworld, spot[0], spot[1], spot[2] + 3), ItemStack("minecraft:gold_ingot", 3))
    ctx.run(f"tp ppickno {spot[0]} {spot[1]} {spot[2] + 3}")
    await ctx.until(lambda: by_player.count, timeout=120, what="PlayerPickupItemEvent")
    await barrier(ctx, bot)
    assert drop.is_valid, "a cancelled pickup still removed the item"
    inventory = await bot.call("local_player.get_inventory_json")
    assert not any(s["id"] == "minecraft:gold_ingot" for s in inventory["inventory"]["slots"])
    assert not ctx.command("clear ppickno gold_ingot 0").success, "the server gave the player the ingot"


@scenario(
    "pact.drop.piglin",
    covers={PlayerDropItemEvent: "normal", ActorPickupItemEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def drop_piglin(ctx: Context) -> None:
    bot, player, spot = await join(ctx, "pdrop", dx=8, dz=4)
    ox = spot[0] - ctx.plot.pos(0, 0, 0)[0]
    drops = ctx.capture(PlayerDropItemEvent, mine("pdrop"))
    by_actor = ctx.capture(ActorPickupItemEvent, lambda e: str(e.actor.type) == "minecraft:piglin")
    by_player = ctx.capture(PlayerPickupItemEvent, pickups("pdrop"))
    piglin = await spawn(ctx, bot, "piglin", ox, 6.0)
    await give(ctx, bot, "pdrop", "gold_ingot")
    await bot.call("local_player.drop_selected_item", True)
    await ctx.until(lambda: drops.count, timeout=40, what="PlayerDropItemEvent")
    await ctx.until(lambda: by_actor.count, timeout=300, what="the piglin to pick the ingot up")
    await barrier(ctx, bot)
    assert drops.count == 1 and drops.events[0]["item"]["type"] == "minecraft:gold_ingot", drops.events
    assert by_actor.events[0]["actor"]["runtime_id"] == piglin.runtime_id, by_actor.events[0]
    assert by_actor.events[0]["remaining"] == 0, by_actor.events[0]
    assert not by_player.count, "the player's own drop was picked up by the player"


@scenario("pact.drop.cancel", covers={PlayerDropItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def drop_cancel(ctx: Context) -> None:
    bot, player, spot = await join(ctx, "pdropno", dx=8, dz=8)
    drops = ctx.capture(PlayerDropItemEvent, mine("pdropno"), cancel=True)
    await give(ctx, bot, "pdropno", "gold_ingot")
    await bot.call("local_player.drop_selected_item", True)
    await ctx.until(lambda: drops.count, timeout=40, what="PlayerDropItemEvent")
    await barrier(ctx, bot)
    assert not ground_items(ctx), "a cancelled drop still spawned an item"
    assert held(player) == "minecraft:gold_ingot"
    assert await held_by_client(bot) == "minecraft:gold_ingot"


