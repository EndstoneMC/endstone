from endstone.block import Campfire, ItemFrame, Lectern, Side, Sign
from endstone.event import (
    ActorExplodeEvent,
    BlockCookEvent,
    BlockExplodeEvent,
    BlockFormEvent,
    CauldronLevelChangeEvent,
)
from endstone.inventory import ItemStack

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.actor import actors


def at_pos(xyz: tuple[int, int, int]):
    return lambda e: (e.block.x, e.block.y, e.block.z) == xyz


async def lava_meets_water(ctx: Context, form):
    """Pour water next to a lava source in the plot; returns the lava's "x y z"."""
    lava = ctx.plot.middle
    ctx.run(f"setblock {ctx.plot.at(*lava)} lava")
    ctx.run(f"setblock {ctx.plot.at(lava[0] + 1, lava[1], lava[2])} water")
    await ctx.until(lambda: form.count, timeout=40, what="BlockFormEvent at the lava")
    return ctx.plot.at(*lava)


@scenario("block.form.normal", covers={BlockFormEvent: "normal"}, plot=True)
async def form_normal(ctx: Context) -> None:
    form = ctx.capture(BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)))
    at = await lava_meets_water(ctx, form)
    await ctx.until(lambda: ctx.block_is(at, "obsidian"), timeout=20, what="obsidian")
    await ctx.ticks(10)
    assert form.count == 1, form.events
    event = form.events[0]
    assert event["block"]["type"] == "minecraft:lava"
    assert (
        event["block"]["x"],
        event["block"]["y"],
        event["block"]["z"],
    ) == ctx.plot.pos(*ctx.plot.middle)
    assert event["new_state"]["type"] == "minecraft:obsidian"


@scenario("block.form.cancel", covers={BlockFormEvent: "cancel"}, plot=True)
async def form_cancel(ctx: Context) -> None:
    form = ctx.capture(
        BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)), cancel=True
    )
    at = await lava_meets_water(ctx, form)
    await ctx.ticks(40)
    assert ctx.block_is(at, "lava"), (
        "the lava solidified although BlockFormEvent was cancelled"
    )


@scenario("block.form.mutate", covers={BlockFormEvent: "mutate"}, plot=True)
async def form_mutate(ctx: Context) -> None:
    def to_gold(event) -> None:
        event.new_state.type = "minecraft:gold_block"

    form = ctx.capture(
        BlockFormEvent, at_pos(ctx.plot.pos(*ctx.plot.middle)), mutate=to_gold
    )
    at = await lava_meets_water(ctx, form)
    await ctx.until(
        lambda: not ctx.block_is(at, "lava"), timeout=20, what="the lava to change"
    )
    assert ctx.block_is(at, "gold_block"), (
        "the new_state written by the listener was not placed"
    )


# =============================================================================
# Explosions
# =============================================================================

TNT = (8, 1, 8)
EAST = (10, 1, 8)
WEST = (6, 1, 8)


async def tnt_between_planks(ctx: Context, explode) -> tuple[str, str]:
    plot = ctx.plot
    east, west = plot.at(*EAST), plot.at(*WEST)
    ctx.run(f"setblock {east} oak_planks")
    ctx.run(f"setblock {west} oak_planks")
    ctx.run(f"summon tnt {plot.at(*TNT)}")
    await ctx.until(lambda: explode.count, timeout=120, what="the TNT to explode")
    await ctx.ticks(2)
    return east, west


def is_tnt(event) -> bool:
    return str(event.actor.type) == "minecraft:tnt"


def positions(event: dict) -> set[tuple[int, int, int]]:
    return {(b["x"], b["y"], b["z"]) for b in event["block_list"]}


@scenario("explode.tnt.normal", covers={ActorExplodeEvent: "normal"}, plot=True)
async def tnt_normal(ctx: Context) -> None:
    explode = ctx.capture(ActorExplodeEvent, is_tnt)
    east, west = await tnt_between_planks(ctx, explode)
    assert explode.count == 1, explode.events
    event = explode.events[0]
    assert event["actor"]["type"] == "minecraft:tnt"
    tnt = ctx.plot.pos(*TNT)
    assert (
        abs(event["location"]["x"] - (tnt[0] + 0.5)) < 1.5
        and abs(event["location"]["z"] - (tnt[2] + 0.5)) < 1.5
    )
    assert {ctx.plot.pos(*EAST), ctx.plot.pos(*WEST)} <= positions(event), event[
        "block_list"
    ]
    assert ctx.block_is(east, "air") and ctx.block_is(west, "air"), (
        "the explosion left a plank in the block list"
    )


@scenario("explode.tnt.cancel", covers={ActorExplodeEvent: "cancel"}, plot=True)
async def tnt_cancel(ctx: Context) -> None:
    explode = ctx.capture(ActorExplodeEvent, is_tnt, cancel=True)
    east, west = await tnt_between_planks(ctx, explode)
    assert ctx.block_is(east, "oak_planks") and ctx.block_is(west, "oak_planks")
    assert ctx.block_is(ctx.plot.at(8, -1, 8), "stone"), (
        "the floor under a cancelled explosion was destroyed"
    )


@scenario("explode.tnt.mutate", covers={ActorExplodeEvent: "mutate"}, plot=True)
async def tnt_mutate(ctx: Context) -> None:
    keep = ctx.plot.pos(*WEST)

    def spare_west(event) -> None:
        event.block_list = [b for b in event.block_list if (b.x, b.y, b.z) != keep]

    explode = ctx.capture(ActorExplodeEvent, is_tnt, mutate=spare_west)
    east, west = await tnt_between_planks(ctx, explode)
    assert keep in positions(explode.events[0])
    assert keep not in positions(explode.events[0]["after"])
    assert ctx.block_is(west, "oak_planks"), (
        "a block removed from block_list was destroyed anyway"
    )
    assert ctx.block_is(east, "air"), "a block left in block_list survived"


async def creeper_by_planks(ctx: Context, explode) -> str:
    plank = ctx.plot.at(*EAST)
    ctx.run(f"setblock {plank} oak_planks")
    ctx.run(f"summon creeper {ctx.plot.at(*TNT)} 0 0 minecraft:start_exploding_forced")
    await ctx.until(lambda: explode.count, timeout=200, what="the creeper to explode")
    await ctx.ticks(2)
    return plank


def is_creeper(event) -> bool:
    return str(event.actor.type) == "minecraft:creeper"


@scenario(
    "explode.creeper.normal",
    covers={ActorExplodeEvent: "normal"},
    plot=True,
    budget=300,
)
async def creeper_normal(ctx: Context) -> None:
    explode = ctx.capture(ActorExplodeEvent, is_creeper)
    plank = await creeper_by_planks(ctx, explode)
    assert explode.count == 1, explode.events
    assert ctx.plot.pos(*EAST) in positions(explode.events[0])
    assert ctx.block_is(plank, "air")


# =============================================================================
# Cauldrons
# =============================================================================

CAULDRON = (8, 0, 8)


async def dispense_water(ctx: Context, change) -> str:
    """Let water drip into a cauldron from pointed dripstone under a water source; no player is involved."""
    plot = ctx.plot
    at = plot.at(*CAULDRON)
    ctx.run(f"setblock {at} cauldron")
    ctx.run(f"setblock {plot.at(8, 4, 8)} water")
    ctx.run(f"setblock {plot.at(8, 3, 8)} dripstone_block")
    ctx.run(
        f'setblock {plot.at(8, 2, 8)} pointed_dripstone ["hanging"=true,"dripstone_thickness"="tip"]'
    )
    ctx.run("gamerule randomtickspeed 4096")
    try:
        await ctx.until(
            lambda: change.count, timeout=200, what="CauldronLevelChangeEvent"
        )
    finally:
        ctx.run("gamerule randomtickspeed 1")
        ctx.info["cauldron"] = str(
            ctx.overworld.get_block_at(*plot.pos(*CAULDRON)).data
        )
    await ctx.ticks(5)
    return at


def at_cauldron(ctx: Context):
    xyz = ctx.plot.pos(*CAULDRON)
    return lambda e: (e.block.x, e.block.y, e.block.z) == xyz


def holds_water(ctx: Context, at: str) -> bool:
    return not ctx.block_is(at, 'cauldron ["fill_level"=0]') and ctx.block_is(
        at, 'cauldron ["cauldron_liquid"="water"]'
    )


@scenario(
    "block.cauldron.normal",
    covers={CauldronLevelChangeEvent: "normal"},
    plot=True,
    budget=300,
)
async def cauldron_normal(ctx: Context) -> None:
    change = ctx.capture(CauldronLevelChangeEvent, at_cauldron(ctx))
    at = await dispense_water(ctx, change)
    event = change.events[0]
    assert event["actor"] is None and event["reason"] == "NATURAL_FILL", event
    data = event["new_state"]["data"]
    assert '"cauldron_liquid"="water"' in data and '"fill_level"=0' not in data, event[
        "new_state"
    ]
    assert holds_water(ctx, at), ctx.info


@scenario(
    "block.cauldron.rain",
    covers={CauldronLevelChangeEvent: "normal"},
    plot=True,
    budget=300,
)
async def cauldron_rain(ctx: Context) -> None:
    change = ctx.capture(CauldronLevelChangeEvent, at_cauldron(ctx))
    at = ctx.plot.at(*CAULDRON)
    ctx.run(f"setblock {at} cauldron")
    ctx.run("weather rain")
    ctx.run("gamerule randomtickspeed 4096")
    try:
        await ctx.until(
            lambda: change.count, timeout=200, what="CauldronLevelChangeEvent"
        )
    finally:
        ctx.run("gamerule randomtickspeed 1")
        ctx.run("weather clear")
    event = change.events[0]
    assert event["actor"] is None and event["reason"] == "NATURAL_FILL", event


@scenario(
    "block.cauldron.cancel",
    covers={CauldronLevelChangeEvent: "cancel"},
    plot=True,
    budget=300,
)
async def cauldron_cancel(ctx: Context) -> None:
    change = ctx.capture(CauldronLevelChangeEvent, at_cauldron(ctx), cancel=True)
    at = await dispense_water(ctx, change)
    assert ctx.block_is(at, 'cauldron ["fill_level"=0]'), (
        "a cancelled fill still changed the cauldron"
    )


@scenario(
    "block.cauldron.mutate",
    covers={CauldronLevelChangeEvent: "mutate"},
    plot=True,
    budget=300,
)
async def cauldron_mutate(ctx: Context) -> None:
    def third_full(event) -> None:
        event.new_state.data = ctx.server.create_block_data(
            "minecraft:cauldron", {"fill_level": 3, "cauldron_liquid": "water"}
        )

    change = ctx.capture(CauldronLevelChangeEvent, at_cauldron(ctx), mutate=third_full)
    at = await dispense_water(ctx, change)
    assert ctx.block_is(at, 'cauldron ["fill_level"=3,"cauldron_liquid"="water"]'), (
        ctx.command(f"testforblock {at} cauldron").output
    )


# =============================================================================
# Explosions with no actor behind them
# =============================================================================

BLAST = (8, 1, 8)


async def scripted_explosion(ctx: Context, explode) -> tuple[str, str]:
    """Planks either side of an explosion the behavior pack creates through Dimension.createExplosion."""
    east, west = ctx.plot.at(*EAST), ctx.plot.at(*WEST)
    ctx.run(f"setblock {east} oak_planks")
    ctx.run(f"setblock {west} oak_planks")
    x, y, z = ctx.plot.pos(*BLAST)
    ctx.run(f"scriptevent endstone_test:explode {x + 0.5} {y + 0.5} {z + 0.5} 3")
    await ctx.until(lambda: explode.count, timeout=40, what="BlockExplodeEvent")
    await ctx.ticks(2)
    return east, west


def blast_at(ctx: Context):
    return at_pos(ctx.plot.pos(*BLAST))


@scenario("explode.block.normal", covers={BlockExplodeEvent: "normal"}, plot=True)
async def block_explode_normal(ctx: Context) -> None:
    explode = ctx.capture(BlockExplodeEvent, blast_at(ctx))
    east, west = await scripted_explosion(ctx, explode)
    assert explode.count == 1, explode.events
    assert {ctx.plot.pos(*EAST), ctx.plot.pos(*WEST)} <= positions(explode.events[0]), (
        explode.events[0]
    )
    assert ctx.block_is(east, "air") and ctx.block_is(west, "air")


@scenario("explode.block.cancel", covers={BlockExplodeEvent: "cancel"}, plot=True)
async def block_explode_cancel(ctx: Context) -> None:
    explode = ctx.capture(BlockExplodeEvent, blast_at(ctx), cancel=True)
    east, west = await scripted_explosion(ctx, explode)
    assert ctx.block_is(east, "oak_planks") and ctx.block_is(west, "oak_planks")
    assert ctx.block_is(ctx.plot.at(8, -1, 8), "stone"), (
        "a cancelled explosion broke the floor"
    )


@scenario("explode.block.mutate", covers={BlockExplodeEvent: "mutate"}, plot=True)
async def block_explode_mutate(ctx: Context) -> None:
    keep = ctx.plot.pos(*WEST)

    def spare_west(event) -> None:
        event.block_list = [b for b in event.block_list if (b.x, b.y, b.z) != keep]

    explode = ctx.capture(BlockExplodeEvent, blast_at(ctx), mutate=spare_west)
    east, west = await scripted_explosion(ctx, explode)
    assert keep not in positions(explode.events[0]["after"])
    assert ctx.block_is(west, "oak_planks"), (
        "a block removed from block_list was destroyed anyway"
    )
    assert ctx.block_is(east, "air")


# =============================================================================
# Campfire cooking
# =============================================================================

FIRE = (8, 0, 8)


def campfire(ctx: Context) -> Campfire:
    state = ctx.overworld.get_block_at(*ctx.plot.pos(*FIRE)).capture_state()
    assert isinstance(state, Campfire), state
    return state


async def beef_on_campfire(ctx: Context, cook) -> None:
    """Raw beef laid on a lit campfire through the Campfire state, nearly done."""
    ctx.run(f"setblock {ctx.plot.at(*FIRE)} campfire")
    state = campfire(ctx)
    state.set_item(0, ItemStack("minecraft:beef"))
    state.set_cook_time(0, 580)
    assert state.update(True), "the campfire state did not apply"
    try:
        await ctx.until(lambda: cook.count, timeout=200, what="BlockCookEvent")
    finally:
        after = campfire(ctx)
        ctx.info["slot0"] = repr(after.get_item(0))
        ctx.info["cook_time"] = after.get_cook_time(0)
    await ctx.ticks(10)


def items_on_ground(ctx: Context) -> list[str]:
    return [
        f"{item.item_stack.type} x{item.item_stack.amount}"
        for item in actors(ctx, "minecraft:item")
    ]


@scenario("block.cook.normal", covers={BlockCookEvent: "normal"}, plot=True, budget=400)
async def cook_normal(ctx: Context) -> None:
    cook = ctx.capture(BlockCookEvent, at_pos(ctx.plot.pos(*FIRE)))
    await beef_on_campfire(ctx, cook)
    assert cook.count == 1, cook.events
    event = cook.events[0]
    assert (
        event["source"]["type"] == "minecraft:beef"
        and event["result"]["type"] == "minecraft:cooked_beef"
    ), event
    recipe = event["recipe"]
    assert recipe is not None, "no recipe for beef on a campfire"
    assert recipe["class"] == "CampfireRecipe" and recipe["tag"] == "campfire", recipe
    assert items_on_ground(ctx) == ["minecraft:cooked_beef x1"], items_on_ground(ctx)
    assert campfire(ctx).get_item(0) is None, "the cooked item stayed on the campfire"


@scenario("block.cook.cancel", covers={BlockCookEvent: "cancel"}, plot=True, budget=400)
async def cook_cancel(ctx: Context) -> None:
    cook = ctx.capture(BlockCookEvent, at_pos(ctx.plot.pos(*FIRE)), cancel=True)
    await beef_on_campfire(ctx, cook)
    assert items_on_ground(ctx) == [], (
        f"a cancelled cook dropped {items_on_ground(ctx)}"
    )
    left = campfire(ctx).get_item(0)
    assert left is not None and str(left.type) == "minecraft:beef", (
        f"a cancelled cook left {left!r}"
    )
    ctx.info["events_while_cancelled"] = cook.count


@scenario("block.cook.mutate", covers={BlockCookEvent: "mutate"}, plot=True, budget=400)
async def cook_mutate(ctx: Context) -> None:
    def golden(event) -> None:
        event.result = ItemStack("minecraft:golden_apple", 2)

    cook = ctx.capture(BlockCookEvent, at_pos(ctx.plot.pos(*FIRE)), mutate=golden)
    await beef_on_campfire(ctx, cook)
    assert cook.count == 1, cook.events
    assert items_on_ground(ctx) == ["minecraft:golden_apple x2"], items_on_ground(ctx)


# =============================================================================
# Block states: signs, item frames, lecterns
# =============================================================================

SIGN = (8, 0, 8)
FRONT, BACK = Side.FRONT, Side.BACK


def state_at(ctx: Context, xyz: tuple[int, int, int], kind: type):
    state = ctx.overworld.get_block_at(*ctx.plot.pos(*xyz)).capture_state()
    assert isinstance(state, kind), (
        f"{type(state).__name__} at {xyz}, expected {kind.__name__}"
    )
    return state


def sign_with_lines(ctx: Context, lines: list[str]) -> None:
    ctx.run(f"setblock {ctx.plot.at(*SIGN)} standing_sign")
    sign = state_at(ctx, SIGN, Sign)
    for i, line in enumerate(lines):
        sign.get_side(FRONT).set_line(i, line)
    assert sign.update(True)


@scenario("block.sign.newline", plot=True)
async def sign_newline(ctx: Context) -> None:
    """A newline set on one line must not leak into the others: a side always has four lines."""
    sign_with_lines(ctx, ["L0", "L1", "L2", "L3"])
    sign = state_at(ctx, SIGN, Sign)
    try:
        sign.get_side(FRONT).set_line(0, "a\nb")
        raised = None
    except ValueError as e:
        raised = str(e)
    kept = sign.get_side(FRONT).get_line(0)
    kept_lines = sign.get_side(FRONT).lines
    sign.update(True)
    after_update = sign.get_side(FRONT).get_line(0)
    front = state_at(ctx, SIGN, Sign).get_side(FRONT).lines
    back = state_at(ctx, SIGN, Sign).get_side(BACK).lines
    live = ctx.overworld.get_block_at(*ctx.plot.pos(*SIGN)).capture_state(use_snapshot=False)
    live.get_side(FRONT).set_line(1, "c\nd")
    live_line = live.get_side(FRONT).get_line(1)
    ctx.info.update(raised=raised, kept=kept, after_update=after_update, front=front, back=back, live_line=live_line)
    assert kept == "a\nb", f"the sign object dropped the text after the line break: {kept!r}"
    assert kept_lines == ["a\nb", "L1", "L2", "L3"], kept_lines
    assert after_update == "a", f"after update() the object still reads {after_update!r}, the world holds 'a'"
    assert front[0] == "a", f"the world kept {front[0]!r} for line 0"
    assert live_line == "c", f"a write-through state reads {live_line!r} where the world holds 'c'"
    assert len(front) == 4, f"the front reads {len(front)} lines: {front}"
    assert front[1:] == ["L1", "L2", "L3"], (
        f"setting line 0 changed the others: {front}"
    )
    assert "\n" not in front[0], f"line 0 kept a newline: {front[0]!r}"
    assert back == ["", "", "", ""], f"the back changed: {back}"


@scenario("block.sign.snapshot", plot=True)
async def sign_snapshot(ctx: Context) -> None:
    sign_with_lines(ctx, ["before", "", "", ""])
    snapshot = state_at(ctx, SIGN, Sign)
    assert snapshot.is_snapshot
    snapshot.get_side(FRONT).set_line(0, "after")
    snapshot.get_side(BACK).set_line(2, "back")
    snapshot.waxed = True
    live = state_at(ctx, SIGN, Sign)
    assert live.get_side(FRONT).get_line(0) == "before", (
        "editing a snapshot changed the sign in the world"
    )
    assert live.get_side(BACK).get_line(2) == "" and not live.waxed
    assert snapshot.get_side(FRONT).get_line(0) == "after", (
        "the snapshot lost its own edit"
    )
    assert snapshot.update(True)
    live = state_at(ctx, SIGN, Sign)
    assert live.get_side(FRONT).get_line(0) == "after"
    assert live.get_side(BACK).get_line(2) == "back" and live.waxed


FRAME = (8, 0, 8)


@scenario("block.item_frame.snapshot", plot=True)
async def item_frame_snapshot(ctx: Context) -> None:
    ctx.run(f'setblock {ctx.plot.at(*FRAME)} frame ["facing_direction"=1]')
    snapshot = state_at(ctx, FRAME, ItemFrame)
    assert snapshot.is_snapshot and str(snapshot.item.type) == "minecraft:air"
    snapshot.item = ItemStack("minecraft:diamond")
    assert str(state_at(ctx, FRAME, ItemFrame).item.type) == "minecraft:air", (
        "setting a snapshot's item filled the frame in the world"
    )
    held = snapshot.item
    ctx.info["snapshot_item"] = f"{held.type} x{held.amount}"
    assert str(held.type) == "minecraft:diamond", ctx.info
    assert snapshot.update(True)
    placed = state_at(ctx, FRAME, ItemFrame).item
    assert str(placed.type) == "minecraft:diamond", repr(placed)
    ctx.info["placed_amount"] = placed.amount


LECTERN = (8, 0, 8)


def book(pages: list[str]) -> ItemStack:
    item = ItemStack("minecraft:writable_book")
    meta = item.item_meta
    meta.pages = pages
    item.item_meta = meta
    return item


def lectern_page_after(ctx: Context, page: int) -> int:
    lectern = state_at(ctx, LECTERN, Lectern)
    lectern.page = page
    assert lectern.update(True)
    return state_at(ctx, LECTERN, Lectern).page


@scenario("block.lectern.page", plot=True)
async def lectern_page(ctx: Context) -> None:
    """The page is clamped to the book: 0 with no book or an empty one, the last page past the end."""
    ctx.run(f"setblock {ctx.plot.at(*LECTERN)} lectern")
    seen = {"no book, page 5": lectern_page_after(ctx, 5)}

    for pages in ([], ["one", "two"]):
        lectern = state_at(ctx, LECTERN, Lectern)
        lectern.snapshot_inventory.set_item(0, book(pages))
        assert lectern.update(True)
        held = state_at(ctx, LECTERN, Lectern).inventory.get_item(0)
        assert held is not None and str(held.type) == "minecraft:writable_book", repr(
            held
        )
        ctx.info[f"held_{len(pages)}"] = held.item_meta.pages
        seen[f"{len(pages)}-page book, page 5"] = lectern_page_after(ctx, 5)
        seen[f"{len(pages)}-page book, page 1"] = lectern_page_after(ctx, 1)

    seen["2-page book, page -1"] = lectern_page_after(ctx, -1)
    ctx.info["pages"] = seen
    assert seen == {
        "no book, page 5": 0,
        "0-page book, page 5": 0,
        "0-page book, page 1": 0,
        "2-page book, page 5": 1,
        "2-page book, page 1": 1,
        "2-page book, page -1": 0,
    }, seen


def sign_texts(packets: list[dict], xyz: tuple[int, int, int]) -> list[tuple[str, str]]:
    found = []
    for observed in packets:
        packet = observed["packet"]
        pos = packet.get("pos") or {}
        if (pos.get("x"), pos.get("y"), pos.get("z")) == xyz:
            data = packet.get("data") or {}
            found.append(
                (
                    data.get("FrontText", {}).get("Text"),
                    data.get("BackText", {}).get("Text"),
                )
            )
    return found


async def client_sign_after(ctx: Context, bot, edit) -> tuple[str, str]:
    since = await bot.call("get_next_packet_index")
    edit()
    await ctx.ticks(10)
    seen = sign_texts(
        await bot.call("get_received_packets", "BlockActorDataPacket", since=since),
        ctx.plot.pos(*SIGN),
    )
    assert seen, "the client got no BlockActorDataPacket for the sign"
    return seen[-1]


@scenario("block.sign.client_view", plot=True, budget=1200)
async def sign_client_view(ctx: Context) -> None:
    """What a client is sent for a sign edited through the API matches what the API reads back."""
    bot = await ctx.bot("SignReader")
    x, y, z = ctx.plot.pos(8, 0, 4)
    ctx.run(f"tp SignReader {x + 0.5} {y} {z + 0.5}")
    await ctx.ticks(60)

    sent = await client_sign_after(
        ctx, bot, lambda: sign_with_lines(ctx, ["one", "two", "three", "four"])
    )
    assert sent == ("one\ntwo\nthree\nfour", ""), sent

    def back_and_newline() -> None:
        sign = state_at(ctx, SIGN, Sign)
        sign.get_side(BACK).set_line(1, "behind")
        sign.get_side(FRONT).set_line(0, "a\nb")
        sign.update(True)

    front_text, back_text = await client_sign_after(ctx, bot, back_and_newline)
    live = state_at(ctx, SIGN, Sign)
    ctx.info["client"] = {"front": front_text, "back": back_text}
    ctx.info["api"] = {
        "front": live.get_side(FRONT).lines,
        "back": live.get_side(BACK).lines,
    }
    assert back_text.split("\n") == live.get_side(BACK).lines, ctx.info
    assert front_text.split("\n") == live.get_side(FRONT).lines, ctx.info



def dropped_near(ctx: Context, at, radius: float = 4.0) -> dict:
    found = {"items": 0, "orbs": 0}
    for actor in ctx.overworld.actors:
        loc = actor.location
        if abs(loc.x - at[0] - 0.5) > radius or abs(loc.y - at[1]) > radius or abs(loc.z - at[2] - 0.5) > radius:
            continue
        kind = str(actor.type)
        if kind == "minecraft:item":
            found["items"] += 1
        elif kind == "minecraft:xp_orb":
            found["orbs"] += 1
    return found


def furnace_slots(ctx: Context, at) -> list:
    state = ctx.overworld.get_block_at(*at).capture_state()
    return [None if item is None else (str(item.type), item.amount) for item in state.inventory.contents]


@scenario("blocks.furnace.update_twice", plot=True, budget=900)
async def furnace_update_twice(ctx: Context) -> None:
    at = ctx.plot.pos(6, 1, 6)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} furnace")
    await ctx.ticks(2)
    state = ctx.overworld.get_block_at(*at).capture_state()
    state.snapshot_inventory.set_item(0, ItemStack("minecraft:iron_ore", 3))
    state.burn_time = 200
    assert state.update()
    assert str(ctx.overworld.get_block_at(*at).type) == "minecraft:lit_furnace"
    state.cook_time = 50
    assert state.update(), "a second update() of the same state was refused"
    live = ctx.overworld.get_block_at(*at).capture_state()
    assert live.cook_time == 50, live.cook_time
    state.burn_time = 0
    assert state.update(), "a third update() of the same state was refused"
    assert str(ctx.overworld.get_block_at(*at).type) == "minecraft:furnace"
    await ctx.ticks(5)
    assert furnace_slots(ctx, at)[0] == ("minecraft:iron_ore", 3), furnace_slots(ctx, at)
    assert dropped_near(ctx, at) == {"items": 0, "orbs": 0}, dropped_near(ctx, at)


@scenario("blocks.furnace.force_mismatch", plot=True, budget=1500)
async def furnace_force_mismatch(ctx: Context) -> None:
    at = ctx.plot.pos(9, 1, 6)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} blast_furnace")
    await ctx.ticks(2)
    seed = ctx.overworld.get_block_at(*at).capture_state()
    seed.snapshot_inventory.set_item(0, ItemStack("minecraft:raw_gold", 2))
    seed.snapshot_inventory.set_item(1, ItemStack("minecraft:coal", 1))
    assert seed.update()
    await ctx.until(lambda: furnace_slots(ctx, at)[2] == ("minecraft:gold_ingot", 2), timeout=600, what="the smelting")
    held = ctx.overworld.get_block_at(*at).capture_state()
    assert str(held.type) == "minecraft:lit_blast_furnace", str(held.type)
    out = ctx.overworld.get_block_at(*at).capture_state()
    out.burn_time = 0
    assert out.update()
    assert str(ctx.overworld.get_block_at(*at).type) == "minecraft:blast_furnace"
    await ctx.ticks(5)
    before = furnace_slots(ctx, at)
    assert dropped_near(ctx, at) == {"items": 0, "orbs": 0}, dropped_near(ctx, at)
    assert held.update(True)
    await ctx.ticks(5)
    ctx.info["slots"] = furnace_slots(ctx, at)
    assert dropped_near(ctx, at) == {"items": 0, "orbs": 0}, (
        f"update(True) across the lit/unlit pair spilled {dropped_near(ctx, at)}"
    )
    assert furnace_slots(ctx, at)[2] == before[2] == ("minecraft:gold_ingot", 2), (before, furnace_slots(ctx, at))
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} air destroy")
    await ctx.ticks(5)
    ctx.info["dropped_on_break"] = dropped_near(ctx, at)
    assert dropped_near(ctx, at)["orbs"] >= 1, "the stored XP was lost"



@scenario("blocks.furnace.update_after_going_out", plot=True, budget=900)
async def furnace_update_after_going_out(ctx: Context) -> None:
    at = ctx.plot.pos(6, 1, 9)
    ctx.run(f"setblock {at[0]} {at[1]} {at[2]} furnace")
    await ctx.ticks(2)
    seed = ctx.overworld.get_block_at(*at).capture_state()
    seed.snapshot_inventory.set_item(0, ItemStack("minecraft:iron_ore", 3))
    seed.burn_time = 20
    assert seed.update()
    assert str(ctx.overworld.get_block_at(*at).type) == "minecraft:lit_furnace"
    state = ctx.overworld.get_block_at(*at).capture_state()
    await ctx.until(
        lambda: str(ctx.overworld.get_block_at(*at).type) == "minecraft:furnace",
        timeout=200,
        what="the furnace to go out",
    )
    state.cook_time = 50
    updated = state.update()
    live = ctx.overworld.get_block_at(*at).capture_state()
    ctx.info["after"] = (str(live.type), live.burn_time, live.cook_time)
    assert updated, "update() of a state captured while lit was refused after the furnace went out"
    assert live.cook_time == 50, live.cook_time
    await ctx.ticks(5)
    assert furnace_slots(ctx, at)[0] == ("minecraft:iron_ore", 3), furnace_slots(ctx, at)
    assert dropped_near(ctx, at) == {"items": 0, "orbs": 0}, dropped_near(ctx, at)


@scenario("block.container.live_and_snapshot_inventory", plot=True)
async def container_live_and_snapshot_inventory(ctx: Context) -> None:
    """On a snapshot, inventory edits reach the world at once and snapshot_inventory edits wait for update()."""
    x, y, z = ctx.plot.pos(6, 1, 12)
    ctx.run(f"setblock {x} {y} {z} chest")
    await ctx.ticks(2)

    def stored(slot: int):
        item = (
            ctx.overworld.get_block_at(x, y, z)
            .capture_state(use_snapshot=False)
            .inventory.get_item(slot)
        )
        return None if item is None else (str(item.type), item.amount)

    state = ctx.overworld.get_block_at(x, y, z).capture_state()
    assert state.is_snapshot
    state.inventory.set_item(0, ItemStack("minecraft:diamond", 3))
    assert stored(0) == ("minecraft:diamond", 3), stored(0)
    assert state.snapshot_inventory.get_item(0) is None

    state.snapshot_inventory.set_item(1, ItemStack("minecraft:gold_ingot", 2))
    assert stored(1) is None, stored(1)
    assert state.inventory.get_item(1) is None

    unsnapshotted = ctx.overworld.get_block_at(x, y, z).capture_state(
        use_snapshot=False
    )
    unsnapshotted.snapshot_inventory.set_item(2, ItemStack("minecraft:emerald", 1))
    assert stored(2) == ("minecraft:emerald", 1), stored(2)


@scenario("block.container.custom_name", plot=True)
async def container_custom_name(ctx: Context) -> None:
    """A container's custom name is None until set, reaches the world on update() and clears again."""
    x, y, z = ctx.plot.pos(10, 1, 12)
    ctx.run(f"setblock {x} {y} {z} chest")
    await ctx.ticks(2)
    block = ctx.overworld.get_block_at(x, y, z)

    state = block.capture_state()
    assert state.custom_name is None, state.custom_name
    state.custom_name = "Loot"
    assert state.custom_name == "Loot"
    assert block.capture_state().custom_name is None
    assert state.update(True)
    assert block.capture_state().custom_name == "Loot", (
        block.capture_state().custom_name
    )

    state.custom_name = None
    assert state.custom_name is None
    assert block.capture_state().custom_name == "Loot"
    assert state.update(True)
    assert block.capture_state().custom_name is None, block.capture_state().custom_name

    block.capture_state(use_snapshot=False).custom_name = "Live"
    assert block.capture_state().custom_name == "Live"


@scenario("block.creature_spawner.spawn_delay_bounds", plot=True)
async def creature_spawner_spawn_delay_bounds(ctx: Context) -> None:
    """min_spawn_delay and max_spawn_delay reject values that would put the minimum above the maximum."""
    x, y, z = ctx.plot.pos(8, 1, 12)
    ctx.run(f"setblock {x} {y} {z} mob_spawner")
    await ctx.ticks(2)
    spawner = ctx.overworld.get_block_at(x, y, z).capture_state()
    spawner.max_spawn_delay = 400
    spawner.min_spawn_delay = 100
    refused = {}
    for name, assign in {
        "min above max": lambda: setattr(spawner, "min_spawn_delay", 401),
        "max below min": lambda: setattr(spawner, "max_spawn_delay", 99),
        "max zero": lambda: setattr(spawner, "max_spawn_delay", 0),
    }.items():
        try:
            assign()
        except ValueError:
            refused[name] = True
        else:
            refused[name] = False
    ctx.info["refused"] = refused
    assert all(refused.values()), refused
    assert (spawner.min_spawn_delay, spawner.max_spawn_delay) == (100, 400)
    spawner.min_spawn_delay = 400
    spawner.max_spawn_delay = 400
    assert (spawner.min_spawn_delay, spawner.max_spawn_delay) == (400, 400)


@scenario("block.lectern.update_pulse", plot=True)
async def lectern_update_pulse(ctx: Context) -> None:
    """Updating a lectern state pulses the lectern like a page turn, so a redstone lamp beside it lights."""
    lamp = ctx.plot.at(LECTERN[0] + 1, LECTERN[1], LECTERN[2])
    ctx.run(f"setblock {ctx.plot.at(*LECTERN)} lectern")
    ctx.run(f"setblock {lamp} redstone_lamp")
    await ctx.ticks(10)
    assert not ctx.block_is(lamp, "lit_redstone_lamp"), (
        "the lamp was lit before any update"
    )
    lectern = state_at(ctx, LECTERN, Lectern)
    assert lectern.update(True)
    await ctx.until(
        lambda: ctx.block_is(lamp, "lit_redstone_lamp"),
        timeout=4,
        what="the lamp to light after Lectern.update()",
    )


@scenario("block.sign.target_side", plot=True, budget=1200)
async def sign_target_side(ctx: Context) -> None:
    """A player in front of a sign and one behind it are given opposite sides."""
    ctx.run(f"setblock {ctx.plot.at(*SIGN)} standing_sign")
    seen = {}
    for name, dz in (("SignFront", 4), ("SignBack", -4)):
        await ctx.bot(name)
        player = ctx.server.get_player(name)
        assert player is not None, f"{name} is not online"
        x, y, z = ctx.plot.pos(SIGN[0], 0, SIGN[2] + dz)
        ctx.run(f"tp {name} {x + 0.5} {y} {z + 0.5}")
        await ctx.ticks(40)
        sign = state_at(ctx, SIGN, Sign)
        seen[name] = (
            "front" if sign.get_target_side(player) is sign.get_side(FRONT) else "back"
        )
    ctx.info["sides"] = seen
    assert seen["SignFront"] != seen["SignBack"], seen


@scenario("block.spawner.reset_timer", plot=True)
async def spawner_reset_timer(ctx: Context) -> None:
    """reset_timer redraws the delay from the minimum and maximum spawn delay."""
    ctx.run(f"setblock {ctx.plot.at(*LECTERN)} mob_spawner")
    spawner = ctx.overworld.get_block_at(*ctx.plot.pos(*LECTERN)).capture_state()
    spawner.min_spawn_delay = 300
    spawner.max_spawn_delay = 300
    spawner.delay = 5
    spawner.reset_timer()
    assert spawner.delay == 300, spawner.delay
