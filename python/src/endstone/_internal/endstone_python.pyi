from __future__ import annotations
import datetime
import numpy
import os
import typing
import uuid
__all__ = ['ActionForm', 'Actor', 'ActorDeathEvent', 'ActorEvent', 'ActorKnockbackEvent', 'ActorRemoveEvent', 'ActorSpawnEvent', 'ActorTeleportEvent', 'BarColor', 'BarFlag', 'BarStyle', 'Block', 'BlockBreakEvent', 'BlockData', 'BlockEvent', 'BlockFace', 'BlockPlaceEvent', 'BlockState', 'BossBar', 'BroadcastMessageEvent', 'ColorFormat', 'Command', 'CommandExecutor', 'CommandSender', 'CommandSenderWrapper', 'ConsoleCommandSender', 'Criteria', 'Dimension', 'DisplaySlot', 'Dropdown', 'Event', 'EventPriority', 'GameMode', 'Inventory', 'ItemStack', 'Label', 'Language', 'Level', 'Location', 'Logger', 'MessageForm', 'Mob', 'ModalForm', 'Objective', 'ObjectiveSortOrder', 'Packet', 'PacketType', 'Permissible', 'Permission', 'PermissionAttachment', 'PermissionAttachmentInfo', 'PermissionDefault', 'Player', 'PlayerChatEvent', 'PlayerCommandEvent', 'PlayerDeathEvent', 'PlayerEvent', 'PlayerInteractActorEvent', 'PlayerInteractEvent', 'PlayerInventory', 'PlayerJoinEvent', 'PlayerKickEvent', 'PlayerLoginEvent', 'PlayerQuitEvent', 'PlayerTeleportEvent', 'Plugin', 'PluginCommand', 'PluginDescription', 'PluginDisableEvent', 'PluginEnableEvent', 'PluginLoadOrder', 'PluginLoader', 'PluginManager', 'Position', 'RenderType', 'Scheduler', 'Score', 'Scoreboard', 'ScriptMessageEvent', 'Server', 'ServerCommandEvent', 'ServerListPingEvent', 'ServerLoadEvent', 'Skin', 'Slider', 'SocketAddress', 'SpawnParticleEffectPacket', 'StepSlider', 'Task', 'TextInput', 'ThunderChangeEvent', 'Toggle', 'Translatable', 'Vector', 'WeatherChangeEvent']
class ActionForm:
    """
    Represents a form with buttons that let the player take action.
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
        def icon(self, arg1: str) -> ActionForm.Button:
            ...
        @property
        def on_click(self) -> typing.Callable[[Player], None]:
            """
            Gets or sets the on click callback.
            """
        @on_click.setter
        def on_click(self, arg1: typing.Callable[[Player], None]) -> ActionForm.Button:
            ...
        @property
        def text(self) -> str | Translatable:
            """
            Gets or sets the text of the button
            """
        @text.setter
        def text(self, arg1: str | Translatable) -> ActionForm.Button:
            ...
    def __init__(self, title: str | Translatable = '', content: str | Translatable = '', buttons: list[ActionForm.Button] | None = None, on_submit: typing.Callable[[Player, int], None] = None, on_close: typing.Callable[[Player], None] = None) -> None:
        ...
    def add_button(self, text: str | Translatable, icon: str | None = None, on_click: typing.Callable[[Player], None] = None) -> ActionForm:
        """
        Adds a button to the form.
        """
    @property
    def buttons(self) -> list[ActionForm.Button]:
        """
        Gets or sets the buttons of the action form.
        """
    @buttons.setter
    def buttons(self, arg1: list[ActionForm.Button]) -> ActionForm:
        ...
    @property
    def content(self) -> str | Translatable:
        """
        Gets or sets the content of the form.
        """
    @content.setter
    def content(self, arg1: str | Translatable) -> ActionForm:
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
    def is_on_ground(self) -> bool:
        """
        Returns true if the actor is supported by a block, i.e. on ground.
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
    def runtime_id(self) -> int:
        """
        Returns the runtime id for this actor.
        """
    @property
    def scoreboard_tags(self) -> list[str]:
        """
        Returns a list of scoreboard tags for this actor.
        """
    @property
    def velocity(self) -> Vector:
        """
        Gets this actor's current velocity.
        """
class ActorDeathEvent(ActorEvent):
    """
    Called when an Actor dies.
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
class ActorKnockbackEvent(ActorEvent):
    """
    Called when a living entity receives knockback.
    """
    @property
    def actor(self) -> Mob:
        """
        Returns the Mob involved in this event
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
class ActorSpawnEvent(ActorEvent):
    """
    Called when an Actor is spawned into a world.
    """
class ActorTeleportEvent(ActorEvent):
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
        Gets or sets the complete data for this block
        """
    @data.setter
    def data(self, arg1: BlockData) -> None:
        ...
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
    @type.setter
    def type(self, arg1: str) -> None:
        ...
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
class BlockBreakEvent(BlockEvent):
    """
    Called when a block is broken by a player.
    """
    @property
    def player(self) -> Player:
        """
        Gets the Player that is breaking the block involved in this event.
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
class BlockPlaceEvent(BlockEvent):
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
    @property
    def visible(self) -> bool:
        """
        If the boss bar is displayed to attached players.
        """
    @visible.setter
    def visible(self, arg1: bool) -> None:
        ...
class BroadcastMessageEvent(Event):
    """
    Event triggered for server broadcast messages such as from Server.broadcast
    """
    @property
    def message(self) -> str:
        """
        Gets or sets the message to broadcast.
        """
    @message.setter
    def message(self, arg1: str) -> None:
        ...
    @property
    def recipients(self) -> set[CommandSender]:
        """
        Gets a set of recipients that this broadcast message will be displayed to.
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
    def registered(self) -> bool:
        """
        Returns the current registered state of this command
        """
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
    @property
    def level(self) -> Level:
        """
        Gets the level to which this dimension belongs
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
class Event:
    """
    Represents an event.
    """
    @property
    def asynchronous(self) -> bool:
        """
        Whether the event fires asynchronously.
        """
    @property
    def cancellable(self) -> bool:
        """
        Whether the event can be cancelled by a plugin or the server.
        """
    @property
    def cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins
        """
    @cancelled.setter
    def cancelled(self, arg1: bool) -> None:
        ...
    @property
    def event_name(self) -> str:
        """
        Gets a user-friendly identifier for this event.
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
class Inventory:
    """
    Interface to the various inventories.
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
class ItemStack:
    """
    Represents a stack of items.
    """
    def __init__(self, type: str = 'minecraft:air', amount: int = 1) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def amount(self) -> int:
        """
        Gets or sets the amount of items in this stack.
        """
    @amount.setter
    def amount(self, arg1: int) -> None:
        ...
    @property
    def type(self) -> str:
        """
        Gets or sets the type of this item.
        """
    @type.setter
    def type(self, arg1: str) -> None:
        ...
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
    def time(self) -> int:
        """
        Gets and sets the relative in-game time on the server
        """
    @time.setter
    def time(self, arg1: int) -> None:
        ...
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
class ModalForm:
    """
    Represents a modal form with controls.
    """
    def __init__(self, title: str | Translatable = '', controls: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle] | None = None, submit_button: str | Translatable | None = None, icon: str | None = None, on_submit: typing.Callable[[Player, str], None] = None, on_close: typing.Callable[[Player], None] = None) -> None:
        ...
    def add_control(self, control: Dropdown | Label | Slider | StepSlider | TextInput | Toggle) -> ModalForm:
        """
        Adds a control to the form.
        """
    @property
    def controls(self) -> list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle]:
        """
        Gets or sets the controls of the modal form.
        """
    @controls.setter
    def controls(self, arg1: list[Dropdown | Label | Slider | StepSlider | TextInput | Toggle]) -> ModalForm:
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
        Gets the display slot this objective is displayed at
        """
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
        Gets and sets the manner in which this objective will be rendered.
        """
    @render_type.setter
    def render_type(self, arg1: RenderType) -> None:
        ...
    @property
    def scoreboard(self) -> Scoreboard:
        """
        Gets the scoreboard to which this objective is attached
        """
    @property
    def sort_order(self) -> ObjectiveSortOrder | None:
        """
        Gets and sets the sort order for this objective
        """
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
class Packet:
    """
    Represents a packet.
    """
    @property
    def type(self) -> PacketType:
        """
        Gets the type of the packet.
        """
class PacketType:
    """
    Represents the types of packets.
    """
    SPAWN_PARTICLE_EFFECT: typing.ClassVar[PacketType]  # value = <PacketType.SPAWN_PARTICLE_EFFECT: 118>
    __members__: typing.ClassVar[dict[str, PacketType]]  # value = {'SPAWN_PARTICLE_EFFECT': <PacketType.SPAWN_PARTICLE_EFFECT: 118>}
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
    def is_op(self) -> bool:
        """
        The operator status of this object
        """
    @is_op.setter
    def is_op(self, arg1: bool) -> None:
        ...
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
    FALSE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.FALSE: 1>
    NOT_OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    NOT_OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.NOT_OP: 3>
    OP: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    OPERATOR: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.OP: 2>
    TRUE: typing.ClassVar[PermissionDefault]  # value = <PermissionDefault.TRUE: 0>
    __members__: typing.ClassVar[dict[str, PermissionDefault]]  # value = {'TRUE': <PermissionDefault.TRUE: 0>, 'FALSE': <PermissionDefault.FALSE: 1>, 'OP': <PermissionDefault.OP: 2>, 'OPERATOR': <PermissionDefault.OP: 2>, 'NOT_OP': <PermissionDefault.NOT_OP: 3>, 'NOT_OPERATOR': <PermissionDefault.NOT_OP: 3>}
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
class Player(Mob):
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
    def reset_title(self) -> None:
        """
        Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values.
        """
    def send_form(self, form: MessageForm | ActionForm | ModalForm) -> None:
        """
        Sends a form to the player.
        """
    def send_packet(self, packet: Packet) -> None:
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
    def locale(self) -> str:
        """
        Get the player's current locale.
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
class PlayerChatEvent(PlayerEvent):
    """
    Called when a player sends a chat message.
    """
    @property
    def message(self) -> str:
        """
        Gets or sets the message that the player will send.
        """
    @message.setter
    def message(self, arg1: str) -> None:
        ...
class PlayerCommandEvent(PlayerEvent):
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
    def death_message(self) -> str:
        """
        Gets or sets the death message that will appear to everyone on the server.
        """
    @death_message.setter
    def death_message(self, arg1: str) -> None:
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
class PlayerInteractActorEvent(PlayerEvent):
    """
    Represents an event that is called when a player right-clicks an actor.
    """
    @property
    def actor(self) -> Actor:
        """
        Gets the actor that was right-clicked by the player.
        """
class PlayerInteractEvent(PlayerEvent):
    """
    Represents an event that is called when a player right-clicks a block.
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
    def clicked_position(self) -> Vector:
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
class PlayerJoinEvent(PlayerEvent):
    """
    Called when a player joins a server
    """
    @property
    def join_message(self) -> str:
        """
        Gets or sets the join message to send to all online players.
        """
    @join_message.setter
    def join_message(self, arg1: str) -> None:
        ...
class PlayerKickEvent(PlayerEvent):
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
class PlayerLoginEvent(PlayerEvent):
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
class PlayerQuitEvent(PlayerEvent):
    """
    Called when a player leaves a server.
    """
    @property
    def quit_message(self) -> str:
        """
        Gets or sets the quit message to send to all online players.
        """
    @quit_message.setter
    def quit_message(self, arg1: str) -> None:
        ...
class PlayerTeleportEvent(PlayerEvent):
    """
    Called when a player is teleported from one location to another.
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
    def enabled(self) -> bool:
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
class PluginDisableEvent(Event):
    """
    Called when a plugin is disabled.
    """
    @property
    def plugin(self) -> Plugin:
        ...
class PluginEnableEvent(Event):
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
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
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
    def get_default_perm_subscriptions(self, op: bool) -> set[Permissible]:
        """
        Gets a set containing all subscribed Permissibles to the given default list, by op status.
        """
    def get_default_permissions(self, op: bool) -> set[Permission]:
        """
        Gets the default permissions for the given op status.
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
    def load_plugins(self, directory: str) -> list[Plugin]:
        """
        Loads the plugin contained within the specified directory
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
    def subscribe_to_default_perms(self, op: bool, permissible: Permissible) -> None:
        """
        Subscribes to the given Default permissions by operator status.
        """
    def subscribe_to_permission(self, permission: str, permissible: Permissible) -> None:
        """
        Subscribes the given Permissible for information about the requested Permission.
        """
    def unsubscribe_from_default_perms(self, op: bool, permissible: Permissible) -> None:
        """
        Unsubscribes from the given Default permissions by operator status.
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
    INTEGER: typing.ClassVar[RenderType]  # value = <RenderType.INTEGER: 0>
    __members__: typing.ClassVar[dict[str, RenderType]]  # value = {'INTEGER': <RenderType.INTEGER: 0>}
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
class ScriptMessageEvent(Event):
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
    def start_time(self) -> datetime.datetime:
        """
        Gets the start time of the server.
        """
    @property
    def version(self) -> str:
        """
        Gets the version of this server implementation.
        """
class ServerCommandEvent(Event):
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
class ServerListPingEvent(Event):
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
        Get the local port of the server.
        """
    @property
    def local_port_v6(self) -> int:
        """
        Get the local port of the server for IPv6 support
        """
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
        Get the unique identifier of the server.
        """
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
class Skin:
    def __init__(self, skin_id: str, skin_data: numpy.ndarray[numpy.uint8], cape_id: str | None = None, cape_data: numpy.ndarray[numpy.uint8] | None = None) -> None:
        ...
    @property
    def cape_data(self) -> numpy.ndarray[numpy.uint8] | None:
        """
        Get the Cape data.
        """
    @property
    def cape_id(self) -> str | None:
        """
        Get the Cape ID.
        """
    @property
    def skin_data(self) -> numpy.ndarray[numpy.uint8]:
        """
        Get the Skin data.
        """
    @property
    def skin_id(self) -> str:
        """
        Get the Skin ID.
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
class SpawnParticleEffectPacket(Packet):
    """
    Represents a packet for spawning a particle effect.
    """
    actor_id: int
    dimension_id: int
    effect_name: str
    molang_variables_json: str | None
    position: Vector
    def __init__(self) -> None:
        ...
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
class ThunderChangeEvent(Event):
    """
    Called when the thunder state in a world is changing.
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
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
class WeatherChangeEvent(Event):
    """
    Called when the weather (rain) state in a world is changing.
    """
    @property
    def level(self) -> Level:
        """
        Returns the Level where this event is occurring
        """
    @property
    def to_weather_state(self) -> bool:
        """
        Gets the state of weather that the world is being set to
        """
