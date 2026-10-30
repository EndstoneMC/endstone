#include "bedrock/world/actor/ai/goal/mob_ai_goals.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

Actor *SleepGoal::tryGetActor() const
{
#ifdef _WIN32
    constexpr std::size_t offset = 0xC0;
#else
    constexpr std::size_t offset = 0xB8;
#endif
    Actor *actor;
    std::memcpy(&actor, reinterpret_cast<const char *>(this) + offset, sizeof(actor));
    return actor;
}

bool SleepGoal::hasFailedNavigation() const
{
#ifdef _WIN32
    constexpr std::size_t attempts_offset = 0x30;
    constexpr std::size_t limit_offset = 0x38;
    constexpr std::size_t reached_offset = 0x44;
    constexpr std::size_t failure_offset = 0x50;
#else
    constexpr std::size_t attempts_offset = 0x28;
    constexpr std::size_t limit_offset = 0x30;
    constexpr std::size_t reached_offset = 0x3C;
    constexpr std::size_t failure_offset = 0x48;
#endif
    const auto *goal = reinterpret_cast<const char *>(this);
    std::int32_t attempts;
    std::int32_t limit;
    std::uint8_t reached;
    std::uint64_t failure;
    std::memcpy(&attempts, goal + attempts_offset, sizeof(attempts));
    std::memcpy(&limit, goal + limit_offset, sizeof(limit));
    std::memcpy(&reached, goal + reached_offset, sizeof(reached));
    std::memcpy(&failure, goal + failure_offset, sizeof(failure));
    return !reached && attempts > 0 && (failure != 0 || attempts > limit);
}

Actor *NearestAttackableTargetGoal::tryGetActor() const
{
#ifdef _WIN32
    constexpr std::size_t offset = 0x30;
#else
    constexpr std::size_t offset = 0x28;
#endif
    Actor *actor;
    std::memcpy(&actor, reinterpret_cast<const char *>(this) + offset, sizeof(actor));
    return actor;
}

Actor *RandomStrollGoal::tryGetActor() const
{
#ifdef _WIN32
    constexpr std::size_t offset = 0x30;
#else
    constexpr std::size_t offset = 0x28;
#endif
    Actor *actor;
    std::memcpy(&actor, reinterpret_cast<const char *>(this) + offset, sizeof(actor));
    return actor;
}

Actor *RandomLookAroundGoal::tryGetActor() const
{
#ifdef _WIN32
    constexpr std::size_t offset = 0x48;
#else
    constexpr std::size_t offset = 0x40;
#endif
    Actor *actor;
    std::memcpy(&actor, reinterpret_cast<const char *>(this) + offset, sizeof(actor));
    return actor;
}
