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

#include <string>
#include <unordered_map>

#include "bedrock/core/hash/hash_types.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/server/commands/command_soft_enum_registry.h"
#include "bedrock/world/events/player_scoreboard_event_listener.h"
#include "bedrock/world/events/scoreboard_event_coordinator.h"
#include "bedrock/world/scores/identity_dictionary.h"
#include "bedrock/world/scores/objective.h"
#include "bedrock/world/scores/player_scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_identity_ref.h"

class Actor;
class Player;

class Scoreboard {
public:
    static constexpr std::string DISPLAY_SLOT_LIST = "list";
    static constexpr std::string DISPLAY_SLOT_SIDEBAR = "sidebar";
    static constexpr std::string DISPLAY_SLOT_BELOWNAME = "belowname";

    virtual ~Scoreboard();
    virtual DisplayObjective const *setDisplayObjective(std::string const &, Objective const &, ObjectiveSortOrder) = 0;
    virtual Objective *clearDisplayObjective(std::string const &) = 0;
    virtual ScoreboardId const &createScoreboardId(Player const &) = 0;
    virtual ScoreboardId const &createScoreboardId(Actor const &) = 0;
    virtual ScoreboardId const &createScoreboardId(std::string const &) = 0;
    virtual void onObjectiveAdded(Objective const &) = 0;
    virtual void onObjectiveRemoved(Objective &) = 0;
    virtual void onScoreChanged(ScoreboardId const &, Objective const &) = 0;
    virtual void onPlayerScoreRemoved(ScoreboardId const &, Objective const &) = 0;
    virtual void onPlayerJoined(Player const &) = 0;
    virtual void onPlayerIdentityUpdated(PlayerScoreboardId const &) = 0;
    virtual void tick() = 0;
    virtual void setPacketSender(PacketSender *sender) = 0;
    virtual void writeToLevelStorage() = 0;
    [[nodiscard]] virtual bool isClientSide() const = 0;

    Objective *getObjective(const std::string &) const;
    Objective *addObjective(const std::string &, const std::string &, const ObjectiveCriteria &);
    bool removeObjective(Objective *);
    std::vector<const Objective *> getObjectives() const;
    std::vector<Objective *> getObjectives();
    std::vector<std::string> getObjectiveNames() const;
    void forEachObjective(const std::function<void(Objective &)>);
    const ObjectiveCriteria &createObjectiveCriteria(const std::string &, bool, ObjectiveRenderType);
    ObjectiveCriteria *getCriteria(const std::string &) const;
    const ObjectiveCriteria &getDefaultCriteria() const;
    std::vector<std::string> getCriteriaNames() const;
    const DisplayObjective *getDisplayObjective(const std::string &) const;
    std::vector<std::string> getDisplayObjectiveSlotNames() const;
    std::vector<ScoreboardId> getTrackedIds() const;
    bool isObjectiveDisplayed(const Objective &) const;
    std::vector<const DisplayObjective *> getAssociatedDisplaySlots(const Objective &) const;
    int modifyPlayerScore(ScoreboardOperationResult &, ScoreboardIdentityRef *, Objective &, int,
                          PlayerScoreSetFunction);
    int modifyPlayerScore(ScoreboardOperationResult &, const ScoreboardId &, Objective &, int, PlayerScoreSetFunction);
    bool resetPlayerScore(const ScoreboardId &, Objective &);
    void resetPlayerScore(const ScoreboardId &);
    std::vector<PlayerScore> getDisplayInfoSorted(
        const std::string &, const std::function<void(ObjectiveSortOrder, std::vector<PlayerScore> &)>) const;
    std::vector<PlayerScore> getDisplayInfoFiltered(const std::string &) const;
    std::optional<ScoreInfo> tryGetIdScore(const ScoreboardId &, const HashType64 &) const;
    std::vector<ScoreInfo> getIdScores(const ScoreboardId &) const;
    std::vector<ScoreboardIdentityRef> getScoreboardIdentityRefs() const;
    void forEachIdentityRef(const std::function<void(ScoreboardIdentityRef &)>);
    const ScoreboardId &getScoreboardId(const Actor &) const;
    const ScoreboardId &getScoreboardId(const Player &) const;
    const ScoreboardId &getScoreboardId(const PlayerScoreboardId &) const;
    const ScoreboardId &getScoreboardId(const ActorUniqueID &) const;
    const ScoreboardId &getScoreboardId(const std::string &) const;
    bool hasIdentityFor(const ScoreboardId &) const;
    size_t getNumTrackedIdentities() const;
    ScoreboardIdentityRef *getScoreboardIdentityRef(const ScoreboardId &);
    const ScoreboardIdentityRef *getScoreboardIdentityRef(const ScoreboardId &) const;
    const ScoreboardIdentityRef &registerScoreboardIdentity(const ScoreboardId &, const PlayerScoreboardId &);
    const ScoreboardIdentityRef &registerScoreboardIdentity(const ScoreboardId &, const ActorUniqueID &);
    const ScoreboardIdentityRef &registerScoreboardIdentity(const ScoreboardId &, const std::string &);
    const ScoreboardIdentityRef &registerScoreboardIdentity(const CompoundTag &);
    bool clearScoreboardIdentity(const ScoreboardId &);
    bool shouldUpdateUI() const;
    void onUpdateUI();
    void replaceFakePlayer(const ScoreboardId &, const PlayerScoreboardId &);
    void addScoreListener(Player &, const std::string &);
    void removeScoreListener(const Player &, const std::string &);
    ScoreboardEventCoordinator &getScoreboardEventCoordinator();

protected:
    CommandSoftEnumRegistry registry_;
    std::unordered_map<std::string, DisplayObjective> display_objectives_;
    IdentityDictionary identity_dictionary_;
    std::unordered_map<ScoreboardId, ScoreboardIdentityRef> identity_refs_;

private:
    bool should_update_ui_;
    std::unordered_map<std::string, std::unique_ptr<Objective>> objectives_;
    std::unordered_map<std::size_t, Bedrock::NonOwnerPointer<Objective>> objectives_lookup_;
    std::unordered_map<std::string, std::unique_ptr<ObjectiveCriteria>> criteria_;
    ScoreboardEventCoordinator scoreboard_event_coordinator_;
    PlayerScoreboardEventListener player_listener_;
};
BEDROCK_STATIC_ASSERT_SIZE(Scoreboard, 768, 536);
