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

#include "bedrock/common_types.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/deps/raknet/packet_priority.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/platform/result.h"

enum class MinecraftPacketIds : int {
    KeepAlive = 0,
    Login = 1,
    PlayStatus = 2,
    ServerToClientHandshake = 3,
    ClientToServerHandshake = 4,
    Disconnect = 5,
    ResourcePacksInfo = 6,
    ResourcePackStack = 7,
    ResourcePackClientResponse = 8,
    Text = 9,
    SetTime = 10,
    StartGame = 11,
    AddPlayer = 12,
    AddActor = 13,
    RemoveActor = 14,
    AddItemActor = 15,
    ServerPlayerPostMovePosition = 16,
    TakeItemActor = 17,
    MoveAbsoluteActor = 18,
    MovePlayer = 19,
    PassengerJump_Deprecated = 20,
    UpdateBlock = 21,
    AddPainting = 22,
    TickSync_deprecated = 23,
    LevelSoundEventV1_DEPRECATED = 24,
    LevelEvent = 25,
    TileEvent = 26,
    ActorEvent = 27,
    MobEffect = 28,
    UpdateAttributes = 29,
    InventoryTransaction = 30,
    PlayerEquipment = 31,
    MobArmorEquipment = 32,
    Interact = 33,
    BlockPickRequest = 34,
    ActorPickRequest = 35,
    PlayerAction = 36,
    ActorFall_deprecated = 37,
    HurtArmor = 38,
    SetActorData = 39,
    SetActorMotion = 40,
    SetActorLink = 41,
    SetHealth = 42,
    SetSpawnPosition = 43,
    Animate = 44,
    Respawn = 45,
    ContainerOpen = 46,
    ContainerClose = 47,
    PlayerHotbar = 48,
    InventoryContent = 49,
    InventorySlot = 50,
    ContainerSetData = 51,
    CraftingData = 52,
    CraftingEvent_Deprecated = 53,
    GuiDataPickItem = 54,
    AdventureSettings_Deprecated = 55,
    BlockActorData = 56,
    PlayerInput_Deprecated = 57,
    FullChunkData = 58,
    SetCommandsEnabled = 59,
    SetDifficulty = 60,
    ChangeDimension = 61,
    SetPlayerGameType = 62,
    PlayerList = 63,
    SimpleEvent = 64,
    LegacyTelemetryEvent = 65,
    SpawnExperienceOrb = 66,
    MapData = 67,
    MapInfoRequest = 68,
    RequestChunkRadius = 69,
    ChunkRadiusUpdated = 70,
    ItemFrameDropItem_Deprecated = 71,
    GameRulesChanged = 72,
    Camera = 73,
    BossEvent = 74,
    ShowCredits = 75,
    AvailableCommands = 76,
    CommandRequest = 77,
    CommandBlockUpdate = 78,
    CommandOutput = 79,
    UpdateTrade = 80,
    UpdateEquip = 81,
    ResourcePackDataInfo = 82,
    ResourcePackChunkData = 83,
    ResourcePackChunkRequest = 84,
    Transfer = 85,
    PlaySound = 86,
    StopSound = 87,
    SetTitle = 88,
    AddBehaviorTree = 89,
    StructureBlockUpdate = 90,
    ShowStoreOffer = 91,
    PurchaseReceipt = 92,
    PlayerSkin = 93,
    SubclientLogin = 94,
    AutomationClientConnect = 95,
    SetLastHurtBy = 96,
    BookEdit = 97,
    NPCRequest = 98,
    PhotoTransfer = 99,
    ShowModalForm = 100,
    ModalFormResponse = 101,
    ServerSettingsRequest = 102,
    ServerSettingsResponse = 103,
    ShowProfile = 104,
    SetDefaultGameType = 105,
    RemoveObjective = 106,
    SetDisplayObjective = 107,
    SetScore = 108,
    LabTable = 109,
    UpdateBlockSynced = 110,
    MoveDeltaActor = 111,
    SetScoreboardIdentity = 112,
    SetLocalPlayerAsInit = 113,
    UpdateSoftEnum = 114,
    Ping = 115,
    BlockPalette_deprecated = 116,
    ScriptCustomEvent = 117,
    SpawnParticleEffect = 118,
    AvailableActorIDList = 119,
    LevelSoundEventV2_DEPRECATED = 120,
    NetworkChunkPublisherUpdate = 121,
    BiomeDefinitionList = 122,
    LevelSoundEvent = 123,
    LevelEventGeneric = 124,
    LecternUpdate = 125,
    VideoStreamConnect_DEPRECATED = 126,
    AddEntity_DEPRECATED = 127,
    RemoveEntity_DEPRECATED = 128,
    ClientCacheStatus = 129,
    OnScreenTextureAnimation = 130,
    MapCreateLockedCopy = 131,
    StructureTemplateDataExportRequest = 132,
    StructureTemplateDataExportResponse = 133,
    UNUSED_PLS_USE_ME = 134,
    ClientCacheBlobStatusPacket = 135,
    ClientCacheMissResponsePacket = 136,
    EducationSettingsPacket = 137,
    Emote = 138,
    MultiplayerSettingsPacket = 139,
    SettingsCommandPacket = 140,
    AnvilDamage = 141,
    CompletedUsingItem = 142,
    NetworkSettings = 143,
    PlayerAuthInputPacket = 144,
    CreativeContent = 145,
    PlayerEnchantOptions = 146,
    ItemStackRequest = 147,
    ItemStackResponse = 148,
    PlayerArmorDamage = 149,
    CodeBuilderPacket = 150,
    UpdatePlayerGameType = 151,
    EmoteList = 152,
    PositionTrackingDBServerBroadcast = 153,
    PositionTrackingDBClientRequest = 154,
    DebugInfoPacket = 155,
    PacketViolationWarning = 156,
    MotionPredictionHints = 157,
    TriggerAnimation = 158,
    CameraShake = 159,
    PlayerFogSetting = 160,
    CorrectPlayerMovePredictionPacket = 161,
    ItemRegistryPacket = 162,
    FilterTextPacket_DEPRECATED = 163,
    ClientBoundDebugRendererPacket = 164,
    SyncActorProperty = 165,
    AddVolumeEntityPacket = 166,
    RemoveVolumeEntityPacket = 167,
    SimulationTypePacket = 168,
    NpcDialoguePacket = 169,
    EduUriResourcePacket = 170,
    CreatePhotoPacket = 171,
    UpdateSubChunkBlocks = 172,
    PhotoInfoRequest_DEPRECATED = 173,
    SubChunkPacket = 174,
    SubChunkRequestPacket = 175,
    PlayerStartItemCooldown = 176,
    ScriptMessagePacket = 177,
    CodeBuilderSourcePacket = 178,
    TickingAreasLoadStatus = 179,
    DimensionDataPacket = 180,
    AgentAction = 181,
    ChangeMobProperty = 182,
    LessonProgressPacket = 183,
    RequestAbilityPacket = 184,
    RequestPermissionsPacket = 185,
    ToastRequest = 186,
    UpdateAbilitiesPacket = 187,
    UpdateAdventureSettingsPacket = 188,
    DeathInfo = 189,
    EditorNetworkPacket = 190,
    FeatureRegistryPacket = 191,
    ServerStats = 192,
    RequestNetworkSettings = 193,
    GameTestRequestPacket = 194,
    GameTestResultsPacket = 195,
    PlayerClientInputPermissions = 196,
    ClientCheatAbilityPacket_Deprecated = 197,
    CameraPresets = 198,
    UnlockedRecipes = 199,
    TitleSpecificPacketsStart = 200,
    TitleSpecificPacketsEnd = 299,
    CameraInstruction = 300,
    CompressedBiomeDefinitionList_DEPRECATED = 301,
    TrimData = 302,
    OpenSign = 303,
    AgentAnimation = 304,
    RefreshEntitlementsPacket = 305,
    PlayerToggleCrafterSlotRequestPacket = 306,
    SetPlayerInventoryOptions = 307,
    SetHudPacket = 308,
    AwardAchievementPacket = 309,
    ClientboundCloseScreen = 310,
    ClientboundLoadingScreenPacket_Deprecated = 311,
    ServerboundLoadingScreenPacket = 312,
    JigsawStructureDataPacket = 313,
    CurrentStructureFeaturePacket = 314,
    ServerboundDiagnosticsPacket = 315,
    CameraAimAssist = 316,
    ContainerRegistryCleanup = 317,
    MovementEffect = 318,
    SetMovementAuthorityMode_DEPRECATED = 319,
    CameraAimAssistPresets = 320,
    ClientCameraAimAssist = 321,
    ClientMovementPredictionSyncPacket = 322,
    UpdateClientOptions = 323,
    PlayerVideoCapturePacket = 324,
    PlayerUpdateEntityOverridesPacket = 325,
    PlayerLocation = 326,
    ClientboundControlSchemeSetPacket = 327,
    ServerScriptDebugDrawerPacket = 328,
    ServerboundPackSettingChangePacket = 329,
    ClientboundDataStore = 330,
    GraphicsParameterOverridePacket = 331,
    ServerboundDataStore = 332,
    ClientboundDataDrivenUIShowScreen = 333,
    ClientboundDataDrivenUICloseAllScreens = 334,
    ClientboundDataDrivenUIReload = 335,
    ClientboundTextureShiftPacket = 336,
    VoxelShapesPacket = 337,
    CameraSpline = 338,
    CameraAimAssistActorPriority = 339,
    EndId = 340,
};

class NetEventCallback;
class Packet;

class IPacketHandlerDispatcher {
public:
    virtual ~IPacketHandlerDispatcher() = default;
    virtual void handle(const NetworkIdentifier &, NetEventCallback &, std::shared_ptr<Packet> &) const = 0;
};

class PacketHeader {
public:
    static constexpr std::uint32_t NUM_CHANNEL_BITS = 1;
    static constexpr std::uint32_t NUM_CHANNELS = 2;
    PacketHeader(SubClientId, MinecraftPacketIds, SubClientId);
    static PacketHeader fromRaw(std::uint32_t);
    [[nodiscard]] MinecraftPacketIds getPacketId() const;
    [[nodiscard]] SubClientId getRecipientSubId() const;
    [[nodiscard]] SubClientId getSenderSubId() const;
    [[nodiscard]] std::uint32_t getChannel() const;
    void write(BinaryStream &);

private:
    static constexpr unsigned int NUM_BITS_FOR_SUBID = 2;
    static constexpr unsigned int NUM_BITS_FOR_PACKETID = 10;
    static constexpr uint32_t PACKET_ID_MASK = (1 << NUM_BITS_FOR_PACKETID) - 1;
    static constexpr uint32_t SUBCLIENT_ID_MASK = (1 << NUM_BITS_FOR_SUBID) - 1;
    std::uint32_t header_data_;
    PacketHeader();
};
static_assert(sizeof(PacketHeader) == 4);

class Packet {
public:
    virtual ~Packet() = default;
    [[nodiscard]] virtual MinecraftPacketIds getId() const = 0;
    [[nodiscard]] virtual std::string_view getName() const = 0;
    [[nodiscard]] virtual size_t getMaxSize() const;
    [[nodiscard]] virtual Bedrock::Result<void> checkSize(std::uint64_t packet_size, bool is_receiver_server) const;
    virtual void writeWithSerializationMode(BinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx,
                                            std::optional<SerializationMode> mode) const;
    virtual void write(BinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx) const;
    virtual void write(BinaryStream &) const = 0;
    virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &);
    [[nodiscard]] virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &stream);
    [[nodiscard]] virtual bool disallowBatching() const;
    [[nodiscard]] virtual bool isValid() const;
    [[nodiscard]] virtual SerializationMode getSerializationMode() const;
    virtual void setSerializationMode(SerializationMode);
    [[nodiscard]] virtual std::string toString() const;

    Bedrock::Result<void> readNoHeader(ReadOnlyBinaryStream &stream, const cereal::ReflectionCtx &reflection_ctx,
                                       const SubClientId &sub_id);
    [[nodiscard]] SubClientId getSenderSubId() const;
    void setSenderSubId(SubClientId sender_sub_id);
    [[nodiscard]] Compressibility getCompressible() const;
    [[nodiscard]] NetworkPeer::Reliability getReliability() const;
    void setReceiveTimestamp(const NetworkPeer::PacketRecvTimepoint &recv_timepoint);
    void handle(const NetworkIdentifier &id, NetEventCallback &callback, std::shared_ptr<Packet> &packet);

private:
    friend class MinecraftPackets;
    friend class NetworkSystem;
    virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx);
    [[nodiscard]] virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &) = 0;

protected:
    PacketPriority priority_{PacketPriority::MEDIUM_PRIORITY};                         // + 8
    NetworkPeer::Reliability reliability_{NetworkPeer::Reliability::ReliableOrdered};  // + 12
    SubClientId sender_sub_id_{SubClientId::PrimaryClient};                            // + 16
    bool is_handled_{false};                                                           // + 17
    NetworkPeer::PacketRecvTimepoint recv_timepoint_;                                  // + 24

private:
    const IPacketHandlerDispatcher *handler_{nullptr};             // + 32
    Compressibility compressible_{Compressibility::Compressible};  // + 40
};
BEDROCK_STATIC_ASSERT_SIZE(Packet, 48, 48);

class MinecraftPackets {
public:
    ENDSTONE_HOOK static std::shared_ptr<Packet> createPacket(MinecraftPacketIds id);
};
