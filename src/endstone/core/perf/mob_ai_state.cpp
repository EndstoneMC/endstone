#include "endstone/core/perf/mob_ai_state.h"

#include <algorithm>
#include <limits>

namespace endstone::core {
namespace {
std::uint64_t deadline(std::uint64_t tick, std::uint64_t duration)
{
    const auto max = std::numeric_limits<std::uint64_t>::max();
    return duration > max - tick ? max : tick + duration;
}
}  // namespace

void MobAiState::recordActivity(std::int64_t id, std::uint64_t tick)
{
    if (id == -1) {
        return;
    }
    auto &entry = entries_[id];
    entry.protected_until = deadline(tick, 100);
    entry.search_after = 0;
}

bool MobAiState::recentlyActive(std::int64_t id, std::uint64_t tick)
{
    const auto it = entries_.find(id);
    return it != entries_.end() && tick < it->second.protected_until;
}

bool MobAiState::allowNewTargetSearch(std::int64_t id, std::uint64_t tick, std::optional<std::uint32_t> interval)
{
    auto it = entries_.find(id);
    if (id == -1 || !interval || (it != entries_.end() && tick < it->second.protected_until)) {
        clearSearch(id);
        return true;
    }
    if (it != entries_.end() && tick < it->second.search_after) {
        return false;
    }
    entries_[id].search_after = deadline(tick, *interval);
    return true;
}

void MobAiState::clearSearch(std::int64_t id)
{
    if (auto it = entries_.find(id); it != entries_.end()) {
        it->second.search_after = 0;
    }
}

void MobAiState::observeSleepPeriod(int world_time)
{
    const auto shifted = static_cast<std::int64_t>(world_time) - 12000;
    const auto period = shifted / 24000 - (shifted % 24000 < 0 ? 1 : 0);
    if (sleep_period_ != period) {
        sleep_failures_.clear();
        sleep_period_ = period;
    }
}

bool MobAiState::canAttemptSleep(std::int64_t id, int world_time)
{
    observeSleepPeriod(world_time);
    return id == -1 || !sleep_failures_.contains(id);
}

void MobAiState::recordSleepFailure(std::int64_t id, int world_time)
{
    observeSleepPeriod(world_time);
    if (id != -1) {
        sleep_failures_.insert(id);
    }
}

void MobAiState::erase(std::int64_t id)
{
    entries_.erase(id);
}

void MobAiState::prune(std::uint64_t tick)
{
    if (tick < prune_after_) {
        return;
    }
    prune_after_ = deadline(tick, 20);
    std::erase_if(entries_, [tick](const auto &item) {
        return item.second.protected_until <= tick && item.second.search_after <= tick;
    });
}

void MobAiState::clear()
{
    entries_.clear();
    prune_after_ = 0;
    sleep_failures_.clear();
    sleep_period_.reset();
}

}  // namespace endstone::core
