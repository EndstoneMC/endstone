import uuid

from endstone.event import (
    ActorRemoveEvent,
    PacketReceiveEvent,
    PacketSendEvent,
    PlayerChatEvent,
    PlayerEmoteEvent,
    PlayerHideActorEvent,
    PlayerShowActorEvent,
    PlayerSkinChangeEvent,
)

from endstone_test.harness import Context, scenario

FIXTURE = "test_fixture"
TEXT_PACKET = 9
BUDGET = 1500


async def join(ctx: Context, name: str):
    bot = await ctx.bot(name)
    player = await ctx.until(
        lambda: ctx.server.get_player(name), timeout=60, what=f"{name} online"
    )
    return bot, player


async def leave(ctx: Context, name: str) -> None:
    await ctx.call("bot.close", bot=name)
    await ctx.until(
        lambda: ctx.server.get_player(name) is None, timeout=200, what=f"{name} offline"
    )


async def three(ctx: Context, prefix: str):
    a, pa = await join(ctx, f"{prefix}A")
    b, pb = await join(ctx, f"{prefix}B")
    c, pc = await join(ctx, f"{prefix}C")
    return (a, pa), (b, pb), (c, pc)


def fixture(ctx: Context):
    plugin = ctx.server.plugin_manager.get_plugin(FIXTURE)
    assert plugin is not None, f"the {FIXTURE} plugin is not loaded"
    assert plugin.is_enabled
    return plugin


async def mark(bot) -> int:
    return await bot.call("get_next_packet_index")


async def marks(*bots) -> dict:
    return {bot: await mark(bot) for bot in bots}


async def received(bot, name: str, since: int) -> list[dict]:
    return await bot.call("get_received_packets", name, since)


def watching(*names: str):
    return lambda e: e.player.name in names


def keys_of(*actors) -> set:
    keys: set = set()
    for actor in actors:
        keys.add(actor.id)
        if hasattr(actor, "unique_id"):
            keys.add(str(actor.unique_id))
            keys.add(actor.runtime_id)
    return keys


async def view(bot, since: int, only: set | None = None) -> list[tuple]:
    out: list[tuple] = []
    for p in await bot.call("get_received_packets", None, since):
        body = p["packet"]
        if p["name"] == "PlayerListPacket":
            for entry in body["entries"]:
                out.append((f"list_{entry['action'].lower()}", entry["uuid"]))
        elif p["name"] == "AddPlayerPacket":
            out.append(("add_player", body["runtime_id"]))
        elif p["name"] == "RemoveActorPacket":
            out.append(("remove_actor", body["entity_id"]))
        elif p["name"] == "AddActorPacket":
            out.append(("add_actor", body["entity_id"]))
    return out if only is None else [item for item in out if item[1] in only]


async def messages(bot, since: int) -> list[str]:
    out = []
    for p in await received(bot, "TextPacket", since):
        message = p["packet"]["body"].get("message")
        if message is not None:
            out.append(message)
    return out


async def hear(
    ctx: Context, bot, since: int, message: str, polls: int = 100
) -> list[str]:
    got: list[str] = []
    for _ in range(polls):
        got = await messages(bot, since)
        if message in got:
            return got
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never heard {message!r}; heard {got}")


async def settle(ctx: Context, since: dict, *extra) -> dict[str, list[tuple]]:
    only = keys_of(*ctx.server.online_players, *extra)
    token = f"@@vis {uuid.uuid4().hex[:8]}"
    ctx.server.broadcast_message(token)
    for bot, start in since.items():
        await hear(ctx, bot, start, token)
    return {bot.name: await view(bot, start, only) for bot, start in since.items()}


async def list_adds(bot, uuid_: str, since: int = 0) -> list[dict]:
    out = []
    for p in await received(bot, "PlayerListPacket", since):
        out.extend(
            e
            for e in p["packet"]["entries"]
            if e["action"] == "ADD" and e["uuid"] == uuid_
        )
    return out


def entry_matches_server(entry: dict, player, skin: bool = True) -> None:
    assert entry["uuid"] == str(player.unique_id), (entry["uuid"], player.unique_id)
    assert entry["id"] == player.id, (entry["id"], player.id)
    assert entry["name"] == player.name, (entry["name"], player.name)
    assert entry["xuid"] == player.xuid, (entry["xuid"], player.xuid)
    if skin:
        assert entry["skin"]["id"] == player.skin.id, (
            entry["skin"]["id"],
            player.skin.id,
        )


def pair(event: dict) -> tuple[str, str]:
    return event["player"]["name"], event["actor"]["name"]


@scenario(
    "visibility.hide.one_way",
    covers={PlayerHideActorEvent: "normal", PlayerShowActorEvent: "normal"},
    budget=BUDGET,
)
async def hide_one_way(ctx: Context) -> None:
    hide = ctx.capture(PlayerHideActorEvent, watching("VhoA", "VhoB", "VhoC"))
    show = ctx.capture(PlayerShowActorEvent, watching("VhoA", "VhoB", "VhoC"))
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vho")
    uuid_b = str(pb.unique_id)
    joined = await list_adds(a, uuid_b)
    assert len(joined) == 1 and joined[0]["name"] == "VhoB", joined
    bots = (a, b, c)
    assert pa.can_see(pb) and pc.can_see(pb) and pb.can_see(pa)

    since = await marks(*bots)
    pa.hide_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhoA"] == [("list_remove", uuid_b), ("remove_actor", pb.id)], after[
        "VhoA"
    ]
    assert after["VhoB"] == [] and after["VhoC"] == [], after
    assert hide.count == 1 and pair(hide.events[0]) == ("VhoA", "VhoB"), hide.events
    assert hide.events[0]["actor"]["runtime_id"] == pb.runtime_id
    assert not pa.can_see(pb)
    assert pc.can_see(pb) and pb.can_see(pa) and pb.can_see(pc), (
        "hiding B from A changed who else can see whom"
    )
    assert show.count == 0

    since = await marks(*bots)
    pa.hide_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert hide.count == 1, (
        f"PlayerHideActorEvent fired for an actor that was already hidden: {hide.events}"
    )
    assert all(lines == [] for lines in after.values()), after

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhoA"] == [("list_add", uuid_b), ("add_player", pb.runtime_id)], (
        after["VhoA"]
    )
    assert after["VhoB"] == [] and after["VhoC"] == [], after
    assert show.count == 1 and pair(show.events[0]) == ("VhoA", "VhoB"), show.events
    assert pa.can_see(pb)
    readded = await list_adds(a, uuid_b, since[a])
    assert len(readded) == 1, readded
    entry_matches_server(readded[0], pb)
    assert readded[0] == joined[0], (
        "the entry sent on show differs from the one sent when B joined"
    )

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert show.count == 1, (
        f"PlayerShowActorEvent fired for an actor that was already visible: {show.events}"
    )
    assert all(lines == [] for lines in after.values()), after
    assert hide.count == 1


@scenario(
    "visibility.hide.both_directions",
    covers={PlayerHideActorEvent: "normal", PlayerShowActorEvent: "normal"},
    budget=BUDGET,
)
async def hide_both_directions(ctx: Context) -> None:
    hide = ctx.capture(PlayerHideActorEvent, watching("VhdA", "VhdB", "VhdC"))
    show = ctx.capture(PlayerShowActorEvent, watching("VhdA", "VhdB", "VhdC"))
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vhd")
    bots = (a, b, c)
    uuid_a, uuid_b = str(pa.unique_id), str(pb.unique_id)

    since = await marks(*bots)
    pb.hide_actor(ctx.plugin, pa)
    after = await settle(ctx, since)
    assert after["VhdB"] == [("list_remove", uuid_a), ("remove_actor", pa.id)], after[
        "VhdB"
    ]
    assert after["VhdA"] == [] and after["VhdC"] == [], after
    assert not pb.can_see(pa) and pa.can_see(pb) and pc.can_see(pa)

    since = await marks(*bots)
    pa.hide_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhdA"] == [("list_remove", uuid_b), ("remove_actor", pb.id)], after[
        "VhdA"
    ]
    assert after["VhdB"] == [] and after["VhdC"] == [], after
    assert not pa.can_see(pb) and not pb.can_see(pa)
    assert [pair(e) for e in hide.events] == [("VhdB", "VhdA"), ("VhdA", "VhdB")], (
        hide.events
    )

    since = await marks(*bots)
    pb.show_actor(ctx.plugin, pa)
    after = await settle(ctx, since)
    assert after["VhdB"] == [("list_add", uuid_a), ("add_player", pa.runtime_id)], (
        after["VhdB"]
    )
    assert after["VhdA"] == [] and after["VhdC"] == [], after
    assert pb.can_see(pa) and not pa.can_see(pb)

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhdA"] == [("list_add", uuid_b), ("add_player", pb.runtime_id)], (
        after["VhdA"]
    )
    assert after["VhdB"] == [] and after["VhdC"] == [], after
    assert [pair(e) for e in show.events] == [("VhdB", "VhdA"), ("VhdA", "VhdB")], (
        show.events
    )


@scenario("visibility.hide.forgotten_on_leave", budget=BUDGET)
async def hide_forgotten_on_leave(ctx: Context) -> None:
    a, pa = await join(ctx, "VhfA")
    b, pb = await join(ctx, "VhfB")
    pa.hide_actor(ctx.plugin, pb)
    assert not pa.can_see(pb)
    await leave(ctx, "VhfB")

    b, pb = await join(ctx, "VhfB")
    assert pa.can_see(pb), "a hide outlived the actor that was hidden"


@scenario("visibility.hide.forgotten_on_mob_remove", plot=True, budget=BUDGET)
async def hide_forgotten_on_mob_remove(ctx: Context) -> None:
    a, pa = await join(ctx, "VhmA")
    ctx.run(f"summon pig {ctx.plot.at(*ctx.plot.middle)}")
    mob = await ctx.until(
        lambda: next(
            (x for x in ctx.level.actors if str(x.type) == "minecraft:pig"), None
        ),
        timeout=20,
        what="the pig",
    )
    pa.hide_actor(ctx.plugin, mob)
    assert not pa.can_see(mob)
    mob.remove()
    assert pa.can_see(mob), "a hide outlived the mob that was hidden"


@scenario("visibility.hide.kept_on_dimension_change", plot=True, budget=BUDGET)
async def hide_kept_on_dimension_change(ctx: Context) -> None:
    a, pa = await join(ctx, "VhdcA")
    removed = ctx.capture(
        ActorRemoveEvent, lambda e: str(e.actor.type) == "minecraft:pig"
    )
    ctx.run(f"summon pig {ctx.plot.at(*ctx.plot.middle)}")
    mob = await ctx.until(
        lambda: next(
            (x for x in ctx.level.actors if str(x.type) == "minecraft:pig"), None
        ),
        timeout=20,
        what="the pig",
    )
    pa.hide_actor(ctx.plugin, mob)
    ctx.run("execute in nether run tp @e[type=pig,c=1] 0 100 0")
    await ctx.until(
        lambda: any(
            str(x.type) == "minecraft:pig"
            for x in ctx.level.get_dimension("nether").actors
        ),
        timeout=20,
        what="the pig in the nether",
    )
    assert removed.count == 0, (
        f"a dimension change fired ActorRemoveEvent: {removed.events}"
    )
    moved = next(
        x
        for x in ctx.level.get_dimension("nether").actors
        if str(x.type) == "minecraft:pig"
    )
    assert not pa.can_see(moved), (
        "a hide was forgotten when the actor changed dimension"
    )


@scenario("visibility.hide.self", budget=BUDGET)
async def hide_self(ctx: Context) -> None:
    hide = ctx.capture(PlayerHideActorEvent, watching("VhsA"))
    show = ctx.capture(PlayerShowActorEvent, watching("VhsA"))
    a, pa = await join(ctx, "VhsA")
    b, pb = await join(ctx, "VhsB")
    since = await marks(a, b)
    pa.hide_actor(ctx.plugin, pa)
    pa.show_actor(ctx.plugin, pa)
    after = await settle(ctx, since)
    assert after == {"VhsA": [], "VhsB": []}, after
    assert hide.count == 0 and show.count == 0, (hide.events, show.events)
    assert pa.can_see(pa)


@scenario(
    "visibility.show.current_skin",
    covers={PlayerShowActorEvent: "normal"},
    budget=BUDGET,
)
async def show_current_skin(ctx: Context) -> None:
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vhk")
    bots = (a, b, c)
    uuid_b = str(pb.unique_id)
    join_skin = (await list_adds(a, uuid_b))[0]["skin"]
    since = await marks(*bots)
    pa.hide_actor(ctx.plugin, pb)
    await settle(ctx, since)

    since = await marks(*bots)
    await b.call("local_player.change_skin", "vh-new-skin", [255, 0, 0, 255])
    skins: list[dict] = []
    for _ in range(100):
        skins = await received(c, "PlayerSkinPacket", since[c])
        if skins:
            break
        await ctx.ticks(1)
    else:
        raise AssertionError("the control client never learned about the skin change")
    await settle(ctx, since)
    new_skin = skins[-1]["packet"]["skin"]
    assert new_skin["id"] == "vh-new-skin" and new_skin["id"] != join_skin["id"], (
        new_skin["id"]
    )

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhkA"] == [("list_add", uuid_b), ("add_player", pb.runtime_id)], (
        after["VhkA"]
    )
    assert after["VhkB"] == [] and after["VhkC"] == [], after
    entries = await list_adds(a, uuid_b, since[a])
    assert len(entries) == 1, entries
    entry_matches_server(entries[0], pb, skin=False)
    assert entries[0]["skin"]["id"] == "vh-new-skin", (
        f"show sent the join skin {entries[0]['skin']['id']}"
    )
    assert entries[0]["skin"]["image_data"] == new_skin["image_data"], (
        "the entry carries a stale skin image"
    )
    assert entries[0]["skin"]["image_data"] != join_skin["image_data"]


@scenario("visibility.skin.player_api", budget=BUDGET)
async def skin_player_api(ctx: Context) -> None:
    changed = ctx.capture(
        PlayerSkinChangeEvent,
        watching("VhlB"),
        extra=lambda e: {"new_skin_id": e.new_skin.id},
    )
    a, pa = await join(ctx, "VhlA")
    b, pb = await join(ctx, "VhlB")
    join_skin = pb.skin.id
    await b.call("local_player.change_skin", "vh-api-skin", [0, 255, 0, 255])
    await ctx.until(lambda: changed.count, timeout=100, what="PlayerSkinChangeEvent")
    assert changed.events[0]["new_skin_id"] == "vh-api-skin", changed.events
    for _ in range(100):
        if pb.skin.id != join_skin:
            break
        await ctx.ticks(1)
    assert pb.skin.id == "vh-api-skin", (
        f"Player.skin is still {pb.skin.id} 100 ticks after PlayerSkinChangeEvent"
    )


async def bodies(bot, since: int) -> list[dict]:
    return [p["packet"]["body"] for p in await received(bot, "TextPacket", since)]


def skin_notice(body: dict, name: str) -> bool:
    return (
        body["type"] == "TEXT_OBJECT"
        and "changeToSkin" in body["message"]
        and name in body["message"]
    )


@scenario(
    "visibility.skin.normal", covers={PlayerSkinChangeEvent: "normal"}, budget=BUDGET
)
async def skin_normal(ctx: Context) -> None:
    changed = ctx.capture(
        PlayerSkinChangeEvent,
        watching("VsnA"),
        extra=lambda e: {"new_skin_id": e.new_skin.id},
    )
    a, pa = await join(ctx, "VsnA")
    b, pb = await join(ctx, "VsnB")
    since = await marks(a, b)
    await a.call("local_player.change_skin", "vs-normal", [0, 0, 255, 255])
    await ctx.until(lambda: changed.count, timeout=100, what="PlayerSkinChangeEvent")
    await settle(ctx, since)
    assert changed.count == 1 and changed.events[0]["new_skin_id"] == "vs-normal", (
        changed.events
    )
    assert changed.events[0]["is_cancelled"] is False
    relayed = await received(b, "PlayerSkinPacket", since[b])
    assert [p["packet"]["skin"]["id"] for p in relayed] == ["vs-normal"], relayed
    assert relayed[0]["packet"]["uuid"] == str(pa.unique_id)
    notices = [x for x in await bodies(b, since[b]) if skin_notice(x, "VsnA")]
    assert len(notices) == 1, notices


@scenario(
    "visibility.skin.cancel", covers={PlayerSkinChangeEvent: "cancel"}, budget=BUDGET
)
async def skin_cancel(ctx: Context) -> None:
    accepted = ctx.capture(PlayerSkinChangeEvent, watching("VscA"))
    a, pa = await join(ctx, "VscA")
    b, pb = await join(ctx, "VscB")
    join_skin = pa.skin.id
    since = await marks(a, b)
    await a.call("local_player.change_skin", "vs-first", [0, 0, 255, 255])
    await ctx.until(
        lambda: accepted.count, timeout=100, what="the first PlayerSkinChangeEvent"
    )
    await ctx.until(
        lambda: pa.skin.id == "vs-first",
        timeout=100,
        what="Player.skin to report the first change",
    )
    await settle(ctx, since)
    assert [
        p["packet"]["skin"]["id"]
        for p in await received(b, "PlayerSkinPacket", since[b])
    ] == ["vs-first"]
    ctx.release(accepted)

    changed = ctx.capture(PlayerSkinChangeEvent, watching("VscA"), cancel=True)
    since = await marks(a, b)
    await a.call("local_player.change_skin", "vs-cancel", [255, 0, 0, 255])
    await ctx.until(lambda: changed.count, timeout=100, what="PlayerSkinChangeEvent")
    await settle(ctx, since)
    assert await received(b, "PlayerSkinPacket", since[b]) == [], (
        "another client was told about a cancelled skin change"
    )
    assert not [x for x in await bodies(b, since[b]) if skin_notice(x, "VscA")]
    assert pa.skin.id == "vs-first", pa.skin.id
    reverted = await received(a, "PlayerSkinPacket", since[a])
    reverted_ids = [p["packet"]["skin"]["id"] for p in reverted]
    assert reverted_ids == ["vs-first"], (
        f"the cancel sent {reverted_ids}, not the current skin (join skin {join_skin})"
    )
    c, pc = await join(ctx, "VscC")
    entries = await list_adds(c, str(pa.unique_id))
    assert len(entries) == 1 and entries[0]["skin"]["id"] == "vs-first", entries


@scenario(
    "visibility.skin.mutate", covers={PlayerSkinChangeEvent: "mutate"}, budget=BUDGET
)
async def skin_mutate(ctx: Context) -> None:
    ctx.capture(
        PlayerSkinChangeEvent,
        watching("VsmA"),
        mutate=lambda e: setattr(e, "skin_change_message", "QA new look"),
    )
    ctx.capture(
        PlayerSkinChangeEvent,
        watching("VsmC"),
        mutate=lambda e: setattr(e, "skin_change_message", None),
    )
    a, pa = await join(ctx, "VsmA")
    b, pb = await join(ctx, "VsmB")
    c, pc = await join(ctx, "VsmC")
    since = await marks(a, b, c)
    await a.call("local_player.change_skin", "vs-mutate", [0, 255, 0, 255])
    await hear(ctx, b, since[b], "QA new look")
    notices = [x for x in await bodies(b, since[b]) if skin_notice(x, "VsmA")]
    assert notices == [], f"the default skin message was sent as well: {notices}"
    mid = await marks(a, b, c)
    await c.call("local_player.change_skin", "vs-silent", [0, 255, 0, 255])
    for _ in range(100):
        if await received(b, "PlayerSkinPacket", mid[b]):
            break
        await ctx.ticks(1)
    else:
        raise AssertionError("the skin change with no message was never relayed")
    await settle(ctx, mid)
    assert not [
        x
        for x in await bodies(b, mid[b])
        if x["type"] != "RAW" or "@@vis" not in x["message"]
    ]


EMOTE = "4c8ae710-df2e-47cd-814d-cc7bf21a3d67"


async def emotes(bot, since: int) -> list[dict]:
    return [p["packet"] for p in await received(bot, "EmotePacket", since)]


async def wait_emote(ctx: Context, bot, since: int) -> list[dict]:
    for _ in range(100):
        got = await emotes(bot, since)
        if got:
            return got
        await ctx.ticks(1)
    raise AssertionError(f"{bot.name} never received an emote")


MUTE_BIT = 2


@scenario("visibility.emote.normal", covers={PlayerEmoteEvent: "normal"}, budget=BUDGET)
async def emote_normal(ctx: Context) -> None:
    emote = ctx.capture(PlayerEmoteEvent, watching("VenA"))
    a, pa = await join(ctx, "VenA")
    b, pb = await join(ctx, "VenB")
    since = await marks(a, b)
    await a.call("local_player.emote", EMOTE, False)
    got = await wait_emote(ctx, b, since[b])
    await settle(ctx, since)
    assert (
        emote.count == 1
        and emote.events[0]["emote_id"] == EMOTE
        and emote.events[0]["is_muted"] is False
    ), emote.events
    assert (
        len(got) == 1
        and got[0]["piece_id"] == EMOTE
        and got[0]["runtime_id"] == pa.runtime_id
    ), got
    assert not got[0]["flags"] & MUTE_BIT
    mid = await marks(a, b)
    await a.call("local_player.emote", EMOTE, True)
    muted = await wait_emote(ctx, b, mid[b])
    assert emote.count == 2 and emote.events[1]["is_muted"] is True, emote.events
    assert muted[0]["flags"] & MUTE_BIT


@scenario("visibility.emote.cancel", covers={PlayerEmoteEvent: "cancel"}, budget=BUDGET)
async def emote_cancel(ctx: Context) -> None:
    emote = ctx.capture(PlayerEmoteEvent, watching("VecA"), cancel=True)
    a, pa = await join(ctx, "VecA")
    b, pb = await join(ctx, "VecB")
    since = await marks(a, b)
    await a.call("local_player.emote", EMOTE, False)
    await ctx.until(lambda: emote.count, timeout=60, what="PlayerEmoteEvent")
    await settle(ctx, since)
    assert emote.count == 1
    assert await emotes(b, since[b]) == [], "a cancelled emote was relayed"
    assert await emotes(a, since[a]) == []


@scenario("visibility.emote.mutate", covers={PlayerEmoteEvent: "mutate"}, budget=BUDGET)
async def emote_mutate(ctx: Context) -> None:
    mute = ctx.capture(
        PlayerEmoteEvent,
        watching("VemA"),
        mutate=lambda e: setattr(e, "is_muted", True),
    )
    unmute = ctx.capture(
        PlayerEmoteEvent,
        watching("VemC"),
        mutate=lambda e: setattr(e, "is_muted", False),
    )
    a, pa = await join(ctx, "VemA")
    b, pb = await join(ctx, "VemB")
    c, pc = await join(ctx, "VemC")
    since = await marks(a, b, c)
    await a.call("local_player.emote", EMOTE, False)
    got = await wait_emote(ctx, b, since[b])
    assert (
        mute.events[0]["is_muted"] is False
        and mute.events[0]["after"]["is_muted"] is True
    )
    assert got[0]["flags"] & MUTE_BIT, f"the emote relayed to B is not muted: {got}"
    mid = await marks(a, b, c)
    await c.call("local_player.emote", EMOTE, True)
    got = await wait_emote(ctx, b, mid[b])
    assert (
        unmute.events[0]["is_muted"] is True
        and unmute.events[0]["after"]["is_muted"] is False
    )
    assert not got[0]["flags"] & MUTE_BIT, (
        f"the emote relayed to B is still muted: {got}"
    )


@scenario(
    "visibility.hide.plugin_holds",
    covers={PlayerHideActorEvent: "normal", PlayerShowActorEvent: "normal"},
    budget=BUDGET,
)
async def hide_plugin_holds(ctx: Context) -> None:
    hide = ctx.capture(PlayerHideActorEvent, watching("VhpA", "VhpB", "VhpC"))
    show = ctx.capture(PlayerShowActorEvent, watching("VhpA", "VhpB", "VhpC"))
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vhp")
    bots = (a, b, c)
    other = fixture(ctx)
    uuid_b = str(pb.unique_id)

    since = await marks(*bots)
    pa.hide_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhpA"] == [("list_remove", uuid_b), ("remove_actor", pb.id)], after[
        "VhpA"
    ]

    since = await marks(*bots)
    pa.hide_actor(other, pb)
    after = await settle(ctx, since)
    assert hide.count == 1, (
        f"a second plugin hiding an already hidden actor fired the event again: {hide.events}"
    )
    assert all(lines == [] for lines in after.values()), after

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert not pa.can_see(pb), (
        "the actor became visible while another plugin still hides it"
    )
    assert show.count == 0, show.events
    assert all(lines == [] for lines in after.values()), after

    since = await marks(*bots)
    pa.show_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert (
        not pa.can_see(pb)
        and show.count == 0
        and all(lines == [] for lines in after.values())
    ), after

    since = await marks(*bots)
    pa.show_actor(other, pb)
    after = await settle(ctx, since)
    assert pa.can_see(pb)
    assert show.count == 1 and hide.count == 1, (show.events, hide.events)
    assert after["VhpA"] == [("list_add", uuid_b), ("add_player", pb.runtime_id)], (
        after["VhpA"]
    )
    assert after["VhpB"] == [] and after["VhpC"] == [], after

    pa.hide_actor(other, pb)
    pa.hide_actor(ctx.plugin, pb)
    pa.show_actor(other, pb)
    assert not pa.can_see(pb)
    pa.show_actor(ctx.plugin, pb)
    assert pa.can_see(pb)
    assert hide.count == 2 and show.count == 2, (hide.count, show.count)


@scenario("visibility.hide.hidden_player_rejoins", budget=2500)
async def hidden_player_rejoins(ctx: Context) -> None:
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vhj")
    first = {"uuid": str(pb.unique_id), "id": pb.id, "runtime_id": pb.runtime_id}
    pa.hide_actor(ctx.plugin, pb)
    await ctx.ticks(5)
    assert not pa.can_see(pb)

    await leave(ctx, "VhjB")
    since = await marks(a, c)
    b, pb = await join(ctx, "VhjB")
    after = await settle(ctx, since)
    uuid_b = str(pb.unique_id)
    ctx.info["rejoin"] = {
        "before": first,
        "after": {"uuid": uuid_b, "id": pb.id, "runtime_id": pb.runtime_id},
    }
    ctx.info["a_view"] = after["VhjA"]
    ctx.info["a_can_see"] = pa.can_see(pb)

    assert ("list_add", uuid_b) in after["VhjC"] and (
        "add_player",
        pb.runtime_id,
    ) in after["VhjC"], after["VhjC"]
    entries = await list_adds(c, uuid_b, since[c])
    assert len(entries) == 1, entries
    entry_matches_server(entries[0], pb)

    told_a = ("list_add", uuid_b) in after["VhjA"] or (
        "add_player",
        pb.runtime_id,
    ) in after["VhjA"]
    if pa.can_see(pb):
        assert ("list_add", uuid_b) in after["VhjA"] and (
            "add_player",
            pb.runtime_id,
        ) in after["VhjA"], (
            f"the server says A can see the rejoined B, but A was sent {after['VhjA']}"
        )
    else:
        assert not told_a, (
            f"the server says A cannot see the rejoined B, but A was sent {after['VhjA']}"
        )


@scenario("visibility.hide.hider_rejoins", budget=2500)
async def hider_rejoins(ctx: Context) -> None:
    (a, pa), (b, pb), (c, pc) = await three(ctx, "Vhr")
    pa.hide_actor(ctx.plugin, pb)
    pa.hide_actor(ctx.plugin, pc)
    await ctx.ticks(5)
    assert not pa.can_see(pb) and not pa.can_see(pc)

    await leave(ctx, "VhrA")
    a, pa = await join(ctx, "VhrA")
    await settle(ctx, {a: 0})
    assert pa.can_see(pb) and pa.can_see(pc), "a hold survived the hider leaving"
    seen = await view(a, 0)
    for other in (pb, pc):
        assert ("list_add", str(other.unique_id)) in seen, (other.name, seen)
        assert ("add_player", other.runtime_id) in seen, (other.name, seen)
        assert ("remove_actor", other.id) not in seen, seen

    since = await marks(a, b, c)
    pa.hide_actor(ctx.plugin, pb)
    after = await settle(ctx, since)
    assert after["VhrA"] == [
        ("list_remove", str(pb.unique_id)),
        ("remove_actor", pb.id),
    ], after["VhrA"]
    assert after["VhrB"] == [] and after["VhrC"] == [], after


@scenario(
    "visibility.hide.actor",
    covers={PlayerHideActorEvent: "normal", PlayerShowActorEvent: "normal"},
    plot=True,
    budget=3000,
)
async def hide_actor(ctx: Context) -> None:
    hide = ctx.capture(PlayerHideActorEvent, watching("VhaA", "VhaC"))
    show = ctx.capture(PlayerShowActorEvent, watching("VhaA", "VhaC"))
    a, pa = await join(ctx, "VhaA")
    c, pc = await join(ctx, "VhaC")
    px, py, pz = ctx.plot.pos(8, 0, 8)
    home = (px + 0.5, py, pz + 0.5)

    def at_home() -> bool:
        return all(
            abs(p.location.x - home[0]) < 0.2 and abs(p.location.z - home[2]) < 0.2
            for p in (pa, pc)
        )

    for name in ("VhaA", "VhaC"):
        ctx.run(f"tp {name} {home[0]} {home[1]} {home[2]}")
    await ctx.until(at_home, timeout=60, what="both players in the plot")
    await ctx.ticks(20)
    ctx.run(f"summon pig {px + 3.5} {py} {pz + 0.5}")
    pig = await ctx.until(
        lambda: next(
            (
                x
                for x in ctx.level.actors
                if str(x.type) == "minecraft:pig"
                and abs(x.location.x - px) < 16
                and abs(x.location.z - pz) < 16
            ),
            None,
        ),
        timeout=60,
        what="the pig",
    )
    for bot in (a, c):
        for _ in range(60):
            if ("add_actor", pig.id) in await view(bot, 0):
                break
            await ctx.ticks(1)
        else:
            raise AssertionError(f"{bot.name} was never sent the pig")

    since = await marks(a, c)
    pa.hide_actor(ctx.plugin, pig)
    after = await settle(ctx, since, pig)
    assert after["VhaA"] == [("remove_actor", pig.id)], after["VhaA"]
    assert after["VhaC"] == [], after["VhaC"]
    assert hide.count == 1 and hide.events[0]["actor"]["type"] == "minecraft:pig", (
        hide.events
    )
    assert not pa.can_see(pig) and pc.can_see(pig)

    for name in ("VhaA", "VhaC"):
        ctx.run(f"tp {name} {px + 400} {py} {pz}")
    await ctx.until(
        lambda: all(p.location.x > px + 300 for p in (pa, pc)),
        timeout=100,
        what="both far from the pig",
    )
    await ctx.ticks(60)
    since = await marks(a, c)
    for name in ("VhaA", "VhaC"):
        ctx.run(f"tp {name} {home[0]} {home[1]} {home[2]}")
    await ctx.until(at_home, timeout=100, what="both players back")
    for _ in range(100):
        if ("add_actor", pig.id) in await view(c, since[c]):
            break
        await ctx.ticks(1)
    else:
        raise AssertionError(
            "the control client was never sent the pig again on re-entering range"
        )
    after = await settle(ctx, since, pig)
    assert ("add_actor", pig.id) not in after["VhaA"], (
        f"a hidden pig was sent to its hider: {after['VhaA']}"
    )

    since = await marks(a, c)
    pa.show_actor(ctx.plugin, pig)
    after = await settle(ctx, since, pig)
    assert pa.can_see(pig)
    assert after["VhaA"] == [("add_actor", pig.id)], after["VhaA"]
    assert after["VhaC"] == [], after["VhaC"]
    assert show.count == 1 and show.events[0]["actor"]["type"] == "minecraft:pig", (
        show.events
    )
    pig.remove()


def carries(token: str, player: str | None = None):
    def where(event) -> bool:
        if event.packet_id != TEXT_PACKET or token.encode() not in event.payload:
            return False
        return player is None or (
            event.player is not None and event.player.name == player
        )

    return where


def swapping(token: str, replacement: str):
    assert len(token) == len(replacement)

    def mutate(event) -> None:
        event.payload = event.payload.replace(token.encode(), replacement.encode())

    return mutate


@scenario(
    "visibility.packet.send.normal", covers={PacketSendEvent: "normal"}, budget=BUDGET
)
async def packet_send_normal(ctx: Context) -> None:
    token = f"@@psend {uuid.uuid4().hex[:6]}"
    sent = ctx.capture(PacketSendEvent, carries(token))
    a, pa = await join(ctx, "VpsA")
    b, pb = await join(ctx, "VpsB")
    since = await marks(a, b)
    ctx.server.broadcast_message(token)
    for bot in (a, b):
        await hear(ctx, bot, since[bot], token)
    assert sorted(e["player"]["name"] for e in sent.events if e["player"]) == [
        "VpsA",
        "VpsB",
    ], sent.events
    for event in sent.events:
        assert event["packet_id"] == TEXT_PACKET
        assert event["sub_client_id"] == 0
        assert event["payload"]["bytes"] > len(token), event["payload"]
        assert event["address"] and event["address"] != "None", event["address"]
    for bot in (a, b):
        assert (await messages(bot, since[bot])).count(token) == 1


@scenario(
    "visibility.packet.send.cancel", covers={PacketSendEvent: "cancel"}, budget=BUDGET
)
async def packet_send_cancel(ctx: Context) -> None:
    token = f"@@pdrop {uuid.uuid4().hex[:6]}"
    dropped = ctx.capture(PacketSendEvent, carries(token, "VpcA"), cancel=True)
    a, pa = await join(ctx, "VpcA")
    b, pb = await join(ctx, "VpcB")
    since = await marks(a, b)
    ctx.server.broadcast_message(token)
    await hear(ctx, b, since[b], token)
    await settle(ctx, since)
    assert dropped.count == 1, dropped.events
    assert token not in await messages(a, since[a]), (
        "the client received a packet whose send was cancelled"
    )
    assert await a.call("is_connected") and ctx.server.get_player("VpcA") is not None


@scenario(
    "visibility.packet.send.mutate", covers={PacketSendEvent: "mutate"}, budget=BUDGET
)
async def packet_send_mutate(ctx: Context) -> None:
    token = f"@@pmut {uuid.uuid4().hex[:6]}"
    swapped = token.replace("@@pmut", "@@PMUT")
    sent = ctx.capture(
        PacketSendEvent, carries(token, "VpmA"), mutate=swapping(token, swapped)
    )
    a, pa = await join(ctx, "VpmA")
    b, pb = await join(ctx, "VpmB")
    since = await marks(a, b)
    ctx.server.broadcast_message(token)
    await hear(ctx, a, since[a], swapped)
    await hear(ctx, b, since[b], token)
    heard_a = await messages(a, since[a])
    heard_b = await messages(b, since[b])
    assert sent.count == 1, sent.events
    assert token not in heard_a and heard_a.count(swapped) == 1, heard_a
    assert swapped not in heard_b and heard_b.count(token) == 1, heard_b


@scenario(
    "visibility.packet.send.nested", covers={PacketSendEvent: "mutate"}, budget=BUDGET
)
async def packet_send_nested(ctx: Context) -> None:
    token = f"@@pnest {uuid.uuid4().hex[:6]}"
    inner = token.replace("@@pnest", "@@INNER")
    nested: list[bytes] = []

    def send_inside(event) -> None:
        if not nested:
            nested.append(event.payload.replace(token.encode(), inner.encode()))
        event.player.send_packet(TEXT_PACKET, nested[0])

    sent = ctx.capture(PacketSendEvent, carries(token, "VpnA"), mutate=send_inside)
    a, pa = await join(ctx, "VpnA")
    b, pb = await join(ctx, "VpnB")
    c, pc = await join(ctx, "VpnC")
    since = await marks(a, b, c)
    ctx.server.broadcast_message(token)
    await settle(ctx, since)
    heard = {bot.name: await messages(bot, since[bot]) for bot in (a, b, c)}
    assert sent.count == 1, sent.events
    assert heard["VpnA"].count(token) == 1, heard["VpnA"]
    assert heard["VpnA"].count(inner) == 1, heard["VpnA"]
    for name in ("VpnB", "VpnC"):
        assert heard[name].count(token) == 1 and inner not in heard[name], (
            name,
            heard[name],
        )


@scenario(
    "visibility.packet.receive.normal",
    covers={PacketReceiveEvent: "normal"},
    budget=BUDGET,
)
async def packet_receive_normal(ctx: Context) -> None:
    line = f"psay{uuid.uuid4().hex[:6]}"
    got = ctx.capture(PacketReceiveEvent, carries(line))
    a, pa = await join(ctx, "VprA")
    b, pb = await join(ctx, "VprB")
    since = await marks(a, b)
    await a.call("local_player.chat", line)
    await hear(ctx, b, since[b], line)
    assert got.count == 1, got.events
    event = got.events[0]
    assert event["packet_id"] == TEXT_PACKET and event["sub_client_id"] == 0
    assert event["player"]["name"] == "VprA", event["player"]
    assert event["payload"]["bytes"] > len(line)
    assert event["address"] and event["address"] != "None"


@scenario(
    "visibility.packet.receive.cancel",
    covers={PacketReceiveEvent: "cancel"},
    budget=BUDGET,
)
async def packet_receive_cancel(ctx: Context) -> None:
    line = f"pnope{uuid.uuid4().hex[:6]}"
    got = ctx.capture(PacketReceiveEvent, carries(line), cancel=True)
    chat = ctx.capture(PlayerChatEvent, lambda e: line in e.message)
    a, pa = await join(ctx, "VpxA")
    b, pb = await join(ctx, "VpxB")
    since = await marks(a, b)
    await a.call("local_player.chat", line)
    await ctx.until(lambda: got.count, timeout=60, what="PacketReceiveEvent")
    await settle(ctx, since)
    assert chat.count == 0, (
        f"a cancelled chat packet still reached the server: {chat.events}"
    )
    assert line not in await messages(a, since[a]) and line not in await messages(
        b, since[b]
    )
    assert await a.call("is_connected")

    follow = f"pyes{uuid.uuid4().hex[:6]}"
    since = await marks(a, b)
    await a.call("local_player.chat", follow)
    await hear(ctx, b, since[b], follow)
    assert chat.count == 0 and got.count == 1


@scenario(
    "visibility.packet.receive.mutate",
    covers={PacketReceiveEvent: "mutate"},
    budget=BUDGET,
)
async def packet_receive_mutate(ctx: Context) -> None:
    line = f"pone{uuid.uuid4().hex[:6]}"
    other = "ptwo" + line[4:]
    got = ctx.capture(PacketReceiveEvent, carries(line), mutate=swapping(line, other))
    chat = ctx.capture(PlayerChatEvent, lambda e: e.message in (line, other))
    a, pa = await join(ctx, "VpyA")
    b, pb = await join(ctx, "VpyB")
    since = await marks(a, b)
    await a.call("local_player.chat", line)
    await hear(ctx, b, since[b], other)
    assert got.count == 1, got.events
    assert [e["message"] for e in chat.events] == [other], chat.events
    assert line not in await messages(b, since[b])
