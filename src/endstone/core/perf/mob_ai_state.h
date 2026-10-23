#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace endstone::core {

// Main-thread state.
class MobAiState {
public:
    void recordActivity(std::int64_t id, std::uint64_t tick);
    [[nodiscard]] bool recentlyActive(std::int64_t id, std::uint64_t tick);
    bool allowNewTargetSearch(std::int64_t id, std::uint64_t tick, std::optional<std::uint32_t> interval);
    void clearSearch(std::int64_t id);
    [[nodiscard]] bool canAttemptSleep(std::int64_t id, int world_time);
    void recordSleepFailure(std::int64_t id, int world_time);
    void erase(std::int64_t id);
    void prune(std::uint64_t tick);
    void clear();

private:
    struct Entry {
        std::uint64_t protected_until = 0;
        std::uint64_t search_after = 0;
    };
    void observeSleepPeriod(int world_time);

    std::unordered_map<std::int64_t, Entry> entries_;
    std::uint64_t prune_after_ = 0;
    std::optional<std::int64_t> sleep_period_;
    std::unordered_set<std::int64_t> sleep_failures_;
};

}  // namespace endstone::core
