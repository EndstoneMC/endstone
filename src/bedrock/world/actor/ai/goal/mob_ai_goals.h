#pragma once

#include "bedrock/bedrock.h"
#include "bedrock/world/actor/actor_unique_id.h"

class Actor;
struct MobDescriptor;

class SleepGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK void stop();
    [[nodiscard]] Actor *tryGetActor() const;
    [[nodiscard]] bool hasFailedNavigation() const;
};

class NearestAttackableTargetGoal {
public:
    ENDSTONE_HOOK ActorUniqueID _findTarget(const MobDescriptor **matched_definition);
    [[nodiscard]] Actor *tryGetActor() const;
};

class RandomStrollGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK bool canContinue();
    [[nodiscard]] Actor *tryGetActor() const;
};

class RandomLookAroundGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK bool canContinue();
    [[nodiscard]] Actor *tryGetActor() const;
};
