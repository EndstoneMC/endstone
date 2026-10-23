#include <gtest/gtest.h>

#include "endstone/core/perf/mob_ai.h"
#include "endstone/core/perf/mob_ai_capabilities.h"

using endstone::core::MobAiDecision;
using endstone::core::MobAiPolicies;
using endstone::core::mobAiWithinRange;
using endstone::core::supportsMobAiIdle;
using endstone::core::supportsMobAiSearch;

TEST(MobAiPoliciesTest, MasterSwitchAndDefaults)
{
    for (const auto config : {"", R"([mob-ai]
enabled = false
[mob-ai.villagers]
sleep-failure-skip-night = true
[[mob-ai.overrides]]
types = ["minecraft:villager_v2"]
[mob-ai.overrides.idle]
enabled = true
)"}) {
        MobAiPolicies policies;
        EXPECT_FALSE(policies.isEnabled());
        ASSERT_TRUE(policies.initialize(toml::parse(config)));
        EXPECT_FALSE(policies.isEnabled());
        EXPECT_EQ(policies.resolve("minecraft:villager_v2"), MobAiDecision{});
        EXPECT_EQ(policies.resolve("minecraft:cow"), MobAiDecision{});
    }
    MobAiPolicies policies;
    EXPECT_FALSE(policies.isEnabled());
    ASSERT_TRUE(policies.initialize(toml::parse("[mob-ai]\nenabled=true\n")));
    EXPECT_TRUE(policies.isEnabled());
    const auto decision = policies.resolve("minecraft:cow");
    ASSERT_TRUE(decision.animal);
    EXPECT_EQ(decision.animal->activation_range_blocks, 32);
    EXPECT_TRUE(decision.animal->random_stroll);
    EXPECT_TRUE(decision.animal->random_look_around);
    EXPECT_EQ(decision.target_search_interval_ticks, 20);
    EXPECT_EQ(decision.target_search_player_range_blocks, 32);
    EXPECT_FALSE(policies.resolve("minecraft:villager_v2").sleep_failure_skip_night);
}

TEST(MobAiPoliciesTest, OverridesInheritAndDisableSelectedFeatures)
{
    MobAiPolicies policies;
    ASSERT_TRUE(policies.initialize(toml::parse(R"([mob-ai]
enabled = true
[mob-ai.idle]
player-range-blocks = 24
[mob-ai.target-search]
player-range-blocks = 48
[mob-ai.villagers]
sleep-failure-skip-night = true
[[mob-ai.overrides]]
types = ["minecraft:villager_v2"]
[mob-ai.overrides.idle]
enabled = false
[mob-ai.overrides.target-search]
interval-ticks = 40
[mob-ai.overrides.villagers]
sleep-failure-skip-night = false
)")));
    const auto villager = policies.resolve("minecraft:villager_v2");
    EXPECT_FALSE(villager.animal);
    EXPECT_FALSE(villager.sleep_failure_skip_night);
    EXPECT_EQ(villager.target_search_interval_ticks, 40);
    EXPECT_EQ(villager.target_search_player_range_blocks, 48);
    const auto cow = policies.resolve("minecraft:cow");
    ASSERT_TRUE(cow.animal);
    EXPECT_EQ(cow.animal->activation_range_blocks, 24);
    EXPECT_EQ(cow.target_search_interval_ticks, 20);
    EXPECT_EQ(cow.target_search_player_range_blocks, 48);
}

TEST(MobAiPoliciesTest, SleepPolicyOnlyAppliesToModernVillagers)
{
    MobAiPolicies policies;
    ASSERT_TRUE(policies.initialize(
        toml::parse("[mob-ai]\nenabled=true\n[mob-ai.villagers]\nsleep-failure-skip-night=true\n")));
    EXPECT_TRUE(policies.resolve("minecraft:villager_v2").sleep_failure_skip_night);
    EXPECT_FALSE(policies.resolve("minecraft:villager").sleep_failure_skip_night);
    EXPECT_FALSE(policies.resolve("custom:villager_v2").sleep_failure_skip_night);
}

TEST(MobAiPoliciesTest, InvalidSettingsLeavePoliciesInactive)
{
    for (const auto config : {"[mob-ai]\nunknown=true\n", "[mob-ai.villagers]\nsleep-failure-skip-night=1\n",
                              "[mob-ai.target-search]\ninterval-ticks=0\n"}) {
        SCOPED_TRACE(config);
        MobAiPolicies policies;
        EXPECT_FALSE(policies.isEnabled());
        ASSERT_FALSE(policies.initialize(toml::parse(config)));
        EXPECT_FALSE(policies.isEnabled());
        EXPECT_EQ(policies.resolve("minecraft:cow"), MobAiDecision{});
        EXPECT_EQ(policies.resolve("minecraft:villager_v2"), MobAiDecision{});
    }
}

TEST(MobAiCapabilitiesTest, IdleAndSearchRequireSupportedMobs)
{
    MobAiPolicies policies;
    ASSERT_TRUE(policies.initialize(toml::parse("[mob-ai]\nenabled=true\n")));
    const auto decision = policies.resolve("minecraft:zombie");
    EXPECT_TRUE(supportsMobAiIdle("minecraft:cow", ActorType::Cow));
    EXPECT_FALSE(supportsMobAiIdle("minecraft:villager_v2", ActorType::Villager));
    EXPECT_TRUE(supportsMobAiSearch("minecraft:zombie", ActorType::Zombie, decision));
    EXPECT_FALSE(supportsMobAiSearch("minecraft:spider", ActorType::Spider, decision));
}

TEST(MobAiCapabilitiesTest, RangeIsInclusiveAndThreeDimensional)
{
    EXPECT_EQ(mobAiWithinRange({0, 0, 0}, {3, 4, 0}, 5), true);
    EXPECT_EQ(mobAiWithinRange({0, 0, 0}, {3, 4, 0}, 4.99), false);
    EXPECT_EQ(mobAiWithinRange({0, 0, 0}, {0, 6, 0}, 5), false);
}
