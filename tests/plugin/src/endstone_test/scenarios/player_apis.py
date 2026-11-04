import json
import uuid

from endstone import Ability, GameMode
from endstone.block import Side
from endstone.boss import BarColor, BarFlag, BarStyle
from endstone.event import BlockPlaceEvent, PlayerOpenSignEvent
from endstone.form import (
    ActionForm,
    Button,
    Divider,
    Dropdown,
    Header,
    Label,
    MessageForm,
    ModalForm,
    Slider,
    StepSlider,
    TextInput,
    Toggle,
)
from endstone.inventory import ItemStack
from endstone.level import Location
from endstone.map import MapRenderer
from endstone.scoreboard import Criteria, DisplaySlot, ObjectiveSortOrder

from endstone_test.harness import Context, scenario

BUDGET = 900
INT_MIN = -2147483648
PAINTED = 0xFF000000 | (30 << 16) | (20 << 8) | 10


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def got(bot, name: str, since: int) -> list[dict]:
    return [p["packet"] for p in await bot.call("get_received_packets", name, since)]


async def wait_for(ctx: Context, bot, name: str, since: int, ok, what: str, polls: int = 80) -> list[dict]:
    found: list[dict] = []
    for _ in range(polls):
        found = await got(bot, name, since)
        if ok(found):
            return found
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never received {what}; {name} since {since}: {found}")


async def settle(ctx: Context, player, bot, since: int) -> None:
    token = f"@@settle {uuid.uuid4().hex[:8]}"
    player.send_message(token)
    await wait_for(
        ctx, bot, "TextPacket", since, lambda f: any(t["body"].get("message") == token for t in f), "the settle token"
    )


async def join(ctx: Context, name: str, op: bool = False):
    bot = await ctx.bot(name)
    player = await ctx.until(lambda: ctx.server.get_player(name), timeout=60, what=f"{name} online")
    if op:
        player.is_op = True
    return bot, player


async def into_plot(ctx: Context, name: str, op: bool = False, dx: int = 8, dz: int = 8):
    bot, player = await join(ctx, name, op)
    ctx.run(f"tp {name} {ctx.plot.at(dx, 0, dz)}")
    x, y, z = ctx.plot.pos(dx, 0, dz)
    position: list[float] = []
    for _ in range(100):
        position = await bot.call("local_player.get_position")
        if abs(position[0] - (x + 0.5)) < 0.2 and abs(position[2] - (z + 0.5)) < 0.2:
            break
        await ctx.ticks(1)
    else:
        raise AssertionError(f"{name} never arrived in the plot; at {position}")
    await ctx.ticks(10)
    return bot, player


def loc(ctx: Context, at, dx: float = 0.0) -> Location:
    return Location(ctx.overworld, at[0] + dx, at[1], at[2])


def pos_of(packet: dict) -> tuple[int, int, int]:
    return packet["pos"]["x"], packet["pos"]["y"], packet["pos"]["z"]


def near(a: float, b: float, tolerance: float = 1e-4) -> bool:
    return abs(a - b) <= tolerance


def type_at(ctx: Context, at) -> str:
    return str(ctx.overworld.get_block_at(*at).type)


@scenario("apis.text", budget=BUDGET)
async def text(ctx: Context) -> None:
    bot, player = await join(ctx, "ApText")
    observer, watcher = await join(ctx, "ApTextO")
    await ctx.ticks(20)
    since, seen = await mark(bot), await mark(observer)
    player.send_message("§aHello §lworld")
    player.send_popup("popup text")
    player.send_tip("tip text")
    player.send_action_bar("bar text")
    player.send_toast("Toast T", "Toast C")
    player.send_title("Big", "Small", 3, 4, 5)
    player.send_title("Def", "Sub")
    player.reset_title()
    await settle(ctx, player, bot, since)
    bodies = [t["body"] for t in await got(bot, "TextPacket", since)]
    assert {"type": "RAW", "message": "§aHello §lworld"} in bodies, bodies
    assert {"type": "POPUP", "message": "popup text", "params": []} in bodies, bodies
    assert {"type": "TIP", "message": "tip text"} in bodies, bodies
    titles = [
        (t["type"], t["title_text"], t["fade_in_time"], t["stay_time"], t["fade_out_time"])
        for t in await got(bot, "SetTitlePacket", since)
    ]
    assert titles == [
        ("ACTIONBAR", "bar text", -1, -1, -1),
        ("TITLE", "Big", 3, 4, 5),
        ("SUBTITLE", "Small", 3, 4, 5),
        ("TITLE", "Def", 10, 70, 20),
        ("SUBTITLE", "Sub", 10, 70, 20),
        ("RESET", "", -1, -1, -1),
    ], titles
    toasts = await got(bot, "ToastRequestPacket", since)
    assert toasts == [{"title": "Toast T", "content": "Toast C"}], toasts
    await settle(ctx, watcher, observer, seen)
    mine = {"§aHello §lworld", "popup text", "tip text"}
    heard = [t["body"].get("message") for t in await got(observer, "TextPacket", seen)]
    assert not mine & set(heard), heard
    assert not await got(observer, "SetTitlePacket", seen)
    assert not await got(observer, "ToastRequestPacket", seen)


@scenario("apis.block_change", plot=True, budget=BUDGET)
async def block_change(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApFake")
    observer, watcher = await into_plot(ctx, "ApFakeO", dx=9, dz=9)
    at = ctx.plot.pos(4, 0, 4)
    where = ctx.plot.at(4, 0, 4)
    air = ctx.overworld.get_block_at(*at).data.runtime_id
    glass = ctx.server.create_block_data("minecraft:glass").runtime_id
    stone = ctx.server.create_block_data("minecraft:stone").runtime_id
    assert len({air, glass, stone}) == 3

    async def sees(who, runtime_id: int, what: str) -> None:
        last = None
        for _ in range(80):
            last = await who.call("get_block", list(at))
            if last == runtime_id:
                return
            await ctx.ticks(1)
        raise AssertionError(f"{who.name} never saw {what} at {at}; sees {last}")

    await sees(bot, air, "air")
    await sees(observer, air, "air")
    since, seen = await mark(bot), await mark(observer)
    player.send_block_change(loc(ctx, at), ctx.server.create_block_data("minecraft:glass"))
    await sees(bot, glass, "the fake glass")
    updates = [p for p in await got(bot, "UpdateBlockPacket", since) if pos_of(p) == at]
    assert [(p["runtime_id"], p["layer"]) for p in updates] == [(glass, 0)], updates
    assert ctx.block_is(where, "air"), "the fake block changed the world"
    assert ctx.overworld.get_block_at(*at).data.runtime_id == air
    await settle(ctx, watcher, observer, seen)
    assert not [p for p in await got(observer, "UpdateBlockPacket", seen) if pos_of(p) == at]
    assert await observer.call("get_block", list(at)) == air
    ctx.run(f"setblock {where} stone")
    await sees(bot, stone, "the real stone replacing the fake glass")
    await sees(observer, stone, "the real stone")
    assert ctx.overworld.get_block_at(*at).data.runtime_id == stone


async def place_sign(ctx: Context):
    at = ctx.plot.pos(4, 0, 4)
    ctx.run(f"setblock {ctx.plot.at(4, 0, 4)} standing_sign")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:standing_sign", what="the sign")
    await ctx.ticks(10)
    return at, ctx.overworld.get_block_at(*at).capture_state()


@scenario("apis.block_update", plot=True, budget=BUDGET)
async def block_update(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApActor")
    observer, watcher = await into_plot(ctx, "ApActorO", dx=9, dz=9)
    at, state = await place_sign(ctx)
    state.get_side(Side.FRONT).set_line(0, "only for me")
    since, seen = await mark(bot), await mark(observer)
    player.send_block_update(loc(ctx, at), state)
    sent = await wait_for(
        ctx, bot, "BlockActorDataPacket", since, lambda f: any(pos_of(p) == at for p in f), "the sign data"
    )
    data = next(p for p in sent if pos_of(p) == at)["data"]
    assert data["id"] == "Sign" and data["FrontText"]["Text"] == "only for me\n\n\n", data
    assert data["BackText"]["Text"] == "", data
    world = ctx.overworld.get_block_at(*at).capture_state().get_side(Side.FRONT)
    assert [world.get_line(i) for i in range(4)] == ["", "", "", ""], "the fake state changed the sign"
    await settle(ctx, watcher, observer, seen)
    assert not [p for p in await got(observer, "BlockActorDataPacket", seen) if pos_of(p) == at]


@scenario("apis.sign.open", covers={PlayerOpenSignEvent: "normal"}, plot=True, budget=BUDGET)
async def sign_open(ctx: Context) -> None:
    opened = ctx.capture(PlayerOpenSignEvent, lambda e: e.player.name == "ApSign")
    bot, player = await into_plot(ctx, "ApSign")
    at, sign = await place_sign(ctx)
    since = await mark(bot)
    player.open_sign(sign, Side.FRONT)
    first = await wait_for(ctx, bot, "OpenSignPacket", since, lambda f: len(f) == 1, "the sign editor")
    assert first == [{"pos": {"x": at[0], "y": at[1], "z": at[2]}, "is_front_side": True}], first
    assert opened.count == 1, opened.events
    event = opened.events[0]
    assert event["cause"] == "PLUGIN" and event["side"] == "FRONT", event
    assert (event["sign"]["x"], event["sign"]["y"], event["sign"]["z"]) == at, event
    player.open_sign(sign, Side.BACK)
    both = await wait_for(ctx, bot, "OpenSignPacket", since, lambda f: len(f) == 2, "the back editor")
    assert both[1]["is_front_side"] is False and pos_of(both[1]) == at, both
    assert opened.count == 2 and opened.events[1]["side"] == "BACK", opened.events
    far = ctx.plot.pos(6, 0, 6)
    player.open_virtual_sign(__import__('endstone.util', fromlist=['Vector']).Vector(*far), Side.BACK)
    virtual = await wait_for(ctx, bot, "OpenSignPacket", since, lambda f: len(f) == 3, "the virtual editor")
    assert virtual[2] == {"pos": {"x": far[0], "y": far[1], "z": far[2]}, "is_front_side": False}, virtual
    await settle(ctx, player, bot, since)
    assert opened.count == 2, f"open_virtual_sign raised PlayerOpenSignEvent: {opened.events[2:]}"


@scenario("apis.sign.cancel", covers={PlayerOpenSignEvent: "cancel"}, plot=True, budget=BUDGET)
async def sign_cancel(ctx: Context) -> None:
    opened = ctx.capture(PlayerOpenSignEvent, lambda e: e.player.name == "ApSignC", cancel=True)
    bot, player = await into_plot(ctx, "ApSignC")
    at, sign = await place_sign(ctx)
    since = await mark(bot)
    player.open_sign(sign, Side.FRONT)
    await settle(ctx, player, bot, since)
    assert opened.count == 1 and opened.events[0]["cause"] == "PLUGIN", opened.events
    assert not await got(bot, "OpenSignPacket", since), "a cancelled open_sign still opened the editor"


@scenario("apis.sign.dimension", plot=True, budget=3000)
async def sign_dimension(ctx: Context) -> None:
    opened = ctx.capture(PlayerOpenSignEvent, lambda e: e.player.name == "ApSignD")
    bot, player = await into_plot(ctx, "ApSignD")
    at, sign = await place_sign(ctx)
    ctx.run("execute as ApSignD in nether run tp @s 8 80 8")
    await ctx.until(lambda: str(player.dimension.id) == "minecraft:nether", timeout=400, what="the nether")
    await ctx.ticks(20)
    since = await mark(bot)
    try:
        player.open_sign(sign, Side.FRONT)
    except ValueError as e:
        ctx.info["rejected_with"] = str(e)
    else:
        raise AssertionError("open_sign accepted a sign in another dimension")
    await settle(ctx, player, bot, since)
    assert not await got(bot, "OpenSignPacket", since)
    assert opened.count == 0, opened.events


@scenario("apis.respawn_location", plot=True, budget=BUDGET)
async def respawn_location(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApSpawn", op=True)
    assert player.respawn_location is None
    cell = ctx.plot.pos(3, 0, 3)
    since = await mark(bot)
    player.respawn_location = loc(ctx, cell)
    set_ = await wait_for(ctx, bot, "SetSpawnPositionPacket", since, lambda f: len(f) == 1, "the new spawn point")
    assert set_[0]["spawn_pos_type"] == "PLAYER_RESPAWN" and set_[0]["dimension_type"] == 0, set_
    assert pos_of(set_[0]) == cell, set_
    stored = player.respawn_location
    assert stored is not None and (stored.block_x, stored.block_y, stored.block_z) == cell
    last = await mark(bot)
    player.respawn_location = None
    cleared = await wait_for(ctx, bot, "SetSpawnPositionPacket", last, lambda f: len(f) >= 1, "the cleared spawn point")
    assert player.respawn_location is None
    assert cleared[-1]["spawn_pos_type"] == "PLAYER_RESPAWN", cleared
    assert pos_of(cleared[-1]) == (INT_MIN, INT_MIN, INT_MIN), cleared[-1]


def base_layer(packets: list[dict]) -> dict | None:
    for packet in reversed(packets):
        for layer in packet["data"]["layers"]:
            if layer["serialized_layer"] == "BASE":
                return {**layer, "player_permissions": packet["data"]["player_permissions"]}
    return None


async def abilities_now(bot, since: int = 0) -> dict:
    layer = base_layer(await got(bot, "UpdateAbilitiesPacket", since))
    assert layer is not None, "the client never received a base ability layer"
    return layer


async def abilities_until(ctx: Context, bot, since: int, ok, what: str) -> dict:
    layer = None
    for _ in range(80):
        layer = base_layer(await got(bot, "UpdateAbilitiesPacket", since))
        if layer is not None and ok(layer):
            return layer
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never received {what}; last base layer {layer}")


MEMBER_ABILITIES = [
    (Ability.BUILD, 0),
    (Ability.MINE, 1),
    (Ability.DOORS_AND_SWITCHES, 2),
    (Ability.OPEN_CONTAINERS, 3),
    (Ability.ATTACK_PLAYERS, 4),
    (Ability.ATTACK_MOBS, 5),
    (Ability.OPERATOR_COMMANDS, 6),
    (Ability.TELEPORT, 7),
]


@scenario("apis.abilities.member", budget=1500)
async def abilities_member(ctx: Context) -> None:
    bot, player = await join(ctx, "ApAbil")
    await ctx.ticks(20)
    base = await abilities_now(bot)
    assert base["player_permissions"] == "MEMBER", base
    for ability, bit in MEMBER_ABILITIES:
        current = player.get_ability(ability)
        assert current == bool(base["ability_values"] >> bit & 1), (ability, current, base)
        since = await mark(bot)
        player.set_ability(ability, not current)
        flipped = await abilities_until(
            ctx, bot, since, lambda update: update["ability_values"] == base["ability_values"] ^ (1 << bit), f"{ability} flipped"
        )
        assert flipped["abilities_set"] == base["abilities_set"], flipped
        assert player.get_ability(ability) is (not current)
        since = await mark(bot)
        player.set_ability(ability, current)
        await abilities_until(
            ctx, bot, since, lambda update: update["ability_values"] == base["ability_values"], f"{ability} restored"
        )
        assert player.get_ability(ability) is current


@scenario("apis.abilities.speed", budget=1500)
async def abilities_speed(ctx: Context) -> None:
    bot, player = await join(ctx, "ApSpeed")
    await ctx.ticks(20)
    base = await abilities_now(bot)
    since = await mark(bot)
    player.set_ability(Ability.FLY_SPEED, 0.2)
    layer = await abilities_until(ctx, bot, since, lambda update: near(update["fly_speed"], 0.2), "fly speed 0.2")
    assert near(layer["walk_speed"], base["walk_speed"]) and layer["ability_values"] == base["ability_values"]
    assert near(player.fly_speed, 0.2)
    since = await mark(bot)
    player.walk_speed = 0.3
    layer = await abilities_until(ctx, bot, since, lambda update: near(update["walk_speed"], 0.3), "walk speed 0.3")
    assert near(layer["fly_speed"], 0.2)
    since = await mark(bot)
    player.set_ability(Ability.VERTICAL_FLY_SPEED, 2.0)
    await abilities_until(ctx, bot, since, lambda update: near(update["vertical_fly_speed"], 2.0), "vertical fly speed 2.0")
    since = await mark(bot)
    for bad in ((Ability.BUILD, 0.5), (Ability.FLY_SPEED, True), ("minecraft:nonsense", True)):
        try:
            player.set_ability(*bad)
        except ValueError:
            continue
        raise AssertionError(f"set_ability{bad} did not raise ValueError")
    await settle(ctx, player, bot, since)
    assert not await got(bot, "UpdateAbilitiesPacket", since), "a rejected set_ability still reached the client"


@scenario("apis.abilities.flying", budget=1500)
async def abilities_flying(ctx: Context) -> None:
    bot, player = await join(ctx, "ApFly")
    await ctx.ticks(20)
    base = await abilities_now(bot)
    since = await mark(bot)
    player.allow_flight = True
    layer = await abilities_until(ctx, bot, since, lambda update: update["ability_values"] >> 10 & 1, "the mayfly bit")
    assert layer["ability_values"] == base["ability_values"] | (1 << 10), layer
    since = await mark(bot)
    player.set_ability(Ability.FLYING, True)
    layer = await abilities_until(ctx, bot, since, lambda update: update["ability_values"] >> 9 & 1, "the flying bit")
    assert player.is_flying and layer["ability_values"] == base["ability_values"] | (3 << 9), layer
    since = await mark(bot)
    player.allow_flight = False
    await abilities_until(ctx, bot, since, lambda update: update["ability_values"] == base["ability_values"], "flight taken away")
    assert not player.is_flying


@scenario("apis.particle", plot=True, budget=BUDGET)
async def particle(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApPart")
    observer, watcher = await into_plot(ctx, "ApPartO", dx=9, dz=9)
    x, y, z = ctx.plot.pos(8, 3, 8)
    since, seen = await mark(bot), await mark(observer)
    variables = {"variable.a": 1.5, "variable.b": 2.0}
    player.spawn_particle("minecraft:basic_flame_particle", Location(ctx.overworld, x + 0.5, y, z + 0.5), variables)
    player.spawn_particle("minecraft:basic_flame_particle", x + 0.5, y + 1.0, z + 0.5, {"variable.c": 4.0})
    player.spawn_particle("minecraft:basic_flame_particle", x + 0.5, y + 2.0, z + 0.5)
    found = await wait_for(ctx, bot, "SpawnParticleEffectPacket", since, lambda f: len(f) == 3, "three particles")
    assert [p["effect_name"] for p in found] == ["minecraft:basic_flame_particle"] * 3, found
    assert [(p["pos"]["x"], p["pos"]["y"], p["pos"]["z"]) for p in found] == [
        (x + 0.5, float(y), z + 0.5),
        (x + 0.5, y + 1.0, z + 0.5),
        (x + 0.5, y + 2.0, z + 0.5),
    ], found
    assert json.loads(found[0]["molang_variables"]) == variables, found[0]
    assert json.loads(found[1]["molang_variables"]) == {"variable.c": 4.0}, found[1]
    assert found[2]["molang_variables"] is None, found[2]
    assert all(p["vanilla_dimension_id"] == 0 and p["actor_id"] == -1 for p in found), found
    await settle(ctx, watcher, observer, seen)
    assert not await got(observer, "SpawnParticleEffectPacket", seen)


def form_of(packet: dict) -> dict:
    return json.loads(packet["form_json"])


def raw(text: str) -> dict:
    return {"rawtext": [{"text": text}]}


@scenario("apis.forms", budget=1500)
async def forms(ctx: Context) -> None:
    bot, player = await join(ctx, "ApForm")
    await ctx.ticks(20)
    since = await mark(bot)
    player.send_form(
        ModalForm(
            "Modal",
            [
                Header("hd"),
                Divider(),
                Label("lb"),
                Dropdown("dd", ["a", "b"], 1),
                Slider("sl", 1, 10, 2, 4),
                StepSlider("ss", ["x", "y", "z"], 2),
                TextInput("ti", "ph", "df"),
                Toggle("tg", True),
            ],
            "Go",
            "textures/items/apple",
        )
    )
    player.send_form(
        ActionForm(
            "Action",
            "ac",
            [
                Header("h"),
                Button("b1", "textures/items/apple"),
                Divider(),
                Label("l"),
                Button("b2", "https://x.y/z.png"),
            ],
        )
    )
    player.send_form(MessageForm("Message", "mc", "yes", "no"))
    sent = await wait_for(ctx, bot, "ModalFormRequestPacket", since, lambda f: len(f) == 3, "three forms")
    assert len({p["form_id"] for p in sent}) == 3, sent
    assert form_of(sent[0]) == {
        "type": "custom_form",
        "title": raw("Modal"),
        "icon": {"type": "path", "data": "textures/items/apple"},
        "submit": raw("Go"),
        "content": [
            {"type": "header", "text": raw("hd")},
            {"type": "divider", "text": ""},
            {"type": "label", "text": raw("lb")},
            {"type": "dropdown", "text": raw("dd"), "options": ["a", "b"], "default": 1},
            {"type": "slider", "text": raw("sl"), "min": 1.0, "max": 10.0, "step": 2.0, "default": 4.0},
            {"type": "step_slider", "text": raw("ss"), "steps": ["x", "y", "z"], "default": 2},
            {"type": "input", "text": raw("ti"), "placeholder": raw("ph"), "default": "df"},
            {"type": "toggle", "text": raw("tg"), "default": True},
        ],
    }, form_of(sent[0])
    assert form_of(sent[1]) == {
        "type": "form",
        "title": raw("Action"),
        "content": raw("ac"),
        "elements": [
            {"type": "header", "text": raw("h")},
            {"type": "button", "text": raw("b1"), "image": {"type": "path", "data": "textures/items/apple"}},
            {"type": "divider", "text": ""},
            {"type": "label", "text": raw("l")},
            {"type": "button", "text": raw("b2"), "image": {"type": "url", "data": "https://x.y/z.png"}},
        ],
    }, form_of(sent[1])
    assert form_of(sent[2]) == {
        "type": "modal",
        "title": raw("Message"),
        "content": raw("mc"),
        "button1": raw("yes"),
        "button2": raw("no"),
    }, form_of(sent[2])
    closing = await mark(bot)
    player.close_form()
    await wait_for(ctx, bot, "ClientboundCloseFormPacket", closing, lambda f: len(f) == 1, "the close form packet")


@scenario("apis.kick", budget=BUDGET)
async def kick(ctx: Context) -> None:
    bot, player = await join(ctx, "ApKick")
    await ctx.ticks(10)
    since = await mark(bot)
    player.kick("QA bye\nsecond line")
    found = await wait_for(ctx, bot, "DisconnectPacket", since, lambda f: len(f) == 1, "the disconnect")
    assert found[0]["reason"] == "KICKED", found
    assert found[0]["messages"]["message"] == "QA bye\nsecond line", found
    await ctx.until(lambda: ctx.server.get_player("ApKick") is None, timeout=80, what="ApKick offline")


@scenario("apis.transfer", budget=BUDGET)
async def transfer(ctx: Context) -> None:
    bot, player = await join(ctx, "ApMove")
    await ctx.ticks(10)
    since = await mark(bot)
    player.transfer("127.0.0.1", 19999)
    player.transfer("example.invalid")
    found = await wait_for(ctx, bot, "TransferPacket", since, lambda f: len(f) == 2, "two transfers")
    assert [(p["destination"], p["destination_port"], p["reload_world"]) for p in found] == [
        ("127.0.0.1", 19999, False),
        ("example.invalid", 19132, False),
    ], found


@scenario("apis.bossbar", budget=1500)
async def bossbar(ctx: Context) -> None:
    bot, player = await join(ctx, "ApBar")
    observer, watcher = await join(ctx, "ApBarO")
    await ctx.ticks(20)
    since, seen = await mark(bot), await mark(observer)
    bar = ctx.server.create_boss_bar("QA bar", BarColor.RED, BarStyle.SOLID)
    bar.add_player(player)
    adds = await wait_for(
        ctx, bot, "BossEventPacket", since, lambda f: any(p["name"] == "QA bar" for p in f), "the boss bar"
    )
    first = next(p for p in adds if p["name"] == "QA bar")
    bid = first["boss_id"]
    assert first["event_type"] == "ADD" and first["color"] == "RED" and first["overlay"] == "PROGRESS", first
    assert near(first["health_percent"], 1.0)

    async def next_event(count: int, what: str) -> dict:
        found = await wait_for(
            ctx,
            bot,
            "BossEventPacket",
            since,
            lambda f: len([p for p in f if p["boss_id"] == bid]) >= count,
            what,
        )
        mine = [p for p in found if p["boss_id"] == bid]
        assert len(mine) == count, mine
        return mine[-1]

    bar.progress = 0.25
    percent = await next_event(2, "the progress update")
    assert percent["event_type"] == "UPDATE_PERCENT" and near(percent["health_percent"], 0.25), percent
    bar.title = "QA renamed"
    renamed = await next_event(3, "the rename")
    assert renamed["event_type"] == "UPDATE_NAME" and renamed["name"] == "QA renamed", renamed
    bar.color = BarColor.BLUE
    colour = await next_event(4, "the colour change")
    assert colour["event_type"] == "UPDATE_STYLE" and colour["color"] == "BLUE", colour
    bar.style = BarStyle.SEGMENTED_6
    style = await next_event(5, "the style change")
    assert style["event_type"] == "UPDATE_STYLE" and style["overlay"] == "NOTCHED_6", style
    bar.is_visible = False
    hidden = await next_event(6, "hiding")
    assert hidden["event_type"] == "REMOVE", hidden
    bar.is_visible = True
    shown = await next_event(7, "showing again")
    assert shown["event_type"] == "ADD" and shown["name"] == "QA renamed", shown
    assert shown["color"] == "BLUE" and shown["overlay"] == "NOTCHED_6" and near(shown["health_percent"], 0.25), shown
    bar.add_flag(BarFlag.DARKEN_SKY)
    flags = await next_event(8, "the flag change")
    assert flags["event_type"] == "UPDATE_PROPERTIES", flags
    bar.remove_player(player)
    removed = await next_event(9, "the removal")
    assert removed["event_type"] == "REMOVE", removed
    bar.progress = 0.5
    bar.title = "QA unseen"
    await settle(ctx, player, bot, since)
    assert len([p for p in await got(bot, "BossEventPacket", since) if p["boss_id"] == bid]) == 9
    await settle(ctx, watcher, observer, seen)
    assert not [p for p in await got(observer, "BossEventPacket", seen) if p["boss_id"] == bid]
    bar.remove_all()


@scenario("apis.sidebar", budget=BUDGET)
async def sidebar(ctx: Context) -> None:
    bot, player = await join(ctx, "ApSide")
    await ctx.ticks(20)
    since = await mark(bot)
    board = ctx.server.scoreboard
    objective = board.add_objective("apsb", Criteria.Type.DUMMY, "Side Title")
    try:
        objective.set_display(DisplaySlot.SIDE_BAR, ObjectiveSortOrder.DESCENDING)
        shown = await wait_for(
            ctx,
            bot,
            "SetDisplayObjectivePacket",
            since,
            lambda f: any(p["objective_name"] == "apsb" for p in f),
            "the sidebar",
        )
        display = next(p for p in shown if p["objective_name"] == "apsb")
        assert display["display_slot_name"] == "sidebar" and display["objective_display_name"] == "Side Title", display
        assert display["criteria_name"] == "dummy" and display["sort_order"] == "DESCENDING", display
        objective.get_score("alice").value = 7
        objective.get_score("alice").value = 9

        def scores(found: list[dict]) -> list[tuple]:
            return [
                (e["objective_name"], e["fake_player_name"], e["score_value"])
                for p in found
                for e in p["score_info"]
                if e["objective_name"] == "apsb"
            ]

        found = await wait_for(ctx, bot, "SetScorePacket", since, lambda f: len(scores(f)) >= 2, "both score updates")
        assert scores(found) == [("apsb", "alice", 7), ("apsb", "alice", 9)], scores(found)
        mid = await mark(bot)
        objective.unregister()
        removed = await wait_for(ctx, bot, "RemoveObjectivePacket", mid, lambda f: len(f) == 1, "the objective removal")
        assert removed[0]["objective_name"] == "apsb", removed
    finally:
        if board.get_objective("apsb") is not None:
            board.get_objective("apsb").unregister()


@scenario("apis.scoreboard.assign", budget=BUDGET)
async def scoreboard_assign(ctx: Context) -> None:
    board = ctx.server.create_scoreboard()
    objective = board.add_objective("apsb2", Criteria.Type.DUMMY, "Own Board")
    objective.set_display(DisplaySlot.SIDE_BAR)
    objective.get_score("bob").value = 3
    bot, player = await join(ctx, "ApBoard")
    await ctx.ticks(20)
    since = await mark(bot)
    player.scoreboard = board
    shown = await wait_for(
        ctx,
        bot,
        "SetDisplayObjectivePacket",
        since,
        lambda f: any(p["objective_name"] == "apsb2" for p in f),
        "the sidebar of the board the player was moved to",
        polls=40,
    )
    assert next(p for p in shown if p["objective_name"] == "apsb2")["display_slot_name"] == "sidebar"
    entries = [
        (e["fake_player_name"], e["score_value"])
        for p in await got(bot, "SetScorePacket", since)
        for e in p["score_info"]
        if e["objective_name"] == "apsb2"
    ]
    assert entries == [("bob", 3)], entries


@scenario("apis.scoreboard.restore", budget=BUDGET)
async def scoreboard_restore(ctx: Context) -> None:
    main = ctx.server.scoreboard.add_objective("apsb3", Criteria.Type.DUMMY, "Main Board")
    try:
        main.set_display(DisplaySlot.SIDE_BAR)
        main.get_score("bob").value = 3
        bot, player = await join(ctx, "ApBoardR")
        await ctx.ticks(20)
        player.scoreboard = ctx.server.create_scoreboard()
        await ctx.ticks(10)
        since = await mark(bot)
        player.scoreboard = ctx.server.scoreboard
        await wait_for(
            ctx,
            bot,
            "SetDisplayObjectivePacket",
            since,
            lambda f: any(p["objective_name"] == "apsb3" for p in f),
            "the main scoreboard's sidebar after the player returned to it",
            polls=40,
        )
    finally:
        main.unregister()


@scenario("apis.commands", budget=1500)
async def commands(ctx: Context) -> None:
    bot, player = await join(ctx, "ApCmd")
    await ctx.ticks(20)

    async def listed() -> list[str]:
        since = await mark(bot)
        player.update_commands()
        await settle(ctx, player, bot, since)
        found = await wait_for(ctx, bot, "AvailableCommandsPacket", since, lambda f: len(f) >= 1, "the command list")
        return [c["name"] for c in found[-1]["commands"]]

    names = await listed()
    assert "testoptmsg" not in names and "gamemode" not in names, names
    attachment = player.add_attachment(ctx.plugin, "endstone_test.command.testoptmsg", True)
    names = await listed()
    assert "testoptmsg" in names and "gamemode" not in names, names
    attachment.remove()
    assert "testoptmsg" not in await listed()
    player.is_op = True
    names = await listed()
    assert "gamemode" in names and "testoptmsg" in names, names
    player.is_op = False
    names = await listed()
    assert "gamemode" not in names and "testoptmsg" not in names, names


@scenario("apis.gamemode", budget=1500)
async def gamemode(ctx: Context) -> None:
    bot, player = await join(ctx, "ApMode")
    await ctx.ticks(20)
    for mode in (GameMode.CREATIVE, GameMode.ADVENTURE, GameMode.SPECTATOR, GameMode.SURVIVAL):
        since = await mark(bot)
        player.game_mode = mode
        changed = await wait_for(
            ctx, bot, "UpdatePlayerGameTypePacket", since, lambda f: len(f) >= 1, f"the {mode.name} game type"
        )
        assert [p["player_game_type"] for p in changed] == [mode.name], changed
        assert player.game_mode == mode
    since = await mark(bot)
    player.game_mode = GameMode.SURVIVAL
    await settle(ctx, player, bot, since)
    assert not await got(bot, "UpdatePlayerGameTypePacket", since), "an unchanged game mode was sent again"


def attribute(packets: list[dict], name: str) -> float | None:
    value = None
    for packet in packets:
        for entry in packet["attribute_data"]:
            if entry["name"] == name:
                value = entry["current_value"]
    return value


@scenario("apis.experience", budget=1500)
async def experience(ctx: Context) -> None:
    bot, player = await join(ctx, "ApXp")
    await ctx.ticks(20)

    async def client_xp(since: int, ok, what: str) -> tuple[float, float]:
        for _ in range(80):
            found = await got(bot, "UpdateAttributesPacket", since)
            level = attribute(found, "minecraft:player.level")
            progress = attribute(found, "minecraft:player.experience")
            if level is not None and progress is not None and ok(level, progress):
                return level, progress
            await ctx.ticks(1)
        raise AssertionError(f"{bot.name} never saw {what}")

    start = await mark(bot)
    since = start
    player.give_exp(30)
    level, progress = await client_xp(since, lambda lvl, p: lvl == 3.0, "level 3")
    assert near(progress, 3 / 13) and player.exp_level == 3 and near(player.exp_progress, 3 / 13)
    player.give_exp_levels(2)
    level, progress = await client_xp(start, lambda lvl, p: lvl == 5.0, "level 5")
    assert player.exp_level == 5 and near(progress, player.exp_progress)
    player.exp_progress = 0.5
    level, progress = await client_xp(start, lambda lvl, p: near(p, 0.5), "half a level")
    assert level == 5.0
    player.exp_level = 0
    await client_xp(start, lambda lvl, p: lvl == 0.0, "level 0")
    assert player.exp_level == 0


@scenario("apis.place.replaced_state", covers={BlockPlaceEvent: "normal"}, plot=True, budget=1500)
async def place_replaced_state(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApPlace", dx=4, dz=8)
    ctx.run("replaceitem entity ApPlace slot.hotbar 0 stone 5")
    for _ in range(80):
        slot = (await bot.call("local_player.get_inventory_json"))["inventory"]["slots"][0]
        if (slot["id"], slot["count"]) == ("minecraft:stone", 5):
            break
        await ctx.ticks(1)
    else:
        raise AssertionError("the client never held the stone")
    at = ctx.plot.pos(6, 0, 8)
    ctx.run(f"setblock {ctx.plot.at(6, 0, 8)} short_grass")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:short_grass", what="the grass")
    await ctx.ticks(10)
    states = []
    ctx.capture(
        BlockPlaceEvent,
        lambda e: e.player.name == "ApPlace",
        extra=lambda e: states.append(e.block_replaced_state) or {},
    )
    await bot.call("game_mode.use_item_on", list(at), 1)
    await ctx.until(lambda: states, timeout=60, what="BlockPlaceEvent")
    await ctx.until(lambda: type_at(ctx, at) == "minecraft:stone", what="the stone")
    await ctx.ticks(5)
    assert str(states[0].type) == "minecraft:short_grass", (
        f"the replaced state reads {states[0].type} after the placement; it should still be the grass"
    )


class VanishingRenderer(MapRenderer):
    def __init__(self):
        MapRenderer.__init__(self, is_contextual=False)
        self.calls = 0

    def render(self, view, canvas, player):
        self.calls += 1
        canvas.set_pixel_color(0, 0, (10, 20, 30, 255))
        view.remove_renderer(self)


class PaintRenderer(MapRenderer):
    def __init__(self):
        MapRenderer.__init__(self, is_contextual=False)

    def render(self, view, canvas, player):
        for x in range(128):
            canvas.set_pixel_color(x, 0, (10, 20, 30, 255))


def map_item(ctx: Context, player, renderer):
    view = ctx.server.create_map(player.dimension)
    for existing in view.renderers:
        view.remove_renderer(existing)
    view.add_renderer(renderer)
    item = ItemStack("minecraft:filled_map")
    meta = item.item_meta
    meta.map_view = view
    item.set_item_meta(meta)
    return view, item


@scenario("apis.map.render", budget=1500)
async def map_render(ctx: Context) -> None:
    bot, player = await join(ctx, "ApMap")
    await ctx.ticks(20)
    view, item = map_item(ctx, player, PaintRenderer())
    since = await mark(bot)
    player.inventory.add_item(item)
    found = await wait_for(
        ctx,
        bot,
        "ClientboundMapItemDataPacket",
        since,
        lambda f: any(len(p["map_pixels"] or []) == 128 * 128 for p in f),
        "the full map texture",
    )
    full = next(p for p in found if len(p["map_pixels"] or []) == 128 * 128)
    assert full["map_pixels"][:128] == [PAINTED] * 128, full["map_pixels"][:130]
    assert PAINTED not in full["map_pixels"][128:]
    assert full["width"] == 128 and full["height"] == 128 and full["dimension"] == 0


@scenario("apis.map.self_remove", budget=1500)
async def map_self_remove(ctx: Context) -> None:
    bot, player = await join(ctx, "ApMapV")
    await ctx.ticks(20)
    renderer = VanishingRenderer()
    view, item = map_item(ctx, player, renderer)
    since = await mark(bot)
    player.inventory.add_item(item)
    await ctx.until(lambda: renderer.calls, timeout=200, what="the render call")
    await wait_for(ctx, bot, "ClientboundMapItemDataPacket", since, bool, "a map data packet")
    await ctx.ticks(40)
    sent = await got(bot, "ClientboundMapItemDataPacket", since)
    assert not any(PAINTED in (p["map_pixels"] or []) for p in sent), "a renderer that removed itself kept its drawing"
    assert renderer.calls == 1, renderer.calls
    assert renderer not in view.renderers
    assert await bot.call("is_connected")


async def next_form(ctx: Context, bot, since: int, count: int) -> int:
    found = await wait_for(
        ctx, bot, "ModalFormRequestPacket", since, lambda f: len(f) >= count, f"{count} form request(s)"
    )
    return found[count - 1]["form_id"]


@scenario("apis.forms.submit", budget=2000)
async def forms_submit(ctx: Context) -> None:
    bot, player = await join(ctx, "ApAns")
    await ctx.ticks(20)
    calls: list[tuple] = []
    since = await mark(bot)
    sent = 0

    def message_form() -> MessageForm:
        return MessageForm(
            "M", "mc", "yes", "no", lambda p, s: calls.append(("message", s)), lambda p: calls.append(("closed",))
        )

    async def send(form) -> int:
        nonlocal sent
        player.send_form(form)
        sent += 1
        return await next_form(ctx, bot, since, sent)

    async def heard(count: int) -> None:
        await ctx.until(lambda: len(calls) >= count, timeout=60, what=f"{count} form callback(s)")

    for answer, expected in ((True, 0), (False, 1)):
        form_id = await send(message_form())
        before = len(calls)
        await bot.call("local_player.answer_form", answer, form_id)
        await heard(before + 1)
        assert calls[-1] == ("message", expected), calls

    clicked: list[str] = []
    form_id = await send(
        ActionForm(
            "A",
            "ac",
            [
                Header("h"),
                Button("b0", on_click=lambda p: clicked.append("b0")),
                Divider(),
                Button("b1", on_click=lambda p: clicked.append("b1")),
                Label("l"),
                Button("b2", on_click=lambda p: clicked.append("b2")),
            ],
            lambda p, s: calls.append(("action", s)),
            lambda p: calls.append(("closed",)),
        )
    )
    before = len(calls)
    await bot.call("local_player.send_form_response", form_id, "2")
    await heard(before + 1)
    assert calls[-1] == ("action", 2) and clicked == ["b2"], (calls, clicked)

    values = [None, None, None, 1, 7, 2, "typed text", True]
    form_id = await send(
        ModalForm(
            "C",
            [
                Header("hd"),
                Divider(),
                Label("lb"),
                Dropdown("dd", ["a", "b"], 0),
                Slider("sl", 1, 10, 1, 4),
                StepSlider("ss", ["x", "y", "z"], 0),
                TextInput("ti", "ph"),
                Toggle("tg"),
            ],
            None,
            None,
            lambda p, v: calls.append(("custom", list(v))),
            lambda p: calls.append(("closed",)),
        )
    )
    before = len(calls)
    await bot.call("local_player.answer_form", values, form_id)
    await heard(before + 1)
    assert calls[-1] == ("custom", values), calls[-1]

    for reason in ("USER_CLOSED", "USER_BUSY"):
        form_id = await send(message_form())
        before = len(calls)
        await bot.call("local_player.close_form", reason, form_id)
        await heard(before + 1)
        assert calls[-1] == ("closed",), (reason, calls)
    submits = [c for c in calls if c[0] != "closed"]
    assert submits == [("message", 0), ("message", 1), ("action", 2), ("custom", values)], calls

    form_id = await send(message_form())
    before = len(calls)
    await bot.call("local_player.answer_form", True, form_id)
    await heard(before + 1)
    after = len(calls)
    await bot.call("local_player.send_form_response", form_id, "true")
    await bot.call("local_player.send_form_response", 987654, "true")
    await bot.call("local_player.send_form_response", 987655, None, "USER_CLOSED")
    probe = await mark(bot)
    await settle(ctx, player, bot, probe)
    await ctx.ticks(10)
    assert len(calls) == after, f"a response for a form the player no longer had ran a callback: {calls[after:]}"
    assert await bot.call("is_connected")


@scenario("apis.forms.malformed", budget=2000)
async def forms_malformed(ctx: Context) -> None:
    bot, player = await join(ctx, "ApBad")
    await ctx.ticks(20)
    calls: list[tuple] = []
    since = await mark(bot)
    sent = 0
    mark_id = ctx.driver.next_mark()
    ctx.plugin.logger.info(f"@@harness mark {mark_id} begin")
    cases = [
        (
            ModalForm(
                "C",
                [Toggle("tg")],
                None,
                None,
                lambda p, v: calls.append(("custom", list(v))),
                lambda p: calls.append(("closed",)),
            ),
            '"not an array"',
        ),
        (
            ActionForm(
                "A",
                "ac",
                [Button("b0")],
                lambda p, s: calls.append(("action", s)),
                lambda p: calls.append(("closed",)),
            ),
            '"not an int"',
        ),
        (
            MessageForm(
                "M", "mc", "yes", "no", lambda p, s: calls.append(("message", s)), lambda p: calls.append(("closed",))
            ),
            '"not a bool"',
        ),
    ]
    for form, body in cases:
        player.send_form(form)
        sent += 1
        form_id = await next_form(ctx, bot, since, sent)
        await bot.call("local_player.send_form_response", form_id, body)
        await ctx.ticks(10)
    probe = await mark(bot)
    await settle(ctx, player, bot, probe)
    ctx.plugin.logger.info(f"@@harness mark {mark_id} end")
    lines = await ctx.call("log.between", mark=mark_id)
    ctx.info["log"] = [line for line in lines if "form" in line.lower()]
    assert calls == [("closed",)] * len(cases), calls
    calls.clear()
    assert await bot.call("is_connected")
    assert ctx.server.get_player("ApBad") is not None
    player.send_form(MessageForm("M", "mc", "yes", "no", lambda p, s: calls.append(("message", s))))
    form_id = await next_form(ctx, bot, since, sent + 1)
    await bot.call("local_player.answer_form", True, form_id)
    await ctx.until(lambda: calls, timeout=60, what="the callback of a well formed answer after the malformed ones")
    assert calls == [("message", 0)], calls


@scenario("apis.sign.edit", plot=True, budget=1500)
async def sign_edit(ctx: Context) -> None:
    bot, player = await into_plot(ctx, "ApSignE")
    at, sign = await place_sign(ctx)

    def lines(side: Side) -> list[str]:
        state = ctx.overworld.get_block_at(*at).capture_state().get_side(side)
        return [state.get_line(i) for i in range(4)]

    player.open_sign(sign, Side.FRONT)
    await wait_for(ctx, bot, "OpenSignPacket", 0, lambda f: len(f) == 1, "the editor")
    await bot.call("local_player.close_sign_editor", "typed front")
    await ctx.until(lambda: lines(Side.FRONT)[0] == "typed front", timeout=60, what="the typed front text")
    assert lines(Side.BACK) == ["", "", "", ""]
    player.open_sign(sign, Side.BACK)
    await wait_for(ctx, bot, "OpenSignPacket", 0, lambda f: len(f) == 2, "the back editor")
    await bot.call("local_player.close_sign_editor", "typed back")
    await ctx.until(lambda: lines(Side.BACK)[0] == "typed back", timeout=60, what="the typed back text")


@scenario("apis.sign.edit_cancelled", covers={PlayerOpenSignEvent: "cancel"}, plot=True, budget=1500)
async def sign_edit_cancelled(ctx: Context) -> None:
    ctx.capture(PlayerOpenSignEvent, lambda e: e.player.name == "ApSignX", cancel=True)
    bot, player = await into_plot(ctx, "ApSignX")
    at, sign = await place_sign(ctx)
    since = await mark(bot)
    player.open_sign(sign, Side.FRONT)
    await settle(ctx, player, bot, since)
    try:
        await bot.call("local_player.close_sign_editor", "never typed")
    except Exception as e:
        ctx.info["no_editor"] = str(e)
    else:
        raise AssertionError("the client had a sign editor open after a cancelled open_sign")
    await ctx.ticks(10)
    state = ctx.overworld.get_block_at(*at).capture_state().get_side(Side.FRONT)
    assert [state.get_line(i) for i in range(4)] == ["", "", "", ""]


def radius_batch(radius: int, trailing: bytes = b"") -> str:
    import zlib

    packet = bytes([0x45, radius * 2, radius])
    inner = bytes([len(packet)]) + packet
    deflater = zlib.compressobj(level=6, wbits=-15)
    body = deflater.compress(inner) + deflater.flush()
    return (bytes([0x00]) + body + trailing).hex()


@scenario("apis.net.trailing_deflate", budget=1500)
async def trailing_deflate(ctx: Context) -> None:
    bot, player = await join(ctx, "ApDefl")
    await ctx.ticks(20)
    since = await mark(bot)
    await bot.call("client_network_handler.send_raw_batch", radius_batch(3), False)
    clean = await wait_for(
        ctx, bot, "ChunkRadiusUpdatedPacket", since, lambda f: len(f) == 1, "the reply to a plain deflate batch"
    )
    since = await mark(bot)
    await bot.call("client_network_handler.send_raw_batch", radius_batch(4, bytes([0, 1]) + b"trailing"), False)
    trailing = await wait_for(
        ctx, bot, "ChunkRadiusUpdatedPacket", since, lambda f: len(f) == 1, "the reply to a deflate batch with trailing bytes"
    )
    assert trailing == clean, (clean, trailing)
    assert await bot.call("is_connected") and await bot.call("get_disconnect_reason") is None
    assert ctx.server.get_player("ApDefl") is not None
