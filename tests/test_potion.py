import importlib

import pytest

from endstone import Identifier
from endstone.potion import Effect, EffectType, PotionType

# =============================================================================
# Section 1: Effect construction
# =============================================================================


def test_effect_properties() -> None:
    """Verify an effect keeps every value it was built with."""
    effect = Effect(EffectType.SPEED, 200, 1, True, False, False)
    assert effect.type == "minecraft:speed"
    assert effect.duration == 200
    assert effect.amplifier == 1
    assert effect.ambient is True
    assert effect.particles is False
    assert effect.icon is False


def test_effect_display_defaults() -> None:
    """Verify the display flags default to a visible, non-ambient effect."""
    effect = Effect(EffectType.POISON, 100, 0)
    assert effect.ambient is False
    assert effect.particles is True
    assert effect.icon is True


def test_effect_is_infinite_with_an_infinite_duration() -> None:
    """Verify a duration of INFINITE_DURATION means the effect never expires."""
    assert Effect.INFINITE_DURATION == -1
    effect = Effect(EffectType.NIGHT_VISION, Effect.INFINITE_DURATION, 0)
    assert effect.duration == Effect.INFINITE_DURATION
    assert effect.infinite is True
    assert Effect(EffectType.NIGHT_VISION, 100, 0).infinite is False


def test_effect_is_read_only() -> None:
    """Verify an effect is immutable."""
    effect = Effect(EffectType.SPEED, 200, 1)
    with pytest.raises(AttributeError):
        effect.duration = 400


def test_effect_is_unhashable() -> None:
    """Verify an effect cannot be used as a dict key or set member."""
    assert Effect.__hash__ is None
    with pytest.raises(TypeError):
        {Effect(EffectType.SPEED, 200, 1)}


# =============================================================================
# Section 2: Effect owns its type
# =============================================================================


@pytest.mark.parametrize(
    "effect_type",
    [EffectType.SPEED, "minecraft:speed", Identifier("minecraft", "speed"), "speed"],
    ids=["constant", "qualified_str", "identifier", "bare_str"],
)
def test_effect_type_round_trips(effect_type) -> None:
    """Verify an effect reports its type back whatever form it was given."""
    effect = Effect(effect_type, 200, 1)
    assert effect.type == "minecraft:speed"
    assert effect.type.namespace == "minecraft"
    assert effect.type.key == "speed"


def test_effect_type_survives_the_source_string() -> None:
    """Verify an effect owns its type rather than viewing the caller's string."""
    name = "minecraft:regeneration"
    effect = Effect(name, 100, 0)
    del name
    assert str(effect.type) == "minecraft:regeneration"


def test_effect_repr() -> None:
    """Verify repr() renders the type, duration and amplifier."""
    assert repr(Effect(EffectType.SPEED, 200, 1)) == "Effect(type='minecraft:speed', duration=200, amplifier=1)"


def test_effect_equality() -> None:
    """Verify equality compares every field and normalises the namespace."""
    assert Effect("speed", 200, 1) == Effect(EffectType.SPEED, 200, 1)
    assert Effect(EffectType.SPEED, 200, 1) != Effect(EffectType.SLOWNESS, 200, 1)
    assert Effect(EffectType.SPEED, 200, 1) != Effect(EffectType.SPEED, 201, 1)
    assert Effect(EffectType.SPEED, 200, 1) != Effect(EffectType.SPEED, 200, 2)


def test_effect_accepts_an_empty_type_but_cannot_read_it_back() -> None:
    """Verify an empty type is only rejected when the identifier is read back."""
    effect = Effect("", 200, 1)
    assert effect.duration == 200
    with pytest.raises(ValueError) as err_info:
        _ = effect.type
    assert str(err_info.value) == "Identifier namespace and key must not be empty."


# =============================================================================
# Section 3: Type constants
# =============================================================================


def test_effect_type_constants() -> None:
    """Verify every effect type constant is an identifier in the minecraft namespace."""
    names = [name for name in dir(EffectType) if name.isupper()]
    assert len(names) == 37
    for name in names:
        constant = getattr(EffectType, name)
        assert isinstance(constant, Identifier)
        assert constant.namespace == "minecraft"
        assert constant.key == {"HERO_OF_THE_VILLAGE": "village_hero"}.get(name, name.lower())


def test_potion_type_constants() -> None:
    """Verify every potion type constant lives under the potion_type namespace."""
    names = [name for name in dir(PotionType) if name.isupper()]
    assert len(names) == 47
    for name in names:
        constant = getattr(PotionType, name)
        assert isinstance(constant, Identifier)
        assert constant.namespace == "minecraft:potion_type"


def test_effect_module_was_removed() -> None:
    """Verify EffectType moved to endstone.potion and endstone.effect is gone."""
    with pytest.raises(ModuleNotFoundError):
        importlib.import_module("endstone.effect")
