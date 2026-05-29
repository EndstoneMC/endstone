"""
Classes relating to the effects that can be applied to entities.
"""

from endstone import Identifier

__all__ = [
    "EffectType",
]

class EffectType:
    """
    All effect types.
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
    HERO_OF_THE_VILLAGE: Identifier[EffectType] = "minecraft:hero_of_the_village"
    DARKNESS: Identifier[EffectType] = "minecraft:darkness"
    TRIAL_OMEN: Identifier[EffectType] = "minecraft:trial_omen"
    WIND_CHARGED: Identifier[EffectType] = "minecraft:wind_charged"
    WEAVING: Identifier[EffectType] = "minecraft:weaving"
    OOZING: Identifier[EffectType] = "minecraft:oozing"
    INFESTED: Identifier[EffectType] = "minecraft:infested"
    RAID_OMEN: Identifier[EffectType] = "minecraft:raid_omen"
    BREATH_OF_THE_NAUTILUS: Identifier[EffectType] = "minecraft:breath_of_the_nautilus"
