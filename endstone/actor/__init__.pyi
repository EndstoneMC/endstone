"""
Classes relating to actors (entities) that can exist in a world, including all players, monsters, projectiles, etc.
"""

import typing

from endstone import Identifier
from endstone.command import CommandSender
from endstone.inventory import ItemStack
from endstone.level import Dimension, Level, Location
from endstone.util import Vector

__all__ = ["Actor", "ActorType", "Item", "Mob"]

class Actor(CommandSender):
    """
    Represents a base actor in the level.
    """
    @property
    def type(self) -> ActorType:
        """
        Gets the type of the actor.
        """
        ...
    @property
    def runtime_id(self) -> int:
        """
        Returns the runtime id for this actor.
        """
        ...
    @property
    def location(self) -> Location:
        """
        Gets the actor's current position.
        """
        ...
    @property
    def velocity(self) -> Vector:
        """
        Gets this actor's current velocity.
        """
        ...
    @property
    def is_on_ground(self) -> bool:
        """
        Returns true if the actor is supported by a block, i.e. on ground.
        """
        ...
    @property
    def is_in_water(self) -> bool:
        """
        Returns true if the actor is in water.
        """
        ...
    @property
    def is_in_lava(self) -> bool:
        """
        Returns true if the actor is in lava.
        """
        ...
    @property
    def level(self) -> Level:
        """
        Gets the current Level this actor resides in.
        """
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the current Dimension this actor resides in.
        """
        ...
    def set_rotation(self, yaw: float, pitch: float) -> None:
        """
        Sets the actor's rotation.
        """
        ...
    @typing.overload
    def teleport(self, location: Location) -> bool:
        """
        Teleports this actor to the given location.
        """
        ...
    @typing.overload
    def teleport(self, target: Actor) -> bool:
        """
        Teleports this actor to the target Actor.
        """
        ...
    @property
    def id(self) -> int:
        """
        Returns a unique id for this actor.
        """
        ...
    def remove(self) -> None:
        """
        Remove this actor from the level.
        """
        ...
    @property
    def is_valid(self) -> bool:
        """
        Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.
        """
        ...
    @property
    def is_dead(self) -> bool:
        """
        Returns true if this actor has been marked for removal.
        """
        ...
    @property
    def scoreboard_tags(self) -> list[str]:
        """
        Returns a list of scoreboard tags for this actor.
        """
        ...
    def add_scoreboard_tag(self, tag: str) -> bool:
        """
        Adds a tag to this actor.
        """
        ...
    def remove_scoreboard_tag(self, tag: str) -> bool:
        """
        Removes a given tag from this actor.
        """
        ...
    @property
    def is_name_tag_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is visible or not.
        """
        ...
    @is_name_tag_visible.setter
    def is_name_tag_visible(self, arg1: bool) -> None: ...
    @property
    def is_name_tag_always_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is always visible or not.
        """
        ...
    @is_name_tag_always_visible.setter
    def is_name_tag_always_visible(self, arg1: bool) -> None: ...
    @property
    def name_tag(self) -> str:
        """
        Gets or sets the current name tag of the actor.
        """
        ...
    @name_tag.setter
    def name_tag(self, arg1: str) -> None: ...
    @property
    def score_tag(self) -> str:
        """
        Gets or sets the current score tag of the actor.
        """
        ...
    @score_tag.setter
    def score_tag(self, arg1: str) -> None: ...

class Mob(Actor):
    """
    Represents a mobile entity (i.e. living entity), such as a monster or player.
    """
    @property
    def is_gliding(self) -> bool:
        """
        Checks to see if an actor is gliding, such as using an Elytra.
        """
        ...
    @property
    def health(self) -> int:
        """
        Gets or sets the entity's health from 0 to its max possible value, where 0 is dead.
        """
        ...
    @health.setter
    def health(self, arg1: int) -> None: ...
    @property
    def max_health(self) -> int:
        """
        Gets or sets the maximum health this entity has.
        """
        ...
    @max_health.setter
    def max_health(self, arg1: int) -> None: ...

class ActorType:
    """
    Represents an actor type.
    """

    AGENT: Identifier[ActorType] = "minecraft:agent"
    ALLAY: Identifier[ActorType] = "minecraft:allay"
    AREA_EFFECT_CLOUD: Identifier[ActorType] = "minecraft:area_effect_cloud"
    ARMADILLO: Identifier[ActorType] = "minecraft:armadillo"
    ARMOR_STAND: Identifier[ActorType] = "minecraft:armor_stand"
    ARROW: Identifier[ActorType] = "minecraft:arrow"
    AXOLOTL: Identifier[ActorType] = "minecraft:axolotl"
    BALLOON: Identifier[ActorType] = "minecraft:balloon"
    BAT: Identifier[ActorType] = "minecraft:bat"
    BEE: Identifier[ActorType] = "minecraft:bee"
    BLAZE: Identifier[ActorType] = "minecraft:blaze"
    BOAT: Identifier[ActorType] = "minecraft:boat"
    BOGGED: Identifier[ActorType] = "minecraft:bogged"
    BREEZE: Identifier[ActorType] = "minecraft:breeze"
    BREEZE_WIND_CHARGE_PROJECTILE: Identifier[ActorType] = "minecraft:breeze_wind_charge_projectile"
    CAMEL: Identifier[ActorType] = "minecraft:camel"
    CAMEL_HUSK: Identifier[ActorType] = "minecraft:camel_husk"
    CAT: Identifier[ActorType] = "minecraft:cat"
    CAVE_SPIDER: Identifier[ActorType] = "minecraft:cave_spider"
    CHEST_BOAT: Identifier[ActorType] = "minecraft:chest_boat"
    CHEST_MINECART: Identifier[ActorType] = "minecraft:chest_minecart"
    CHICKEN: Identifier[ActorType] = "minecraft:chicken"
    COD: Identifier[ActorType] = "minecraft:cod"
    COMMAND_BLOCK_MINECART: Identifier[ActorType] = "minecraft:command_block_minecart"
    COPPER_GOLEM: Identifier[ActorType] = "minecraft:copper_golem"
    COW: Identifier[ActorType] = "minecraft:cow"
    CREAKING: Identifier[ActorType] = "minecraft:creaking"
    CREEPER: Identifier[ActorType] = "minecraft:creeper"
    DOLPHIN: Identifier[ActorType] = "minecraft:dolphin"
    DONKEY: Identifier[ActorType] = "minecraft:donkey"
    DRAGON_FIREBALL: Identifier[ActorType] = "minecraft:dragon_fireball"
    DROWNED: Identifier[ActorType] = "minecraft:drowned"
    EGG: Identifier[ActorType] = "minecraft:egg"
    ELDER_GUARDIAN: Identifier[ActorType] = "minecraft:elder_guardian"
    ELDER_GUARDIAN_GHOST: Identifier[ActorType] = "minecraft:elder_guardian_ghost"
    ENDER_CRYSTAL: Identifier[ActorType] = "minecraft:ender_crystal"
    ENDER_DRAGON: Identifier[ActorType] = "minecraft:ender_dragon"
    ENDER_PEARL: Identifier[ActorType] = "minecraft:ender_pearl"
    ENDERMAN: Identifier[ActorType] = "minecraft:enderman"
    ENDERMITE: Identifier[ActorType] = "minecraft:endermite"
    EVOCATION_FANG: Identifier[ActorType] = "minecraft:evocation_fang"
    EVOCATION_ILLAGER: Identifier[ActorType] = "minecraft:evocation_illager"
    EYE_OF_ENDER_SIGNAL: Identifier[ActorType] = "minecraft:eye_of_ender_signal"
    FALLING_BLOCK: Identifier[ActorType] = "minecraft:falling_block"
    FIREBALL: Identifier[ActorType] = "minecraft:fireball"
    FIREWORKS_ROCKET: Identifier[ActorType] = "minecraft:fireworks_rocket"
    FISHING_HOOK: Identifier[ActorType] = "minecraft:fishing_hook"
    FOX: Identifier[ActorType] = "minecraft:fox"
    FROG: Identifier[ActorType] = "minecraft:frog"
    GHAST: Identifier[ActorType] = "minecraft:ghast"
    GLOW_SQUID: Identifier[ActorType] = "minecraft:glow_squid"
    GOAT: Identifier[ActorType] = "minecraft:goat"
    GUARDIAN: Identifier[ActorType] = "minecraft:guardian"
    HAPPY_GHAST: Identifier[ActorType] = "minecraft:happy_ghast"
    HOGLIN: Identifier[ActorType] = "minecraft:hoglin"
    HOPPER_MINECART: Identifier[ActorType] = "minecraft:hopper_minecart"
    HORSE: Identifier[ActorType] = "minecraft:horse"
    HUSK: Identifier[ActorType] = "minecraft:husk"
    ICE_BOMB: Identifier[ActorType] = "minecraft:ice_bomb"
    IRON_GOLEM: Identifier[ActorType] = "minecraft:iron_golem"
    ITEM: Identifier[ActorType] = "minecraft:item"
    LEASH_KNOT: Identifier[ActorType] = "minecraft:leash_knot"
    LIGHTNING_BOLT: Identifier[ActorType] = "minecraft:lightning_bolt"
    LINGERING_POTION: Identifier[ActorType] = "minecraft:lingering_potion"
    LLAMA: Identifier[ActorType] = "minecraft:llama"
    LLAMA_SPIT: Identifier[ActorType] = "minecraft:llama_spit"
    MAGMA_CUBE: Identifier[ActorType] = "minecraft:magma_cube"
    MINECART: Identifier[ActorType] = "minecraft:minecart"
    MOOSHROOM: Identifier[ActorType] = "minecraft:mooshroom"
    MULE: Identifier[ActorType] = "minecraft:mule"
    NAUTILUS: Identifier[ActorType] = "minecraft:nautilus"
    NPC: Identifier[ActorType] = "minecraft:npc"
    OCELOT: Identifier[ActorType] = "minecraft:ocelot"
    OMINOUS_ITEM_SPAWNER: Identifier[ActorType] = "minecraft:ominous_item_spawner"
    PAINTING: Identifier[ActorType] = "minecraft:painting"
    PANDA: Identifier[ActorType] = "minecraft:panda"
    PARCHED: Identifier[ActorType] = "minecraft:parched"
    PARROT: Identifier[ActorType] = "minecraft:parrot"
    PHANTOM: Identifier[ActorType] = "minecraft:phantom"
    PIG: Identifier[ActorType] = "minecraft:pig"
    PIGLIN: Identifier[ActorType] = "minecraft:piglin"
    PIGLIN_BRUTE: Identifier[ActorType] = "minecraft:piglin_brute"
    PILLAGER: Identifier[ActorType] = "minecraft:pillager"
    PLAYER: Identifier[ActorType] = "minecraft:player"
    POLAR_BEAR: Identifier[ActorType] = "minecraft:polar_bear"
    PUFFERFISH: Identifier[ActorType] = "minecraft:pufferfish"
    RABBIT: Identifier[ActorType] = "minecraft:rabbit"
    RAVAGER: Identifier[ActorType] = "minecraft:ravager"
    SALMON: Identifier[ActorType] = "minecraft:salmon"
    SHEEP: Identifier[ActorType] = "minecraft:sheep"
    SHULKER: Identifier[ActorType] = "minecraft:shulker"
    SHULKER_BULLET: Identifier[ActorType] = "minecraft:shulker_bullet"
    SILVERFISH: Identifier[ActorType] = "minecraft:silverfish"
    SKELETON: Identifier[ActorType] = "minecraft:skeleton"
    SKELETON_HORSE: Identifier[ActorType] = "minecraft:skeleton_horse"
    SLIME: Identifier[ActorType] = "minecraft:slime"
    SMALL_FIREBALL: Identifier[ActorType] = "minecraft:small_fireball"
    SNIFFER: Identifier[ActorType] = "minecraft:sniffer"
    SNOW_GOLEM: Identifier[ActorType] = "minecraft:snow_golem"
    SNOWBALL: Identifier[ActorType] = "minecraft:snowball"
    SPIDER: Identifier[ActorType] = "minecraft:spider"
    SPLASH_POTION: Identifier[ActorType] = "minecraft:splash_potion"
    SQUID: Identifier[ActorType] = "minecraft:squid"
    STRAY: Identifier[ActorType] = "minecraft:stray"
    STRIDER: Identifier[ActorType] = "minecraft:strider"
    TADPOLE: Identifier[ActorType] = "minecraft:tadpole"
    THROWN_TRIDENT: Identifier[ActorType] = "minecraft:thrown_trident"
    TNT: Identifier[ActorType] = "minecraft:tnt"
    TNT_MINECART: Identifier[ActorType] = "minecraft:tnt_minecart"
    TRADER_LLAMA: Identifier[ActorType] = "minecraft:trader_llama"
    TRIPOD_CAMERA: Identifier[ActorType] = "minecraft:tripod_camera"
    TROPICALFISH: Identifier[ActorType] = "minecraft:tropicalfish"
    TURTLE: Identifier[ActorType] = "minecraft:turtle"
    VEX: Identifier[ActorType] = "minecraft:vex"
    VILLAGER: Identifier[ActorType] = "minecraft:villager"
    VILLAGER_V2: Identifier[ActorType] = "minecraft:villager_v2"
    VINDICATOR: Identifier[ActorType] = "minecraft:vindicator"
    WANDERING_TRADER: Identifier[ActorType] = "minecraft:wandering_trader"
    WARDEN: Identifier[ActorType] = "minecraft:warden"
    WIND_CHARGE_PROJECTILE: Identifier[ActorType] = "minecraft:wind_charge_projectile"
    WITCH: Identifier[ActorType] = "minecraft:witch"
    WITHER: Identifier[ActorType] = "minecraft:wither"
    WITHER_SKELETON: Identifier[ActorType] = "minecraft:wither_skeleton"
    WITHER_SKULL: Identifier[ActorType] = "minecraft:wither_skull"
    WITHER_SKULL_DANGEROUS: Identifier[ActorType] = "minecraft:wither_skull_dangerous"
    WOLF: Identifier[ActorType] = "minecraft:wolf"
    XP_BOTTLE: Identifier[ActorType] = "minecraft:xp_bottle"
    XP_ORB: Identifier[ActorType] = "minecraft:xp_orb"
    ZOGLIN: Identifier[ActorType] = "minecraft:zoglin"
    ZOMBIE: Identifier[ActorType] = "minecraft:zombie"
    ZOMBIE_HORSE: Identifier[ActorType] = "minecraft:zombie_horse"
    ZOMBIE_NAUTILUS: Identifier[ActorType] = "minecraft:zombie_nautilus"
    ZOMBIE_PIGMAN: Identifier[ActorType] = "minecraft:zombie_pigman"
    ZOMBIE_VILLAGER: Identifier[ActorType] = "minecraft:zombie_villager"
    ZOMBIE_VILLAGER_V2: Identifier[ActorType] = "minecraft:zombie_villager_v2"
    @property
    def id(self) -> Identifier[ActorType]:
        """
        Return the identifier of this actor type.
        """
        ...
    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
        ...
    @staticmethod
    def get(name: Identifier[ActorType] | str) -> ActorType:
        """
        Attempts to get the ActorType with the given name.
        """
        ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...
    def __hash__(self) -> int: ...
    def __eq__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...

class Item(Actor):
    """
    Represents a base actor in the level.
    """
    @property
    def item_stack(self) -> ItemStack:
        """
        Gets or sets the item stack associated with this item drop.
        """
        ...
    @item_stack.setter
    def item_stack(self, arg1: ItemStack) -> None: ...
    @property
    def pickup_delay(self) -> int:
        """
        Gets or sets the delay before this Item is available to be picked up by players.
        """
        ...
    @pickup_delay.setter
    def pickup_delay(self, arg1: int) -> None: ...
    @property
    def is_unlimited_lifetime(self) -> bool:
        """
        Gets or sets if this Item lives forever
        """
        ...
    @is_unlimited_lifetime.setter
    def is_unlimited_lifetime(self, arg1: bool) -> None: ...
    @property
    def thrower(self) -> int | None:
        """
        Gets or sets the thrower of this item.
        """
        ...
    @thrower.setter
    def thrower(self, arg1: int | None) -> None: ...
