#include "bedrock/world/actor/ai/goal/mob_ai_goals.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <optional>
#include <utility>

#include <entt/locator/locator.hpp>

#include "bedrock/entity/components/actor_data_flag_component.h"
#include "bedrock/entity/components/actor_type_flag_component.h"
#include "bedrock/entity/components/actor_unique_id_component.h"
#include "bedrock/entity/components/dimension_type_component.h"
#include "bedrock/entity/components/offsets_component.h"
#include "bedrock/entity/components/state_vector_component.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/actor/provider/actor_riding.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/pathfinder/path_finder.h"
#include "endstone/core/perf/mob_ai_capabilities.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace {
using endstone::core::EndstoneServer;
using endstone::core::MobAiState;
using endstone::core::mobAiWithinRange;
using endstone::core::supportsMobAiIdle;
using endstone::core::supportsMobAiSearch;

bool mobAiEnabled()
{
    return entt::locator<EndstoneServer>::has_value() && EndstoneServer::getInstance().getMobAiPolicies().isEnabled();
}

std::optional<std::int64_t> sleepPolicyId(Actor *actor)
{
    if (!actor || actor->getActorIdentifier().getCanonicalName() != "minecraft:villager_v2") {
        return std::nullopt;
    }
    const auto *id = actor->tryGetComponent<ActorUniqueIDComponent>();
    if (!id || id->unique_id.raw_id == -1) {
        return std::nullopt;
    }
    auto &server = EndstoneServer::getInstance();
    if (!server.getMobAiPolicies().resolve("minecraft:villager_v2").sleep_failure_skip_night) {
        return std::nullopt;
    }
    return id->unique_id.raw_id;
}

struct SleepAttempt;
thread_local SleepAttempt *sleep_attempt = nullptr;

struct SleepAttempt {
    SleepAttempt(Actor *actor, std::int64_t id, MobAiState &state)
        : actor(actor), id(id), state(state), previous(std::exchange(sleep_attempt, this))
    {
    }

    ~SleepAttempt() { sleep_attempt = previous; }
    SleepAttempt(const SleepAttempt &) = delete;
    SleepAttempt &operator=(const SleepAttempt &) = delete;

    Actor *actor;
    std::int64_t id;
    MobAiState &state;
    SleepAttempt *previous;
};

void observeSleepPath(Actor *actor, const Path *path)
{
    const auto *attempt = sleep_attempt;
    if (!attempt || attempt->actor != actor) {
        return;
    }
    if (!path || path->getSize() == 0) {
        attempt->state.recordSleepFailure(attempt->id, actor->getLevel().getTime());
    }
}

void clearSearch(const Actor *actor)
{
    if (actor) {
        if (const auto *id = actor->tryGetComponent<ActorUniqueIDComponent>(); id && id->unique_id.raw_id != -1) {
            EndstoneServer::getInstance().getMobAiState().clearSearch(id->unique_id.raw_id);
        }
    }
}

bool usableMob(const Actor *actor)
{
    if (!actor || actor->isRemoved() || actor->isClientSide() || actor->isPlayer()) {
        return false;
    }
    const auto *id = actor->tryGetComponent<ActorUniqueIDComponent>();
    const auto *type = actor->tryGetComponent<ActorTypeComponent>();
    return id && id->unique_id.raw_id != -1 && type && (type->type & ActorType::Mob) == ActorType::Mob &&
           type->type != ActorType::Player && actor->tryGetComponent<ActorDataFlagComponent>();
}

std::optional<std::array<double, 3>> feetPosition(const Actor &actor)
{
    const auto *position = actor.tryGetComponent<StateVectorComponent>();
    const auto *offset = actor.tryGetComponent<OffsetsComponent>();
    if (!position || !offset || !std::isfinite(position->pos.x) || !std::isfinite(position->pos.y) ||
        !std::isfinite(position->pos.z) || !std::isfinite(offset->height_offset)) {
        return std::nullopt;
    }
    const std::array<double, 3> feet{position->pos.x, static_cast<double>(position->pos.y) - offset->height_offset,
                                     position->pos.z};
    return feet;
}

bool activeMob(const Actor &actor, std::int64_t id, double range, MobAiState &state)
{
    if (actor.getTargetId().raw_id != -1 || actor.isRiding() ||
        !ActorRiding::getPassengers(actor.getEntity()).empty() ||
        state.recentlyActive(id, actor.getLevel().getCurrentServerTick().tick_id)) {
        return true;
    }
    constexpr std::array protected_flags{
        ActorFlags::LEASHED,        ActorFlags::INLOVE,    ActorFlags::TEMPTED,
        ActorFlags::TAMED,          ActorFlags::ANGRY,     ActorFlags::SCARED,
        ActorFlags::ONFIRE,         ActorFlags::USINGITEM, ActorFlags::TRADE_INTEREST,
        ActorFlags::IS_IN_UI,       ActorFlags::ADMIRING,  ActorFlags::CASTING,
        ActorFlags::CHARGING,       ActorFlags::IGNITED,   ActorFlags::FACING_TARGET_TO_RANGE_ATTACK,
        ActorFlags::DELAYED_ATTACK, ActorFlags::RAM_ATTACK};
    const auto *flags = actor.tryGetComponent<ActorDataFlagComponent>();
    for (const auto flag : protected_flags) {
        if (flags->getStatusFlag(flag)) {
            return true;
        }
    }
    const auto position = feetPosition(actor);
    const auto *dimension = actor.tryGetComponent<DimensionTypeComponent>();
    if (!position || !dimension || dimension->type == VanillaDimensions::Undefined) {
        return true;
    }
    bool active = false;
    auto visitor = [&](const Player &player) {
        const auto game_type = player.getPlayerGameType();
        if (game_type == GameType::Spectator) {
            return true;
        }
        if (player.isClientSide()) {
            active = true;
            return false;
        }
        switch (game_type) {
        case GameType::Survival:
        case GameType::Creative:
        case GameType::Adventure:
            break;
        default:
            active = true;
            return false;
        }
        const auto player_position = feetPosition(player);
        const auto nearby = player_position ? mobAiWithinRange(*position, *player_position, range) : std::nullopt;
        active = !nearby || *nearby;
        return !active;
    };
    actor.getDimension().forEachPlayer(visitor);
    return active;
}

bool suppressIdle(Actor *actor, bool stroll)
{
    if (!usableMob(actor)) {
        clearSearch(actor);
        return false;
    }
    const auto &type = actor->getActorIdentifier().getCanonicalName();
    if (!supportsMobAiIdle(type, actor->tryGetComponent<ActorTypeComponent>()->type)) {
        clearSearch(actor);
        return false;
    }
    const auto id = actor->tryGetComponent<ActorUniqueIDComponent>()->unique_id.raw_id;
    auto &server = EndstoneServer::getInstance();
    const auto decision = server.getMobAiPolicies().resolve(type);
    const auto &policy = decision.animal;
    if (!policy || !(stroll ? policy->random_stroll : policy->random_look_around)) {
        return false;
    }
    auto &state = server.getMobAiState();
    if (activeMob(*actor, id, policy->activation_range_blocks, state)) {
        if (supportsMobAiSearch(type, actor->tryGetComponent<ActorTypeComponent>()->type, decision) &&
            (*decision.target_search_player_range_blocks == policy->activation_range_blocks ||
             activeMob(*actor, id, *decision.target_search_player_range_blocks, state))) {
            state.clearSearch(id);
        }
        return false;
    }
    return true;
}
}  // namespace

bool SleepGoal::canUse()
{
    if (mobAiEnabled()) {
        auto *actor = tryGetActor();
        const auto id = sleepPolicyId(actor);
        if (id) {
            const auto world_time = actor->getLevel().getTime();
            auto &state = EndstoneServer::getInstance().getMobAiState();
            if (!state.canAttemptSleep(*id, world_time)) {
                return false;
            }
            const SleepAttempt attempt{actor, *id, state};
            return ENDSTONE_HOOK_CALL_ORIGINAL(&SleepGoal::canUse, this);
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&SleepGoal::canUse, this);
}

void SleepGoal::stop()
{
    if (mobAiEnabled()) {
        auto *actor = tryGetActor();
        const auto id = sleepPolicyId(actor);
        if (id && !actor->isSleeping() && hasFailedNavigation()) {
            EndstoneServer::getInstance().getMobAiState().recordSleepFailure(*id, actor->getLevel().getTime());
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&SleepGoal::stop, this);
}

#ifdef _WIN32
std::unique_ptr<Path> Level::findPath(Actor &actor, int x, int y, int z, NavigationComponent &navigation)
{
    using Function = std::unique_ptr<Path> (Level::*)(Actor &, int, int, int, NavigationComponent &);
    auto path = ENDSTONE_HOOK_CALL_ORIGINAL(static_cast<Function>(&Level::findPath), this, actor, x, y, z, navigation);
    observeSleepPath(&actor, path.get());
    return path;
}
#else
std::unique_ptr<Path> PathFinder::findPath(Actor &actor, int x, int y, int z, float maximum_distance)
{
    auto path = ENDSTONE_HOOK_CALL_ORIGINAL(&PathFinder::findPath, this, actor, x, y, z, maximum_distance);
    observeSleepPath(&actor, path.get());
    return path;
}
#endif

bool RandomStrollGoal::canUse()
{
    if (mobAiEnabled() && suppressIdle(tryGetActor(), true)) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RandomStrollGoal::canUse, this);
}

bool RandomStrollGoal::canContinue()
{
    if (mobAiEnabled() && suppressIdle(tryGetActor(), true)) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RandomStrollGoal::canContinue, this);
}

bool RandomLookAroundGoal::canUse()
{
    if (mobAiEnabled() && suppressIdle(tryGetActor(), false)) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RandomLookAroundGoal::canUse, this);
}

bool RandomLookAroundGoal::canContinue()
{
    if (mobAiEnabled() && suppressIdle(tryGetActor(), false)) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&RandomLookAroundGoal::canContinue, this);
}

ActorUniqueID NearestAttackableTargetGoal::_findTarget(const MobDescriptor **matched_definition)
{
    if (mobAiEnabled()) {
        const auto *actor = tryGetActor();
        if (!usableMob(actor)) {
            clearSearch(actor);
        }
        else {
            const auto id = actor->tryGetComponent<ActorUniqueIDComponent>()->unique_id.raw_id;
            const auto &type = actor->getActorIdentifier().getCanonicalName();
            auto &server = EndstoneServer::getInstance();
            auto &state = server.getMobAiState();
            const auto decision = server.getMobAiPolicies().resolve(type);
            if (!supportsMobAiSearch(type, actor->tryGetComponent<ActorTypeComponent>()->type, decision) ||
                activeMob(*actor, id, *decision.target_search_player_range_blocks, state)) {
                state.clearSearch(id);
            }
            else if (!state.allowNewTargetSearch(id, actor->getLevel().getCurrentServerTick().tick_id,
                                                 decision.target_search_interval_ticks)) {
                *matched_definition = nullptr;
                return ActorUniqueID::INVALID_ID;
            }
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&NearestAttackableTargetGoal::_findTarget, this, matched_definition);
}
