# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Added `ActorExplodeEvent::setBlockList()` and `BlockExplodeEvent::setBlockList()`, and made their `BlockList` alias public, so the blown-up block list can be replaced wholesale from C++ as it already could from Python.
- Added `Player.send_action_bar()` for sending a message above the player's hotbar.
- Added `/restart` command (console-only) that gracefully restarts the server without manually relaunching.
- Added support for custom Python events with optional cancellation.
- Added `ActorType` to the registry API, along with the missing `ActorType.SULFUR_CUBE` constant.
- Added an attribute API: `Mob.get_attribute()`, `Mob.has_attribute()` and `Mob.attributes` expose a living entity's attributes (health, movement speed, attack damage, etc.). Each `AttributeInstance` reports its current, base, minimum and maximum value, and takes `AttributeModifier`s at runtime.
- Added `endstone.Identifier` for namespaced ids, so `dim.id.namespace` and `dim.id.key` are separable and a type checker can tell `Identifier[Dimension]` apart from `Identifier[ActorType]`. Plain strings are still accepted wherever an `Identifier` is required.
- Added `BlockData.translation_key` for retrieving the translation key of a block.
- Added `Dimension.mobs` and `Dimension.players`, mirroring Bukkit's `World#getLivingEntities()` and `World#getPlayers()`. Both narrow `Dimension.actors`, which lists everything in the dimension.
- Added `Level.create_dimension()` for creating custom dimensions at runtime, which registers and returns an empty (void) dimension built from -64 to 320. A dimension's terrain and actors survive a restart but the registration does not, so call it again on every startup to get the same dimension back.
- Added `WritableBookMeta`, `BookMeta` and `CrossbowMeta` item meta types.
- Added `PlayerEditBookEvent`, called when a player edits a page of a book and quill or signs it into a written book. The event carries the book metadata before and after the edit, the inventory slot the book sits in, and whether the book is being signed; `new_book_meta` and `is_signing` are writable, and cancelling it discards the edit.
- Added `PotionMeta` for potions, splash potions and lingering potions, giving the `PotionType` identifiers a consumer: `meta.base_potion_type` reads and writes the potion a bottle holds. Bedrock derives a potion's effects and its colour from the base potion type, so there is no `custom_effects` or `color` as in Bukkit's `PotionMeta`.
- Added binary NBT serialization (`dump`/`load`) with support for multiple formats.
- Added a unified `Object.as<T>()`/`is<T>()` casting API, replacing the per-type `asPlayer()`, `asMob()`, etc. `NotNull<T>` and `Nullable<T>` carry the same pair, so a handle can be narrowed without unwrapping it: `event.getActor().as<Player>()` returns a `Nullable<Player>` that shares ownership with the original, or a null one if the actor is not a player.
- Added a healthcheck to the Docker image so `docker ps` and Docker Compose report whether the server is actually online.
- Added `PUID`/`PGID` environment variables to the Docker image to keep server files owned by your host user.
- Added a `docker-compose.yml` for running the server with Docker Compose.
- Added graceful shutdown on Linux, so stopping the server (e.g. via `docker stop`) lets the Bedrock server save the world before it exits.
- Added `PlayerRespawnEvent.respawn_reason` (`RespawnReason.DEATH` / `RespawnReason.END_PORTAL`) so plugins can tell a normal death respawn apart from a player returning through the End exit portal.
- Added `PlayerLevelChangeEvent`, called when a player's experience level changes, reporting `old_level` and `new_level`.
- Added `PlayerExpChangeEvent`, called when a player gains experience, reporting the `amount` gained. It is not called when experience is reset on death.
- Added `PlayerPickupArrowEvent`, called when a player picks up an arrow or a thrown trident from the ground. Cancelling it leaves the projectile on the ground.
- Added `PlayerRecipeBookSettingsChangeEvent`, called when a player opens, closes or filters their recipe book, reporting `recipe_book_type`, `is_open` and `is_filtering`.
- Added `PlayerInputEvent`, called when a player's movement input changes, and the `Input` type it carries: `is_forward`, `is_backward`, `is_left`, `is_right`, `is_jump`, `is_sneak` and `is_sprint`. Like Bukkit's event, it fires only when the input differs from the last one the player sent, not every tick.
- Added `PlayerArmSwingEvent`, called when a player swings their arm, reporting the `item` in their hand. It follows the server's own swing handling, so it also covers swings the server drives itself, such as dropping an item.
- Added `PlayerRiptideEvent`, called when a player activates the riptide enchantment and is propelled by their trident, reporting the `item` used. It fires only once the server has accepted the riptide, so a trident that is merely thrown does not trigger it.
- Added `PlayerArmorStandManipulateEvent`, called when a player swaps, retrieves or places an item on an armor stand, reporting `armor_stand_item`, `player_item` and the affected `slot`. Cancelling it leaves the armor stand untouched.
- Added `PlayerBucketActorEvent`, called when a player captures an actor in a bucket, reporting the `actor` being captured, the `original_bucket` used and the `hand` it was held in. Cancelling it leaves the actor in the world and the bucket unfilled. Bedrock only builds the filled bucket after the capture is committed, so there is no `entity_bucket` as in Bukkit's `PlayerBucketEntityEvent`.
- Added `PlayerShearActorEvent`, called when a player shears an actor, reporting the `actor` being sheared, the `item` used and the `hand` it was held in. Cancelling it leaves the actor unsheared. This is Bukkit's `PlayerShearEntityEvent`, spelled with `Actor` as everywhere else in Endstone; Bedrock rolls the shear loot table only after the interaction is committed, so there is no `drops` list to read or replace.
- Added `ActorCollideWithActorEvent`, called when two actors collide with each other, reporting both in `actors`. This is Paper's `EntityCollideWithEntityEvent`, spelled with `Actor` as everywhere else in Endstone. Cancelling it stops the two from being pushed apart; because the server boards riders on the same code path, it also stops boats and minecarts from being boarded by walking into them. Like Paper's event, it is called before the server decides whether the collision leads to a push, so it also fires for pairs that are never pushed, and more than once per tick while a pair keeps overlapping. It is skipped entirely when no plugin is listening.
- Added `ActorEffectEvent`, called before an effect is applied to a living entity, reporting the `action` taken (currently only `Action.ADDED`) and the `effect` involved. Cancelling it stops the effect being applied, and assigning to `effect` replaces it with a different one.
- Added `ActorPickupItemEvent`, called when a non-player entity picks an item up from the ground, reporting the `item` and the `amount` taken. Cancelling it leaves the item on the ground. Players are covered by `PlayerPickupItemEvent`.
- Added `ActorDismountEvent`, called when an entity stops riding another entity, reporting the `vehicle` being left. Cancelling it keeps the rider aboard.
- Added `ActorChangeBlockEvent`, called when a mob changes a block through its own behaviour, such as a creeper exploding, an enderman picking a block up, a ravager trampling crops or a zombie breaking a door. Cancelling it leaves the block alone. Unlike Bukkit's event of the same name, it is not called for falling blocks landing or for sheep eating grass, and the resulting block state is not available.
- Added `PlayerPickupExperienceEvent`, called when a player picks up an experience orb, reporting the `amount` the orb is worth. Cancelling it leaves the orb in the world.
- Added `PlayerCraftItemEvent`, called when a player crafts an item from a crafting grid or the recipe book, reporting the crafted `item`, the `recipe_id` and the `amount` of crafts. Cancelling it stops the craft and leaves the ingredients untouched.
- Added a `Container` block state for reading and modifying container blocks such as chests, barrels, hoppers, dispensers, droppers, shulker boxes and furnaces, exposing their items via `container.inventory`.
- Added a `CreatureSpawner` block state for monster spawners: `spawned_type`, `delay`, `min_spawn_delay`, `max_spawn_delay`, `spawn_count`, `max_nearby_entities`, `required_player_range` and `spawn_range`, all matching their Bukkit counterparts.
- Added a `Campfire` block state for the four cooking slots of a campfire, with `get_item()`/`set_item()` and `get_cook_time()`/`set_cook_time()`.
- Added a `Lectern` block state exposing the displayed `page`. It extends `Container`, so `lectern.inventory` gives the book it holds.
- Added a `Furnace` block state for furnaces, blast furnaces and smokers. It extends `Container`, so `furnace.inventory` gives the ingredient, fuel and result slots. Bedrock derives the total cook time from the recipe rather than storing it, so there is no `cook_time_total`.
- Added a `Sign` block state for signs and hanging signs, with `sign.get_side(Sign.Side.FRONT)` giving that side's `lines`, `glowing_text` and `color`, and `sign.waxed` controlling whether players can still edit it. Bedrock stores a sign's text colour as a colour rather than a dye, so `color` is a `Color` where Bukkit has a `DyeColor`.
- Added an `ItemFrame` block state for item frames and glow item frames, exposing `item`, `rotation` (a `Rotation`, in 45 degree steps) and `item_drop_chance`. The names match Bukkit's `ItemFrame`, except that item frames are blocks on Bedrock rather than entities.
- Added a `Biome` type and `Block.biome` for reading the biome at a block. Biomes can be looked up with `Biome.get("minecraft:plains")` and enumerated via `server.get_registry(Biome)`.
- Added an effect API: `Mob.add_effect()`, `Mob.remove_effect()`, `Mob.has_effect()`, `Mob.get_effect()` and `Mob.active_effects` apply, remove and query a living entity's status effects. Effects are described by the new `Effect` type, carrying an effect type, duration in ticks, amplifier, and ambient/particles/icon display flags.
- Added `EffectType` and `PotionType` to the registry API, so both can be looked up by name and enumerated with `server.get_registry()`, reporting what the server actually supports. Each entry carries a `translation_key`, and the existing constants are unchanged.
- Added a chunk loading API: `Dimension.load_chunk()`, `Dimension.unload_chunk()` and `Dimension.is_chunk_loaded()`. `load_chunk()` queues the load and pins the chunk as soon as it returns, so `is_chunk_loaded()` may still report `False` on the same tick; `unload_chunk()` releases it again so it can unload once nothing else is keeping it loaded.
- Added `PlayerToggleSneakEvent` and `PlayerToggleSprintEvent`, fired when a player starts or stops sneaking or sprinting, with the new state in `is_sneaking`/`is_sprinting`. Both carry their Bukkit names, so a plugin ported from Spigot or Paper listens for them unchanged.
- Added `ActorToggleSwimEvent` and `ActorToggleGlideEvent`, fired when an actor starts or stops swimming, or starts or stops gliding with an elytra, with the new state in `is_swimming`/`is_gliding`. These are Bukkit's `EntityToggleSwimEvent` and `EntityToggleGlideEvent`, spelled with `Actor` as everywhere else in Endstone.
- Added `Mob.is_swimming`, mirroring Bukkit's `LivingEntity#isSwimming()`, alongside the existing `Mob.is_gliding`.
- Added `Player.is_crawling`. Bukkit has no equivalent because crawling is a pose there, while on Bedrock it is a state the server tracks in its own right, and `PlayerToggleCrawlEvent` already reports it.
- Added `PlayerToggleFlightEvent`, fired when a player starts or stops flying, with the new state in `is_flying`. Like Bukkit's event, it fires only for a player who is allowed to fly.
- Added `PlayerToggleCrawlEvent`, fired when a player starts or stops crawling, with the new state in `is_crawling`. Bukkit has no equivalent because crawling is only a pose there, while on Bedrock it is a state the server tracks in its own right.
- Added `Server.command_map` (`Server::getCommandMap()`), mirroring Paper's `Server#getCommandMap()`, which Bukkit leaves off the interface so plugins there resort to reflection. `CommandMap` is now available from Python with `register_command()`, `dispatch()`, `clear_commands()` and `get_command()`, so a plugin can register a command at runtime rather than declaring it up front.
- `Command` can now be subclassed in Python to override `execute()`, the way Paper plugins subclass `Command`. Previously a Python subclass could be written but its `execute()` was never called.

### Changed

- **BREAKING**: Permission attachments and permissions are handed out as smart handles. `Permissible.add_attachment()` returns one, `Permissible.effective_permissions` is a set of them, and `PluginManager.get_permission()`, `get_default_permissions()` and `permissions` hand back permissions the same way. An attachment you keep no longer stops being safe to touch when whatever it was attached to goes away: it stays readable, its `permissible` reads `None`, and `remove()` reports `False`. Python call sites are unchanged; C++ plugins use `->` where they used `.`, and pass `std::make_shared<Permission>(...)` to `addPermission()` where they passed a `std::unique_ptr`.
- **BREAKING**: `Permissible.add_attachment()` raises `ValueError` when the permission name is empty or the plugin is disabled, instead of logging an error and handing back `None` while its signature promised an attachment. Bukkit rejects both the same way. `Permission.add_parent()` likewise raises when the permission it is called on was never registered, where it used to do nothing at all.
- **BREAKING**: The permission subscription lists hold their subscribers weakly. A player who has disconnected drops out of them on their own once nothing else refers to them, so a plugin that holds on to one no longer has to unsubscribe it by hand. `PluginManager.get_permission_subscriptions()`, `get_default_perm_subscriptions()` and `Permission.permissibles` hand back sets of handles; holding one of those sets keeps everything in it alive, so read it and let it go.
- **BREAKING**: `BroadcastMessageEvent.recipients` is a set of handles, so a listener can keep the set past the event.
- **BREAKING**: `ActorExplodeEvent.block_list` and `BlockExplodeEvent.block_list` now hand back a live `BlockList` instead of a copied `list`, so `block_list.remove(block)`, `.append()`, `.pop()` and `del block_list[i]` change what actually explodes, as they do in Bukkit. Assigning a plain list still works. Note that `remove()` matches on handle identity, so pass a block taken from the list itself rather than one looked up separately.
- **BREAKING**: Every API that handed back a raw `std::unique_ptr` now returns a smart handle whose type states whether it can be absent, matching the nullability its Bukkit counterpart documents. Six of them can be `None` and never said so before: `ItemStack.item_meta`, `ItemFactory.get_item_meta()` and `ItemFactory.as_meta_for()` (all `None` for air), `Scoreboard.get_objective()` (both overloads), `Mob.get_attribute()`, and `PlayerInteractEvent.block` (absent when the player clicked air). A type checker now flags the unchecked `None` in each. The rest - the block, block data, chunk, objective, score, boss bar and item meta accessors on `Block`, `BlockState`, `BlockType`, `Dimension`, `Location`, `BlockCommandSender`, `Objective`, `Scoreboard`, `ItemMeta` and `Server` - are `NotNull`, as are the block and damage-source accessors on the block and actor events. C++ plugins use `->` where they used `.`; Python call sites are unchanged.
- **BREAKING**: Removed `Block::clone()`. It only existed because `std::unique_ptr<Block>` could not be copied; now that blocks are handed out as shared handles, copying the handle does the same thing.
- **BREAKING**: Block states are handed out as smart handles too. `Block::captureState()` returns `NotNull<BlockState>` instead of `std::unique_ptr<BlockState>`, and `BlockGrowEvent::getNewState()` / `BlockPlaceEvent::getBlockReplacedState()` return `const NotNull<BlockState> &` instead of a raw reference, matching Bukkit's `@NotNull` on `Block#getState()` and `BlockPlaceEvent#getBlockReplacedState()`. A captured state can now outlive the call that made it. C++ plugins use `->` where they used `.`; Python is unchanged.
- **BREAKING**: Actors and players are handed out as smart handles everywhere, including from events. `ActorEvent.actor`, `PlayerEvent.player`, `PlayerInteractActorEvent.actor`, `PlayerPickupItemEvent.item` and `BlockBreakEvent.player`/`BlockPlaceEvent.player` return `NotNull`; `ActorKnockbackEvent.source`, `PacketSendEvent.player` and `PacketReceiveEvent.player` return `Nullable`. `Server.online_players`, `Level.actors`, `Dimension.actors`, `BossBar.players` and `PlayerChatEvent.recipients` are lists of handles, and `Dimension.spawn_actor()` returns `Nullable`. Holding an actor past the event is now safe: touching one whose entity is gone raises instead of corrupting memory, so check `is_valid` first. C++ plugins use `->` where they used `.`.
- **BREAKING**: The dimension accessors hand back a smart handle. `Actor::getDimension()`, `Block::getDimension()`, `BlockState::getDimension()` and `Chunk::getDimension()` return `NotNull<Dimension>`; `Level::getDimension()`, `Level::createDimension()` and `MapView::getDimension()` return `Nullable<Dimension>`; `Level::getDimensions()` returns `std::vector<NotNull<Dimension>>`. In Python these are typed `Dimension | None`, so a type checker flags the unchecked `None` that was previously hidden.
- **BREAKING**: `Player::getScoreboard()` returns `NotNull<Scoreboard>` and `Player::setScoreboard()` takes one, while `ServiceManager::registerService()` takes `NotNull<Service>` and `get()`/`load()` return `Nullable<Service>`. In Python, `ServiceManager.load()` is typed `Service | None`, so a missing provider is visible to a type checker.
- **BREAKING**: The command API takes the sender as a smart handle: `Command::execute()`, `CommandExecutor::onCommand()`, `PluginCommand::execute()`, `CommandMap::dispatch()`, `Server::dispatchCommand()` and `Command::testPermission()`/`testPermissionSilently()` take `NotNull<CommandSender>`, and `Server::getCommandSender()` returns `NotNull<ConsoleCommandSender>`. A C++ plugin must update the first parameter of its `onCommand` override or it will quietly stop overriding anything. Python plugins are unaffected.
- **BREAKING**: The remaining raw-reference accessors use handles too: `Actor::teleport()`, `BossBar::addPlayer()`/`removePlayer()`, `MapRenderer::render()`, `ServiceManager::unregister()`, `Server::createMap()`, `MapView::setDimension()`, `Objective::getScoreboard()`, `Score::getScoreboard()`, `Dimension::dropItem()`, and the dimension and sender members of `DimensionEvent`, `PlayerDimensionChangeEvent`, `ScriptMessageEvent` and `ServerCommandEvent`. `Player::getAddress()` returns a reference rather than a fresh copy each call. Python call sites are unchanged.
- **BREAKING**: The last `shared_ptr`-backed types are handles as well. `Scheduler::runTask()` and its five variants return `Nullable<Task>`; `CommandMap::registerCommand()` takes `NotNull<Command>` and `getCommand()` returns `Nullable<Command>`; `PluginCommand::setExecutor()` takes `Nullable<CommandExecutor>`, where a null handle still falls back to the owning plugin; and `MapView`'s renderer methods use `NotNull<MapRenderer>`. In Python, `Scheduler.run_task()` is typed `Task | None`.
- **BREAKING**: `Location.dimension` holds the dimension by a weak reference, mirroring Bukkit's `Location.getWorld()`. It gives `None` when no dimension is set, the dimension when it is loaded, and raises once it has been unloaded; the new `Location.is_dimension_loaded` and `Dimension.is_valid` let you check first.
- **BREAKING**: `Server.level` returns a `Level` and raises if the level has not been loaded yet, instead of returning `None`. Access it only once the level is available.
- **BREAKING**: Moved `EffectType` from `endstone.effect` to `endstone.potion`, mirroring Bukkit; the `endstone.effect` module is gone. In C++ the header moved to `endstone/potion/effect_type.h`.
- **BREAKING**: Replaced the `Dimension::Type` enum with `DimensionId`, a namespaced identifier that supports custom dimensions. Use the `Dimension.OVERWORLD`, `Dimension.NETHER` and `Dimension.THE_END` constants or look up by identifier; `Dimension.name` is now `Dimension.id`.
- **BREAKING**: Replaced `asPlayer()`, `asMob()`, `asConsole()`, `asPluginCommand()` and friends with `as<T>()`/`is<T>()` on all `CommandSender`, `Actor` and `Command` subtypes. In Python, use `isinstance()` as before.
- **BREAKING**: `Player` no longer inherits from `OfflinePlayer`.
- **BREAKING**: `BlockPlaceEvent.block` returns the placed block instead of the replaced block, and `block_placed_state`/`block_replaced` are renamed to `block_placed`/`block_replaced_state` to match Bukkit.
- **BREAKING**: The Docker image stores server data in `/data` instead of `/home/endstone/bedrock_server`. Update your volume mount accordingly (e.g. `-v ./data:/data`).
- **BREAKING**: `Block.type`, `BlockState.type` and `BlockData.type` return a `BlockType` instead of a string, matching `ItemStack.type`. It still compares equal to its `"namespace:key"` string and stringifies to it, but is no longer a `str`.
- `str()` on `BlockType`, `Enchantment` and `ItemType` returns a plain `"namespace:key"` string instead of the underlying `Identifier` repr.
- `Block.set_type`, `BlockState.type`, `Server.create_block_data` and `Inventory.contains`/`contains_at_least`/`all`/`first`/`remove` take an `Identifier` (e.g. `BlockType.AIR`) instead of a plain string, matching `ItemStack.type` and `Dimension.spawn_actor`. Plain `"namespace:key"` strings are still accepted.
- **BREAKING**: `ActorDeathEvent` is now called when a player dies, as well as for every other mob. It previously skipped players, even though `PlayerDeathEvent` derives from it, so a listener on `ActorDeathEvent` never saw them. Bukkit's `EntityDeathEvent` receives player deaths, and this now matches. A listener that assumed it only ever saw non-player mobs should check `actor.type` or listen for `PlayerDeathEvent` instead.
- Dropped Python 3.10 support (end-of-life). Minimum version is now Python 3.11.

### Fixed

- Fixed the server reading freed memory when a `Permissible` was destroyed while still subscribed to a permission, such as a player who disconnected while a plugin still held on to them. The next broadcast or permission recalculation could read from an object that was already gone.
- Fixed the default permission lists holding on to a permission that `PluginManager.remove_permission()` had already freed. As in Bukkit, removing a permission does not take it out of those lists, but it is now kept alive rather than left dangling.
- Fixed `Permission.add_parent()` and `Permission.permissibles` reading an uninitialised field on a permission that was never registered with a plugin manager, which could return nothing or crash depending on the build.
- Fixed type annotations for `Plugin` and `event_handler`.
- Fixed the type stubs so a type checker no longer reports errors on them. `__eq__`/`__ne__` now take `object` as the data model requires, in-place operators such as `Vector.__iadd__` accept everything their binary counterpart does and return `Self`, an optional callback is typed `Callable[...] | None`, a skin image is a `numpy.typing.NDArray`, and `Plugin.config` is a `dict[str, Any]`.
- Fixed the NBT bindings hiding what their containers hold: `ByteArrayTag(iterable)` and `IntArrayTag(iterable)` take an `Iterable[int]`, `ListTag(iterable)` an `Iterable[Tag]`, `CompoundTag(mapping)` a `dict[str, Tag]`, and `to_list()`/`to_dict()` return `list[Any]`/`dict[str, Any]`.
- Fixed `Plugin.default_permission` rejecting a string or a bool (`"operator"`, `"not op"`, `True`), which the `default` of an individual entry in `Plugin.permissions` already accepted. Only `PermissionDefault` worked; anything else raised `TypeError`.
- Fixed `ServerLoadEvent` missing its `ServerEvent` base in Python, so `isinstance(event, ServerEvent)` was `False`, and `LoadType.RELOAD` never being exposed even though the server fires it on `/reload`.
- Fixed every vanilla command failing when dispatched through a `CommandSenderWrapper`. The wrapper did not report its own type, so the command origin could not be resolved from it and the command was refused as an unsupported sender before it ran.
- Fixed `Enchantment.FORTUNE` missing from Python. The constant existed in C++ but was never bound.
- Fixed `Mob.has_attribute()` raising for a name that is not an attribute, instead of answering `False`.
- Fixed `Dimension.actors` and `Level.actors` leaving out connected players. Bedrock keeps players in a separate list from other entities and only the latter was read, so a player was never among the actors of the dimension they were standing in.
- Fixed `str()` on a `Translatable` giving `<endstone._python.lang.Translatable object at 0x...>` instead of its text. Command output captured through a `CommandSenderWrapper` arrives as `Translatable`, so it was unreadable without reaching for `.text` by hand.
- Fixed `PlayerBedLeaveEvent` only being called when a player pressed the leave button in a bed. It now follows the server's own wake-up handling, so it is also called when morning comes, when the bed is broken or otherwise obstructed, and when the player is woken by anything else. The `bed` block it reports is the bed being slept in rather than the player's respawn point.
- Fixed the server crashing on a broadcast once a player had left and rejoined. A plugin that holds on to a `Player` after they disconnect kept them subscribed to broadcasts, and the next chat or quit message aimed at them took the server down. A player's permissions are now cleared when they leave, as the Java edition does, and sending a message to a player who is no longer on the server is a no-op rather than an error.

## [0.11.9] - 2026-08-17

### Added

- Added support for BDS version 1.26.44. The network protocol version is unchanged at 2168, so clients that could join a 1.26.40 server can still join.

- `PlayerInteractEvent` is now fired with the `LEFT_CLICK_AIR` action when a player swings at neither a block nor an actor. The action existed since the event was introduced but was never fired. Cancelling the event suppresses the swing, including its attack sound (#316).

- `endstone.block.BlockType` is now importable from Python.

### Changed

- **BREAKING**: The concrete registry classes are no longer importable from `endstone` (Python). `EnchantmentRegistry` and `ItemRegistry` are gone; obtain a registry through `Server.get_registry()` and annotate it as `Registry[T]`.

### Fixed

- Fixed services outliving the plugin that registered them. Disabling a plugin left its services in the service manager, so after a `/reload` another plugin could still look one up and call into a plugin that is no longer running. For C++ plugins the provider's library has been unloaded by that point, so the call would crash the server. A plugin's services are now unregistered when it is disabled, as the Java edition does.

- Fixed unreadable archives in `logs/`. Sessions that never logged anything are no longer archived, and archives are now written as a single clean gzip stream.

- Fixed the last lines of a session going missing from `logs/`. Roughly a quarter of archives ended mid-line, usually part-way through the plugin shutdown messages. Every message is now flushed as it is written.

- Fixed `Metrics` reporting the emulated architecture instead of the host's (Python), so a plugin running in an emulated container sent the wrong architecture to bStats.

- Fixed ARM servers being reported to bStats as `arm64` on Windows but `aarch64` on Linux, splitting the same hardware across two entries. Both now report `aarch64`.

- Fixed `from endstone import ItemRegistry` raising `AttributeError` (Python). The class bound by the server is `ItemTypeRegistry`.

- Fixed scoreboard score removals being misread by clients older than 1.26.44. Those clients negotiate the same network protocol version as 1.26.44 but expect the older form, which they are now sent.

- Fixed truncated or corrupt NBT being accepted as valid, and well-formed empty lists being rejected as corrupt.

- Fixed `server.properties` never gaining the entries a Bedrock Dedicated Server update adds, such as `transport`, unless you deleted the file and let the server write a fresh one. New entries are now appended along with the comments documenting them, and everything already in the file is left untouched.

- Fixed `Player.address` being unreadable once a player's connection has closed, so a handler that asked during disconnect got nothing back.

- Fixed the server list entry losing its last two fields, currently whether the world is an editor world and whether it is hardcore, whenever a plugin listened for `ServerListPingEvent`. Any field Endstone does not itself expose is now passed through untouched.

## [0.11.8] - 2026-08-07

### Fixed

- Fixed a crash on Windows when reading the item charged into a crossbow, for example through its item meta. The server called into an invalid address instead of the item's loader. Introduced in 0.11.7 along with BDS 1.26.40 support.

- Fixed clients timing out on the connection screen when the world's `LANBroadcast` flag is off. BDS then never publishes the server advertisement, so it answers the client's ping with an empty response, and recent clients refuse to start the connection handshake without a valid one. The advertisement is now restored on startup, using the `server-name` from `server.properties`. Note that a server in this state also starts using the LAN discovery ports (19132/19133) again, as it would have with the flag on. Servers hidden with `enable-lan-visibility=false` are advertised again as well (#423, #465).

## [0.11.7] - 2026-08-05

### Added

- Added support for BDS version 1.26.40.
- Added equality comparison, hashing, and `std::format` support to `endstone::SocketAddress` (C++), so it can be used directly as a key in `std::unordered_map`/`std::unordered_set` and formatted as `hostname:port`.

### Changed

- `/reload` now waits up to 2.5 seconds for running async tasks to finish, then logs a warning naming the plugin ("Nag author(s) ...") and proceeds, matching CraftBukkit. Previously the reload tore plugins down immediately, so a still-running async task could crash the server.
- A shared library in `plugins/` without an entry point is now only reported as an error if its name starts with `endstone_`. `endstone_add_plugin` gives every plugin that prefix, so a prefixed file missing `ENDSTONE_PLUGIN` is still called out by name; anything else is treated as a library a plugin ships alongside itself and skipped quietly. Previously every such file produced a "Did you forget ENDSTONE_PLUGIN?" error, so plugins could not place their own libraries in the folder.
- Log files now show the thread name instead of a numeric thread id. Threads without a name still fall back to the id.
- `endstone_add_plugin` now builds plugins with hidden symbol visibility on Linux, so a plugin exports only its `ENDSTONE_PLUGIN` entry point, as it already did on Windows. This stops a plugin's own symbols, and those of the libraries it bundles, from colliding with the server's copies inside the BDS process. A plugin that deliberately exports more can set `CXX_VISIBILITY_PRESET default` on its target.

### Fixed

- Fixed a resource pack being applied twice when it is both listed in `world_resource_packs.json` and present as an archive in `resource_packs/`. Archives that are already on the world's pack stack are now left alone.

- Fixed memory corruption on Linux whenever a soft enum was updated, which happens every time a command's dynamic choices change. The update packet was written at the wrong offsets and overran the end of the packet.

- Fixed clients being disconnected by a custom map render when the map tracks no entities. The map packet carries its decorations and their tracked actor ids as parallel lists, and the rendered cursors were replacing only the decorations, leaving the two lists at different lengths (#459).

- Fixed events fired from a plugin's `on_load`, or from the `on_enable` of a plugin with `load: startup`, being rejected with "must be triggered synchronously from server thread". Both run before the server thread exists, so the main thread is now reported as the primary thread until it does, matching Spigot.

- Fixed every script log line being followed by a blank line in the console and log file. Script output arrives with a trailing line break, which is now stripped before the message is logged.
- Fixed the `endstone` launcher exiting with `Aborted!` and code 1 when the server is stopped with Ctrl+C. The launcher now lets the server handle Ctrl+C, waits for it to shut down gracefully, and reports its actual exit code.
- Fixed `Scheduler.is_running()` returning the opposite of the truth for async tasks: `True` while the task was idle and `False` while it was actually executing.
- Fixed a class of scheduler crashes and leaks around async tasks: a task submitted to the thread pool could be destroyed while still queued, a task cancelled at the wrong moment could still run, and tasks scheduled from another thread or from inside a task callback could leak or fire one tick early (#436).
- Fixed cancelled tasks holding on to their callbacks until their scheduled tick; they are now released on the next tick, and before plugin libraries are unloaded on `/reload`.
- Fixed `PluginLoader.disable_plugin` in Python enabling the plugin instead of disabling it.
- Fixed an access-violation crash when converting NBT data containing an empty byte array to a string, for example the item NBT of a firework star (#443).
- Fixed death messages for entity and projectile kills always showing the generic "Player died" instead of the detailed vanilla message, such as "Player was slain by Zombie". Death message overrides set on the damage source are now honored as well (#438).
- Fixed C++ plugins failing to compile against the public headers with libc++ 18, which rejected the `std::formatter` specializations due to their declared return type (#437).
- Fixed C++ plugin builds on Linux silently compiling against libstdc++ when Endstone is consumed via CMake FetchContent. The `endstone::endstone` target now propagates `-stdlib=libc++` to every target that links against it, so plugin projects no longer need to pass the flag themselves.
- Fixed the public C++ headers pulling in `<Windows.h>`, which leaked macros such as `min`, `max` and `ERROR` into every plugin that includes an Endstone header. The headers now declare the two Win32 functions they need themselves, and the `endstone::endstone` target defines `NOMINMAX` and `WIN32_LEAN_AND_MEAN`, so a `<Windows.h>` the plugin includes itself stays out of the way too.

## [0.11.6] - 2026-07-10

### Added

- Added support for BDS version 1.26.33.

### Changed

- The interactive console is now enabled by default on all platforms, including Linux (previously Windows-only). Pass `--no-interactive` to disable it, for example when running under a process manager that captures stdin.

### Fixed

- Fixed the Windows standalone bundle's `start.cmd` potentially launching the Microsoft Store Python stub. Both `start.cmd` and `start.sh` now provision a uv-managed Python (`--managed-python`, downloaded on demand) into a reusable `.venv` in the server folder instead of picking up whatever interpreter happens to be on `PATH`, which also makes subsequent startups faster.
- Fixed the interactive console (`-i`) not submitting commands under panels such as Pterodactyl. The console only accepted a line on carriage return, but web consoles send a line feed, so typed commands piled up without ever running. It now accepts either.
- Fixed the crash reporter failing to start with `error while loading shared libraries: libc++-*.so` on Linux (e.g. under the Pterodactyl egg). The crash handler is now loaded from its bundled location instead of being copied into the server folder, so it can find its vendored `libc++` (#429).
- Fixed custom map renderers not being called when the map had no decorations, leaving the map blank instead of showing the rendered image (#426).
- Fixed death messages no longer showing in chat. A new vanilla game rule shifted the internal game-rule indices, so Endstone was reading the wrong rule when deciding whether to broadcast death messages (#424).
- Fixed an access-violation crash on server shutdown when plugins were loaded, caused by plugin loaders being destroyed in the wrong order (#339).

## [0.11.5] - 2026-07-05

### Added

- Added support for BDS version 1.26.32.

### Changed

- **BREAKING**: Standalone bundles now ship for both Windows (`endstone-<version>-windows-x86_64.zip`) and Linux (`endstone-<version>-linux-x86_64.zip`), and use [uv](https://docs.astral.sh/uv/) to provision Python on demand instead of shipping a Python interpreter. The included `start.cmd` / `start.sh` installs uv on first run if it isn't already on `PATH`, then launches the server via `uv run` against the bundled wheel. The old `bin/python/` directory is gone; the server folder (`./bedrock_server/`) is unchanged.
- **BREAKING**: Removed `scripts/autoinstall.sh`. The Linux bundle's `start.sh` now provisions Python via uv on any distro without sudo, superseding the script's apt/dnf/pacman bootstrap.

### Fixed

- Fixed a crash on startup when a behavior pack registered a custom command whose name contained non-ASCII characters, such as Turkish letters (#406).
- Fixed a crash (`resource deadlock would occur`) when reloading the server or cancelling tasks while an asynchronous task scheduled with `run_task_timer_async`/`run_task_later_async` was pending. The plugin no longer fails to re-enable after `/reload` (#351).
- Fixed scheduled tasks ignoring their delay when registered during plugin enable or `ServerLoadEvent` on a world that has already been played for a while — they now correctly wait for the requested delay instead of running immediately on the first tick (#317).
- Fixed a use-after-free crash, reliably reproducible after `/reload`, when a client was disconnected on a connection the server was already closing — the connection-closed teardown no longer runs twice over freed objects (#395).
- Fixed a server crash on Linux when a campfire finished cooking an item.

## [0.11.4] - 2026-05-25

### Added

- Added support for BDS version 1.26.20.

### Fixed

- Fixed optional `message`-type command parameters being received as `['']` instead of `[]` when the argument was omitted.
- Fixed `Player.send_message` silently sending malformed packets when given an empty message. Empty messages are now rejected with an error.

## [0.11.3] - 2026-04-02

### Added

- Added support for BDS version 1.26.12.
- Added `BlockType` to the registry API.
- Added `Lunge` enchantment type for spears.

### Fixed

- Fixed `PacketReceiveEvent` error messages to include the packet ID for easier debugging.
- Fixed `TextPacket` and `InventorySlotPacket` serialization causing bad packet disconnections.
- Fixed `Translatable` messages being sent as raw `Translate` type instead of JSON `TextObject` (#344).
- Fixed several issues in the metrics module.

## [0.11.2] - 2026-03-02

### Added

- Added support for BDS version 1.26.3.1.
- Added `BlockExplodeEvent` for non-actor explosions (e.g., bed in the Nether, respawn anchor in the Overworld).
- Added `Player.send_map()` method for sending full map data to players, enabling custom map renderers to push updates
  on demand (e.g., animated maps via the scheduler).

### Fixed

- Fixed a crash caused by vulnerabilities introduced in Mojang's RakNet modifications (MCPE-228407).
- Fixed missing exports for `BlockFormEvent`, `BlockFromToEvent`, and `BlockGrowEvent` events.

## [0.11.1] - 2026-02-20

### Added

- Added support for BDS version 1.26.1.1.
- Added `ListTag.to_list()` and `CompoundTag.to_dict()` methods (Python) that recursively convert NBT tag trees to
  native Python data structures (`list` and `dict`).
- Exported `attribute`, `effect`, `nbt`, and `potion` submodules from the top-level `endstone` Python package.
- C++ API headers are now self-contained and no longer require additional standard library includes.

### Changed

- **BREAKING**: `ServerListPingEvent.remote_host` and `ServerListPingEvent.remote_port` properties have been replaced by
  a single `ServerListPingEvent.address` property that returns a `SocketAddress`.

### Fixed

- Fixed `PlayerMoveEvent` and `PlayerJumpEvent` not firing.
- Fixed `WeatherChangeEvent` and `ThunderChangeEvent` not firing.
- Fixed `ServerListPingEvent` not firing.
- Fixed `Enchantment` objects being unhashable, preventing their use as dictionary keys and in sets.
- Fixed a duplicate keyword argument error when loading Python plugins that define a `name` class attribute.
- Fixed several issues that caused the Map API to not work correctly in some occasions. Maps now behave consistently
  across different platforms.

## [0.11.0] - 2026-02-13

### Added

- Added support for BDS 1.26.0.
- **NBT API**: Plugins can now read, create, and manipulate NBT data directly. All 11 standard tag types are supported (
  `ByteTag` through `CompoundTag`), with full Python bindings. `CompoundTag` works like a dictionary and `ListTag` like
  a list. Tags can be printed in SNBT format for debugging.
- **ItemStack NBT access**: Read and write NBT data on items using `ItemStack.nbt` (Python) / `ItemStack::getNbt()` and
  `ItemStack::setNbt()` (C++). Useful for inspecting or modifying item data directly.
- **Enchantment API**: All 33 vanilla enchantments are available as named constants. Plugins can check max level,
  conflicts between enchantments, and whether an enchantment can be applied to a given item. Accessible via the new
  Registry system.
- **Map API**: Plugins can now create and customise in-game maps. Draw pixels and images on the map canvas, add map
  cursors (24 types including Player, Mansion, Monument, and TrialChambers), control scale and center position, and
  listen for `MapInitializeEvent` when a new map is created.
- **New events**:
    - `BlockFromToEvent`: fires on liquid flow teleportation. Cancellable.
    - `PlayerPortalEvent`: fires when a player enters a portal, with access to source and destination locations.
      Cancellable.
    - `PlayerDimensionChangeEvent`: fires when a player moves between dimensions (Overworld, Nether, The End).
- **New ItemMeta types**:
    - `BookMeta`: read and write signed book title, author, generation, and pages.
    - `WritableBookMeta`: manage book-and-quill pages.
    - `CrossbowMeta`: inspect and modify loaded crossbow projectiles.
- **Command system**: Commands can now accept entity type arguments with tab completion via the new `entity_type`
  parameter type.
- **Dimension API**: `Dimension.spawn_actor()` creates an entity at a given location. `Dimension.drop_item()` drops an
  item on the ground as a collectible entity.
- **Mob API**: `Mob.max_health` is now writable, allowing plugins to change a mob's maximum health.
- **Interactive console**: New console experience with persistent command history across server restarts. Enabled by
  default on Windows, disabled on Linux. Use `--interactive` / `--no-interactive` CLI flags to override, or set the
  `ENDSTONE_USE_INTERACTIVE_CONSOLE` environment variable.
- **Crash reports**: Server crashes now automatically save a report to `crash_reports/` with cleaned-up stack traces for
  easier debugging.
- **`endstone.asyncio` module**: Run async code from plugins using a background event loop. Call `submit(coro)` to
  schedule a coroutine and get a `Future` back, without blocking the server thread.
- **Registry system**: A unified way to look up game objects (e.g., enchantments) by their identifier. Supports lookup,
  existence checks, and iteration.
- **BossBar**: Added `CREATE_FOG` flag to control whether a boss bar creates fog effects for players.
- **Logging**: Rotated log files are now gzip-compressed to save disk space.
- **`endstone.metrics` module**: Built-in [bStats](https://bstats.org) integration for plugin analytics. Create a
  `Metrics` instance with your plugin and service ID to start reporting. Includes 8 chart types: `SimplePie`,
  `AdvancedPie`, `DrilldownPie`, `SingleLineChart`, `MultiLineChart`, `SimpleBarChart`, `AdvancedBarChart`, and
  `CustomChart`. Data is submitted asynchronously via `endstone.asyncio`.
- **`ItemStack.translation_key`** property for getting an item's localisation key.
- Added support for Python 3.14.

### Changed

- **BREAKING**: `NamespacedKey` has been replaced by `Identifier<T>`, a type-safe template that parses `"namespace:key"`
  strings and defaults to the `"minecraft"` namespace. C++ plugins must update all references. In Python, identifiers
  are simply strings.
- **BREAKING**: `Result<T>` has been removed. API methods now return values directly and throw exceptions on invalid
  input via `Preconditions` checks. These exceptions signal programming errors (e.g., passing invalid arguments).
  Affected areas include health, distance calculations, boss bar progress, colour construction, and scoreboard
  operations. Python plugins are unaffected.
- **BREAKING**: Dimension argument reordered and made optional in several APIs. Call sites may need updating.
- Health methods (`get_health`, `set_health`, `get_max_health`, `set_max_health`) moved from `Actor` to `Mob`, since
  only mobs have health.
- `PlayerDeathEvent` now extends `ActorDeathEvent` instead of `PlayerEvent`.
- Default config file renamed from `endstone.toml` to `endstone.default.toml`.
- Enchantment APIs now use typed `EnchantmentId` constants instead of raw strings.
- Dropped Python 3.9 support (end-of-life). Minimum version is now Python 3.10.
- Crash stack traces are now cleaner and easier to read.
- Suppressed verbose BDS content logs and database auto-compaction messages to reduce log clutter.

### Fixed

- Fixed log files not reopening properly after rotation, which could cause logs to be written to stale files.
- Fixed Python interpreter not being found reliably in multi-Python environments.
- Fixed a crash on server shutdown.
- Fixed plugin loading failing in environments where `pip` is not available (such as `uv`), since `pip` is used at
  runtime to install plugins from file.

## [0.10.18] - 2025-12-11

### Fixed

- Patched a vulnerability where a malformed login packet could crash the server.

## [0.10.17] - 2025-12-11

### Changed

- Verbose content logs are now suppressed.

### Fixed

- Resolved several crashes.

## [0.10.16] - 2025-12-11

### Added

- Added support for BDS version 1.21.130.4.

## [0.10.15] - 2025-11-21

### Added

- Added support for BDS version 1.21.124.2.

## [0.10.14] - 2025-11-18

### Added

- Enable packet rate limiting by default to prevent packet flooding.
- Enabled connection frequency limiting to prevent abuse.

### Fixed

- Patched a vulnerability that allows malicious clients to continue sending packets after the connection is closed.

## [0.10.13] - 2025-11-18

### Fixed

- Resolved a crash that could occur with certain worlds.

## [0.10.12] - 2025-11-18

### Added

- Added support for BDS version 1.21.123.2.

### Fixed

- Resolved a memory leak caused by chunks not unloading properly.
- Patched a vulnerability where a malformed login packet could crash the server.
- Patched a vulnerability where a malformed packet could freeze the server.
- Fixed a crash related to custom dimensions (LeviStone).
- IP bans are now checked before the server processes player login.

## [0.10.11] - 2025-11-06

### Added

- Added support for BDS version 1.21.121.1.

### Changed

- `PlayerChatEvent` is now triggered before Script API's `ChatSendBeforeEvent`.
- Verbose database logs (e.g. `Running AutoCompaction...`) are now suppressed.

### Fixed

- Resolved a crash that could occur when updating the scoreboard of an entity.
- `Player::setWalkSpeed` now correctly sets the player's walk speed.

## [0.10.10] - 2025-11-01

### Fixed

- `PlayerItemHeldEvent` is no longer triggered when the `from` slot is the same as the `to` slot.

## [0.10.9] - 2025-10-30

### Fixed

- Resolved crashes in the chunk-related API.
- Stack traces are no longer shown when the server fails to start due to a port conflict on Windows.
- Fixed an issue where `CommandSenderWrapper` could not capture the output of custom commands.
- Fixed an issue in the `/status` command where permission checks were not performed.
- Fixed a crash that could occur when the Minecraft service was unavailable.

## [0.10.8] - 2025-10-28

### Added

- Added support for BDS version 1.21.120.4.

## [0.10.7] - 2025-10-14

### Added

- Added support for BDS version 1.21.113.1.

### Fixed

- Resolve a crash when joining a server using a grayscale skin with an alpha channel.
- Resolve an issue where item stacks containing blocks could not be given to players.

## [0.10.6] - 2025-10-02

### Added

- Added support for BDS version 1.21.111.1.

## [0.10.5] - 2025-09-26

### Added

- Added support for BDS version 1.21.102.1.
- Added `Level::getSeed` to retrieve the seed of the current level.
- Added the `/seed` command to display the current level's seed.
- Added `PlayerChatEvent::getFormat` and `PlayerChatEvent::setFormat` to customize the message format.

### Changed

- If a command usage includes a parameter of type `message`, it must be the final parameter. No additional parameters
  are allowed after it.
- On Windows, if the automatic loopback exemption fails, a warning will be shown, but the server will no longer stop
  running.

### Fixed

- Fixed a crash that could occur when a sub-client joined.
- Ensured `Inventory::setItem` properly updates the client.
- Item data is now handled correctly during `ItemStack` construction.
- `ItemStack::setCount` now correctly updates the item count instead of clearing the entire stack.
- `PlayerChatEvent::setMessage` now correctly displays the updated message to the client.

## [0.10.4] - 2025-08-10

### Fixed

- Death messages are now correctly displayed on the death screen.
- Resolved a crash that could occur after payload updates in `PacketReceiveEvent`.
- Resolved a crash when running `/reload` after updating native plugins.

## [0.10.3] - 2025-08-06

### Added

- Added support for BDS version 1.21.100.7

## [0.10.2] - 2025-08-05

### Added

- Added support for game version 1.21.100

### Fixed

- Fixed an issue where knockback is still applied when `ActorDamageEvent` is cancelled

## [0.10.1] - 2025-07-31

### Fixed

- Fixed a crash that occurred when parsing a command with an empty message argument.
- Resolve an issue where an actor could be damaged again in their invulnerability window if the damage value from a
  prior `ActorDamageEvent` had been lowered.

## [0.10.0] - 2025-07-30

### Added

- Added support for BDS version 1.21.95.1.
- Added ability to register commands as console-only by setting `console` in the `default` field.
- Added `ItemStack::getData` and `ItemStack::setData` methods for accessing item data values.
- Added `LeavesDecayEvent` by @killcerr in [#209](https://github.com/EndstoneMC/endstone/pull/209)
- Added `BlockCookEvent` by @killcerr in [#212](https://github.com/EndstoneMC/endstone/pull/212)
- Added `PlayerItemHeldEvent` by @killcerr in [#213](https://github.com/EndstoneMC/endstone/pull/213)
- Added `PlayerInteractEvent::Action` enum for distinguishing interaction types.
- Added `BlockPistonExtendEvent` and `BlockPistonRetractEvent` for piston-related events.
- Added `PlayerBedEnterEvent` and `PlayerBedLeaveEvent` for player sleeping events by @dreamguxiang
  in [#196](https://github.com/EndstoneMC/endstone/pull/196)
- Added `Server::getProtocolVersion` to retrieve the supported network protocol version.
- Added `PlayerEmoteEvent::setMuted` to suppress server broadcasts for player emotes.
- Added `PlayerSkinChangeEvent`, triggered when a player changes their in-game skin.
- Added `Player::getEnderChest` to get a player's Ender Chest inventory.
- Added `Item` entity for dropped items.
- Added a new configuration option `commands.log` to `endstone.toml` to control whether the server logs when a player
  executes a command.

### Changed

- `PlayerInteractEvent` now triggers on right- and left-clicks with both air and blocks.
- `PlayerEmoteEvent` is now cancellable; cancelling prevents broadcasting to other players.
- Player bans and IP bans are now checked before the server sends resource packs to clients.
- `PlayerPickupItemEvent::getItem()` now returns an `Item` entity instead of an `ItemStack`.
- **BREAKING**: `Button` class has been moved from the `ActionForm` class to the `endstone.form`
  module.
- **BREAKING**: `PlayerJoinEvent`, `PlayerQuitEvent` and `PlayerDeathEvent` now use `Translatable`
  for messages by default.
- Changed VENV_DIR to "$HOME/.virtualenvs/endstone" in `autoinstall.sh` and `start.sh` to avoid polluting users' home
  directory by @GyanPrakash2483 in [#226](https://github.com/EndstoneMC/endstone/pull/226)

### Fixed

- `PlayerKickEvent` no longer triggers when `PlayerLoginEvent` is cancelled.
- Input (`stdin`) can now be passed down from a parent process when Endstone is running as a subprocess.
- `ActorDamageEvent` now respects the invulnerable time after hurt and will be triggered by damage from a potion.
- Fixed an error that could occur with `ActionForm` when a button's `onClick` is not set.
- Use the correct colour for average TPS in `/status` command output by @MisledWater79
  in [#232](https://github.com/EndstoneMC/endstone/pull/232)
- Fixed a bug where the selector argument got cut short when parsing commands.
- Fixed `Server.name` property in Python API by @cenk in [#220](https://github.com/EndstoneMC/endstone/pull/220)

## [0.9.4] - 2025-07-14

### Added

- Added support for BDS version 1.21.94.2.

## [0.9.3] - 2025-07-04

### Added

- Added support for BDS version 1.21.93.1.

## [0.9.2] - 2025-06-30

### Added

- Added support for BDS version 1.21.92.1.
- Added `ChunkLoadEvent` and `ChunkUnloadEvent`, which are called during chunk loading and unloading.
- Added `PlayerPickupItemEvent`, which is called when a player is about to pick up an item from the ground.

### Changed

- `ActorRemoveEvent` is no longer triggered if `ActorSpawnEvent` is cancelled.
- `PlayerTeleportEvent` is no longer triggered if `PlayerMoveEvent` or `PlayerJumpEvent` is cancelled.

### Fixed

- `Chunk::getZ()` now returns the correct value.
- Fixed a crash when accessing a registry entry multiple times.

## [0.9.1] - 2025-06-19

### Added

- Added support for BDS version 1.21.91.1.

### Fixed

- `ItemType::getTranslationKey` now returns the correct translation key.

## [0.9.0] - 2025-06-17

### Added

- Added support for BDS version 1.21.90.28 (preview).
- Added `PlayerConsumeItemEvent`, triggered when a player consumes an item (e.g., food, milk bucket).
- Added `PlayerMoveEvent`, triggered when a player moves.
- Added `PlayerJumpEvent`, triggered when a player jumps.
- Added the `ItemType` API to retrieve inherent attributes of a known item type, such as max durability.
- Added `Server::getPort` and `Server::getPortV6` to retrieve server ports.
- Expanded the `Inventory` API with additional methods.

### Changed

- **BREAKING**: Renamed various fields in the `Skin` class:
    - `Skin::getSkinImage` -> `Skin::getImage`
    - `Skin::getSkinId` -> `Skin::getId`
- **BREAKING**: `ItemStack::getType` now returns `ItemType` instead of `std::string`. You can still compare an
  `ItemType` with a string.
- **BREAKING**: `PacketSendEvent` and `PacketReceiveEvent` now fire for both pre-login and post-login packets.
  Ensure null safety when accessing the player object, as it may be `null` during pre-login.

## [0.8.2] - 2025-06-07

### Added

- Added support for BDS version 1.21.84.1

### Fixed

- Fixed an issue when parsing arguments in custom commands (#190)
- Ensure only certain server files are overwritten during update (#185)

## [0.8.1] - 2025-05-20

### Added

- Added support for BDS version 1.21.82.1

### Fixed

- Resolve compatibility issues with custom commands registered via the beta Script API (
  `init.customCommandRegistry.registerCommand`).
- Resolve a compatibility issue with LegacyScriptEngine when used in LeviStone, a downstream project.
- Fix [#150](https://github.com/EndstoneMC/endstone/issues/150) by allowing explicit permission grants for non-OP
  players to execute OP commands.
- `ActorDamageEvent` will no longer fire if the `damage_sensor` determines that no damage should be dealt.
- `BlockBreakEvent` will no longer fire when a player uses an item that's not supposed to break blocks in Creative mode.

## [0.8.0] - 2025-05-11

### Added

- Added support for BDS version 1.21.80.3
- Introduced a new Registry API for accessing the built-in definitions (e.g., all available enchantments via
  `Server::getEnchantmentRegistry`)
- Added ability to edit item enchantments via the `ItemMeta` API
- Added ability to get and set an item's repair cost and mark it as unbreakable via the `ItemMeta` API

### Fixed

- C++ plugins can now register event handlers for `PlayerDeathEvent`

## [0.7.3] - 2025-05-06

### Added

- Added support for Minecraft v1.21.80.
- Added divider and header for `ActionForm`.

### Fixed

- Fixed a bug where player initialization could be processed multiple times
  ([#171](https://github.com/EndstoneMC/endstone/issues/171)).
- Death message is now correctly localised on the client side.
- Python interpreter now launches correctly in multiprocessing scenarios.

## [0.7.2] - 2025-03-29

### Added

- Added support for BDS version 1.21.71.01.
- Added `getDamage` and `setDamage` to `ItemMeta` for handling item durability.
- Added `getRuntimeId` to `BlockData` to retrieve the runtime ID of a block permutation (also known as the network hash
  ID).

### Fixed

- Fixed an issue where outgoing packet statistics were not written to `packet-statistics.txt`.

## [0.7.1] - 2025-03-27

### Added

- Added support for BDS version 1.21.70.04.
- Added new `setPayload` method in `PacketSendEvent` and `PacketReceiveEvent` for editing packet contents.
- Introduced a configuration file, `endstone.toml`, for Endstone configurations.

### Changed

- Updated the behavior of server pack enforcement to match vanilla BDS: clients are not allowed to use custom packs when
  `texturepack-required` is set to `true` in `server.properties`. This behavior can be overridden by setting
  `allow-client-packs` in `endstone.toml`.

### Fixed

- Resolved a crash caused by sending packets within the `PacketSendEvent` handler.

## [0.7.0] - 2025-03-25

### Added

- Added support for Minecraft v1.21.70.
- Introduced the `Service` API, enabling plugins to register services that can be utilized by other plugins.
- Implemented dependency resolution for the plugin manager.
- Anonymous enums are now supported in command registration. For example, instead of writing  
  `/home (add|remove)<action: HomeAction>`, you can now simply write `/home <add|remove>`.
- **Inventory API**: Added a basic `ItemMeta` API, allowing you to get and set the display name and lore of an item.
- **Inventory API**: Introduced various methods to get and set items in armor slots as well as main/off-hand slots.
- **Inventory API**: Added methods to get and set the selected hotbar slot.

### Changed

- **BREAKING**: `DataPacketReceiveEvent` and `DataPacketSendEvent` have been renamed to `PacketReceiveEvent`
  and  
  `PacketSendEvent`, respectively. Both events now provide a packet ID and payload instead of a `data` field.
- **BREAKING**: `Player::sendPacket` now requires a packet ID and payload as arguments instead of a `Packet`
  object.

### Fixed

- Fixed a crash caused by a conflict with the event system of the scripting API.

## [0.6.2] - 2025-03-09

### Added

- Added support for Minecraft v1.21.62.
- Added `DataPacketReceiveEvent` and `DataPacketSendEvent` to allow viewing/intercepting low-level packet payloads.
- Added `Actor::remove` to remove an actor from its associated dimension.
- **Python API**: Added `Dimension.get_highest_block_at` to get the top non-air block at a given coordinate.

### Fixed

- Usages are now displayed correctly as percentages in `/status` command output.
- Boss bars are now displayed correctly.
- Death messages are no longer sent when the `SHOW_DEATH_MESSAGES` game rule is set to false.
- Command feedback is now suppressed when the `SEND_COMMAND_FEEDBACK` game rule is set to false.
- `PlayerJoinEvent` is now triggered after the client is fully initialized.
- Crash reporting now works properly on Linux.

## [0.6.1] - 2025-02-20

### Fixed

- Resolved a crash caused by `ProxiedCommandSender`

## [0.6.0] - 2025-02-19

### Added

- Added support for Minecraft v1.21.60.
- Added more liquid-related data to the DevTools by [@smartcmd](https://github.com/smartcmd) in #113.
- Added Sentry for crash reporting.
- Added `Player::playSound`, `Player::stopSound`, and `Player::stopAllSounds` for client-side sound playing.
- Added `Player::isSneaking` and `Player::setSneaking` to toggle the sneaking mode of a player.
- Added `Player::isSprinting` and `Player::setSprinting` to toggle the sprinting mode of a player.
- Added `PlayerGameModeChangeEvent`, triggered when a player's game mode changes. This event is cancellable.
- Added `PlayerEmoteEvent`, triggered when a player sends an emote.
- Added `PlayerRespawnEvent`, triggered when a player respawns after death.
- Added `ActorDamageEvent`, triggered when a living entity receives damage. You can cancel this event to stop the damage
  from being effective.
- Added `ActorExplodeEvent`, triggered when a mob (e.g. Creeper) explodes. You can cancel this event to stop the
  explosion.
- Added more statistics to the `/status` command (e.g., memory usage, chunk status, etc.).
- Added damage source to `ActorDeathEvent` and `PlayerDeathEvent` to retrieve information about the damager.
- Added a basic chunk API.
- Added block collision data to the DevTools by [@smartcmd](https://github.com/smartcmd) in #132.

### Changed

- **Python API**: Events that are cancellable now inherit from `Cancellable`. You can check if an event is cancellable
  by using `isinstance(event, Cancellable)`.
- `Dimension::getBlockAt` now returns a pointer to a `Block` directly instead of a `Result` wrapper, making the
  interface more concise. If the requested block is in an unloaded or unpopulated chunk, it returns an empty block.

### Fixed

- Fixed the logic of finding the Python executable associated with the current interpreter
  by [@killcerr](https://github.com/killcerr) in #114.
- Fixed the logic for parsing the `message` command parameter type.

## [0.5.7.1] - 2024-12-24

### Fixed

- Fixed an issue where the `event.is_cancelled` property in the Python binding was not writable.
- Prevented player command execution from being printed to the console if the `PlayerCommandEvent` was already cancelled
  by plugins.

## [0.5.7] - 2024-12-23

### Added

- Added getter and setter for name tags and score tags in the `Actor` class.
- Added the `Actor::getType` function to retrieve actor types (e.g., `minecraft:pig`).
- Added `isNameTagAlwaysVisible` and `setNameTagAlwaysVisible` methods to the `Actor` class to control the visibility of
  name tags.
- Implemented `/ban-ip` and `/unban-ip` commands.
- Added `Server::getIpBanList` for managing IP bans.
- Added setters for fields in `ServerListPingEvent` by [@theaddonn](https://github.com/theaddonn).
- Added a new color code (`§v`).

### Changed

- Renamed `Server::getPlayerBanList` to `Server::getBanList`.
- Removed deprecated material from DevTools by [@smartcmd](https://github.com/smartcmd).

### Fixed

- Prevented a crash caused by killing a player in a form callback, which invalidated the player object.
- Fixed an issue where the player inventory was not updated after calling `clear`.
- Fixed loading of encrypted resource packs.
- Ensured the correct plugin loader is selected when loading from a file by third-party plugin loaders
  by [@engsr6982](https://github.com/engsr6982).
- Ensured players with matching IP addresses are immediately kicked upon banning.

## [0.5.6] - 2024-12-06

### Added

- Added support for Minecraft v1.21.50 (The Garden Awakens).
- Added `Inventory::getItem` and `Inventory::setItem` methods to manipulate a player's inventory.
- Added `Inventory::getContents` method to retrieve all items in an inventory.
- Added `/ban` and `/unban` commands to add or remove players from the deny list.
- Added `/banlist [ips|players]` command to list all banned players and/or IPs.
- Added `PluginManager::loadPlugin` to allow third-party plugin loaders to load plugins from single files instead of
  directories. [#98](https://github.com/EndstoneMC/endstone/issues/98)

### Changed

- Added support for loading a mixture of folder packs listed in `world_resource_packs.json` (the conventional method)
  and zipped packs in `.zip` or `.mcpack` format placed under `resource_packs`.

### Fixed

- Fixed an issue where `block_palette.nbt` was dumped twice in the DevTools.

## [0.5.5] - 2024-10-26

### Added

- Added support for Minecraft v1.21.40 (Bundles of Bravery).
- Python plugins now receive the correct command sender type (e.g., `Player`) instead of the base type.
- Added the `Objective::isDisplayed` method to check if an objective is being displayed in any of the scoreboard slots.
- Added `Server::getOnlineMode` to check if the server is running with authentication enabled.
- Added `Actor::getScoreboardTags` to get the list of tags added to the actor.
- Added `Actor::addScoreboardTag` and `Actor::removeScoreboardTag` to modify the actor's tags.
- Added support for loading zipped resource packs. You can now put `.zip` and `.mcpack` resource packs under
  the `resource_packs` folder, and they will be loaded automatically when the server starts.
- Added support for encrypted resource packs. You can put the keys in the `.zip.key` or `.mcpack.key` format next to the
  resource packs.
- Added the `ScriptMessageEvent`, which will get called when a script message is sent using the `/scriptevent` command.
- Added `Server::getLanguage` to translate a message into a specific locale.
- Added `Actor::getHealth`, `Actor::setHealth`, and `Actor::getMaxHealth` for getting and setting actor health.
- Added `CommandSenderWrapper` to capture the output of command execution for any command sender.
- Added `Player::spawnParticle` to send a particle effect that is only visible to the player.

### Changed

- Error messages from DevTools, when OpenGL is not available, are now suppressed.
- Several API methods now throw descriptive exceptions on invalid input instead of returning error codes.

### Fixed

- Fixed a crash that occurred while handling server ping
  requests ([#60](https://github.com/EndstoneMC/endstone/issues/60)).

## [0.5.4.1] - 2024-09-19

### Fixed

- Fixed a crash that could occur when the player uses emotes.

## [0.5.4] - 2024-09-19

### Added

- Added support for Minecraft v1.21.30.
- Added support for Python 3.13.
- Added biome data to the DevTools by [@CoolLoong](https://github.com/CoolLoong).

### Changed

- **BREAKING**: `Player::performCommand` now executes the command as a Player to
  resolve [#53](https://github.com/EndstoneMC/endstone/issues/53). This means the command execution will now depend on
  the player's permissions as operator permissions are no longer granted. If you still want to have the player perform a
  command with elevated permissions, consider using `Server::dispatchCommand` to execute
  `execute as {player_name} run {command}`.

### Fixed

- `Actor::isOnGround`, `Actor::isInWater`, and `Actor::isInLava` now return the correct values.
- Fixed a typo in the documentation by [@thatgurkangurk](https://github.com/thatgurkangurk).
- Fixed the command parser logic for chained commands.
- Fixed a crash when spawning a simulated player.

## [0.5.3] - 2024-09-12

### Added

- Added support for Minecraft v1.21.23.
- Introduced the `BlockData` class, which encapsulates both the block type and a specific state from all possible block
  states.
- Block types and block states can now be used as command arguments.
- Added `PlayerKnockbackEvent`, triggered when a player receives knockback. Developers can modify the knockback that the
  player will receive.
- Added `PlayerKickEvent`, triggered when a player is kicked from the server, allowing for customization of kick
  messages.
- Introduced the `BlockState` class, representing a snapshot of a block at a specific point in time.
- `PlayerJoinEvent` and `PlayerQuitEvent` now include getter and setter methods for customizing join/quit messages.
- Integrated bStats metrics.

### Changed

- **BREAKING**: Renamed `Server::getNewScoreboard` to `Server::createScoreboard` for consistency with other
  methods like `createBossbar` and `createBlockData`.
- Lifted the restriction on the `/reload` command for console use. It can now be executed in-game.

### Fixed

- Enforced the use of libc++ for C++ plugins on Linux to resolve symbol errors.
- `Player::setExpProgress` now correctly sets experience progress without precision loss.
- Fixed an issue where death messages were being sent to players twice.
- Resolved a crash when calling `ItemStack::getType` on an item from `PlayerInteractEvent` when the player interacts
  with a block with bare hands.
- `PlayerInteractEvent::hasItem` now correctly returns `false` if the player has no item in hand when interacting.

## [0.5.2] - 2024-08-30

### Added

- Support for Minecraft v1.21.22.
- Boss bar API.
- `Block::getRelative` to get a block relative to a block face.
- `BlockBreakEvent` triggered when a player breaks a block.
- `BlockPlaceEvent` triggered when a player places a block against another block.
- `PlayerInteractEvent` triggered when a player right-clicks a block.
- `PlayerInteractActorEvent` triggered when a player interacts with an actor.
- An auto installer script for installing Endstone on Linux (thanks to [@legitbox](https://github.com/legitbox)).

### Changed

- Check chunk states before getting the block in `Dimension::getBlockAt`.

### Fixed

- [#38](https://github.com/EndstoneMC/endstone/issues/38) - Plugins can now be properly loaded inside a virtual
  environment on Linux.

## [0.5.1] - 2024-08-24

### Added

- Support for Minecraft v1.21.20.
- `Player::getScoreboard` and `Player::setScoreboard` methods to allow each player to have their own scoreboard (
  separate from the main shared scoreboard).
- `PlayerTeleportEvent`, which is called when a player is teleported from one location to another.
- `Actor::teleport` method to teleport an actor to a given location or actor.
- The server can now be gracefully shut down when `Ctrl` + `C` is pressed.
- `/status` command to show the uptime and performance of the server.
- Basic network API to allow plugins to send custom packets. Currently, only `SpawnParticleEffectPacket` is supported.
- `/reload` command to reload plugins in addition to addons.
- `Player::sendToast` method to send a toast message to players.
- Furnace-related item data to DevTools by [@smartcmd](https://github.com/smartcmd).

### Changed

- `Player::isOp` and `Player::setOp` methods are now consistent with the similar methods in the scripting API.
- The fix for the warning message about empty packets is now redundant and has been removed.
- **BREAKING**: `Server::getLevels` has been replaced by `Server::getLevel`.
- **BREAKING**: `Player::getDeviceId` now returns a string object instead of a UUID object, as it is not
  guaranteed to be a UUID on all platforms.

### Fixed

- [#29](https://github.com/EndstoneMC/endstone/issues/29) - Commands will no longer be sent to a player who doesn't have
- [#32](https://github.com/EndstoneMC/endstone/issues/32) - Creating an action form without an `on_submit` callback will
  no longer crashes the server
- Plugin config files are now always read and written in UTF-8 encoding.
- Added a missing parameter to `ActionForm::addButton`.
- Action forms without any buttons can now be displayed correctly on the client side.
- The Python executable can now be located in multiple possible locations on Linux.

## [0.5.0] - 2024-07-29

### Added

- Support for Minecraft v1.21.3.
- `Actor::getId` function to get an actor's unique id, persisting between server runs.
- `Actor::isDead` function to check if an actor is marked for removal post-death.
- `ActorTeleport` event that triggers when an entity teleports (e.g., Enderman, Wolf).
- `Mob::isGliding` function to check if a living entity is gliding.
- `Player::sendTitle` and `Player::resetTitle` functions for sending and resetting an on-screen title.
- `Player::transfer` function to transfer a player to another server.
- `Player::getXuid` to get the Xbox User ID (XUID) of the player.
- Python: package metadata can now be used as a fallback for plugin metadata.
- Python: `isinstance` function can now be used to check if a `CommandSender` is a `Player` or `ConsoleCommandSender`.
- **Experimental** Scoreboard API.
- **Experimental** Form API.

### Changed

- `Player` class now inherits from the `Mob` class instead of `Actor` class.
- **BREAKING**: Plugin names should only contain lowercase letters, numbers, and underscores. Use the prefix
  property to display a different name for the plugin logger.
- **BREAKING**: The `player.ping` property now returns an `int` instead of `datetime.timedelta`.

### Fixed

- Fixed a crash when setting the maximum number of players.
- Fixed issue that python executable cannot be located when running in a virtual environment.

## [0.4.10] - 2024-07-10

### Added

- Support for Minecraft v1.21.2.
- `Player::getSkin` method to retrieve the current skin of a player.
- Support for recipes with output items possessing NBT (Named Binary Tag) tags. These tags are now also included
  in the dumped data.

### Fixed

- Issue with shaped recipes. Recipes that have item tags as input (instead of item names) can now be dumped correctly.

## [0.4.9] - 2024-07-05

### Added

- Support for Minecraft v1.21.1.
- `ActorDeathEvent` and `PlayerDeathEvent` triggers when an actor or a player dies.
- `Player::giveExp` and `Player::giveExpLevels` functionality for adding experience points/levels added.
- `Player::getAllowFlight` and `Player::setAllowFlight` to check or grant the ability for players not in creative mode
  to fly.
- `Player::setFlying` to toggle a player's flying status.
- An overload of `sendMessage` to allow sending of translatable messages with parameters that will be localized on the
  client side.
- `Player::getExpProgress` and `Player::setExpProgress` to get or set a player's progress towards the next level.
- `Player::getExpLevel` and `Player::setExpLevel` to get or set a player's current experience level.
- `Player::getTotalExp` to get the total amount of experiences a player has collected.
- `Player::getLocale` to get the player's current locale.
- `Player::getDeviceId` and `Player::getDeviceOS` to get information related to player's device.
- A DevTools to help community projects get necessary data from BDS (currently only available on Windows). To open the
  DevTools, use command `/devtools` in the console.

### Changed

- `Permissible.op` property in the python binding is renamed to `Permissible.is_op`.
- Crash dumps will now print system and version info in addition to stacktraces.

### Fixed

- Suppressed warning message about empty UDP packets from the output.

## [0.4.8.1] - 2024-06-15

### Added

- Support Minecraft v1.21 (Tricky Trials)

### Fixed

- `Player::getPing` now returns correct value instead of -1 on Linux.
- `Player::isFlying` now works correctly in all conditions.
- Calling python binding of `Server::getPlayer` with uuid no longer crashes.
- Fixed a regression in vanilla command permissions.
- Permission attachment now works as expected.

## [0.4.7] - 2024-06-13

### Added

- `Player::performCommand` allows a player to execute a given command. During the command execution, the player is
  temporarily granted the operator permission.
- `Level::getDimension` and `Level::getDimensions` for getting dimensions within a specific level.
- `Actor::getDimension` for getting the dimension that an actor resides in.
- `Player::getGameMode` and `Player::setGameMode` for getting and setting a player's game mode.
- Plugin configuration for Python plugins.
- World seed will no longer be sent to the client when `client-side-chunk-generation-enabled` is disabled
  in `server.properties`.
- Command permissions are now checked before its parsing.
- `Server::dispatchCommand` allows one to dispatch a command on the server.
- `Server::getMaxPlayers` and `Server::setMaxPlayers` for getting and setting the maximum amount of players allowed to
  be logged in.
- `Server::getPlayer` now also accepts a string argument to get a player by name instead of UUID.

### Changed

- Endstone no longer requires debug symbols at runtime.

### Fixed

- `Actor::getVelocity` now returns the correct velocity when the actor rides a vehicle.
- Python plugin loader now works correctly with Python 3.9.

## [0.4.6] - 2024-05-15

### Fixed

- Improved the detection of Python executable.

## [0.4.5] - 2024-05-14

### Added

- Bootstrap now protects specific files from overwriting during update.
- Python plugin loader now loads all `*.whl` plugins under the `plguins` folder and their dependencies during server
  start up.

### Fixed

- Scheduler now works on Linux.

## [0.4.3] - 2024-05-13

### Added

- `Server::getOnlinePlayers` for getting all online players.
- `Player::kick` for kicking a specific player with a custom message.
- `Player::getPing` for getting the average latency of a connected player.
- `Actor::getLocation` and `Actor::getVelocity` for getting the location and velocity of an actor respectively.
- `Actor::getLevel` for getting the `Level` that an actor resides in.

### Changed

- `Level` is no longer created for each `Dimension` in a Minecraft world. Instead, one `Level` now manages multiple
  `Dimension`s.
- `Player::getAddress` now returns a `SocketAddress` instead of `std::string`. `Player::getPort` was removed,
  use `SocketAddress::getPort` instead.
- Executable `endstone` now accepts `--server-folder` instead of `--install-folder`, and `--no-confirm` instead
  of `--install`.
- Endstone server will now ask the user whether to update the Bedrock Dedicated Server if an older version is found.

## [0.4.2] - 2024-04-30

### Added

- `Level::getDimension` for getting the dimension of a specific level.
- `Player::getAddress` and `Player::getPort` for getting the socket address and port of a specific player, respectively.
- Logs are now written to `logs/latest.log` with rotations.
- Basic scheduler implementation, currently supporting only synchronized task execution.
- `PlayerLoginEvent` is called when a player attempts to log in.

### Changed

- Support for Minecraft v1.20.81 (Bedrock)

### Fixed

- Server no longer crashes when the weather is changed on Linux.

## [0.4.1] - 2024-04-24

### Added

- Commands executed by players are now logged to the console.
- Message of `/say` command will be logged to the console.
- `PlayerCommandEvent` is called when a player attempts to execute a command.
- `Server::broadcastMessage` for broadcasting messages to all players.
- `BroadcastMessageEvent` is called when a message is broadcast by the server.

### Changed

- Support for Minecraft v1.20.80 - Armored Paws (Bedrock)

### Fixed

- Player permissions are recalculated when the op status is changed.
- Incorrect UUID for Players corrected.

## [0.4.0] - 2024-04-20

### Added

- Event system that allows plugins to listen for specific events.
- `ServerLoadEvent` is called when a server is started up.
- `Plugin::registerEventHandler` for registering an event handler in C++ plugins.
- Decorator `@event_handler` for registering an event handler in Python plugins.
- `PluginEnableEvent` and `PluginDisableEvent` are called when a plugin is enabled or disabled.
- `ServerPingListEvent` can be listened to for customizing how the server appears in the client list.
- `ServerCommandEvent` is called when the server executes a command.
- `WeatherChangeEvent` and `ThunderChangeEvent` are called when the weather or thunder status changes in a level.
- Basic interface `endstone::Level` for a level/world.
- Basic interface `endstone::Actor` for entities/actors.
- Basic interface `endstone::Player` for players.
- `Server::getPlayer` for getting a player instance by UUID.
- `Player::sendPopup` and `Player::sendTip` for sending popup and tip messages.
- Stack traces are now printed to the console when an unrecoverable error occurs.
- `PlayerJoinEvent` and `PlayerQuitEvent` are called when a player joins and leaves the server.
- `ActorSpawnEvent` is called when an actor is spawned in the level.
- `ActorRemoveEvent` is called when an actor is removed from the level.
- `PlayerChatEvent` is called when a player sends a message.

### Changed

- Macro `ENDSTONE_PLUGIN` is improved to simplify the definition of plugin metadata.
- `ColorFormat` and `GameMode` have been moved from `endstone.util` to `endstone` in the Python package.
- Release builds now include debug information for stack trace printing.
- Support for Minecraft v1.20.73 (Bedrock)

### Fixed

- Server no longer crashes when `/listd` command is executed.

## [0.3.0] - 2024-03-21

### Added

- Basic permission systems.
- Enforced Plugin API version checks for C++ and Python plugins to ensure ABI/API compatibility.
- Commands are now defined within the plugin metadata which will be automatically registered when the associated plugin
  is enabled.
- PluginDescription properties are extended to support `website`, `load_order`, `depend`, `soft_depend`, `load_before`,
  and `provides`.

### Changed

- Improved `ENDSTONE_PLUGIN` macro to further simplify the definition of plugin metadata.
- Plugin names and websites are now displayed when using `/version [plugin: PluginName]`.
- Support for Minecraft v1.20.72 (Bedrock)

### Fixed

- C++ plugin loader now respects the `prefix` property of a plugin.

## [0.2.0] - 2024-03-19

### Added

- Basic plugin loader for C++ and Python plugins.
- Basic command system that allows plugins to register custom commands.

[Unreleased]: https://github.com/EndstoneMC/endstone/compare/v0.11.9...HEAD
[0.11.9]: https://github.com/EndstoneMC/endstone/compare/v0.11.8...v0.11.9
[0.11.8]: https://github.com/EndstoneMC/endstone/compare/v0.11.7...v0.11.8
[0.11.7]: https://github.com/EndstoneMC/endstone/compare/v0.11.6...v0.11.7
[0.11.6]: https://github.com/EndstoneMC/endstone/compare/v0.11.5...v0.11.6
[0.11.5]: https://github.com/EndstoneMC/endstone/compare/v0.11.4...v0.11.5
[0.11.4]: https://github.com/EndstoneMC/endstone/compare/v0.11.3...v0.11.4
[0.11.3]: https://github.com/EndstoneMC/endstone/compare/v0.11.2...v0.11.3
[0.11.2]: https://github.com/EndstoneMC/endstone/compare/v0.11.1...v0.11.2
[0.11.1]: https://github.com/EndstoneMC/endstone/compare/v0.11.0...v0.11.1
[0.11.0]: https://github.com/EndstoneMC/endstone/compare/v0.10.18...v0.11.0
[0.10.18]: https://github.com/EndstoneMC/endstone/compare/v0.10.17...v0.10.18
[0.10.17]: https://github.com/EndstoneMC/endstone/compare/v0.10.16...v0.10.17
[0.10.16]: https://github.com/EndstoneMC/endstone/compare/v0.10.15...v0.10.16
[0.10.15]: https://github.com/EndstoneMC/endstone/compare/v0.10.14...v0.10.15
[0.10.14]: https://github.com/EndstoneMC/endstone/compare/v0.10.13...v0.10.14
[0.10.13]: https://github.com/EndstoneMC/endstone/compare/v0.10.12...v0.10.13
[0.10.12]: https://github.com/EndstoneMC/endstone/compare/v0.10.11...v0.10.12
[0.10.11]: https://github.com/EndstoneMC/endstone/compare/v0.10.10...v0.10.11
[0.10.10]: https://github.com/EndstoneMC/endstone/compare/v0.10.9...v0.10.10
[0.10.9]: https://github.com/EndstoneMC/endstone/compare/v0.10.8...v0.10.9
[0.10.8]: https://github.com/EndstoneMC/endstone/compare/v0.10.7...v0.10.8
[0.10.7]: https://github.com/EndstoneMC/endstone/compare/v0.10.6...v0.10.7
[0.10.6]: https://github.com/EndstoneMC/endstone/compare/v0.10.5...v0.10.6
[0.10.5]: https://github.com/EndstoneMC/endstone/compare/v0.10.4...v0.10.5
[0.10.4]: https://github.com/EndstoneMC/endstone/compare/v0.10.3...v0.10.4
[0.10.3]: https://github.com/EndstoneMC/endstone/compare/v0.10.2...v0.10.3
[0.10.2]: https://github.com/EndstoneMC/endstone/compare/v0.10.1...v0.10.2
[0.10.1]: https://github.com/EndstoneMC/endstone/compare/v0.10.0...v0.10.1
[0.10.0]: https://github.com/EndstoneMC/endstone/compare/v0.9.4...v0.10.0
[0.9.4]: https://github.com/EndstoneMC/endstone/compare/v0.9.3...v0.9.4
[0.9.3]: https://github.com/EndstoneMC/endstone/compare/v0.9.2...v0.9.3
[0.9.2]: https://github.com/EndstoneMC/endstone/compare/v0.9.1...v0.9.2
[0.9.1]: https://github.com/EndstoneMC/endstone/compare/v0.9.0...v0.9.1
[0.9.0]: https://github.com/EndstoneMC/endstone/compare/v0.8.2...v0.9.0
[0.8.2]: https://github.com/EndstoneMC/endstone/compare/v0.8.1...v0.8.2
[0.8.1]: https://github.com/EndstoneMC/endstone/compare/v0.8.0...v0.8.1
[0.8.0]: https://github.com/EndstoneMC/endstone/compare/v0.7.3...v0.8.0
[0.7.3]: https://github.com/EndstoneMC/endstone/compare/v0.7.2...v0.7.3
[0.7.2]: https://github.com/EndstoneMC/endstone/compare/v0.7.1...v0.7.2
[0.7.1]: https://github.com/EndstoneMC/endstone/compare/v0.7.0...v0.7.1
[0.7.0]: https://github.com/EndstoneMC/endstone/compare/v0.6.2...v0.7.0
[0.6.2]: https://github.com/EndstoneMC/endstone/compare/v0.6.1...v0.6.2
[0.6.1]: https://github.com/EndstoneMC/endstone/compare/v0.6.0...v0.6.1
[0.6.0]: https://github.com/EndstoneMC/endstone/compare/v0.5.7.1...v0.6.0
[0.5.7.1]: https://github.com/EndstoneMC/endstone/compare/v0.5.7...v0.5.7.1
[0.5.7]: https://github.com/EndstoneMC/endstone/compare/v0.5.6...v0.5.7
[0.5.6]: https://github.com/EndstoneMC/endstone/compare/v0.5.5...v0.5.6
[0.5.5]: https://github.com/EndstoneMC/endstone/compare/v0.5.4.1...v0.5.5
[0.5.4.1]: https://github.com/EndstoneMC/endstone/compare/v0.5.4...v0.5.4.1
[0.5.4]: https://github.com/EndstoneMC/endstone/compare/v0.5.3...v0.5.4
[0.5.3]: https://github.com/EndstoneMC/endstone/compare/v0.5.2...v0.5.3
[0.5.2]: https://github.com/EndstoneMC/endstone/compare/v0.5.1...v0.5.2
[0.5.1]: https://github.com/EndstoneMC/endstone/compare/v0.5.0...v0.5.1
[0.5.0]: https://github.com/EndstoneMC/endstone/compare/v0.4.10...v0.5.0
[0.4.10]: https://github.com/EndstoneMC/endstone/compare/v0.4.9...v0.4.10
[0.4.9]: https://github.com/EndstoneMC/endstone/compare/v0.4.8.1...v0.4.9
[0.4.8.1]: https://github.com/EndstoneMC/endstone/compare/v0.4.7...v0.4.8.1
[0.4.7]: https://github.com/EndstoneMC/endstone/compare/v0.4.6...v0.4.7
[0.4.6]: https://github.com/EndstoneMC/endstone/compare/v0.4.5...v0.4.6
[0.4.5]: https://github.com/EndstoneMC/endstone/compare/v0.4.3...v0.4.5
[0.4.3]: https://github.com/EndstoneMC/endstone/compare/v0.4.2...v0.4.3
[0.4.2]: https://github.com/EndstoneMC/endstone/compare/v0.4.1...v0.4.2
[0.4.1]: https://github.com/EndstoneMC/endstone/compare/v0.4.0...v0.4.1
[0.4.0]: https://github.com/EndstoneMC/endstone/compare/v0.3.0...v0.4.0
[0.3.0]: https://github.com/EndstoneMC/endstone/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/EndstoneMC/endstone/releases/tag/v0.2.0
