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
class EndstonePlayer;
class EndstoneServer;
class EndstoneServerNetworkEventHandler;
}  // namespace core
}  // namespace endstone

using AttributeMessageType = int64_t;

// Classes
class _TickPtr;  // NOLINT
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
class ActorInteraction;
class ActorRotationComponent;
class ActorSpawnRuleGroup;
class ActorUnloadedChunkTransferEntry;
class AllowList;
class AnimationComponent;
class AppConfigs;
class AppPlatform;
class AsyncJoinTaskManager {};
class AutoCompleteInformation;
class AutomationBehaviorTreeGroup;
class BaseLightTextureImageBuilder;
class BasicTimer {};
class BehaviorFactory;
class BiomeComponentFactory;
class BiomeManager;
class BlendingDataProvider;
class BlockActorDynamicPropertiesComponent {};
class BlockComponentFactory;
class BlockDefinitionGroup;
class BlockEventDispatcher;
class BlockGraphicsModeChangeContext;
class BlockItem;
class BlockReducer;
class BodyControl;
class BossEventSubscriptionManager;
class BoundingBox;
class Bounds;
class CameraPresets;
class ChalkboardBlockActor;
class ChangeDimensionPacket;
class ChestBlockActor;
class ChunkKey;
class ChunkTickRangeManager;
class ChunkViewSource;
class ClassroomModeNetworkHandler;
class ClientNetworkSystem;
class CommandOriginSystem;
class CompactionStatus;
class ComplexInventoryTransaction;
class ContainerManagerModel;
class ContainerModel;
class ContentCatalogPackSource;
class CopperBehavior;
class CraftingContainer;
class CraftingContext;
class DataLoadHelper;
class DeathTickingComponent;
class DimensionBrightnessRamp;
class DimensionConversionData;
class DynamicContainerTracker;
class EducationLevelSettings;
class EducationOptions;
class EntitySystems;
class EquipmentSlot;
class Experiments;
class FeatureRegistry;
class FeatureTypeFactory;
class FileArchiver;
class FogDefinition;
class FrameUpdateContextBase;
class GameEventDispatcher {};
class GameMode;
class GameModuleServer;
class GameTestLevelListener {};
class GetCollisionShapeInterface;
class HudContainerManagerModel;
class I18nObserver;
class IAddActorEntityProxy;
class IContainerRegistryAccess;
class IContainerRegistryTracker;
class IContentTierManager;
class IDynamicContainerSerialization;
class IFileAccess;
class IFoodItemComponent;
class IGameModuleShared;
class IInPackagePacks;
class ILevelChunkEventManagerProxy {};
class ILevelCrashDumpManager;
class ILevelRandom;
class ILevelStorageManagerConnector;
class IMinecraftEventing;
class INpcDialogueData;
class InternalComponentRegistry;
class InvalidPacks {};
class IOptionsReader;
class IServerNetworkController;
class ISubChunkLighter {};
class ItemComponent;
struct ItemComprehensiveLoadResult;
class ItemData;
struct ItemIconInfoFactory;
class ItemStackNetManagerBase;
class ItemStackRequestData {};
class ITickingSystem;
class IUnknownBlockTypeRegistry;
class IWorldRegistriesProvider;
class JigsawStructureRegistry;
class LevelChunkBuilderData;
class LevelChunkMetaData;
class LevelChunkMetaDataDictionary;
class LevelChunkVolumeData;
class LevelEventManager;
class LevelSoundManager;
class LevelStorageObserver;
class LevelStorageWriteBatch;
class LinkedAssetValidator {};
class LocalConnector;
class LootTables;
class MinecraftGameTest {};
class MinecraftServiceKeyManager;
class MobAnimationComponent;
class MobHurtTimeComponent;
class MolangPackSettingsCache;
class MolangVariable;
class MoveInputComponent;
class NavigationComponent;
class NetworkChunkPublisher;
class NetworkSessionOwner;
class NetworkStatistics {};
class Options;
class PackedItemUseLegacyInventoryTransaction {};
class PackLoadContext;
class PackManifestFactory;
class PackMetadata;
class PackSettingsFactory;
class Particle;
class Path;
class PauseManager;
class PermissionsFile;
class PhotoStorage;
class PlayerAbilitiesManager;
class PlayerContainerRefresher;
class PlayerDestroyProgressCacheComponent;
class PlayerLimboActorManager;
class PlayerListener;
class PlayerListEntries;
class PlayerListEntry;
class PlayerLocationReceiver;
class PlayerMovementSettings;
class PlayerPermissionsManager;
class PlayerPermissionsSynchroniser;
class PortalForcer;
class PositionTrackingDBClient;
class PositionTrackingDBServer;
class PostprocessingManager;
class PrivateKeyManager;
class ProjectileFactory;
class PropertyGroupManager;
class RenderParams;
class RepositoryPacks;
class ResolvedItemIconInfo;
class ResolvedTextObject;
class ResourceLoadManager;
class ResourcePackFileUploadManager;
class ResourcePackListener;
class ResourcePackRepositoryRefreshQueue {};
class RopeSystem;
class RuntimeLightingManager;
class SavedDataStorage;
class Scheduler;
class ScreenSetupCleanupHelper;
class ScreenshotOptions;
class ScriptDeferredFlushTracker;
class ScriptPackSettingsCache;
class SearchQuery;
class Seasons {};
class ServerConnectionAuthValidator;
class ServerInstanceEventHandler;
class ServerLocator;
class ServerMetrics;
class ServerPlayer;
class SnapshotFilenameAndLength;
class SoundPlayerInterface;
class SpawnConditions;
class SpawnGroupRegistry;
class SpawnSettings;
class StructureManager;
class SubChunkInterlocker;
class SubChunkPacket;
class SubChunkPos;
class SubpackInfoCollection;
class SurfaceBuilderRegistry;
struct SyncedClientOptionsComponent;
class TaskGroup {};
class TextFilteringProcessor;
class TickingAreaList;
class TickingAreasManager;
class TickTimeManager;
class TintMapColor;
class TrackedUniqueChunkPtr;
class TrimMaterialRegistry;
class TrimPatternRegistry;
class VolumeEntityManagerServer;
class WorldClockRegistry;
class WorldGenerator;
class WorldHistoryPackSource;
class WorldSessionEndPoint {};

// Enums
enum class ArmorMaterialType;
enum class BehaviorStatus;
enum BlockActorRendererId : std::uint8_t;
enum class BlockRenderLayer : std::uint8_t;
enum class BlockSupportType;
enum class ChunkCachedDataState : std::int8_t;
enum class ChunkDebugDisplaySavedState : std::int8_t;
enum class ChunkTerrainDataState : std::int8_t;
enum class ClientPlayMode : uint32_t;
enum class DeviceMemoryTier : int;
enum class FertilizerType;
enum class InHandUpdateType : std::int8_t;
enum class ItemColor : std::int8_t;
enum class ItemCooldownType : unsigned char;
enum class JsonBetaState;
enum class LevelChunkFormat : std::int8_t;
enum class LevelEvent;
enum class NewInteractionModel : int;
enum class NoteBlockInstrument;
enum class ParticleType;
enum class PlatformType : int;
enum class PlayerSleepStatus;
enum class ShowStoreOfferRedirectType : unsigned char;
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
namespace br::worldgen {
struct SpawnerData;
class StructureSetRegistry;
}  // namespace br::worldgen
namespace cereal {
class ReflectionCtx;
}
namespace cg {
class ImageBuffer;
}
namespace Core {
class FilePathManager;
class FileStorageArea;
class FileSystem;
class LevelStorageResult;
class Random {};
class StorageAreaState;
}  // namespace Core
namespace ClientBlobCache {
namespace Server {
class ActiveTransfersManager;
}
}  // namespace ClientBlobCache
namespace SpawnCategory {
enum Type : int32_t;
}
namespace Editor {
class IEditorManager {};
}  // namespace Editor
namespace Social {
class GameConnectionInfo;
class GamePublishSetting;
class MultiplayerServiceManager;
struct Nonce;
}  // namespace Social
namespace Scripting {
class LifetimeRegistryReference;
class RegistryManager {};
template <typename T>
class TypedObjectHandle;
}  // namespace Scripting
namespace ScriptModuleMinecraft {
class IScriptWorldBeforeEvents;
class ScriptBlockComponentRegistry;
class ScriptItemComponentRegistry;
class ScriptPropertyRegistry;
}  // namespace ScriptModuleMinecraft
namespace ResourcePackPathLifetimeHelpers {
class ResourcePackPathCache {};
}  // namespace ResourcePackPathLifetimeHelpers
namespace VoxelShapes {
class VoxelShapeRegistry;
}

namespace br::spawn {
class EntityTypeCache;
class SpawnPlacements;
struct State;
}  // namespace br::spawn

namespace MovementDataExtractionUtility {
class SnapshotAccessor;
}
namespace SharedTypes::Comprehensive {
struct CameraPreset;
}

class PacketGroupDefinition {
public:
    struct PacketGroupBuilder;
};

// Template Classes
template <typename Type, typename... Args>
class Factory;
template <typename Type, typename... Args>
class OwnerPtrFactory;
template <typename T>
class ServiceReference;
