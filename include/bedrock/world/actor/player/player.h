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

#include <cstddef>
#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/command/command_permission_level.h"
#include "bedrock/network/protocol/packet_sender.h"
#include "bedrock/world/actor/mob/mob.h"
#include "bedrock/world/level/event/event_coordinator.h"
#include "endstone/detail/player.h"
#include "endstone/player.h"

class Player : public Mob {
public:
    ~Player() override = 0;

    virtual void prepareRegion(ChunkSource &) = 0;
    virtual void destroyRegion() = 0;
    virtual void suspendRegion() = 0;

protected:
    virtual void _fireDimensionChanged() = 0;

public:
    virtual void changeDimensionWithCredits(AutomaticID<Dimension, int>) = 0;
    virtual void tickWorld(Tick const &) = 0;
    virtual void frameUpdate(FrameUpdateContextBase &) = 0;
    [[nodiscard]] virtual std::vector<ChunkPos> const &getTickingOffsets() const = 0;
    virtual void moveView() = 0;
    virtual void moveSpawnView(Vec3 const &, AutomaticID<Dimension, int>) = 0;
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
    virtual BedSleepingResult startSleepInBed(BlockPos const &) = 0;
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
    BEDROCK_API void setPermissions(CommandPermissionLevel level);
    [[nodiscard]] BEDROCK_API const std::string &getName() const;

private:
public:
    [[nodiscard]] endstone::detail::EndstonePlayer &getEndstonePlayer() const;  // Endstone

protected:
    void sendCommands() const;  // Endstone
};
