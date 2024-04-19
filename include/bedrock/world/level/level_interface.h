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
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "bedrock/automatic_id.h"
#include "bedrock/bedrock.h"
#include "bedrock/command/command_origin.h"
#include "bedrock/command/command_registry.h"
#include "bedrock/core.h"
#include "bedrock/forward.h"
#include "bedrock/mce.h"
#include "bedrock/memory.h"
#include "bedrock/network/protocol/packet_sender.h"
#include "bedrock/world/actor/components/user_entity_identifier_component.h"
#include "bedrock/world/actor/mob/mob.h"
#include "bedrock/world/actor/player/permissions_handler.h"
#include "bedrock/world/level/event/event_coordinator.h"
#include "bedrock/world/level/level_listener.h"

class ILevel {
public:
    virtual ~ILevel() = default;
    virtual bool initialize(std::string const &, LevelSettings const &, LevelData *, Experiments const &,
                            std::string const *) = 0;
    virtual void startLeaveGame() = 0;
    virtual bool isLeaveGameDone() = 0;
    virtual WeakRef<Dimension> getOrCreateDimension(AutomaticID<Dimension, int>) = 0;
    [[nodiscard]] virtual WeakRef<Dimension> getDimension(AutomaticID<Dimension, int>) const = 0;
    [[nodiscard]] virtual AutomaticID<Dimension, int> getLastOrDefaultSpawnDimensionId(
        AutomaticID<Dimension, int>) const = 0;
    virtual void forEachDimension(std::function<bool(Dimension &)>) = 0;
    virtual void forEachDimension(std::function<bool(Dimension const &)>) const = 0;
    virtual DimensionManager const &getDimensionManager() = 0;
    [[nodiscard]] virtual DimensionManager const &getDimensionManager() const = 0;
    [[nodiscard]] virtual std::uint32_t getChunkTickRange() const = 0;
    [[nodiscard]] virtual ChunkTickRangeManager const &getChunkTickRangeManager() const = 0;
    virtual PortalForcer &getPortalForcer() = 0;
    virtual void requestPlayerChangeDimension(Player &, ChangeDimensionRequest &&) = 0;
    virtual void entityChangeDimension(Actor &, AutomaticID<Dimension, int>, std::optional<Vec3>) = 0;
    [[nodiscard]] virtual Spawner &getSpawner() const = 0;
    virtual Bedrock::NonOwnerPointer<BossEventSubscriptionManager> getBossEventSubscriptionManager() = 0;
    [[nodiscard]] virtual ProjectileFactory &getProjectileFactory() const = 0;
    [[nodiscard]] virtual ActorDefinitionGroup *getEntityDefinitions() const = 0;
    [[nodiscard]] virtual Bedrock::NonOwnerPointer<ActorAnimationGroup> getActorAnimationGroup() const = 0;
    [[nodiscard]] virtual Bedrock::NonOwnerPointer<ActorAnimationControllerGroup> getActorAnimationControllerGroup()
        const = 0;
    [[nodiscard]] virtual BlockDefinitionGroup *getBlockDefinitions() const = 0;
    virtual BlockComponentFactory &getBlockComponentFactory() = 0;
    [[nodiscard]] virtual BlockComponentFactory const &getBlockComponentFactory() const = 0;
    [[nodiscard]] virtual PropertyGroupManager &getActorPropertyGroup() const = 0;
    virtual CameraPresets &getCameraPresets() = 0;
    [[nodiscard]] virtual CameraPresets const &getCameraPresets() const = 0;
    [[nodiscard]] virtual bool getDisablePlayerInteractions() const = 0;
    virtual void setDisablePlayerInteractions(bool) = 0;
    [[nodiscard]] virtual AutomationBehaviorTreeGroup &getAutomationBehaviorTreeGroup() const = 0;
    [[nodiscard]] virtual BehaviorFactory &getBehaviorFactory() const = 0;
    [[nodiscard]] virtual Difficulty getDifficulty() const = 0;
    [[nodiscard]] virtual InternalComponentRegistry &getInternalComponentRegistry() const = 0;
    [[nodiscard]] virtual DimensionConversionData getDimensionConversionData() const = 0;
    [[nodiscard]] virtual float getSpecialMultiplier(AutomaticID<Dimension, int>) const = 0;
    [[nodiscard]] virtual bool hasCommandsEnabled() const = 0;
    [[nodiscard]] virtual bool useMsaGamertagsOnly() const = 0;
    virtual void setMsaGamertagsOnly(bool) = 0;
    virtual Actor *addEntity(BlockSource &, OwnerPtr<EntityContext>) = 0;
    virtual Actor *addGlobalEntity(BlockSource &, OwnerPtr<EntityContext>) = 0;
    virtual Actor *addAutonomousEntity(BlockSource &, OwnerPtr<EntityContext>) = 0;
    virtual void addUser(OwnerPtr<EntityContext>) = 0;
    virtual Actor *addDisplayEntity(BlockSource &, OwnerPtr<EntityContext>) = 0;
    virtual void removeDisplayEntity(WeakEntityRef) = 0;
    virtual void suspendPlayer(Player &) = 0;
    virtual void resumePlayer(Player &) = 0;
    virtual bool isPlayerSuspended(Player &) const = 0;
    virtual OwnerPtr<EntityContext> removeActorAndTakeEntity(WeakEntityRef) = 0;
    virtual OwnerPtr<EntityContext> removeActorFromWorldAndTakeEntity(WeakEntityRef) = 0;
    virtual OwnerPtr<EntityContext> takeEntity(WeakEntityRef, LevelChunk &) = 0;
    [[nodiscard]] virtual StrictEntityContext fetchStrictEntity(ActorUniqueID, bool) const = 0;
    [[nodiscard]] virtual Actor *fetchEntity(ActorUniqueID, bool) const = 0;
    [[nodiscard]] virtual Actor *getRuntimeEntity(ActorRuntimeID, bool) const = 0;
    [[nodiscard]] virtual Mob *getMob(ActorUniqueID) const = 0;
    [[nodiscard]] virtual Player *getPlayer(ActorUniqueID) const = 0;
    [[nodiscard]] virtual Player *getPlayer(mce::UUID const &) const = 0;
    [[nodiscard]] virtual Player *getPlayer(std::string const &) const = 0;
    [[nodiscard]] virtual Player *getPlayerByXuid(std::string const &) const = 0;
    [[nodiscard]] virtual Player *getPlatformPlayer(std::string const &) const = 0;
    [[nodiscard]] virtual Player *getPlayerFromServerId(std::string const &) const = 0;
    [[nodiscard]] virtual Player *getRuntimePlayer(ActorRuntimeID) const = 0;
    virtual int getNumRemotePlayers() = 0;
    [[nodiscard]] virtual Player *getPrimaryLocalPlayer() const = 0;
    virtual IMinecraftEventing &getEventing() = 0;
    [[nodiscard]] virtual mce::Color getPlayerColor(Player const &) const = 0;
    [[nodiscard]] virtual Tick const &getCurrentTick() const = 0;
    [[nodiscard]] virtual Tick getCurrentServerTick() const = 0;
    [[nodiscard]] virtual BiomeRegistry const &getBiomeRegistry() const = 0;
    virtual BiomeRegistry &getBiomeRegistry() = 0;
    [[nodiscard]] virtual BlockPalette const &getBlockPalette() const = 0;
    virtual BlockPalette &getBlockPalette() = 0;
    [[nodiscard]] virtual FeatureRegistry const &getFeatureRegistry() const = 0;
    virtual FeatureRegistry &getFeatureRegistry() = 0;
    [[nodiscard]] virtual FeatureTypeFactory const &getFeatureTypeFactory() const = 0;
    virtual FeatureTypeFactory &getFeatureTypeFactory() = 0;
    [[nodiscard]] virtual JigsawStructureRegistry const &getJigsawStructureRegistry() const = 0;
    virtual JigsawStructureRegistry &getJigsawStructureRegistry() = 0;
    [[nodiscard]] virtual Bedrock::NonOwnerPointer<StructureManager> getStructureManager() const = 0;
    virtual Bedrock::NonOwnerPointer<StructureManager> getStructureManager() = 0;
    [[nodiscard]] virtual BiomeComponentFactory const &getBiomeComponentFactory() const = 0;
    virtual BiomeComponentFactory &getBiomeComponentFactory() = 0;
    [[nodiscard]] virtual SurfaceBuilderRegistry const &getSurfaceBuilderRegistry() const = 0;
    virtual SurfaceBuilderRegistry &getSurfaceBuilderRegistry() = 0;
    [[nodiscard]] virtual BiomeManager const &getBiomeManager() const = 0;
    virtual BiomeManager &getBiomeManager() = 0;
    [[nodiscard]] virtual OwnerPtrFactory<Dimension, ILevel &, Scheduler &> const &getDimensionFactory() const = 0;
    virtual OwnerPtrFactory<Dimension, ILevel &, Scheduler &> &getDimensionFactory() = 0;
    [[nodiscard]] virtual Factory<BaseLightTextureImageBuilder, Level &, Scheduler &> const &
    getLightTextureImageBuilderFactory() const = 0;
    virtual Factory<BaseLightTextureImageBuilder, Level &, Scheduler &> &getLightTextureImageBuilderFactory() = 0;

    [[nodiscard]] virtual void *getWorldRegistriesProvider() const = 0;
    virtual void *getWorldRegistriesProvider() = 0;
    virtual void addListener(LevelListener &) = 0;
    virtual void removeListener(LevelListener &) = 0;
    virtual void tickEntities() = 0;
    virtual void tickEntitySystems() = 0;
    virtual StackRefResult<PauseManager> getPauseManager() = 0;
    [[nodiscard]] virtual StackRefResult<PauseManager const> getPauseManager() const = 0;
    virtual void onPlayerDeath(Player &, ActorDamageSource const &) = 0;
    virtual void tick() = 0;

private:
    virtual void directTickEntities(BlockSource &) = 0;

public:
    virtual bool explode(Explosion &) = 0;
    virtual bool explode(BlockSource &, Actor *, Vec3 const &, float, bool, bool, float, bool) = 0;
    virtual void spawnParticleEffect(std::string const &, Vec3 const &, Dimension *) = 0;
    virtual void denyEffect(BlockSource &, Vec3 const &) = 0;
    virtual void potionSplash(Vec3 const &, mce::Color const &, bool) = 0;
    virtual bool extinguishFire(BlockSource &, BlockPos const &, std::uint8_t, Actor *) = 0;
    virtual std::unique_ptr<Path> findPath(Actor &, Actor &, NavigationComponent &) = 0;
    virtual std::unique_ptr<Path> findPath(Actor &, int, int, int, NavigationComponent &) = 0;
    virtual void updateSleepingPlayerList() = 0;
    virtual void setSleepStatus(PlayerSleepStatus const &) = 0;
    [[nodiscard]] virtual PlayerSleepStatus getSleepStatus() const = 0;
    [[nodiscard]] virtual int getTime() const = 0;
    virtual void setTime(int) = 0;
    virtual std::uint32_t getSeed() = 0;
    [[nodiscard]] virtual BlockPos const &getSharedSpawnPos() const = 0;
    virtual void setDefaultSpawn(BlockPos const &) = 0;
    [[nodiscard]] virtual BlockPos const &getDefaultSpawn() const = 0;
    virtual void setDefaultGameType(GameType) = 0;
    [[nodiscard]] virtual GameType getDefaultGameType() const = 0;
    virtual void setDifficulty(Difficulty) = 0;
    virtual void setMultiplayerGameIntent(bool) = 0;
    [[nodiscard]] virtual bool getMultiplayerGameIntent() const = 0;
    virtual void setMultiplayerGame(bool) = 0;
    [[nodiscard]] virtual bool isMultiplayerGame() const = 0;
    virtual void setLANBroadcastIntent(bool) = 0;
    [[nodiscard]] virtual bool getLANBroadcastIntent() const = 0;
    virtual void setLANBroadcast(bool) = 0;
    [[nodiscard]] virtual bool getLANBroadcast() const = 0;
    virtual void setXBLBroadcastIntent(Social::GamePublishSetting) = 0;
    [[nodiscard]] virtual Social::GamePublishSetting getXBLBroadcastIntent() const = 0;
    [[nodiscard]] virtual bool hasXBLBroadcastIntent() const = 0;
    virtual void setXBLBroadcastMode(Social::GamePublishSetting) = 0;
    [[nodiscard]] virtual Social::GamePublishSetting getXBLBroadcastMode() const = 0;
    [[nodiscard]] virtual bool hasXBLBroadcast() const = 0;
    virtual void setPlatformBroadcastIntent(Social::GamePublishSetting) = 0;
    [[nodiscard]] virtual Social::GamePublishSetting getPlatformBroadcastIntent() const = 0;
    [[nodiscard]] virtual bool hasPlatformBroadcastIntent() const = 0;
    virtual void setPlatformBroadcastMode(Social::GamePublishSetting) = 0;
    [[nodiscard]] virtual Social::GamePublishSetting getPlatformBroadcastMode() const = 0;
    [[nodiscard]] virtual bool hasPlatformBroadcast() const = 0;
    virtual void setHasLockedBehaviorPack(bool) = 0;
    virtual void setHasLockedResourcePack(bool) = 0;
    virtual void setCommandsEnabled(bool) = 0;
    virtual void setWorldTemplateOptionsUnlocked() = 0;
    [[nodiscard]] virtual bool hasLevelStorage() const = 0;
    virtual LevelStorage &getLevelStorage() = 0;
    [[nodiscard]] virtual LevelStorage const &getLevelStorage() const = 0;
    virtual LevelData &getLevelData() = 0;
    [[nodiscard]] virtual LevelData const &getLevelData() const = 0;
    virtual PhotoStorage &getPhotoStorage() = 0;
    virtual void createPhotoStorage() = 0;
    virtual void setEducationLevelSettings(EducationLevelSettings) = 0;
    [[nodiscard]] virtual std::optional<EducationLevelSettings> const &getEducationLevelSettings() const = 0;
    virtual void save() = 0;
    virtual void saveLevelData() = 0;
    virtual void saveGameData() = 0;
    virtual std::shared_ptr<void *> requestTimedStorageDeferment() = 0;
    virtual TickingAreasManager &getTickingAreasMgr() = 0;
    virtual void addTickingAreaList(AutomaticID<Dimension, int>, std::shared_ptr<TickingAreaList> const &) = 0;
    virtual void sendServerLegacyParticle(ParticleType, Vec3 const &, Vec3 const &, int) = 0;
    virtual void playSound(AutomaticID<Dimension, int>, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                           ActorDefinitionIdentifier const &, bool, bool) = 0;
    virtual void playSound(IConstBlockSource const &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                           ActorDefinitionIdentifier const &, bool, bool) = 0;
    virtual void playSound(std::string const &, Vec3 const &, float, float) = 0;
    virtual void playSound(Puv::Legacy::LevelSoundEvent, Vec3 const &, float, float) = 0;
    virtual void playSound(Puv::Legacy::LevelSoundEvent, Vec3 const &, int, ActorDefinitionIdentifier const &, bool,
                           bool) = 0;
    virtual void registerEventCoordinators() = 0;
    virtual void setRemotePlayerEventCoordinator(std::unique_ptr<PlayerEventCoordinator> &&) = 0;
    virtual PlayerEventCoordinator &getRemotePlayerEventCoordinator() = 0;
    virtual void setServerPlayerEventCoordinator(std::unique_ptr<ServerPlayerEventCoordinator> &&) = 0;
    virtual ServerPlayerEventCoordinator &getServerPlayerEventCoordinator() = 0;
    virtual void setClientPlayerEventCoordinator(std::unique_ptr<ClientPlayerEventCoordinator> &&) = 0;
    virtual ClientPlayerEventCoordinator &getClientPlayerEventCoordinator() = 0;
    virtual void setActorEventCoordinator(std::unique_ptr<ActorEventCoordinator> &&) = 0;
    virtual ActorEventCoordinator &getActorEventCoordinator() = 0;
    virtual void setBlockEventCoordinator(std::unique_ptr<BlockEventCoordinator> &&) = 0;
    virtual BlockEventCoordinator &getBlockEventCoordinator() = 0;
    virtual void setItemEventCoordinator(std::unique_ptr<ItemEventCoordinator> &&) = 0;
    virtual ItemEventCoordinator &getItemEventCoordinator() = 0;
    virtual void setServerNetworkEventCoordinator(std::unique_ptr<ServerNetworkEventCoordinator> &&) = 0;
    virtual ServerNetworkEventCoordinator &getServerNetworkEventCoordinator() = 0;
    virtual void setScriptingEventCoordinator(std::unique_ptr<ScriptingEventCoordinator> &&) = 0;
    virtual ScriptingEventCoordinator &getScriptingEventCoordinator() = 0;
    virtual void setScriptDeferredEventCoordinator(std::unique_ptr<ScriptDeferredEventCoordinator> &&) = 0;
    virtual ScriptDeferredEventCoordinator &getScriptDeferredEventCoordinator() = 0;
    virtual LevelEventCoordinator &getLevelEventCoordinator() = 0;
    virtual void handleLevelEvent(LevelEvent, CompoundTag const &) = 0;
    virtual void handleLevelEvent(LevelEvent, Vec3 const &, int) = 0;
    virtual void handleStopSoundEvent(std::string const &) = 0;
    virtual void handleStopAllSounds() = 0;
    virtual void broadcastLevelEvent(LevelEvent, CompoundTag const &, UserEntityIdentifierComponent const *) = 0;
    virtual void broadcastLevelEvent(LevelEvent, Vec3 const &, int, UserEntityIdentifierComponent const *) = 0;
    virtual void broadcastLocalEvent(BlockSource &, LevelEvent, Vec3 const &, Block const &) = 0;
    virtual void broadcastLocalEvent(BlockSource &, LevelEvent, Vec3 const &, int) = 0;
    virtual void broadcastSoundEvent(Dimension &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                                     ActorDefinitionIdentifier const &, bool, bool) = 0;
    virtual void broadcastSoundEvent(BlockSource &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                                     ActorDefinitionIdentifier const &, bool, bool) = 0;
    virtual void broadcastSoundEvent(BlockSource &, Puv::Legacy::LevelSoundEvent, Vec3 const &, Block const &,
                                     ActorDefinitionIdentifier const &, bool, bool) = 0;
    virtual void broadcastActorEvent(Actor &, ActorEvent, int) const = 0;
    virtual void addChunkViewTracker(std::weak_ptr<ChunkViewSource>) = 0;
    virtual void onChunkReload(Bounds const &) = 0;
    virtual void onChunkReloaded(ChunkSource &, LevelChunk &) = 0;
    [[nodiscard]] virtual int getActivePlayerCount() const = 0;
    [[nodiscard]] virtual int getActiveUsersCount() const = 0;
    virtual void forEachPlayer(std::function<bool(Player const &)>) const = 0;
    virtual void forEachPlayer(std::function<bool(Player &)>) = 0;
    virtual void forEachUser(std::function<bool(EntityContext const &)>) const = 0;
    virtual void forEachUser(std::function<bool(EntityContext &)>) = 0;
    virtual Player *findPlayer(std::function<bool(WeakEntityRef const &)>) const = 0;
    virtual Player *findPlayer(std::function<bool(Player const &)>) const = 0;
    [[nodiscard]] virtual int getUserCount() const = 0;
    [[nodiscard]] virtual int countUsersWithMatchingNetworkId(NetworkIdentifier const &) const = 0;
    [[nodiscard]] virtual std::vector<OwnerPtr<EntityContext>> const &getUsers() const = 0;
    [[nodiscard]] virtual std::vector<OwnerPtr<EntityContext>> const &getEntities() const = 0;

private:
    virtual void onSubChunkLoaded(ChunkSource &, LevelChunk &, std::int16_t, bool) = 0;

public:
    virtual void onChunkLoaded(ChunkSource &, LevelChunk &) = 0;
    virtual void onChunkDiscarded(LevelChunk &) = 0;

private:
    virtual void *getLevelChunkMetaDataManager() = 0;

public:
    virtual void queueEntityDestruction(OwnerPtr<EntityContext>) = 0;
    virtual OwnerPtr<EntityContext> removeEntity(WeakEntityRef) = 0;
    virtual OwnerPtr<EntityContext> removeEntity(Actor &) = 0;
    virtual void forceRemoveEntity(Actor &) = 0;
    virtual void forceRemoveEntityfromWorld(Actor &) = 0;
    virtual void forceFlushRemovedPlayers() = 0;

private:
    virtual void loadFunctionManager() = 0;

public:
    virtual void levelCleanupQueueEntityRemoval(OwnerPtr<EntityContext>) = 0;
    virtual void registerTemporaryPointer(_TickPtr &) = 0;
    virtual void unregisterTemporaryPointer(_TickPtr &) = 0;
    virtual bool destroyBlock(BlockSource &, BlockPos const &, bool) = 0;
    virtual void upgradeStorageVersion(StorageVersion) = 0;
    virtual void suspendAndSave() = 0;
    virtual Particle *addParticle(ParticleType, Vec3 const &, Vec3 const &, int, CompoundTag const *, bool) = 0;
    virtual void destroyEffect(BlockPos const &, Block const &, int) = 0;
    virtual void addParticleEffect(HashedString const &, Vec3 const &, MolangVariableMap const &) = 0;
    virtual void addTerrainParticleEffect(BlockPos const &, Block const &, Vec3 const &, float, float, float) = 0;
    virtual void addTerrainSlideEffect(BlockPos const &, Block const &, Vec3 const &, float, float, float) = 0;
    virtual void addBreakingItemParticleEffect(Vec3 const &, ParticleType, ResolvedItemIconInfo const &) = 0;
    virtual ActorUniqueID getNewUniqueID() = 0;
    virtual ActorRuntimeID getNextRuntimeID() = 0;
    [[nodiscard]] virtual std::vector<ChunkPos> const &getTickingOffsets() const = 0;
    [[nodiscard]] virtual std::vector<ChunkPos> const &getClientTickingOffsets() const = 0;
    [[nodiscard]] virtual bool isExporting() const = 0;
    virtual void setIsExporting(bool) = 0;
    virtual SavedDataStorage &getSavedData() = 0;
    virtual MapItemSavedData *getMapSavedData(CompoundTag const &) = 0;
    virtual MapItemSavedData *getMapSavedData(CompoundTag const *) = 0;
    virtual MapItemSavedData *getMapSavedData(ActorUniqueID) = 0;
    virtual void requestMapInfo(ActorUniqueID, bool) = 0;
    virtual ActorUniqueID expandMapByID(ActorUniqueID, bool) = 0;
    virtual bool copyAndLockMap(ActorUniqueID, ActorUniqueID) = 0;
    virtual MapItemSavedData &createMapSavedData(std::vector<ActorUniqueID> const &, BlockPos const &,
                                                 AutomaticID<Dimension, int>, int) = 0;
    virtual MapItemSavedData &createMapSavedData(ActorUniqueID const &, BlockPos const &, AutomaticID<Dimension, int>,
                                                 int) = 0;
    [[nodiscard]] virtual Core::PathBuffer<std::string> getScreenshotsFolder() const = 0;
    [[nodiscard]] virtual std::string getLevelId() const = 0;
    virtual void setLevelId(std::string) = 0;
    virtual TaskGroup &getSyncTasksGroup() = 0;
    virtual TaskGroup &getIOTasksGroup() = 0;

private:
    [[nodiscard]] virtual void *getClientResourcePackManager() const = 0;
    [[nodiscard]] virtual void *getServerResourcePackManager() const = 0;
    virtual void *getTradeTables() = 0;
    virtual void decrementTagCache(std::string const &,
                                   TagRegistry<IDType<LevelTagIDType>, IDType<LevelTagSetIDType>> &) = 0;
    virtual void incrementTagCache(std::string const &,
                                   TagRegistry<IDType<LevelTagIDType>, IDType<LevelTagSetIDType>> &) = 0;

public:
    [[nodiscard]] virtual bool isEdu() const = 0;
    virtual ActorFactory &getActorFactory() = 0;
    [[nodiscard]] virtual ActorFactory const &getActorFactory() const = 0;
    virtual void *getActorInfoRegistry() = 0;
    [[nodiscard]] virtual StackRefResult<EntityRegistry const> getEntityRegistry() const = 0;
    virtual StackRefResult<EntityRegistry> getEntityRegistry() = 0;
    virtual EntitySystems &getEntitySystems() = 0;
    virtual WeakRef<EntityContext> getLevelEntity() = 0;

private:
    virtual void runCommand(HashedString const &, CommandOrigin &, CommandOriginSystem, CurrentCmdVersion) = 0;
    virtual void runCommand(Command &, CommandOrigin &, CommandOriginSystem) = 0;

public:
    [[nodiscard]] virtual PlayerCapabilities::ISharedController const &getCapabilities() const = 0;
    virtual std::string const &getTagRegistry() = 0;
    [[nodiscard]] virtual PlayerMovementSettings const &getPlayerMovementSettings() const = 0;
    virtual void setPlayerMovementSettings(PlayerMovementSettings const &) = 0;
    [[nodiscard]] virtual bool canUseSkin(SerializedSkin const &, NetworkIdentifier const &, mce::UUID const &,
                                          ActorUniqueID const &) const = 0;
    [[nodiscard]] virtual PositionTrackingDB::PositionTrackingDBClient *getPositionTrackerDBClient() const = 0;

private:
    [[nodiscard]] virtual PositionTrackingDB::PositionTrackingDBServer *getPositionTrackerDBServer() const = 0;

public:
    virtual void flushRunTimeLighting() = 0;
    virtual void loadBlockDefinitionGroup(Experiments const &) = 0;
    virtual void initializeBlockDefinitionGroup() = 0;
    virtual Bedrock::NonOwnerPointer<IUnknownBlockTypeRegistry> getUnknownBlockTypeRegistry() = 0;
    virtual void setServerTickOffset(std::int32_t) = 0;
    [[nodiscard]] virtual bool isClientSide() const = 0;
    virtual std::unordered_map<mce::UUID, PlayerListEntry> &getPlayerList() = 0;
    [[nodiscard]] virtual std::unordered_map<mce::UUID, PlayerListEntry> const &getPlayerList() const = 0;
    [[nodiscard]] virtual std::string const &getPlayerXUID(mce::UUID const &) const = 0;
    [[nodiscard]] virtual std::string const &getPlayerPlatformOnlineId(mce::UUID const &) const = 0;
    [[nodiscard]] virtual std::vector<WeakEntityRef> const &getActiveUsers() const = 0;
    [[nodiscard]] virtual std::vector<Actor *> getRuntimeActorList() const = 0;

private:
    virtual void notifySubChunkRequestManager(SubChunkPacket const &) = 0;
    virtual void *getSubChunkRequestManager() = 0;

public:
    [[nodiscard]] virtual PacketSender *getPacketSender() const = 0;
    virtual void setPacketSender(PacketSender *) = 0;
    [[nodiscard]] virtual Bedrock::NonOwnerPointer<NetEventCallback> getNetEventCallback() const = 0;
    virtual void setNetEventCallback(Bedrock::NonOwnerPointer<NetEventCallback>) = 0;
    [[nodiscard]] virtual Random &getIRandom() const = 0;
    [[nodiscard]] virtual Random &getRandom() const = 0;
    [[nodiscard]] virtual Random &getThreadRandom() const = 0;
    virtual HitResult &getHitResult() = 0;
    virtual HitResult &getLiquidHitResult() = 0;
    [[nodiscard]] virtual std::string const &getImmersiveReaderString() const = 0;
    virtual void setImmersiveReaderString(std::string) = 0;
    virtual AdventureSettings &getAdventureSettings() = 0;
    [[nodiscard]] virtual AdventureSettings const &getAdventureSettings() const = 0;
    [[nodiscard]] virtual GameRules const &getGameRules() const = 0;
    virtual GameRules &getGameRules() = 0;
    [[nodiscard]] virtual bool hasStartWithMapEnabled() const = 0;
    [[nodiscard]] virtual bool isEditorWorld() const = 0;
    virtual Abilities &getDefaultAbilities() = 0;
    virtual PermissionsHandler &getDefaultPermissions() = 0;
    [[nodiscard]] virtual PermissionsHandler const &getDefaultPermissions() const = 0;
    [[nodiscard]] virtual bool getTearingDown() const = 0;
    virtual void takePicture(cg::ImageBuffer &, Actor *, Actor *, ScreenshotOptions &) = 0;
    virtual Bedrock::NonOwnerPointer<LevelSoundManager> getLevelSoundManager() = 0;
    [[nodiscard]] virtual Bedrock::NonOwnerPointer<SoundPlayerInterface> getSoundPlayer() const = 0;
    virtual void setSimPaused(bool) = 0;
    virtual bool getSimPaused() = 0;
    virtual void setFinishedInitializing() = 0;
    virtual LootTables &getLootTables() = 0;
    virtual void updateWeather(float, int, float, int) = 0;
    [[nodiscard]] virtual int getNetherScale() const = 0;
    virtual Scoreboard &getScoreboard() = 0;
    [[nodiscard]] virtual Scoreboard const &getScoreboard() const = 0;
    virtual Scoreboard *tryGetScoreboard() = 0;
    virtual LayeredAbilities *getPlayerAbilities(ActorUniqueID const &) = 0;
    virtual void setPlayerAbilities(ActorUniqueID const &, LayeredAbilities const &) = 0;
    virtual void sendAllPlayerAbilities(Player const &) = 0;
    [[nodiscard]] virtual Recipes &getRecipes() const = 0;
    [[nodiscard]] virtual BlockReducer *getBlockReducer() const = 0;
    [[nodiscard]] virtual std::weak_ptr<TrimPatternRegistry const> getTrimPatternRegistry() const = 0;
    virtual std::weak_ptr<TrimPatternRegistry> getTrimPatternRegistry() = 0;
    [[nodiscard]] virtual std::weak_ptr<TrimMaterialRegistry const> getTrimMaterialRegistry() const = 0;
    virtual std::weak_ptr<TrimMaterialRegistry> getTrimMaterialRegistry() = 0;
    virtual void digestServerItemComponents(ItemComponentPacket const &) = 0;
    [[nodiscard]] virtual BlockLegacy const &getRegisteredBorderBlock() const = 0;
    [[nodiscard]] virtual bool use3DBiomeMaps() const = 0;
    virtual void addBlockSourceForValidityTracking(BlockSource *) = 0;
    virtual void removeBlockSourceFromValidityTracking(BlockSource *) = 0;

protected:
    virtual Level *asLevel() = 0;

private:
    virtual void *asMultiPlayerLevel() = 0;

public:
    virtual bool isClientSideGenerationEnabled() = 0;
    virtual bool blockNetworkIdsAreHashes() = 0;
    [[nodiscard]] virtual ItemRegistryRef getItemRegistry() const = 0;
    [[nodiscard]] virtual std::weak_ptr<BlockTypeRegistry> getBlockRegistry() const = 0;
    virtual void pauseAndFlushTaskGroups() = 0;

private:
    virtual void *getPlayerDeathManager() = 0;
    virtual void *getMapDataManager() = 0;
    virtual void *getArmorTrimUnloader() = 0;
    [[nodiscard]] virtual void *getPlayerSleepManager() const = 0;
    virtual void *getPlayerSleepManager() = 0;
#ifdef __linux__  // wtf mojang
    virtual void onSourceCreated(BlockSource &) = 0;
    virtual void onSourceDestroyed(BlockSource &) = 0;
#endif

protected:
    virtual void subTick() = 0;
    virtual void initializeMapDataManager() = 0;
};
