from endstone import GameMode
from endstone._python.block import BlockList
from endstone._python.event import Cancellable
from endstone.command import CommandSenderWrapper
from endstone.event import (
    ActorTeleportEvent,
    BlockExplodeEvent,
    BlockFormEvent,
    BlockGrowEvent,
    BlockPistonEvent,
    BlockPistonExtendEvent,
    BlockPistonRetractEvent,
    BroadcastMessageEvent,
    ServerListPingEvent,
)
from endstone.level import Location

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.actor import pig, plot_filter
from endstone_test.scenarios.block import BLAST, EAST, WEST, at_pos, lava_meets_water
from endstone_test.scenarios.server import advertisement, delivered
from endstone_test.scenarios.world import piston_with_stone, power


class Checks:
    def __init__(self) -> None:
        self.failed: list[str] = []

    def expect(self, condition, what: str) -> None:
        if not condition:
            self.failed.append(what)

    def raises(self, error: type[BaseException], action, what: str) -> None:
        try:
            action()
        except error:
            return
        except BaseException as e:  # noqa: BLE001
            self.failed.append(f"{what}: raised {type(e).__name__} instead of {error.__name__}")
            return
        self.failed.append(f"{what}: did not raise {error.__name__}")

    def done(self, *captures) -> None:
        for capture in captures:
            if capture.error is not None:
                raise capture.error
        assert not self.failed, self.failed


async def broadcast_round(ctx: Context, text: str, mutate) -> list[str]:
    capture = ctx.capture(BroadcastMessageEvent, lambda e: e.message == text, mutate=mutate)
    lines = await ctx.log_of(lambda: ctx.server.broadcast_message(text))
    assert capture.count == 1, capture.events
    ctx.release(capture)
    if capture.error is not None:
        raise capture.error
    return lines


@scenario("apis.events.cancellable.set", budget=200)
async def cancellable_set(ctx: Context) -> None:
    checks = Checks()
    text = "endstone-test cancellable set"

    def cancel_by_setter(event) -> None:
        checks.expect(Cancellable.is_cancelled.__get__(event) is False, "a fresh event was cancelled")
        Cancellable.is_cancelled.__set__(event, True)
        checks.expect(Cancellable.is_cancelled.__get__(event) is True, "the base setter did not cancel")

    lines = await broadcast_round(ctx, text, cancel_by_setter)
    checks.done()
    assert not delivered(lines, text), "a broadcast cancelled through Cancellable.is_cancelled was delivered"


@scenario("apis.events.cancellable.cancel", budget=200)
async def cancellable_cancel(ctx: Context) -> None:
    checks = Checks()
    text = "endstone-test cancellable cancel"

    def cancel_by_method(event) -> None:
        Cancellable.cancel(event)
        checks.expect(Cancellable.is_cancelled.__get__(event) is True, "Cancellable.cancel did not cancel")

    lines = await broadcast_round(ctx, text, cancel_by_method)
    checks.done()
    assert not delivered(lines, text), "a broadcast cancelled through Cancellable.cancel was delivered"


@scenario("apis.events.cancellable.restore", budget=200)
async def cancellable_restore(ctx: Context) -> None:
    checks = Checks()
    text = "endstone-test cancellable restore"

    def cancel_then_restore(event) -> None:
        Cancellable.cancel(event)
        Cancellable.is_cancelled.__set__(event, False)
        checks.expect(Cancellable.is_cancelled.__get__(event) is False, "the base setter did not uncancel")

    lines = await broadcast_round(ctx, text, cancel_then_restore)
    checks.done()
    assert delivered(lines, text), "a broadcast uncancelled through Cancellable.is_cancelled was dropped"


@scenario("apis.events.recipients.algebra", budget=300)
async def recipients_algebra(ctx: Context) -> None:
    checks = Checks()
    text = "endstone-test recipients algebra"
    console = ctx.server.command_sender
    got = {name: [] for name in "abc"}
    senders = {
        name: CommandSenderWrapper(console, on_message=lambda m, name=name: got[name].append(ctx.text(m)))
        for name in "abc"
    }
    wa, wb, wc = senders["a"], senders["b"], senders["c"]
    outcome: dict = {}

    def probe(event) -> None:
        r = event.recipients
        checks.expect(console in r, "the console is not among the broadcast recipients")
        r.intersection_update({console})
        checks.expect(set(r) == {console}, "intersection_update did not narrow the recipients")
        r.add(wa)
        r.add(wb)
        full = {console, wa, wb}
        checks.expect(len(r) == 3, "len")
        checks.expect(r == full and full == r, "__eq__")
        checks.expect(r != {console} and {console} != r, "__ne__")
        checks.expect(r & {wa, wc} == {wa}, "__and__")
        checks.expect({wa, wc} & r == {wa}, "__rand__")
        checks.expect(r | {wc} == full | {wc}, "__or__")
        checks.expect({wc} | r == full | {wc}, "__ror__")
        checks.expect(r ^ {wa, wc} == {console, wb, wc}, "__xor__")
        checks.expect({wa, wc} ^ r == {console, wb, wc}, "__rxor__")
        checks.expect({console, wc} - r == {wc}, "__rsub__")
        checks.expect(r - {wa} == {console, wb}, "__sub__")
        checks.expect(r <= full and r <= r and not r <= {console, wa}, "__le__")
        checks.expect(r < full | {wc} and not r < full, "__lt__")
        checks.expect(r >= {console, wa} and r >= r and not r >= {wc}, "__ge__")
        checks.expect(r > {wa} and not r > full, "__gt__")
        checks.expect(full | {wc} > r and {wa} < r, "reflected __gt__ and __lt__")
        checks.expect(r.union({wc}) == full | {wc}, "union")
        checks.expect(r.intersection({console, wc}) == {console}, "intersection")
        checks.expect(r.difference({wa, wb}) == {console}, "difference")
        checks.expect(r.symmetric_difference({wa, wc}) == {console, wb, wc}, "symmetric_difference")
        checks.expect(r.issubset(full | {wc}) and not r.issubset({console}), "issubset")
        checks.expect(r.issuperset({wa, wb}) and not r.issuperset({wc}), "issuperset")
        checks.expect(r.isdisjoint({wc}) and not r.isdisjoint({wa, wc}), "isdisjoint")
        checks.expect(set(r) == full, "a read-only operation changed the recipients")
        r.difference_update({wb})
        checks.expect(set(r) == {console, wa}, "difference_update")
        r.intersection_update({console, wa, wc})
        checks.expect(set(r) == {console, wa}, "intersection_update")
        r ^= {wa, wc}
        checks.expect(set(r) == {console, wc}, "__ixor__")
        r &= {wb, wc}
        checks.expect(set(r) == {wc}, "__iand__")
        r.add(wa)
        r.add(wb)
        popped = r.pop()
        checks.expect(popped in {wa, wb, wc} and popped not in r and len(r) == 2, "pop")
        checks.raises(KeyError, lambda: r.remove(popped), "remove of a recipient that was popped")
        r.remove(next(iter(r)))
        checks.expect(len(r) == 1, "remove")
        outcome["last"] = next(iter(r))

    lines = await broadcast_round(ctx, text, probe)
    checks.done()
    last = outcome["last"]
    for name, sender in senders.items():
        expected = [text] if sender is last else []
        assert got[name] == expected, (name, got[name], expected)
    assert delivered(lines, text) == (last is console), (last is console, lines)


@scenario("apis.events.recipients.clear", budget=300)
async def recipients_clear(ctx: Context) -> None:
    checks = Checks()
    text = "endstone-test recipients clear"
    console = ctx.server.command_sender
    got: list[str] = []
    wrapper = CommandSenderWrapper(console, on_message=lambda m: got.append(ctx.text(m)))

    def wipe(event) -> None:
        r = event.recipients
        r.add(wrapper)
        checks.expect(len(r) >= 2, "the wrapper was not added")
        r.clear()
        checks.expect(len(r) == 0 and set(r) == set(), "clear")

    lines = await broadcast_round(ctx, text, wipe)
    checks.done()
    assert got == [], got
    assert not delivered(lines, text), lines


POSITIONS = {
    "east": EAST,
    "west": WEST,
    "north": (8, 1, 6),
    "south": (8, 1, 10),
    "far": (13, 1, 13),
    "farther": (3, 1, 13),
    "away": (13, 1, 3),
}
PLANKS = ("east", "west", "north", "south")
DIRT = ("far", "farther", "away")


@scenario("apis.events.block_list.edit", plot=True, budget=300)
async def block_list_edit(ctx: Context) -> None:
    checks = Checks()
    spots = {name: ctx.plot.pos(*offset) for name, offset in POSITIONS.items()}
    for name in PLANKS:
        ctx.run(f"setblock {ctx.plot.at(*POSITIONS[name])} oak_planks")
    for name in DIRT:
        ctx.run(f"setblock {ctx.plot.at(*POSITIONS[name])} dirt")

    def where(block) -> tuple[int, int, int]:
        return block.x, block.y, block.z

    def edit(event) -> None:
        bl = event.block_list
        size = len(bl)
        checks.expect(size >= 4 and bool(bl), "the blast did not list the planks")
        found = {where(b): b for b in bl}
        east, west, north, south = (found[spots[n]] for n in PLANKS)
        outsider = ctx.overworld.get_block_at(*spots["far"])
        checks.expect(east in bl and bl.count(east) == 1, "__contains__ or count of a listed block")
        checks.expect(outsider not in bl and bl.count(outsider) == 0, "__contains__ or count of an unlisted block")
        checks.expect(where(bl[0]) in found and where(bl[-1]) in found, "__getitem__")
        checks.expect(len(bl[0:2]) == 2, "__getitem__ with a slice")

        bl.remove(east)
        checks.expect(len(bl) == size - 1 and east not in bl, "remove")
        checks.raises(ValueError, lambda: bl.remove(east), "remove of a block that is gone")

        del bl[[where(b) for b in bl].index(spots["west"])]
        checks.expect(len(bl) == size - 2 and west not in bl, "__delitem__")

        bl[[where(b) for b in bl].index(spots["north"])] = ctx.overworld.get_block_at(*spots["away"])
        checks.expect(north not in bl, "__setitem__ left the replaced block listed")
        checks.expect(any(where(b) == spots["away"] for b in bl), "__setitem__ did not list the new block")

        taken = bl.pop([where(b) for b in bl].index(spots["south"]))
        checks.expect(where(taken) == spots["south"] and taken not in bl, "pop")
        bl.insert(0, taken)
        checks.expect(where(bl[0]) == spots["south"], "insert")

        bl.append(ctx.overworld.get_block_at(*spots["far"]))
        bl.extend([ctx.overworld.get_block_at(*spots["farther"])])
        checks.expect(any(where(b) == spots["far"] for b in bl), "append")
        checks.expect(any(where(b) == spots["farther"] for b in bl), "extend")

        kept = list(bl)
        bl.clear()
        checks.expect(len(bl) == 0 and not bl, "clear")
        bl.extend(kept)
        checks.expect(len(bl) == len(kept), "extend after clear")
        checks.expect(bl == BlockList(kept) and not bl != BlockList(kept), "__eq__")
        checks.expect(bl != BlockList([]) and not bl == BlockList([]), "__ne__")

    explode = ctx.capture(BlockExplodeEvent, at_pos(ctx.plot.pos(*BLAST)), mutate=edit)
    x, y, z = ctx.plot.pos(*BLAST)
    ctx.run(f"scriptevent endstone_test:explode {x + 0.5} {y + 0.5} {z + 0.5} 3")
    await ctx.until(lambda: explode.count, timeout=40, what="BlockExplodeEvent")
    await ctx.ticks(5)
    checks.done(explode)
    at = ctx.plot.at
    for name in ("east", "west", "north"):
        assert ctx.block_is(at(*POSITIONS[name]), "oak_planks"), f"the {name} plank was destroyed although it left the list"
    assert ctx.block_is(at(*POSITIONS["south"]), "air"), "a plank that stayed in the list survived"
    for name in ("far", "farther", "away"):
        assert ctx.block_is(at(*POSITIONS[name]), "air"), f"the {name} block was added to the list but not destroyed"


@scenario("apis.events.grow.set", plot=True, budget=300)
async def grow_set(ctx: Context) -> None:
    checks = Checks()

    def cancel_by_setter(event) -> None:
        checks.expect(BlockGrowEvent.is_cancelled.__get__(event) is False, "a fresh event was cancelled")
        BlockGrowEvent.is_cancelled.__set__(event, True)
        checks.expect(BlockGrowEvent.is_cancelled.__get__(event) is True, "the base setter did not cancel")

    form = ctx.capture(BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)), mutate=cancel_by_setter)
    at = await lava_meets_water(ctx, form)
    await ctx.ticks(40)
    checks.done(form)
    assert ctx.block_is(at, "lava"), "the lava solidified although BlockGrowEvent.is_cancelled was set"


@scenario("apis.events.grow.cancel", plot=True, budget=300)
async def grow_cancel(ctx: Context) -> None:
    checks = Checks()

    def cancel_by_method(event) -> None:
        BlockGrowEvent.cancel(event)
        checks.expect(BlockGrowEvent.is_cancelled.__get__(event) is True, "BlockGrowEvent.cancel did not cancel")

    form = ctx.capture(BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)), mutate=cancel_by_method)
    at = await lava_meets_water(ctx, form)
    await ctx.ticks(40)
    checks.done(form)
    assert ctx.block_is(at, "lava"), "the lava solidified although BlockGrowEvent.cancel was called"


@scenario("apis.events.grow.restore", plot=True, budget=300)
async def grow_restore(ctx: Context) -> None:
    checks = Checks()

    def cancel_then_restore(event) -> None:
        BlockGrowEvent.cancel(event)
        BlockGrowEvent.is_cancelled.__set__(event, False)
        checks.expect(BlockGrowEvent.is_cancelled.__get__(event) is False, "the base setter did not uncancel")

    form = ctx.capture(BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)), mutate=cancel_then_restore)
    at = await lava_meets_water(ctx, form)
    await ctx.until(lambda: ctx.block_is(at, "obsidian"), timeout=40, what="obsidian after the cancel was undone")
    checks.done(form)


@scenario("apis.events.piston.set", plot=True, budget=300)
async def piston_set(ctx: Context) -> None:
    checks = Checks()

    def cancel_by_setter(event) -> None:
        checks.expect(BlockPistonEvent.is_cancelled.__get__(event) is False, "a fresh event was cancelled")
        BlockPistonEvent.is_cancelled.__set__(event, True)
        checks.expect(BlockPistonEvent.is_cancelled.__get__(event) is True, "the base setter did not cancel")

    extend = ctx.capture(BlockPistonExtendEvent, at_pos(ctx.plot.pos(8, 0, 8)), mutate=cancel_by_setter)
    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(lambda: extend.count, timeout=40, what="BlockPistonExtendEvent")
    await ctx.ticks(20)
    checks.done(extend)
    assert ctx.block_is(ctx.plot.at(8, 1, 8), "stone"), "an extension cancelled through the base setter moved the stone"


@scenario("apis.events.piston.cancel", plot=True, budget=400)
async def piston_cancel(ctx: Context) -> None:
    checks = Checks()
    gate = {"on": False}

    def cancel_by_method(event) -> None:
        if gate["on"]:
            BlockPistonEvent.cancel(event)
            checks.expect(BlockPistonEvent.is_cancelled.__get__(event) is True, "BlockPistonEvent.cancel did not cancel")

    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(lambda: ctx.block_is(ctx.plot.at(8, 2, 8), "stone"), timeout=40, what="the stone to be pushed up")
    retract = ctx.capture(BlockPistonRetractEvent, at_pos(ctx.plot.pos(8, 0, 8)), mutate=cancel_by_method)
    gate["on"] = True
    await power(ctx, False)
    await ctx.until(lambda: retract.count, timeout=40, what="BlockPistonRetractEvent")
    await ctx.ticks(20)
    checks.done(retract)
    assert ctx.block_is(ctx.plot.at(8, 2, 8), "stone"), "a retraction cancelled through the base method pulled the stone"


@scenario("apis.events.piston.restore", plot=True, budget=300)
async def piston_restore(ctx: Context) -> None:
    checks = Checks()

    def cancel_then_restore(event) -> None:
        BlockPistonEvent.cancel(event)
        BlockPistonEvent.is_cancelled.__set__(event, False)
        checks.expect(BlockPistonEvent.is_cancelled.__get__(event) is False, "the base setter did not uncancel")

    extend = ctx.capture(BlockPistonExtendEvent, at_pos(ctx.plot.pos(8, 0, 8)), mutate=cancel_then_restore)
    await piston_with_stone(ctx)
    await power(ctx, True)
    await ctx.until(lambda: ctx.block_is(ctx.plot.at(8, 2, 8), "stone"), timeout=40, what="the stone to be pushed up")
    checks.done(extend)


def tp_pig(ctx: Context, dx: int, dz: int) -> None:
    ctx.run(f"tp @e[type=pig,{ctx.plot.volume}] {ctx.plot.at(dx, 0, dz)}")


def pig_at(ctx: Context, dx: int, dz: int) -> bool:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    return ctx.command(f"testfor @e[type=pig,x={x},y={y},z={z},dx=0,dy=1,dz=0]").success


@scenario("apis.events.teleport.from_location", plot=True, budget=300)
async def teleport_from_location(ctx: Context) -> None:
    checks = Checks()
    await pig(ctx)
    lie = ctx.plot.pos(14, 0, 14)

    def rewrite_from(event) -> None:
        event.from_location = Location(ctx.overworld, lie[0] + 0.5, lie[1], lie[2] + 0.5)

    teleport = ctx.capture(ActorTeleportEvent, plot_filter(ctx, "minecraft:pig"), mutate=rewrite_from)
    tp_pig(ctx, 2, 2)
    await ctx.until(lambda: teleport.count, timeout=20, what="the first ActorTeleportEvent")
    await ctx.ticks(2)
    assert pig_at(ctx, 2, 2), "rewriting from_location changed where the pig went"
    ctx.release(teleport)
    again = ctx.capture(ActorTeleportEvent, plot_filter(ctx, "minecraft:pig"))
    tp_pig(ctx, 12, 12)
    await ctx.until(lambda: again.count, timeout=20, what="the second ActorTeleportEvent")
    origin = again.events[0]["from_location"]
    start = ctx.plot.pos(2, 0, 2)
    assert (int(origin["x"]), int(origin["z"])) == (start[0], start[2]), (origin, start)
    checks.done(teleport, again)


@scenario("apis.events.list_ping.identity")
async def list_ping_identity(ctx: Context) -> None:
    def rewrite(event) -> None:
        event.server_guid = "424242"
        event.local_port = 24680
        event.game_mode = GameMode.SPECTATOR

    ping = ctx.capture(ServerListPingEvent, mutate=rewrite)
    parts = advertisement(await ctx.call("ping"))
    ctx.info["advertisement"] = parts
    assert ping.count == 1, ping.events
    assert ping.error is None, ping.error
    assert parts[8] == "Spectator", parts
    if len(parts) > 10:
        assert parts[6] == "424242", parts
        assert parts[10] == "24680", parts
