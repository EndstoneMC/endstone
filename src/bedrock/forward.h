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

#include <cstdint>

namespace endstone {
class CommandSender;
class Dimension;
class Level;

namespace core {
class EndstoneActor;
class EndstoneLevel;
class EndstoneServer;
}  // namespace core
}  // namespace endstone

// Classes
class ActionQueue;
class ActorAnimationControllerGroup;
class ActorAnimationGroup;
class ActorBlockSyncMessage;
class ActorChunkTransferEntry;
class ActorDefinition;
class ActorDefinitionDescriptor;
class ActorDefinitionDiffList;
class ActorDefinitionGroup;
class ActorEvent;
class ActorFactory;
class ActorInteraction;
class ActorUnloadedChunkTransferEntry;
class AddActorBasePacket;
class AdventureSettings;
class AllowList;
class AnimationComponent;
class AttributeInstanceDelegate;
class AutomationBehaviorTreeGroup;
class BaseLightTextureImageBuilder;
class BehaviorFactory;
class BiomeComponentFactory;
class BiomeManager;
class BlendingDataProvider;
class BlockClimberDefinition;
class BlockComponentFactory;
class BlockDefinitionGroup;
class BlockEventDispatcher;
class BlockGraphicsModeChangeContext;
class BlockItem;
class BlockReducer;
class BlockState;
class BodyControl;
class BossEventSubscriptionManager;
class BoundingBox;
class Bounds;
class CameraPresets;
class ChalkboardBlockActor;
class ChangeDimensionPacket;
class ChunkKey;
class ChunkTickRangeManager;
class ChunkViewSource;
class ClientNetworkSystem;
class CommandOriginSystem;
class CompactionStatus;
class ComplexInventoryTransaction;
class ContainerManagerModel;
class ContainerModel;
class ContainerRemovedListener;
class CopperBehavior;
class CraftingContainer;
class CraftingContext;
class DataLoadHelper;
class Difficulty;
class DimensionBrightnessRamp;
class DimensionConversionData;
class DimensionManager;
class EducationLevelSettings;
class EducationOptions;
class EnderChestContainer;
class EntitySystems;
class EquipmentSlot;
class Experiments;
class FeatureRegistry;
class FeatureTypeFactory;
class FogDefinition;
class FrameUpdateContextBase;
class GameEvent;
class GameMode;
class GameModuleServer;
class GetCollisionShapeInterface;
class HudContainerManagerModel;
class I18nObserver;
class IAddActorEntityProxy;
class IContainerManager;
class IContentTierManager;
class IFoodItemComponent;
class IGameModuleShared;
class ILevelRandom;
class ILevelStorageManagerConnector;
class IMinecraftEventing;
class INpcDialogueData;
class IUnknownBlockTypeRegistry;
class InternalComponentRegistry;
class InventoryTransaction;
class IPackLoadContext;
class ItemComponent;
class ItemComponentPacket;
class ItemData;
class ItemStackNetManagerBase;
class JigsawStructureRegistry;
class LevelChunkBuilderData;
class LevelChunkMetaData;
class LevelChunkMetaDataDictionary;
class LevelData;
class LevelSoundManager;
class LevelStorageObserver;
class LevelStorageWriteBatch;
class LootTables;
class MapDataManager;
class MapItemSavedData;
class MaterialTypeHelper;
class MobAnimationComponent;
class MobHurtTimeComponent;
class MolangVariable;
class NavigationComponent;
class NetworkChunkPublisher;
class NewInteractionModel;
class Options;
class PackManifestFactory;
class PackMetadata;
class PackSettingsFactory;
class PackSourceFactory;
class Particle;
class Path;
class PauseManager;
class PermissionsFile;
class PhotoStorage;
class PlayerDestroyProgressCacheComponent;
class PlayerListener;
class PlayerListEntry;
class PlayerMovementSettings;
class PortalForcer;
class PositionTrackingDBClient;
class PositionTrackingDBServer;
class PostprocessingManager;
class PrivateKeyManager;
class PropertyGroupManager;
class PropertiesSettings;
class ProjectileFactory;
class Random;
class RenderParams;
class ResolvedItemIconInfo;
class ResolvedTextObject;
class ResourceDropsContext;
class ResourceLoadManager;
class ResourcePackListener;
class RopeSystem;
class RuntimeLightingManager;
class SavedDataStorage;
class Scheduler;
class ScreenSetupCleanupHelper;
class ScreenshotOptions;
class SearchQuery;
class SerializedSkin;
class ServerLocator;
class ServerMetrics;
class ServerNetworkSystem;
class SnapshotFilenameAndLength;
class SoundPlayerInterface;
class SpawnConditions;
class Spawner;
class StorageVersion;
class StructureManager;
class SubChunkInterlocker;
class SubChunkPacket;
class SubChunkPos;
class SurfaceBuilderRegistry;
class SubpackInfoCollection;
class TestPackSource;
class TextObjectRoot;
class TaskGroup;
class TickingAreaList;
class TickingAreasManager;
class TrackedPacketDataContainer;
class TrackedUniqueChunkPtr;
class TrimMaterialRegistry;
class TrimPatternRegistry;
class UpdateEntityAfterFallOnInterface;
class _TickPtr;  // NOLINT

// Enums
enum class ArmorMaterialType;
enum class ArmorSlot;
enum class ArmorTextureType;
enum class BedSleepingResult;
enum class BlockActorType;
enum class BlockRenderLayer : std::uint8_t;
enum class BlockSupportType;
enum class BlockTintType;
enum class BurnOdds;
enum class ChunkCachedDataState : std::int8_t;
enum class ChunkDebugDisplaySavedState : std::int8_t;
enum class ChunkTerrainDataState : std::int8_t;
enum class CreativeItemCategory;
enum class FertilizerType;
enum class FlameOdds;
enum class InHandUpdateType : std::int8_t;
enum class ItemColor : std::int8_t;
enum class JsonBetaState;
enum class LavaFlammable;
enum class LevelChunkFormat : std::int8_t;
enum class LevelEvent;
enum class LevelSoundEvent;
enum class NoteBlockInstrument;
enum class ParticleType;
enum class PlayerSleepStatus;
enum class ShapeType : std::int8_t;
enum class StructureFeatureType;

// Nested Classes and Namespaces
class BlockSourceVisitor {
public:
    class CollisionShape;
};

class PositionTrackingDB {
public:
    class PositionTrackingDBClient;
    class PositionTrackingDBServer;
};
namespace Automation {
class AutomationClient;
}
namespace cg {
class ImageBuffer;
}
namespace Core {
class FilePathManager;
class LevelStorageResult;
class StorageAreaState;
}  // namespace Core

namespace Social {
class GamePublishSetting;
}
namespace Puv::Legacy {
enum class LevelSoundEvent : int;
}
namespace Scripting {
class LifetimeRegistryReference;
template <typename T>
class TypedObjectHandle;
}  // namespace Scripting
namespace ScriptModuleMinecraft {
class IScriptWorldBeforeEvents;
class ScriptBlockComponentRegistry;
class ScriptItemComponentRegistry;
class ScriptPropertyRegistry;
}  // namespace ScriptModuleMinecraft

// Template Classes
template <typename A, typename B, typename C>
class OwnerPtrFactory;
template <typename A, typename B, typename C>
class Factory;
