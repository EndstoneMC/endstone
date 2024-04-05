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

#include <functional>
#include <optional>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/core.h"
#include "bedrock/event/event_coordinator.h"
#include "bedrock/forward.h"
#include "bedrock/level/actor/mob.h"
#include "bedrock/mce.h"
#include "bedrock/network/packet_sender.h"

class ILevel {
public:
public:
    virtual ~ILevel(void);
    virtual bool initialize(std::string const &, LevelSettings const &, LevelData *, Experiments const &,
                            std::string const *);
    virtual void startLeaveGame(void);
    virtual bool isLeaveGameDone(void);
    virtual WeakRef<Dimension> getOrCreateDimension(AutomaticID<Dimension, int>);
    virtual WeakRef<Dimension> getDimension(AutomaticID<Dimension, int>) const;
    virtual AutomaticID<Dimension, int> getLastOrDefaultSpawnDimensionId(AutomaticID<Dimension, int>) const;
    virtual void forEachDimension(std::function<bool(Dimension &)>);
    virtual void forEachDimension(std::function<bool(Dimension const &)>) const;
    virtual DimensionManager const &getDimensionManager(void);
    virtual DimensionManager const &getDimensionManager(void) const;
    virtual uint32_t getChunkTickRange(void) const;
    virtual ChunkTickRangeManager const &getChunkTickRangeManager(void) const;
    virtual PortalForcer &getPortalForcer(void);
    virtual void requestPlayerChangeDimension(Player &, ChangeDimensionRequest &&);
    virtual void entityChangeDimension(Actor &, AutomaticID<Dimension, int>, std::optional<Vec3>);
    virtual Spawner &getSpawner(void) const;
    virtual Bedrock::NonOwnerPointer<BossEventSubscriptionManager> getBossEventSubscriptionManager(void);
    virtual ProjectileFactory &getProjectileFactory(void) const;
    virtual ActorDefinitionGroup *getEntityDefinitions(void) const;
    virtual Bedrock::NonOwnerPointer<ActorAnimationGroup> getActorAnimationGroup(void) const;
    virtual Bedrock::NonOwnerPointer<ActorAnimationControllerGroup> getActorAnimationControllerGroup(void) const;
    virtual BlockDefinitionGroup *getBlockDefinitions(void) const;
    virtual BlockComponentFactory &getBlockComponentFactory(void);
    virtual BlockComponentFactory const &getBlockComponentFactory(void) const;
    virtual PropertyGroupManager &getActorPropertyGroup(void) const;
    virtual CameraPresets &getCameraPresets(void);
    virtual CameraPresets const &getCameraPresets(void) const;
    virtual bool getDisablePlayerInteractions(void) const;
    virtual void setDisablePlayerInteractions(bool);
    virtual AutomationBehaviorTreeGroup &getAutomationBehaviorTreeGroup(void) const;
    virtual BehaviorFactory &getBehaviorFactory(void) const;
    virtual Difficulty getDifficulty(void) const;
    virtual InternalComponentRegistry &getInternalComponentRegistry(void) const;
    virtual DimensionConversionData getDimensionConversionData(void) const;
    virtual float getSpecialMultiplier(AutomaticID<Dimension, int>) const;
    virtual bool hasCommandsEnabled(void) const;
    virtual bool useMsaGamertagsOnly(void) const;
    virtual void setMsaGamertagsOnly(bool);
    virtual Actor *addEntity(BlockSource &, OwnerPtr<EntityContext>);
    virtual Actor *addGlobalEntity(BlockSource &, OwnerPtr<EntityContext>);
    virtual Actor *addAutonomousEntity(BlockSource &, OwnerPtr<EntityContext>);
    virtual void addUser(OwnerPtr<EntityContext>);
    virtual Actor *addDisplayEntity(BlockSource &, OwnerPtr<EntityContext>);
    virtual void removeDisplayEntity(WeakEntityRef);
    virtual void suspendPlayer(Player &);
    virtual void resumePlayer(Player &);
    virtual bool isPlayerSuspended(Player &) const;
    virtual OwnerPtr<EntityContext> removeActorAndTakeEntity(WeakEntityRef);
    virtual OwnerPtr<EntityContext> removeActorFromWorldAndTakeEntity(WeakEntityRef);
    virtual OwnerPtr<EntityContext> takeEntity(WeakEntityRef, LevelChunk &);
    virtual StrictEntityContext fetchStrictEntity(ActorUniqueID, bool) const;
    virtual Actor *fetchEntity(ActorUniqueID, bool) const;
    virtual Actor *getRuntimeEntity(ActorRuntimeID, bool) const;
    virtual Mob *getMob(ActorUniqueID) const;
    virtual Player *getPlayer(ActorUniqueID) const;
    virtual Player *getPlayer(mce::UUID const &) const;
    virtual Player *getPlayer(std::string const &) const;
    virtual Player *getPlayerByXuid(std::string const &) const;
    virtual Player *getPlatformPlayer(std::string const &) const;
    virtual Player *getPlayerFromServerId(std::string const &) const;
    virtual Player *getRuntimePlayer(ActorRuntimeID) const;
    virtual int getNumRemotePlayers(void);
    virtual Player *getPrimaryLocalPlayer(void) const;
    virtual IMinecraftEventing &getEventing(void);
    virtual mce::Color getPlayerColor(Player const &) const;
    virtual Tick const &getCurrentTick(void) const;
    virtual Tick const getCurrentServerTick(void) const;
    virtual BiomeRegistry const &getBiomeRegistry(void) const;
    virtual BiomeRegistry &getBiomeRegistry(void);
    virtual BlockPalette const &getBlockPalette(void) const;
    virtual BlockPalette &getBlockPalette(void);
    virtual FeatureRegistry const &getFeatureRegistry(void) const;
    virtual FeatureRegistry &getFeatureRegistry(void);
    virtual FeatureTypeFactory const &getFeatureTypeFactory(void) const;
    virtual FeatureTypeFactory &getFeatureTypeFactory(void);
    virtual JigsawStructureRegistry const &getJigsawStructureRegistry(void) const;
    virtual JigsawStructureRegistry &getJigsawStructureRegistry(void);
    virtual Bedrock::NonOwnerPointer<StructureManager> const getStructureManager(void) const;
    virtual Bedrock::NonOwnerPointer<StructureManager> getStructureManager(void);
    virtual BiomeComponentFactory const &getBiomeComponentFactory(void) const;
    virtual BiomeComponentFactory &getBiomeComponentFactory(void);
    virtual SurfaceBuilderRegistry const &getSurfaceBuilderRegistry(void) const;
    virtual SurfaceBuilderRegistry &getSurfaceBuilderRegistry(void);
    virtual BiomeManager const &getBiomeManager(void) const;
    virtual BiomeManager &getBiomeManager(void);
    virtual OwnerPtrFactory<Dimension, ILevel &, Scheduler &> const &getDimensionFactory(void) const;
    virtual OwnerPtrFactory<Dimension, ILevel &, Scheduler &> &getDimensionFactory(void);
    virtual Factory<BaseLightTextureImageBuilder, Level &, Scheduler &> const &getLightTextureImageBuilderFactory(
        void) const;
    virtual Factory<BaseLightTextureImageBuilder, Level &, Scheduler &> &getLightTextureImageBuilderFactory(void);

    virtual void *getWorldRegistriesProvider(void) const;
    virtual void *getWorldRegistriesProvider(void);
    virtual void addListener(LevelListener &);
    virtual void removeListener(LevelListener &);
    virtual void tickEntities(void);
    virtual void tickEntitySystems(void);
    virtual StackRefResult<PauseManager> getPauseManager(void);
    virtual StackRefResult<PauseManager const> getPauseManager(void) const;
    virtual void onPlayerDeath(Player &, ActorDamageSource const &);
    virtual void tick(void);

private:
    virtual void directTickEntities(BlockSource &);

public:
    virtual bool explode(Explosion &);
    virtual bool explode(BlockSource &, Actor *, Vec3 const &, float, bool, bool, float, bool);
    virtual void spawnParticleEffect(std::string const &, Vec3 const &, Dimension *);
    virtual void denyEffect(BlockSource &, Vec3 const &);
    virtual void potionSplash(Vec3 const &, mce::Color const &, bool);
    virtual bool extinguishFire(BlockSource &, BlockPos const &, uint8_t, Actor *);
    virtual std::unique_ptr<Path> findPath(Actor &, Actor &, NavigationComponent &);
    virtual std::unique_ptr<Path> findPath(Actor &, int, int, int, NavigationComponent &);
    virtual void updateSleepingPlayerList(void);
    virtual void setSleepStatus(PlayerSleepStatus const &);
    virtual PlayerSleepStatus getSleepStatus(void) const;
    virtual int getTime(void) const;
    virtual void setTime(int);
    virtual uint32_t getSeed(void);
    virtual BlockPos const &getSharedSpawnPos(void) const;
    virtual void setDefaultSpawn(BlockPos const &);
    virtual BlockPos const &getDefaultSpawn(void) const;
    virtual void setDefaultGameType(GameType);
    virtual GameType getDefaultGameType(void) const;
    virtual void setDifficulty(Difficulty);
    virtual void setMultiplayerGameIntent(bool);
    virtual bool getMultiplayerGameIntent(void) const;
    virtual void setMultiplayerGame(bool);
    virtual bool isMultiplayerGame(void) const;
    virtual void setLANBroadcastIntent(bool);
    virtual bool getLANBroadcastIntent(void) const;
    virtual void setLANBroadcast(bool);
    virtual bool getLANBroadcast(void) const;
    virtual void setXBLBroadcastIntent(Social::GamePublishSetting);
    virtual Social::GamePublishSetting getXBLBroadcastIntent(void) const;
    virtual bool hasXBLBroadcastIntent(void) const;
    virtual void setXBLBroadcastMode(Social::GamePublishSetting);
    virtual Social::GamePublishSetting getXBLBroadcastMode(void) const;
    virtual bool hasXBLBroadcast(void) const;
    virtual void setPlatformBroadcastIntent(Social::GamePublishSetting);
    virtual Social::GamePublishSetting getPlatformBroadcastIntent(void) const;
    virtual bool hasPlatformBroadcastIntent(void) const;
    virtual void setPlatformBroadcastMode(Social::GamePublishSetting);
    virtual Social::GamePublishSetting getPlatformBroadcastMode(void) const;
    virtual bool hasPlatformBroadcast(void) const;
    virtual void setHasLockedBehaviorPack(bool);
    virtual void setHasLockedResourcePack(bool);
    virtual void setCommandsEnabled(bool);
    virtual void setWorldTemplateOptionsUnlocked(void);
    virtual bool hasLevelStorage(void) const;
    virtual LevelStorage &getLevelStorage(void);
    virtual LevelStorage const &getLevelStorage(void) const;
    virtual LevelData &getLevelData(void);
    virtual LevelData const &getLevelData(void) const;
    virtual PhotoStorage &getPhotoStorage(void);
    virtual void createPhotoStorage(void);
    virtual void setEducationLevelSettings(EducationLevelSettings);
    virtual std::optional<EducationLevelSettings> const &getEducationLevelSettings(void) const;
    virtual void save(void);
    virtual void saveLevelData(void);
    virtual void saveGameData(void);
    virtual std::shared_ptr<void *> requestTimedStorageDeferment(void);
    virtual TickingAreasManager &getTickingAreasMgr(void);
    virtual void addTickingAreaList(AutomaticID<Dimension, int>, std::shared_ptr<TickingAreaList> const &);
    virtual void sendServerLegacyParticle(ParticleType, Vec3 const &, Vec3 const &, int);
    virtual void playSound(AutomaticID<Dimension, int>, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                           ActorDefinitionIdentifier const &, bool, bool);
    virtual void playSound(IConstBlockSource const &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                           ActorDefinitionIdentifier const &, bool, bool);
    virtual void playSound(std::string const &, Vec3 const &, float, float);
    virtual void playSound(Puv::Legacy::LevelSoundEvent, Vec3 const &, float, float);
    virtual void playSound(Puv::Legacy::LevelSoundEvent, Vec3 const &, int, ActorDefinitionIdentifier const &, bool,
                           bool);
    virtual void registerEventCoordinators(void);
    virtual void setRemotePlayerEventCoordinator(std::unique_ptr<PlayerEventCoordinator> &&);
    virtual PlayerEventCoordinator &getRemotePlayerEventCoordinator(void);
    virtual void setServerPlayerEventCoordinator(std::unique_ptr<ServerPlayerEventCoordinator> &&);
    virtual ServerPlayerEventCoordinator &getServerPlayerEventCoordinator(void);
    virtual void setClientPlayerEventCoordinator(std::unique_ptr<ClientPlayerEventCoordinator> &&);
    virtual ClientPlayerEventCoordinator &getClientPlayerEventCoordinator(void);
    virtual void setActorEventCoordinator(std::unique_ptr<ActorEventCoordinator> &&);
    virtual ActorEventCoordinator &getActorEventCoordinator(void);
    virtual void setBlockEventCoordinator(std::unique_ptr<BlockEventCoordinator> &&);
    virtual BlockEventCoordinator &getBlockEventCoordinator(void);
    virtual void setItemEventCoordinator(std::unique_ptr<ItemEventCoordinator> &&);
    virtual ItemEventCoordinator &getItemEventCoordinator(void);
    virtual void setServerNetworkEventCoordinator(std::unique_ptr<ServerNetworkEventCoordinator> &&);
    virtual ServerNetworkEventCoordinator &getServerNetworkEventCoordinator(void);
    virtual void setScriptingEventCoordinator(std::unique_ptr<ScriptingEventCoordinator> &&);
    virtual ScriptingEventCoordinator &getScriptingEventCoordinator(void);
    virtual void setScriptDeferredEventCoordinator(std::unique_ptr<ScriptDeferredEventCoordinator> &&);
    virtual ScriptDeferredEventCoordinator &getScriptDeferredEventCoordinator(void);
    virtual LevelEventCoordinator &getLevelEventCoordinator(void);
    virtual void handleLevelEvent(LevelEvent, CompoundTag const &);
    virtual void handleLevelEvent(LevelEvent, Vec3 const &, int);
    virtual void handleStopSoundEvent(std::string const &);
    virtual void handleStopAllSounds(void);
    virtual void broadcastLevelEvent(LevelEvent, CompoundTag const &, UserEntityIdentifierComponent const *);
    virtual void broadcastLevelEvent(LevelEvent, Vec3 const &, int, UserEntityIdentifierComponent const *);
    virtual void broadcastLocalEvent(BlockSource &, LevelEvent, Vec3 const &, Block const &);
    virtual void broadcastLocalEvent(BlockSource &, LevelEvent, Vec3 const &, int);
    virtual void broadcastSoundEvent(Dimension &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                                     ActorDefinitionIdentifier const &, bool, bool);
    virtual void broadcastSoundEvent(BlockSource &, Puv::Legacy::LevelSoundEvent, Vec3 const &, int,
                                     ActorDefinitionIdentifier const &, bool, bool);
    virtual void broadcastSoundEvent(BlockSource &, Puv::Legacy::LevelSoundEvent, Vec3 const &, Block const &,
                                     ActorDefinitionIdentifier const &, bool, bool);
    virtual void broadcastActorEvent(Actor &, ActorEvent, int) const;
    virtual void addChunkViewTracker(std::weak_ptr<ChunkViewSource>);
    virtual void onChunkReload(Bounds const &);
    virtual void onChunkReloaded(ChunkSource &, LevelChunk &);
    virtual int getActivePlayerCount(void) const;
    virtual int getActiveUsersCount(void) const;
    virtual void forEachPlayer(std::function<bool(Player const &)>) const;
    virtual void forEachPlayer(std::function<bool(Player &)>);
    virtual void forEachUser(std::function<bool(EntityContext const &)>) const;
    virtual void forEachUser(std::function<bool(EntityContext &)>);
    virtual Player *findPlayer(std::function<bool(WeakEntityRef const &)>) const;
    virtual Player *findPlayer(std::function<bool(Player const &)>) const;
    virtual int getUserCount(void) const;
    virtual int countUsersWithMatchingNetworkId(NetworkIdentifier const &) const;
    virtual std::vector<OwnerPtr<EntityContext>> const &getUsers(void) const;
    virtual std::vector<OwnerPtr<EntityContext>> const &getEntities(void) const;

private:
    virtual void onSubChunkLoaded(ChunkSource &, LevelChunk &, short, bool);

public:
    virtual void onChunkLoaded(ChunkSource &, LevelChunk &);
    virtual void onChunkDiscarded(LevelChunk &);

private:
    virtual void *getLevelChunkMetaDataManager(void);

public:
    virtual void queueEntityDestruction(OwnerPtr<EntityContext>);
    virtual OwnerPtr<EntityContext> removeEntity(WeakEntityRef);
    virtual OwnerPtr<EntityContext> removeEntity(Actor &);
    virtual void forceRemoveEntity(Actor &);
    virtual void forceRemoveEntityfromWorld(Actor &);
    virtual void forceFlushRemovedPlayers(void);

private:
    virtual void loadFunctionManager(void);

public:
    virtual void levelCleanupQueueEntityRemoval(OwnerPtr<EntityContext>);
    virtual void registerTemporaryPointer(_TickPtr &);
    virtual void unregisterTemporaryPointer(_TickPtr &);
    virtual bool destroyBlock(BlockSource &, BlockPos const &, bool);
    virtual void upgradeStorageVersion(StorageVersion);
    virtual void suspendAndSave(void);
    virtual Particle *addParticle(ParticleType, Vec3 const &, Vec3 const &, int, CompoundTag const *, bool);
    virtual void _destroyEffect(BlockPos const &, Block const &, int);
    virtual void addParticleEffect(HashedString const &, Vec3 const &, MolangVariableMap const &);
    virtual void addTerrainParticleEffect(BlockPos const &, Block const &, Vec3 const &, float, float, float);
    virtual void addTerrainSlideEffect(BlockPos const &, Block const &, Vec3 const &, float, float, float);
    virtual void addBreakingItemParticleEffect(Vec3 const &, ParticleType, ResolvedItemIconInfo const &);
    virtual ActorUniqueID getNewUniqueID(void);
    virtual ActorRuntimeID getNextRuntimeID(void);
    virtual std::vector<ChunkPos> const &getTickingOffsets(void) const;
    virtual std::vector<ChunkPos> const &getClientTickingOffsets(void) const;
    virtual bool isExporting(void) const;
    virtual void setIsExporting(bool);
    virtual SavedDataStorage &getSavedData(void);
    virtual MapItemSavedData *getMapSavedData(CompoundTag const &);
    virtual MapItemSavedData *getMapSavedData(CompoundTag const *);
    virtual MapItemSavedData *getMapSavedData(ActorUniqueID);
    virtual void requestMapInfo(ActorUniqueID, bool);
    virtual ActorUniqueID expandMapByID(ActorUniqueID, bool);
    virtual bool copyAndLockMap(ActorUniqueID, ActorUniqueID);
    virtual MapItemSavedData &createMapSavedData(std::vector<ActorUniqueID> const &, BlockPos const &,
                                                 AutomaticID<Dimension, int>, int);
    virtual MapItemSavedData &createMapSavedData(ActorUniqueID const &, BlockPos const &, AutomaticID<Dimension, int>,
                                                 int);
    virtual Core::PathBuffer<std::string> getScreenshotsFolder(void) const;
    virtual std::string getLevelId(void) const;
    virtual void setLevelId(std::string);
    virtual TaskGroup &getSyncTasksGroup(void);
    virtual TaskGroup &getIOTasksGroup(void);

private:
    virtual void *getClientResourcePackManager(void) const;
    virtual void *getServerResourcePackManager(void) const;
    virtual void *getTradeTables(void);
    virtual void decrementTagCache(std::string const &,
                                   TagRegistry<IDType<LevelTagIDType>, IDType<LevelTagSetIDType>> &);
    virtual void incrementTagCache(std::string const &,
                                   TagRegistry<IDType<LevelTagIDType>, IDType<LevelTagSetIDType>> &);

public:
    virtual bool isEdu(void) const;
    virtual ActorFactory &getActorFactory(void);
    virtual ActorFactory const &getActorFactory(void) const;
    virtual void *getActorInfoRegistry(void);
    virtual StackRefResult<EntityRegistry const> getEntityRegistry(void) const;
    virtual StackRefResult<EntityRegistry> getEntityRegistry(void);
    virtual EntitySystems &getEntitySystems(void);
    virtual WeakRef<EntityContext> getLevelEntity(void);

private:
    virtual void runCommand(HashedString const &, CommandOrigin &, CommandOriginSystem, CurrentCmdVersion);
    virtual void runCommand(Command &, CommandOrigin &, CommandOriginSystem);

public:
    virtual PlayerCapabilities::ISharedController const &getCapabilities(void) const;
    virtual std::string const &getTagRegistry(void);
    virtual PlayerMovementSettings const &getPlayerMovementSettings(void) const;
    virtual void setPlayerMovementSettings(PlayerMovementSettings const &);
    virtual bool canUseSkin(SerializedSkin const &, NetworkIdentifier const &, mce::UUID const &,
                            ActorUniqueID const &) const;
    virtual PositionTrackingDB::PositionTrackingDBClient *getPositionTrackerDBClient(void) const;

private:
    virtual PositionTrackingDB::PositionTrackingDBServer *getPositionTrackerDBServer(void) const;

public:
    virtual void flushRunTimeLighting(void);
    virtual void loadBlockDefinitionGroup(Experiments const &);
    virtual void initializeBlockDefinitionGroup(void);
    virtual Bedrock::NonOwnerPointer<IUnknownBlockTypeRegistry> getUnknownBlockTypeRegistry(void);
    virtual void setServerTickOffset(long);
    virtual bool isClientSide(void) const;
    virtual std::unordered_map<mce::UUID, PlayerListEntry> &getPlayerList(void);
    virtual std::unordered_map<mce::UUID, PlayerListEntry> const &getPlayerList(void) const;
    virtual std::string const &getPlayerXUID(mce::UUID const &) const;
    virtual std::string const &getPlayerPlatformOnlineId(mce::UUID const &) const;
    virtual std::vector<WeakEntityRef> const &getActiveUsers(void) const;
    virtual std::vector<Actor *> getRuntimeActorList(void) const;

private:
    virtual void notifySubChunkRequestManager(SubChunkPacket const &);
    virtual void *getSubChunkRequestManager(void);

public:
    virtual PacketSender *getPacketSender(void) const;
    virtual void setPacketSender(PacketSender *);
    virtual Bedrock::NonOwnerPointer<NetEventCallback> getNetEventCallback(void) const;
    virtual void setNetEventCallback(Bedrock::NonOwnerPointer<NetEventCallback>);
    virtual Random &getIRandom(void) const;
    virtual Random &getRandom(void) const;
    virtual Random &getThreadRandom(void) const;
    virtual HitResult &getHitResult(void);
    virtual HitResult &getLiquidHitResult(void);
    virtual std::string const &getImmersiveReaderString(void) const;
    virtual void setImmersiveReaderString(std::string);
    virtual AdventureSettings &getAdventureSettings(void);
    virtual AdventureSettings const &getAdventureSettings(void) const;
    virtual GameRules const &getGameRules(void) const;
    virtual GameRules &getGameRules(void);
    virtual bool hasStartWithMapEnabled(void) const;
    virtual bool isEditorWorld(void) const;
    virtual Abilities &getDefaultAbilities(void);
    virtual PermissionsHandler &getDefaultPermissions(void);
    virtual PermissionsHandler const &getDefaultPermissions(void) const;
    virtual bool getTearingDown(void) const;
    virtual void takePicture(cg::ImageBuffer &, Actor *, Actor *, ScreenshotOptions &);
    virtual Bedrock::NonOwnerPointer<LevelSoundManager> getLevelSoundManager(void);
    virtual Bedrock::NonOwnerPointer<SoundPlayerInterface> getSoundPlayer(void) const;
    virtual void setSimPaused(bool);
    virtual bool getSimPaused(void);
    virtual void setFinishedInitializing(void);
    virtual LootTables &getLootTables(void);
    virtual void updateWeather(float, int, float, int);
    virtual int getNetherScale(void) const;
    virtual Scoreboard &getScoreboard(void);
    virtual Scoreboard const &getScoreboard(void) const;
    virtual Scoreboard *tryGetScoreboard(void);
    virtual LayeredAbilities *getPlayerAbilities(ActorUniqueID const &);
    virtual void setPlayerAbilities(ActorUniqueID const &, LayeredAbilities const &);
    virtual void sendAllPlayerAbilities(Player const &);
    virtual Recipes &getRecipes(void) const;
    virtual BlockReducer *getBlockReducer(void) const;
    virtual std::weak_ptr<TrimPatternRegistry const> getTrimPatternRegistry(void) const;
    virtual std::weak_ptr<TrimPatternRegistry> getTrimPatternRegistry(void);
    virtual std::weak_ptr<TrimMaterialRegistry const> getTrimMaterialRegistry(void) const;
    virtual std::weak_ptr<TrimMaterialRegistry> getTrimMaterialRegistry(void);
    virtual void digestServerItemComponents(ItemComponentPacket const &);
    virtual BlockLegacy const &getRegisteredBorderBlock(void) const;
    virtual bool use3DBiomeMaps(void) const;
    virtual void addBlockSourceForValidityTracking(BlockSource *);
    virtual void removeBlockSourceFromValidityTracking(BlockSource *);

protected:
    virtual Level *asLevel();

private:
    virtual void *asMultiPlayerLevel(void);

public:
    virtual bool isClientSideGenerationEnabled(void);
    virtual bool blockNetworkIdsAreHashes(void);
    virtual ItemRegistryRef getItemRegistry(void) const;
    virtual std::weak_ptr<BlockTypeRegistry> getBlockRegistry(void) const;
    virtual void pauseAndFlushTaskGroups(void);

private:
    virtual void *_getPlayerDeathManager(void);
    virtual void *_getMapDataManager(void);
    virtual void *getArmorTrimUnloader(void);
    virtual void *getPlayerSleepManager(void) const;
    virtual void *getPlayerSleepManager(void);
#ifdef __linux__
    virtual void onSourceCreated(BlockSource &);
    virtual void onSourceDestroyed(BlockSource &);
#endif

protected:
    virtual void _subTick(void);
    virtual void _initializeMapDataManager(void);
};
