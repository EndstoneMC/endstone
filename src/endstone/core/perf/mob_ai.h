#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include <toml++/toml.h>

#include "endstone/util/result.h"

namespace endstone::core {

struct MobAiAnimalPolicy {
    double activation_range_blocks;
    bool random_stroll = false;
    bool random_look_around = false;
    bool operator==(const MobAiAnimalPolicy &) const = default;
};

struct MobAiDecision {
    bool sleep_failure_skip_night = false;
    std::optional<MobAiAnimalPolicy> animal;
    std::optional<std::uint32_t> target_search_interval_ticks;
    std::optional<double> target_search_player_range_blocks;
    bool operator==(const MobAiDecision &) const = default;
};

// Main-thread only; returned decisions do not refer to the active snapshot.
class MobAiPolicies {
public:
    Result<void> initialize(const toml::table &config);
    [[nodiscard]] bool isEnabled() const { return snapshot_.enabled; }
    [[nodiscard]] MobAiDecision resolve(std::string_view type) const;

private:
    struct Snapshot {
        bool enabled = false;
        MobAiDecision default_decision;
        std::map<std::string, MobAiDecision, std::less<>> overrides;
    };

    static Snapshot parse(const toml::table &config);

    Snapshot snapshot_;
    bool initialized_ = false;
};

}  // namespace endstone::core
