#pragma once

#include "bedrock/world/actor/ai/goal/move_to_block_goal.h"

class SleepGoal : public MoveToBlockGoal {
public:
    ENDSTONE_HOOK bool canUse();
    ENDSTONE_HOOK void stop();

    [[nodiscard]] Mob *tryGetActor() const { return owner_; }
    [[nodiscard]] bool hasFailedNavigation() const
    {
        return !reached_ && attempts_ > 0 && (failure_ != 0 || attempts_ > limit_);
    }

private:
    Mob *owner_;
};
