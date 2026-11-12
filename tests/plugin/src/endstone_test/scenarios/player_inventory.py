import uuid

from endstone.enchantments import Enchantment, EnchantmentOffer
from endstone.event import (
    CraftItemEvent,
    EnchantItemEvent,
    InventoryCloseEvent,
    InventoryOpenEvent,
    PlayerDropItemEvent,
    PlayerEditBookEvent,
    PlayerItemConsumeEvent,
    PlayerPickupArrowEvent,
    PlayerPickupItemEvent,
    PlayerRecipeBookSettingsChangeEvent,
    PrepareItemEnchantEvent,
)
from endstone.inventory import Inventory, ItemStack
from endstone.level import Location

from endstone_test.harness import Context, scenario
from endstone_test.harness.capture import Capture, plain, properties

BUDGET = 900

CONTAINERS = {
    "minecraft:chest": 27,
    "minecraft:barrel": 27,
    "minecraft:furnace": 3,
    "minecraft:brewing_stand": 5,
}


class InventoryCapture(Capture):
    def offer(self, event) -> None:
        try:
            if self.where is not None and not self.where(event):
                return
            fields = {}
            for name in properties(type(event)):
                try:
                    value = getattr(event, name)
                except Exception as e:  # noqa: BLE001
                    fields[name] = {"error": f"{type(e).__name__}: {e}"}
                    continue
                fields[name] = (
                    {"size": value.size, "slot_empty": value.get_item(0) is None}
                    if isinstance(value, Inventory)
                    else plain(value)
                )
            if self.extra is not None:
                fields.update(self.extra(event))
            self.events.append(fields)
            if self.cancel:
                event.cancel()
            if self.mutate is not None:
                self.mutate(event)
                fields["after"] = {
                    name: plain(getattr(event, name)) for name in properties(type(event)) if name != "inventory"
                }
        except Exception as e:  # noqa: BLE001
            if self.error is None:
                self.error = e


def capture(ctx: Context, event, where=None, *, cancel: bool = False, mutate=None, extra=None) -> Capture:
    made = InventoryCapture(event.__name__, where=where, cancel=cancel, mutate=mutate, extra=extra)
    ctx.driver.hub.captures.setdefault(made.name, []).append(made)
    ctx.captures.append(made)
    return made


async def poll(ctx: Context, read, ok, timeout: int = 100, what: str = "a condition"):
    for _ in range(timeout):
        value = await read()
        if ok(value):
            return value
        await ctx.ticks(1)
    raise TimeoutError(f"timed out after {timeout} polls waiting for {what}")


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def packets(bot, name: str, since: int) -> list[dict]:
    return await bot.call("get_received_packets", name, since=since)


async def barrier(ctx: Context, bot, since: int) -> None:
    token = f"@@barrier {uuid.uuid4().hex[:8]}"
    ctx.server.broadcast_message(token)

    async def seen():
        return [p for p in await packets(bot, "TextPacket", since) if token in str(p["packet"])]

    await poll(ctx, seen, bool, what=f"{bot.name} to hear {token}")


def where(player):
    loc = player.location
    return loc.x, loc.y, loc.z


def flat(a, b) -> float:
    return max(abs(a[0] - b[0]), abs(a[2] - b[2]))


async def join(ctx: Context, name: str, mode: str = "survival", dx: int = 8, dz: int = 6):
    bot = await ctx.bot(name)
    player = ctx.server.get_player(name)
    assert player is not None, f"{name} is not online"
    ctx.run(f"gamemode {mode} {name}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    spot = (x + 0.5, y, z + 0.5)
    ctx.run(f"tp {name} {spot[0]} {spot[1]} {spot[2]}")
    await ctx.until(
        lambda: abs(where(player)[0] - spot[0]) < 0.1 and abs(where(player)[2] - spot[2]) < 0.1,
        timeout=60,
        what=f"{name} to arrive in the plot",
    )

    async def seen():
        return await bot.call("local_player.get_position")

    await poll(
        ctx,
        seen,
        lambda p: abs(p[0] - spot[0]) < 0.1 and abs(p[2] - spot[2]) < 0.1,
        what=f"{name}'s client to see the teleport",
    )
    await ctx.ticks(10)
    return bot, player


def mine(name: str):
    return lambda e: (e.who_clicked if hasattr(e, "who_clicked") else e.player).name == name


def mine_enchanter(name: str):
    return lambda e: e.enchanter.name == name


def place(ctx: Context, block: str, dx: int, dz: int, dy: int = 0) -> tuple[int, int, int]:
    x, y, z = ctx.plot.pos(dx, dy, dz)
    ctx.run(f"setblock {x} {y} {z} {block}")
    return x, y, z


async def put(ctx: Context, bot, block: str, dx: int, dz: int, dy: int = 0, stock: bool = False):
    pos = place(ctx, block, dx, dz, dy)
    if stock:
        seed(ctx, block, pos)

    want = ctx.overworld.get_block_at(*pos).data.runtime_id & 0xFFFFFFFF

    async def seen():
        return await bot.call("get_block", list(pos))

    await poll(ctx, seen, lambda r: r is not None and r & 0xFFFFFFFF == want, what=f"{bot.name} to see {block}")
    return pos


SEED = {
    "minecraft:chest": (0, "minecraft:diamond", 3),
    "minecraft:barrel": (0, "minecraft:emerald", 5),
    "minecraft:furnace": (0, "minecraft:iron_ore", 4),
    "minecraft:brewing_stand": (0, "minecraft:nether_wart", 5),
}
SCREEN = {
    "minecraft:chest": "CONTAINER",
    "minecraft:barrel": "CONTAINER",
    "minecraft:furnace": "FURNACE",
    "minecraft:brewing_stand": "BREWING_STAND",
}
SLOT_CONTAINER = {"minecraft:chest": "LEVEL_ENTITY_CONTAINER", "minecraft:barrel": "BARREL_CONTAINER"}


def contents(ctx: Context, pos) -> list:
    inventory = ctx.overworld.get_block_at(*pos).capture_state().inventory
    return [None if item is None else (str(item.type), item.amount) for item in inventory.contents]


def seed(ctx: Context, block: str, pos) -> list:
    slot, item, count = SEED[block]
    name = item.removeprefix("minecraft:")
    ctx.run(f"replaceitem block {pos[0]} {pos[1]} {pos[2]} slot.container {slot} {name} {count}")
    return contents(ctx, pos)


def shown(screen: dict) -> list:
    return [(s["id"], s["count"]) if s["count"] else None for s in screen["slots"]]


async def open_screen(ctx: Context, bot, pos):
    await bot.call("game_mode.use_item_on", list(pos), 1)
    return await poll(
        ctx, lambda: bot.call("local_player.get_open_container"), lambda c: c is not None, what="the screen"
    )


async def verdict(ctx: Context, bot, request: int) -> dict:
    return await poll(
        ctx,
        lambda: bot.call("item_stack_net_manager.get_response", request),
        lambda r: r is not None,
        what="the response",
    )


def carried(player, item: str) -> int:
    return sum(i.amount for i in player.inventory.contents if i is not None and str(i.type) == item)


@scenario(
    "inventory.open.normal",
    covers={InventoryOpenEvent: "normal", InventoryCloseEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def open_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invopen")
    opens = capture(ctx, InventoryOpenEvent, mine("invopen"))
    closes = capture(ctx, InventoryCloseEvent, mine("invopen"))
    for n, block in enumerate(SEED):
        pos = await put(ctx, bot, block, 8, 8, stock=True)
        expected = contents(ctx, pos)
        assert any(expected), f"{block} was not seeded"
        index = await mark(bot)
        screen = await open_screen(ctx, bot, pos)
        await ctx.ticks(5)
        assert opens.count == n + 1 and closes.count == n, (block, opens.count, closes.count)
        assert opens.events[-1]["inventory"]["size"] == len(expected) == CONTAINERS[block], opens.events[-1]
        assert opens.events[-1]["is_cancelled"] is False
        assert len(await packets(bot, "ContainerOpenPacket", index)) == 1
        assert screen["container_type"] == SCREEN[block], screen
        assert screen["block_pos"] == list(pos), (screen["block_pos"], pos)
        assert shown(screen) == expected, (block, shown(screen), expected)
        if block in SLOT_CONTAINER:
            slot, item, count = SEED[block]
            source = {"container": SLOT_CONTAINER[block], "slot": slot}
            destination = {"container": "COMBINED_HOTBAR_AND_INVENTORY_CONTAINER", "slot": n}
            before = carried(player, item)
            request = await bot.call("item_stack_net_manager.take", 2, source, destination)
            took = await verdict(ctx, bot, request)
            assert took["result"] == "SUCCESS", (block, took, shown(screen))
            await ctx.ticks(3)
            assert contents(ctx, pos)[slot] == (item, count - 2), contents(ctx, pos)
            assert carried(player, item) == before + 2
        await bot.call("local_player.close_container_screen")
        await ctx.until(lambda: closes.count == n + 1, timeout=40, what="InventoryCloseEvent")
        await ctx.ticks(5)
        assert closes.count == n + 1 and opens.count == n + 1, (block, opens.count, closes.count)
        assert closes.events[-1]["inventory"]["size"] == CONTAINERS[block]
        assert await bot.call("local_player.get_open_container") is None


@scenario("inventory.open.cancel", covers={InventoryOpenEvent: "cancel"}, plot=True, budget=BUDGET)
async def open_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnoopen")
    opens = capture(ctx, InventoryOpenEvent, mine("invnoopen"), cancel=True)
    closes = capture(ctx, InventoryCloseEvent, mine("invnoopen"))
    for n, block in enumerate(SEED):
        pos = await put(ctx, bot, block, 5 + 2 * n, 8, stock=True)
        expected = contents(ctx, pos)
        index = await mark(bot)
        await bot.call("game_mode.use_item_on", list(pos), 1)
        await ctx.until(lambda: opens.count == n + 1, timeout=40, what=f"InventoryOpenEvent for {block}")
        await barrier(ctx, bot, index)
        await ctx.ticks(5)
        assert not await packets(bot, "ContainerOpenPacket", index), f"{block}: the client was sent a container screen"
        assert await bot.call("local_player.get_open_container") is None
        assert contents(ctx, pos) == expected
        if block in SLOT_CONTAINER:
            slot, item, count = SEED[block]
            request = await bot.call(
                "item_stack_net_manager.take",
                1,
                {"container": SLOT_CONTAINER[block], "slot": slot},
                {"container": "COMBINED_HOTBAR_AND_INVENTORY_CONTAINER", "slot": 0},
            )
            assert (await verdict(ctx, bot, request))["result"] != "SUCCESS", f"{block}: took an item without a screen"
            await ctx.ticks(3)
            assert contents(ctx, pos) == expected
            assert carried(player, item) == 0
    assert closes.count == 0, closes.events
    opens.cancel = False
    pos = await put(ctx, bot, "minecraft:chest", 8, 8, stock=True)
    expected = contents(ctx, pos)
    screen = await open_screen(ctx, bot, pos)
    assert shown(screen) == expected
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count == 1, timeout=40, what="InventoryCloseEvent")


async def chest_at(ctx: Context, bot, dx: int, dz: int):
    return await put(ctx, bot, "minecraft:chest", dx, dz, stock=True)


@scenario("inventory.close.walk_away", covers={InventoryCloseEvent: "normal"}, plot=True, budget=BUDGET)
async def close_walk_away(ctx: Context) -> None:
    bot, player = await join(ctx, "invwalk", dx=3, dz=3)
    opens = capture(ctx, InventoryOpenEvent, mine("invwalk"))
    closes = capture(ctx, InventoryCloseEvent, mine("invwalk"))
    pos = await chest_at(ctx, bot, 2, 2)
    await open_screen(ctx, bot, pos)
    index = await mark(bot)
    x, y, z = ctx.plot.pos(14, 0, 14)
    await bot.call("local_player.move_to", [x + 0.5, y, z + 0.5])
    await ctx.until(
        lambda: flat(where(player), (x + 0.5, y, z + 0.5)) < 0.2, timeout=300, what="the player to arrive far away"
    )
    await bot.call("local_player.stop_moving")
    await ctx.ticks(40)
    assert closes.count == 0, "BDS does not close a screen for distance, so no close event may fire"
    assert await bot.call("local_player.get_open_container") is not None
    assert not await packets(bot, "ContainerClosePacket", index)
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count == 1, timeout=40, what="InventoryCloseEvent once the client closes the screen")
    await ctx.ticks(10)
    assert (opens.count, closes.count) == (1, 1), (opens.count, closes.count)


@scenario("inventory.close.block_broken", covers={InventoryCloseEvent: "normal"}, plot=True, budget=BUDGET)
async def close_block_broken(ctx: Context) -> None:
    bot, player = await join(ctx, "invbreak")
    opens = capture(ctx, InventoryOpenEvent, mine("invbreak"))
    closes = capture(ctx, InventoryCloseEvent, mine("invbreak"))
    pos = await chest_at(ctx, bot, 8, 8)
    await open_screen(ctx, bot, pos)
    index = await mark(bot)
    ctx.run(f"setblock {pos[0]} {pos[1]} {pos[2]} air destroy")
    await poll(
        ctx,
        lambda: bot.call("local_player.get_open_container"),
        lambda c: c is None,
        what="the client to lose the screen",
    )
    await ctx.ticks(20)
    assert await packets(bot, "ContainerClosePacket", index)
    assert (opens.count, closes.count) == (1, 1), f"opens {opens.count}, closes {closes.count}"
    assert closes.error is None, closes.error
    assert closes.events[-1]["inventory"] == {"size": 27, "slot_empty": True}, (
        closes.events[-1]
    )


@scenario(
    "inventory.close.block_replaced",
    covers={InventoryCloseEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def close_block_replaced(ctx: Context) -> None:
    bot, player = await join(ctx, "invswap")
    opens = capture(ctx, InventoryOpenEvent, mine("invswap"))
    closes = capture(ctx, InventoryCloseEvent, mine("invswap"))
    pos = await chest_at(ctx, bot, 8, 8)
    await open_screen(ctx, bot, pos)
    ctx.run(f"setblock {pos[0]} {pos[1]} {pos[2]} minecraft:stone")
    await poll(
        ctx,
        lambda: bot.call("local_player.get_open_container"),
        lambda c: c is None,
        what="the client to lose the screen",
    )
    await ctx.ticks(20)
    assert (opens.count, closes.count) == (1, 1), (
        f"opens {opens.count}, closes {closes.count}"
    )
    assert closes.error is None, closes.error
    assert closes.events[-1]["inventory"] == {"size": 27, "slot_empty": True}, (
        closes.events[-1]
    )


@scenario("inventory.close.disconnect", covers={InventoryCloseEvent: "normal"}, plot=True, budget=BUDGET)
async def close_disconnect(ctx: Context) -> None:
    bot, player = await join(ctx, "invquit")
    opens = capture(ctx, InventoryOpenEvent, mine("invquit"))
    closes = capture(ctx, InventoryCloseEvent, mine("invquit"))
    pos = await chest_at(ctx, bot, 8, 8)
    await open_screen(ctx, bot, pos)
    await ctx.call("bot.close", bot="invquit")
    await ctx.until(lambda: ctx.server.get_player("invquit") is None, timeout=80, what="invquit offline")
    await ctx.ticks(20)
    assert (opens.count, closes.count) == (1, 1), f"opens {opens.count}, closes {closes.count}"


COMBINED = "COMBINED_HOTBAR_AND_INVENTORY_CONTAINER"
GRID = "CRAFTING_INPUT_CONTAINER"


def slot(container: str, index: int) -> dict:
    return {"container": container, "slot": index}


async def grid_screen(ctx: Context, bot, expect: str = "INVENTORY"):
    await bot.call("local_player.open_inventory")
    return await poll(
        ctx,
        lambda: bot.call("local_player.get_open_container"),
        lambda c: c is not None and c["container_type"] == expect,
        what=f"the {expect} screen",
    )


async def stock(ctx: Context, bot, player, index: int, item: str, count: int) -> None:
    name = player.name
    ctx.run(f"replaceitem entity {name} slot.hotbar {index} {item.removeprefix('minecraft:')} {count}")

    async def seen():
        return (await bot.call("local_player.get_inventory_json"))["inventory"]["slots"][index]

    await poll(ctx, seen, lambda s: s["id"] == item and s["count"] == count, what=f"{name}'s client to hold {item}")


async def move(ctx: Context, bot, amount: int, source: dict, destination: dict) -> dict:
    request = await bot.call("item_stack_net_manager.take", amount, source, destination)
    return await verdict(ctx, bot, request)


async def craft(ctx: Context, bot, recipe: str, destination: dict, times: int = 1, auto: bool = False) -> dict:
    method = "craft_recipe_auto" if auto else "craft_recipe"
    request = await bot.call(f"item_stack_net_manager.{method}", recipe, destination, times)
    return await verdict(ctx, bot, request)


PLANKS = "minecraft:oak_planks"
LOG = "minecraft:oak_log"


def seen_craft(events) -> list:
    return [
        {
            "recipe": e["recipe"]["id"],
            "tag": e["recipe"]["tag"],
            "ingredients": sorted((i["type"], i["amount"]) for i in e["ingredients"]),
            "results": [(i["type"], i["amount"]) for i in e["results"]],
            "repetitions": e["repetitions"],
        }
        for e in events
    ]


async def slots_of(bot) -> list:
    return (await bot.call("local_player.get_inventory_json"))["inventory"]["slots"]


async def grid_craft_setup(ctx: Context, name: str):
    bot, player = await join(ctx, name)
    crafts = capture(ctx, CraftItemEvent, mine(name))
    await grid_screen(ctx, bot)
    await stock(ctx, bot, player, 0, LOG, 4)
    placed = await move(ctx, bot, 4, slot(COMBINED, 0), slot(GRID, 28))
    assert placed["result"] == "SUCCESS", placed
    return bot, player, crafts


@scenario("inventory.craft.grid_normal", covers={CraftItemEvent: "normal"}, plot=True, budget=BUDGET)
async def craft_grid_normal(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invcraft")
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=2)
    assert reply["result"] == "SUCCESS", reply
    await ctx.ticks(5)
    assert crafts.count == 1, crafts.events
    event = seen_craft(crafts.events)[0]
    assert event["recipe"] == "minecraft:oak_planks" and event["tag"] == "crafting_table", event
    assert [i[0] for i in event["ingredients"]] == [LOG], event
    assert event["results"] == [(PLANKS, 4)], event
    assert event["repetitions"] == 2, event
    assert crafts.events[0]["recipe"]["class"] == "ShapedRecipe"
    assert carried(player, PLANKS) == 8
    left = {(s["container"], s["slot"]): s["amount"] for s in reply["slots"]}
    assert left[(GRID, 28)] == 2 and left[(COMBINED, 10)] == 8, reply
    back = await move(ctx, bot, 2, slot(GRID, 28), slot(COMBINED, 20))
    assert back["result"] == "SUCCESS", "the server no longer held the two logs the craft left in the grid"
    assert carried(player, LOG) == 2
    held = (await slots_of(bot))[10]
    assert (held["id"], held["count"]) == (PLANKS, 8), held


@scenario("inventory.craft.table_normal", covers={CraftItemEvent: "normal"}, plot=True, budget=BUDGET)
async def craft_table_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invtable")
    crafts = capture(ctx, CraftItemEvent, mine("invtable"))
    table = await put(ctx, bot, "minecraft:crafting_table", 8, 8)
    screen = await open_screen(ctx, bot, table)
    assert screen["container_type"] == "WORKBENCH", screen
    await stock(ctx, bot, player, 0, PLANKS, 3)
    await stock(ctx, bot, player, 1, "minecraft:stick", 2)
    for index in (32, 33, 34):
        assert (await move(ctx, bot, 1, slot(COMBINED, 0), slot(GRID, index)))["result"] == "SUCCESS"
    for index in (36, 39):
        assert (await move(ctx, bot, 1, slot(COMBINED, 1), slot(GRID, index)))["result"] == "SUCCESS"
    reply = await craft(ctx, bot, "minecraft:wooden_pickaxe", slot(COMBINED, 10))
    assert reply["result"] == "SUCCESS", reply
    await ctx.ticks(5)
    assert crafts.count == 1, crafts.events
    event = seen_craft(crafts.events)[0]
    assert event["recipe"] == "minecraft:wooden_pickaxe", event
    assert event["ingredients"] == sorted([(PLANKS, 1)] * 3 + [("minecraft:stick", 1)] * 2), event
    assert event["results"] == [("minecraft:wooden_pickaxe", 1)], event
    assert event["repetitions"] == 1
    assert carried(player, "minecraft:wooden_pickaxe") == 1
    assert carried(player, PLANKS) == 0 and carried(player, "minecraft:stick") == 0


@scenario("inventory.craft.book_normal", covers={CraftItemEvent: "normal"}, plot=True, budget=BUDGET)
async def craft_book_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invbook")
    crafts = capture(ctx, CraftItemEvent, mine("invbook"))
    await grid_screen(ctx, bot)
    await stock(ctx, bot, player, 0, LOG, 4)
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=2, auto=True)
    assert reply["result"] == "SUCCESS", reply
    await ctx.ticks(5)
    assert crafts.count == 1, crafts.events
    event = seen_craft(crafts.events)[0]
    assert event["recipe"] == "minecraft:oak_planks", event
    assert event["ingredients"] == [(LOG, 1)], event
    assert event["results"] == [(PLANKS, 4)] and event["repetitions"] == 2, event
    assert carried(player, PLANKS) == 8 and carried(player, LOG) == 2


@scenario("inventory.craft.cancel", covers={CraftItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def craft_cancel(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invnocraft")
    crafts.cancel = True
    grid = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=2)
    assert grid["result"] != "SUCCESS", grid
    await ctx.ticks(5)
    assert crafts.count == 1
    assert carried(player, PLANKS) == 0
    back = await move(ctx, bot, 4, slot(GRID, 28), slot(COMBINED, 0))
    assert back["result"] == "SUCCESS" and carried(player, LOG) == 4, "the grid lost logs to a cancelled craft"
    after = await slots_of(bot)
    assert after[10]["count"] == 0, f"the client holds the result of a cancelled craft: {after[10]}"
    assert (after[0]["id"], after[0]["count"]) == (LOG, 4), after[0]
    book = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 11), times=2, auto=True)
    assert book["result"] != "SUCCESS", book
    await ctx.ticks(5)
    assert crafts.count == 2
    assert carried(player, PLANKS) == 0 and carried(player, LOG) == 4
    after = await slots_of(bot)
    assert (after[0]["id"], after[0]["count"]) == (LOG, 4) and after[11]["count"] == 0, (after[0], after[11])
    crafts.cancel = False
    again = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 12), times=1, auto=True)
    assert again["result"] == "SUCCESS", again
    assert carried(player, PLANKS) == 4 and carried(player, LOG) == 3


@scenario("inventory.craft.mutate_results_grid", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_results_grid(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invgemgrid")
    crafts.mutate = lambda e: setattr(e, "results", [ItemStack("minecraft:diamond", 4)])
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=1)
    assert reply["result"] == "SUCCESS", reply
    await ctx.ticks(5)
    assert carried(player, "minecraft:diamond") == 4 and carried(player, PLANKS) == 0
    index = await mark(bot)
    for _ in range(60):
        held = (await slots_of(bot))[10]
        if held["id"] == "minecraft:diamond":
            break
        await ctx.ticks(1)
    resync = [
        p["name"]
        for p in await bot.call("get_received_packets", None, since=index)
        if p["name"].startswith("Inventory")
    ]
    ctx.info["resync"] = resync
    assert (held["id"], held["count"]) == ("minecraft:diamond", 4), (held, resync)


PLAYER_INVENTORY = {COMBINED, "HOTBAR_CONTAINER", "INVENTORY_CONTAINER"}


@scenario("inventory.craft.mutate_results_slots", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_results_slots(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invgemslot")
    crafts.mutate = lambda e: setattr(e, "results", [ItemStack("minecraft:diamond", 4)])
    index = await mark(bot)
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=1)
    assert reply["result"] == "SUCCESS", reply
    await poll(ctx, lambda: slots_of(bot), lambda s: s[10]["id"] == "minecraft:diamond", what="the client's diamonds")
    await barrier(ctx, bot, index)
    expected = {
        ("INVENTORY" if s["container"] in PLAYER_INVENTORY else "PLAYER_ONLY_UI", s["slot"]): s["amount"]
        for s in reply["slots"]
    }
    received = await bot.call("get_received_packets", None, since=index)
    response = max(p["index"] for p in received if p["name"] == "ItemStackResponsePacket")
    sent = [p for p in received if p["name"] == "InventorySlotPacket"]
    ctx.info["expected"] = sorted(expected.items())
    ctx.info["sent"] = [(p["index"], p["packet"]["inventory_id"], p["packet"]["slot"], p["packet"]["item"]) for p in sent]
    assert all(p["index"] > response for p in sent), "a slot was resent before the ItemStackResponsePacket"
    got = {(p["packet"]["inventory_id"], p["packet"]["slot"]): p["packet"]["item"] for p in sent}
    assert set(got) == set(expected), (sorted(got), sorted(expected))
    for key, amount in expected.items():
        assert got[key]["stack_size"] == amount, (key, got[key], amount)
    held = (await slots_of(bot))[10]
    diamonds = got[("INVENTORY", 10)]
    assert (held["id"], held["count"]) == ("minecraft:diamond", 4), held
    assert diamonds["net_id_variant"] == held["net_id"], (diamonds, held)
    assert carried(player, "minecraft:diamond") == 4 and carried(player, PLANKS) == 0


@scenario("inventory.craft.mutate_results_book", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_results_book(ctx: Context) -> None:
    bot, player = await join(ctx, "invgembook")
    crafts = capture(
        ctx,
        CraftItemEvent,
        mine("invgembook"),
        mutate=lambda e: setattr(e, "results", [ItemStack("minecraft:diamond", 4)]),
    )
    await grid_screen(ctx, bot)
    await stock(ctx, bot, player, 0, LOG, 4)
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=1, auto=True)
    await ctx.ticks(5)
    assert crafts.count == 1
    assert carried(player, "minecraft:diamond") == 4 and carried(player, PLANKS) == 0, (
        reply["result"],
        carried(player, "minecraft:diamond"),
        carried(player, PLANKS),
    )


@scenario("inventory.craft.mutate_results_amount", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_results_amount(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invgemamt")
    refused = []

    def shrink(e) -> None:
        try:
            e.results = [ItemStack(PLANKS, 2)]
        except ValueError as error:
            refused.append(str(error))

    crafts.mutate = shrink
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=1)
    await ctx.ticks(5)
    assert refused, "a result with a different amount was accepted"
    assert reply["result"] == "SUCCESS" and carried(player, PLANKS) == 4, (reply["result"], carried(player, PLANKS))


def try_repetitions(refused: list):
    def mutate(e) -> None:
        try:
            e.repetitions = 1
        except AttributeError as error:
            refused.append(str(error))

    return mutate


@scenario("inventory.craft.mutate_repetitions", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_repetitions(ctx: Context) -> None:
    bot, player, crafts = await grid_craft_setup(ctx, "invreps")
    refused = []
    crafts.mutate = try_repetitions(refused)
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 10), times=2)
    await ctx.ticks(5)
    assert refused, "repetitions could be assigned"
    assert reply["result"] == "SUCCESS" and carried(player, PLANKS) == 8, (reply["result"], carried(player, PLANKS))


@scenario("inventory.craft.mutate_repetitions_book", covers={CraftItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def craft_mutate_repetitions_book(ctx: Context) -> None:
    bot, player = await join(ctx, "invrepsbook")
    refused = []
    capture(ctx, CraftItemEvent, mine("invrepsbook"), mutate=try_repetitions(refused))
    await grid_screen(ctx, bot)
    await stock(ctx, bot, player, 1, LOG, 4)
    reply = await craft(ctx, bot, "minecraft:oak_planks", slot(COMBINED, 11), times=2, auto=True)
    await ctx.ticks(5)
    assert refused, "repetitions could be assigned"
    assert reply["result"] == "SUCCESS" and carried(player, PLANKS) == 8, (reply["result"], carried(player, PLANKS))


ENCHANT_IN = {"container": "ENCHANTING_INPUT_CONTAINER", "slot": 14}
ENCHANT_LAPIS = {"container": "ENCHANTING_MATERIAL_CONTAINER", "slot": 15}
SHELVES = ((6, 8), (10, 8), (8, 6), (8, 10))


def offers_of(event) -> list:
    return [None if o is None else (o.cost, sorted((ename(k), v) for k, v in o.enchants.items())) for o in event.offers]


def enchants_of(event) -> list:
    return sorted((ename(k), v) for k, v in event.enchants_to_add.items())


def ename(key) -> str:
    return str(key).removeprefix("Enchantment(").removesuffix(")")


def gear(player) -> list:
    found = []
    for item in player.inventory.contents:
        if item is None or str(item.type) == "minecraft:lapis_lazuli":
            continue
        meta = item.item_meta
        found.append((str(item.type), sorted((ename(k), v) for k, v in meta.enchants.items()) if meta else []))
    return found


async def enchant_setup(ctx: Context, name: str, item: str = "minecraft:diamond_sword", place_item: bool = True):
    bot, player = await join(ctx, name, dx=11, dz=11)
    prepare = capture(ctx, PrepareItemEnchantEvent, mine_enchanter(name), extra=lambda e: {"offers_data": offers_of(e)})
    enchant = capture(ctx, EnchantItemEvent, mine_enchanter(name), extra=lambda e: {"enchants": enchants_of(e)})
    table = await put(ctx, bot, "minecraft:enchanting_table", 8, 8)
    for dx, dz in SHELVES:
        place(ctx, "minecraft:bookshelf", dx, dz)
    ctx.run(f"xp 30L {name}")
    await stock(ctx, bot, player, 0, item, 1)
    await stock(ctx, bot, player, 1, "minecraft:lapis_lazuli", 10)
    await open_screen(ctx, bot, table)
    assert prepare.count == 0, "offers were prepared before an item was placed"
    await move(ctx, bot, 10, slot(COMBINED, 1), ENCHANT_LAPIS)
    if place_item:
        assert (await move(ctx, bot, 1, slot(COMBINED, 0), ENCHANT_IN))["result"] == "SUCCESS"
        await poll(
            ctx,
            lambda: bot.call("item_stack_net_manager.get_enchant_options"),
            lambda o: len(o) == 3,
            what="the enchant options",
        )
    return bot, player, prepare, enchant


async def finish(ctx: Context, bot, lapis: int = 10) -> None:
    returned = await move(ctx, bot, 1, ENCHANT_IN, slot(COMBINED, 0))
    assert returned["result"] == "SUCCESS", returned
    if lapis:
        returned = await move(ctx, bot, lapis, ENCHANT_LAPIS, slot(COMBINED, 1))
        assert returned["result"] == "SUCCESS", returned
    await bot.call("local_player.close_container_screen")
    await poll(
        ctx, lambda: bot.call("local_player.get_open_container"), lambda c: c is None, what="the screen to close"
    )
    await ctx.ticks(10)


@scenario("inventory.enchant.prepare_normal", covers={PrepareItemEnchantEvent: "normal"}, plot=True, budget=BUDGET)
async def enchant_prepare_normal(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invprep")
    assert prepare.count == 1, prepare.events
    event = prepare.events[0]
    assert event["item"]["type"] == "minecraft:diamond_sword"
    assert event["enchant_block"]["type"] == "minecraft:enchanting_table"
    assert event["enchantment_bonus"] == len(SHELVES), event["enchantment_bonus"]
    assert event["is_cancelled"] is False
    assert len(event["offers_data"]) == 3 and all(o is not None and o[1] for o in event["offers_data"]), event
    shown = await bot.call("item_stack_net_manager.get_enchant_options")
    assert [o["cost"] for o in shown] == [o[0] for o in event["offers_data"]], (shown, event["offers_data"])
    assert all(1 <= c <= 30 for c in (o["cost"] for o in shown))
    assert enchant.count == 0


@scenario("inventory.enchant.prepare_cancel", covers={PrepareItemEnchantEvent: "cancel"}, plot=True, budget=BUDGET)
async def enchant_prepare_cancel(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invnoprep", place_item=False)
    prepare.cancel = True
    index = await mark(bot)
    assert (await move(ctx, bot, 1, slot(COMBINED, 0), ENCHANT_IN))["result"] == "SUCCESS"
    await ctx.until(lambda: prepare.count == 1, timeout=40, what="PrepareItemEnchantEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    sent = await packets(bot, "PlayerEnchantOptionsPacket", index)
    assert sent, "the client was never told the offers were removed"
    assert not any(o for p in sent for o in p["packet"].get("options") or []), sent[-1]
    assert await bot.call("item_stack_net_manager.get_enchant_options") == []
    prepare.cancel = False
    assert (await move(ctx, bot, 1, ENCHANT_IN, slot(COMBINED, 0)))["result"] == "SUCCESS"
    assert (await move(ctx, bot, 1, slot(COMBINED, 0), ENCHANT_IN))["result"] == "SUCCESS"
    options = await poll(
        ctx, lambda: bot.call("item_stack_net_manager.get_enchant_options"), lambda o: len(o) == 3, what="offers again"
    )
    assert all(o["cost"] > 0 for o in options), options


@scenario("inventory.enchant.prepare_mutate", covers={PrepareItemEnchantEvent: "mutate"}, plot=True, budget=BUDGET)
async def enchant_prepare_mutate(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invmodprep", place_item=False)
    sharpness = ctx.server.get_registry(Enchantment).get(Enchantment.SHARPNESS)

    def rewrite(event) -> None:
        offers = list(event.offers)
        offers[0] = None
        offers[1] = EnchantmentOffer({sharpness: 5}, 20)
        event.offers = offers

    prepare.mutate = rewrite
    assert (await move(ctx, bot, 1, slot(COMBINED, 0), ENCHANT_IN))["result"] == "SUCCESS"
    options = await poll(
        ctx, lambda: bot.call("item_stack_net_manager.get_enchant_options"), lambda o: len(o) == 3, what="offers"
    )
    original = prepare.events[0]["offers_data"]
    assert options[0]["cost"] == 0 and options[0]["net_id"] == 0, options[0]
    assert options[1]["cost"] == 20, options[1]
    assert options[2]["cost"] == original[2][0], (options[2], original[2])
    chosen = await bot.call("item_stack_net_manager.enchant", 1)
    assert (await verdict(ctx, bot, chosen))["result"] == "SUCCESS"
    await ctx.until(lambda: enchant.count == 1, timeout=40, what="EnchantItemEvent")
    assert enchant.events[0]["enchants"] == [("minecraft:sharpness", 5)], enchant.events[0]["enchants"]
    assert enchant.events[0]["exp_level_cost"] == 20
    assert enchant.events[0]["which_button"] == 1
    await finish(ctx, bot, lapis=8)
    assert gear(player) == [("minecraft:diamond_sword", [("minecraft:sharpness", 5)])], gear(player)
    assert player.exp_level == 28 and carried(player, "minecraft:lapis_lazuli") == 8


@scenario("inventory.enchant.apply_normal", covers={EnchantItemEvent: "normal"}, plot=True, budget=BUDGET)
async def enchant_apply_normal(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invench")
    offered = prepare.events[-1]["offers_data"]
    request = await bot.call("item_stack_net_manager.enchant", 1)
    reply = await verdict(ctx, bot, request)
    assert reply["result"] == "SUCCESS", reply
    await ctx.until(lambda: enchant.count, timeout=40, what="EnchantItemEvent")
    await ctx.ticks(5)
    assert enchant.count == 1, enchant.events
    event = enchant.events[0]
    assert event["which_button"] == 1 and event["is_cancelled"] is False
    assert event["item"]["type"] == "minecraft:diamond_sword"
    assert event["exp_level_cost"] == offered[1][0], (event["exp_level_cost"], offered)
    assert event["enchants"] == offered[1][1], (event["enchants"], offered[1])
    assert event["enchant_block"]["type"] == "minecraft:enchanting_table"
    assert player.exp_level == 28, player.exp_level
    await finish(ctx, bot, lapis=8)
    assert gear(player) == [("minecraft:diamond_sword", offered[1][1])], gear(player)
    assert carried(player, "minecraft:lapis_lazuli") == 8
    back = await slots_of(bot)
    assert any(s["id"] == "minecraft:diamond_sword" for s in back), "the client lost the sword"


@scenario("inventory.enchant.apply_cancel", covers={EnchantItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def enchant_apply_cancel(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invnoench")
    enchant.cancel = True
    request = await bot.call("item_stack_net_manager.enchant", 1)
    reply = await verdict(ctx, bot, request)
    assert reply["result"] != "SUCCESS", reply
    await ctx.ticks(5)
    assert enchant.count == 1
    assert player.exp_level == 30
    options = await bot.call("item_stack_net_manager.get_enchant_options")
    assert len(options) == 3 and all(o["cost"] > 0 for o in options), options
    enchant.cancel = False
    again = await bot.call("item_stack_net_manager.enchant", 1)
    assert (await verdict(ctx, bot, again))["result"] == "SUCCESS", "a cancelled enchant left the table unusable"
    await finish(ctx, bot, lapis=8)
    assert enchant.count == 2
    assert player.exp_level == 28 and carried(player, "minecraft:lapis_lazuli") == 8
    assert [g[0] for g in gear(player)] == ["minecraft:diamond_sword"] and gear(player)[0][1], gear(player)


@scenario("inventory.enchant.apply_cancel_untouched", covers={EnchantItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def enchant_apply_cancel_untouched(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invkeep")
    enchant.cancel = True
    request = await bot.call("item_stack_net_manager.enchant", 2)
    assert (await verdict(ctx, bot, request))["result"] != "SUCCESS"
    await ctx.ticks(5)
    await finish(ctx, bot)
    assert gear(player) == [("minecraft:diamond_sword", [])], gear(player)
    assert carried(player, "minecraft:lapis_lazuli") == 10
    assert player.exp_level == 30
    after = await slots_of(bot)
    sword = [s for s in after if s["id"] == "minecraft:diamond_sword"]
    lapis = sum(s["count"] for s in after if s["id"] == "minecraft:lapis_lazuli")
    assert len(sword) == 1 and lapis == 10, (sword, lapis)


@scenario("inventory.enchant.apply_mutate_enchants", covers={EnchantItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def enchant_apply_mutate_enchants(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invmodench")
    unbreaking = ctx.server.get_registry(Enchantment).get(Enchantment.UNBREAKING)
    enchant.mutate = lambda e: setattr(e, "enchants_to_add", {unbreaking: 3})
    request = await bot.call("item_stack_net_manager.enchant", 0)
    assert (await verdict(ctx, bot, request))["result"] == "SUCCESS"
    await ctx.ticks(5)
    await finish(ctx, bot, lapis=9)
    assert gear(player) == [("minecraft:diamond_sword", [("minecraft:unbreaking", 3)])], gear(player)


@scenario("inventory.enchant.apply_mutate_cost", covers={EnchantItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def enchant_apply_mutate_cost(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invmodcost")
    rejected = []

    def mutate(e):
        for cost in (0, -1):
            try:
                e.exp_level_cost = cost
            except ValueError:
                rejected.append(cost)
        e.exp_level_cost = 31

    enchant.mutate = mutate
    request = await bot.call("item_stack_net_manager.enchant", 0)
    reply = await verdict(ctx, bot, request)
    assert rejected == [0, -1], rejected
    await ctx.ticks(5)
    await finish(ctx, bot)
    assert reply["result"] != "SUCCESS", "an offer now costing more levels than the player has was still bought"
    assert gear(player) == [("minecraft:diamond_sword", [])], gear(player)
    assert player.exp_level == 30 and carried(player, "minecraft:lapis_lazuli") == 10


@scenario("inventory.enchant.apply_mutate_item", covers={EnchantItemEvent: "mutate"}, plot=True, budget=BUDGET)
async def enchant_apply_mutate_item(ctx: Context) -> None:
    bot, player, prepare, enchant = await enchant_setup(ctx, "invmoditem")
    enchant.mutate = lambda e: setattr(e, "item", ItemStack("minecraft:diamond_pickaxe", 1))
    request = await bot.call("item_stack_net_manager.enchant", 0)
    assert (await verdict(ctx, bot, request))["result"] == "SUCCESS"
    await ctx.ticks(5)
    await finish(ctx, bot, lapis=9)
    result = gear(player)
    assert [g[0] for g in result] == ["minecraft:diamond_pickaxe"] and result[0][1], result
    after = await slots_of(bot)
    assert [s["id"] for s in after if s["id"].startswith("minecraft:diamond")] == ["minecraft:diamond_pickaxe"], after


def ground_items(ctx: Context, player, item: str | None = None) -> list:
    here = where(player)
    found = []
    for actor in ctx.overworld.actors:
        if str(actor.type) != "minecraft:item":
            continue
        loc = actor.location
        if abs(loc.x - here[0]) > 8 or abs(loc.z - here[2]) > 8 or abs(loc.y - here[1]) > 8:
            continue
        stack = actor.item_stack
        if item is None or str(stack.type) == item:
            found.append((str(stack.type), stack.amount))
    return sorted(found)


async def client_items(bot, item: str | None = None) -> list:
    found = await bot.call("get_actors", "minecraft:item")
    return found if item is None else [a for a in found if a.get("item") == item or a.get("item_id") == item]


@scenario("inventory.drop.normal", covers={PlayerDropItemEvent: "normal"}, plot=True, budget=BUDGET)
async def drop_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invdrop")
    drops = capture(ctx, PlayerDropItemEvent, mine("invdrop"))
    await stock(ctx, bot, player, 0, "minecraft:cobblestone", 5)
    await stock(ctx, bot, player, 1, "minecraft:stone", 3)
    index = await mark(bot)
    await bot.call("local_player.drop_selected_item", False)
    await ctx.until(lambda: drops.count == 1, timeout=40, what="PlayerDropItemEvent")
    await ctx.until(lambda: ground_items(ctx, player, "minecraft:cobblestone"), timeout=40, what="the dropped item")
    assert drops.events[0]["item"] == {"type": "minecraft:cobblestone", "amount": 1}, drops.events[0]
    assert ground_items(ctx, player) == [("minecraft:cobblestone", 1)]
    assert carried(player, "minecraft:cobblestone") == 4
    await poll(ctx, lambda: packets(bot, "AddItemActorPacket", index), bool, what="the client to be shown the item")
    assert len(await packets(bot, "AddItemActorPacket", index)) == 1
    await bot.call("local_player.select_slot", 1)
    await ctx.ticks(5)
    await bot.call("local_player.drop_selected_item", True)
    await ctx.until(lambda: drops.count == 2, timeout=40, what="the second PlayerDropItemEvent")
    await ctx.until(lambda: ground_items(ctx, player, "minecraft:stone"), timeout=40, what="the dropped stack")
    assert drops.events[1]["item"] == {"type": "minecraft:stone", "amount": 3}, drops.events[1]
    assert carried(player, "minecraft:stone") == 0 and carried(player, "minecraft:cobblestone") == 4
    held = await slots_of(bot)
    assert (held[0]["id"], held[0]["count"]) == ("minecraft:cobblestone", 4) and held[1]["count"] == 0, held[:2]


@scenario("inventory.drop.from_screen", covers={PlayerDropItemEvent: "normal"}, plot=True, budget=BUDGET)
async def drop_from_screen(ctx: Context) -> None:
    bot, player = await join(ctx, "invdropscr")
    drops = capture(ctx, PlayerDropItemEvent, mine("invdropscr"))
    await stock(ctx, bot, player, 2, "minecraft:cobblestone", 6)
    await grid_screen(ctx, bot)
    request = await bot.call("item_stack_net_manager.drop", 4, slot(COMBINED, 2))
    assert (await verdict(ctx, bot, request))["result"] == "SUCCESS"
    await ctx.until(lambda: drops.count == 1, timeout=40, what="PlayerDropItemEvent")
    await ctx.until(lambda: ground_items(ctx, player), timeout=40, what="the dropped item")
    assert drops.events[0]["item"] == {"type": "minecraft:cobblestone", "amount": 4}, drops.events[0]
    assert ground_items(ctx, player) == [("minecraft:cobblestone", 4)]
    assert carried(player, "minecraft:cobblestone") == 2


def rows(player) -> list:
    return [(str(i.type), i.amount) if i else None for i in player.inventory.contents[:3]]


@scenario("inventory.drop.cancel", covers={PlayerDropItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def drop_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnodrop")
    drops = capture(ctx, PlayerDropItemEvent, mine("invnodrop"), cancel=True)
    await stock(ctx, bot, player, 0, "minecraft:cobblestone", 5)
    index = await mark(bot)
    await bot.call("local_player.drop_selected_item", True)
    await ctx.until(lambda: drops.count == 1, timeout=40, what="PlayerDropItemEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(10)
    assert drops.events[0]["item"] == {"type": "minecraft:cobblestone", "amount": 5}
    assert ground_items(ctx, player) == []
    assert not await packets(bot, "AddItemActorPacket", index)
    assert carried(player, "minecraft:cobblestone") == 5, rows(player)
    held = await poll(
        ctx,
        lambda: slots_of(bot),
        lambda s: s[0]["count"] == 5,
        what="the client to be given the item back",
    )
    assert held[0]["id"] == "minecraft:cobblestone"


@scenario("inventory.drop.cancel_one", covers={PlayerDropItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def drop_cancel_one(ctx: Context) -> None:
    bot, player = await join(ctx, "invnodrop1")
    drops = capture(
        ctx,
        PlayerDropItemEvent,
        mine("invnodrop1"),
        cancel=True,
        extra=lambda e: {"slot_now": rows(e.player)[0]},
    )
    await stock(ctx, bot, player, 0, "minecraft:cobblestone", 5)
    index = await mark(bot)
    await bot.call("local_player.drop_selected_item", False)
    await ctx.until(lambda: drops.count == 1, timeout=40, what="PlayerDropItemEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(10)
    assert ground_items(ctx, player) == []
    ctx.info["slot_at_event"] = drops.events[0]["slot_now"]
    assert carried(player, "minecraft:cobblestone") == 5, rows(player)
    held = await poll(
        ctx, lambda: slots_of(bot), lambda s: s[0]["count"] == 5, what="the client to be given the item back"
    )
    assert held[0]["id"] == "minecraft:cobblestone"


@scenario("inventory.drop.cancel_screen", covers={PlayerDropItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def drop_cancel_screen(ctx: Context) -> None:
    bot, player = await join(ctx, "invnodropscr")
    drops = capture(ctx, PlayerDropItemEvent, mine("invnodropscr"), cancel=True)
    await stock(ctx, bot, player, 0, "minecraft:cobblestone", 5)
    await grid_screen(ctx, bot)
    request = await bot.call("item_stack_net_manager.drop", 3, slot(COMBINED, 0))
    reply = await verdict(ctx, bot, request)
    await ctx.ticks(10)
    assert drops.count == 1
    assert ground_items(ctx, player) == []
    assert carried(player, "minecraft:cobblestone") == 5, (reply["result"], rows(player))
    held = await poll(ctx, lambda: slots_of(bot), lambda s: s[0]["count"] == 5, what="the screen drop to be undone")
    assert held[0]["id"] == "minecraft:cobblestone"


def drop_at(ctx: Context, dx: int, dz: int, stack: ItemStack):
    x, y, z = ctx.plot.pos(dx, 0, dz)
    return ctx.overworld.drop_item(Location(ctx.overworld, x + 0.5, y, z + 0.5), stack)


def takes(since_packets: list) -> list:
    return [p for p in since_packets if p["name"] == "TakeItemActorPacket"]


async def walk_to(ctx: Context, bot, dx: int, dz: int) -> None:
    x, y, z = ctx.plot.pos(dx, 0, dz)
    await bot.call("local_player.move_to", [x + 0.5, y, z + 0.5])


@scenario("inventory.pickup.normal", covers={PlayerPickupItemEvent: "normal"}, plot=True, budget=BUDGET)
async def pickup_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invpick", dx=4, dz=4)
    picks = capture(
        ctx,
        PlayerPickupItemEvent,
        mine("invpick"),
        extra=lambda e: {"stack": (str(e.item.item_stack.type), e.item.item_stack.amount)},
    )
    drop_at(ctx, 9, 4, ItemStack("minecraft:emerald", 3))
    await ctx.ticks(15)
    assert picks.count == 0, "the item was picked up from three blocks away"
    index = await mark(bot)
    await walk_to(ctx, bot, 9, 4)
    await ctx.until(lambda: picks.count, timeout=200, what="PlayerPickupItemEvent")
    await ctx.until(lambda: carried(player, "minecraft:emerald") == 3, timeout=40, what="the emeralds in the inventory")
    await bot.call("local_player.stop_moving")
    await ctx.ticks(10)
    assert picks.count == 1, picks.events
    assert picks.events[0]["stack"] == ("minecraft:emerald", 3), picks.events[0]
    assert ground_items(ctx, player) == []
    await poll(ctx, lambda: packets(bot, "TakeItemActorPacket", index), bool, what="the client to be shown the pickup")
    held = await poll(
        ctx, lambda: slots_of(bot), lambda s: any(x["id"] == "minecraft:emerald" for x in s), what="the stack"
    )
    assert sum(x["count"] for x in held if x["id"] == "minecraft:emerald") == 3
    visible = await client_items(bot)
    assert not visible, f"the client still shows the item on the ground: {visible}"


@scenario("inventory.pickup.cancel", covers={PlayerPickupItemEvent: "cancel"}, plot=True, budget=BUDGET)
async def pickup_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnopick", dx=4, dz=4)
    picks = capture(ctx, PlayerPickupItemEvent, mine("invnopick"), cancel=True)
    drop_at(ctx, 9, 4, ItemStack("minecraft:emerald", 3))
    await ctx.ticks(15)
    index = await mark(bot)
    await walk_to(ctx, bot, 9, 4)
    await ctx.until(lambda: picks.count, timeout=200, what="PlayerPickupItemEvent")
    await bot.call("local_player.stop_moving")
    await barrier(ctx, bot, index)
    await ctx.ticks(20)
    assert ground_items(ctx, player) == [("minecraft:emerald", 3)]
    assert carried(player, "minecraft:emerald") == 0
    assert not takes(await packets(bot, "TakeItemActorPacket", index))
    visible = await client_items(bot)
    assert len(visible) == 1, visible
    picks.cancel = False
    await ctx.until(lambda: carried(player, "minecraft:emerald") == 3, timeout=100, what="the pickup once allowed")
    assert ground_items(ctx, player) == []


async def shoot_down(ctx: Context, bot, player, arrows: int):
    ctx.run(f"replaceitem entity {player.name} slot.weapon.mainhand 0 bow")
    ctx.run(f"replaceitem entity {player.name} slot.inventory 0 arrow {arrows}")
    await poll(
        ctx,
        lambda: slots_of(bot),
        lambda s: any(x["id"] == "minecraft:arrow" and x["count"] == arrows for x in s),
        what="the client to hold the arrows",
    )
    await bot.call("local_player.set_rotation", 90.0, 0.0)
    await ctx.ticks(5)
    await bot.call("game_mode.use_item")
    await ctx.ticks(25)
    await bot.call("game_mode.release_using_item")


def arrows_around(ctx: Context, player) -> list:
    here = where(player)
    return [
        a
        for a in ctx.overworld.actors
        if "arrow" in str(a.type) and abs(a.location.x - here[0]) < 6 and abs(a.location.z - here[2]) < 6
    ]


@scenario("inventory.pickup.arrow_normal", covers={PlayerPickupArrowEvent: "normal"}, plot=True, budget=BUDGET)
async def pickup_arrow_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invarrow")
    picks = capture(ctx, PlayerPickupArrowEvent, mine("invarrow"))
    index = await mark(bot)
    await shoot_down(ctx, bot, player, 5)
    await ctx.until(lambda: picks.count, timeout=100, what="PlayerPickupArrowEvent")
    await ctx.until(lambda: carried(player, "minecraft:arrow") == 5, timeout=40, what="the arrow back in the inventory")
    await ctx.ticks(10)
    assert picks.count == 1, picks.events
    assert picks.events[0]["arrow"]["type"] == "minecraft:arrow", picks.events[0]
    assert arrows_around(ctx, player) == []
    await poll(ctx, lambda: packets(bot, "TakeItemActorPacket", index), bool, what="the client to be shown the pickup")


@scenario("inventory.pickup.arrow_cancel", covers={PlayerPickupArrowEvent: "cancel"}, plot=True, budget=BUDGET)
async def pickup_arrow_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnoarrow")
    picks = capture(ctx, PlayerPickupArrowEvent, mine("invnoarrow"), cancel=True)
    index = await mark(bot)
    await shoot_down(ctx, bot, player, 5)
    await ctx.until(lambda: picks.count, timeout=100, what="PlayerPickupArrowEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(20)
    assert len(arrows_around(ctx, player)) == 1, "the cancelled arrow is gone from the world"
    assert carried(player, "minecraft:arrow") == 4
    assert not takes(await packets(bot, "TakeItemActorPacket", index))
    picks.cancel = False
    await ctx.until(lambda: carried(player, "minecraft:arrow") == 5, timeout=100, what="the arrow once allowed")
    assert arrows_around(ctx, player) == []


async def hungry(ctx: Context, bot, player, level: float = 10.0) -> None:
    player.get_attribute("minecraft:player.hunger").base_value = level
    player.get_attribute("minecraft:player.saturation").base_value = 0.0
    await stock(ctx, bot, player, 0, "minecraft:bread", 3)


def hunger(player) -> float:
    return player.get_attribute("minecraft:player.hunger").value


@scenario("inventory.consume.normal", covers={PlayerItemConsumeEvent: "normal"}, plot=True, budget=BUDGET)
async def consume_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "inveat")
    eats = capture(ctx, PlayerItemConsumeEvent, mine("inveat"))
    await hungry(ctx, bot, player)
    await bot.call("game_mode.use_item")
    await ctx.until(lambda: eats.count, timeout=100, what="PlayerItemConsumeEvent")
    await ctx.ticks(10)
    assert eats.count == 1, eats.events
    assert eats.events[0]["item"]["type"] == "minecraft:bread" and eats.events[0]["hand"] == "HAND", eats.events[0]
    assert hunger(player) == 15.0, hunger(player)
    assert carried(player, "minecraft:bread") == 2
    held = await poll(
        ctx, lambda: slots_of(bot), lambda s: s[0]["count"] == 2, what="the client to see one bread eaten"
    )
    assert held[0]["id"] == "minecraft:bread"


@scenario("inventory.consume.cancel", covers={PlayerItemConsumeEvent: "cancel"}, plot=True, budget=BUDGET)
async def consume_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnoeat")
    eats = capture(ctx, PlayerItemConsumeEvent, mine("invnoeat"), cancel=True)
    await hungry(ctx, bot, player)
    index = await mark(bot)
    await bot.call("game_mode.use_item")
    await ctx.until(lambda: eats.count, timeout=100, what="PlayerItemConsumeEvent")
    await bot.call("game_mode.release_using_item")
    await barrier(ctx, bot, index)
    await ctx.ticks(20)
    assert hunger(player) == 10.0, hunger(player)
    assert carried(player, "minecraft:bread") == 3
    held = await slots_of(bot)
    assert (held[0]["id"], held[0]["count"]) == ("minecraft:bread", 3), held[0]
    eats.cancel = False
    await bot.call("game_mode.use_item")
    await ctx.until(lambda: hunger(player) == 15.0, timeout=100, what="the bread to be eaten once allowed")
    assert carried(player, "minecraft:bread") == 2


def book_info(event) -> dict:
    previous, new = event.previous_book_meta, event.new_book_meta
    return {
        "previous_pages": list(previous.pages),
        "new_pages": list(new.pages),
        "new_title": new.title if new.has_title else None,
        "new_author": new.author if new.has_author else None,
        "previous_has_title": previous.has_title,
    }


def with_pages(meta, pages: list[str]):
    meta.pages = pages
    return meta


async def give_book(ctx: Context, bot, player, pages: list[str]) -> None:
    item = ItemStack("minecraft:writable_book")
    item.set_item_meta(with_pages(item.item_meta, pages))
    player.inventory.set_item(0, item)

    async def seen():
        return (await slots_of(bot))[0]["id"]

    await poll(ctx, seen, lambda i: i == "minecraft:writable_book", what="the client to hold the book")
    await ctx.ticks(3)


def book_in_slot(player) -> tuple:
    item = player.inventory.get_item(0)
    if item is None:
        return None
    meta = item.item_meta
    title = meta.title if getattr(meta, "has_title", False) else None
    author = meta.author if getattr(meta, "has_author", False) else None
    return str(item.type), list(meta.pages), title, author


@scenario("inventory.book.edit_normal", covers={PlayerEditBookEvent: "normal"}, plot=True, budget=BUDGET)
async def book_edit_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invbookedit")
    edits = capture(ctx, PlayerEditBookEvent, mine("invbookedit"), extra=book_info)
    await give_book(ctx, bot, player, ["one", "two"])
    steps = [
        (("replace_page", 0, "edited", 0), ["edited", "two"]),
        (("add_page", 1, "inserted", 0), ["edited", "inserted", "two"]),
        (("swap_pages", 0, "", 2), ["two", "inserted", "edited"]),
        (("delete_page", 1, "", 0), ["two", "edited"]),
    ]
    for n, ((op, page, text, other), pages) in enumerate(steps):
        before = list(book_in_slot(player)[1])
        await bot.call("local_player.edit_book", 0, op, page, text, other)
        await ctx.until(lambda: edits.count == n + 1, timeout=40, what=f"PlayerEditBookEvent for {op}")
        await ctx.until(lambda: book_in_slot(player)[1] == pages, timeout=40, what=f"the book to read {pages}")
        event = edits.events[-1]
        assert event["slot"] == 0 and event["is_signing"] is False, event
        assert event["previous_pages"] == before, (event["previous_pages"], before)
        assert event["new_pages"] == pages, (op, event["new_pages"], pages)
        assert event["new_title"] is None and event["new_author"] is None, event
    await ctx.ticks(5)
    assert edits.count == 4
    assert book_in_slot(player) == ("minecraft:writable_book", ["two", "edited"], None, None)


@scenario("inventory.book.sign_normal", covers={PlayerEditBookEvent: "normal"}, plot=True, budget=BUDGET)
async def book_sign_normal(ctx: Context) -> None:
    bot, player = await join(ctx, "invbooksign")
    edits = capture(ctx, PlayerEditBookEvent, mine("invbooksign"), extra=book_info)
    await give_book(ctx, bot, player, ["one", "two"])
    await bot.call("local_player.sign_book", 0, "Tales", "Qa Author")
    await ctx.until(lambda: edits.count, timeout=40, what="PlayerEditBookEvent")
    await ctx.until(lambda: book_in_slot(player)[0] == "minecraft:written_book", timeout=40, what="the signed book")
    await ctx.ticks(5)
    assert edits.count == 1, edits.events
    event = edits.events[0]
    assert event["is_signing"] is True and event["slot"] == 0, event
    assert event["new_title"] == "Tales" and event["new_author"] == "Qa Author", event
    assert event["new_pages"] == ["one", "two"] and event["previous_has_title"] is False, event
    assert book_in_slot(player) == ("minecraft:written_book", ["one", "two"], "Tales", "Qa Author")
    held = (await slots_of(bot))[0]
    assert held["id"] == "minecraft:written_book", held


@scenario("inventory.book.cancel", covers={PlayerEditBookEvent: "cancel"}, plot=True, budget=BUDGET)
async def book_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invnobook")
    edits = capture(ctx, PlayerEditBookEvent, mine("invnobook"), cancel=True, extra=book_info)
    await give_book(ctx, bot, player, ["one", "two"])
    index = await mark(bot)
    await bot.call("local_player.edit_book", 0, "replace_page", 0, "edited", 0)
    await ctx.until(lambda: edits.count == 1, timeout=40, what="PlayerEditBookEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    assert book_in_slot(player) == ("minecraft:writable_book", ["one", "two"], None, None)
    everything = await bot.call("get_received_packets", None, since=index)
    ctx.info["after_cancel"] = sorted({p["name"] for p in everything})
    resent = [
        p
        for p in everything
        if p["name"] in ("InventorySlotPacket", "InventoryContentPacket", "InventoryTransactionPacket")
    ]
    assert resent, "the client was never sent its book back after the edit was dropped"
    index = await mark(bot)
    await bot.call("local_player.sign_book", 0, "Nope", "Me")
    await ctx.until(lambda: edits.count == 2, timeout=40, what="the second PlayerEditBookEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    assert edits.events[1]["is_signing"] is True
    assert book_in_slot(player) == ("minecraft:writable_book", ["one", "two"], None, None)
    held = (await slots_of(bot))[0]
    assert held["id"] == "minecraft:writable_book", held
    edits.cancel = False
    await bot.call("local_player.edit_book", 0, "replace_page", 0, "kept", 0)
    await ctx.until(lambda: book_in_slot(player)[1] == ["kept", "two"], timeout=40, what="the allowed edit")


@scenario("inventory.book.mutate_meta", covers={PlayerEditBookEvent: "mutate"}, plot=True, budget=BUDGET)
async def book_mutate_meta(ctx: Context) -> None:
    bot, player = await join(ctx, "invbookmod")

    def rewrite(event) -> None:
        meta = event.new_book_meta
        meta.pages = ["mutated", "pages"]
        event.new_book_meta = meta

    edits = capture(ctx, PlayerEditBookEvent, mine("invbookmod"), mutate=rewrite)
    await give_book(ctx, bot, player, ["one", "two"])
    await bot.call("local_player.edit_book", 0, "replace_page", 0, "edited", 0)
    await ctx.until(lambda: edits.count == 1, timeout=40, what="PlayerEditBookEvent")
    await ctx.until(
        lambda: book_in_slot(player)[1] == ["mutated", "pages"], timeout=40, what="the listener's pages on the book"
    )
    assert book_in_slot(player) == ("minecraft:writable_book", ["mutated", "pages"], None, None)
    held = (await slots_of(bot))[0]
    assert held["id"] == "minecraft:writable_book"


@scenario("inventory.book.mutate_sign_meta", covers={PlayerEditBookEvent: "mutate"}, plot=True, budget=BUDGET)
async def book_mutate_sign_meta(ctx: Context) -> None:
    bot, player = await join(ctx, "invbookmodsign")

    def rewrite(event) -> None:
        meta = event.new_book_meta
        meta.title = "Forced"
        meta.author = "Plugin"
        event.new_book_meta = meta

    edits = capture(ctx, PlayerEditBookEvent, mine("invbookmodsign"), mutate=rewrite)
    await give_book(ctx, bot, player, ["one", "two"])
    await bot.call("local_player.sign_book", 0, "Mine", "Me")
    await ctx.until(lambda: edits.count == 1, timeout=40, what="PlayerEditBookEvent")
    await ctx.until(lambda: book_in_slot(player)[0] == "minecraft:written_book", timeout=40, what="the signed book")
    await ctx.ticks(5)
    assert book_in_slot(player) == ("minecraft:written_book", ["one", "two"], "Forced", "Plugin"), book_in_slot(player)


@scenario("inventory.book.mutate_title_dropped", covers={PlayerEditBookEvent: "mutate"}, plot=True, budget=BUDGET)
async def book_mutate_title_dropped(ctx: Context) -> None:
    bot, player = await join(ctx, "invbooktitle")

    def rewrite(event) -> None:
        meta = event.new_book_meta
        meta.title = "Ignored"
        meta.author = "Ignored"
        event.new_book_meta = meta

    edits = capture(ctx, PlayerEditBookEvent, mine("invbooktitle"), mutate=rewrite)
    await give_book(ctx, bot, player, ["one", "two"])
    await bot.call("local_player.edit_book", 0, "replace_page", 1, "second", 0)
    await ctx.until(lambda: book_in_slot(player)[1] == ["one", "second"], timeout=40, what="the edited book")
    await ctx.ticks(5)
    assert edits.count == 1 and edits.events[0]["is_signing"] is False
    item = player.inventory.get_item(0)
    assert str(item.type) == "minecraft:writable_book" and not getattr(item.item_meta, "has_title", False), item


@scenario("inventory.book.mutate_signing", covers={PlayerEditBookEvent: "mutate"}, plot=True, budget=BUDGET)
async def book_mutate_signing(ctx: Context) -> None:
    bot, player = await join(ctx, "invbooksigning")
    edits = capture(ctx, PlayerEditBookEvent, mine("invbooksigning"), mutate=lambda e: setattr(e, "is_signing", True))
    await give_book(ctx, bot, player, ["one", "two"])
    await bot.call("local_player.edit_book", 0, "replace_page", 0, "edited", 0)
    await ctx.until(lambda: edits.count == 1, timeout=40, what="PlayerEditBookEvent")
    await ctx.ticks(20)
    assert book_in_slot(player)[0] == "minecraft:written_book", f"the book was not signed: {book_in_slot(player)}"
    assert book_in_slot(player)[1] == ["edited", "two"], book_in_slot(player)
    held = await poll(
        ctx, lambda: slots_of(bot), lambda s: s[0]["id"] == "minecraft:written_book", what="the client's book"
    )
    assert held[0]["id"] == "minecraft:written_book"


@scenario("inventory.book.mutate_unsigning", covers={PlayerEditBookEvent: "mutate"}, plot=True, budget=BUDGET)
async def book_mutate_unsigning(ctx: Context) -> None:
    bot, player = await join(ctx, "invbookunsign")
    edits = capture(ctx, PlayerEditBookEvent, mine("invbookunsign"), mutate=lambda e: setattr(e, "is_signing", False))
    await give_book(ctx, bot, player, ["one", "two"])
    index = await mark(bot)
    await bot.call("local_player.sign_book", 0, "Tales", "Me")
    await ctx.until(lambda: edits.count == 1, timeout=40, what="PlayerEditBookEvent")
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    assert edits.events[0]["is_signing"] is True
    assert book_in_slot(player) == ("minecraft:writable_book", ["one", "two"], None, None), book_in_slot(player)
    held = await poll(
        ctx, lambda: slots_of(bot), lambda s: s[0]["id"] == "minecraft:writable_book", what="the client's book"
    )
    assert held[0]["id"] == "minecraft:writable_book"


@scenario(
    "inventory.recipe_book.settings",
    covers={PlayerRecipeBookSettingsChangeEvent: "normal"},
    plot=True,
    budget=BUDGET,
)
async def recipe_book_settings(ctx: Context) -> None:
    bot, player = await join(ctx, "invrbook")
    changes = capture(ctx, PlayerRecipeBookSettingsChangeEvent, mine("invrbook"))
    steps = [
        (dict(filtering=True, inventory_layout="DEFAULT", crafting_layout="DEFAULT"), True, True),
        (dict(filtering=False, inventory_layout="DEFAULT", crafting_layout="DEFAULT"), True, False),
        (dict(filtering=False, inventory_layout="INVENTORY_ONLY", crafting_layout="INVENTORY_ONLY"), False, False),
        (dict(filtering=True, inventory_layout="INVENTORY_ONLY", crafting_layout="INVENTORY_ONLY"), False, True),
        (dict(filtering=True, inventory_layout="RECIPE_BOOK_ONLY", crafting_layout="INVENTORY_ONLY"), True, True),
        (dict(filtering=True, inventory_layout="NONE", crafting_layout="NONE"), False, True),
    ]
    for n, (settings, is_open, filtering) in enumerate(steps):
        await bot.call("local_player.set_recipe_book_settings", **settings)
        await ctx.until(lambda: changes.count == n + 1, timeout=40, what=f"settings change {n}")
        event = changes.events[n]
        assert (event["is_open"], event["is_filtering"]) == (is_open, filtering), (n, settings, event)
        assert event["recipe_book_type"] == "CRAFTING"
    index = await mark(bot)
    await bot.call("local_player.set_recipe_book_settings", **steps[-1][0])
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    assert changes.count == len(steps), "sending the settings it already had raised another event"


def server_slots(player) -> list:
    return [(str(i.type), i.amount) if i else None for i in player.inventory.contents[:36]]


def client_slots(slots: list) -> list:
    return [(s["id"], s["count"]) if s["count"] else None for s in slots[:36]]


async def synced(ctx: Context, bot, player, what: str) -> None:
    last = {}

    async def view():
        last["client"] = client_slots(await slots_of(bot))
        return last["client"]

    try:
        await poll(ctx, view, lambda c: c == server_slots(player), timeout=40, what=what)
    except TimeoutError:
        server = server_slots(player)
        diff = {i: (server[i], last["client"][i]) for i in range(36) if server[i] != last["client"][i]}
        raise AssertionError(f"{what}: client differs from the server in {diff}") from None


@scenario("inventory.api.slots", plot=True, budget=BUDGET)
async def api_slots(ctx: Context) -> None:
    bot, player = await join(ctx, "invapi")
    inventory = player.inventory
    assert len(inventory) == 36 and inventory.is_empty
    inventory.set_item(5, ItemStack("minecraft:diamond", 3))
    assert server_slots(player)[5] == ("minecraft:diamond", 3)
    await synced(ctx, bot, player, "set_item")
    inventory[7] = ItemStack("minecraft:stone", 64)
    await synced(ctx, bot, player, "__setitem__")
    left = inventory.add_item(ItemStack("minecraft:stone", 70), ItemStack("minecraft:diamond", 70))
    assert left == {}, left
    stones = sum(a for t, a in filter(None, server_slots(player)) if t == "minecraft:stone")
    diamonds = sum(a for t, a in filter(None, server_slots(player)) if t == "minecraft:diamond")
    assert (stones, diamonds) == (134, 73), (stones, diamonds)
    assert all(a <= 64 for _, a in filter(None, server_slots(player)))
    await synced(ctx, bot, player, "add_item")
    left = inventory.remove_item(ItemStack("minecraft:stone", 100))
    assert left == {}, left
    assert sum(a for t, a in filter(None, server_slots(player)) if t == "minecraft:stone") == 34
    await synced(ctx, bot, player, "remove_item")
    inventory.remove("minecraft:diamond")
    assert not any(t == "minecraft:diamond" for t, _ in filter(None, server_slots(player)))
    await synced(ctx, bot, player, "remove by type")
    inventory.clear(7)
    await synced(ctx, bot, player, "clear(index)")
    contents = [ItemStack("minecraft:cobblestone", 2)] + [None] * 34 + [ItemStack("minecraft:stick", 1)]
    inventory.contents = contents
    assert server_slots(player)[0] == ("minecraft:cobblestone", 2) and server_slots(player)[35] == (
        "minecraft:stick",
        1,
    )
    await synced(ctx, bot, player, "contents setter")
    inventory.clear()
    assert inventory.is_empty
    await synced(ctx, bot, player, "clear()")
    for i in range(36):
        inventory.set_item(i, ItemStack("minecraft:dirt", 64))
    left = inventory.add_item(ItemStack("minecraft:dirt", 10), ItemStack("minecraft:stone", 5))
    assert {k: (str(v.type), v.amount) for k, v in left.items()} == {
        0: ("minecraft:dirt", 10),
        1: ("minecraft:stone", 5),
    }, left
    await synced(ctx, bot, player, "add_item into a full inventory")


@scenario("inventory.api.equipment", plot=True, budget=BUDGET)
async def api_equipment(ctx: Context) -> None:
    bot, player = await join(ctx, "invgear")
    inventory = player.inventory
    pieces = {
        "helmet": "minecraft:iron_helmet",
        "chestplate": "minecraft:iron_chestplate",
        "leggings": "minecraft:iron_leggings",
        "boots": "minecraft:iron_boots",
    }
    for name, item in pieces.items():
        setattr(inventory, name, ItemStack(item))
        assert str(getattr(inventory, name).type) == item
    inventory.item_in_off_hand = ItemStack("minecraft:shield")
    inventory.item_in_main_hand = ItemStack("minecraft:diamond_sword")
    assert str(inventory.item_in_main_hand.type) == "minecraft:diamond_sword"
    assert str(inventory.get_item(inventory.held_item_slot).type) == "minecraft:diamond_sword"

    async def view():
        return await bot.call("local_player.get_inventory_json")

    def worn(state) -> list:
        return [a["id"] if a["count"] else None for a in state["armor"][:4]]

    state = await poll(
        ctx,
        view,
        lambda s: worn(s) == list(pieces.values()) and s["offhand"][0]["id"] == "minecraft:shield",
        what="the client to wear the armor and hold the shield",
    )
    assert state["inventory"]["slots"][0]["id"] == "minecraft:diamond_sword", state["inventory"]["slots"][0]
    inventory.leggings = None
    inventory.item_in_off_hand = None
    state = await poll(
        ctx,
        view,
        lambda s: worn(s)[2] is None and not s["offhand"][0]["count"],
        what="the client to lose the leggings and the shield",
    )
    assert worn(state) == [pieces["helmet"], pieces["chestplate"], None, pieces["boots"]], worn(state)
    inventory.set_item(4, ItemStack("minecraft:apple", 2))
    inventory.held_item_slot = 4
    state = await poll(
        ctx, view, lambda s: s["inventory"]["selected_slot"] == 4, what="the client to select the hotbar slot"
    )
    assert inventory.held_item_slot == 4
    assert str(inventory.item_in_main_hand.type) == "minecraft:apple"
    inventory.item_in_main_hand = ItemStack("minecraft:bread", 3)
    await poll(
        ctx,
        lambda: slots_of(bot),
        lambda s: (s[4]["id"], s[4]["count"]) == ("minecraft:bread", 3),
        what="the client to hold the bread",
    )
    assert server_slots(player)[4] == ("minecraft:bread", 3)


@scenario("inventory.book.fresh_book", covers={PlayerEditBookEvent: "normal"}, plot=True, budget=BUDGET)
async def book_fresh(ctx: Context) -> None:
    bot, player = await join(ctx, "invbookfresh")
    edits = capture(ctx, PlayerEditBookEvent, mine("invbookfresh"), extra=book_info)
    player.inventory.set_item(0, ItemStack("minecraft:writable_book"))
    await poll(ctx, lambda: slots_of(bot), lambda s: s[0]["id"] == "minecraft:writable_book", what="the client's book")
    index = await mark(bot)
    await bot.call("local_player.edit_book", 0, "replace_page", 0, "first words", 0)
    await barrier(ctx, bot, index)
    await ctx.ticks(10)
    assert edits.count == 0, "an edit of a book with no pages raised an event"
    assert book_in_slot(player) == ("minecraft:writable_book", [], None, None), book_in_slot(player)
    await bot.call("local_player.edit_book", 0, "add_page", 0, "first words", 0)
    await ctx.ticks(10)
    ctx.info["fresh_add_page"] = [edits.count, book_in_slot(player)[1]]


@scenario("inventory.open.entity_container", covers={InventoryOpenEvent: "normal"}, plot=True, budget=BUDGET)
async def open_entity_container(ctx: Context) -> None:
    bot, player = await join(ctx, "invcart")
    opens = capture(ctx, InventoryOpenEvent, mine("invcart"))
    closes = capture(ctx, InventoryCloseEvent, mine("invcart"))
    x, y, z = ctx.plot.pos(8, 0, 8)
    cart = ctx.overworld.spawn_actor(Location(ctx.overworld, x + 0.5, y, z + 0.5), "minecraft:chest_minecart")
    assert cart is not None
    await ctx.ticks(10)
    await bot.call("game_mode.interact", cart.runtime_id)
    screen = await poll(
        ctx, lambda: bot.call("local_player.get_open_container"), lambda c: c is not None, what="the cart screen"
    )
    await ctx.ticks(5)
    assert screen["entity_unique_id"] != -1, screen
    assert opens.count == 1, f"opening a chest minecart raised {opens.count} InventoryOpenEvent"
    assert opens.events[0]["inventory"]["size"] == 27
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count, timeout=40, what="InventoryCloseEvent")
    assert (opens.count, closes.count) == (1, 1)


async def cart_at(ctx: Context, dx: int, dz: int):
    x, y, z = ctx.plot.pos(dx, 0, dz)
    cart = ctx.overworld.spawn_actor(Location(ctx.overworld, x + 0.5, y, z + 0.5), "minecraft:chest_minecart")
    assert cart is not None
    await ctx.ticks(10)
    return cart


@scenario(
    "inventory.open.entity_container_cancel", covers={InventoryOpenEvent: "cancel"}, plot=True, budget=BUDGET
)
async def open_entity_container_cancel(ctx: Context) -> None:
    bot, player = await join(ctx, "invcartno")
    opens = capture(ctx, InventoryOpenEvent, mine("invcartno"), cancel=True)
    closes = capture(ctx, InventoryCloseEvent, mine("invcartno"))
    cart = await cart_at(ctx, 8, 8)
    index = await mark(bot)
    await bot.call("game_mode.interact", cart.runtime_id)
    await ctx.until(lambda: opens.count, timeout=40, what="InventoryOpenEvent for the cart")
    await barrier(ctx, bot, index)
    await ctx.ticks(5)
    assert opens.count == 1 and opens.events[0]["inventory"]["size"] == 27, opens.events
    assert not await packets(bot, "ContainerOpenPacket", index), "a cancelled open sent the cart screen"
    assert await bot.call("local_player.get_open_container") is None
    assert closes.count == 0, closes.events
    opens.cancel = False
    await bot.call("game_mode.interact", cart.runtime_id)
    await poll(
        ctx, lambda: bot.call("local_player.get_open_container"), lambda c: c is not None, what="the cart screen"
    )
    assert opens.count == 2, opens.count
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count == 1, timeout=40, what="InventoryCloseEvent")
    cart.remove()


@scenario("inventory.close.entity_removed", covers={InventoryCloseEvent: "normal"}, plot=True, budget=BUDGET)
async def close_entity_removed(ctx: Context) -> None:
    bot, player = await join(ctx, "invcartgone")
    opens = capture(ctx, InventoryOpenEvent, mine("invcartgone"))
    closes = capture(ctx, InventoryCloseEvent, mine("invcartgone"))
    cart = await cart_at(ctx, 8, 8)
    await bot.call("game_mode.interact", cart.runtime_id)
    await poll(
        ctx, lambda: bot.call("local_player.get_open_container"), lambda c: c is not None, what="the cart screen"
    )
    assert opens.count == 1, opens.count
    cart.remove()
    await ctx.until(lambda: closes.count, timeout=40, what="InventoryCloseEvent for the removed cart")
    assert closes.events[0]["inventory"]["size"] == 27, closes.events
    await poll(
        ctx,
        lambda: bot.call("local_player.get_open_container"),
        lambda c: c is None,
        what="the client to lose the cart screen",
    )
    await ctx.ticks(20)
    assert (opens.count, closes.count) == (1, 1), (opens.count, closes.count)


@scenario("inventory.close.death", covers={InventoryCloseEvent: "normal"}, plot=True, budget=BUDGET)
async def close_death(ctx: Context) -> None:
    bot, player = await join(ctx, "invdie")
    opens = capture(ctx, InventoryOpenEvent, mine("invdie"))
    closes = capture(ctx, InventoryCloseEvent, mine("invdie"))
    pos = await chest_at(ctx, bot, 8, 8)
    await open_screen(ctx, bot, pos)
    ctx.run("kill invdie")
    await poll(ctx, lambda: bot.call("local_player.is_dead"), bool, what="the death")
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count, timeout=60, what="InventoryCloseEvent after dying")
    await ctx.ticks(10)
    assert (opens.count, closes.count) == (1, 1), (opens.count, closes.count)
    await bot.call("local_player.respawn")
    await ctx.until(lambda: player.health > 0, timeout=100, what="the respawn")
    await ctx.ticks(20)
    x, y, z = ctx.plot.pos(8, 0, 6)
    ctx.run(f"tp invdie {x + 0.5} {y} {z + 0.5}")
    await poll(
        ctx,
        lambda: bot.call("local_player.get_position"),
        lambda p: abs(p[0] - (x + 0.5)) < 0.2 and abs(p[2] - (z + 0.5)) < 0.2,
        what="the client to see the teleport home",
    )
    await ctx.ticks(10)
    pos2 = await chest_at(ctx, bot, 12, 8)
    await open_screen(ctx, bot, pos2)
    await bot.call("local_player.close_container_screen")
    await ctx.until(lambda: closes.count == 2, timeout=60, what="the close of a chest opened after respawning")
    assert opens.count == 2
