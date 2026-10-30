#pragma once

#include "bedrock/world/actor/ai/goal/goal.h"
#include "bedrock/world/actor/actor_unique_id.h"

struct MobDescriptor;

class NearestAttackableTargetGoal : public Goal {
public:
    ENDSTONE_HOOK ActorUniqueID _findTarget(const MobDescriptor **matched_definition);

    [[nodiscard]] Mob *tryGetActor() const { return owner_; }

private:
    Mob *owner_;
};
