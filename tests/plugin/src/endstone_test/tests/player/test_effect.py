import pytest
from endstone import Player
from endstone.potion import Effect, EffectType

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture
def mob(player: Player) -> Player:
    """Clear every effect before and after each test."""
    server = player.server
    server.dispatch_command(server.command_sender, f"effect {player.name} clear")
    assert player.active_effects == []

    yield player

    server.dispatch_command(server.command_sender, f"effect {player.name} clear")
    assert player.active_effects == []


# =============================================================================
# Section 1: Applying and removing
# =============================================================================


def test_add_effect(mob: Player) -> None:
    """Verify an effect can be applied and is then reported as active."""
    assert mob.has_effect(EffectType.SPEED) is False

    mob.add_effect(Effect(EffectType.SPEED, 200, 1))
    assert mob.has_effect(EffectType.SPEED) is True


def test_get_effect_round_trips(mob: Player) -> None:
    """Verify an applied effect is read back with the values it was given."""
    mob.add_effect(Effect(EffectType.REGENERATION, 300, 2, True, False, False))

    effect = mob.get_effect(EffectType.REGENERATION)
    assert effect is not None
    assert effect.type == "minecraft:regeneration"
    assert effect.amplifier == 2
    assert effect.ambient is True
    assert effect.particles is False
    assert effect.icon is False
    assert 0 < effect.duration <= 300


def test_get_effect_for_an_inactive_type(mob: Player) -> None:
    """Verify an effect that is not applied reads back as None."""
    assert mob.get_effect(EffectType.WITHER) is None


def test_remove_effect(mob: Player) -> None:
    """Verify an applied effect can be taken away again."""
    mob.add_effect(Effect(EffectType.POISON, 200, 0))
    assert mob.has_effect(EffectType.POISON) is True

    mob.remove_effect(EffectType.POISON)
    assert mob.has_effect(EffectType.POISON) is False
    assert mob.get_effect(EffectType.POISON) is None


def test_active_effects(mob: Player) -> None:
    """Verify every applied effect appears in active_effects."""
    mob.add_effect(Effect(EffectType.SPEED, 200, 0))
    mob.add_effect(Effect(EffectType.HASTE, 200, 0))

    types = {str(effect.type) for effect in mob.active_effects}
    assert types == {"minecraft:speed", "minecraft:haste"}


def test_infinite_effect(mob: Player) -> None:
    """Verify an effect applied with an infinite duration never expires."""
    mob.add_effect(Effect(EffectType.NIGHT_VISION, Effect.INFINITE_DURATION, 0))

    effect = mob.get_effect(EffectType.NIGHT_VISION)
    assert effect is not None
    assert effect.infinite is True
    assert effect.duration == Effect.INFINITE_DURATION


def test_reapplying_replaces_the_effect(mob: Player) -> None:
    """Verify applying the same type again updates it rather than stacking."""
    mob.add_effect(Effect(EffectType.SPEED, 200, 0))
    mob.add_effect(Effect(EffectType.SPEED, 400, 3))

    assert len([e for e in mob.active_effects if e.type == "minecraft:speed"]) == 1
    assert mob.get_effect(EffectType.SPEED).amplifier == 3


# =============================================================================
# Section 2: Accepting identifiers and strings
# =============================================================================


@pytest.mark.parametrize(
    "effect_type",
    [EffectType.SPEED, "minecraft:speed", "speed"],
    ids=["constant", "qualified_str", "bare_str"],
)
def test_effect_type_forms(mob: Player, effect_type) -> None:
    """Verify a constant, a qualified id and a bare key all name the same effect."""
    mob.add_effect(Effect(effect_type, 200, 0))
    assert mob.has_effect(effect_type) is True
    assert mob.has_effect(EffectType.SPEED) is True


# =============================================================================
# Section 3: Unknown effect types
# =============================================================================


def test_add_an_unknown_effect(mob: Player) -> None:
    """Verify applying an unknown effect type is refused."""
    with pytest.raises(ValueError):
        mob.add_effect(Effect("minecraft:not_an_effect", 200, 0))


def test_remove_an_unknown_effect(mob: Player) -> None:
    """Verify removing an unknown effect type is refused."""
    with pytest.raises(ValueError):
        mob.remove_effect("minecraft:not_an_effect")


def test_has_an_unknown_effect(mob: Player) -> None:
    """Verify asking about an unknown effect type reports absence."""
    assert mob.has_effect("minecraft:not_an_effect") is False
