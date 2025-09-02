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

#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/core/sound/sound_system_types.h"
#include "bedrock/core/timing/time.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/server/commands/command_permission_level.h"
#include "bedrock/textobject/text_object.h"
#include "bedrock/world/actor/mob.h"
#include "bedrock/world/actor/player/layered_abilities.h"
#include "bedrock/world/actor/player/player_inventory.h"
#include "bedrock/world/actor/player/player_item_in_use.h"
#include "bedrock/world/actor/player/player_types.h"
#include "bedrock/world/actor/player/serialized_skin.h"
#include "bedrock/world/containers/managers/container_manager.h"
#include "bedrock/world/events/event_coordinator.h"
#include "bedrock/world/events/player_event_coordinator.h"
#include "bedrock/world/inventory/ender_chest_inventory.h"
#include "bedrock/world/inventory/inventory_options.h"
#include "bedrock/world/inventory/transaction/inventory_transaction_manager.h"
#include "bedrock/world/item/item_group.h"
#include "bedrock/world/level/game_type.h"
#include "bedrock/world/player_ui_container.h"

class PlayerRespawnRandomizer;

enum class BedSleepingResult : int {  // NOLINTBEGIN
    OK = 0,
    NOT_POSSIBLE_HERE = 1,
    NOT_POSSIBLE_NOW = 2,
    TOO_FAR_AWAY = 3,
    OTHER_PROBLEM = 4,
    NOT_SAFE = 5,
    BED_OBSTRUCTED = 6,  // NOLINTEND
};

class Player : public Mob {
public:
    Player(Level &, PacketSender &, GameType, bool, const NetworkIdentifier &, SubClientId, mce::UUID,
           const std::string &, const std::string &, PlayerAuthenticationType, const PlayerAuthenticationInfo &,
           EntityContext &, const std::string &, const std::string &);

    ~Player() override = 0;
    ENDSTONE_HOOK void teleportTo(const Vec3 &, bool, int, int, bool) override;
    ENDSTONE_HOOK void completeUsingItem();
    ENDSTONE_HOOK bool drop(const ItemStack &item, bool randomly) override;
    ENDSTONE_HOOK bool take(Actor &actor, int, int favored_slot);

    virtual void prepareRegion(ChunkSource &) = 0;
    virtual void destroyRegion() = 0;
    virtual void suspendRegion() = 0;

protected:
    virtual void _fireDimensionChanged() = 0;

public:
    virtual void changeDimensionWithCredits(DimensionType) = 0;
    virtual void tickWorld(Tick const &) = 0;
    virtual void frameUpdate(FrameUpdateContextBase &) = 0;
    [[nodiscard]] virtual std::vector<ChunkPos> const &getTickingOffsets() const = 0;
    virtual void moveView() = 0;
    virtual void moveSpawnView(Vec3 const &, DimensionType) = 0;
    virtual void checkMovementStats(Vec3 const &) = 0;
    [[nodiscard]] virtual StructureFeatureType getCurrentStructureFeature() const = 0;
    virtual bool isAutoJumpEnabled() = 0;
    virtual void respawn() = 0;
    virtual void resetRot() = 0;
    virtual bool isInTrialMode() = 0;
    virtual void openPortfolio() = 0;
    virtual void openBook(int, bool, int, BlockActor *) = 0;
    virtual void openTrading(ActorUniqueID const &, bool) = 0;
    virtual void openChalkboard(ChalkboardBlockActor &, bool) = 0;
    virtual void openNpcInteractScreen(std::shared_ptr<INpcDialogueData>) = 0;
    virtual void openInventory() = 0;
    virtual void displayChatMessage(std::string const &, std::string const &) = 0;
    virtual void displayClientMessage(std::string const &) = 0;
    virtual void displayTextObjectMessage(TextObjectRoot const &, std::string const &, std::string const &) = 0;
    virtual void displayTextObjectWhisperMessage(ResolvedTextObject const &, std::string const &,
                                                 std::string const &) = 0;
    virtual void displayTextObjectWhisperMessage(std::string const &, std::string const &, std::string const &) = 0;
    virtual void displayWhisperMessage(std::string const &, std::string const &, std::string const &,
                                       std::string const &) = 0;
    ENDSTONE_HOOK virtual BedSleepingResult startSleepInBed(BlockPos const &bed_block_pos);
    virtual void stopSleepInBed(bool, bool) = 0;
    virtual bool canStartSleepInBed() = 0;
    virtual void openSign(BlockPos const &, bool) = 0;
    virtual void playEmote(std::string const &, bool) = 0;
    [[nodiscard]] virtual bool isLoading() const = 0;
    [[nodiscard]] virtual bool isPlayerInitialized() const = 0;
    virtual void stopLoading() = 0;
    virtual void setPlayerGameType(GameType) = 0;
    virtual void initHUDContainerManager() = 0;

protected:
    virtual void _crit(Actor &) = 0;

public:
    [[nodiscard]] virtual IMinecraftEventing *getEventing() const = 0;
    virtual std::string getUserId() = 0;
    virtual void addExperience(int) = 0;
    virtual void addLevels(int) = 0;
    virtual void setContainerData(IContainerManager &, int, int) = 0;
    virtual void slotChanged(IContainerManager &, Container &, int, ItemStack const &, ItemStack const &, bool) = 0;
    virtual void refreshContainer(IContainerManager &) = 0;
    virtual void deleteContainerManager() = 0;
    virtual bool isActorRelevant(Actor const &) = 0;
    [[nodiscard]] virtual bool isTeacher() const = 0;
    virtual void onSuspension() = 0;
    virtual void onLinkedSlotsChanged() = 0;
    virtual void sendInventoryTransaction(InventoryTransaction const &) const = 0;
    virtual void sendComplexInventoryTransaction(std::unique_ptr<ComplexInventoryTransaction>) const = 0;
    virtual void sendNetworkPacket(Packet &) const = 0;
    virtual PlayerEventCoordinator &getPlayerEventCoordinator() = 0;
    virtual bool isSimulated() = 0;
    [[nodiscard]] virtual std::string getXuid() const = 0;
    [[nodiscard]] virtual PlayerMovementSettings getMovementSettings() const = 0;
    virtual void requestMissingSubChunk(SubChunkPos const &) = 0;
    [[nodiscard]] virtual std::uint8_t getMaxChunkBuildRadius() const = 0;
    virtual void onMovePlayerPacketNormal(Vec3 const &, Vec2 const &, float) = 0;

protected:
    virtual std::shared_ptr<ChunkViewSource> _createChunkSource(ChunkSource &) = 0;

public:
    virtual void setAbilities(LayeredAbilities const &) = 0;
    [[nodiscard]] virtual void *getEditorPlayer() const = 0;
    virtual void destroyEditorPlayer() = 0;

protected:
    [[nodiscard]] virtual int _getSpawnChunkLimit() const = 0;
    virtual void updateChunkPublisherView(Vec3 const &, float) = 0;

public:
    static Player *tryGetFromEntity(EntityContext &entity, bool include_removed = false);

    [[nodiscard]] bool hasBedPosition() const;
    [[nodiscard]] const BlockPos &getBedPosition() const;
    [[nodiscard]] const PlayerInventory &getSupplies() const;
    PlayerInventory &getSupplies();
    [[nodiscard]] const Container &getInventory() const;
    Container &getInventory();
    EnderChestContainer *getEnderChestContainer();
    [[nodiscard]] const EnderChestContainer *getEnderChestContainer() const;
    [[nodiscard]] int getSelectedItemSlot() const;
    const ItemStack &setSelectedSlot(int);
    [[nodiscard]] const std::string &getPlatformOnlineId() const;
    [[nodiscard]] const std::string &getName() const;
    void setPermissions(CommandPermissionLevel permission);
    void setBedRespawnPosition(const BlockPos &);
    bool setSpawnBlockRespawnPosition(const BlockPos &, DimensionType);
    bool canSleep() const;
    void stopGliding();
    [[nodiscard]] const SerializedSkin &getSkin() const;
    SerializedSkin &getSkin();

    [[nodiscard]] GameType getPlayerGameType() const;
    [[nodiscard]] PlayerPermissionLevel getPlayerPermissionLevel() const;

    // Abilities
    [[nodiscard]] LayeredAbilities &getAbilities();
    [[nodiscard]] LayeredAbilities const &getAbilities() const;
    [[nodiscard]] bool isEmoting() const;
    [[nodiscard]] bool isFlying() const;
    // Attributes
    [[nodiscard]] int getPlayerLevel() const;
    [[nodiscard]] float getLevelProgress() const;

    static int getXpNeededForLevelRange(int start, int end);

    // Endstone begins
    BedSleepingResult getBedResult(BlockPos const &bed_pos);  // moved from startSleepInBed into separate method
    // Endstone ends

    std::vector<std::uint16_t> ocean_biomes;  // +1120
    std::vector<std::uint16_t> froglights;
    const float sneak_height;
    const float sneak_offset;
    int score;
    BuildPlatform build_platform;  // +1180
    std::string unique_name;
    std::string server_id;
    std::string self_signed_id;
    std::string platform_offline_id;
    std::uint64_t client_random_id;
    std::string platform_id;
    ActorUniqueID pending_vehicle_id;
    ActorUniqueID pending_left_shoulder_passenger_id;
    ActorUniqueID pending_right_shoulder_passenger_id;
    ActorUniqueID interact_target;
    Vec3 interact_target_pos;
    bool has_fake_inventory;
    bool is_region_suspended;
    bool update_mobs;
    std::shared_ptr<ChunkViewSource> chunk_source;
    std::shared_ptr<ChunkViewSource> spawn_chunk_source;
    Vec3 cape_pos_o;
    Vec3 cape_pos;

protected:
    std::shared_ptr<ContainerManagerModel> container_manager_;
    Bedrock::PubSub::PublisherPtr<void(const ContainerManagerModel *), Bedrock::PubSub::ThreadModel::SingleThreaded>
        container_manager_subscribers_;
    std::unique_ptr<PlayerInventory> inventory_;  // +1480
    InventoryOptions inventory_options_;
    float distance_since_transform_event_;
    std::vector<ItemInstance> creative_item_list_;
    // std::array<std::vector<ItemGroup>, 4> filtered_creative_item_list_;
    std::string platform_online_id_;  // +1536

public:
    enum class SpawnPositionState : int {
        InitializeSpawnPositionRandomizer = 0,
        WaitForClientAck = 1,
        DetermineDimension = 2,
        ChangeDimension = 3,
        WaitForDimension = 4,
        ChooseSpawnArea = 5,
        CheckLoadedChunk = 6,
        ChooseSpawnPosition = 7,
        SpawnComplete = 8,
    };

    enum class SpawnPositionSource : int {
        Randomizer = 0,
        Respawn = 1,
        Teleport = 2,
        Static = 3,
    };

protected:
    SpawnPositionState spawn_position_state_;
    SpawnPositionSource spawn_position_source_;
    Vec3 spawn_positioning_test_position_;
    uint32_t respawn_chunk_builder_policy_handle_;
    struct CachedSpawnData {
        DimensionType respawn_dimension_id;
        Vec3 actual_respawn_position;
        BlockPos requested_respawn_position;
        bool has_respawn_position;
        BlockPos shared_spawn_position;
        bool respawn_ready;
        Vec3 player_pos;
        bool is_forced_respawn;
        bool is_adventure;
        bool is_flying_or_not_overworld;
        bool position_loaded_from_save;
        CachedSpawnData();
    };
    static_assert(sizeof(CachedSpawnData) == 64);

    CachedSpawnData cached_spawn_data_;
    OwnerPtr<BlockSource> spawn_block_source_;
    Stopwatch respawn_stopwatch_searching_;
    Vec3 respawn_original_position_;
    DimensionType respawn_original_dimension_;
    std::string respawn_message_;
    bool respawn_ready_;
    bool is_initial_spawn_done_;
    bool respawning_from_the_end_;
    bool position_loaded_from_save_;
    bool block_respawn_until_client_message_;
    bool has_seen_credits_;
    std::optional<PlayerSpawnFallbackType> spawn_fallback_type_;
    std::optional<Vec3> spawn_fallback_position_;
    PlayerItemInUse item_in_use_;
    ActorType last_hurt_by_;
    ActorUniqueID previous_interact_entity_;
    int previous_carried_item_;
    int emote_ticks_;
    std::shared_ptr<NetworkChunkPublisher> chunk_publisher_view_;
    PacketSender &packet_sender_;
    std::shared_ptr<HudContainerManagerModel> hud_container_manager_model_;
    std::unique_ptr<EnderChestContainer> ender_chest_inventory_;
    std::vector<ActorUniqueID> tracked_boss_ids_;
    using PlayerListenerList = std::vector<PlayerListener *>;
    ItemGroup cursor_selected_item_group_;
    PlayerUIContainer player_ui_container_;
    InventoryTransactionManager transaction_manager_;  // +2648
    std::unique_ptr<GameMode> game_mode_;
    std::unique_ptr<PlayerRespawnRandomizer> spawn_randomizer_;
    std::unique_ptr<SerializedSkin> skin_;
    std::unique_ptr<ItemStackNetManagerBase> item_stack_net_manager_;
    std::shared_ptr<AnimationComponent> ui_animation_component_;
    std::shared_ptr<AnimationComponent> map_animation_component_;
    struct PlayerSpawnPoint {
        BlockPos spawn_block_pos;
        BlockPos player_position;
        DimensionType dimension;
    } player_respawn_point_;
    float server_build_ratio_;
    SubClientId client_id_;
    bool interact_data_dirty_;
    bool should_client_generate_chunks_;

private:
    bool use_map_animation_component_;
    bool is_deferred_rendering_first_person_objects_;
    bool destroying_block_;
    bool player_level_changed_;
    int previous_level_requirement_;
    int last_level_up_time_;
    std::shared_ptr<AnimationComponent> first_person_animation_component_;
    PlayerListenerList listeners_;
    Vec3 respawn_position_candidate_;
    Vec3 pre_dimension_transfer_spawn_position_;
    int enchantment_seed_;
    std::vector<unsigned int> on_screen_animation_textures_;
    int on_screen_animation_ticks_;
    std::uint32_t chunk_radius_;
    int map_index_;
    float elytra_volume_;
    LoopingSoundHandle elytra_loop_;
    std::unordered_map<HashedString, int> cooldowns_;
    std::unordered_map<HashedString, HashedString> vanilla_cooldowns_;
    std::int64_t started_blocking_time_stamp_;
    std::int64_t blocked_using_shield_time_stamp_;
    std::int64_t blocked_using_damaged_shield_time_stamp_;
    std::string name_;  // +3080
    std::string last_emote_played_;
    time_t emote_easter_egg_end_time_;
    unsigned int emote_message_count_;
    std::string device_id_;
    bool flag_client_for_bai_reset_;
    bool send_inventory_options_to_client_;
    bool is_hosting_player_;
    bool prev_blocked_using_shield_;
    bool prev_blocked_using_damaged_shield_;
    bool used_potion_;
    PlayerDestroyProgressCacheComponent &destroy_progress_cache_;
};
