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
class ActorSpawnRuleGroup;
class ActorUnloadedChunkTransferEntry;
class AllowList;
class AnimationComponent;
class AppConfigs;
class AppPlatform;
class AsyncJoinTaskManager {};
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
class ContainerRemovedListener;
class ContentCatalogPackSource;
class CopperBehavior;
class CraftingContainer;
class CraftingContext;
class DataLoadHelper;
class DeathTickingComponent;
class DimensionBrightnessRamp;
class DimensionConversionData;
class DimensionManager;
class DynamicContainerTracker;
class EducationLevelSettings;
class EducationOptions;
class EntitySystems;
class EquipmentSlot;
class Experiments;
class FeatureRegistry;
class FeatureTypeFactory;
class FogDefinition;
class FrameUpdateContextBase;
class GameEventDispatcher {};
class GameMode;
class GameModuleServer;
class GetCollisionShapeInterface;
class HudContainerManagerModel;
class I18nObserver;
class IAddActorEntityProxy;
class IContainerRegistryAccess;
class IContentTierManager;
class IFileAccess;
class IFoodItemComponent;
class IGameModuleShared;
class IInPackagePacks;
class ILevelChunkEventManagerProxy {};
class ILevelRandom;
class ILevelStorageManagerConnector;
class IMinecraftEventing;
class INpcDialogueData;
class IUnknownBlockTypeRegistry;
class InternalComponentRegistry;
class IPackLoadContext;
class IServerNetworkController;
class ItemComponent;
class ItemComponentPacket;
class ItemData;
class ItemStackNetManagerBase;
class ItemStackRequestData {};
class IWorldRegistriesProvider;
class JigsawStructureRegistry;
class LevelChunkBuilderData;
class LevelChunkMetaData;
class LevelChunkMetaDataDictionary;
class LevelChunkVolumeData;
class LevelSoundManager;
class LevelStorageObserver;
class LevelStorageWriteBatch;
class LinkedAssetValidator {};
class LocalConnector;
class LootTables;
class MapDataManager;
class MaterialTypeHelper;
class MinecraftGameTest;
class MobAnimationComponent;
class MobHurtTimeComponent;
class MolangVariable;
class MoveInputComponent;
class NavigationComponent;
class NetworkChunkPublisher;
class NetworkSessionOwner;
class NetworkStatistics {};
class Options;
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
class PackedItemUseLegacyInventoryTransaction {};
class PlayerPermissionsManager;
class PlayerPermissionsSynchroniser;
class PlayerDestroyProgressCacheComponent;
class PlayerListener;
class PlayerListEntry;
class PlayerLocationReceiver;
class PlayerMovementSettings;
class PortalForcer;
class PositionTrackingDBClient;
class PositionTrackingDBServer;
class PostprocessingManager;
class PrivateKeyManager;
class PropertyGroupManager;
class PropertiesSettings;
class ProjectileFactory;
class RenderParams;
class RepositoryPacks;
class ResolvedItemIconInfo;
class ResolvedTextObject;
class ResourceDropsContext;
class ResourceLoadManager;
class ResourcePackFileUploadManager;
class ResourcePackListener;
class RopeSystem;
class RuntimeLightingManager;
class SavedDataStorage;
class Scheduler;
class ScreenSetupCleanupHelper;
class ScreenshotOptions;
class SearchQuery;
class Seasons {};
class ServerLocator;
class ServerMetrics;
class SnapshotFilenameAndLength;
class SoundPlayerInterface;
class SpawnConditions;
class SpawnGroupRegistry;
class SpawnSettings;
class StructureManager;
class SubChunkInterlocker;
class SubChunkPacket;
class SubChunkPos;
class SurfaceBuilderRegistry;
class SubpackInfoCollection;
class TextFilteringProcessor;
class TestPackSource;
class TaskGroup {};
class TickingAreaList;
class TickingAreasManager;
class TintMapColor;
class TrackedPacketDataContainer;
class TrackedUniqueChunkPtr;
class TrimMaterialRegistry;
class TrimPatternRegistry;
class UpdateEntityAfterFallOnInterface;
class VolumeEntityManagerServer;
class WorldGenerator;
class WorldHistoryPackSource;
class WorldSessionEndPoint {};
class _TickPtr;  // NOLINT

struct SyncedClientOptionsComponent;

// Enums
enum class ArmorMaterialType;
enum class ArmorTextureType;
enum BlockActorRendererId : std::uint8_t;
enum class BlockRenderLayer : std::uint8_t;
enum class BlockSupportType;
enum class BurnOdds;
enum class ChunkCachedDataState : std::int8_t;
enum class ChunkDebugDisplaySavedState : std::int8_t;
enum class ChunkTerrainDataState : std::int8_t;
enum class ClientPlayMode : uint32_t;
enum class DeviceMemoryTier : int;
enum class FertilizerType;
enum class FlameOdds;
enum class InHandUpdateType : std::int8_t;
enum class ItemColor : std::int8_t;
enum class JsonBetaState;
enum class LavaFlammable;
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
}
namespace cereal {
class ReflectionCtx;
}
namespace cg {
class ImageBuffer;
}
namespace Core {
class FilePathManager;
class FileStorageArea;
class LevelStorageResult;
class StorageAreaState;
class Random {};
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

namespace br::spawn {
class EntityTypeCache;
struct State;
class SpawnPlacements;
}  // namespace br::spawn

namespace MovementDataExtractionUtility {
class SnapshotAccessor;
}

class PacketGroupDefinition {
public:
    struct PacketGroupBuilder;
};

// Template Classes
template <typename A, typename B, typename C>
class OwnerPtrFactory;
template <typename A, typename B, typename C>
class Factory;
template <typename T>
class ServiceReference;
