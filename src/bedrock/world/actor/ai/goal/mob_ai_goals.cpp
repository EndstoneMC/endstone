#include "bedrock/world/actor/ai/goal/mob_ai_goals.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "bedrock/symbol.h"

namespace {
#ifdef _WIN32
constexpr std::size_t sleep_goal_vtable = 0xA994440;
#else
constexpr std::size_t sleep_goal_vtable = 0xEEFC570;
#endif

bool hasVtable(const void *goal, std::size_t vtable)
{
    const void *actual_vtable;
    std::memcpy(&actual_vtable, goal, sizeof(actual_vtable));
    return actual_vtable == static_cast<char *>(endstone::runtime::get_executable_base()) + vtable;
}

Actor *readActor(const void *goal, std::size_t vtable, std::size_t owner_offset)
{
    if (!hasVtable(goal, vtable)) {
        return nullptr;
    }
    Actor *actor;
    std::memcpy(&actor, static_cast<const char *>(goal) + owner_offset, sizeof(actor));
    return actor;
}
}  // namespace

Actor *SleepGoal::tryGetActor() const
{
#ifdef _WIN32
    return readActor(this, sleep_goal_vtable, 0xC0);
#else
    return readActor(this, sleep_goal_vtable, 0xB8);
#endif
}

bool SleepGoal::hasFailedNavigation() const
{
    if (!hasVtable(this, sleep_goal_vtable)) {
        return false;
    }
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
    return readActor(this, 0xA961300, 0x30);
#else
    return readActor(this, 0xF242670, 0x28);
#endif
}

Actor *RandomStrollGoal::tryGetActor() const
{
#ifdef _WIN32
    return readActor(this, 0xA960EC0, 0x30);
#else
    return readActor(this, 0xF242178, 0x28);
#endif
}

Actor *RandomLookAroundGoal::tryGetActor() const
{
#ifdef _WIN32
    return readActor(this, 0xA98F7F0, 0x48);
#else
    return readActor(this, 0xEEE7378, 0x40);
#endif
}
