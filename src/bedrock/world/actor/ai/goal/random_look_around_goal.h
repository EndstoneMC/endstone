#pragma once

#include "bedrock/world/actor/ai/goal/goal.h"

class RandomLookAroundGoal : public Goal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK bool canContinue();

    [[nodiscard]] Mob *tryGetActor() const { return owner_; }

private:
    Mob *owner_;
};
