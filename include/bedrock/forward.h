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
class AABB;
class Abilities;
class ActorAnimationControllerGroup;
class ActorAnimationGroup;
class ActorUnloadedChunkTransferEntry;
class ActorDamageCause;
class ActorDamageSource;
class ActorDefinitionGroup;
class ActorDefinitionIdentifier;
class ActorEvent;
class ActorFactory;
class ActorInteraction;
class AddActorBasePacket;
class AdventureSettings;
class AnimationComponent;
class AutomationBehaviorTreeGroup;
class BaseLightTextureImageBuilder;
class BehaviorFactory;
class BiomeComponentFactory;
class BiomeManager;
class BiomeRegistry;
class Block;
class BlockActor;
class BlockClimberDefinition;
class BlockComponentFactory;
class BlockDefinitionGroup;
class BlockLegacy;
class BlockPos;
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
class CommandOriginData;
class CommandOriginIdentity;
class CommandOriginSystem;
class CommandPositionFloat;
class ComplexInventoryTransaction;
class CompoundTag;
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
class GameRules;
class HandSlot;
class HashedString;
class HitResult;
class IAddActorEntityProxy;
class IConstBlockSource;
class IContainerManager;
class IMinecraftEventing;
class INpcDialogueData;
class IUnknownBlockTypeRegistry;
class InputMode;
class InternalComponentRegistry;
class InventoryTransaction;
class ItemComponentPacket;
class ItemRegistryRef;
class ItemUseMethod;
class ItemStack;
class ItemStackBase;
class JigsawStructureRegistry;
class LayeredAbilities;
class LevelChunk;
class LevelData;
class LevelSettings;
class LevelSoundManager;
class LevelStorage;
class LootTables;
class MapItemSavedData;
class MaterialTypeHelper;
class MobEffectInstance;
class MolangVariableMap;
class NavigationComponent;
class NetEventCallback;
class NewInteractionModel;
class Options;
class Particle;
class Path;
class PauseManager;
class PhotoStorage;
class PlayerListEntry;
class PlayerMovementSettings;
class PortalForcer;
class PositionTrackingDBClient;
class PositionTrackingDBServer;
class PropertyGroupManager;
class PropertiesSettings;
class ProjectileFactory;
class Random;
class Recipes;
class RenderParams;
class ResolvedItemIconInfo;
class ResolvedTextObject;
class SavedDataStorage;
class Scheduler;
class Scoreboard;
class ScreenSetupCleanupHelper;
class ScreenshotOptions;
class SearchQuery;
class SerializedSkin;
class SoundPlayerInterface;
class Spawner;
class StorageVersion;
class StrictEntityContext;
class StructureManager;
class SubChunkPacket;
class SubChunkPos;
class SurfaceBuilderRegistry;
class TextObjectRoot;
class TaskGroup;
class Tick;
class TickingAreaList;
class TickingAreasManager;
class TrackedPacketDataContainer;
class TrimMaterialRegistry;
class TrimPatternRegistry;
class VariantParameterList;
class Vec2;
class WeakEntityRef;
class _TickPtr;  // NOLINT

// Enums
enum class AbilitiesIndex;
enum class ActorFlags;
enum class ActorType;
enum class ArmorMaterialType;
enum class ArmorSlot;
enum class ArmorTextureType;
enum class BedSleepingResult;
enum class GameType;
enum class LevelEvent;
enum class LevelTagIDType;
enum class LevelTagSetIDType;
enum class MaterialType;
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
