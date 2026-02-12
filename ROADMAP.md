# Endstone Feature Roadmap: Paper + ScriptAPI Parity

## Context

Endstone already provides a solid Bukkit-like plugin API for Bedrock Dedicated Servers with 60+ events, player/entity
APIs, commands, inventory, permissions, scheduler, scoreboard, forms UI, boss bars, maps, damage system, bans, packet
events, and NBT.

This roadmap identifies all gaps versus Paper (Java) and ScriptAPI (Bedrock) and organizes them into
prioritized phases.

---

## Phase 1: Plugin Developer Essentials

*Remove the most painful friction points for plugin developers. Nearly every non-trivial plugin needs these.*

### 1.1 Configuration API

- YAML/TOML plugin config with `Plugin::saveDefaultConfig()`, `getConfig()`, `reloadConfig()`, `saveConfig()`
- Every plugin needs persistent config; without it, developers roll their own file I/O inconsistently
- `Plugin::getDataFolder()` already exists (`include/endstone/plugin/plugin.h`); add config methods alongside it
- Use `nlohmann_json` (already a dependency) or add TOML/YAML parser

### 1.2 Persistent Data Container / Dynamic Properties

- Store arbitrary typed key-value data on entities, items, and the world that persists across restarts
- **#1 overlapping gap** between Paper (PDC) and ScriptAPI (Dynamic Properties)
- Implement via namespaced NBT subtag (e.g., `endstone:custom_data`) on entities/items
- `ItemStack` already has full NBT (`getNbt()`/`setNbt()`); entities need save/load hooks
- World-level data via `SavedData` in bedrock layer (`src/bedrock/world/level/saveddata/`)
- API: `PersistentDataHolder` interface with typed get/set/has/remove using `NamespacedKey`

### 1.3 Potion Effect Management (Expose Existing Bedrock Layer)

- **Status**: `EffectType` registry exists (26+ types) but **NO methods to add/remove/query effects on entities**
- Add to `Mob`: `addEffect(type, duration, amplifier)`, `removeEffect(type)`, `getEffect(type)`, `getActiveEffects()`
- Create `PotionEffect` wrapper class (duration, amplifier, ambient, particles, icon)
- Bedrock layer has `MobEffect` and `MobEffectInstance` internally -- just needs public API wrappers
- Both Paper and ScriptAPI have this; it's a basic gameplay mechanic

### 1.4 Entity Attribute Access (Expose Existing Bedrock Layer)

- **Status**: `Attribute` registry (17 types), `AttributeModifier` (full with 3 operations), `AttributeInstance` (full)
  all exist but **NO way to get them from an entity**
- Add to `Mob`: `getAttribute(type) -> AttributeInstance`
- Internally, `EndstoneActor` already calls `getMutableAttribute()` for health -- just needs public exposure
- Enables RPG plugins, custom combat, speed modifiers, custom armor values
- Both Paper and ScriptAPI have this

### 1.5 Potion Item Support

- **Status**: `PotionType` registry (28+ types) exists but **NO way to create potion items or apply to ItemStack**
- Create `PotionMeta` ItemMeta subclass with `setBasePotionType()`, `addCustomEffect()`, `getCustomEffects()`
- Support regular potions, splash potions, lingering potions, tipped arrows
- Depends on ItemMeta system (already exists with BookMeta, MapMeta pattern)

### 1.6 Player Visibility / Vanish

- `Player::hidePlayer(Plugin, Player)` / `Player::showPlayer(Plugin, Player)`
- Essential for moderation, spectator systems, vanish plugins
- Use `PlayerListPacket` + entity despawn packets; track visibility per-player in `EndstonePlayer`

---

## Phase 2: Content Creation APIs

*Let plugins add and modify game content: recipes, block entities, rich text, block states.*

### 2.1 Recipe API

- Register custom shaped, shapeless, and furnace recipes at runtime
- Bedrock layer already has full recipe structures (`src/bedrock/world/item/crafting/recipes.h`)
- Send `CraftingDataPacket` to sync recipes to clients
- API: `ShapedRecipe`, `ShapelessRecipe`, `FurnaceRecipe`; `Server::addRecipe()` / `removeRecipe()`

### 2.2 Block Actor (Block Entity) API

- Access/modify data of chests, signs, furnaces, hoppers, beehives, etc.
- `BlockActor` base class exists (`src/bedrock/world/level/block/actor/block_actor.h`) with save/load via `CompoundTag`
- Extend `BlockState` (from `Block::captureState()`) with type-specific subclasses: `Sign`, `Chest`, `Furnace`
- Send update packets after modification

### 2.3 Rich Text / Component API

- Structured text with colors, hover events, click events (run command, open URL)
- Current `Message` type supports only plain strings and translatables
- Bedrock uses JSON text objects (`src/bedrock/textobject/`) in `TextPacket`
- API: `TextComponent` builder pattern; `Player::sendMessage(TextComponent)`
- Consider MiniMessage-like format strings for Python convenience

### 2.4 Chunk API (Expand Minimal Implementation)

- **Status**: `Chunk` class has only 4 getters (`x`, `z`, `level`, `dimension`). `ChunkLoadEvent`/`ChunkUnloadEvent`
  are defined but **not firing** (commented out in `bedrock_hooks/level.cpp`)
- Enable chunk events: wire up `LevelChunkEventManager` connectors (load/reload/discard) in runtime hooks
- Add to `Chunk`: `getBlock(x, y, z)`, `getEntities()`, `isLoaded()`, `isGenerated()`, `isSlimeChunk()`
- Add force-loading: `Chunk::setForceLoaded(bool)`, `Dimension::getForceLoadedChunks()`
- Bedrock has `ChunkSource` with `getOrLoadChunk()`, `saveLevelChunk()`, full `ChunkState` enum (13 states)
- Add async chunk loading: `Dimension::getChunkAtAsync(x, z)` returning future/callback
- Both Paper and ScriptAPI provide rich chunk access; current API is too minimal for world-manipulation plugins

### 2.5 Enhanced Block Permutation API

- Rich block state queries, type-safe property access, permutation iteration
- Current `BlockData` uses raw `unordered_map<string, variant>` -- functional but not discoverable
- Bedrock has complete `BlockState`/`BlockStateInstance`/`BlockStateVariant` system
- Enhance `BlockData` with `getProperties()`, `getProperty(name)`, `setProperty(name, value)`

---

## Phase 3: Commands & Entity Control

*Deep control over how players interact with plugins and how mobs behave.*

### 3.1 Typed Command Arguments (Brigadier-like)

- Type-safe parameters (int, float, player, entity, position, block, item) with tab completion
- Current system uses raw `vector<string>` args -- error-prone, user has to parse manually
- BDS has its own type system for command parsing (already hooked in `bedrock_hooks/minecraft_commands.cpp`)
- Extend API: `CommandBuilder::argument("target", ArgumentType::Player)`
- Add `SoftEnum` registration for custom enums (`update_soft_enum_packet.h` exists)

### 3.2 Entity AI / Pathfinding API

- Access mob AI goals, set pathfinding targets, control behavior
- Enables NPCs, custom mobs, quest systems, tower defense
- Start simple: `Mob::navigateTo(Location)`, `Mob::setTarget(Actor)`, `Mob::lookAt(Actor/Location)`
- Advanced (later): custom goal registration in behavior tree

### 3.3 Vehicle Events

- `VehicleEnterEvent`, `VehicleExitEvent`, `VehicleMoveEvent`, `VehicleDamageEvent`
- Hook into actor riding/dismounting events in BDS
- Add to `include/endstone/event/actor/`

### 3.4 Per-Player Block Changes

- Send fake block changes visible only to specific players
- Use `UpdateBlockPacket` sent per-player (not broadcast)
- API: `Player::sendBlockChange(Location, BlockData)`
- Track fake blocks per player for chunk reload resending

---

## Phase 4: World & Structure APIs

*Large-scale world manipulation and Bedrock-specific features.*

### 4.1 Structure API

- Find, load, save, and place structures programmatically
- BDS has `StructureManager` / `StructureTemplate` (referenced in `src/bedrock/forward.h`)
- API: `StructureManager::createStructure()`, `saveStructure()`, `placeStructure()`, `locateStructure()`
- Leverage `.mcstructure` file format

### 4.2 Camera API (Bedrock-specific)

- Camera attach, fade, perspective control for cutscenes and cinematics
- ScriptAPI has this; differentiates Bedrock from Java
- Reverse-engineer `CameraInstruction` packets
- API: `Player::setCamera(CameraInstruction)` with fade, perspective, attach options

### 4.3 World Border API

- Bedrock has NO native world border -- must implement as soft feature
- Use particle effects for visuals, movement interception for enforcement, damage ticks
- API: `WorldBorder` class with `setCenter()`, `setSize()`, `getDamagePerBlock()`

### 4.4 Loot Table API

- Custom loot tables for blocks, entities, chests, fishing
- Read-only access to existing tables + custom table registration
- Hook into drop events (`ActorDeathEvent`, block break) to override loot

---

## Phase 5: External Communication & Developer Tools

*Connect servers to the outside world and improve DX.*

### 5.1 Plugin Messaging Channels

- Custom byte-array payloads between server and client/proxy
- Enables cross-server communication for Waterdog-style proxies
- API: `Messenger` with `registerIncomingPluginChannel()`, `sendPluginMessage()`

### 5.2 Debug Visualization

- Draw debug shapes (boxes, lines, text) visible to players
- Use particle packets to render shapes
- API: `Player::sendDebugMarker()`, `Player::drawLine(start, end, color, duration)`

### 5.3 Admin/Server Config API

- Server variable and secret management for behavior configuration
- JSON-based secure credential storage (matches ScriptAPI's `@minecraft/server-admin`)

---

## Phase 6: Ecosystem & Advanced (Long-term)

*High-complexity features that complete the vision.*

### 6.1 World Generation API

- Custom chunk generators, biome providers, feature placement
- Extremely complex; BDS generation deeply embedded in `src/bedrock/world/level/levelgen/`
- Start with post-generation population events rather than full generator replacement

### 6.2 Custom Enchantments (Simulated)

- Cannot create truly new enchantments (BDS limitation)
- Simulate via lore text + event listeners for custom behaviors
- Enchantment registry already reads vanilla enchantments

### 6.3 Raid API

- `RaidTriggerEvent`, `RaidFinishEvent`, `RaidStopEvent`
- Hook into BDS raid system events

### 6.4 Entity Component System Access

- Expose BDS ECS components for advanced entity manipulation
- `src/bedrock/entity/components/` has the component system
- Very complex and tightly coupled to BDS internals; long-term goal

---

## Cross-Cutting: Every Feature Requires

1. Public C++ API header in `include/endstone/` (pure virtual interface)
2. Core implementation in `src/endstone/core/` wrapping bedrock types
3. Python bindings in `src/endstone/python/` via pybind11
4. Bedrock hooks in `src/endstone/runtime/bedrock_hooks/` where BDS interception is needed
5. Events for anything observable/cancellable
6. Documentation via mkdocs

## Priority Summary

| Phase | Theme               | Effort      | Impact      | Unlocks                  |
|-------|---------------------|-------------|-------------|--------------------------|
| 1     | Plugin Essentials   | Medium      | Very High   | Every non-trivial plugin |
| 2     | Content Creation    | Medium-High | High        | RPG, custom items, signs |
| 3     | Commands & Entities | High        | High        | Minigames, NPCs, UX      |
| 4     | World & Structures  | High        | Medium-High | Adventure maps, dungeons |
| 5     | External Comms      | Variable    | Medium      | Webhooks, proxies, DX    |
| 6     | Ecosystem Polish    | Very High   | Medium-Low  | Full parity vision       |