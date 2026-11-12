from endstone.actor import Actor, Mob
from endstone.event import (
    ActorDamageEvent,
    ActorDeathEvent,
    ActorEffectEvent,
    ActorPickupItemEvent,
    ActorRemoveEvent,
    ActorSpawnEvent,
    ActorTeleportEvent,
)
from endstone.inventory import ItemStack
from endstone.level import Location
from endstone.potion import EffectType

from endstone_test.harness import Context, scenario
from endstone_test.harness.plot import HEIGHT, SIZE


def in_plot(ctx: Context, actor: Actor) -> bool:
    location = actor.location
    plot = ctx.plot
    return (
        plot.x <= location.x < plot.x + SIZE
        and plot.y - 1 <= location.y < plot.y + HEIGHT
        and plot.z <= location.z < plot.z + SIZE
    )


def actors(ctx: Context, kind: str) -> list[Actor]:
    return [a for a in ctx.level.actors if str(a.type) == kind and in_plot(ctx, a)]


def plot_filter(ctx: Context, kind: str):
    return lambda e: str(e.actor.type) == kind and in_plot(ctx, e.actor)


async def pig(ctx: Context) -> Mob:
    """Summon one pig in the middle of the plot and return it."""
    ctx.run(f"summon pig {ctx.plot.at(*ctx.plot.middle)}")
    found = await ctx.until(
        lambda: actors(ctx, "minecraft:pig"), timeout=20, what="the summoned pig"
    )
    assert len(found) == 1, found
    return found[0]


def pig_exists(ctx: Context) -> bool:
    return ctx.command(f"testfor @e[type=pig,{ctx.plot.volume}]").success


# =============================================================================
# Spawn and remove
# =============================================================================


@scenario("actor.spawn.normal", covers={ActorSpawnEvent: "normal"}, plot=True)
async def spawn_normal(ctx: Context) -> None:
    spawn = ctx.capture(ActorSpawnEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"summon pig {ctx.plot.at(*ctx.plot.middle)}")
    await ctx.until(lambda: spawn.count, timeout=20, what="ActorSpawnEvent")
    await ctx.ticks(5)
    assert spawn.count == 1, spawn.events
    assert spawn.events[0]["actor"]["type"] == "minecraft:pig"
    assert pig_exists(ctx)


@scenario("actor.spawn.cancel", covers={ActorSpawnEvent: "cancel"}, plot=True)
async def spawn_cancel(ctx: Context) -> None:
    spawn = ctx.capture(ActorSpawnEvent, plot_filter(ctx, "minecraft:pig"), cancel=True)
    removed = ctx.capture(ActorRemoveEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.command(f"summon pig {ctx.plot.at(*ctx.plot.middle)}")
    await ctx.until(lambda: spawn.count, timeout=20, what="ActorSpawnEvent")
    await ctx.ticks(10)
    assert not pig_exists(ctx), "a pig whose spawn was cancelled is still in the world"
    assert removed.count == 0, (
        f"cancelling a spawn fired ActorRemoveEvent: {removed.events}"
    )


@scenario(
    "actor.remove.normal",
    covers={ActorRemoveEvent: "normal", ActorDeathEvent: "normal"},
    plot=True,
)
async def remove_normal(ctx: Context) -> None:
    await pig(ctx)
    death = ctx.capture(ActorDeathEvent, plot_filter(ctx, "minecraft:pig"))
    removed = ctx.capture(ActorRemoveEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"kill @e[type=pig,{ctx.plot.volume}]")
    await ctx.until(
        lambda: removed.count, timeout=100, what="ActorRemoveEvent after /kill"
    )
    assert death.count == 1, death.events
    assert death.events[0]["damage_source"], death.events[0]
    assert removed.count == 1
    assert not pig_exists(ctx)


# =============================================================================
# Damage
# =============================================================================


@scenario("actor.damage.normal", covers={ActorDamageEvent: "normal"}, plot=True)
async def damage_normal(ctx: Context) -> None:
    target = await pig(ctx)
    before = target.health
    damage = ctx.capture(ActorDamageEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"damage @e[type=pig,{ctx.plot.volume}] 3")
    await ctx.until(lambda: damage.count, timeout=20, what="ActorDamageEvent")
    await ctx.ticks(2)
    assert damage.count == 1, damage.events
    assert damage.events[0]["damage"] == 3
    assert target.health == before - 3, (before, target.health)


@scenario("actor.damage.cancel", covers={ActorDamageEvent: "cancel"}, plot=True)
async def damage_cancel(ctx: Context) -> None:
    target = await pig(ctx)
    before = target.health
    damage = ctx.capture(
        ActorDamageEvent, plot_filter(ctx, "minecraft:pig"), cancel=True
    )
    ctx.command(f"damage @e[type=pig,{ctx.plot.volume}] 3")
    await ctx.until(lambda: damage.count, timeout=20, what="ActorDamageEvent")
    await ctx.ticks(2)
    assert target.health == before, (
        f"a cancelled hit still took {before - target.health} health"
    )


@scenario("actor.damage.mutate", covers={ActorDamageEvent: "mutate"}, plot=True)
async def damage_mutate(ctx: Context) -> None:
    await pig(ctx)
    death = ctx.capture(ActorDeathEvent, plot_filter(ctx, "minecraft:pig"))
    damage = ctx.capture(
        ActorDamageEvent,
        plot_filter(ctx, "minecraft:pig"),
        mutate=lambda e: setattr(e, "damage", 100),
    )
    ctx.run(f"damage @e[type=pig,{ctx.plot.volume}] 1")
    await ctx.until(
        lambda: death.count, timeout=40, what="the pig to die of the raised damage"
    )
    assert (
        damage.events[0]["damage"] == 1 and damage.events[0]["after"]["damage"] == 100
    )


# =============================================================================
# Teleport
# =============================================================================


def pig_near(ctx: Context, dx: int, dz: int) -> bool:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    return ctx.command(f"testfor @e[type=pig,x={x},y={y},z={z},dx=0,dy=1,dz=0]").success


@scenario("actor.teleport.normal", covers={ActorTeleportEvent: "normal"}, plot=True)
async def teleport_normal(ctx: Context) -> None:
    await pig(ctx)
    teleport = ctx.capture(ActorTeleportEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"tp @e[type=pig,{ctx.plot.volume}] {ctx.plot.at(2, 0, 2)}")
    await ctx.until(lambda: teleport.count, timeout=20, what="ActorTeleportEvent")
    assert teleport.count == 1, teleport.events
    event = teleport.events[0]
    assert (
        int(event["to_location"]["x"]),
        int(event["to_location"]["z"]),
    ) == ctx.plot.pos(2, 0, 2)[::2]
    assert event["from_location"] != event["to_location"]
    assert pig_near(ctx, 2, 2), "the pig is not where it was teleported"


@scenario("actor.teleport.cancel", covers={ActorTeleportEvent: "cancel"}, plot=True)
async def teleport_cancel(ctx: Context) -> None:
    await pig(ctx)
    teleport = ctx.capture(
        ActorTeleportEvent, plot_filter(ctx, "minecraft:pig"), cancel=True
    )
    ctx.command(f"tp @e[type=pig,{ctx.plot.volume}] {ctx.plot.at(2, 0, 2)}")
    await ctx.until(lambda: teleport.count, timeout=20, what="ActorTeleportEvent")
    await ctx.ticks(2)
    assert not pig_near(ctx, 2, 2), "a cancelled teleport still moved the pig"


@scenario("actor.teleport.mutate", covers={ActorTeleportEvent: "mutate"}, plot=True)
async def teleport_mutate(ctx: Context) -> None:
    await pig(ctx)
    x, y, z = ctx.plot.pos(13, 0, 13)

    def elsewhere(event) -> None:
        event.to_location = Location(ctx.overworld, x + 0.5, y, z + 0.5)

    teleport = ctx.capture(
        ActorTeleportEvent, plot_filter(ctx, "minecraft:pig"), mutate=elsewhere
    )
    ctx.run(f"tp @e[type=pig,{ctx.plot.volume}] {ctx.plot.at(2, 0, 2)}")
    await ctx.until(lambda: teleport.count, timeout=20, what="ActorTeleportEvent")
    await ctx.ticks(2)
    assert pig_near(ctx, 13, 13), (
        "the pig did not go to the to_location the listener wrote"
    )
    assert not pig_near(ctx, 2, 2)


# =============================================================================
# Effects
# =============================================================================


@scenario("actor.effect.normal", covers={ActorEffectEvent: "normal"}, plot=True)
async def effect_normal(ctx: Context) -> None:
    target = await pig(ctx)
    effect = ctx.capture(ActorEffectEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"effect @e[type=pig,{ctx.plot.volume}] speed 30 1")
    await ctx.until(lambda: effect.count, timeout=20, what="ActorEffectEvent")
    assert effect.count == 1, effect.events
    event = effect.events[0]
    assert event["action"] == "ADDED"
    assert target.has_effect(EffectType.SPEED)
    assert target.get_effect(EffectType.SPEED).amplifier == 1


@scenario("actor.effect.cancel", covers={ActorEffectEvent: "cancel"}, plot=True)
async def effect_cancel(ctx: Context) -> None:
    target = await pig(ctx)
    effect = ctx.capture(
        ActorEffectEvent, plot_filter(ctx, "minecraft:pig"), cancel=True
    )
    ctx.command(f"effect @e[type=pig,{ctx.plot.volume}] speed 30 1")
    await ctx.until(lambda: effect.count, timeout=20, what="ActorEffectEvent")
    await ctx.ticks(2)
    assert not target.has_effect(EffectType.SPEED), "a cancelled effect was applied"


@scenario("actor.effect.fields", covers={ActorEffectEvent: "normal"}, plot=True)
async def effect_fields(ctx: Context) -> None:
    """A fresh effect reports no old effect, the new effect and its type."""
    target = await pig(ctx)
    seen: list[dict] = []

    def record(event) -> None:
        seen.append(
            {
                "action": event.action.name,
                "modified_type": str(event.modified_type),
                "old_effect": event.old_effect,
                "new_type": str(event.new_effect.type),
                "new_amplifier": event.new_effect.amplifier,
            }
        )

    effect = ctx.capture(
        ActorEffectEvent, plot_filter(ctx, "minecraft:pig"), mutate=record
    )
    ctx.run(f"effect @e[type=pig,{ctx.plot.volume}] speed 30 1")
    await ctx.until(lambda: effect.count, timeout=20, what="ActorEffectEvent")
    ctx.info["seen"] = seen
    assert seen == [
        {
            "action": "ADDED",
            "modified_type": "minecraft:speed",
            "old_effect": None,
            "new_type": "minecraft:speed",
            "new_amplifier": 1,
        }
    ], seen
    assert target.get_effect(EffectType.SPEED).amplifier == 1


@scenario("actor.effect.readd", covers={ActorEffectEvent: "normal"}, plot=True)
async def effect_readd(ctx: Context) -> None:
    """Re-adding an effect the mob already has reports CHANGED with the old effect."""
    target = await pig(ctx)
    effect = ctx.capture(ActorEffectEvent, plot_filter(ctx, "minecraft:pig"))
    ctx.run(f"effect @e[type=pig,{ctx.plot.volume}] speed 30 1")
    await ctx.until(lambda: effect.count, timeout=20, what="ActorEffectEvent")
    seen: list[tuple] = []
    ctx.capture(
        ActorEffectEvent,
        plot_filter(ctx, "minecraft:pig"),
        mutate=lambda e: seen.append(
            (e.action.name, e.old_effect.amplifier, e.new_effect.amplifier)
        ),
    )
    ctx.run(f"effect @e[type=pig,{ctx.plot.volume}] speed 30 4")
    await ctx.until(lambda: seen, timeout=20, what="ActorEffectEvent")
    ctx.info["events"] = seen
    assert seen == [("CHANGED", 1, 4)], seen
    assert target.get_effect(EffectType.SPEED).amplifier == 4


# =============================================================================
# Picking up items
# =============================================================================


async def gold_for_a_piglin(ctx: Context, pickup, amount: int = 1) -> None:
    ctx.run(f"summon piglin {ctx.plot.at(*ctx.plot.middle)}")
    await ctx.ticks(2)
    x, y, z = ctx.plot.pos(*ctx.plot.middle)
    ctx.overworld.drop_item(
        Location(ctx.overworld, x + 0.5, y + 0.2, z + 0.5),
        ItemStack("minecraft:gold_ingot", amount),
    )
    await ctx.until(lambda: pickup.count, timeout=100, what="ActorPickupItemEvent")
    await ctx.ticks(10)


def gold_on_ground(ctx: Context) -> bool:
    return ctx.command(f"testfor @e[type=item,{ctx.plot.volume}]").success


@scenario("actor.pickup.normal", covers={ActorPickupItemEvent: "normal"}, plot=True)
async def pickup_normal(ctx: Context) -> None:
    pickup = ctx.capture(ActorPickupItemEvent, plot_filter(ctx, "minecraft:piglin"))
    await gold_for_a_piglin(ctx, pickup)
    assert pickup.count == 1, pickup.events
    assert pickup.events[0]["remaining"] == 0, pickup.events
    assert not gold_on_ground(ctx), "the piglin did not take the gold"


@scenario("actor.pickup.remaining", plot=True)
async def pickup_remaining(ctx: Context) -> None:
    pickup = ctx.capture(ActorPickupItemEvent, plot_filter(ctx, "minecraft:piglin"))
    await gold_for_a_piglin(ctx, pickup, amount=5)
    left = sum(item.item_stack.amount for item in actors(ctx, "minecraft:item"))
    ctx.info.update(events=pickup.events, left_on_ground=left)
    assert pickup.events[0]["remaining"] == left, (pickup.events, left)


@scenario("actor.pickup.cancel", covers={ActorPickupItemEvent: "cancel"}, plot=True)
async def pickup_cancel(ctx: Context) -> None:
    pickup = ctx.capture(
        ActorPickupItemEvent, plot_filter(ctx, "minecraft:piglin"), cancel=True
    )
    await gold_for_a_piglin(ctx, pickup)
    assert gold_on_ground(ctx), "the piglin took gold although the pickup was cancelled"


def ground_count(ctx: Context, kind: str) -> int:
    return sum(
        a.item_stack.amount for a in actors(ctx, "minecraft:item") if str(a.item_stack.type) == kind
    )


async def drop_on(ctx: Context, target: Actor, pickup, kind: str, amount: int) -> None:
    loc = target.location
    ctx.overworld.drop_item(Location(ctx.overworld, loc.x, loc.y + 0.2, loc.z), ItemStack(kind, amount))
    await ctx.until(lambda: pickup.count, timeout=400, what="ActorPickupItemEvent")
    await ctx.ticks(10)


async def hopper_minecart(ctx: Context, slots: list[str]) -> Actor:
    ctx.run(f"summon hopper_minecart {ctx.plot.at(*ctx.plot.middle)}")
    found = await ctx.until(
        lambda: actors(ctx, "minecraft:hopper_minecart"), timeout=20, what="the hopper minecart"
    )
    for slot, item in enumerate(slots):
        ctx.run(f"replaceitem entity @e[type=hopper_minecart,{ctx.plot.volume}] slot.inventory {slot} {item}")
    await ctx.ticks(2)
    return found[0]


@scenario("actor.pickup.hopper_minecart_partial", plot=True)
async def pickup_hopper_minecart_partial(ctx: Context) -> None:
    pickup = ctx.capture(ActorPickupItemEvent, plot_filter(ctx, "minecraft:hopper_minecart"))
    cart = await hopper_minecart(ctx, ["dirt 60"] + ["stone 64"] * 4)
    await drop_on(ctx, cart, pickup, "minecraft:dirt", 10)
    left = ground_count(ctx, "minecraft:dirt")
    ctx.info.update(events=pickup.events, left_on_ground=left)
    assert pickup.events[0]["remaining"] == 6, pickup.events
    assert left == 6, left


@scenario("actor.pickup.hopper_minecart_full", plot=True)
async def pickup_hopper_minecart_full(ctx: Context) -> None:
    pickup = ctx.capture(ActorPickupItemEvent, plot_filter(ctx, "minecraft:hopper_minecart"))
    cart = await hopper_minecart(ctx, ["stone 64"] * 5)
    await drop_on(ctx, cart, pickup, "minecraft:dirt", 3)
    left = ground_count(ctx, "minecraft:dirt")
    ctx.info.update(events=pickup.events, left_on_ground=left)
    assert pickup.events[0]["remaining"] == 3, pickup.events
    assert left == 3, left
