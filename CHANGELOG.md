# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased](https://github.com/EndstoneMC/endstone/compare/v0.11.1...HEAD)

<small>[Compare with 0.11.1](https://github.com/EndstoneMC/endstone/compare/v0.11.1...HEAD)</small>

### Added

- Added support for BDS version 1.26.2.1.
- Added `BlockExplodeEvent` for non-actor explosions (e.g., bed in the Nether, respawn anchor in the Overworld).

### Fixed

- Fixed a crash caused by vulnerabilities introduced in Mojang's RakNet modifications (MCPE-228407).
- Fixed missing exports for `BlockFormEvent`, `BlockFromToEvent`, and `BlockGrowEvent` events.

## [0.11.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.11.1) - 2026-02-20

<small>[Compare with 0.11.0](https://github.com/EndstoneMC/endstone/compare/v0.11.0...v0.11.1)</small>

### Added

- Added support for BDS version 1.26.1.1.
- Added `ListTag.to_list()` and `CompoundTag.to_dict()` methods (Python) that recursively convert NBT tag trees to
  native Python data structures (`list` and `dict`).
- Exported `attribute`, `effect`, `nbt`, and `potion` submodules from the top-level `endstone` Python package.
- Added explicit standard library `#include` directives across all public C++ API headers for self-contained
  compilation.

### Fixed

- Fixed `PlayerMoveEvent` and `PlayerJumpEvent` not firing.
- Fixed `WeatherChangeEvent` and `ThunderChangeEvent` not firing.
- Fixed `ServerListPingEvent` not firing.
- Fixed `Enchantment` objects being unhashable, preventing their use as dictionary keys and in sets.
- Fixed a duplicate keyword argument error when loading Python plugins that define a `name` class attribute.
- Fixed several issues that caused the Map API to not work correctly in some occasions. Maps now behave consistently
  across different platforms.

### Changed

- **BREAKING**: `ServerListPingEvent.remote_host` and `ServerListPingEvent.remote_port` properties have been replaced by
  a single `ServerListPingEvent.address` property that returns a `SocketAddress`.

## [0.11.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.11.0) - 2026-02-13

<small>[Compare with 0.10.18](https://github.com/EndstoneMC/endstone/compare/v0.10.18...v0.11.0)</small>

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
- Crash stack traces now filter out noise (CRT/runtime frames) and show demangled symbol names.
- Suppressed verbose BDS content logs and database auto-compaction messages to reduce log clutter.

### Fixed

- Fixed log files not reopening properly after rotation, which could cause logs to be written to stale files.
- Fixed Python interpreter not being found reliably in multi-Python environments.
- Fixed a crash on server shutdown caused by C++ destructors running after the Python interpreter was already torn down.
- Fixed plugin loading failing in environments where `pip` is not available (such as `uv`), since `pip` is used at
  runtime to install plugins from file.

## [0.10.18](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.18) - 2025-12-11

<small>[Compare with 0.10.17](https://github.com/EndstoneMC/endstone/compare/v0.10.17...v0.10.18)</small>

### Fixed

- Patched a new vulnerability in BDS where a bad LoginPacket could crash the server (well done mojang, well done).

## [0.10.17](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.17) - 2025-12-11

<small>[Compare with 0.10.16](https://github.com/EndstoneMC/endstone/compare/v0.10.16...v0.10.17)</small>

### Fixed

- Resolved several crashes.

### Changed

- Verbose content logs are now suppressed.

## [0.10.16](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.16) - 2025-12-11

<small>[Compare with 0.10.15](https://github.com/EndstoneMC/endstone/compare/v0.10.15...v0.10.16)</small>

### Added

- Added support for BDS version 1.21.130.4.

## [0.10.15](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.15) - 2025-11-21

<small>[Compare with 0.10.14](https://github.com/EndstoneMC/endstone/compare/v0.10.14...v0.10.15)</small>

### Added

- Added support for BDS version 1.21.124.2.

## [0.10.14](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.14) - 2025-11-18

<small>[Compare with 0.10.13](https://github.com/EndstoneMC/endstone/compare/v0.10.13...v0.10.14)</small>

### Added

- Enable packet rate limiting by default to prevent packet flooding.
- Enable RakNet's connection frequency limiting.

### Fixed

- Patched a vulnerability that allows malicious clients to continue sending packets after the connection is closed.

## [0.10.13](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.13) - 2025-11-18

<small>[Compare with 0.10.12](https://github.com/EndstoneMC/endstone/compare/v0.10.12...v0.10.13)</small>

### Fixed

- Resolved a crash that could occur with certain worlds.

## [0.10.12](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.12) - 2025-11-18

<small>[Compare with 0.10.11](https://github.com/EndstoneMC/endstone/compare/v0.10.11...v0.10.12)</small>

### Added

- Added support for BDS version 1.21.123.2.

### Fixed

- Resolved a memory leak caused by chunks not unloading properly.
- Patched a vulnerability where a crafted LoginPacket with an excessively long certificate chain could crash the server.
- Patched a vulnerability where a malformed InventoryTransactionPacket could freeze the server.
- Fixed a crash related to custom dimensions (LeviStone).
- IP bans are now checked before the server processes player login.

## [0.10.11](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.11) - 2025-11-06

<small>[Compare with 0.10.10](https://github.com/EndstoneMC/endstone/compare/v0.10.10...v0.10.11)</small>

### Added

- Added support for BDS version 1.21.121.1.

### Fixed

- Resolved a crash that could occur when updating the scoreboard of an entity.
- `Player::setWalkSpeed` now correctly sets the player's walk speed.

### Changed

- `PlayerChatEvent` is now triggered before Script API's `ChatSendBeforeEvent`.
- Verbose database logs (e.g. `Running AutoCompaction...`) are now suppressed.

## [0.10.10](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.10) - 2025-11-01

<small>[Compare with 0.10.9](https://github.com/EndstoneMC/endstone/compare/v0.10.9...v0.10.10)</small>

### Fixed

- `PlayerItemHeldEvent` is no longer triggered when the `from` slot is the same as the `to` slot.

## [0.10.9](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.9) - 2025-10-30

<small>[Compare with 0.10.8](https://github.com/EndstoneMC/endstone/compare/v0.10.8...v0.10.9)</small>

### Fixed

- Resolved crashes in the chunk-related API.
- Stack traces are no longer shown when the server fails to start due to a port conflict on Windows.
- Fixed an issue where `CommandSenderWrapper` could not capture the output of custom commands.
- Fixed an issue in the `/status` command where permission checks were not performed.
- Fixed a crash that could occur when the Minecraft service was unavailable.

## [0.10.8](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.8) - 2025-10-28

<small>[Compare with 0.10.7](https://github.com/EndstoneMC/endstone/compare/v0.10.7...v0.10.8)</small>

### Added

- Added support for BDS version 1.21.120.4.

## [0.10.7](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.7) - 2025-10-14

<small>[Compare with 0.10.6](https://github.com/EndstoneMC/endstone/compare/v0.10.6...v0.10.7)</small>

### Added

- Added support for BDS version 1.21.113.1.

### Fixed

- Resolve a crash when joining a server using a grayscale skin with an alpha channel.
- Resolve an issue where item stacks containing blocks could not be given to players.

## [0.10.6](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.6) - 2025-10-02

<small>[Compare with 0.10.5](https://github.com/EndstoneMC/endstone/compare/v0.10.5...v0.10.6)</small>

### Added

- Added support for BDS version 1.21.111.1.

## [0.10.5](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.5) - 2025-09-26

<small>[Compare with 0.10.4](https://github.com/EndstoneMC/endstone/compare/v0.10.4...v0.10.5)</small>

### Added

- Added support for BDS version 1.21.102.1.
- Added `Level::getSeed` to retrieve the seed of the current level.
- Added the `/seed` command to display the current level's seed.
- Added `PlayerChatEvent::getFormat` and `PlayerChatEvent::setFormat` to customize the message format.

### Fixed

- Fixed a crash that could occur when a sub-client joined.
- Ensured `Inventory::setItem` properly updates the client.
- Item data is now handled correctly during `ItemStack` construction.
- `ItemStack::setCount` now correctly updates the item count instead of clearing the entire stack.
- `PlayerChatEvent::setMessage` now correctly displays the updated message to the client.

### Changed

- If a command usage includes a parameter of type `message`, it must be the final parameter. No additional parameters
  are allowed after it.
- On Windows, if the automatic loopback exemption fails, a warning will be shown, but the server will no longer stop
  running.

## [0.10.4](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.4) - 2025-08-10

<small>[Compare with 0.10.3](https://github.com/EndstoneMC/endstone/compare/v0.10.3...v0.10.4)</small>

### Fixed

- Death messages are now correctly displayed on the death screen.
- Resolved a crash that could occur after payload updates in `PacketReceiveEvent`.
- Resolved a crash when running `/reload` after updating native plugins.

## [0.10.3](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.3) - 2025-08-06

<small>[Compare with 0.10.2](https://github.com/EndstoneMC/endstone/compare/v0.10.2...v0.10.3)</small>

### Added

- Added support for BDS version 1.21.100.7

## [0.10.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.2) - 2025-08-05

<small>[Compare with 0.10.1](https://github.com/EndstoneMC/endstone/compare/v0.10.1...v0.10.2)</small>

### Added

- Added support for game version 1.21.100

### Fixed

- Fixed an issue where knockback is still applied when `ActorDamageEvent` is cancelled

## [0.10.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.1) - 2025-07-31

<small>[Compare with 0.10.0](https://github.com/EndstoneMC/endstone/compare/v0.10.0...v0.10.1)</small>

### Fixed

- Fixed a crash that occurred when parsing a command with an empty message argument.
- Resolve an issue where an actor could be damaged again in their invulnerability window if the damage value from a
  prior `ActorDamageEvent` had been lowered.

## [0.10.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.10.0) - 2025-07-30

<small>[Compare with 0.9](https://github.com/EndstoneMC/endstone/compare/v0.9...v0.10.0)</small>

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
- **BREAKING CHANGE (Python API)**: `Button` class has been moved from the `ActionForm` class to the `endstone.form`
  module.
- **BREAKING CHANGE (Python API)**: `PlayerJoinEvent`, `PlayerQuitEvent` and `PlayerDeathEvent` now use `Translatable`
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

## [0.9.4](https://github.com/EndstoneMC/endstone/releases/tag/v0.9.4) - 2025-07-14

<small>[Compare with 0.9.3](https://github.com/EndstoneMC/endstone/compare/v0.9.3...v0.9.4)</small>

### Added

- Added support for BDS version 1.21.94.2.

## [0.9.3](https://github.com/EndstoneMC/endstone/releases/tag/v0.9.3) - 2025-07-04

<small>[Compare with 0.9.2](https://github.com/EndstoneMC/endstone/compare/v0.9.2...v0.9.3)</small>

### Added

- Added support for BDS version 1.21.93.1.

## [0.9.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.9.2) - 2025-06-30

<small>[Compare with 0.9.1](https://github.com/EndstoneMC/endstone/compare/v0.9.1...v0.9.2)</small>

### Added

- Added support for BDS version 1.21.92.1.
- Added `ChunkLoadEvent` and `ChunkUnloadEvent`, which are called during chunk loading and unloading.
- Added `PlayerPickupItemEvent`, which is called when a player is about to pick up an item from the ground.

### Fixed

- `Chunk::getZ()` now returns the correct value.
- Fixed a crash when accessing a registry entry multiple times.

### Changed

- `ActorRemoveEvent` is no longer triggered if `ActorSpawnEvent` is cancelled.
- `PlayerTeleportEvent` is no longer triggered if `PlayerMoveEvent` or `PlayerJumpEvent` is cancelled.

## [0.9.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.9.1) - 2025-06-19

<small>[Compare with 0.9.0](https://github.com/EndstoneMC/endstone/compare/v0.9.0...v0.9.1)</small>

### Added

- Added support for BDS version 1.21.91.1.

### Fixed

- `ItemType::getTranslationKey` now returns the correct translation key.

## [0.9.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.9.0) - 2025-06-17

<small>[Compare with 0.8.2](https://github.com/EndstoneMC/endstone/compare/v0.8.2...v0.9.0)</small>

### Added

- Added support for BDS version 1.21.90.28 (preview).
- Added `PlayerConsumeItemEvent`, triggered when a player consumes an item (e.g., food, milk bucket).
- Added `PlayerMoveEvent`, triggered when a player moves.
- Added `PlayerJumpEvent`, triggered when a player jumps.
- Added the `ItemType` API to retrieve inherent attributes of a known item type, such as max durability.
- Added `Server::getPort` and `Server::getPortV6` to retrieve server ports.
- Expanded the `Inventory` API with additional methods.

### Changed

- **BREAKING CHANGE**: Renamed various fields in the `Skin` class:
    - `Skin::getSkinImage` -> `Skin::getImage`
    - `Skin::getSkinId` -> `Skin::getId`
- **BREAKING CHANGE**: `ItemStack::getType` now returns `ItemType` instead of `std::string`. You can still compare an
  `ItemType` with a string.
- **BREAKING CHANGE**: `PacketSendEvent` and `PacketReceiveEvent` now fire for both pre-login and post-login packets.
  Ensure null safety when accessing the player object, as it may be `null` during pre-login.

## [0.8.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.8.2) - 2025-06-07

<small>[Compare with 0.8.1](https://github.com/EndstoneMC/endstone/compare/v0.8.1...v0.8.2)</small>

### Added

- Added support for BDS version 1.21.84.1

### Fixed

- Fixed an issue when parsing arguments in custom commands (#190)
- Ensure only certain server files are overwritten during update (#185)

## [0.8.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.8.1) - 2025-05-20

<small>[Compare with 0.8.0](https://github.com/EndstoneMC/endstone/compare/v0.8.0...v0.8.1)</small>

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

## [0.8.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.8.0) - 2025-05-11

<small>[Compare with 0.7.3](https://github.com/EndstoneMC/endstone/compare/v0.7.3...v0.8.0)</small>

### Added

- Added support for BDS version 1.21.80.3
- Introduced a new Registry API for accessing the built-in definitions (e.g., all available enchantments via
  `Server::getEnchantmentRegistry`)
- Added ability to edit item enchantments via the `ItemMeta` API
- Added ability to get and set an item's repair cost and mark it as unbreakable via the `ItemMeta` API

### Fixed

- C++ plugins can now register event handlers for `PlayerDeathEvent`

### Changed

- Simplified `endstone::Result` for improved error handling

## [0.7.3](https://github.com/EndstoneMC/endstone/releases/tag/v0.7.3) – 2025-05-06

<small>[Compare with 0.7.2](https://github.com/EndstoneMC/endstone/compare/v0.7.2...v0.7.3)</small>

### Added

- Added support for Minecraft v1.21.80.
- Added divider and header for `ActionForm`.

### Fixed

- Ensure SetLocalPlayerAsInitializedPacket is handled only once when a player
  joins ([#171](https://github.com/EndstoneMC/endstone/issues/171)).
- Death message is now correctly localised on the client side.
- Python interpreter now launches correctly in multiprocessing scenarios.

## [0.7.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.7.2) – 2025-03-29

<small>[Compare with 0.7.1](https://github.com/EndstoneMC/endstone/compare/v0.7.1...v0.7.2)</small>

### Added

- Added support for BDS version 1.21.71.01.
- Added `getDamage` and `setDamage` to `ItemMeta` for handling item durability.
- Added `getRuntimeId` to `BlockData` to retrieve the runtime ID of a block permutation (also known as the network hash
  ID).

### Fixed

- Fixed an issue where outgoing packet statistics were not written to `packet-statistics.txt`.

## [0.7.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.7.1) - 2025-03-27

<small>[Compare with 0.7.0](https://github.com/EndstoneMC/endstone/compare/v0.7.0...v0.7.1)</small>

### Added

- Added support for BDS version 1.21.70.04.
- Added new `setPayload` method in `PacketSendEvent` and `PacketReceiveEvent` for editing packet contents.
- Introduced a configuration file, `endstone.toml`, for Endstone configurations.

### Fixed

- Resolved a crash caused by sending packets within the `PacketSendEvent` handler.

### Changed

- Updated the behavior of server pack enforcement to match vanilla BDS: clients are not allowed to use custom packs when
  `texturepack-required` is set to `true` in `server.properties`. This behavior can be overridden by setting
  `allow-client-packs` in `endstone.toml`.

## [0.7.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.7.0) - 2025-03-25

<small>[Compare with 0.6.2](https://github.com/EndstoneMC/endstone/compare/v0.6.2...v0.7.0)</small>

### Added

- Added support for Minecraft v1.21.70.
- Introduced the `Service` API, enabling plugins to register services that can be utilized by other plugins.
- Implemented dependency resolution for the plugin manager.
- Anonymous enums are now supported in command registration. For example, instead of writing  
  `/home (add|remove)<action: HomeAction>`, you can now simply write `/home <add|remove>`.
- **Inventory API**: Added a basic `ItemMeta` API, allowing you to get and set the display name and lore of an item.
- **Inventory API**: Introduced various methods to get and set items in armor slots as well as main/off-hand slots.
- **Inventory API**: Added methods to get and set the selected hotbar slot.

### Fixed

- Fixed a crash caused by a conflict with the event system of the scripting API.

### Changed

- **BREAKING CHANGE**: `DataPacketReceiveEvent` and `DataPacketSendEvent` have been renamed to `PacketReceiveEvent`
  and  
  `PacketSendEvent`, respectively. Both events now provide a packet ID and payload instead of a `data` field.
- **BREAKING CHANGE**: `Player::sendPacket` now requires a packet ID and payload as arguments instead of a `Packet`
  object.

## [0.6.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.6.2) - 2025-03-09

<small>[Compare with 0.6.1](https://github.com/EndstoneMC/endstone/compare/v0.6.1...v0.6.2)</small>

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

## [0.6.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.6.1) - 2025-02-20

<small>[Compare with 0.6.0](https://github.com/EndstoneMC/endstone/compare/v0.6.0...v0.6.1)</small>

### Fixed

- Resolved a crash caused by `ProxiedCommandSender`

## [0.6.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.6.0) – 2025-02-19

<small>[Compare with 0.5.7.1](https://github.com/EndstoneMC/endstone/compare/v0.5.7.1...v0.6.0)</small>

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

### Fixed

- Fixed the logic of finding the Python executable associated with the current interpreter
  by [@killcerr](https://github.com/killcerr) in #114.
- Fixed the logic for parsing the `message` command parameter type.

### Changed

- **Python API**: Events that are cancellable now inherit from `Cancellable`. You can check if an event is cancellable
  by using `isinstance(event, Cancellable)`.
- `Dimension::getBlockAt` now returns a pointer to a `Block` directly instead of a `Result` wrapper, making the
  interface more concise. If the requested block is in an unloaded or unpopulated chunk, it returns an empty block.

## [0.5.7.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.7.1) - 2024-12-24

<small>[Compare with 0.5.7](https://github.com/EndstoneMC/endstone/compare/v0.5.7...v0.5.7.1)</small>

### Fixed

- Fixed an issue where the `event.is_cancelled` property in the Python binding was not writable.
- Prevented player command execution from being printed to the console if the `PlayerCommandEvent` was already cancelled
  by plugins.

## [0.5.7](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.7) - 2024-12-23

<small>[Compare with 0.5.6](https://github.com/EndstoneMC/endstone/compare/v0.5.6...v0.5.7)</small>

### Added

- Added getter and setter for name tags and score tags in the `Actor` class.
- Added the `Actor::getType` function to retrieve actor types (e.g., `minecraft:pig`).
- Added `isNameTagAlwaysVisible` and `setNameTagAlwaysVisible` methods to the `Actor` class to control the visibility of
  name tags.
- Implemented `/ban-ip` and `/unban-ip` commands.
- Added `Server::getIpBanList` for managing IP bans.
- Added setters for fields in `ServerListPingEvent` by [@theaddonn](https://github.com/theaddonn).
- Added a new color code (`§v`).

### Fixed

- Prevented a crash caused by killing a player in a form callback, which invalidated the player object.
- Fixed an issue where the player inventory was not updated after calling `clear`.
- Fixed loading of encrypted resource packs.
- Ensured the correct plugin loader is selected when loading from a file by third-party plugin loaders
  by [@engsr6982](https://github.com/engsr6982).
- Ensured players with matching IP addresses are immediately kicked upon banning.

### Changed

- Renamed `Server::getPlayerBanList` to `Server::getBanList`.
- Removed deprecated material from DevTools by [@smartcmd](https://github.com/smartcmd).

## [0.5.6](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.6) - 2024-12-06

<small>[Compare with 0.5.5](https://github.com/EndstoneMC/endstone/compare/v0.5.5...v0.5.6)</small>

### Added

- Added support for Minecraft v1.21.50 (The Garden Awakens).
- Added `Inventory::getItem` and `Inventory::setItem` methods to manipulate a player's inventory.
- Added `Inventory::getContents` method to retrieve all items in an inventory.
- Added `/ban` and `/unban` commands to add or remove players from the deny list.
- Added `/banlist [ips|players]` command to list all banned players and/or IPs.
- Added `PluginManager::loadPlugin` to allow third-party plugin loaders to load plugins from single files instead of
  directories. [#98](https://github.com/EndstoneMC/endstone/issues/98)

### Fixed

- Fixed an issue where `block_palette.nbt` was dumped twice in the DevTools.

### Changed

- Added support for loading a mixture of folder packs listed in `world_resource_packs.json` (the conventional method)
  and zipped packs in `.zip` or `.mcpack` format placed under `resource_packs`.

## [0.5.5](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.5) - 2024-10-26

<small>[Compare with 0.5.4.1](https://github.com/EndstoneMC/endstone/compare/v0.5.4.1...v0.5.5)</small>

### Added

- Added support for Minecraft v1.21.40 (Bundles of Bravery).
- CommandSender will now be downcast to the correct derived type on the Python side.
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
- The return type of several methods has been changed to `endstone::Result` to include error messages, which will
  automatically throw exceptions with the same messages on the Python side.

### Fixed

- Fixed a crash that occurred while handling server ping
  requests ([#60](https://github.com/EndstoneMC/endstone/issues/60)).

## [0.5.4.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.4.1) - 2023-09-19

<small>[Compare with 0.5.4](https://github.com/EndstoneMC/endstone/compare/v0.5.4...v0.5.4.1)</small>

### Fixed

- Fixed a crash that could occur when the player uses emotes.

## [0.5.4](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.4) - 2024-09-19

<small>[Compare with 0.5.3](https://github.com/EndstoneMC/endstone/compare/v0.5.3...v0.5.4)</small>

### Added

- Added support for Minecraft v1.21.30.
- Added support for Python 3.13.
- Added biome data to the DevTools by [@CoolLoong](https://github.com/CoolLoong).

### Changed

- **BREAKING CHANGE**: `Player::performCommand` now executes the command as a Player to
  resolve [#53](https://github.com/EndstoneMC/endstone/issues/53). This means the command execution will now depend on
  the player's permissions as operator permissions are no longer granted. If you still want to have the player perform a
  command with elevated permissions, consider using `Server::dispatchCommand` to execute
  `execute as {player_name} run {command}`.

### Fixed

- `Actor::isOnGround`, `Actor::isInWater`, and `Actor::isInLava` now return the correct values.
- Fixed a typo in the documentation by [@thatgurkangurk](https://github.com/thatgurkangurk).
- Fixed the command parser logic for chained commands.
- Fixed a server crash when spawning a simulated player due to the absence of an invalid network identifier.

## [0.5.3](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.3) - 2024-09-12

<small>[Compare with 0.5.2](https://github.com/EndstoneMC/endstone/compare/v0.5.2...v0.5.3)</small>

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

- **BREAKING CHANGE**: Renamed `Server::getNewScoreboard` to `Server::createScoreboard` for consistency with other
  methods like `createBossbar` and `createBlockData`.
- Lifted the restriction on the `/reload` command for console use. It can now be executed in-game.

### Fixed

- Enforced the use of libc++ for C++ plugins on Linux to resolve symbol errors.
- `Player::setExpProgress` now correctly sets experience progress without precision loss.
- Fixed an issue where death messages were being sent to players twice.
- Resolved a crash when calling `ItemStack::getType` on an item from `PlayerInteractEvent` when the player interacts
  with a block with bare hands.
- `PlayerInteractEvent::hasItem` now correctly returns `false` if the player has no item in hand when interacting.

## [0.5.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.2) - 2024-08-30

<small>[Compare with 0.5.1](https://github.com/EndstoneMC/endstone/compare/v0.5.1...v0.5.2)</small>

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

## [0.5.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.1) - 2024-08-24

<small>[Compare with 0.5.0](https://github.com/EndstoneMC/endstone/compare/v0.5.0...v0.5.1)</small>

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
- **BREAKING CHANGE:** `Server::getLevels` has been replaced by `Server::getLevel`.
- **BREAKING CHANGE:** `Player::getDeviceId` now returns a string object instead of a UUID object, as it is not
  guaranteed to be a UUID on all platforms.

### Fixed

- [#29](https://github.com/EndstoneMC/endstone/issues/29) - Commands will no longer be sent to a player who doesn't have
- [#32](https://github.com/EndstoneMC/endstone/issues/32) - Creating an action form without an `on_submit` callback will
  no longer crashes the server
- Plugin config files are now always read and written in UTF-8 encoding.
- Added a missing parameter to `ActionForm::addButton`.
- Action forms without any buttons can now be displayed correctly on the client side.
- The Python executable can now be located in multiple possible locations on Linux.

## [0.5.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.5.0) - 2024-07-29

<small>[Compare with 0.4.10](https://github.com/EndstoneMC/endstone/compare/v0.4.10...v0.5.0)</small>

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
- **BREAKING CHANGE:** Plugin names should only contain lowercase letters, numbers, and underscores. Use the prefix
  property to display a different name for the plugin logger.
- **BREAKING CHANGE:** The `player.ping` property now returns an `int` instead of `datetime.timedelta`.

### Fixed

- Fixed issue where `Server::setMaxPlayers` would crash due to a change in the internal struct in BDS.
- Fixed issue that python executable cannot be located when running in a virtual environment.

## [0.4.10](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.10) - 2024-07-10

<small>[Compare with 0.4.9](https://github.com/EndstoneMC/endstone/compare/v0.4.9...v0.4.10)</small>

### Added

- Support for Minecraft v1.21.2.
- `Player::getSkin` method to retrieve the current skin of a player.
- Support for recipes with output items possessing NBT (Named Binary Tag) tags. These tags are now also included
  in the dumped data.

### Fixed

- Issue with shaped recipes. Recipes that have item tags as input (instead of item names) can now be dumped correctly.

## [0.4.9](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.9) - 2024-07-05

<small>[Compare with 0.4.8.1](https://github.com/EndstoneMC/endstone/compare/v0.4.8.1...v0.4.9)</small>

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

## [0.4.8.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.8.1) - 2024-06-15

<small>[Compare with 0.4.7](https://github.com/EndstoneMC/endstone/compare/v0.4.7...v0.4.8.1)</small>

### Added

- Support Minecraft v1.21 (Tricky Trials)

### Fixed

- `Player::getPing` now returns correct value instead of -1 on Linux.
- `Player::isFlying` now works correctly in all conditions.
- Calling python binding of `Server::getPlayer` with uuid no longer crashes.
- Fixed a regression in vanilla command permissions.
- Permission attachment now works as expected.

## [0.4.7](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.7) - 2024-06-13

<small>[Compare with 0.4.6](https://github.com/EndstoneMC/endstone/compare/v0.4.6...v0.4.7)</small>

### Added

- `Player::performCommand` allows a player to execute a given command. During the command execution, the player is
  temporarily granted the operator permission.
- `Level::getDimension` and `Level::getDimensions` for getting dimensions within a specific level.
- `Actor::getDimension` for getting the dimension that an actor resides in.
- `Player::getGameMode` and `Player::setGameMode` for getting and setting a player's game mode.
- Plugin configuration for Python plugins.
- Added more struct info by [@Duckos-Mods](https://github.com/Duckos-Mods)
  in [#11](https://github.com/EndstoneMC/endstone/pull/11).
- World seed will no longer be sent to the client when `client-side-chunk-generation-enabled` is disabled
  in `server.properties`.
- Command permissions are now checked before its parsing.
- `Server::dispatchCommand` allows one to dispatch a command on the server.
- `Server::getMaxPlayers` and `Server::setMaxPlayers` for getting and setting the maximum amount of players allowed to
  be logged in.
- `Server::getPlayer` now also accepts a string argument to get a player by name instead of UUID.

### Changed

- Changed logging level look-up from using a hashmap to using a switch by [@Duckos-Mods](https://github.com/Duckos-Mods)
  in [#12](https://github.com/EndstoneMC/endstone/pull/12)
- Endstone no longer requires the presence of debug symbols at runtime.

### Fixed

- `Actor::getVelocity` now returns the correct velocity when the actor rides a vehicle.
- Python plugin loader now works correctly with Python 3.9.

## [0.4.6](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.6) - 2024-05-15

<small>[Compare with 0.4.5](https://github.com/EndstoneMC/endstone/compare/v0.4.5...v0.4.6)</small>

### Fixed

- Improved the detection of Python executable.

## [0.4.5](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.5) - 2024-05-14

<small>[Compare with 0.4.3](https://github.com/EndstoneMC/endstone/compare/v0.4.3...v0.4.5)</small>

### Added

- Bootstrap now protects specific files from overwriting during update.
- Python plugin loader now loads all `*.whl` plugins under the `plguins` folder and their dependencies during server
  start up.

### Fixed

- Scheduler now works on Linux.

## [0.4.3](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.3) - 2024-05-13

<small>[Compare with 0.4.2](https://github.com/EndstoneMC/endstone/compare/v0.4.2...v0.4.3)</small>

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

## [0.4.2](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.2) - 2024-04-30

<small>[Compare with 0.4.1](https://github.com/EndstoneMC/endstone/compare/v0.4.1...v0.4.2)</small>

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

## [0.4.1](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.1) - 2024-04-24

<small>[Compare with 0.4.0](https://github.com/EndstoneMC/endstone/compare/v0.4.0...v0.4.1)</small>

This release focuses on supporting game version v1.20.80, which adds several new features.
Additionally, we add a few useful functions to `Player` and `Server`.

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

## [0.4.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.4.0) - 2024-04-20

<small>[Compare with 0.3.0](https://github.com/EndstoneMC/endstone/compare/v0.3.0...v0.4.0)</small>

In this release, we added an event system and several basic interfaces for game objects like `Level`, `Actor`
and `Player`.

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

- Singletons are now managed using `entt::locator`.
- Macro `ENDSTONE_PLUGIN` is improved to simplify the definition of plugin metadata.
- `ColorFormat` and `GameMode` have been moved from `endstone.util` to `endstone` in the Python package.
- Wheels are now built and released with `RelWithDebInfo` configuration to enable stack trace printing.
- Support for Minecraft v1.20.73 (Bedrock)

### Fixed

- Server no longer crashes when `/listd` command is executed.

## [0.3.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.3.0) - 2024-03-21

<small>[Compare with 0.2.0](https://github.com/EndstoneMC/endstone/compare/v0.2.0...v0.3.0)</small>

This is the second release of Endstone with a focus on the permission system and improving the plugin loading
mechanisms.

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

## [0.2.0](https://github.com/EndstoneMC/endstone/releases/tag/v0.2.0) - 2024-03-19

Hello World! This is the first release of Endstone.

### Added

- Basic plugin loader for C++ and Python plugins.
- Basic command system that allows plugins to register custom commands.
