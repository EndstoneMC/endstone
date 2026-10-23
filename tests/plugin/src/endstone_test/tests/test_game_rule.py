import pytest
from endstone import GameRule, Server
from endstone.command import CommandSenderWrapper
from endstone.level import Level

BOOL_RULE = GameRule.DO_FIRE_TICK
INT_RULE = GameRule.SPAWN_RADIUS


@pytest.fixture
def level(server: Server) -> Level:
    return server.level


@pytest.fixture(scope="function", autouse=True)
def restore_game_rules(level: Level):
    touched = [BOOL_RULE, INT_RULE, GameRule.SEND_COMMAND_FEEDBACK]
    saved = {rule: level.get_game_rule_value(rule) for rule in touched}

    level.set_game_rule(GameRule.SEND_COMMAND_FEEDBACK, True)

    yield

    for rule, value in saved.items():
        level.set_game_rule(rule, value)


def run_command(server: Server, command_line: str) -> list[str]:
    """Dispatch a command from the console and return everything it sent back."""
    output: list[str] = []
    sender = CommandSenderWrapper(
        server.command_sender,
        on_message=lambda message: output.append(str(message)),
        on_error=lambda message: output.append(str(message)),
    )
    assert server.dispatch_command(sender, command_line), f"{command_line!r} failed"
    return output


# =============================================================================
# Presence and value types
# =============================================================================


def test_is_game_rule(level: Level) -> None:
    """A known rule is present, an unknown one is not."""
    assert level.is_game_rule(BOOL_RULE)
    assert level.is_game_rule(INT_RULE)
    assert not level.is_game_rule("minecraft:not_a_game_rule")


def test_value_types(level: Level) -> None:
    """A bool rule reads as bool, an int rule as int."""
    assert isinstance(level.get_game_rule_value(BOOL_RULE), bool)
    value = level.get_game_rule_value(INT_RULE)
    assert isinstance(value, int) and not isinstance(value, bool)


def test_every_registered_rule_is_readable(server: Server, level: Level) -> None:
    """Reading any rule in the registry yields a value instead of killing the server.

    Regression test for the `Type::Invalid` path, which used to call `std::abort()`.
    """
    registry = server.get_registry(GameRule)
    assert len(registry) > 0

    for rule in registry:
        assert level.is_game_rule(rule.id)
        assert isinstance(level.get_game_rule_value(str(rule.id)), (bool, int, float))


# =============================================================================
# Command writes, API reads
# =============================================================================


@pytest.mark.parametrize("value", [False, True])
def test_command_set_is_visible_to_api_bool(
    server: Server, level: Level, value: bool
) -> None:
    """`/gamerule` on a bool rule is observed by `get_game_rule_value`."""
    run_command(server, f"gamerule dofiretick {str(value).lower()}")
    assert level.get_game_rule_value(BOOL_RULE) is value


@pytest.mark.parametrize("value", [3, 7])
def test_command_set_is_visible_to_api_int(
    server: Server, level: Level, value: int
) -> None:
    """`/gamerule` on an int rule is observed by `get_game_rule_value`."""
    run_command(server, f"gamerule spawnradius {value}")
    assert level.get_game_rule_value(INT_RULE) == value


# =============================================================================
# API writes, command reads
# =============================================================================


@pytest.mark.parametrize("value", [False, True])
def test_api_set_is_visible_to_command_bool(
    server: Server, level: Level, value: bool
) -> None:
    """`set_game_rule` on a bool rule is observed by a `/gamerule` query."""
    level.set_game_rule(BOOL_RULE, value)
    assert level.get_game_rule_value(BOOL_RULE) is value

    output = " ".join(run_command(server, "gamerule dofiretick")).lower()
    assert str(value).lower() in output, f"query returned {output!r}"


@pytest.mark.parametrize("value", [3, 7])
def test_api_set_is_visible_to_command_int(
    server: Server, level: Level, value: int
) -> None:
    """`set_game_rule` on an int rule is observed by a `/gamerule` query."""
    level.set_game_rule(INT_RULE, value)
    assert level.get_game_rule_value(INT_RULE) == value

    output = " ".join(run_command(server, "gamerule spawnradius"))
    assert str(value) in output, f"query returned {output!r}"


# =============================================================================
# Round trips and isolation
# =============================================================================


def test_api_round_trip_survives_a_flip(level: Level) -> None:
    """Writing a rule twice through the API lands both times."""
    original = level.get_game_rule_value(BOOL_RULE)

    level.set_game_rule(BOOL_RULE, not original)
    assert level.get_game_rule_value(BOOL_RULE) is (not original)

    level.set_game_rule(BOOL_RULE, original)
    assert level.get_game_rule_value(BOOL_RULE) is original


def test_set_does_not_disturb_other_rules(server: Server, level: Level) -> None:
    """Setting one rule leaves every other rule alone.

    `_setGameRule` derives the native rule index by pointer arithmetic into the
    level's rule array, so a bad index would silently write to a neighbour.
    """
    names = [str(rule.id) for rule in server.get_registry(GameRule)]
    before = {name: level.get_game_rule_value(name) for name in names}

    target = str(BOOL_RULE)
    level.set_game_rule(BOOL_RULE, not before[target])

    after = {name: level.get_game_rule_value(name) for name in names}
    changed = {name for name in names if after[name] != before[name]}
    assert changed == {target}


# =============================================================================
# Failure modes
# =============================================================================


def test_get_unknown_rule_raises(level: Level) -> None:
    """Reading a rule that does not exist raises rather than returning a default."""
    with pytest.raises(ValueError):
        level.get_game_rule_value("minecraft:not_a_game_rule")


def test_set_unknown_rule_returns_false(level: Level) -> None:
    """Writing a rule that does not exist is reported as not set."""
    assert level.set_game_rule("minecraft:not_a_game_rule", True) is False


def test_set_wrong_value_type_raises(level: Level) -> None:
    """A value whose type does not match the rule is rejected."""
    with pytest.raises(ValueError):
        level.set_game_rule(INT_RULE, True)
