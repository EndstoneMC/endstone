from endstone.event import ActorDamageEvent

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.actor import pig, plot_filter


async def damaged_with(ctx: Context, command: str) -> dict:
    target = await pig(ctx)
    seen: list[dict] = []

    def record(event) -> None:
        source = event.damage_source
        seen.append(
            {
                "text": str(source),
                "type": source.type,
                "indirect": source.is_indirect,
                "actor": source.actor.id if source.actor else None,
                "damaging": source.damaging_actor.id if source.damaging_actor else None,
            }
        )

    damage = ctx.capture(ActorDamageEvent, plot_filter(ctx, "minecraft:pig"), mutate=record)
    ctx.run(command.format(volume=ctx.plot.volume))
    await ctx.until(lambda: damage.count, timeout=20, what="ActorDamageEvent")
    assert len(seen) == 1, seen
    seen[0]["target"] = target.id
    return seen[0]


@scenario("api.actors.damage_source.sourceless", plot=True, budget=300)
async def damage_source_sourceless(ctx: Context) -> None:
    got = await damaged_with(ctx, "damage @e[type=pig,{volume}] 2 fall")
    ctx.info["got"] = got
    assert got["type"] == "fall", got
    assert got["actor"] is None and got["damaging"] is None, got
    assert got["indirect"] is False, got
    assert got["text"] == "DamageSource(type=fall, actor=None, damaging_actor=None)", got


async def cow(ctx: Context):
    ctx.run(f"summon cow {ctx.plot.at(ctx.plot.middle[0] + 3, ctx.plot.middle[1], ctx.plot.middle[2])}")
    found = await ctx.until(
        lambda: [a for a in ctx.level.actors if str(a.type) == "minecraft:cow"],
        timeout=20,
        what="the summoned cow",
    )
    return found[0]


@scenario("api.actors.damage_source.attacker", plot=True, budget=300)
async def damage_source_attacker(ctx: Context) -> None:
    attacker = await cow(ctx)
    got = await damaged_with(ctx, "damage @e[type=pig,{volume}] 2 entity_attack entity @e[type=cow,{volume}]")
    ctx.info["got"] = got
    assert got["type"] == "entity_attack", got
    assert got["actor"] == attacker.id and got["damaging"] == attacker.id, got
    assert got["indirect"] is False, got
    assert got["text"].startswith("DamageSource(type=entity_attack, actor="), got
    assert "actor=None" not in got["text"], got


@scenario("api.actors.damage_source.projectile", plot=True, budget=300)
async def damage_source_projectile(ctx: Context) -> None:
    await cow(ctx)
    got = await damaged_with(ctx, "damage @e[type=pig,{volume}] 2 projectile entity @e[type=cow,{volume}]")
    ctx.info["got"] = got
    assert got["type"] == "projectile", got
    assert got["indirect"] is (got["actor"] != got["damaging"]), got
    assert got["actor"] is not None, got
