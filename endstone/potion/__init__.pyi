"""
Classes relating to potion effects.
"""

from endstone import Identifier

__all__ = [
    "Effect",
    "EffectType",
    "PotionType",
]

class EffectType:
    """
    Represents an effect type.
    """
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def id(self) -> Identifier[EffectType]:
        """
        The identifier of this effect type.
        """

    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """

    @staticmethod
    def get(name: Identifier[EffectType] | str) -> EffectType:
        """
        Attempts to get the `EffectType` with the given name.

        Args:
            name: The identifier of the effect type (e.g. `minecraft:speed`).

        Returns:
            The `EffectType`, or `None` if no effect type with that name exists.
        """

    SPEED: Identifier[EffectType] = "minecraft:speed"
    SLOWNESS: Identifier[EffectType] = "minecraft:slowness"
    HASTE: Identifier[EffectType] = "minecraft:haste"
    MINING_FATIGUE: Identifier[EffectType] = "minecraft:mining_fatigue"
    STRENGTH: Identifier[EffectType] = "minecraft:strength"
    INSTANT_HEALTH: Identifier[EffectType] = "minecraft:instant_health"
    INSTANT_DAMAGE: Identifier[EffectType] = "minecraft:instant_damage"
    JUMP_BOOST: Identifier[EffectType] = "minecraft:jump_boost"
    NAUSEA: Identifier[EffectType] = "minecraft:nausea"
    REGENERATION: Identifier[EffectType] = "minecraft:regeneration"
    RESISTANCE: Identifier[EffectType] = "minecraft:resistance"
    FIRE_RESISTANCE: Identifier[EffectType] = "minecraft:fire_resistance"
    WATER_BREATHING: Identifier[EffectType] = "minecraft:water_breathing"
    INVISIBILITY: Identifier[EffectType] = "minecraft:invisibility"
    BLINDNESS: Identifier[EffectType] = "minecraft:blindness"
    NIGHT_VISION: Identifier[EffectType] = "minecraft:night_vision"
    HUNGER: Identifier[EffectType] = "minecraft:hunger"
    WEAKNESS: Identifier[EffectType] = "minecraft:weakness"
    POISON: Identifier[EffectType] = "minecraft:poison"
    WITHER: Identifier[EffectType] = "minecraft:wither"
    HEALTH_BOOST: Identifier[EffectType] = "minecraft:health_boost"
    ABSORPTION: Identifier[EffectType] = "minecraft:absorption"
    SATURATION: Identifier[EffectType] = "minecraft:saturation"
    LEVITATION: Identifier[EffectType] = "minecraft:levitation"
    FATAL_POISON: Identifier[EffectType] = "minecraft:fatal_poison"
    CONDUIT_POWER: Identifier[EffectType] = "minecraft:conduit_power"
    SLOW_FALLING: Identifier[EffectType] = "minecraft:slow_falling"
    BAD_OMEN: Identifier[EffectType] = "minecraft:bad_omen"
    VILLAGE_HERO: Identifier[EffectType] = "minecraft:village_hero"
    DARKNESS: Identifier[EffectType] = "minecraft:darkness"
    TRIAL_OMEN: Identifier[EffectType] = "minecraft:trial_omen"
    WIND_CHARGED: Identifier[EffectType] = "minecraft:wind_charged"
    WEAVING: Identifier[EffectType] = "minecraft:weaving"
    OOZING: Identifier[EffectType] = "minecraft:oozing"
    INFESTED: Identifier[EffectType] = "minecraft:infested"
    RAID_OMEN: Identifier[EffectType] = "minecraft:raid_omen"
    BREATH_OF_THE_NAUTILUS: Identifier[EffectType] = "minecraft:breath_of_the_nautilus"

class Effect:
    """
    Represents an effect that can be added to a `Mob`.
    """
    def __init__(
        self,
        type: Identifier[EffectType] | str,
        duration: int | None,
        amplifier: int,
        ambient: bool = False,
        particles: bool = True,
        icon: bool = True,
    ) -> None:
        """
        Creates an effect. A duration of None represents an infinite duration.
        """

    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def type(self) -> Identifier[EffectType]:
        """
        Gets the type of this effect.
        """

    @property
    def duration(self) -> int | None:
        """
        Gets the duration of this effect in ticks, or None if this effect is infinite.
        """

    @property
    def infinite(self) -> bool:
        """
        Whether this effect has an infinite duration.
        """

    @property
    def amplifier(self) -> int:
        """
        Gets the amplifier of this effect.
        """

    @property
    def ambient(self) -> bool:
        """
        Whether this effect produces more, translucent, particles.
        """

    @property
    def particles(self) -> bool:
        """
        Whether this effect has particles.
        """

    @property
    def icon(self) -> bool:
        """
        Whether this effect has an icon.
        """

class PotionType:
    """
    Represents a potion type.
    """
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def id(self) -> Identifier[PotionType]:
        """
        The identifier of this potion type.
        """

    @property
    def translation_key(self) -> str:
        """
        The translation key, suitable for use in a translation component.
        """

    @staticmethod
    def get(name: Identifier[PotionType] | str) -> PotionType:
        """
        Attempts to get the `PotionType` with the given name.

        Args:
            name: The identifier of the potion type (e.g. `minecraft:potion_type:water`).

        Returns:
            The `PotionType`, or `None` if no potion type with that name exists.
        """

    WATER: Identifier[PotionType] = "minecraft:potion_type:water"
    MUNDANE: Identifier[PotionType] = "minecraft:potion_type:mundane"
    LONG_MUNDANE: Identifier[PotionType] = "minecraft:potion_type:long_mundane"
    THICK: Identifier[PotionType] = "minecraft:potion_type:thick"
    AWKWARD: Identifier[PotionType] = "minecraft:potion_type:awkward"
    NIGHTVISION: Identifier[PotionType] = "minecraft:potion_type:nightvision"
    LONG_NIGHTVISION: Identifier[PotionType] = "minecraft:potion_type:long_nightvision"
    INVISIBILITY: Identifier[PotionType] = "minecraft:potion_type:invisibility"
    LONG_INVISIBILITY: Identifier[PotionType] = "minecraft:potion_type:long_invisibility"
    LEAPING: Identifier[PotionType] = "minecraft:potion_type:leaping"
    LONG_LEAPING: Identifier[PotionType] = "minecraft:potion_type:long_leaping"
    STRONG_LEAPING: Identifier[PotionType] = "minecraft:potion_type:strong_leaping"
    FIRE_RESISTANCE: Identifier[PotionType] = "minecraft:potion_type:fire_resistance"
    LONG_FIRE_RESISTANCE: Identifier[PotionType] = "minecraft:potion_type:long_fire_resistance"
    SWIFTNESS: Identifier[PotionType] = "minecraft:potion_type:swiftness"
    LONG_SWIFTNESS: Identifier[PotionType] = "minecraft:potion_type:long_swiftness"
    STRONG_SWIFTNESS: Identifier[PotionType] = "minecraft:potion_type:strong_swiftness"
    SLOWNESS: Identifier[PotionType] = "minecraft:potion_type:slowness"
    LONG_SLOWNESS: Identifier[PotionType] = "minecraft:potion_type:long_slowness"
    STRONG_SLOWNESS: Identifier[PotionType] = "minecraft:potion_type:strong_slowness"
    WATER_BREATHING: Identifier[PotionType] = "minecraft:potion_type:water_breathing"
    LONG_WATER_BREATHING: Identifier[PotionType] = "minecraft:potion_type:long_water_breathing"
    HEALING: Identifier[PotionType] = "minecraft:potion_type:healing"
    STRONG_HEALING: Identifier[PotionType] = "minecraft:potion_type:strong_healing"
    HARMING: Identifier[PotionType] = "minecraft:potion_type:harming"
    STRONG_HARMING: Identifier[PotionType] = "minecraft:potion_type:strong_harming"
    POISON: Identifier[PotionType] = "minecraft:potion_type:poison"
    LONG_POISON: Identifier[PotionType] = "minecraft:potion_type:long_poison"
    STRONG_POISON: Identifier[PotionType] = "minecraft:potion_type:strong_poison"
    REGENERATION: Identifier[PotionType] = "minecraft:potion_type:regeneration"
    LONG_REGENERATION: Identifier[PotionType] = "minecraft:potion_type:long_regeneration"
    STRONG_REGENERATION: Identifier[PotionType] = "minecraft:potion_type:strong_regeneration"
    STRENGTH: Identifier[PotionType] = "minecraft:potion_type:strength"
    LONG_STRENGTH: Identifier[PotionType] = "minecraft:potion_type:long_strength"
    STRONG_STRENGTH: Identifier[PotionType] = "minecraft:potion_type:strong_strength"
    WEAKNESS: Identifier[PotionType] = "minecraft:potion_type:weakness"
    LONG_WEAKNESS: Identifier[PotionType] = "minecraft:potion_type:long_weakness"
    WITHER: Identifier[PotionType] = "minecraft:potion_type:wither"
    TURTLE_MASTER: Identifier[PotionType] = "minecraft:potion_type:turtle_master"
    LONG_TURTLE_MASTER: Identifier[PotionType] = "minecraft:potion_type:long_turtle_master"
    STRONG_TURTLE_MASTER: Identifier[PotionType] = "minecraft:potion_type:strong_turtle_master"
    SLOW_FALLING: Identifier[PotionType] = "minecraft:potion_type:slow_falling"
    LONG_SLOW_FALLING: Identifier[PotionType] = "minecraft:potion_type:long_slow_falling"
    WIND_CHARGED: Identifier[PotionType] = "minecraft:potion_type:wind_charged"
    WEAVING: Identifier[PotionType] = "minecraft:potion_type:weaving"
    OOZING: Identifier[PotionType] = "minecraft:potion_type:oozing"
    INFESTED: Identifier[PotionType] = "minecraft:potion_type:infested"
