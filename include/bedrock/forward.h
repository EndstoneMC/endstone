// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// Classes
class ActorAnimationControllerGroup;
class ActorAnimationGroup;
class ActorChunkTransferEntry;
class ActorDamageCause;
class ActorDamageSource;
class ActorDefinitionDescriptor;
class ActorDefinitionGroup;
class ActorDefinitionIdentifier;
class ActorEvent;
class ActorFactory;
class ActorInteraction;
class ActorUnloadedChunkTransferEntry;
class AddActorBasePacket;
class AdventureSettings;
class AllowList;
class AnimationComponent;
class AutomationBehaviorTreeGroup;
class BaseLightTextureImageBuilder;
class BehaviorFactory;
class BinaryStream;
class BiomeComponentFactory;
class BiomeManager;
class BiomeRegistry;
class Block;
class BlockActor;
class BlockClimberDefinition;
class BlockComponentFactory;
class BlockDefinitionGroup;
class BlockLegacy;
class BlockPalette;
class BlockReducer;
class BlockSource;
class BlockTypeRegistry;
class BodyControl;
class BossEventSubscriptionManager;
class Bounds;
class CameraPresets;
class ChalkboardBlockActor;
class ChangeDimensionPacket;
class ChangeDimensionRequest;
class ChunkKey;
class ChunkPos;
class ChunkSource;
class ChunkTickRangeManager;
class ChunkViewSource;
class CommandOriginSystem;
class CommandPositionFloat;
class ComplexInventoryTransaction;
class Container;
class CurrentCmdVersion;
class DataLoadHelper;
class Difficulty;
class DimensionConversionData;
class DimensionManager;
class EducationLevelSettings;
class EntitySystems;
class EquipmentSlot;
class Experiments;
class Explosion;
class FeatureRegistry;
class FeatureTypeFactory;
class FrameUpdateContextBase;
class GameModuleServer;
class GameRules;
class HandSlot;
class HitResult;
class I18nObserver;
class IAddActorEntityProxy;
class IConstBlockSource;
class IContainerManager;
class IMinecraftApp;
class IMinecraftEventing;
class INpcDialogueData;
class IUnknownBlockTypeRegistry;
class InputMode;
class InternalComponentRegistry;
class InventoryTransaction;
class Item;
class ItemComponentPacket;
class ItemData;
class ItemInstance;
class ItemRegistryRef;
class ItemUseMethod;
class JigsawStructureRegistry;
class LevelChunk;
class LevelData;
class LevelEventListener;
class LevelSoundManager;
class LevelStorage;
class Localization;
class LootTables;
class MapDataManager;
class MapItemSavedData;
class MaterialTypeHelper;
class MobEffectInstance;
class MolangVariableMap;
class NavigationComponent;
class NewInteractionModel;
class Options;
class PackAccessStrategy;
class PackManifest;
class Particle;
class Path;
class PauseManager;
class PermissionsFile;
class PhotoStorage;
class PlayerMovementSettings;
class PortalForcer;
class PositionTrackingDBClient;
class PositionTrackingDBServer;
class PrivateKeyManager;
class PropertyGroupManager;
class PropertiesSettings;
class ProjectileFactory;
class Random;
class ReadOnlyBinaryStream;
class Recipes;
class RenderParams;
class ResolvedItemIconInfo;
class ResolvedTextObject;
class ResourceLoadManager;
class ResourcePackManager;
class RopeSystem;
class SavedDataStorage;
class Scheduler;
class Scoreboard;
class ScreenSetupCleanupHelper;
class ScreenshotOptions;
class SearchQuery;
class SerializedSkin;
class ServerMetrics;
class SoundPlayerInterface;
class Spawner;
class StorageVersion;
class StructureManager;
class SubChunkPacket;
class SubChunkPos;
class SurfaceBuilderRegistry;
class TextObjectRoot;
class TaskGroup;
class TickingAreaList;
class TickingAreasManager;
class TrackedPacketDataContainer;
class TrimMaterialRegistry;
class TrimPatternRegistry;
class VariantParameterList;
class WeakEntityRef;
class _TickPtr;  // NOLINT

// Enums
enum class ActorType;
enum class ArmorMaterialType;
enum class ArmorSlot;
enum class ArmorTextureType;
enum class BedSleepingResult;
enum class BuildPlatform;
enum class LevelEvent;
enum class LevelTagIDType;
enum class LevelTagSetIDType;
enum class ParticleType;
enum class PlayerSleepStatus;
enum class StructureFeatureType;

// Nested Classes and Namespaces
class PlayerCapabilities {
public:
    class ISharedController;
};
class PositionTrackingDB {
public:
    class PositionTrackingDBClient;
    class PositionTrackingDBServer;
};
namespace cg {
class ImageBuffer;
}

namespace Bedrock::PubSub {
class SubscriptionBase;
}

namespace Social {
class GamePublishSetting;
}
namespace Puv::Legacy {
enum class LevelSoundEvent : int;
}

// Template Classes
template <typename A, typename B, typename C>
class OwnerPtrFactory;
template <typename T>
class StackRefResult;
template <typename A, typename B, typename C>
class Factory;
template <typename A, typename B>
class TagRegistry;
template <typename T>
class IDType;
template <typename T>
using optional_ref = T *;  // NOLINT(*-identifier-naming)
