#include <limits>

#include <gtest/gtest.h>

#include "endstone/core/perf/mob_ai_state.h"

using endstone::core::MobAiState;

TEST(MobAiStateTest, SearchCadenceUsesActualSearchTicks)
{
    MobAiState state;
    EXPECT_TRUE(state.allowNewTargetSearch(12, 7, 20));
    EXPECT_FALSE(state.allowNewTargetSearch(12, 26, 20));
    EXPECT_TRUE(state.allowNewTargetSearch(12, 30, 20));
    EXPECT_FALSE(state.allowNewTargetSearch(12, 49, 20));
    EXPECT_TRUE(state.allowNewTargetSearch(12, 50, 20));
}

TEST(MobAiStateTest, ActivityTemporarilyBypassesSearchDelay)
{
    MobAiState state;
    EXPECT_TRUE(state.allowNewTargetSearch(12, 1, 200));
    state.recordActivity(12, 10);
    EXPECT_TRUE(state.recentlyActive(12, 109));
    EXPECT_TRUE(state.allowNewTargetSearch(12, 109, 20));
    EXPECT_FALSE(state.recentlyActive(12, 110));
    EXPECT_TRUE(state.allowNewTargetSearch(12, 110, 20));
    EXPECT_FALSE(state.allowNewTargetSearch(12, 129, 20));
    EXPECT_TRUE(state.allowNewTargetSearch(12, 130, 20));
}

TEST(MobAiStateTest, SleepFailureLastsUntilNextEvening)
{
    MobAiState state;
    state.recordSleepFailure(12, 13000);
    for (const auto world_time : {13000, 23000, 25000, 35999}) {
        EXPECT_FALSE(state.canAttemptSleep(12, world_time));
    }
    EXPECT_TRUE(state.canAttemptSleep(13, 35999));
    EXPECT_TRUE(state.canAttemptSleep(12, 36000));
}

TEST(MobAiStateTest, EraseAndPrunePreserveSleepFailure)
{
    MobAiState state;
    state.recordActivity(12, 1);
    EXPECT_TRUE(state.allowNewTargetSearch(13, 1, 200));
    EXPECT_FALSE(state.allowNewTargetSearch(13, 2, 200));
    state.recordSleepFailure(12, 13000);
    state.erase(12);
    state.erase(13);
    EXPECT_FALSE(state.recentlyActive(12, 2));
    EXPECT_TRUE(state.allowNewTargetSearch(13, 2, 200));
    EXPECT_FALSE(state.canAttemptSleep(12, 14000));
    state.prune(500);
    EXPECT_FALSE(state.canAttemptSleep(12, 25000));
}

TEST(MobAiStateTest, ClearResetsActivitySearchAndSleepFailure)
{
    MobAiState state;
    state.recordActivity(12, 1);
    EXPECT_TRUE(state.allowNewTargetSearch(13, 1, 20));
    EXPECT_FALSE(state.allowNewTargetSearch(13, 2, 20));
    state.recordSleepFailure(12, 13000);
    state.clear();
    EXPECT_FALSE(state.recentlyActive(12, 2));
    EXPECT_TRUE(state.allowNewTargetSearch(13, 2, 20));
    EXPECT_TRUE(state.canAttemptSleep(12, 13000));
}

TEST(MobAiStateTest, SleepPeriodsHandleExtremeWorldTimes)
{
    MobAiState state;
    state.recordSleepFailure(12, std::numeric_limits<int>::min());
    EXPECT_FALSE(state.canAttemptSleep(12, std::numeric_limits<int>::min() + 1));
    EXPECT_TRUE(state.canAttemptSleep(12, std::numeric_limits<int>::max()));
}
