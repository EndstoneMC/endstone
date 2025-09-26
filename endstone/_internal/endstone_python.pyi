from __future__ import annotations
import datetime
import numpy
import os
import typing
import uuid
__all__: list[str] = ['ActionForm', 'Actor', 'ActorDamageEvent', 'ActorDeathEvent', 'ActorEvent', 'ActorExplodeEvent', 'ActorKnockbackEvent', 'ActorRemoveEvent', 'ActorSpawnEvent', 'ActorTeleportEvent', 'BanEntry', 'BarColor', 'BarFlag', 'BarStyle', 'Block', 'BlockBreakEvent', 'BlockCommandSender', 'BlockCookEvent', 'BlockData', 'BlockEvent', 'BlockFace', 'BlockPistonEvent', 'BlockPistonExtendEvent', 'BlockPistonRetractEvent', 'BlockPlaceEvent', 'BlockState', 'BossBar', 'BroadcastMessageEvent', 'Button', 'Cancellable', 'Chunk', 'ChunkEvent', 'ChunkLoadEvent', 'ChunkUnloadEvent', 'ColorFormat', 'Command', 'CommandExecutor', 'CommandSender', 'CommandSenderWrapper', 'ConsoleCommandSender', 'Criteria', 'DamageSource', 'Dimension', 'DimensionEvent', 'DisplaySlot', 'Divider', 'Dropdown', 'Enchantment', 'EnchantmentRegistry', 'EquipmentSlot', 'Event', 'EventPriority', 'EventResult', 'GameMode', 'Header', 'Inventory', 'IpBanEntry', 'IpBanList', 'Item', 'ItemFactory', 'ItemMeta', 'ItemRegistry', 'ItemStack', 'ItemType', 'Label', 'Language', 'LeavesDecayEvent', 'Level', 'LevelEvent', 'Location', 'Logger', 'MapCanvas', 'MapMeta', 'MapRenderer', 'MapView', 'MessageForm', 'Mob', 'MobEvent', 'ModalForm', 'NamespacedKey', 'Objective', 'ObjectiveSortOrder', 'OfflinePlayer', 'PacketReceiveEvent', 'PacketSendEvent', 'Permissible', 'Permission', 'PermissionAttachment', 'PermissionAttachmentInfo', 'PermissionDefault', 'PermissionLevel', 'Player', 'PlayerBanEntry', 'PlayerBanList', 'PlayerBedEnterEvent', 'PlayerBedLeaveEvent', 'PlayerChatEvent', 'PlayerCommandEvent', 'PlayerDeathEvent', 'PlayerDropItemEvent', 'PlayerEmoteEvent', 'PlayerEvent', 'PlayerGameModeChangeEvent', 'PlayerInteractActorEvent', 'PlayerInteractEvent', 'PlayerInventory', 'PlayerItemConsumeEvent', 'PlayerItemHeldEvent', 'PlayerJoinEvent', 'PlayerJumpEvent', 'PlayerKickEvent', 'PlayerLoginEvent', 'PlayerMoveEvent', 'PlayerPickupItemEvent', 'PlayerQuitEvent', 'PlayerRespawnEvent', 'PlayerSkinChangeEvent', 'PlayerTeleportEvent', 'Plugin', 'PluginCommand', 'PluginDescription', 'PluginDisableEvent', 'PluginEnableEvent', 'PluginLoadOrder', 'PluginLoader', 'PluginManager', 'Position', 'RenderType', 'Scheduler', 'Score', 'Scoreboard', 'ScriptMessageEvent', 'Server', 'ServerCommandEvent', 'ServerEvent', 'ServerListPingEvent', 'ServerLoadEvent', 'Service', 'ServiceManager', 'ServicePriority', 'Skin', 'Slider', 'SocketAddress', 'StepSlider', 'Task', 'TextInput', 'ThunderChangeEvent', 'Toggle', 'Translatable', 'Vector', 'WeatherChangeEvent', 'WeatherEvent']
class ActionForm:
    """
    Represents a form with buttons that let the player take action.
    """
    def __init__(self, title: str | Translatable = '', content: str | Translatable = '', buttons: list[Button | Divider | Header | Label] | None = None, on_submit: typing.Callable[[Player, int], None] = None, on_close: typing.Callable[[Player], None] = None) -> None:
        ...
    def add_button(self, text: str | Translatable, icon: str | None = None, on_click: typing.Callable[[Player], None] = None) -> ActionForm:
        """
        Adds a button to the form.
        """
    def add_divider(self) -> ActionForm:
        """
        Adds a divider to the form.
        """
    def add_header(self, text: str | Translatable) -> ActionForm:
        """
        Adds a header to the form.
        """
    def add_label(self, text: str | Translatable) -> ActionForm:
        """
        Adds a label to the form.
        """
    @property
    def content(self) -> str | Translatable:
        """
        Gets or sets the content of the form.
        """
    @content.setter
    def content(self, arg1: str | Translatable) -> ActionForm:
        ...
    @property
    def controls(self) -> list[Button | Divider | Header | Label]:
        """
        Gets or sets the controls of the action form.
        """
    @controls.setter
    def controls(self, arg1: list[Button | Divider | Header | Label]) -> ActionForm:
        ...
    @property
    def on_close(self) -> typing.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """
    @on_close.setter
    def on_close(self, arg1: typing.Callable[[Player], None]) -> ActionForm:
        ...
    @property
    def on_submit(self) -> typing.Callable[[Player, int], None]:
        """
        Gets or sets the on submit callback.
        """
    @on_submit.setter
    def on_submit(self, arg1: typing.Callable[[Player, int], None]) -> ActionForm:
        ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """
    @title.setter
    def title(self, arg1: str | Translatable) -> ActionForm:
        ...
class Actor(CommandSender):
    """
    Represents a base actor in the level.
    """
    def add_scoreboard_tag(self, tag: str) -> bool:
        """
        Adds a tag to this actor.
        """
    def remove(self) -> None:
        """
        Remove this actor from the level.
        """
    def remove_scoreboard_tag(self, tag: str) -> bool:
        """
        Removes a given tag from this actor.
        """
    def set_rotation(self, yaw: float, pitch: float) -> None:
        """
        Sets the actor's rotation.
        """
    @typing.overload
    def teleport(self, location: Location) -> None:
        """
        Teleports this actor to the given location.
        """
    @typing.overload
    def teleport(self, target: Actor) -> None:
        """
        Teleports this actor to the target Actor.
        """
    @property
    def dimension(self) -> Dimension:
        """
        Gets the current Dimension this actor resides in.
        """
    @property
    def health(self) -> int:
        """
        Gets or sets the entity's health from 0 to its max possible value, where 0 is dead.
        """
    @health.setter
    def health(self, arg1: int) -> None:
        ...
    @property
    def id(self) -> int:
        """
        Returns a unique id for this actor.
        """
    @property
    def is_dead(self) -> bool:
        """
        Returns true if this actor has been marked for removal.
        """
    @property
    def is_in_lava(self) -> bool:
        """
        Returns true if the actor is in lava.
        """
    @property
    def is_in_water(self) -> bool:
        """
        Returns true if the actor is in water.
        """
    @property
    def is_name_tag_always_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is always visible or not.
        """
    @is_name_tag_always_visible.setter
    def is_name_tag_always_visible(self, arg1: bool) -> None:
        ...
    @property
    def is_name_tag_visible(self) -> bool:
        """
        Gets or sets if the actor's name tag is visible or not.
        """
    @is_name_tag_visible.setter
    def is_name_tag_visible(self, arg1: bool) -> None:
        ...
    @property
    def is_on_ground(self) -> bool:
        """
        Returns true if the actor is supported by a block, i.e. on ground.
        """
    @property
    def is_valid(self) -> bool:
        """
        Returns false if the entity has died, been despawned for some other reason, or has not been added to the level.
        """
    @property
    def level(self) -> Level:
        """
        Gets the current Level this actor resides in.
        """
    @property
    def location(self) -> Location:
        """
        Gets the actor's current position.
        """
    @property
    def max_health(self) -> int:
        """
        Gets the maximum health this entity has.
        """
    @property
    def name_tag(self) -> str:
        """
        Gets or sets the current name tag of the actor.
        """
    @name_tag.setter
    def name_tag(self, arg1: str) -> None:
        ...
    @property
    def runtime_id(self) -> int:
        """
        Returns the runtime id for this actor.
        """
    @property
    def score_tag(self) -> str:
        """
        Gets or sets the current score tag of the actor.
        """
    @score_tag.setter
    def score_tag(self, arg1: str) -> None:
        ...
    @property
    def scoreboard_tags(self) -> list[str]:
        """
        Returns a list of scoreboard tags for this actor.
        """
    @property
    def type(self) -> str:
        """
        Gets the type of the actor.
        """
    @property
    def velocity(self) -> Vector:
        """
        Gets this actor's current velocity.
        """
class ActorDamageEvent(MobEvent, Cancellable):
    """
    Called when an Actor is damaged.
    """
    @property
    def damage(self) -> float:
        """
        Gets or sets the amount of damage caused by the event
        """
    @damage.setter
    def damage(self, arg1: float) -> None:
        ...
    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage.
        """
class ActorDeathEvent(MobEvent):
    """
    Called when an Actor dies.
    """
    @property
    def damage_source(self) -> DamageSource:
        """
        Gets the source of damage which caused the death.
        """
class ActorEvent(Event):
    """
    Represents an Actor-related event.
    """
    @property
    def actor(self) -> Actor:
        """
        Returns the Actor involved in this event
        """
class ActorExplodeEvent(ActorEvent, Cancellable):
    """
    Called when an Actor explodes.
    """
    @property
    def block_list(self) -> list[Block]:
        """
        Gets or sets the list of blocks that would have been removed or were removed from the explosion event.
        """
    @block_list.setter
    def block_list(self, arg1: list[Block]) -> None:
        ...
    @property
    def location(self) -> Location:
        """
        Returns the location where the explosion happened.
        """
class ActorKnockbackEvent(MobEvent, Cancellable):
    """
    Called when a living entity receives knockback.
    """
    @property
    def knockback(self) -> Vector:
        """
        Gets or sets the knockback that will be applied to the entity.
        """
    @knockback.setter
    def knockback(self, arg1: Vector) -> None:
        ...
    @property
    def source(self) -> Actor:
        """
        Get the source actor that has caused knockback to the defender, if exists.
        """
class ActorRemoveEvent(ActorEvent):
    """
    Called when an Actor is removed.
    """
class ActorSpawnEvent(ActorEvent, Cancellable):
    """
    Called when an Actor is spawned into a world.
    """
class ActorTeleportEvent(ActorEvent, Cancellable):
    """
    Called when a non-player entity is teleported from one location to another.
    """
    @property
    def from_location(self) -> Location:
        """
        Gets or sets the location that this actor moved from.
        """
    @from_location.setter
    def from_location(self, arg1: Location) -> None:
        ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this actor moved to.
        """
    @to_location.setter
    def to_location(self, arg1: Location) -> None:
        ...
class BanEntry:
    """
    A single entry from a ban list.
    """
    @property
    def created(self) -> datetime.datetime:
        """
        Gets or sets the date this ban entry was created.
        """
    @created.setter
    def created(self, arg1: datetime.datetime) -> None:
        ...
    @property
    def expiration(self) -> datetime.datetime | None:
        """
        Gets or sets the date this ban expires on.
        """
    @expiration.setter
    def expiration(self, arg1: datetime.datetime | None) -> None:
        ...
    @property
    def reason(self) -> str:
        """
        Gets or sets the reason for this ban.
        """
    @reason.setter
    def reason(self, arg1: str) -> None:
        ...
    @property
    def source(self) -> str:
        """
        Gets or sets the source of this ban.
        """
    @source.setter
    def source(self, arg1: str) -> None:
        ...
class BarColor:
    BLUE: typing.ClassVar[BarColor]  # value = <BarColor.BLUE: 1>
    GREEN: typing.ClassVar[BarColor]  # value = <BarColor.GREEN: 3>
    PINK: typing.ClassVar[BarColor]  # value = <BarColor.PINK: 0>
    PURPLE: typing.ClassVar[BarColor]  # value = <BarColor.PURPLE: 5>
    REBECCA_PURPLE: typing.ClassVar[BarColor]  # value = <BarColor.REBECCA_PURPLE: 6>
    RED: typing.ClassVar[BarColor]  # value = <BarColor.RED: 2>
    WHITE: typing.ClassVar[BarColor]  # value = <BarColor.WHITE: 7>
    YELLOW: typing.ClassVar[BarColor]  # value = <BarColor.YELLOW: 4>
    __members__: typing.ClassVar[dict[str, BarColor]]  # value = {'PINK': <BarColor.PINK: 0>, 'BLUE': <BarColor.BLUE: 1>, 'RED': <BarColor.RED: 2>, 'GREEN': <BarColor.GREEN: 3>, 'YELLOW': <BarColor.YELLOW: 4>, 'PURPLE': <BarColor.PURPLE: 5>, 'REBECCA_PURPLE': <BarColor.REBECCA_PURPLE: 6>, 'WHITE': <BarColor.WHITE: 7>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class BarFlag:
    DARKEN_SKY: typing.ClassVar[BarFlag]  # value = <BarFlag.DARKEN_SKY: 0>
    __members__: typing.ClassVar[dict[str, BarFlag]]  # value = {'DARKEN_SKY': <BarFlag.DARKEN_SKY: 0>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class BarStyle:
    SEGMENTED_10: typing.ClassVar[BarStyle]  # value = <BarStyle.SEGMENTED_10: 2>
    SEGMENTED_12: typing.ClassVar[BarStyle]  # value = <BarStyle.SEGMENTED_12: 3>
    SEGMENTED_20: typing.ClassVar[BarStyle]  # value = <BarStyle.SEGMENTED_20: 4>
    SEGMENTED_6: typing.ClassVar[BarStyle]  # value = <BarStyle.SEGMENTED_6: 1>
    SOLID: typing.ClassVar[BarStyle]  # value = <BarStyle.SOLID: 0>
    __members__: typing.ClassVar[dict[str, BarStyle]]  # value = {'SOLID': <BarStyle.SOLID: 0>, 'SEGMENTED_6': <BarStyle.SEGMENTED_6: 1>, 'SEGMENTED_10': <BarStyle.SEGMENTED_10: 2>, 'SEGMENTED_12': <BarStyle.SEGMENTED_12: 3>, 'SEGMENTED_20': <BarStyle.SEGMENTED_20: 4>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Block:
    """
    Represents a block.
    """
    def __str__(self) -> str:
        ...
    def capture_state(self) -> BlockState:
        """
        Captures the current state of this block. The returned object will never be updated, and you are not guaranteed that (for example) a sign is still a sign after you capture its state.
        """
    @typing.overload
    def get_relative(self, offset_x: int, offset_y: int, offset_z: int) -> Block:
        """
        Gets the block at the given offsets
        """
    @typing.overload
    def get_relative(self, face: BlockFace, distance: int = 1) -> Block:
        """
        Gets the block at the given distance of the given face
        """
    def set_data(self, data: BlockData, apply_physics: bool = True) -> None:
        """
        Sets the complete data for this block
        """
    def set_type(self, type: str, apply_physics: bool = True) -> None:
        """
        Sets the type of this block
        """
    @property
    def data(self) -> BlockData:
        """
        Gets the complete data for this block
        """
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension which contains this Block
        """
    @property
    def location(self) -> Location:
        """
        Gets the Location of the block
        """
    @property
    def type(self) -> str:
        """
        Gets or sets the type of the block.
        """
    @property
    def x(self) -> int:
        """
        Gets the x-coordinate of this block
        """
    @property
    def y(self) -> int:
        """
        Gets the y-coordinate of this block
        """
    @property
    def z(self) -> int:
        """
        Gets the z-coordinate of this block
        """
class BlockBreakEvent(BlockEvent, Cancellable):
    """
    Called when a block is broken by a player.
    """
    @property
    def player(self) -> Player:
        """
        Gets the Player that is breaking the block involved in this event.
        """
class BlockCommandSender(CommandSender):
    """
    Represents a block command sender.
    """
    @property
    def block(self) -> Block:
        """
        Returns the block this command sender belongs to
        """
class BlockCookEvent(BlockEvent, Cancellable):
    """
    Called when an ItemStack is successfully cooked in a block.
    """
    @property
    def result(self) -> ItemStack:
        """
        Gets or sets the resultant ItemStack for this event
        """
    @result.setter
    def result(self, arg1: ItemStack) -> None:
        ...
    @property
    def source(self) -> ItemStack:
        """
        Gets the smelted ItemStack for this event
        """
class BlockData:
    """
    Represents the data related to a live block
    """
    def __str__(self) -> str:
        ...
    @property
    def block_states(self) -> dict[str, bool | str | int]:
        """
        Gets the block states for this block.
        """
    @property
    def runtime_id(self) -> int:
        """
        Gets the runtime id for this block.
        """
    @property
    def type(self) -> str:
        """
        Get the block type represented by this block data.
        """
class BlockEvent(Event):
    """
    Represents an Block-related event
    """
    @property
    def block(self) -> Block:
        """
        Gets the block involved in this event.
        """
class BlockFace:
    DOWN: typing.ClassVar[BlockFace]  # value = <BlockFace.DOWN: 0>
    EAST: typing.ClassVar[BlockFace]  # value = <BlockFace.EAST: 5>
    NORTH: typing.ClassVar[BlockFace]  # value = <BlockFace.NORTH: 2>
    SOUTH: typing.ClassVar[BlockFace]  # value = <BlockFace.SOUTH: 3>
    UP: typing.ClassVar[BlockFace]  # value = <BlockFace.UP: 1>
    WEST: typing.ClassVar[BlockFace]  # value = <BlockFace.WEST: 4>
    __members__: typing.ClassVar[dict[str, BlockFace]]  # value = {'DOWN': <BlockFace.DOWN: 0>, 'UP': <BlockFace.UP: 1>, 'NORTH': <BlockFace.NORTH: 2>, 'SOUTH': <BlockFace.SOUTH: 3>, 'WEST': <BlockFace.WEST: 4>, 'EAST': <BlockFace.EAST: 5>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class BlockPistonEvent(BlockEvent, Cancellable):
    """
    Called when a piston block is triggered
    """
    @property
    def direction(self) -> BlockFace:
        """
        Return the direction in which the piston will operate.
        """
class BlockPistonExtendEvent(BlockPistonEvent):
    """
    Called when a piston extends.
    """
class BlockPistonRetractEvent(BlockPistonEvent):
    """
    Called when a piston retracts.
    """
class BlockPlaceEvent(BlockEvent, Cancellable):
    """
    Called when a block is placed by a player.
    """
    @property
    def block_against(self) -> Block:
        """
        Gets the block that this block was placed against
        """
    @property
    def block_placed_state(self) -> BlockState:
        """
        Gets the BlockState for the block which was placed.
        """
    @property
    def block_replaced(self) -> Block:
        """
        Gets the block which was replaced.
        """
    @property
    def player(self) -> Player:
        """
        Gets the player who placed the block involved in this event.
        """
class BlockState:
    """
    Represents a captured state of a block, which will not update automatically.
    """
    def __str__(self) -> str:
        ...
    def update(self, force: bool = False, apply_physics: bool = True) -> bool:
        """
        Attempts to update the block represented by this state.
        """
    @property
    def block(self) -> Block:
        """
        Gets the block represented by this block state.
        """
    @property
    def data(self) -> BlockData:
        """
        Gets or sets the data for this block state.
        """
    @data.setter
    def data(self, arg1: BlockData) -> None:
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension which contains the block represented by this block state.
        """
    @property
    def location(self) -> Location:
        """
        Gets the location of this block state.
        """
    @property
    def type(self) -> str:
        """
        Gets or sets the type of this block state.
        """
    @type.setter
    def type(self, arg1: str) -> None:
        ...
    @property
    def x(self) -> int:
        """
        Gets the x-coordinate of this block state.
        """
    @property
    def y(self) -> int:
        """
        Gets the y-coordinate of this block state.
        """
    @property
    def z(self) -> int:
        """
        Gets the z-coordinate of this block state.
        """
class BossBar:
    """
    Represents a boss bar that is displayed to players.
    """
    def add_flag(self, flag: BarFlag) -> None:
        """
        Adds an optional flag to this boss bar.
        """
    def add_player(self, player: Player) -> None:
        """
        Adds the player to this boss bar causing it to display on their screen.
        """
    def has_flag(self, flag: BarFlag) -> bool:
        """
        Checks whether this boss bar has the passed flag set.
        """
    def remove_all(self) -> None:
        """
        Removes all players from this boss bar.
        """
    def remove_flag(self, flag: BarFlag) -> None:
        """
        Removes an existing flag on this boss bar.
        """
    def remove_player(self, player: Player) -> None:
        """
        Removes the player from this boss bar causing it to be removed from their screen.
        """
    @property
    def color(self) -> BarColor:
        """
        The color of this boss bar.
        """
    @color.setter
    def color(self, arg1: BarColor) -> None:
        ...
    @property
    def is_visible(self) -> bool:
        """
        If the boss bar is displayed to attached players.
        """
    @is_visible.setter
    def is_visible(self, arg1: bool) -> None:
        ...
    @property
    def players(self) -> list[Player]:
        """
        Returns all players viewing this boss bar.
        """
    @property
    def progress(self) -> float:
        """
        The progress of the bar between 0.0 and 1.0.
        """
    @progress.setter
    def progress(self, arg1: float) -> None:
        ...
    @property
    def style(self) -> BarStyle:
        """
        The style of this boss bar.
        """
    @style.setter
    def style(self, arg1: BarStyle) -> None:
        ...
    @property
    def title(self) -> str:
        """
        The title of this boss bar.
        """
    @title.setter
    def title(self, arg1: str) -> None:
        ...
class BroadcastMessageEvent(ServerEvent, Cancellable):
    """
    Event triggered for server broadcast messages such as from Server.broadcast
    """
    @property
    def message(self) -> str | Translatable:
        """
        Gets or sets the message to broadcast.
        """
    @message.setter
    def message(self, arg1: str | Translatable) -> None:
        ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        Gets a set of recipients that this broadcast message will be displayed to.
        """
class Button:
    """
    Represents a button with text and an optional icon.
    """
    def __init__(self, text: str | Translatable = '', icon: str | None = None, on_click: typing.Callable[[Player], None] = None) -> None:
        ...
    @property
    def icon(self) -> str | None:
        """
        Gets or sets the icon path or URL of the button
        """
    @icon.setter
    def icon(self, arg1: str) -> Button:
        ...
    @property
    def on_click(self) -> typing.Callable[[Player], None]:
        """
        Gets or sets the on click callback.
        """
    @on_click.setter
    def on_click(self, arg1: typing.Callable[[Player], None]) -> Button:
        ...
    @property
    def text(self) -> str | Translatable:
        """
        Gets or sets the text of the button
        """
    @text.setter
    def text(self, arg1: str | Translatable) -> Button:
        ...
class Cancellable:
    """
    Represents an event that may be cancelled by a plugin or the server.
    """
    def cancel(self) -> None:
        """
        Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
    @property
    def cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins. [Warning] Deprecated: Use is_cancelled instead.
        """
    @cancelled.setter
    def cancelled(self, arg1: bool) -> None:
        ...
    @property
    def is_cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
    @is_cancelled.setter
    def is_cancelled(self, arg1: bool) -> None:
        ...
class Chunk:
    """
    Represents a chunk of blocks.
    """
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension containing this chunk
        """
    @property
    def level(self) -> Level:
        """
        Gets the level containing this chunk
        """
    @property
    def x(self) -> int:
        """
        Gets the X-coordinate of this chunk
        """
    @property
    def z(self) -> int:
        """
        Gets the Z-coordinate of this chunk
        """
class ChunkEvent(DimensionEvent):
    """
    Represents a Chunk related event
    """
    @property
    def chunk(self) -> Chunk:
        """
        Gets the chunk being loaded/unloaded
        """
class ChunkLoadEvent(ChunkEvent):
    """
    Called when a chunk is loaded
    """
class ChunkUnloadEvent(ChunkEvent):
    """
    Called when a chunk is unloaded
    """
class ColorFormat:
    """
    All supported color and format codes.
    """
    AQUA: typing.ClassVar[str] = '§b'
    BLACK: typing.ClassVar[str] = '§0'
    BLUE: typing.ClassVar[str] = '§9'
    BOLD: typing.ClassVar[str] = '§l'
    DARK_AQUA: typing.ClassVar[str] = '§3'
    DARK_BLUE: typing.ClassVar[str] = '§1'
    DARK_GRAY: typing.ClassVar[str] = '§8'
    DARK_GREEN: typing.ClassVar[str] = '§2'
    DARK_PURPLE: typing.ClassVar[str] = '§5'
    DARK_RED: typing.ClassVar[str] = '§4'
    GOLD: typing.ClassVar[str] = '§6'
    GRAY: typing.ClassVar[str] = '§7'
    GREEN: typing.ClassVar[str] = '§a'
    ITALIC: typing.ClassVar[str] = '§o'
    LIGHT_PURPLE: typing.ClassVar[str] = '§d'
    MATERIAL_AMETHYST: typing.ClassVar[str] = '§u'
    MATERIAL_COPPER: typing.ClassVar[str] = '§n'
    MATERIAL_DIAMOND: typing.ClassVar[str] = '§s'
    MATERIAL_EMERALD: typing.ClassVar[str] = '§q'
    MATERIAL_GOLD: typing.ClassVar[str] = '§p'
    MATERIAL_IRON: typing.ClassVar[str] = '§i'
    MATERIAL_LAPIS: typing.ClassVar[str] = '§t'
    MATERIAL_NETHERITE: typing.ClassVar[str] = '§j'
    MATERIAL_QUARTZ: typing.ClassVar[str] = '§h'
    MATERIAL_REDSTONE: typing.ClassVar[str] = '§m'
    MATERIAL_RESIN: typing.ClassVar[str] = '§v'
    MINECOIN_GOLD: typing.ClassVar[str] = '§g'
    OBFUSCATED: typing.ClassVar[str] = '§k'
    RED: typing.ClassVar[str] = '§c'
    RESET: typing.ClassVar[str] = '§r'
    WHITE: typing.ClassVar[str] = '§f'
    YELLOW: typing.ClassVar[str] = '§e'
class Command:
    """
    Represents a Command, which executes various tasks upon user input
    """
    def __init__(self, name: str, description: str | None = None, usages: list[str] | None = None, aliases: list[str] | None = None, permissions: list[str] | None = None, *args, **kwargs) -> None:
        ...
    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        """
        Executes the command, returning its success
        """
    def test_permission(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command.
        """
    def test_permission_silently(self, target: CommandSender) -> bool:
        """
        Tests the given CommandSender to see if they can perform this command. No error is sent to the sender.
        """
    @property
    def aliases(self) -> list[str]:
        """
        List of aliases of this command
        """
    @aliases.setter
    def aliases(self, arg1: list[str]) -> None:
        ...
    @property
    def description(self) -> str:
        """
        Brief description of this command
        """
    @description.setter
    def description(self, arg1: str) -> None:
        ...
    @property
    def is_registered(self) -> bool:
        """
        Returns the current registered state of this command
        """
    @property
    def name(self) -> str:
        """
        Name of this command.
        """
    @name.setter
    def name(self, arg1: str) -> None:
        ...
    @property
    def permissions(self) -> list[str]:
        """
        The permissions required by users to be able to perform this command
        """
    @permissions.setter
    def permissions(self, arg1: list[str]) -> None:
        ...
    @property
    def usages(self) -> list[str]:
        """
        List of usages of this command
        """
    @usages.setter
    def usages(self, arg1: list[str]) -> None:
        ...
class CommandExecutor:
    """
    Represents a class which contains a single method for executing commands
    """
    def __init__(self) -> None:
        ...
    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        """
        Executes the given command, returning its success.
        """
class CommandSender(Permissible):
    """
    Represents a command sender.
    """
    def send_error_message(self, message: str | Translatable) -> None:
        """
        Sends this sender an error message
        """
    def send_message(self, message: str | Translatable) -> None:
        """
        Sends this sender a message
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this command sender
        """
    @property
    def server(self) -> Server:
        """
        Returns the server instance that this command is running on
        """
class CommandSenderWrapper(CommandSender):
    """
    Represents a wrapper that forwards commands to the wrapped CommandSender and captures its output
    """
    def __init__(self, sender: CommandSender, on_message: typing.Callable[[str | Translatable], None] = None, on_error: typing.Callable[[str | Translatable], None] = None) -> None:
        ...
class ConsoleCommandSender(CommandSender):
    """
    Represents a console command sender.
    """
class Criteria:
    """
    Represents a scoreboard criteria.
    """
    class Type:
        """
        Represents a scoreboard criteria.
        """
        DUMMY: typing.ClassVar[Criteria.Type]  # value = <Type.DUMMY: 0>
        __members__: typing.ClassVar[dict[str, Criteria.Type]]  # value = {'DUMMY': <Type.DUMMY: 0>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    DUMMY: typing.ClassVar[Criteria.Type]  # value = <Type.DUMMY: 0>
    @property
    def default_render_type(self) -> RenderType:
        ...
    @property
    def is_read_only(self) -> bool:
        ...
    @property
    def name(self) -> str:
        ...
class DamageSource:
    """
    Represents a source of damage.
    """
    def __str__(self) -> str:
        ...
    @property
    def actor(self) -> Actor:
        """
        Get the actor that caused the damage to occur.
        """
    @property
    def damaging_actor(self) -> Actor:
        """
        Get the actor that directly caused the damage.
        """
    @property
    def is_indirect(self) -> bool:
        """
        Get if this damage is indirect.
        """
    @property
    def type(self) -> str:
        """
        Get the damage type.
        """
class Dimension:
    """
    Represents a dimension within a Level.
    """
    class Type:
        """
        Represents various dimension types.
        """
        CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
        NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
        OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
        THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>
        __members__: typing.ClassVar[dict[str, Dimension.Type]]  # value = {'OVERWORLD': <Type.OVERWORLD: 0>, 'NETHER': <Type.NETHER: 1>, 'THE_END': <Type.THE_END: 2>, 'CUSTOM': <Type.CUSTOM: 999>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    CUSTOM: typing.ClassVar[Dimension.Type]  # value = <Type.CUSTOM: 999>
    NETHER: typing.ClassVar[Dimension.Type]  # value = <Type.NETHER: 1>
    OVERWORLD: typing.ClassVar[Dimension.Type]  # value = <Type.OVERWORLD: 0>
    THE_END: typing.ClassVar[Dimension.Type]  # value = <Type.THE_END: 2>
    @typing.overload
    def get_block_at(self, location: Location) -> Block:
        """
        Gets the Block at the given Location
        """
    @typing.overload
    def get_block_at(self, x: int, y: int, z: int) -> Block:
        """
        Gets the Block at the given coordinates
        """
    @typing.overload
    def get_highest_block_at(self, location: Location) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given Location.
        """
    @typing.overload
    def get_highest_block_at(self, x: int, z: int) -> Block:
        """
        Gets the highest non-empty (impassable) block at the given coordinates.
        """
    def get_highest_block_y_at(self, x: int, z: int) -> int:
        """
        Gets the highest non-empty (impassable) coordinate at the given coordinates.
        """
    @property
    def level(self) -> Level:
        """
        Gets the level to which this dimension belongs
        """
    @property
    def loaded_chunks(self) -> list[Chunk]:
        """
        Gets a list of all loaded Chunks
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this dimension
        """
    @property
    def type(self) -> Dimension.Type:
        """
        Gets the type of this dimension
        """
class DimensionEvent(LevelEvent):
    """
    Represents events within a dimension
    """
    @property
    def dimension(self) -> Dimension:
        """
        Gets the dimension primarily involved with this event
        """
class DisplaySlot:
    """
    Locations for displaying objectives to the player
    """
    BELOW_NAME: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.BELOW_NAME: 0>
    PLAYER_LIST: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.PLAYER_LIST: 1>
    SIDE_BAR: typing.ClassVar[DisplaySlot]  # value = <DisplaySlot.SIDE_BAR: 2>
    __members__: typing.ClassVar[dict[str, DisplaySlot]]  # value = {'BELOW_NAME': <DisplaySlot.BELOW_NAME: 0>, 'PLAYER_LIST': <DisplaySlot.PLAYER_LIST: 1>, 'SIDE_BAR': <DisplaySlot.SIDE_BAR: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Divider:
    """
    Represents a divider.
    """
    def __init__(self) -> None:
        ...
class Dropdown:
    """
    Represents a dropdown with a set of predefined options.
    """
    def __init__(self, label: str | Translatable = '', options: list[str] | None = None, default_index: int | None = None) -> None:
        ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the dropdown.
        """
    @property
    def default_index(self) -> int | None:
        """
        Gets or sets the optional default index of the dropdown.
        """
    @default_index.setter
    def default_index(self, arg1: int | None) -> Dropdown:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the dropdown.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown:
        ...
    @property
    def options(self) -> list[str]:
        """
        Gets or sets the options of the dropdown.
        """
    @options.setter
    def options(self, arg1: list[str]) -> Dropdown:
        ...
class Enchantment:
    def can_enchant_item(self, item: ItemStack) -> bool:
        """
        Checks if this Enchantment may be applied to the given ItemStack.
        
        This does not check if it conflicts with any enchantments already applied to the item.
        """
    def conflicts_with(self, other: Enchantment) -> bool:
        """
        Check if this enchantment conflicts with another enchantment.
        """
    @property
    def key(self) -> NamespacedKey:
        """
        Return the namespaced identifier for this enchantment.
        """
    @property
    def max_level(self) -> int:
        """
        Gets the maximum level that this Enchantment may become.
        """
    @property
    def start_level(self) -> int:
        """
        Gets the level that this Enchantment should start at (also known as minimum level).
        """
    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
class EnchantmentRegistry:
    @typing.overload
    def __contains__(self, key: NamespacedKey) -> bool:
        ...
    @typing.overload
    def __contains__(self, key: str) -> bool:
        ...
    @typing.overload
    def __getitem__(self, key: NamespacedKey) -> Enchantment:
        ...
    @typing.overload
    def __getitem__(self, key: str) -> Enchantment:
        ...
    def __iter__(self) -> list:
        ...
    def get(self, key: NamespacedKey) -> Enchantment:
        ...
    def get_or_throw(self, key: NamespacedKey) -> Enchantment:
        ...
class EquipmentSlot:
    BODY: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.BODY: 6>
    CHEST: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.CHEST: 4>
    FEET: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.FEET: 2>
    HAND: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.HAND: 0>
    HEAD: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.HEAD: 5>
    LEGS: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.LEGS: 3>
    OFF_HAND: typing.ClassVar[EquipmentSlot]  # value = <EquipmentSlot.OFF_HAND: 1>
    __members__: typing.ClassVar[dict[str, EquipmentSlot]]  # value = {'HAND': <EquipmentSlot.HAND: 0>, 'OFF_HAND': <EquipmentSlot.OFF_HAND: 1>, 'FEET': <EquipmentSlot.FEET: 2>, 'LEGS': <EquipmentSlot.LEGS: 3>, 'CHEST': <EquipmentSlot.CHEST: 4>, 'HEAD': <EquipmentSlot.HEAD: 5>, 'BODY': <EquipmentSlot.BODY: 6>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Event:
    """
    Represents an event.
    """
    @property
    def event_name(self) -> str:
        """
        Gets a user-friendly identifier for this event.
        """
    @property
    def is_asynchronous(self) -> bool:
        """
        Whether the event fires asynchronously.
        """
class EventPriority:
    """
    Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR
    """
    HIGH: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGH: 3>
    HIGHEST: typing.ClassVar[EventPriority]  # value = <EventPriority.HIGHEST: 4>
    LOW: typing.ClassVar[EventPriority]  # value = <EventPriority.LOW: 1>
    LOWEST: typing.ClassVar[EventPriority]  # value = <EventPriority.LOWEST: 0>
    MONITOR: typing.ClassVar[EventPriority]  # value = <EventPriority.MONITOR: 5>
    NORMAL: typing.ClassVar[EventPriority]  # value = <EventPriority.NORMAL: 2>
    __members__: typing.ClassVar[dict[str, EventPriority]]  # value = {'LOWEST': <EventPriority.LOWEST: 0>, 'LOW': <EventPriority.LOW: 1>, 'NORMAL': <EventPriority.NORMAL: 2>, 'HIGH': <EventPriority.HIGH: 3>, 'HIGHEST': <EventPriority.HIGHEST: 4>, 'MONITOR': <EventPriority.MONITOR: 5>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class EventResult:
    ALLOW: typing.ClassVar[EventResult]  # value = <EventResult.ALLOW: 2>
    DEFAULT: typing.ClassVar[EventResult]  # value = <EventResult.DEFAULT: 1>
    DENY: typing.ClassVar[EventResult]  # value = <EventResult.DENY: 0>
    __members__: typing.ClassVar[dict[str, EventResult]]  # value = {'DENY': <EventResult.DENY: 0>, 'DEFAULT': <EventResult.DEFAULT: 1>, 'ALLOW': <EventResult.ALLOW: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class GameMode:
    """
    Represents the various type of game modes that Players may have.
    """
    ADVENTURE: typing.ClassVar[GameMode]  # value = <GameMode.ADVENTURE: 2>
    CREATIVE: typing.ClassVar[GameMode]  # value = <GameMode.CREATIVE: 1>
    SPECTATOR: typing.ClassVar[GameMode]  # value = <GameMode.SPECTATOR: 3>
    SURVIVAL: typing.ClassVar[GameMode]  # value = <GameMode.SURVIVAL: 0>
    __members__: typing.ClassVar[dict[str, GameMode]]  # value = {'SURVIVAL': <GameMode.SURVIVAL: 0>, 'CREATIVE': <GameMode.CREATIVE: 1>, 'ADVENTURE': <GameMode.ADVENTURE: 2>, 'SPECTATOR': <GameMode.SPECTATOR: 3>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Header:
    """
    Represents a header with a label.
    """
    def __init__(self, label: str | Translatable = '') -> None:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the header.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> Header:
        ...
class Inventory:
    """
    Interface to the various inventories.
    """
    @typing.overload
    def __contains__(self, item: ItemStack) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemStack.
        """
    @typing.overload
    def __contains__(self, type: str) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType.
        """
    def __getitem__(self, index: int) -> ItemStack:
        """
        Returns the ItemStack found in the slot at the given index
        """
    def __len__(self) -> int:
        """
        Returns the size of the inventory
        """
    def __setitem__(self, index: int, item: ItemStack) -> None:
        """
        Stores the ItemStack at the given index of the inventory.
        """
    def add_item(self, *args) -> dict[int, ItemStack]:
        """
        Stores the given ItemStacks in the inventory.
        This will try to fill existing stacks and empty slots as well as it can.
        
        The returned map contains what it couldn't store, where the key is the index, and the value is the ItemStack.
        If all items are stored, it will return an empty dict.
        """
    @typing.overload
    def all(self, item: ItemStack) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemStack.
        This will only match slots if both the type and the amount of the stack match
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
    @typing.overload
    def all(self, type: str) -> dict[int, ItemStack]:
        """
        Finds all slots in the inventory containing any ItemStacks with the given ItemType.
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
    @typing.overload
    def clear(self, index: int) -> None:
        """
        Clears out a particular slot in the index.
        """
    @typing.overload
    def clear(self) -> None:
        """
        Clears out the whole Inventory.
        """
    @typing.overload
    def contains(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains at least the minimum amount specified of exactly matching ItemStacks.
        An ItemStack only counts if both the type and the amount of the stack match.
        """
    @typing.overload
    def contains(self, item: ItemStack) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemStack.
        This will only return true if both the type and the amount of the stack match.
        """
    @typing.overload
    def contains(self, type: str) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType.
        """
    @typing.overload
    def contains_at_least(self, item: ItemStack, amount: int) -> bool:
        """
        Checks if the inventory contains ItemStacks matching the given ItemStack whose amounts sum to at least the minimum amount specified.
        """
    @typing.overload
    def contains_at_least(self, type: str, amount: int) -> bool:
        """
        Checks if the inventory contains any ItemStacks with the given ItemType, adding to at least the minimum amount specified.
        """
    @typing.overload
    def first(self, item: ItemStack) -> int:
        """
        Returns the first slot in the inventory containing an ItemStack with the given stack.
        This will only match slots if both the type and the amount of the stack match
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
    @typing.overload
    def first(self, type: str) -> int:
        """
        Finds the first slot in the inventory containing an ItemStack with the given ItemType.
        The returned map contains entries where, the key is the slot index, and the value is the ItemStack in that slot. If no matching ItemStack is found, an empty dict is returned.
        """
    def get_item(self, index: int) -> ItemStack:
        """
        Returns the ItemStack found in the slot at the given index
        """
    @typing.overload
    def remove(self, item: ItemStack) -> None:
        """
        Removes all stacks in the inventory matching the given stack.
        This will only match a slot if both the type and the amount of the stack match
        """
    @typing.overload
    def remove(self, type: str) -> None:
        """
        Removes all stacks in the inventory matching the given ItemType.
        """
    def remove_item(self, *args) -> dict[int, ItemStack]:
        """
        Removes the given ItemStacks from the inventory.
        It will try to remove 'as much as possible' from the types and amounts you give as arguments.
        
        The returned HashMap contains what it couldn't remove, where the key is the index, and the value is the ItemStack.
        If all the given ItemStacks are removed, it will return an empty dict.
        """
    def set_item(self, index: int, item: ItemStack) -> None:
        """
        Stores the ItemStack at the given index of the inventory.
        """
    @property
    def contents(self) -> list[ItemStack]:
        """
        Returns all ItemStacks from the inventory
        """
    @contents.setter
    def contents(self, arg1: list[ItemStack]) -> None:
        ...
    @property
    def first_empty(self) -> int:
        """
        Returns the first empty Slot.
        """
    @property
    def is_empty(self) -> bool:
        """
         Check whether this inventory is empty. An inventory is considered to be empty if there are no ItemStacks in any slot of this inventory.
        """
    @property
    def max_stack_size(self) -> int:
        """
        Returns the maximum stack size for an ItemStack in this inventory.
        """
    @property
    def size(self) -> int:
        """
        Returns the size of the inventory
        """
class IpBanEntry(BanEntry):
    """
    Represents a ban entry for an IP address.
    """
    def __init__(self, address: str) -> None:
        ...
    @property
    def address(self) -> str:
        """
        Gets the banned IP address.
        """
class IpBanList:
    """
    Represents a ban list containing banned IP addresses.
    """
    def add_ban(self, address: str, reason: str | None = None, expires: datetime.datetime | None = None, source: str | None = None) -> IpBanEntry:
        """
        Adds a ban to this list, or updates an existing one.
        """
    def get_ban_entry(self, address: str) -> IpBanEntry:
        """
        Gets a BanEntry by IP address.
        """
    def is_banned(self, address: str) -> bool:
        """
        Checks if a BanEntry exists for the target by IP address.
        """
    def remove_ban(self, address: str) -> None:
        """
        Removes an IP address from the ban list.
        """
    @property
    def entries(self) -> list[IpBanEntry]:
        """
        Gets a vector of pointers to entries in the ban list.
        """
class Item(Actor):
    """
    Represents a base actor in the level.
    """
    @property
    def is_unlimited_lifetime(self) -> bool:
        """
        Gets or sets if this Item lives forever
        """
    @is_unlimited_lifetime.setter
    def is_unlimited_lifetime(self, arg1: bool) -> None:
        ...
    @property
    def item_stack(self) -> ItemStack:
        """
        Gets or sets the item stack associated with this item drop.
        """
    @item_stack.setter
    def item_stack(self, arg1: ItemStack) -> None:
        ...
    @property
    def pickup_delay(self) -> int:
        """
        Gets or sets the delay before this Item is available to be picked up by players.
        """
    @pickup_delay.setter
    def pickup_delay(self, arg1: int) -> None:
        ...
    @property
    def thrower(self) -> int | None:
        """
        Gets or sets the thrower of this item.
        """
    @thrower.setter
    def thrower(self, arg1: int | None) -> None:
        ...
class ItemFactory:
    def as_meta_for(self, meta: ItemMeta, type: ItemType) -> ItemMeta:
        """
        Returns an appropriate item meta for the specified item type.
        """
    def equals(self, meta1: ItemMeta, meta2: ItemMeta) -> bool:
        """
        This method is used to compare two ItemMeta objects.
        """
    def get_item_meta(self, type: ItemType) -> ItemMeta:
        """
        This creates a new item meta for the item type.
        """
    def is_applicable(self, meta: ItemMeta, type: ItemType) -> bool:
        """
        This method checks the item meta to confirm that it is applicable (no data lost if applied) to the specified ItemStack
        """
class ItemMeta:
    """
    Represents the metadata of a generic item.
    """
    def add_enchant(self, id: str, level: int, force: bool = False) -> bool:
        """
        Adds the specified enchantment to this item meta.
        """
    def clone(self) -> ItemMeta:
        """
        Creates a clone of the current metadata.
        """
    def get_enchant_level(self, id: str) -> int:
        """
        Checks for the level of the specified enchantment.
        """
    def has_enchant(self, id: str) -> bool:
        """
        Checks for existence of the specified enchantment.
        """
    def remove_enchant(self, id: str) -> bool:
        """
        Removes the specified enchantment from this item meta.
        """
    def remove_enchants(self) -> None:
        """
        Removes all enchantments from this item meta.
        """
    @property
    def damage(self) -> int:
        """
        Gets or sets the damage.
        """
    @damage.setter
    def damage(self, arg1: int) -> None:
        ...
    @property
    def display_name(self) -> str | None:
        """
        Gets or sets the display name.
        """
    @display_name.setter
    def display_name(self, arg1: str | None) -> None:
        ...
    @property
    def enchants(self) -> dict[str, int]:
        """
        Returns a copy the enchantments in this ItemMeta.
        """
    @property
    def has_damage(self) -> bool:
        """
        Checks to see if this item has damage.
        """
    @property
    def has_display_name(self) -> bool:
        """
        Checks for existence of a display name.
        """
    @property
    def has_enchants(self) -> bool:
        """
        Checks for the existence of any enchantments.
        """
    @property
    def has_lore(self) -> bool:
        """
        Checks for existence of lore.
        """
    @property
    def has_repair_cost(self) -> bool:
        """
        Checks to see if this item has a repair penalty.
        """
    @property
    def is_unbreakable(self) -> bool:
        """
        Gets or sets the unbreakable tag. An unbreakable item will not lose durability.
        """
    @is_unbreakable.setter
    def is_unbreakable(self, arg1: bool) -> None:
        ...
    @property
    def lore(self) -> list[str] | None:
        """
        Gets or sets the lore for this item.
        """
    @lore.setter
    def lore(self, arg1: list[str] | None) -> None:
        ...
    @property
    def repair_cost(self) -> int:
        """
        Gets or sets the repair penalty.
        """
    @repair_cost.setter
    def repair_cost(self, arg1: int) -> None:
        ...
class ItemRegistry:
    @typing.overload
    def __contains__(self, key: NamespacedKey) -> bool:
        ...
    @typing.overload
    def __contains__(self, key: str) -> bool:
        ...
    @typing.overload
    def __getitem__(self, key: NamespacedKey) -> ItemType:
        ...
    @typing.overload
    def __getitem__(self, key: str) -> ItemType:
        ...
    def __iter__(self) -> list:
        ...
    def get(self, key: NamespacedKey) -> ItemType:
        ...
    def get_or_throw(self, key: NamespacedKey) -> ItemType:
        ...
class ItemStack:
    """
    Represents a stack of items.
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, arg0: ItemStack) -> bool:
        ...
    def __init__(self, type: str, amount: int = 1, data: int = 0) -> None:
        ...
    def __ne__(self, arg0: ItemStack) -> bool:
        ...
    def __str__(self) -> str:
        ...
    def is_similar(self, other: ItemStack) -> bool:
        """
        Checks if the two stacks are equal, but does not consider stack size (amount).
        """
    def set_item_meta(self, meta: ItemMeta) -> bool:
        """
        Set the ItemMeta of this ItemStack.
        """
    @property
    def amount(self) -> int:
        """
        Gets or sets the amount of items in this stack.
        """
    @amount.setter
    def amount(self, arg1: int) -> None:
        ...
    @property
    def data(self) -> int:
        """
        Gets or sets the data for this stack of items.
        """
    @data.setter
    def data(self, arg1: int) -> None:
        ...
    @property
    def item_meta(self) -> ItemMeta:
        """
        Gets a copy of the ItemMeta of this ItemStack.
        """
    @property
    def max_stack_size(self) -> int:
        """
        Get the maximum stack size for this item.
        """
    @property
    def type(self) -> ItemType:
        """
        Gets or sets the type of this item.
        """
    @type.setter
    def type(self, arg1: str) -> None:
        ...
class ItemType:
    """
    Represents an item type.
    """
    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def get(name: str) -> ItemType:
        """
        Attempts to get the ItemType with the given name.
        """
    @typing.overload
    def __eq__(self, arg0: ItemType) -> bool:
        ...
    @typing.overload
    def __eq__(self, arg0: str) -> bool:
        ...
    @typing.overload
    def __ne__(self, arg0: ItemType) -> bool:
        ...
    @typing.overload
    def __ne__(self, arg0: str) -> bool:
        ...
    def __str__(self) -> str:
        ...
    @property
    def id(self) -> str:
        """
        Return the identifier of this item type.
        """
    @property
    def key(self) -> NamespacedKey:
        """
        Return the namespaced identifier of this item type.
        """
    @property
    def max_durability(self) -> int:
        """
        Gets the maximum durability of this item type
        """
    @property
    def max_stack_size(self) -> int:
        """
        Gets the maximum amount of this item type that can be held in a stack.
        """
    @property
    def translation_key(self) -> str:
        """
        Get the translation key, suitable for use in a translation component.
        """
class Label:
    """
    Represents a text label.
    """
    def __init__(self, text: str | Translatable = '') -> None:
        ...
    @property
    def text(self) -> str | Translatable:
        """
        Gets or sets the text of the label.
        """
    @text.setter
    def text(self, arg1: str | Translatable) -> Label:
        ...
class Language:
    """
    Represents the interface for translating text into different languages.
    """
    @typing.overload
    def translate(self, text: str, params: list[str] | None = None, locale: str | None = None) -> str:
        """
        Translates a given text using a set of parameters for a specific locale.
        """
    @typing.overload
    def translate(self, translatable: Translatable, locale: str | None = None) -> str:
        """
        Translates a Translatable object into a specific locale.
        """
    @property
    def locale(self) -> str:
        """
        Gets the current locale.
        """
class LeavesDecayEvent(BlockEvent, Cancellable):
    """
    Called when leaves are decaying naturally.
    If a Leaves Decay event is cancelled, the leaves will not decay.
    """
class Level:
    def get_dimension(self, name: str) -> Dimension:
        """
        Gets the dimension with the given name.
        """
    @property
    def actors(self) -> list[Actor]:
        """
        Get a list of all actors in this level
        """
    @property
    def dimensions(self) -> list[Dimension]:
        """
        Gets a list of all dimensions within this level.
        """
    @property
    def name(self) -> str:
        """
        Gets the unique name of this level
        """
    @property
    def seed(self) -> int:
        """
        Gets the Seed for this level.
        """
    @property
    def time(self) -> int:
        """
        Gets and sets the relative in-game time on the server
        """
    @time.setter
    def time(self, arg1: int) -> None:
        ...
class LevelEvent(Event):
    """
    Represents events within a level
    """
    @property
    def level(self) -> Level:
        """
        Gets the level primarily involved with this event
        """
class Location(Position):
    """
    Represents a 3-dimensional location in a dimension within a level.
    """
    def __init__(self, dimension: Dimension, x: float, y: float, z: float, pitch: float = 0.0, yaw: float = 0.0) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def pitch(self) -> float:
        """
        The pitch of this location, measured in degrees.
        """
    @pitch.setter
    def pitch(self, arg1: float) -> None:
        ...
    @property
    def yaw(self) -> float:
        """
        The yaw of this location, measured in degrees.
        """
    @yaw.setter
    def yaw(self, arg1: float) -> None:
        ...
class Logger:
    """
    Logger class which can format and output varies levels of logs.
    """
    class Level:
        """
        Specifies the log level.
        """
        CRITICAL: typing.ClassVar[Logger.Level]  # value = <Level.CRITICAL: 5>
        DEBUG: typing.ClassVar[Logger.Level]  # value = <Level.DEBUG: 1>
        ERROR: typing.ClassVar[Logger.Level]  # value = <Level.ERROR: 4>
        INFO: typing.ClassVar[Logger.Level]  # value = <Level.INFO: 2>
        TRACE: typing.ClassVar[Logger.Level]  # value = <Level.TRACE: 0>
        WARNING: typing.ClassVar[Logger.Level]  # value = <Level.WARNING: 3>
        __members__: typing.ClassVar[dict[str, Logger.Level]]  # value = {'TRACE': <Level.TRACE: 0>, 'DEBUG': <Level.DEBUG: 1>, 'INFO': <Level.INFO: 2>, 'WARNING': <Level.WARNING: 3>, 'ERROR': <Level.ERROR: 4>, 'CRITICAL': <Level.CRITICAL: 5>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    CRITICAL: typing.ClassVar[Logger.Level]  # value = <Level.CRITICAL: 5>
    DEBUG: typing.ClassVar[Logger.Level]  # value = <Level.DEBUG: 1>
    ERROR: typing.ClassVar[Logger.Level]  # value = <Level.ERROR: 4>
    INFO: typing.ClassVar[Logger.Level]  # value = <Level.INFO: 2>
    TRACE: typing.ClassVar[Logger.Level]  # value = <Level.TRACE: 0>
    WARNING: typing.ClassVar[Logger.Level]  # value = <Level.WARNING: 3>
    def critical(self, message: str) -> None:
        """
        Log a message at the CRITICAL level.
        """
    def debug(self, message: str) -> None:
        """
        Log a message at the DEBUG level.
        """
    def error(self, message: str) -> None:
        """
        Log a message at the ERROR level.
        """
    def info(self, message: str) -> None:
        """
        Log a message at the INFO level.
        """
    def is_enabled_for(self, level: Logger.Level) -> bool:
        """
        Check if the Logger instance is enabled for the given log Level.
        """
    def set_level(self, level: Logger.Level) -> None:
        """
        Set the logging level for this Logger instance.
        """
    def trace(self, message: str) -> None:
        """
        Log a message at the TRACE level.
        """
    def warning(self, message: str) -> None:
        """
        Log a message at the WARNING level.
        """
    @property
    def name(self) -> str:
        """
        Get the name of this Logger instance.
        """
class MapCanvas:
    """
    Represents a canvas for drawing to a map. Each canvas is associated with a specific MapRenderer and represents that renderer's layer on the map.
    """
    def draw_image(self, x: int, y: int, image: numpy.ndarray[numpy.uint8]) -> None:
        """
        Draw an image to the map. The image will be clipped if necessary.
        """
    def get_base_pixel_color(self, x: int, y: int) -> tuple[int, ...]:
        """
        Get a pixel from the layers below this canvas.
        """
    def get_pixel_color(self, x: int, y: int) -> tuple[int, ...] | None:
        """
        Get a pixel from the canvas.
        
        If no color is set at the given position for this canvas, then None is returned and the color returned by get_base_pixel_color() is shown on the map
        """
    def set_pixel_color(self, x: int, y: int, color: tuple[int, ...] | None) -> None:
        """
        Draw a pixel to the canvas.
        
        If None is used as color, then the color returned by get_base_pixel_color() is shown on the map.
        """
    @property
    def map_view(self) -> MapView:
        """
        Get the map this canvas is attached to.
        """
class MapMeta(ItemMeta):
    """
    Represents the metadata for a map item.
    """
class MapRenderer:
    """
    Represents a renderer for a map.
    """
    def __init__(self, is_contextual: bool = False) -> None:
        """
        Initialize the map renderer base with the given contextual status.
        """
    def initialize(self, view: MapView) -> None:
        """
        Initialize this MapRenderer for the given map.
        """
    def render(self, map: MapView, canvas: MapCanvas, player: Player) -> None:
        """
        Render to the given map.
        """
class MapView:
    """
    Represents a map item.
    """
    class Scale:
        """
        An enum representing all possible scales a map can be set to.
        """
        CLOSE: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSE: 1>
        CLOSEST: typing.ClassVar[MapView.Scale]  # value = <Scale.CLOSEST: 0>
        FAR: typing.ClassVar[MapView.Scale]  # value = <Scale.FAR: 3>
        FARTHEST: typing.ClassVar[MapView.Scale]  # value = <Scale.FARTHEST: 4>
        NORMAL: typing.ClassVar[MapView.Scale]  # value = <Scale.NORMAL: 2>
        __members__: typing.ClassVar[dict[str, MapView.Scale]]  # value = {'CLOSEST': <Scale.CLOSEST: 0>, 'CLOSE': <Scale.CLOSE: 1>, 'NORMAL': <Scale.NORMAL: 2>, 'FAR': <Scale.FAR: 3>, 'FARTHEST': <Scale.FARTHEST: 4>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    def add_renderer(self, renderer: MapRenderer) -> None:
        """
        Add a renderer to this map.
        """
    def remove_renderer(self, renderer: MapRenderer) -> bool:
        """
        Remove a renderer from this map.
        """
    @property
    def center_x(self) -> int:
        """
        Get or set the center X position of this map.
        """
    @center_x.setter
    def center_x(self, arg1: int) -> None:
        ...
    @property
    def center_z(self) -> int:
        """
        Get or set the center Z position of this map.
        """
    @center_z.setter
    def center_z(self, arg1: int) -> None:
        ...
    @property
    def dimension(self) -> Dimension:
        """
        Get or set the dimension that this map is associated with.
        """
    @dimension.setter
    def dimension(self, arg1: Dimension) -> None:
        ...
    @property
    def id(self) -> int:
        """
        Get the ID of this map item for use with MapMeta.
        """
    @property
    def is_unlimited_tracking(self) -> bool:
        """
        Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range.
        """
    @is_unlimited_tracking.setter
    def is_unlimited_tracking(self, arg1: bool) -> None:
        ...
    @property
    def is_virtual(self) -> bool:
        """
        Check whether this map is virtual.
        """
    @property
    def locked(self) -> bool:
        """
        Whether the map is locked or not. A locked map may not be explored further.
        """
    @locked.setter
    def locked(self, arg1: bool) -> None:
        ...
    @property
    def renderers(self) -> list[MapRenderer]:
        """
        Get a copied list of MapRenderers currently in effect.
        """
    @property
    def scale(self) -> MapView.Scale:
        """
        Get or set the scale of this map.
        """
    @scale.setter
    def scale(self, arg1: MapView.Scale) -> None:
        ...
class MessageForm:
    """
    Represents a form with two buttons.
    """
    def __init__(self, title: str | Translatable = '', content: str | Translatable = '', button1: str | Translatable = '', button2: str | Translatable = '', on_submit: typing.Callable[[Player, int], None] = None, on_close: typing.Callable[[Player], None] = None) -> None:
        ...
    @property
    def button1(self) -> str | Translatable:
        """
        Gets or sets the text of button1.
        """
    @button1.setter
    def button1(self, arg1: str | Translatable) -> MessageForm:
        ...
    @property
    def button2(self) -> str | Translatable:
        """
        Gets or sets the text of button2.
        """
    @button2.setter
    def button2(self, arg1: str | Translatable) -> MessageForm:
        ...
    @property
    def content(self) -> str | Translatable:
        """
        Gets or sets the content of the form.
        """
    @content.setter
    def content(self, arg1: str | Translatable) -> MessageForm:
        ...
    @property
    def on_close(self) -> typing.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """
    @on_close.setter
    def on_close(self, arg1: typing.Callable[[Player], None]) -> MessageForm:
        ...
    @property
    def on_submit(self) -> typing.Callable[[Player, int], None]:
        """
        Gets or sets the on submit callback.
        """
    @on_submit.setter
    def on_submit(self, arg1: typing.Callable[[Player, int], None]) -> MessageForm:
        ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """
    @title.setter
    def title(self, arg1: str | Translatable) -> MessageForm:
        ...
class Mob(Actor):
    """
    Represents a mobile entity (i.e. living entity), such as a monster or player.
    """
    @property
    def is_gliding(self) -> bool:
        """
        Checks to see if an actor is gliding, such as using an Elytra.
        """
class MobEvent(Event):
    """
    Represents an Mob-related event.
    """
    @property
    def actor(self) -> Mob:
        """
        Returns the Mob involved in this event
        """
class ModalForm:
    """
    Represents a modal form with controls.
    """
    def __init__(self, title: str | Translatable = '', controls: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header] | None = None, submit_button: str | Translatable | None = None, icon: str | None = None, on_submit: typing.Callable[[Player, str], None] = None, on_close: typing.Callable[[Player], None] = None) -> None:
        ...
    def add_control(self, control: Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header) -> ModalForm:
        """
        Adds a control to the form.
        """
    @property
    def controls(self) -> list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header]:
        """
        Gets or sets the controls of the modal form.
        """
    @controls.setter
    def controls(self, arg1: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle | Divider | Header]) -> ModalForm:
        ...
    @property
    def icon(self) -> str | None:
        """
        Gets or sets the icon of the form.
        """
    @icon.setter
    def icon(self, arg1: str | None) -> ModalForm:
        ...
    @property
    def on_close(self) -> typing.Callable[[Player], None]:
        """
        Gets or sets the on close callback.
        """
    @on_close.setter
    def on_close(self, arg1: typing.Callable[[Player], None]) -> ModalForm:
        ...
    @property
    def on_submit(self) -> typing.Callable[[Player, str], None]:
        """
        Gets or sets the on submit callback.
        """
    @on_submit.setter
    def on_submit(self, arg1: typing.Callable[[Player, str], None]) -> ModalForm:
        ...
    @property
    def submit_button(self) -> str | Translatable | None:
        """
        Gets or sets the submit button message of the form.
        """
    @submit_button.setter
    def submit_button(self, arg1: str | Translatable | None) -> ModalForm:
        ...
    @property
    def title(self) -> str | Translatable:
        """
        Gets or sets the title of the form.
        """
    @title.setter
    def title(self, arg1: str | Translatable) -> ModalForm:
        ...
class NamespacedKey:
    """
    Represents a string-based key which consists of two components - a namespace and a key.
    """
    @staticmethod
    def from_string(input: str, plugin: Plugin = None) -> NamespacedKey:
        """
        Parses a NamespacedKey from a string.
        """
    def __init__(self, arg0: Plugin, arg1: str) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def key(self) -> str:
        """
        Returns the key of the NamespacedKey.
        """
    @property
    def namespace(self) -> str:
        """
        Returns the namespace of the NamespacedKey.
        """
class Objective:
    """
    Represents an objective on a scoreboard that can show scores specific to entries.
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, arg0: Objective) -> bool:
        ...
    def __ne__(self, arg0: Objective) -> bool:
        ...
    def get_score(self, entry: Player | Actor | str) -> Score:
        """
        Gets an entry's Score for this objective
        """
    def set_display(self, slot: DisplaySlot | None, order: ObjectiveSortOrder | None = None) -> None:
        """
        Sets the display slot and sort order for this objective. This will remove it from any other display slot.
        """
    def unregister(self) -> None:
        """
        Unregisters this objective from the associated Scoreboard.
        """
    @property
    def criteria(self) -> Criteria:
        """
        Gets the criteria this objective tracks
        """
    @property
    def display_name(self) -> str:
        """
        Gets or sets the name displayed to players for this objective
        """
    @display_name.setter
    def display_name(self, arg1: str) -> None:
        ...
    @property
    def display_slot(self) -> DisplaySlot | None:
        """
        Gets or sets the display slot this objective is displayed at
        """
    @display_slot.setter
    def display_slot(self, arg1: DisplaySlot | None) -> None:
        ...
    @property
    def is_displayed(self) -> bool:
        """
        Gets if the objective is currently displayed in a slot.
        """
    @property
    def is_modifiable(self) -> bool:
        """
        Gets if the objective's scores can be modified directly by a plugin
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this Objective
        """
    @property
    def render_type(self) -> RenderType:
        """
        Gets the manner in which this objective will be rendered.
        """
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the scoreboard to which this objective is attached
        """
    @property
    def sort_order(self) -> ObjectiveSortOrder | None:
        """
        Gets or sets the sort order for this objective
        """
    @sort_order.setter
    def sort_order(self, arg1: ObjectiveSortOrder) -> None:
        ...
class ObjectiveSortOrder:
    """
    Represents the sort order of objectives on a DisplaySlot.
    """
    ASCENDING: typing.ClassVar[ObjectiveSortOrder]  # value = <ObjectiveSortOrder.ASCENDING: 0>
    DESCENDING: typing.ClassVar[ObjectiveSortOrder]  # value = <ObjectiveSortOrder.DESCENDING: 1>
    __members__: typing.ClassVar[dict[str, ObjectiveSortOrder]]  # value = {'ASCENDING': <ObjectiveSortOrder.ASCENDING: 0>, 'DESCENDING': <ObjectiveSortOrder.DESCENDING: 1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class OfflinePlayer:
    """
    Represents a reference to a player identity and the data belonging to a player that is stored on the disk and can, thus, be retrieved without the player needing to be online.
    """
    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
class PacketReceiveEvent(ServerEvent, Cancellable):
    """
    Called when the server receives a packet from a connected client.
    """
    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """
    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """
    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """
    @payload.setter
    def payload(self, arg1: bytes) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """
    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """
class PacketSendEvent(ServerEvent, Cancellable):
    """
    Called when the server sends a packet to a connected client.
    """
    @property
    def address(self) -> SocketAddress:
        """
        Gets the network address to which this packet is being sent.
        """
    @property
    def packet_id(self) -> int:
        """
        Gets the ID of the packet.
        """
    @property
    def payload(self) -> bytes:
        """
        Gets or sets the raw packet data **excluding** the header.
        """
    @payload.setter
    def payload(self, arg1: bytes) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Gets the player involved in this event
        NOTE: This may return None if the packet is sent before the player completes the login process.
        """
    @property
    def sub_client_id(self) -> int:
        """
        Gets the SubClient ID (0 = primary client; 1-3 = split-screen clients).
        """
class Permissible:
    """
    Represents an object that may become a server operator and can be assigned permissions.
    """
    @typing.overload
    def add_attachment(self, plugin: Plugin, name: str, value: bool) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """
    @typing.overload
    def add_attachment(self, plugin: Plugin) -> PermissionAttachment:
        """
        Adds a new PermissionAttachment.
        """
    @typing.overload
    def has_permission(self, name: str) -> bool:
        """
        Checks if a permissions is available by name.
        """
    @typing.overload
    def has_permission(self, perm: Permission) -> bool:
        """
        Checks if a permissions is available by permission.
        """
    @typing.overload
    def is_permission_set(self, name: str) -> bool:
        """
        Checks if a permissions is set by name.
        """
    @typing.overload
    def is_permission_set(self, perm: Permission) -> bool:
        """
        Checks if a permissions is set by permission.
        """
    def recalculate_permissions(self) -> None:
        """
        Recalculates the permissions.
        """
    def remove_attachment(self, attachment: PermissionAttachment) -> None:
        """
        Removes a given PermissionAttachment.
        """
    @property
    def effective_permissions(self) -> set[PermissionAttachmentInfo]:
        """
        Gets effective permissions.
        """
    @property
    def permission_level(self) -> PermissionLevel:
        """
        Gets the permission level of this object
        """
class Permission:
    """
    Represents a unique permission that may be attached to a Permissible
    """
    def __init__(self, name: str, description: str | None = None, default: PermissionDefault | None = None, children: dict[str, bool] | None = None, *args, **kwargs) -> None:
        ...
    @typing.overload
    def add_parent(self, name: str, value: bool) -> Permission:
        """
        Adds this permission to the specified parent permission.
        """
    @typing.overload
    def add_parent(self, perm: Permission, value: bool) -> None:
        """
        Adds this permission to the specified parent permission.
        """
    def recalculate_permissibles(self) -> None:
        """
        Recalculates all Permissibles that contain this permission.
        """
    @property
    def children(self) -> dict[str, bool]:
        """
        Gets the children of this permission.
        """
    @property
    def default(self) -> PermissionDefault:
        """
        The default value of this permission.
        """
    @default.setter
    def default(self, arg1: PermissionDefault) -> None:
        ...
    @property
    def description(self) -> str:
        """
        The brief description of this permission
        """
    @description.setter
    def description(self, arg1: str) -> None:
        ...
    @property
    def name(self) -> str:
        """
        Gets the unique fully qualified name of this Permission.
        """
    @property
    def permissibles(self) -> set[Permissible]:
        """
        Gets a set containing every Permissible that has this permission.
        """
class PermissionAttachment:
    """
    Holds information about a permission attachment on a Permissible object
    """
    def __init__(self, plugin: Plugin, permissible: Permissible) -> None:
        ...
    def remove(self) -> bool:
        """
        Removes this attachment from its registered Permissible.
        """
    @typing.overload
    def set_permission(self, name: str, value: bool) -> None:
        """
        Sets a permission to the given value, by its fully qualified name.
        """
    @typing.overload
    def set_permission(self, perm: Permission, value: bool) -> None:
        """
        Sets a permission to the given value.
        """
    @typing.overload
    def unset_permission(self, name: str) -> None:
        """
        Removes the specified permission from this attachment by name.
        """
    @typing.overload
    def unset_permission(self, perm: Permission) -> None:
        """
        Removes the specified permission from this attachment.
        """
    @property
    def permissible(self) -> Permissible:
        """
        Gets the Permissible that this is attached to.
        """
    @property
    def permissions(self) -> dict[str, bool]:
        """
        Gets a copy of all set permissions and values contained within this attachment.
        """
    @property
    def plugin(self) -> Plugin:
        """
        Gets the plugin responsible for this attachment.
        """
    @property
    def removal_callback(self) -> typing.Callable[[PermissionAttachment], None]:
        """
        The callback to be called when this attachment is removed.
        """
    @removal_callback.setter
    def removal_callback(self, arg1: typing.Callable[[PermissionAttachment], None]) -> None:
        ...
class PermissionAttachmentInfo:
    """
    Holds information on a permission and which PermissionAttachment provides it
    """
    def __init__(self, permissible: Permissible, permission: str, attachment: PermissionAttachment, value: bool) -> None:
        ...
    @property
    def attachment(self) -> PermissionAttachment:
        """
        Gets the attachment providing this permission.
        """
    @property
    def permissible(self) -> Permissible:
        """
        Get the permissible this is attached to
        """
    @property
    def permission(self) -> str:
        """
        Gets the permission being set
        """
    @property
    def value(self) -> bool:
        """
        Gets the value of this permission
        """
class PermissionDefault:
    """
    Represents the possible default values for permissions
    """
    CONSOLE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.CONSOLE: 4>
    FALSE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.FALSE: 1>
    NOT_OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    NOT_OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    TRUE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.TRUE: 0>
    __members__: typing.ClassVar[dict[str, PermissionDefault]]  # value = {'TRUE': <PermissionDefault.TRUE: 0>, 'FALSE': <PermissionDefault.FALSE: 1>, 'OP': <PermissionDefault.OP: 2>, 'OPERATOR': <PermissionDefault.OP: 2>, 'NOT_OP': <PermissionDefault.NOT_OP: 3>, 'NOT_OPERATOR': <PermissionDefault.NOT_OP: 3>, 'CONSOLE': <PermissionDefault.CONSOLE: 4>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class PermissionLevel:
    CONSOLE: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.CONSOLE: 2>
    DEFAULT: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.DEFAULT: 0>
    OP: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.OP: 1>
    OPERATOR: typing.ClassVar[PermissionLevel]  # value = <PermissionLevel.OP: 1>
    __members__: typing.ClassVar[dict[str, PermissionLevel]]  # value = {'DEFAULT': <PermissionLevel.DEFAULT: 0>, 'OP': <PermissionLevel.OP: 1>, 'OPERATOR': <PermissionLevel.OP: 1>, 'CONSOLE': <PermissionLevel.CONSOLE: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Player(Mob, OfflinePlayer):
    """
    Represents a player.
    """
    def close_form(self) -> None:
        """
        Closes the forms that are currently open for the player.
        """
    def give_exp(self, amount: int) -> None:
        """
        Gives the player the amount of experience specified.
        """
    def give_exp_levels(self, amount: int) -> None:
        """
        Gives the player the amount of experience levels specified.
        """
    def kick(self, message: str) -> None:
        """
        Kicks player with custom kick message.
        """
    def perform_command(self, command: str) -> bool:
        """
        Makes the player perform the given command.
        """
    def play_sound(self, location: Location, sound: str, volume: float = 1.0, pitch: float = 1.0) -> None:
        """
        Play a sound for a player at the location.
        """
    def reset_title(self) -> None:
        """
        Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values.
        """
    def send_form(self, form: MessageForm | ActionForm | ModalForm) -> None:
        """
        Sends a form to the player.
        """
    def send_packet(self, packet_id: int, payload: bytes) -> None:
        """
        Sends a packet to the player.
        """
    def send_popup(self, message: str) -> None:
        """
        Sends this player a popup message
        """
    def send_tip(self, message: str) -> None:
        """
        Sends this player a tip message
        """
    def send_title(self, title: str, subtitle: str, fade_in: int = 10, stay: int = 70, fade_out: int = 20) -> None:
        """
        Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such.
        """
    def send_toast(self, title: str, content: str) -> None:
        """
        Sends this player a toast notification.
        """
    @typing.overload
    def spawn_particle(self, name: str, location: Location, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.
        """
    @typing.overload
    def spawn_particle(self, name: str, x: float, y: float, z: float, molang_variables_json: str | None = None) -> None:
        """
        Spawns the particle at the target location.
        """
    def stop_all_sounds(self) -> None:
        """
        Stop all sounds from playing.
        """
    def stop_sound(self, sound: str) -> None:
        """
        Stop the specified sound from playing.
        """
    def transfer(self, host: str, port: int = 19132) -> None:
        """
        Transfers the player to another server.
        """
    def update_commands(self) -> None:
        """
        Send the list of commands to the client.
        """
    @property
    def address(self) -> SocketAddress:
        """
        Gets the socket address of this player
        """
    @property
    def allow_flight(self) -> bool:
        """
        If the Player is allowed to fly via jump key double-tap.
        """
    @allow_flight.setter
    def allow_flight(self, arg1: bool) -> None:
        ...
    @property
    def device_id(self) -> str:
        """
        Get the player's current device id.
        """
    @property
    def device_os(self) -> str:
        """
        Get the player's current device's operation system (OS).
        """
    @property
    def ender_chest(self) -> Inventory:
        """
        Get the player's EnderChest inventory.
        """
    @property
    def exp_level(self) -> int:
        """
        Gets or sets the players current experience level.
        """
    @exp_level.setter
    def exp_level(self, arg1: int) -> None:
        ...
    @property
    def exp_progress(self) -> float:
        """
        Gets or sets the players current experience progress towards the next level.
        """
    @exp_progress.setter
    def exp_progress(self, arg1: float) -> None:
        ...
    @property
    def fly_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can fly.
        """
    @fly_speed.setter
    def fly_speed(self, arg1: float) -> None:
        ...
    @property
    def game_mode(self) -> GameMode:
        """
        The player's current game mode.
        """
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None:
        ...
    @property
    def game_version(self) -> str:
        """
        Get the player's current game version.
        """
    @property
    def inventory(self) -> PlayerInventory:
        """
        Get the player's inventory.
        """
    @property
    def is_flying(self) -> bool:
        """
        If the player is currently flying or not.
        """
    @is_flying.setter
    def is_flying(self, arg1: bool) -> None:
        ...
    @property
    def is_op(self) -> bool:
        """
        The operator status of this playerall
        """
    @is_op.setter
    def is_op(self, arg1: bool) -> None:
        ...
    @property
    def is_sneaking(self) -> bool:
        """
        Gets or sets the sneak mode of the player
        """
    @is_sneaking.setter
    def is_sneaking(self, arg1: bool) -> None:
        ...
    @property
    def is_sprinting(self) -> bool:
        """
        Gets or sets whether the player is sprinting or not.
        """
    @is_sprinting.setter
    def is_sprinting(self, arg1: bool) -> None:
        ...
    @property
    def locale(self) -> str:
        """
        Get the player's current locale.
        """
    @property
    def name(self) -> str:
        """
        Returns the name of this player
        """
    @property
    def ping(self) -> int:
        """
        Gets the player's average ping in milliseconds.
        """
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets or sets the player's visible Scoreboard.
        """
    @scoreboard.setter
    def scoreboard(self, arg1: Scoreboard) -> None:
        ...
    @property
    def skin(self) -> Skin:
        """
        Get the player's skin.
        """
    @property
    def total_exp(self) -> int:
        """
        Gets the players total experience points.
        """
    @property
    def unique_id(self) -> uuid.UUID:
        """
        Returns the UUID of this player
        """
    @property
    def walk_speed(self) -> float:
        """
        Gets or sets the current allowed speed that a client can walk.
        """
    @walk_speed.setter
    def walk_speed(self, arg1: float) -> None:
        ...
    @property
    def xuid(self) -> str:
        """
        Returns the Xbox User ID (XUID) of this player
        """
class PlayerBanEntry(BanEntry):
    """
    Represents a ban entry for a player.
    """
    def __init__(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None:
        ...
    @property
    def name(self) -> str:
        """
        Gets the banned player's name.
        """
    @property
    def unique_id(self) -> uuid.UUID | None:
        """
        Gets the banned player's unique ID, or None if not available.
        """
    @property
    def xuid(self) -> str | None:
        """
        Gets the banned player's Xbox user ID (XUID), or None if not available.
        """
class PlayerBanList:
    """
    Represents a ban list containing banned players.
    """
    def add_ban(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None, reason: str | None = None, expires: datetime.datetime | None = None, source: str | None = None) -> PlayerBanEntry:
        """
        Adds a ban to this list, or updates an existing one.
        """
    def get_ban_entry(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> PlayerBanEntry:
        """
        Gets a BanEntry by player name, UUID, or XUID.
        """
    def is_banned(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> bool:
        """
        Checks if a BanEntry exists for the target by name, UUID, or XUID.
        """
    def remove_ban(self, name: str, uuid: uuid.UUID | None = None, xuid: str | None = None) -> None:
        """
        Removes a player from the ban list by name, UUID, or XUID.
        """
    @property
    def entries(self) -> list[PlayerBanEntry]:
        """
        Gets a vector of pointers to entries in the ban list.
        """
class PlayerBedEnterEvent(PlayerEvent, Cancellable):
    """
    Called when a player is almost about to enter the bed.
    """
    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """
class PlayerBedLeaveEvent(PlayerEvent):
    """
    Called when a player is leaving a bed.
    """
    @property
    def bed(self) -> Block:
        """
        Returns the bed block involved in this event.
        """
class PlayerChatEvent(PlayerEvent, Cancellable):
    """
    Called when a player sends a chat message.
    """
    @property
    def format(self) -> str:
        """
        Sets the format to use to display this chat message
        """
    @format.setter
    def format(self, arg1: str) -> None:
        ...
    @property
    def message(self) -> str:
        """
        Gets or sets the message that the player will send.
        """
    @message.setter
    def message(self, arg1: str) -> None:
        ...
    @property
    def player(self) -> Player:
        """
        Gets or sets the player that this message will display as
        """
    @player.setter
    def player(self, arg1: Player) -> None:
        ...
    @property
    def recipients(self) -> list[Player]:
        """
        Gets a set of recipients that this chat message will be displayed to
        """
class PlayerCommandEvent(PlayerEvent, Cancellable):
    """
    Called whenever a player runs a command.
    """
    @property
    def command(self) -> str:
        """
        Gets or sets the command that the player will send.
        """
    @command.setter
    def command(self, arg1: str) -> None:
        ...
class PlayerDeathEvent(ActorDeathEvent, PlayerEvent):
    """
    Called when a player dies
    """
    @property
    def death_message(self) -> str | Translatable | None:
        """
        Gets or sets the death message that will appear to everyone on the server.
        """
    @death_message.setter
    def death_message(self, arg1: str | Translatable | None) -> None:
        ...
class PlayerDropItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player drops an item from their inventory
    """
    @property
    def item(self) -> ItemStack:
        """
        Gets the ItemStack dropped by the player
        """
class PlayerEmoteEvent(PlayerEvent, Cancellable):
    """
    Called when a player uses and emote
    """
    @property
    def emote_id(self) -> str:
        """
        Gets the emote piece ID
        """
    @property
    def is_muted(self) -> bool:
        """
        Gets or sets the muted state for the emote.
        """
    @is_muted.setter
    def is_muted(self, arg1: bool) -> None:
        ...
class PlayerEvent(Event):
    """
    Represents a player related event
    """
    @property
    def player(self) -> Player:
        """
        Returns the player involved in this event.
        """
class PlayerGameModeChangeEvent(PlayerEvent, Cancellable):
    """
    Called when the GameMode of the player is changed.
    """
    @property
    def new_game_mode(self) -> GameMode:
        """
        Gets the GameMode the player is switched to.
        """
class PlayerInteractActorEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks an actor.
    """
    @property
    def actor(self) -> Actor:
        """
        Gets the actor that was right-clicked by the player.
        """
class PlayerInteractEvent(PlayerEvent, Cancellable):
    """
    Represents an event that is called when a player right-clicks a block.
    """
    class Action:
        LEFT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_AIR: 2>
        LEFT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.LEFT_CLICK_BLOCK: 0>
        RIGHT_CLICK_AIR: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_AIR: 3>
        RIGHT_CLICK_BLOCK: typing.ClassVar[PlayerInteractEvent.Action]  # value = <Action.RIGHT_CLICK_BLOCK: 1>
        __members__: typing.ClassVar[dict[str, PlayerInteractEvent.Action]]  # value = {'LEFT_CLICK_BLOCK': <Action.LEFT_CLICK_BLOCK: 0>, 'RIGHT_CLICK_BLOCK': <Action.RIGHT_CLICK_BLOCK: 1>, 'LEFT_CLICK_AIR': <Action.LEFT_CLICK_AIR: 2>, 'RIGHT_CLICK_AIR': <Action.RIGHT_CLICK_AIR: 3>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    @property
    def action(self) -> PlayerInteractEvent.Action:
        """
        Returns the action type of interaction
        """
    @property
    def block(self) -> Block:
        """
        Returns the clicked block
        """
    @property
    def block_face(self) -> BlockFace:
        """
        Returns the face of the block that was clicked
        """
    @property
    def clicked_position(self) -> Vector | None:
        """
        Gets the exact position on the block the player interacted with.
        """
    @property
    def has_block(self) -> bool:
        """
        Check if this event involved a block
        """
    @property
    def has_item(self) -> bool:
        """
        Check if this event involved an item
        """
    @property
    def item(self) -> ItemStack:
        """
        Returns the item in hand represented by this event
        """
class PlayerInventory(Inventory):
    """
    Interface to the inventory of a Player, including the four armor slots and any extra slots.
    """
    @property
    def boots(self) -> ItemStack:
        """
        Gets or sets the ItemStack in the boots slot
        """
    @boots.setter
    def boots(self, arg1: ItemStack) -> None:
        ...
    @property
    def chestplate(self) -> ItemStack:
        """
        Gets or sets the ItemStack in the chestplate slot
        """
    @chestplate.setter
    def chestplate(self, arg1: ItemStack) -> None:
        ...
    @property
    def held_item_slot(self) -> int:
        """
        Gets or sets the slot number of the currently held item
        """
    @held_item_slot.setter
    def held_item_slot(self, arg1: int) -> None:
        ...
    @property
    def helmet(self) -> ItemStack:
        """
        Gets or sets the ItemStack in the helmet slot
        """
    @helmet.setter
    def helmet(self, arg1: ItemStack) -> None:
        ...
    @property
    def item_in_main_hand(self) -> ItemStack:
        """
        Gets or sets the item the player is currently holding in their main hand.
        """
    @item_in_main_hand.setter
    def item_in_main_hand(self, arg1: ItemStack) -> None:
        ...
    @property
    def item_in_off_hand(self) -> ItemStack:
        """
        Gets or sets the item the player is currently holding in their off hand.
        """
    @item_in_off_hand.setter
    def item_in_off_hand(self, arg1: ItemStack) -> None:
        ...
    @property
    def leggings(self) -> ItemStack:
        """
        Gets or sets the ItemStack in the leg slot
        """
    @leggings.setter
    def leggings(self, arg1: ItemStack) -> None:
        ...
class PlayerItemConsumeEvent(PlayerEvent, Cancellable):
    """
    Called when a player is finishing consuming an item (food, potion, milk bucket).
    """
    @property
    def hand(self) -> EquipmentSlot:
        """
        Get the hand used to consume the item.
        """
    @property
    def item(self) -> ItemStack:
        """
        Gets or sets the item that is being consumed.
        """
class PlayerItemHeldEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their currently held item.
    """
    @property
    def new_slot(self) -> int:
        """
        Gets the new held slot index
        """
    @property
    def previous_slot(self) -> int:
        """
        Gets the previous held slot index.
        """
class PlayerJoinEvent(PlayerEvent):
    """
    Called when a player joins a server
    """
    @property
    def join_message(self) -> str | Translatable | None:
        """
        Gets or sets the join message to send to all online players.
        """
    @join_message.setter
    def join_message(self, arg1: str | Translatable | None) -> None:
        ...
class PlayerJumpEvent(PlayerMoveEvent):
    """
    Called when a player jumps.
    """
class PlayerKickEvent(PlayerEvent, Cancellable):
    """
    Called when a player gets kicked from the server
    """
    @property
    def reason(self) -> str:
        """
        Gets or sets the reason why the player is getting kicked
        """
    @reason.setter
    def reason(self, arg1: str) -> None:
        ...
class PlayerLoginEvent(PlayerEvent, Cancellable):
    """
    Called when a player attempts to login in.
    """
    @property
    def kick_message(self) -> str:
        """
        Gets or sets kick message to display if event is cancelled
        """
    @kick_message.setter
    def kick_message(self, arg1: str) -> None:
        ...
class PlayerMoveEvent(PlayerEvent, Cancellable):
    """
    Called when a player moves.
    """
    @property
    def from_location(self) -> Location:
        """
        Gets or sets the location that this player moved from.
        """
    @from_location.setter
    def from_location(self, arg1: Location) -> None:
        ...
    @property
    def to_location(self) -> Location:
        """
        Gets or sets the location that this player moved to.
        """
    @to_location.setter
    def to_location(self, arg1: Location) -> None:
        ...
class PlayerPickupItemEvent(PlayerEvent, Cancellable):
    """
    Called when a player picks an item up from the ground.
    """
    @property
    def item(self) -> Item:
        """
        Gets the Item picked up by the entity.
        """
class PlayerQuitEvent(PlayerEvent):
    """
    Called when a player leaves a server.
    """
    @property
    def quit_message(self) -> str | Translatable | None:
        """
        Gets or sets the quit message to send to all online players.
        """
    @quit_message.setter
    def quit_message(self, arg1: str | Translatable | None) -> None:
        ...
class PlayerRespawnEvent(PlayerEvent):
    """
    Called when a player respawns.
    """
class PlayerSkinChangeEvent(PlayerEvent, Cancellable):
    """
    Called when a player changes their skin.
    """
    @property
    def new_skin(self) -> Skin:
        """
        Gets the player's new skin.
        """
    @property
    def skin_change_message(self) -> str | Translatable | None:
        """
        Gets or sets the message to send to all online players for this skin change.
        """
    @skin_change_message.setter
    def skin_change_message(self, arg1: str | Translatable | None) -> None:
        ...
class PlayerTeleportEvent(PlayerMoveEvent):
    """
    Called when a player is teleported from one location to another.
    """
class Plugin(CommandExecutor):
    """
    Represents a Plugin
    """
    def __init__(self) -> None:
        ...
    def _get_description(self) -> PluginDescription:
        ...
    def get_command(self, name: str) -> PluginCommand:
        """
        Gets the command with the given name, specific to this plugin.
        """
    def on_disable(self) -> None:
        """
        Called when this plugin is disabled
        """
    def on_enable(self) -> None:
        """
        Called when this plugin is enabled
        """
    def on_load(self) -> None:
        """
        Called after a plugin is loaded but before it has been enabled.
        """
    @property
    def data_folder(self) -> os.PathLike:
        """
        Returns the folder that the plugin data's files are located in.
        """
    @property
    def is_enabled(self) -> bool:
        """
        Returns a value indicating whether this plugin is currently enabled
        """
    @property
    def logger(self) -> Logger:
        """
        Returns the plugin logger associated with this server's logger.
        """
    @property
    def name(self) -> str:
        """
        Returns the name of the plugin.
        """
    @property
    def plugin_loader(self) -> PluginLoader:
        """
        Gets the associated PluginLoader responsible for this plugin
        """
    @property
    def server(self) -> Server:
        """
        Returns the Server instance currently running this plugin
        """
class PluginCommand(Command):
    """
    Represents a Command belonging to a Plugin
    """
    @property
    def executor(self) -> CommandExecutor:
        """
        The CommandExecutor to run when parsing this command
        """
    @executor.setter
    def executor(self, arg1: CommandExecutor) -> None:
        ...
    @property
    def plugin(self) -> Plugin:
        """
        The owner of this PluginCommand
        """
class PluginDescription:
    """
    Represents the basic information about a plugin that the plugin loader needs to know.
    """
    def __init__(self, name: str, version: str, description: str | None = None, load: PluginLoadOrder | None = None, authors: list[str] | None = None, contributors: list[str] | None = None, website: str | None = None, prefix: str | None = None, provides: list[str] | None = None, depend: list[str] | None = None, soft_depend: list[str] | None = None, load_before: list[str] | None = None, default_permission: PermissionDefault | None = None, commands: list[Command] | None = None, permissions: list[Permission] | None = None, *args, **kwargs) -> None:
        ...
    @property
    def api_version(self) -> str:
        """
        Gives the API version which this plugin is designed to support.
        """
    @property
    def authors(self) -> list[str]:
        """
        Gives the list of authors for the plugin.
        """
    @property
    def commands(self) -> list[Command]:
        """
        Gives the list of commands the plugin will register at runtime.
        """
    @property
    def contributors(self) -> list[str]:
        """
        Gives the list of contributors for the plugin.
        """
    @property
    def default_permission(self) -> PermissionDefault:
        """
        Gives the default value of permissions registered for the plugin.
        """
    @property
    def depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires.
        """
    @property
    def description(self) -> str:
        """
        Gives a human-friendly description of the functionality the plugin provides.
        """
    @property
    def full_name(self) -> str:
        """
        Returns the name of a plugin, including the version.
        """
    @property
    def load(self) -> PluginLoadOrder:
        """
        Gives the phase of server startup that the plugin should be loaded.
        """
    @property
    def load_before(self) -> list[str]:
        """
        Gets the list of plugins that should consider this plugin a soft-dependency.
        """
    @property
    def name(self) -> str:
        """
        Gives the name of the plugin. This name is a unique identifier for plugins.
        """
    @property
    def permissions(self) -> list[Permission]:
        """
        Gives the list of permissions the plugin will register at runtime, immediately proceeding enabling.
        """
    @property
    def prefix(self) -> str:
        """
        Gives the token to prefix plugin-specific logging messages with.
        """
    @property
    def provides(self) -> list[str]:
        """
        Gives the list of other plugin APIs which this plugin provides. These are usable for other plugins to depend on.
        """
    @property
    def soft_depend(self) -> list[str]:
        """
        Gives a list of other plugins that the plugin requires for full functionality.
        """
    @property
    def version(self) -> str:
        """
        Gives the version of the plugin.
        """
    @property
    def website(self) -> str:
        """
        Gives the plugin's or plugin's author's website.
        """
class PluginDisableEvent(ServerEvent):
    """
    Called when a plugin is disabled.
    """
    @property
    def plugin(self) -> Plugin:
        ...
class PluginEnableEvent(ServerEvent):
    """
    Called when a plugin is enabled.
    """
    @property
    def plugin(self) -> Plugin:
        ...
class PluginLoadOrder:
    """
    Represents the order in which a plugin should be initialized and enabled.
    """
    POSTWORLD: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.POSTWORLD: 1>
    STARTUP: typing.ClassVar[PluginLoadOrder]  # value = <PluginLoadOrder.STARTUP: 0>
    __members__: typing.ClassVar[dict[str, PluginLoadOrder]]  # value = {'STARTUP': <PluginLoadOrder.STARTUP: 0>, 'POSTWORLD': <PluginLoadOrder.POSTWORLD: 1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class PluginLoader:
    """
    Represents a plugin loader, which handles direct access to specific types of plugins
    """
    def __init__(self, server: Server) -> None:
        ...
    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """
    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """
    def load_plugin(self, file: str) -> Plugin:
        """
        Loads the plugin contained in the specified file
        """
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """
    @property
    def plugin_file_filters(self) -> list[str]:
        """
        Returns a list of all filename filters expected by this PluginLoader
        """
    @property
    def server(self) -> Server:
        """
        Retrieves the Server object associated with the PluginLoader.
        """
class PluginManager:
    """
    Represents a plugin manager that handles all plugins from the Server
    """
    def call_event(self, event: Event) -> None:
        """
        Calls an event which will be passed to plugins.
        """
    def clear_plugins(self) -> None:
        """
        Disables and removes all plugins
        """
    def disable_plugin(self, plugin: Plugin) -> None:
        """
        Disables the specified plugin
        """
    def disable_plugins(self) -> None:
        """
        Disables all the loaded plugins
        """
    def enable_plugin(self, plugin: Plugin) -> None:
        """
        Enables the specified plugin
        """
    def enable_plugins(self) -> None:
        """
        Enable all the loaded plugins
        """
    def get_default_perm_subscriptions(self, level: PermissionLevel) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given default list, by permission level.
        """
    def get_default_permissions(self, level: PermissionLevel) -> list[Permission]:
        """
        Gets the default permissions for the given permission level.
        """
    def get_permission(self, name: str) -> Permission:
        """
        Gets a Permission from its fully qualified name.
        """
    def get_permission_subscriptions(self, permission: str) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given permission.
        """
    def get_plugin(self, name: str) -> Plugin:
        """
        Checks if the given plugin is loaded and returns it when applicable.
        """
    @typing.overload
    def is_plugin_enabled(self, plugin: str) -> bool:
        """
        Checks if the given plugin is enabled or not
        """
    @typing.overload
    def is_plugin_enabled(self, plugin: Plugin) -> bool:
        """
        Checks if the given plugin is enabled or not
        """
    def load_plugin(self, file: str) -> Plugin:
        """
        Loads the plugin in the specified file
        """
    @typing.overload
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
        """
    @typing.overload
    def load_plugins(self, files: list[str]) -> list[Plugin]:
        """
        Loads the plugins in the list of the files
        """
    def recalculate_permission_defaults(self, perm: Permission) -> None:
        """
        Recalculates the defaults for the given Permission.
        """
    def register_event(self, name: str, executor: typing.Callable[[Event], None], priority: EventPriority, plugin: Plugin, ignore_cancelled: bool) -> None:
        """
        Registers the given event
        """
    @typing.overload
    def remove_permission(self, perm: Permission) -> None:
        """
        Removes a Permission registration from this plugin manager by permission object.
        """
    @typing.overload
    def remove_permission(self, name: str) -> None:
        """
        Removes a Permission registration from this plugin manager by name.
        """
    def subscribe_to_default_perms(self, level: PermissionLevel, permissible: Permissible) -> None:
        """
        Subscribes to the given Default permissions by permission level.
        """
    def subscribe_to_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Subscribes the given Permissible for information about the requested Permission.
        """
    def unsubscribe_from_default_perms(self, level: PermissionLevel, permissible: Permissible) -> None:
        """
        Unsubscribes from the given Default permissions by permission level.
        """
    def unsubscribe_from_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Unsubscribes the given Permissible for information about the requested Permission.
        """
    @property
    def permissions(self) -> set[Permission]:
        """
        Gets a set of all registered permissions.
        """
    @property
    def plugins(self) -> list[Plugin]:
        """
        Gets a list of all currently loaded plugins
        """
class Position(Vector):
    """
    Represents a 3-dimensional position in a dimension within a level.
    """
    def __init__(self, dimension: Dimension, x: float, y: float, z: float) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def block_x(self) -> int:
        """
        Gets the floored value of the X component, indicating the block that this location is contained with.
        """
    @property
    def block_y(self) -> int:
        """
        Gets the floored value of the Y component, indicating the block that this location is contained with.
        """
    @property
    def block_z(self) -> int:
        """
        Gets the floored value of the Z component, indicating the block that this location is contained with.
        """
    @property
    def dimension(self) -> Dimension:
        """
        The Dimension that contains this position
        """
    @dimension.setter
    def dimension(self, arg1: Dimension) -> None:
        ...
class RenderType:
    """
    Controls the way in which an Objective is rendered on the client side.
    """
    HEARTS: typing.ClassVar[RenderType]  # value = <RenderType.HEARTS: 1>
    INTEGER: typing.ClassVar[RenderType]  # value = <RenderType.INTEGER: 0>
    __members__: typing.ClassVar[dict[str, RenderType]]  # value = {'INTEGER': <RenderType.INTEGER: 0>, 'HEARTS': <RenderType.HEARTS: 1>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Scheduler:
    """
    Represents a scheduler that executes various tasks
    """
    def cancel_task(self, id: int) -> None:
        """
        Removes task from scheduler.
        """
    def cancel_tasks(self, plugin: Plugin) -> None:
        """
        Removes all tasks associated with a particular plugin from the scheduler.
        """
    def get_pending_tasks(self) -> list[Task]:
        """
        Returns a vector of all pending tasks.
        """
    def is_queued(self, id: int) -> bool:
        """
        Check if the task queued to be run later.
        """
    def is_running(self, id: int) -> bool:
        """
        Check if the task currently running.
        """
    def run_task(self, plugin: Plugin, task: typing.Callable[[], None], delay: int = 0, period: int = 0) -> Task:
        """
        Returns a task that will be executed synchronously
        """
class Score:
    """
    Represents a score for an objective on a scoreboard.
    """
    @property
    def entry(self) -> Player | Actor | str:
        """
        Gets the entry being tracked by this Score
        """
    @property
    def is_score_set(self) -> bool:
        """
        Shows if this score has been set at any point in time.
        """
    @property
    def objective(self) -> Objective:
        """
        Gets the Objective being tracked by this Score.
        """
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the scoreboard for the associated objective.
        """
    @property
    def value(self) -> int:
        """
        Gets or sets the current score.
        """
    @value.setter
    def value(self, arg1: int) -> None:
        ...
class Scoreboard:
    """
    Represents a scoreboard
    """
    def add_objective(self, name: str, criteria: Criteria.Type, display_name: str | None = None, render_type: RenderType = RenderType.INTEGER) -> Objective:
        """
        Registers an Objective on this Scoreboard with a name displayed to players
        """
    def clear_slot(self, slot: DisplaySlot) -> None:
        """
        Clears any objective in the specified slot
        """
    @typing.overload
    def get_objective(self, name: str) -> Objective:
        """
        Gets an Objective on this Scoreboard by name
        """
    @typing.overload
    def get_objective(self, slot: DisplaySlot) -> Objective:
        """
        Gets the Objective currently displayed in a DisplaySlot on this Scoreboard
        """
    def get_objectives_by_criteria(self, criteria: Criteria.Type) -> list[Objective]:
        """
        Gets all Objectives of a Criteria on the Scoreboard
        """
    def get_scores(self, entry: Player | Actor | str) -> list[Score]:
        """
        Gets all scores for an entry on this Scoreboard
        """
    def reset_scores(self, entry: Player | Actor | str) -> None:
        """
        Removes all scores for an entry on this Scoreboard
        """
    @property
    def entries(self) -> list[Player | Actor | str]:
        """
        Gets all entries tracked by this Scoreboard
        """
    @property
    def objectives(self) -> list[Objective]:
        """
        Gets all Objectives on this Scoreboard
        """
class ScriptMessageEvent(ServerEvent, Cancellable):
    """
    Called when a message is sent by `/scriptevent` command
    """
    @property
    def message(self) -> str:
        """
        Get the message to send.
        """
    @property
    def message_id(self) -> str:
        """
        Get the message id to send.
        """
    @property
    def sender(self) -> CommandSender:
        """
        Gets the command sender who initiated the command.
        """
class Server:
    """
    Represents a server implementation.
    """
    def broadcast(self, message: str | Translatable, permission: str) -> None:
        """
        Broadcasts the specified message to every user with the given permission name.
        """
    def broadcast_message(self, message: str | Translatable) -> None:
        """
        Broadcasts the specified message to every user with permission endstone.broadcast.user
        """
    def create_block_data(self, type: str, block_states: dict[str, bool | str | int] | None = None) -> BlockData:
        """
        Creates a new BlockData instance for the specified block type, with all properties initialized to defaults, except for those provided.
        """
    def create_boss_bar(self, title: str, color: BarColor, style: BarStyle, flags: list[BarFlag] | None = None) -> BossBar:
        """
        Creates a boss bar instance to display to players. The progress defaults to 1.0.
        """
    def create_scoreboard(self) -> Scoreboard:
        """
        Creates a new Scoreboard to be tracked by the server.
        """
    def dispatch_command(self, sender: CommandSender, command_line: str) -> bool:
        """
        Dispatches a command on this server, and executes it if found.
        """
    @typing.overload
    def get_player(self, name: str) -> Player:
        """
        Gets the player with the exact given name, case insensitive.
        """
    @typing.overload
    def get_player(self, unique_id: uuid.UUID) -> Player:
        """
        Gets the player with the given UUID.
        """
    def get_plugin_command(self, name: str) -> PluginCommand:
        """
        Gets a PluginCommand with the given name or alias.
        """
    def reload(self) -> None:
        """
        Reloads the server configuration, functions, scripts and plugins.
        """
    def reload_data(self) -> None:
        """
        Reload only the Minecraft data for the server.
        """
    def shutdown(self) -> None:
        """
        Shutdowns the server, stopping everything.
        """
    @property
    def average_mspt(self) -> float:
        """
        Gets the average milliseconds per tick (MSPT).
        """
    @property
    def average_tick_usage(self) -> float:
        """
        Gets the average tick usage of the server.
        """
    @property
    def average_tps(self) -> float:
        """
        Gets the average ticks per second (TPS).
        """
    @property
    def ban_list(self) -> PlayerBanList:
        """
        Gets the player ban list.
        """
    @property
    def command_sender(self) -> ConsoleCommandSender:
        """
        Gets a CommandSender for this server.
        """
    @property
    def current_mspt(self) -> float:
        """
        Gets the current milliseconds per tick (MSPT).
        """
    @property
    def current_tick_usage(self) -> float:
        """
        Gets the current tick usage of the server.
        """
    @property
    def current_tps(self) -> float:
        """
        Gets the current ticks per second (TPS).
        """
    @property
    def enchantment_registry(self) -> EnchantmentRegistry:
        """
        Returns the registry for all the enchantments.
        """
    @property
    def ip_ban_list(self) -> IpBanList:
        """
        Gets the IP ban list.
        """
    @property
    def item_factory(self) -> ItemFactory:
        """
        Gets the instance of the item factory (for ItemMeta).
        """
    @property
    def item_registry(self) -> ItemRegistry:
        """
        Returns the registry for all the item types.
        """
    @property
    def language(self) -> Language:
        """
        Gets the current language interface used by the server.
        """
    @property
    def level(self) -> Level:
        """
        Gets the server level.
        """
    @property
    def logger(self) -> Logger:
        """
        Returns the primary logger associated with this server instance.
        """
    @property
    def max_players(self) -> int:
        """
        The maximum amount of players which can login to this server.
        """
    @max_players.setter
    def max_players(self, arg1: int) -> None:
        ...
    @property
    def minecraft_version(self) -> str:
        """
        Gets the Minecraft version that this server is running.
        """
    @property
    def name(self) -> str:
        """
        Gets the name of this server implementation.
        """
    @property
    def online_mode(self) -> bool:
        """
        Gets whether the Server is in online mode or not.
        """
    @property
    def online_players(self) -> list[Player]:
        """
        Gets a list of all currently online players.
        """
    @property
    def plugin_manager(self) -> PluginManager:
        """
        Gets the plugin manager for interfacing with plugins.
        """
    @property
    def port(self) -> int:
        """
        Get the game port that the server runs on.
        """
    @property
    def port_v6(self) -> int:
        """
        Get the game port (IPv6) that the server runs on.
        """
    @property
    def protocol_version(self) -> int:
        """
        Gets the network protocol version that this server supports.
        """
    @property
    def scheduler(self) -> Scheduler:
        """
        Gets the scheduler for managing scheduled events.
        """
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the primary Scoreboard controlled by the server.
        """
    @property
    def service_manager(self) -> ServiceManager:
        """
        Gets the service manager.
        """
    @property
    def start_time(self) -> datetime.datetime:
        """
        Gets the start time of the server.
        """
    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """
class ServerCommandEvent(ServerEvent, Cancellable):
    """
    Called when the console runs a command, early in the process.
    """
    @property
    def command(self) -> str:
        """
        Gets or sets the command that the server will execute
        """
    @command.setter
    def command(self, arg1: str) -> None:
        ...
    @property
    def sender(self) -> CommandSender:
        """
        Get the command sender.
        """
class ServerEvent(Event):
    """
    Represents a server-related event
    """
class ServerListPingEvent(ServerEvent, Cancellable):
    """
    Called when a server ping is coming in.
    """
    @property
    def game_mode(self) -> GameMode:
        """
        Gets or sets the current game mode.
        """
    @game_mode.setter
    def game_mode(self, arg1: GameMode) -> None:
        ...
    @property
    def level_name(self) -> str:
        """
        Gets or sets the level name.
        """
    @level_name.setter
    def level_name(self, arg1: str) -> None:
        ...
    @property
    def local_port(self) -> int:
        """
        Get or set the local port of the server.
        """
    @local_port.setter
    def local_port(self, arg1: int) -> None:
        ...
    @property
    def local_port_v6(self) -> int:
        """
        Get or set the local port of the server for IPv6 support
        """
    @local_port_v6.setter
    def local_port_v6(self, arg1: int) -> None:
        ...
    @property
    def max_players(self) -> int:
        """
        Gets or sets the maximum number of players allowed.
        """
    @max_players.setter
    def max_players(self, arg1: int) -> None:
        ...
    @property
    def minecraft_version_network(self) -> str:
        """
        Gets or sets the network version of Minecraft that is supported by this server
        """
    @minecraft_version_network.setter
    def minecraft_version_network(self, arg1: str) -> None:
        ...
    @property
    def motd(self) -> str:
        """
        Gets or sets the message of the day message.
        """
    @motd.setter
    def motd(self, arg1: str) -> None:
        ...
    @property
    def network_protocol_version(self) -> int:
        """
        Get the network protocol version of this server
        """
    @property
    def num_players(self) -> int:
        """
        Gets or sets the number of players online.
        """
    @num_players.setter
    def num_players(self, arg1: int) -> None:
        ...
    @property
    def remote_host(self) -> str:
        """
        Get the host the ping is coming from.
        """
    @property
    def remote_port(self) -> int:
        """
        Get the port the ping is coming from.
        """
    @property
    def server_guid(self) -> str:
        """
        Get or set the unique identifier of the server.
        """
    @server_guid.setter
    def server_guid(self, arg1: str) -> None:
        ...
class ServerLoadEvent(Event):
    """
    Called when either the server startup or reload has completed.
    """
    class LoadType:
        STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>
        __members__: typing.ClassVar[dict[str, ServerLoadEvent.LoadType]]  # value = {'STARTUP': <LoadType.STARTUP: 0>}
        def __eq__(self, other: typing.Any) -> bool:
            ...
        def __getstate__(self) -> int:
            ...
        def __hash__(self) -> int:
            ...
        def __index__(self) -> int:
            ...
        def __init__(self, value: int) -> None:
            ...
        def __int__(self) -> int:
            ...
        def __ne__(self, other: typing.Any) -> bool:
            ...
        def __repr__(self) -> str:
            ...
        def __setstate__(self, state: int) -> None:
            ...
        def __str__(self) -> str:
            ...
        @property
        def name(self) -> str:
            ...
        @property
        def value(self) -> int:
            ...
    STARTUP: typing.ClassVar[ServerLoadEvent.LoadType]  # value = <LoadType.STARTUP: 0>
    @property
    def type(self) -> ServerLoadEvent.LoadType:
        ...
class Service:
    """
    Represents a list of methods.
    """
    def __init__(self) -> None:
        ...
class ServiceManager:
    """
    Represent a service manager that manages services and service providers.
    """
    def load(self, name: str) -> Service:
        ...
    def register(self, name: str, provider: Service, plugin: Plugin, priority: ServicePriority) -> None:
        """
        Register a provider of a service.
        """
    @typing.overload
    def unregister(self, name: str, provider: Service) -> None:
        """
        Unregister a particular provider for a particular service.
        """
    @typing.overload
    def unregister(self, provider: Service) -> None:
        """
        Unregister a particular provider.
        """
    def unregister_all(self, plugin: Plugin) -> None:
        """
        Unregister all the services registered by a particular plugin.
        """
class ServicePriority:
    """
    Represents various priorities of a provider.
    """
    HIGH: typing.ClassVar[ServicePriority]  # value = <ServicePriority.HIGH: 3>
    HIGHEST: typing.ClassVar[ServicePriority]  # value = <ServicePriority.HIGHEST: 4>
    LOW: typing.ClassVar[ServicePriority]  # value = <ServicePriority.LOW: 1>
    LOWEST: typing.ClassVar[ServicePriority]  # value = <ServicePriority.LOWEST: 0>
    NORMAL: typing.ClassVar[ServicePriority]  # value = <ServicePriority.NORMAL: 2>
    __members__: typing.ClassVar[dict[str, ServicePriority]]  # value = {'LOWEST': <ServicePriority.LOWEST: 0>, 'LOW': <ServicePriority.LOW: 1>, 'NORMAL': <ServicePriority.NORMAL: 2>, 'HIGH': <ServicePriority.HIGH: 3>, 'HIGHEST': <ServicePriority.HIGHEST: 4>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Skin:
    """
    Represents a player skin.
    """
    def __init__(self, id: str, image: numpy.ndarray[numpy.uint8], cape_id: str | None = None, cape_image: numpy.ndarray[numpy.uint8] | None = None) -> None:
        ...
    @property
    def cape_id(self) -> str | None:
        """
        Get the Cape ID.
        """
    @property
    def cape_image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Cape image.
        """
    @property
    def id(self) -> str:
        """
        Get the Skin ID.
        """
    @property
    def image(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Skin image.
        """
class Slider:
    """
    Represents a slider with a label.
    """
    def __init__(self, label: str | Translatable = '', min: float = 0, max: float = 100, step: float = 20, default_value: float | None = None) -> None:
        ...
    @property
    def default_value(self) -> float | None:
        """
        Gets or sets the optional default value of the slider.
        """
    @default_value.setter
    def default_value(self, arg1: float | None) -> Slider:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the slider.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> Slider:
        ...
    @property
    def max(self) -> float:
        """
        Gets or sets the maximum value of the slider.
        """
    @max.setter
    def max(self, arg1: float) -> Slider:
        ...
    @property
    def min(self) -> float:
        """
        Gets or sets the minimum value of the slider.
        """
    @min.setter
    def min(self, arg1: float) -> Slider:
        ...
    @property
    def step(self) -> float:
        """
        Gets or sets the step size of the slider.
        """
    @step.setter
    def step(self, arg1: float) -> Slider:
        ...
class SocketAddress:
    """
    Represents an IP Socket Address (hostname + port number).
    """
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, hostname: str, port: int) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
    @property
    def hostname(self) -> str:
        """
        Gets the hostname.
        """
    @property
    def port(self) -> int:
        """
        Gets the port number.
        """
class StepSlider:
    """
    Represents a step slider with a set of predefined options.
    """
    def __init__(self, label: str | Translatable = '', options: list[str] | None = None, default_index: int | None = None) -> None:
        ...
    def add_option(self, option: str) -> Dropdown:
        """
        Adds a new option to the step slider.
        """
    @property
    def default_index(self) -> int | None:
        """
        Gets or sets the optional default index of the step slider.
        """
    @default_index.setter
    def default_index(self, arg1: int | None) -> Dropdown:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the step slider.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> Dropdown:
        ...
    @property
    def options(self) -> list[str]:
        """
        Gets or sets the options of the step slider.
        """
    @options.setter
    def options(self, arg1: list[str]) -> Dropdown:
        ...
class Task:
    """
    Represents a task being executed by the scheduler
    """
    def cancel(self) -> None:
        """
        Attempts to cancel this task.
        """
    @property
    def is_cancelled(self) -> bool:
        """
        Returns true if the task has been cancelled.
        """
    @property
    def is_sync(self) -> bool:
        """
        Returns true if the task is run by server thread.
        """
    @property
    def owner(self) -> Plugin:
        """
        Returns the Plugin that owns the task.
        """
    @property
    def task_id(self) -> int:
        """
        Returns the task id.
        """
class TextInput:
    """
    Represents a text input field.
    """
    def __init__(self, label: str | Translatable = '', placeholder: str | Translatable = '', default_value: str | None = None) -> None:
        ...
    @property
    def default_value(self) -> str | None:
        """
        Gets or sets the optional default text of the text input field.
        """
    @default_value.setter
    def default_value(self, arg1: str | None) -> TextInput:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the text input field.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> TextInput:
        ...
    @property
    def placeholder(self) -> str | Translatable:
        """
        Gets or sets the placeholder of the text input field.
        """
    @placeholder.setter
    def placeholder(self, arg1: str | Translatable) -> TextInput:
        ...
class ThunderChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the thunder state in a world is changing.
    """
    @property
    def to_thunder_state(self) -> bool:
        """
        Gets the state of thunder that the world is being set to
        """
class Toggle:
    """
    Represents a toggle button with a label.
    """
    def __init__(self, label: str | Translatable = '', default_value: bool = False) -> None:
        ...
    @property
    def default_value(self) -> bool:
        """
        Gets or sets the value of the toggle.
        """
    @default_value.setter
    def default_value(self, arg1: bool) -> Toggle:
        ...
    @property
    def label(self) -> str | Translatable:
        """
        Gets or sets the label of the toggle.
        """
    @label.setter
    def label(self, arg1: str | Translatable) -> Toggle:
        ...
class Translatable:
    """
    Represents an object with a text representation that can be translated by the Minecraft client.
    """
    def __init__(self, text: str, params: list[str] | None = None) -> None:
        ...
    @property
    def params(self) -> list[str]:
        """
        Get the translation parameters.
        """
    @property
    def text(self) -> str:
        """
        Get the text to be translated.
        """
class Vector:
    """
    Represents a 3-dimensional vector.
    """
    @typing.overload
    def __add__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __add__(self, arg0: float) -> Vector:
        ...
    def __iadd__(self, arg0: Vector) -> Vector:
        ...
    def __imul__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, x: float, y: float, z: float) -> None:
        ...
    def __isub__(self, arg0: Vector) -> Vector:
        ...
    def __itruediv__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __mul__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __mul__(self, arg0: float) -> Vector:
        ...
    def __radd__(self, arg0: float) -> Vector:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, arg0: float) -> Vector:
        ...
    def __rsub__(self, arg0: float) -> Vector:
        ...
    def __rtruediv__(self, arg0: float) -> Vector:
        ...
    def __str__(self) -> str:
        ...
    @typing.overload
    def __sub__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __sub__(self, arg0: float) -> Vector:
        ...
    @typing.overload
    def __truediv__(self, arg0: Vector) -> Vector:
        ...
    @typing.overload
    def __truediv__(self, arg0: float) -> Vector:
        ...
    def distance(self, other: Vector) -> float:
        """
        The distance between this Vector and another
        """
    def distance_squared(self, other: Vector) -> float:
        """
        The squared distance between this Vector and another
        """
    @property
    def length(self) -> float:
        """
        The magnitude of the Vector
        """
    @property
    def length_squared(self) -> float:
        """
        The squared magnitude of the Vector
        """
    @property
    def x(self) -> float:
        """
        The X component of the vector
        """
    @x.setter
    def x(self, arg1: float) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The Y component of the vector
        """
    @y.setter
    def y(self, arg1: float) -> None:
        ...
    @property
    def z(self) -> float:
        """
        The Z component of the vector
        """
    @z.setter
    def z(self, arg1: float) -> None:
        ...
class WeatherChangeEvent(WeatherEvent, Cancellable):
    """
    Called when the weather (rain) state in a world is changing.
    """
    @property
    def to_weather_state(self) -> bool:
        """
        Gets the state of weather that the world is being set to
        """
class WeatherEvent(Event):
    """
    Represents a weather-related event
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """
