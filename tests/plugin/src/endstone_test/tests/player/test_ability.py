import pytest
from endstone import Ability, Player, Server

BOOL_ABILITY = Ability.NO_CLIP
FLOAT_ABILITY = Ability.FLY_SPEED

# The eight the client shows in its pause menu.
MEMBER_PERMISSIONS = [
    Ability.BUILD,
    Ability.MINE,
    Ability.DOORS_AND_SWITCHES,
    Ability.OPEN_CONTAINERS,
    Ability.ATTACK_PLAYERS,
    Ability.ATTACK_MOBS,
    Ability.OPERATOR_COMMANDS,
    Ability.TELEPORT,
]


@pytest.fixture(scope="function", autouse=True)
def restore_abilities(player: Player):
    touched = [BOOL_ABILITY, FLOAT_ABILITY, Ability.VERTICAL_FLY_SPEED, *MEMBER_PERMISSIONS]
    saved = {ability: player.get_ability(ability) for ability in touched}

    yield

    for ability, value in saved.items():
        player.set_ability(ability, value)


# =============================================================================
# Presence and value types
# =============================================================================


def test_value_types(player: Player) -> None:
    """A flag reads as bool, a speed as float."""
    assert isinstance(player.get_ability(BOOL_ABILITY), bool)
    value = player.get_ability(FLOAT_ABILITY)
    assert isinstance(value, float) and not isinstance(value, bool)


def test_every_registered_ability_is_readable(server: Server, player: Player) -> None:
    """Reading any ability in the registry yields a value of the right kind."""
    registry = server.get_registry(Ability)
    assert len(registry) == 20

    for ability in registry:
        assert isinstance(player.get_ability(ability.id), (bool, float))


# =============================================================================
# Round trips
# =============================================================================


def test_bool_round_trip(player: Player) -> None:
    """Writing a flag twice through the API lands both times."""
    original = player.get_ability(BOOL_ABILITY)

    player.set_ability(BOOL_ABILITY, not original)
    assert player.get_ability(BOOL_ABILITY) is (not original)

    player.set_ability(BOOL_ABILITY, original)
    assert player.get_ability(BOOL_ABILITY) is original


def test_float_round_trip(player: Player) -> None:
    """A speed survives a write and read back."""
    player.set_ability(FLOAT_ABILITY, 0.15)
    assert player.get_ability(FLOAT_ABILITY) == pytest.approx(0.15)

    player.set_ability(Ability.VERTICAL_FLY_SPEED, 2.0)
    assert player.get_ability(Ability.VERTICAL_FLY_SPEED) == pytest.approx(2.0)


@pytest.mark.parametrize("ability", MEMBER_PERMISSIONS, ids=lambda a: a.key)
def test_member_permission_round_trip(player: Player, ability) -> None:
    """Each pause-menu member permission can be flipped and read back."""
    original = player.get_ability(ability)

    player.set_ability(ability, not original)
    assert player.get_ability(ability) is (not original)

    player.set_ability(ability, original)
    assert player.get_ability(ability) is original


def test_fly_speed_matches_legacy_accessor(player: Player) -> None:
    """The keyed API and the dedicated fly_speed property address the same ability."""
    player.set_ability(FLOAT_ABILITY, 0.2)
    assert player.fly_speed == pytest.approx(0.2)

    player.fly_speed = 0.05
    assert player.get_ability(FLOAT_ABILITY) == pytest.approx(0.05)


def test_set_does_not_disturb_other_abilities(server: Server, player: Player) -> None:
    """Setting one ability leaves every other ability alone."""
    names = [str(ability.id) for ability in server.get_registry(Ability)]
    before = {name: player.get_ability(name) for name in names}

    target = str(BOOL_ABILITY)
    player.set_ability(BOOL_ABILITY, not before[target])

    after = {name: player.get_ability(name) for name in names}
    changed = {name for name in names if after[name] != before[name]}
    assert changed == {target}


# =============================================================================
# Failure modes
# =============================================================================


def test_get_unknown_ability_raises(player: Player) -> None:
    """Reading an ability that does not exist raises rather than returning a default."""
    with pytest.raises(IndexError):
        player.get_ability("minecraft:not_an_ability")


def test_set_unknown_ability_raises(player: Player) -> None:
    """Writing an ability that does not exist raises."""
    with pytest.raises(ValueError):
        player.set_ability("minecraft:not_an_ability", True)


def test_set_wrong_value_type_raises(player: Player) -> None:
    """A value whose type does not match the ability is rejected."""
    with pytest.raises(ValueError):
        player.set_ability(FLOAT_ABILITY, True)
    with pytest.raises(ValueError):
        player.set_ability(BOOL_ABILITY, 0.5)
