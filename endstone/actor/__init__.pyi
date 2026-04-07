"""
Classes relating to actors (entities) that can exist in a world, including all players, monsters, projectiles, etc.
"""

import typing

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

    AGENT = "minecraft:agent"
    ALLAY = "minecraft:allay"
    AREA_EFFECT_CLOUD = "minecraft:area_effect_cloud"
    ARMADILLO = "minecraft:armadillo"
    ARMOR_STAND = "minecraft:armor_stand"
    ARROW = "minecraft:arrow"
    AXOLOTL = "minecraft:axolotl"
    BALLOON = "minecraft:balloon"
    BAT = "minecraft:bat"
    BEE = "minecraft:bee"
    BLAZE = "minecraft:blaze"
    BOAT = "minecraft:boat"
    BOGGED = "minecraft:bogged"
    BREEZE = "minecraft:breeze"
    BREEZE_WIND_CHARGE_PROJECTILE = "minecraft:breeze_wind_charge_projectile"
    CAMEL = "minecraft:camel"
    CAMEL_HUSK = "minecraft:camel_husk"
    CAT = "minecraft:cat"
    CAVE_SPIDER = "minecraft:cave_spider"
    CHEST_BOAT = "minecraft:chest_boat"
    CHEST_MINECART = "minecraft:chest_minecart"
    CHICKEN = "minecraft:chicken"
    COD = "minecraft:cod"
    COMMAND_BLOCK_MINECART = "minecraft:command_block_minecart"
    COPPER_GOLEM = "minecraft:copper_golem"
    COW = "minecraft:cow"
    CREAKING = "minecraft:creaking"
    CREEPER = "minecraft:creeper"
    DOLPHIN = "minecraft:dolphin"
    DONKEY = "minecraft:donkey"
    DRAGON_FIREBALL = "minecraft:dragon_fireball"
    DROWNED = "minecraft:drowned"
    EGG = "minecraft:egg"
    ELDER_GUARDIAN = "minecraft:elder_guardian"
    ELDER_GUARDIAN_GHOST = "minecraft:elder_guardian_ghost"
    ENDER_CRYSTAL = "minecraft:ender_crystal"
    ENDER_DRAGON = "minecraft:ender_dragon"
    ENDER_PEARL = "minecraft:ender_pearl"
    ENDERMAN = "minecraft:enderman"
    ENDERMITE = "minecraft:endermite"
    EVOCATION_FANG = "minecraft:evocation_fang"
    EVOCATION_ILLAGER = "minecraft:evocation_illager"
    EYE_OF_ENDER_SIGNAL = "minecraft:eye_of_ender_signal"
    FALLING_BLOCK = "minecraft:falling_block"
    FIREBALL = "minecraft:fireball"
    FIREWORKS_ROCKET = "minecraft:fireworks_rocket"
    FISHING_HOOK = "minecraft:fishing_hook"
    FOX = "minecraft:fox"
    FROG = "minecraft:frog"
    GHAST = "minecraft:ghast"
    GLOW_SQUID = "minecraft:glow_squid"
    GOAT = "minecraft:goat"
    GUARDIAN = "minecraft:guardian"
    HAPPY_GHAST = "minecraft:happy_ghast"
    HOGLIN = "minecraft:hoglin"
    HOPPER_MINECART = "minecraft:hopper_minecart"
    HORSE = "minecraft:horse"
    HUSK = "minecraft:husk"
    ICE_BOMB = "minecraft:ice_bomb"
    IRON_GOLEM = "minecraft:iron_golem"
    ITEM = "minecraft:item"
    LEASH_KNOT = "minecraft:leash_knot"
    LIGHTNING_BOLT = "minecraft:lightning_bolt"
    LINGERING_POTION = "minecraft:lingering_potion"
    LLAMA = "minecraft:llama"
    LLAMA_SPIT = "minecraft:llama_spit"
    MAGMA_CUBE = "minecraft:magma_cube"
    MINECART = "minecraft:minecart"
    MOOSHROOM = "minecraft:mooshroom"
    MULE = "minecraft:mule"
    NAUTILUS = "minecraft:nautilus"
    NPC = "minecraft:npc"
    OCELOT = "minecraft:ocelot"
    OMINOUS_ITEM_SPAWNER = "minecraft:ominous_item_spawner"
    PAINTING = "minecraft:painting"
    PANDA = "minecraft:panda"
    PARCHED = "minecraft:parched"
    PARROT = "minecraft:parrot"
    PHANTOM = "minecraft:phantom"
    PIG = "minecraft:pig"
    PIGLIN = "minecraft:piglin"
    PIGLIN_BRUTE = "minecraft:piglin_brute"
    PILLAGER = "minecraft:pillager"
    PLAYER = "minecraft:player"
    POLAR_BEAR = "minecraft:polar_bear"
    PUFFERFISH = "minecraft:pufferfish"
    RABBIT = "minecraft:rabbit"
    RAVAGER = "minecraft:ravager"
    SALMON = "minecraft:salmon"
    SHEEP = "minecraft:sheep"
    SHULKER = "minecraft:shulker"
    SHULKER_BULLET = "minecraft:shulker_bullet"
    SILVERFISH = "minecraft:silverfish"
    SKELETON = "minecraft:skeleton"
    SKELETON_HORSE = "minecraft:skeleton_horse"
    SLIME = "minecraft:slime"
    SMALL_FIREBALL = "minecraft:small_fireball"
    SNIFFER = "minecraft:sniffer"
    SNOW_GOLEM = "minecraft:snow_golem"
    SNOWBALL = "minecraft:snowball"
    SPIDER = "minecraft:spider"
    SPLASH_POTION = "minecraft:splash_potion"
    SQUID = "minecraft:squid"
    STRAY = "minecraft:stray"
    STRIDER = "minecraft:strider"
    TADPOLE = "minecraft:tadpole"
    THROWN_TRIDENT = "minecraft:thrown_trident"
    TNT = "minecraft:tnt"
    TNT_MINECART = "minecraft:tnt_minecart"
    TRADER_LLAMA = "minecraft:trader_llama"
    TRIPOD_CAMERA = "minecraft:tripod_camera"
    TROPICALFISH = "minecraft:tropicalfish"
    TURTLE = "minecraft:turtle"
    VEX = "minecraft:vex"
    VILLAGER = "minecraft:villager"
    VILLAGER_V2 = "minecraft:villager_v2"
    VINDICATOR = "minecraft:vindicator"
    WANDERING_TRADER = "minecraft:wandering_trader"
    WARDEN = "minecraft:warden"
    WIND_CHARGE_PROJECTILE = "minecraft:wind_charge_projectile"
    WITCH = "minecraft:witch"
    WITHER = "minecraft:wither"
    WITHER_SKELETON = "minecraft:wither_skeleton"
    WITHER_SKULL = "minecraft:wither_skull"
    WITHER_SKULL_DANGEROUS = "minecraft:wither_skull_dangerous"
    WOLF = "minecraft:wolf"
    XP_BOTTLE = "minecraft:xp_bottle"
    XP_ORB = "minecraft:xp_orb"
    ZOGLIN = "minecraft:zoglin"
    ZOMBIE = "minecraft:zombie"
    ZOMBIE_HORSE = "minecraft:zombie_horse"
    ZOMBIE_NAUTILUS = "minecraft:zombie_nautilus"
    ZOMBIE_PIGMAN = "minecraft:zombie_pigman"
    ZOMBIE_VILLAGER = "minecraft:zombie_villager"
    ZOMBIE_VILLAGER_V2 = "minecraft:zombie_villager_v2"
    @property
    def id(self) -> str:
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
    def get(name: str) -> ActorType:
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
