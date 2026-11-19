import json

from endstone import Player
from endstone.actor import ActorType
from endstone.block import BlockType
from endstone.brigadier import (
    BoolArgumentType,
    CommandContext,
    CommandDispatcher,
    CommandSyntaxException,
    DoubleArgumentType,
    FloatArgumentType,
    IntegerArgumentType,
    StringArgumentType,
)
from endstone.command import (
    ActorSelectorArgumentResolver,
    ArgumentTypes,
    BasicCommand,
    BlockCommandSender,
    BlockPositionResolver,
    Command,
    CommandExecutor,
    Commands,
    FinePositionResolver,
    PlayerSelectorArgumentResolver,
)
from endstone.inventory import ItemType
from endstone.level import Dimension

from endstone_test.dimensions import CUSTOM_REALM
from endstone_test.harness import Context, scenario
from endstone_test.harness.plot import SIZE
from endstone_test.scenarios.player_actors import barrier
from endstone_test.scenarios.player_apis import got, join, mark, settle, wait_for

FIXTURE = "test_fixture"
ENUM_FLAG = 0x200000
SOFT_ENUM_FLAG = 0x4000000
WARPS = "EndstoneTestWarps"

LEGACY_ARGS = [
    ("int 42", ["int", "42"]),
    ("float 1.5", ["float", "1.5"]),
    ("str word", ["str", "word"]),
    ('str "two words"', ["str", "two words"]),
    ("bool true", ["bool", "true"]),
    ("player @a", ["player", "@a"]),
    ("actor @e[type=zombie]", ["actor", "@e[type=zombie]"]),
    ("pos 1 ~2 3", ["pos", "1 ~2 3"]),
    ("block_pos 1 2 3", ["block_pos", "1 2 3"]),
    ("entity_type zombie", ["entity_type", "zombie"]),
    ("block stone", ["block", "stone"]),
    ("message hello world", ["message", "hello world"]),
    ('json {"a": [1, 2]}', ["json", '{"a": [1, 2]}']),
    ('block_states ["color"="red"]', ["block_states", '["color"="red"]']),
    ("target @s", ["target", "@s"]),
    ("entity @e[type=cow]", ["entity", "@e[type=cow]"]),
    ("vec3 1 2 3", ["vec3", "1 2 3"]),
    ("vec3i 1 ~ 3", ["vec3i", "1 ~ 3"]),
    ("vec3f ^1 ^2 ^3", ["vec3f", "^1 ^2 ^3"]),
]


class RecordingCommand(Command):
    def __init__(self, name: str, usages: list[str]) -> None:
        super().__init__(name, usages=usages)
        self.calls: list[list[str]] = []

    def execute(self, sender, args: list[str]) -> bool:
        self.calls.append(list(args))
        return True


def symbol(packet: dict, parse_symbol: int) -> tuple:
    if parse_symbol & SOFT_ENUM_FLAG:
        return "soft", packet["soft_enums"][parse_symbol & 0xFFFF]["name"]
    if parse_symbol & ENUM_FLAG:
        found = packet["enums"][parse_symbol & 0xFFFF]
        return (
            "enum",
            found["name"],
            tuple(packet["enum_values"][v] for v in found["values"]),
        )
    return "type", parse_symbol


def view(packet: dict) -> dict:
    commands = {}
    for command in packet["commands"]:
        aliases = ()
        if command["alias_enum"] >= 0:
            aliases = tuple(
                sorted(
                    packet["enum_values"][v]
                    for v in packet["enums"][command["alias_enum"]]["values"]
                )
            )
        overloads = sorted(
            [
                (p["name"], p["optional"], symbol(packet, p["parse_symbol"]))
                for p in overload["params"]
            ]
            for overload in command["overloads"]
        )
        commands[command["name"]] = (
            command["description"],
            command["permission"],
            aliases,
            overloads,
        )
    return commands


def soft_enum(packet: dict, name: str) -> list[str]:
    return next((e["values"] for e in packet["soft_enums"] if e["name"] == name), [])


async def available(ctx: Context, bot, player) -> dict:
    since = await mark(bot)
    player.update_commands()
    await settle(ctx, player, bot, since)
    found = await wait_for(
        ctx,
        bot,
        "AvailableCommandsPacket",
        since,
        lambda f: len(f) >= 1,
        "the command list",
    )
    return found[-1]


@scenario("command.disable_keeps_commands", budget=1500)
async def disable_keeps_commands(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None and fixture.is_enabled, (
        f"the {FIXTURE} plugin is not enabled"
    )
    bot, player = await join(ctx, "CmdRegister", op=True)
    await ctx.ticks(20)
    first = await available(ctx, bot, player)
    assert "fixturecmd" in view(first), sorted(view(first))
    assert FIXTURE in soft_enum(first, "PluginNames"), first["soft_enums"]
    try:
        manager.disable_plugin(fixture)
        await ctx.ticks(2)
        kept = await available(ctx, bot, player)
        assert view(kept)["fixturecmd"] == view(first)["fixturecmd"], sorted(view(kept))
        assert any(e["name"] == "FixturecmdPlace" for e in kept["enums"]), [
            e["name"] for e in kept["enums"]
        ]
        assert FIXTURE not in soft_enum(kept, "PluginNames"), kept["soft_enums"]
        command = ctx.server.get_plugin_command("fixturecmd")
        assert command is not None and command.plugin.name == FIXTURE, command
        before = len(fixture.received)
        result = ctx.command("fixturecmd zombie")
        assert result.messages == [
            f"Cannot execute command 'fixturecmd' in plugin "
            f"{fixture._get_description().full_name} - plugin is disabled."
        ], result.output
        assert len(fixture.received) == before, fixture.received[before:]
    finally:
        manager.enable_plugin(fixture)
    await ctx.ticks(2)
    again = await available(ctx, bot, player)
    assert view(again) == view(first)
    assert soft_enum(again, "PluginNames") == soft_enum(first, "PluginNames")


FIXTURE_TREE_CALLS: list[str] = []


@scenario("command.disable_keeps_tree_commands", budget=1500)
async def disable_keeps_tree_commands(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None and fixture.is_enabled, (
        f"the {FIXTURE} plugin is not enabled"
    )
    label = "fixturetree"

    def run(context: CommandContext) -> int:
        FIXTURE_TREE_CALLS.append("run")
        return 1

    def allowed(source) -> bool:
        FIXTURE_TREE_CALLS.append("requires")
        return True

    if ctx.server.command_map.get_command(label) is None:
        labels = fixture.register_command(
            Commands.literal(label)
            .executes(run)
            .then(Commands.literal("secret").requires(allowed).executes(run))
            .build(),
            "A tree command registered by a plugin.",
            [],
        )
        assert label in labels, labels
    bot, player = await join(ctx, "CmdTreeDisabled", op=True)
    await ctx.ticks(20)
    first = await available(ctx, bot, player)
    assert label in view(first), sorted(view(first))
    FIXTURE_TREE_CALLS.clear()
    assert ctx.command(f"{label} secret").success
    assert "run" in FIXTURE_TREE_CALLS, FIXTURE_TREE_CALLS
    expected = (
        f"Cannot execute command '{label}' in plugin "
        f"{fixture._get_description().full_name} - plugin is disabled."
    )
    try:
        manager.disable_plugin(fixture)
        await ctx.ticks(2)
        FIXTURE_TREE_CALLS.clear()
        kept = await available(ctx, bot, player)
        assert view(kept)[label] == view(first)[label], sorted(view(kept))
        result = ctx.command(f"{label} secret")
        assert result.messages == [expected], result.output
        command = ctx.server.command_map.get_command(label)
        assert command is not None
        assert not command.execute(ctx.server.command_sender, ["secret"])
        since = await mark(bot)
        await bot.call("local_player.run_command", f"/{label} secret")
        await wait_for(
            ctx,
            bot,
            "TextPacket",
            since,
            lambda found: any(expected in json.dumps(p) for p in found),
            what="the disabled plugin message",
        )
        assert FIXTURE_TREE_CALLS == [], FIXTURE_TREE_CALLS
    finally:
        manager.enable_plugin(fixture)
    await ctx.ticks(2)
    assert ctx.command(f"{label} secret").success
    assert view(await available(ctx, bot, player))[label] == view(first)[label]


@scenario("command.reload_clean", budget=4000)
async def reload_clean(ctx: Context) -> None:
    stash = ctx.stash
    stage = stash.get("stage", 0)
    if stage < 2:
        if stage == 1:
            bot, player = await join(ctx, "CmdReloadA", op=True)
            await ctx.ticks(20)
            stash["first"] = view(await available(ctx, bot, player))
        stash["stage"] = stage + 1
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    bot, player = await join(ctx, "CmdReloadB", op=True)
    await ctx.ticks(20)
    second = view(await available(ctx, bot, player))
    assert second == stash["first"], sorted(set(second) ^ set(stash["first"]))


@scenario("command.legacy_clash_registered")
async def legacy_clash_registered(ctx: Context) -> None:
    command = RecordingCommand(
        "endstone_test_clash",
        [
            "/endstone_test_clash <target: player> [reason: message]",
            "/endstone_test_clash <name: str>",
        ],
    )
    results = {}
    lines = await ctx.log_of(
        lambda: results.__setitem__(
            "ok", ctx.server.command_map.register_command(command)
        )
    )
    assert results["ok"] is True
    refused = [line for line in lines if "Unable to register" in line]
    assert not refused, refused
    result = ctx.command("endstone_test_clash somebody")
    assert result.success, result.output
    assert command.calls == [["somebody"]], command.calls


@scenario("command.legacy_keyword_beside_argument", budget=1500)
async def legacy_keyword_beside_argument(ctx: Context) -> None:
    bot, player = await join(ctx, "CmdLegacyWarp", op=True)
    await ctx.ticks(20)
    for name, usages in [
        (
            "endstone_test_warp_first",
            [
                "/endstone_test_warp_first (list)<action: EndstoneTestWarpFirstList>",
                "/endstone_test_warp_first <name: str>",
            ],
        ),
        (
            "endstone_test_warp_second",
            [
                "/endstone_test_warp_second <name: str>",
                "/endstone_test_warp_second (list)<action: EndstoneTestWarpSecondList>",
            ],
        ),
    ]:
        command = RecordingCommand(name, usages)
        lines = await ctx.log_of(
            lambda command=command: ctx.server.command_map.register_command(command)
        )
        refused = [line for line in lines if "Unable to register" in line]
        assert not refused, refused
        await ctx.ticks(2)
        overloads = view(await available(ctx, bot, player))[name][3]
        assert len(overloads) == 2, overloads
        for line, args in [(f"{name} list", ["list"]), (f"{name} spawn", ["spawn"])]:
            command.calls.clear()
            result = ctx.command(line)
            assert result.success, (line, result.output)
            assert command.calls == [args], (line, command.calls)


@scenario("command.legacy_optional_before_required", budget=1500)
async def legacy_optional_before_required(ctx: Context) -> None:
    label = "endstone_test_optreq"
    command = RecordingCommand(label, [f"/{label} [list] <name: str>"])
    lines = await ctx.log_of(lambda: ctx.server.command_map.register_command(command))
    refused = [line for line in lines if "Unable to register" in line]
    assert not refused, refused
    bot, player = await join(ctx, "CmdLegacyOptReq", op=True)
    await ctx.ticks(20)
    forms = [
        [p[0] for p in o] for o in view(await available(ctx, bot, player))[label][3]
    ]
    assert sorted(forms) == [["list", "name"], ["name"]], forms
    for line, args in [
        (f"{label} foo", ["foo"]),
        (f"{label} list foo", ["list", "foo"]),
    ]:
        command.calls.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert command.calls == [args], (line, command.calls)
    for line in [label, f"{label} list", f"{label} foo bar"]:
        command.calls.clear()
        result = ctx.command(line)
        assert not result.success, (line, result.output)
        assert any("Syntax error" in e for e in result.errors), (line, result.errors)
        assert command.calls == [], (line, command.calls)


@scenario("command.tree_literal_beside_argument")
async def tree_literal_beside_argument(ctx: Context) -> None:
    calls: list[tuple] = []

    def listed(context: CommandContext) -> int:
        calls.append(("list",))
        return 1

    def named(context: CommandContext) -> int:
        calls.append(("name", context.get_argument("name")))
        return 1

    for label, children in [
        (
            "endstone_test_twarp_first",
            [
                Commands.literal("list").executes(listed),
                Commands.argument("name", StringArgumentType.word()).executes(named),
            ],
        ),
        (
            "endstone_test_twarp_second",
            [
                Commands.argument("name", StringArgumentType.word()).executes(named),
                Commands.literal("list").executes(listed),
            ],
        ),
    ]:
        node = Commands.literal(label)
        for child in children:
            node = node.then(child)
        assert label in tree(ctx, node.build()), label
        await ctx.ticks(2)
        for line, expected in [
            (f"{label} list", ("list",)),
            (f"{label} LIST", ("list",)),
            (f"{label} spawn", ("name", "spawn")),
            (f'{label} "list"', ("name", "list")),
        ]:
            calls.clear()
            result = ctx.command(line)
            assert result.success, (line, result.output)
            assert calls == [expected], (line, calls)


@scenario("command.optional_message")
async def optional_message(ctx: Context) -> None:
    command = RecordingCommand(
        "endstone_test_spark",
        [
            "/endstone_test_spark (profiler|sampler)<module: EndstoneTestSparkModule> "
            "(start|stop|info)[action: EndstoneTestSparkAction] [flags: message]"
        ],
    )
    assert ctx.server.command_map.register_command(command)
    for line, args in [
        ("endstone_test_spark profiler", ["profiler"]),
        ("endstone_test_spark profiler info", ["profiler", "info"]),
        (
            "endstone_test_spark sampler start --interval 10",
            ["sampler", "start", "--interval 10"],
        ),
    ]:
        command.calls.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert command.calls == [args], (line, command.calls)


@scenario("command.uppercase_enum")
async def uppercase_enum(ctx: Context) -> None:
    command = RecordingCommand(
        "endstone_test_upper",
        ["/endstone_test_upper (Alpha|BETA)<pick: EndstoneTestUpperPick>"],
    )
    assert ctx.server.command_map.register_command(command)
    for line, args in [
        ("endstone_test_upper beta", ["beta"]),
        ("endstone_test_upper alpha", ["alpha"]),
    ]:
        command.calls.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert command.calls == [args], (line, command.calls)


@scenario("command.vanilla_softenum_survives", budget=1500)
async def vanilla_softenum_survives(ctx: Context) -> None:
    bot, player = await join(ctx, "CmdSoftEnum", op=True)
    await ctx.ticks(20)
    ctx.command("scoreboard objectives remove qa_soft_objective")
    try:
        assert ctx.command("scoreboard objectives add qa_soft_objective dummy").success
        assert ctx.command("tag CmdSoftEnum add qa_soft_tag").success
        assert ctx.server.command_map.register_command(
            RecordingCommand("endstone_test_rebuild", [])
        )
        packet = await available(ctx, bot, player)
        assert "endstone_test_rebuild" in view(packet)
        assert "qa_soft_objective" in soft_enum(packet, "ScoreboardObjectives"), packet[
            "soft_enums"
        ]
        assert "qa_soft_tag" in soft_enum(packet, "TagValues"), packet["soft_enums"]
    finally:
        ctx.command("scoreboard objectives remove qa_soft_objective")
        ctx.command("tag CmdSoftEnum remove qa_soft_tag")


@scenario("command.script_custom_command_survives", budget=4000)
async def script_custom_command_survives(ctx: Context) -> None:
    stash = ctx.stash
    result = ctx.command("endstone_test:scriptcmd")
    assert result.success and "endstone-test scriptcmd ran" in result.output, (
        result.output
    )
    if not stash:
        assert ctx.server.command_map.register_command(
            RecordingCommand("endstone_test_script_rebuild", [])
        )
        assert ctx.command("endstone_test_script_rebuild").success
        result = ctx.command("endstone_test:scriptcmd")
        assert result.success and "endstone-test scriptcmd ran" in result.output, (
            result.output
        )
        stash["reloaded"] = True
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")


@scenario("command.legacy_args_unchanged")
async def legacy_args_unchanged(ctx: Context) -> None:
    command = RecordingCommand(
        "endstone_test_legacy",
        [
            "/endstone_test_legacy int <value: int>",
            "/endstone_test_legacy float <value: float>",
            "/endstone_test_legacy str <value: str>",
            "/endstone_test_legacy bool <value: bool>",
            "/endstone_test_legacy player <value: player>",
            "/endstone_test_legacy actor <value: actor>",
            "/endstone_test_legacy pos <value: pos>",
            "/endstone_test_legacy block_pos <value: block_pos>",
            "/endstone_test_legacy entity_type <value: entity_type>",
            "/endstone_test_legacy block <value: block>",
            "/endstone_test_legacy message <value: message>",
            "/endstone_test_legacy json <value: json>",
            "/endstone_test_legacy block_states <value: block_states>",
            "/endstone_test_legacy target <value: target>",
            "/endstone_test_legacy entity <value: entity>",
            "/endstone_test_legacy vec3 <value: vec3>",
            "/endstone_test_legacy vec3i <value: vec3i>",
            "/endstone_test_legacy vec3f <value: vec3f>",
        ],
    )
    results = {}
    lines = await ctx.log_of(
        lambda: results.__setitem__(
            "ok", ctx.server.command_map.register_command(command)
        )
    )
    assert results["ok"] is True
    assert not any("endstone_test_legacy" in line for line in lines), lines
    for line, args in LEGACY_ARGS:
        command.calls.clear()
        result = ctx.command(f"endstone_test_legacy {line}")
        assert result.success, (line, result.output)
        assert command.calls == [args], (line, command.calls)


@scenario("command.softenum_update", budget=1500)
async def softenum_update(ctx: Context) -> None:
    warps = ctx.plugin.register_soft_enum(WARPS, ["spawn"])
    bot, player = await join(ctx, "CmdSoftUpdate", op=True)
    await ctx.ticks(20)
    first = await available(ctx, bot, player)
    assert soft_enum(first, WARPS) == ["spawn"], first["soft_enums"]
    since = await mark(bot)
    warps.add_values(["home"])
    warps.remove_values(["spawn"])
    warps.set_values(["mine", "farm"])
    found = await wait_for(
        ctx,
        bot,
        "UpdateSoftEnumPacket",
        since,
        lambda f: sum(p["enum_name"] == WARPS for p in f) >= 3,
        "the three soft enum updates",
    )
    updates = [(p["type"], p["values"]) for p in found if p["enum_name"] == WARPS]
    assert updates == [
        ("ADD", ["home"]),
        ("REMOVE", ["spawn"]),
        ("REPLACE", ["mine", "farm"]),
    ], updates
    assert warps.values == ["mine", "farm"], warps.values
    await barrier(ctx, bot)
    resent = await got(bot, "AvailableCommandsPacket", since)
    assert resent == [], len(resent)


@scenario("command.softenum_word_outside")
async def softenum_word_outside(ctx: Context) -> None:
    ctx.plugin.register_soft_enum(WARPS, ["spawn"])
    command = RecordingCommand(
        "endstone_test_warp", [f"/endstone_test_warp <warp: {WARPS}>"]
    )
    assert ctx.server.command_map.register_command(command)
    for line, args in [
        ("endstone_test_warp spawn", ["spawn"]),
        ("endstone_test_warp nowhere", ["nowhere"]),
    ]:
        command.calls.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert command.calls == [args], (line, command.calls)


@scenario("command.softenum_owner_disable", budget=1500)
async def softenum_owner_disable(ctx: Context) -> None:
    manager = ctx.server.plugin_manager
    fixture = manager.get_plugin(FIXTURE)
    assert fixture is not None and fixture.is_enabled, (
        f"the {FIXTURE} plugin is not enabled"
    )
    name = "EndstoneTestFixtureWarps"
    owned = fixture.register_soft_enum(name, ["a"])
    bot, player = await join(ctx, "CmdSoftOwner", op=True)
    await ctx.ticks(20)
    first = await available(ctx, bot, player)
    assert soft_enum(first, name) == ["a"], first["soft_enums"]
    try:
        manager.disable_plugin(fixture)
        await ctx.ticks(2)
        kept = await available(ctx, bot, player)
        assert soft_enum(kept, name) == ["a"], kept["soft_enums"]
        found = ctx.server.command_map.get_soft_enum(name)
        assert found is not None and found.values == ["a"], found
        owned.add_values(["b"])
        await ctx.ticks(2)
        grown = await available(ctx, bot, player)
        assert soft_enum(grown, name) == ["a", "b"], grown["soft_enums"]
    finally:
        manager.enable_plugin(fixture)
    again = fixture.register_soft_enum(name, ["c"])
    assert again.values == ["c"] and owned.values == ["c"], (again.values, owned.values)


@scenario("command.softenum_vanilla_readonly")
async def softenum_vanilla_readonly(ctx: Context) -> None:
    for name in ["ScoreboardObjectives", "TagValues", "PluginNames"]:
        vanilla = ctx.server.command_map.get_soft_enum(name)
        assert vanilla is not None, name
        assert vanilla.name == name
        for mutate in [vanilla.set_values, vanilla.add_values, vanilla.remove_values]:
            try:
                mutate(["qa_readonly"])
            except ValueError:
                pass
            else:
                raise AssertionError(f"{name} accepted {mutate.__name__}")
        assert "qa_readonly" not in vanilla.values, vanilla.values
        try:
            ctx.plugin.register_soft_enum(name)
        except ValueError:
            pass
        else:
            raise AssertionError(f"a plugin registered the server's {name}")


def tree(ctx: Context, node, aliases: list[str] | None = None) -> set[str]:
    return ctx.plugin.register_command(
        node, "An endstone_test tree command.", aliases or []
    )


@scenario("command.tree_typed_args")
async def tree_typed_args(ctx: Context) -> None:
    seen: list[tuple] = []
    inputs: list[str] = []

    def record(context: CommandContext) -> int:
        seen.append(
            tuple(context.get_argument(name) for name in ("word", "text", "rest"))
        )
        inputs.append(context.input)
        return 1

    def word_only(context: CommandContext) -> int:
        seen.append((context.get_argument("word"),))
        inputs.append(context.input)
        return 1

    labels = tree(
        ctx,
        Commands.literal("endstone_test_typed")
        .then(
            Commands.argument("word", StringArgumentType.word())
            .executes(word_only)
            .then(
                Commands.argument("text", StringArgumentType.string()).then(
                    Commands.argument(
                        "rest", StringArgumentType.greedy_string()
                    ).executes(record)
                )
            )
        )
        .build(),
    )
    assert "endstone_test_typed" in labels, labels
    for line, expected in [
        ("endstone_test_typed alpha", ("alpha",)),
        (
            'endstone_test_typed alpha "two words" the rest of it',
            ("alpha", "two words", "the rest of it"),
        ),
    ]:
        seen.clear()
        inputs.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert seen == [expected], (line, seen)
        assert inputs == [line], (line, inputs)
        assert all(type(value) is str for value in seen[0]), seen


@scenario("command.tree_requires", budget=1500)
async def tree_requires(ctx: Context) -> None:
    calls: list[str] = []

    def run(name: str):
        def command(context: CommandContext) -> int:
            calls.append(name)
            return 1

        return command

    tree(
        ctx,
        Commands.literal("endstone_test_gate")
        .then(Commands.literal("open").executes(run("open")))
        .then(
            Commands.literal("secret")
            .requires(lambda source: source.sender.name != "CmdTreeDenied")
            .executes(run("secret"))
        )
        .build(),
    )
    denied_bot, denied = await join(ctx, "CmdTreeDenied", op=True)
    allowed_bot, allowed = await join(ctx, "CmdTreeAllowed", op=True)
    await ctx.ticks(20)
    hidden = view(await available(ctx, denied_bot, denied))["endstone_test_gate"][3]
    shown = view(await available(ctx, allowed_bot, allowed))["endstone_test_gate"][3]
    assert [[p[0] for p in o] for o in hidden] == [["open"]], hidden
    assert sorted([p[0] for p in o] for o in shown) == [["open"], ["secret"]], shown

    await denied_bot.call("local_player.run_command", "/endstone_test_gate secret")
    await allowed_bot.call("local_player.run_command", "/endstone_test_gate secret")
    await ctx.until(lambda: "secret" in calls, what="the allowed player's /secret")
    await barrier(ctx, denied_bot)
    assert calls == ["secret"], calls


@scenario("command.basic_command")
async def basic_command(ctx: Context) -> None:
    class Echo(BasicCommand):
        def __init__(self) -> None:
            super().__init__()
            self.calls: list[list[str]] = []

        def execute(self, source, args: list[str]) -> None:
            if args == ["fail"]:
                raise CommandSyntaxException("endstone_test echo refused")
            self.calls.append(list(args))

    echo = Echo()
    labels = ctx.plugin.register_command(
        "endstone_test_echo", "Echoes its words.", ["endstone_test_say"], echo
    )
    assert {"endstone_test_echo", "endstone_test_say"} <= labels, labels
    for line, args in [
        ("endstone_test_echo", []),
        ("endstone_test_echo a  b c", ["a", "b", "c"]),
        ("endstone_test_say one", ["one"]),
    ]:
        echo.calls.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert echo.calls == [args], (line, echo.calls)
    failed = ctx.command("endstone_test_echo fail")
    assert not failed.success
    assert any("endstone_test echo refused" in e for e in failed.errors), failed.errors


@scenario("command.tree_console")
async def tree_console(ctx: Context) -> None:
    sources = []

    def run(context: CommandContext) -> int:
        sources.append(context.source)
        return 1

    def fail(context: CommandContext) -> int:
        raise CommandSyntaxException("endstone_test refused this")

    raised = []

    def nobody(context: CommandContext) -> int:
        try:
            context.get_argument("who").resolve(context.source)
        except CommandSyntaxException as e:
            raised.append(e)
            raise
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_console")
        .executes(run)
        .then(Commands.literal("fail").executes(fail))
        .then(
            Commands.literal("nobody").then(
                Commands.argument("who", ArgumentTypes.player()).executes(nobody)
            )
        )
        .build(),
    )
    result = ctx.command("endstone_test_console")
    assert result.success, result.output
    (source,) = sources
    assert source.executor is None, source.executor
    assert source.sender.name == ctx.server.command_sender.name, source.sender.name
    failed = ctx.command("endstone_test_console fail")
    assert not failed.success
    assert any("endstone_test refused this" in e for e in failed.errors), failed.errors
    unmatched = ctx.command(
        "endstone_test_console nobody @a[name=endstone_test_nobody]"
    )
    assert not unmatched.success
    assert len(raised) == 1, raised
    assert any("No targets matched selector" in e for e in unmatched.errors), (
        unmatched.errors
    )

    seen = {}

    def probe(context: CommandContext) -> int:
        seen["word"] = context.get_argument("word")
        seen["source"] = context.source
        try:
            context.get_argument("missing")
        except ValueError as e:
            seen["missing"] = str(e)
        return 7

    dispatcher = CommandDispatcher()
    dispatcher.register(
        Commands.literal("t")
        .requires(lambda candidate: candidate.sender.name == source.sender.name)
        .then(
            Commands.argument("word", StringArgumentType.word())
            .executes(probe)
            .then(Commands.literal("fail").executes(fail))
        )
    )
    assert dispatcher.root.children[0].can_use(source)
    assert dispatcher.execute("t hello", source) == 7
    assert seen["word"] == "hello" and type(seen["word"]) is str, seen
    assert seen["source"].sender.name == source.sender.name, seen
    assert "No such argument 'missing'" in seen["missing"], seen
    try:
        dispatcher.execute("t hello fail", source)
    except CommandSyntaxException as e:
        assert "endstone_test refused this" in str(e), e
    else:
        raise AssertionError("the dispatcher did not raise CommandSyntaxException")


@scenario("command.tree_reload", budget=4000)
async def tree_reload(ctx: Context) -> None:
    stash = ctx.stash
    label = "endstone_test_reloaded"
    calls: list[int] = []

    def run(context: CommandContext) -> int:
        calls.append(1)
        return 1

    if not stash.get("reloaded"):
        tree(ctx, Commands.literal(label).executes(run).build())
        assert ctx.command(label).success
        stash["reloaded"] = True
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    assert ctx.server.command_map.get_command(label) is None
    lines = await ctx.log_of(
        lambda: stash.__setitem__(
            "labels", tree(ctx, Commands.literal(label).executes(run).build())
        )
    )
    assert label in stash["labels"], stash["labels"]
    assert not [line for line in lines if "already exists" in line], lines
    result = ctx.command(label)
    assert result.success, result.output
    assert calls == [1], calls


@scenario("command.runcommand_after_reload", budget=4000)
async def runcommand_after_reload(ctx: Context) -> None:
    stash = ctx.stash
    label = "endstone_test_reshaped"
    line = f"{label} 5"
    if not stash.get("reshaped"):
        command = RecordingCommand(label, [f"/{label} <n: int>"])
        assert ctx.server.command_map.register_command(command)
        result = ctx.command(line)
        assert result.success, result.output
        assert command.calls == [["5"]], command.calls
        stash["reshaped"] = True
        await ctx.console("reload")
        await ctx.ticks(1200)
        raise AssertionError("the plugin was not reloaded")
    seen: list[str] = []

    def run(context: CommandContext) -> int:
        seen.append(context.get_argument("word"))
        return 1

    tree(
        ctx,
        Commands.literal(label)
        .then(Commands.argument("word", StringArgumentType.word()).executes(run))
        .build(),
    )
    result = ctx.command(f"{label} five")
    assert result.success, result.output
    assert seen == ["five"], seen
    assert not ctx.command(line).success


@scenario("command.tree_executes_none")
async def tree_executes_none(ctx: Context) -> None:
    sources = []

    def run(context: CommandContext) -> None:
        sources.append(context.source)

    tree(ctx, Commands.literal("endstone_test_none").executes(run).build())
    lines = await ctx.log_of(lambda: sources.append(ctx.command("endstone_test_none")))
    source, result = sources
    assert result.success, result.output
    assert not [line for line in lines if "Unhandled exception" in line], lines
    dispatcher = CommandDispatcher()
    dispatcher.register(Commands.literal("n").executes(run))
    assert dispatcher.execute("n", source) == 1


@scenario("command.selector_args", budget=1500)
async def selector_args(ctx: Context) -> None:
    seen: dict[str, list] = {}

    def resolve(name: str):
        def command(context: CommandContext) -> int:
            resolver = context.get_argument("targets")
            seen[name] = list(resolver.resolve(context.source))
            seen[name + ".type"] = type(resolver)
            return 1

        return command

    def single(context: CommandContext) -> int:
        seen["single"] = list(context.get_argument("target").resolve(context.source))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_select")
        .then(
            Commands.literal("players").then(
                Commands.argument("targets", ArgumentTypes.players()).executes(
                    resolve("players")
                )
            )
        )
        .then(
            Commands.literal("actors").then(
                Commands.argument("targets", ArgumentTypes.actors()).executes(
                    resolve("actors")
                )
            )
        )
        .then(
            Commands.literal("one").then(
                Commands.argument("target", ArgumentTypes.player()).executes(single)
            )
        )
        .build(),
    )
    _, near = await join(ctx, "CmdSelNear", op=True)
    _, far = await join(ctx, "CmdSelFar", op=True)
    await ctx.until(lambda: far.dimension is not None, what="CmdSelFar spawned")
    location = far.location
    location.x += 50
    far.teleport(location)
    await ctx.ticks(10)

    assert ctx.server.dispatch_command(near, "endstone_test_select players @a")
    names = {p.name for p in seen["players"]}
    assert {"CmdSelNear", "CmdSelFar"} <= names, names
    assert seen["players.type"] is PlayerSelectorArgumentResolver
    assert all(isinstance(p, Player) for p in seen["players"]), seen["players"]

    assert ctx.server.dispatch_command(near, "endstone_test_select players @p[r=5]")
    assert [p.name for p in seen["players"]] == ["CmdSelNear"], seen["players"]

    assert ctx.server.dispatch_command(near, "endstone_test_select players CmdSelFar")
    assert [p.name for p in seen["players"]] == ["CmdSelFar"], seen["players"]

    assert ctx.server.dispatch_command(near, "endstone_test_select actors @s")
    assert seen["actors.type"] is ActorSelectorArgumentResolver
    assert [a.name for a in seen["actors"]] == ["CmdSelNear"], seen["actors"]

    assert ctx.server.dispatch_command(near, "endstone_test_select one CmdSelNear")
    assert [p.name for p in seen["single"]] == ["CmdSelNear"], seen["single"]

    seen.pop("single", None)
    many = ctx.command("endstone_test_select one @a")
    assert not many.success
    assert "single" not in seen
    assert any("Too many targets matched selector" in e for e in many.errors), (
        many.errors
    )
    none = ctx.command("endstone_test_select one @a[name=EndstoneTestNobody]")
    assert not none.success
    assert any("No targets matched selector" in e for e in none.errors), none.errors


@scenario("command.execute_as_executor", budget=1500)
async def execute_as_executor(ctx: Context) -> None:
    seen = {}

    def run(context: CommandContext) -> int:
        seen["executor"] = context.source.executor
        seen["sender"] = context.source.sender
        seen["self"] = list(context.get_argument("who").resolve(context.source))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_whoami")
        .then(Commands.argument("who", ArgumentTypes.actors()).executes(run))
        .build(),
    )
    _, player = await join(ctx, "CmdExecAs", op=True)
    await ctx.console("execute as CmdExecAs run endstone_test_whoami @s")
    await ctx.until(lambda: "self" in seen, what="the command under /execute as")
    assert seen["executor"] is not None and seen["executor"].name == "CmdExecAs", seen
    assert [a.name for a in seen["self"]] == ["CmdExecAs"], seen

    seen.clear()
    result = ctx.command("endstone_test_whoami @s")
    assert result.success, result.output
    assert seen["executor"] is None, seen
    assert seen["self"] == [], seen


@scenario("command.position_relative", budget=1500)
async def position_relative(ctx: Context) -> None:
    seen = {}

    def fine(context: CommandContext) -> int:
        seen["fine"] = context.get_argument("pos").resolve(context.source)
        seen["fine.type"] = type(context.get_argument("pos"))
        return 1

    def block(context: CommandContext) -> int:
        seen["block"] = context.get_argument("pos").resolve(context.source)
        seen["block.type"] = type(context.get_argument("pos"))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_pos")
        .then(
            Commands.literal("fine").then(
                Commands.argument("pos", ArgumentTypes.fine_position()).executes(fine)
            )
        )
        .then(
            Commands.literal("block").then(
                Commands.argument("pos", ArgumentTypes.block_position()).executes(block)
            )
        )
        .build(),
    )
    _, player = await join(ctx, "CmdPosRel", op=True)
    await ctx.ticks(20)
    where = player.location
    assert ctx.server.dispatch_command(player, "endstone_test_pos fine ~ ~1 ~")
    fine_pos = seen["fine"]
    assert seen["fine.type"] is FinePositionResolver
    assert abs(fine_pos.x - where.x) < 0.01, (fine_pos, where)
    assert abs(fine_pos.y - (where.y + 1)) < 0.01, (fine_pos, where)
    assert abs(fine_pos.z - where.z) < 0.01, (fine_pos, where)

    assert ctx.server.dispatch_command(player, "endstone_test_pos block ~ ~1 ~")
    block_pos = seen["block"]
    assert seen["block.type"] is BlockPositionResolver
    assert (block_pos.x, block_pos.y, block_pos.z) == (
        where.block_x,
        where.block_y + 1,
        where.block_z,
    ), (block_pos, where)

    assert ctx.server.dispatch_command(player, "endstone_test_pos block 1 2 3")
    assert (seen["block"].x, seen["block"].y, seen["block"].z) == (1, 2, 3)


@scenario("command.dimension_arg")
async def dimension_arg(ctx: Context) -> None:
    seen: list = []

    def run(context: CommandContext) -> int:
        seen.append(context.get_argument("dim"))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_dim")
        .then(Commands.argument("dim", ArgumentTypes.dimension()).executes(run))
        .build(),
    )
    for text, expected in [
        ("overworld", "minecraft:overworld"),
        ("the_end", "minecraft:the_end"),
        (CUSTOM_REALM, CUSTOM_REALM),
    ]:
        seen.clear()
        result = ctx.command(f"endstone_test_dim {text}")
        assert result.success, (text, result.output)
        (dimension,) = seen
        assert isinstance(dimension, Dimension), (text, dimension)
        assert str(dimension.id) == expected, (text, dimension.id)
    seen.clear()
    unknown = ctx.command("endstone_test_dim endstone_test:no_such_realm")
    assert not unknown.success
    assert any(
        'Syntax error: Unexpected "endstone_test:no_such_realm"' in e
        for e in unknown.errors
    ), unknown.errors
    assert seen == [], seen


@scenario("command.dimension_arg_hidden")
async def dimension_arg_hidden(ctx: Context) -> None:
    seen: list = []

    def run(context: CommandContext) -> int:
        seen.append(context.get_argument("dim"))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_dimhide")
        .then(
            Commands.argument("dim", ArgumentTypes.dimension())
            .requires(lambda source: False)
            .executes(run)
        )
        .build(),
    )
    for text in ["overworld", CUSTOM_REALM]:
        result = ctx.command(f"endstone_test_dimhide {text}")
        assert not result.success, (text, result.output)
        assert any("Unknown command" in e for e in result.errors), (text, result.errors)
        assert not any("dimension" in e for e in result.errors), (text, result.errors)
    unknown = ctx.command("endstone_test_dimhide endstone_test:no_such_realm")
    assert not unknown.success
    assert any(
        'Syntax error: Unexpected "endstone_test:no_such_realm"' in e
        for e in unknown.errors
    ), unknown.errors
    assert seen == [], seen


@scenario("command.resource_arg")
async def resource_arg(ctx: Context) -> None:
    seen: list = []

    def run(context: CommandContext) -> int:
        seen.append(context.get_argument("value"))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_res")
        .then(
            Commands.literal("block").then(
                Commands.argument("value", ArgumentTypes.resource(BlockType)).executes(
                    run
                )
            )
        )
        .then(
            Commands.literal("item").then(
                Commands.argument("value", ArgumentTypes.resource(ItemType)).executes(
                    run
                )
            )
        )
        .then(
            Commands.literal("actor").then(
                Commands.argument("value", ArgumentTypes.resource(ActorType)).executes(
                    run
                )
            )
        )
        .build(),
    )
    for line, cls, expected in [
        ("block stone", BlockType, "minecraft:stone"),
        ("block minecraft:oak_planks", BlockType, "minecraft:oak_planks"),
        ("item apple", ItemType, "minecraft:apple"),
        ("actor zombie", ActorType, "minecraft:zombie"),
    ]:
        seen.clear()
        result = ctx.command(f"endstone_test_res {line}")
        assert result.success, (line, result.output)
        (value,) = seen
        assert isinstance(value, cls), (line, value)
        assert value == expected, (line, value)
    seen.clear()
    unknown = ctx.command("endstone_test_res block endstone_test_no_block")
    assert not unknown.success
    assert any("endstone_test_no_block" in e for e in unknown.errors), unknown.errors
    assert seen == [], seen


def make_numbers(seen: list) -> object:
    def record(name: str):
        def command(context: CommandContext) -> int:
            seen.append(context.get_argument(name))
            return 1

        return command

    return (
        Commands.literal("endstone_test_numbers")
        .then(
            Commands.literal("int").then(
                Commands.argument("n", IntegerArgumentType.integer(1, 64)).executes(
                    record("n")
                )
            )
        )
        .then(
            Commands.literal("float").then(
                Commands.argument("x", FloatArgumentType.float_arg(0, 1)).executes(
                    record("x")
                )
            )
        )
        .then(
            Commands.literal("double").then(
                Commands.argument(
                    "d", DoubleArgumentType.double_arg(-2.5, 2.5)
                ).executes(record("d"))
            )
        )
        .then(
            Commands.literal("bool").then(
                Commands.argument("b", BoolArgumentType.bool()).executes(record("b"))
            )
        )
        .build()
    )


@scenario("command.tree_primitives")
async def tree_primitives(ctx: Context) -> None:
    seen: list = []
    tree(ctx, make_numbers(seen))
    for line, expected, kind in [
        ("endstone_test_numbers int 64", 64, int),
        ("endstone_test_numbers float 0.5", 0.5, float),
        ("endstone_test_numbers double -2.25", -2.25, float),
        ("endstone_test_numbers bool true", True, bool),
        ("endstone_test_numbers bool false", False, bool),
    ]:
        seen.clear()
        result = ctx.command(line)
        assert result.success, (line, result.output)
        assert seen == [expected], (line, seen)
        assert type(seen[0]) is kind, (line, seen)
    for line, message in [
        ("endstone_test_numbers int 65", "Integer must not be more than 64, found 65"),
        ("endstone_test_numbers int 0", "Integer must not be less than 1, found 0"),
        (
            "endstone_test_numbers float -0.5",
            "Float must not be less than 0.0, found -0.5",
        ),
        (
            "endstone_test_numbers double 3",
            "Double must not be more than 2.5, found 3.0",
        ),
    ]:
        seen.clear()
        result = ctx.command(line)
        assert not result.success, (line, result.output)
        assert message in result.errors, (line, result.errors)
        assert seen == [], (line, seen)


@scenario("command.tree_range_error_player", budget=1500)
async def tree_range_error_player(ctx: Context) -> None:
    seen: list = []
    tree(ctx, make_numbers(seen))
    bot, _ = await join(ctx, "CmdTreeRange", op=True)
    await ctx.ticks(20)
    message = "Integer must not be more than 64, found 100"
    since = await mark(bot)
    await bot.call("local_player.run_command", "/endstone_test_numbers int 100")
    await wait_for(
        ctx,
        bot,
        "TextPacket",
        since,
        lambda found: any(message in json.dumps(p) for p in found),
        what="the range error",
    )
    assert seen == [], seen


class BlockRunRecorder(CommandExecutor):
    def __init__(self) -> None:
        CommandExecutor.__init__(self)
        self.args: list[list[str]] = []

    def on_command(self, sender, command, args) -> bool:
        if isinstance(sender, BlockCommandSender):
            self.args.append(list(args))
        return True


@scenario("command.legacy_command_block_after_reload", plot=True, budget=4000)
async def legacy_command_block_after_reload(ctx: Context) -> None:
    stash = ctx.stash
    if not stash.get("block"):
        x, y, z = ctx.plot.pos(SIZE + 8, 0, SIZE // 2)
        stash["block"] = (x, y, z)
    x, y, z = stash["block"]
    power = f"{x + 1} {y} {z}"
    recorder = BlockRunRecorder()
    command = ctx.plugin.get_command("testoptmsg")
    command.executor = recorder
    after_reload = bool(stash.get("reloaded"))
    try:
        if not after_reload:
            ctx.run(f"structure load endstone_test:command_block_redstone {x} {y} {z}")
            ctx.run(f"setblock {power} redstone_block")
            await ctx.until(
                lambda: recorder.args, timeout=100, what="the command block's first run"
            )
            assert recorder.args == [["covblock"]], recorder.args
            ctx.run(f"setblock {power} air")
            await ctx.ticks(2)
            stash["reloaded"] = True
            await ctx.console("reload")
            await ctx.ticks(1200)
            raise AssertionError("the plugin was not reloaded")
        ctx.run(f"setblock {power} redstone_block")
        await ctx.until(
            lambda: recorder.args,
            timeout=100,
            what="the same command block's run after /reload",
        )
        assert recorder.args == [["covblock"]], recorder.args
    finally:
        command.executor = None
        if after_reload:
            ctx.command(f"setblock {power} air")
            ctx.command(f"setblock {x} {y} {z} air")


@scenario("command.tree_command_block_after_reload", plot=True, budget=4000)
async def tree_command_block_after_reload(ctx: Context) -> None:
    stash = ctx.stash
    if not stash.get("block"):
        x, y, z = ctx.plot.pos(SIZE + 8, 0, SIZE // 2)
        stash["block"] = (x, y, z)
    x, y, z = stash["block"]
    power = f"{x + 1} {y} {z}"
    seen: list[str] = []

    def run(context: CommandContext) -> int:
        if isinstance(context.source.sender, BlockCommandSender):
            seen.append(context.get_argument("word"))
        return 1

    tree(
        ctx,
        Commands.literal("endstone_test_blocktree")
        .then(Commands.argument("word", StringArgumentType.word()).executes(run))
        .build(),
    )
    after_reload = bool(stash.get("reloaded"))
    try:
        if not after_reload:
            ctx.run(f"structure load endstone_test:command_block_tree {x} {y} {z}")
            ctx.run(f"setblock {power} redstone_block")
            await ctx.until(
                lambda: seen, timeout=100, what="the command block's first run"
            )
            assert seen == ["covblock"], seen
            ctx.run(f"setblock {power} air")
            await ctx.ticks(2)
            stash["reloaded"] = True
            await ctx.console("reload")
            await ctx.ticks(1200)
            raise AssertionError("the plugin was not reloaded")
        ctx.run(f"setblock {power} redstone_block")
        await ctx.until(
            lambda: seen,
            timeout=100,
            what="the same command block's run after /reload",
        )
        assert seen == ["covblock"], seen
    finally:
        if after_reload:
            ctx.command(f"setblock {power} air")
            ctx.command(f"setblock {x} {y} {z} air")
