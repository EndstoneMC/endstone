#include "endstone/core/perf/mob_ai.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <set>
#include <stdexcept>

#include <toml++/toml.h>

namespace endstone::core {
namespace {

void require(bool condition, const std::string &message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void keys(const toml::table &table, std::initializer_list<std::string_view> allowed)
{
    for (const auto &[key, value] : table) {
        require(std::find(allowed.begin(), allowed.end(), key.str()) != allowed.end(),
                "Unknown policy field '" + std::string(key.str()) + "'.");
    }
}

std::set<std::string, std::less<>> strings(const toml::node *node, std::string_view field)
{
    require(node && node->is_array(), "Expected array for '" + std::string(field) + "'.");
    std::set<std::string, std::less<>> result;
    for (const auto &entry : *node->as_array()) {
        const auto value = entry.value<std::string>();
        require(value && !value->empty(), "Expected nonempty strings in '" + std::string(field) + "'.");
        require(result.insert(*value).second, "Duplicate string in '" + std::string(field) + "'.");
    }
    require(!result.empty(), "Empty array for '" + std::string(field) + "'.");
    return result;
}

bool boolean(const toml::table &table, std::string_view field, bool fallback)
{
    const auto *node = table.get(field);
    if (!node) {
        return fallback;
    }
    require(node->is_boolean(), std::string(field) + " must be a boolean.");
    return *node->value<bool>();
}

double playerRange(const toml::node &node)
{
    require(node.is_integer() || node.is_floating_point(), "player-range-blocks must be numeric.");
    const auto range = node.is_integer() ? static_cast<double>(node.as_integer()->get())
                                         : node.as_floating_point()->get();
    require(std::isfinite(range) && range > 0, "player-range-blocks must be finite and positive.");
    return range;
}

bool exactEntityType(std::string_view type)
{
    const auto separator = type.find(':');
    if (separator == std::string_view::npos || separator == 0 || separator + 1 == type.size()) {
        return false;
    }
    const auto identifier_character = [](char character) {
        return (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') ||
               character == '_' || character == '-' || character == '.';
    };
    const auto namespace_name = type.substr(0, separator);
    const auto path = type.substr(separator + 1);
    return std::all_of(namespace_name.begin(), namespace_name.end(), identifier_character) &&
           std::all_of(path.begin(), path.end(), [&](char character) {
               return identifier_character(character) || character == '/';
           });
}

struct MobAiSettings {
    bool idle_enabled = true;
    MobAiAnimalPolicy idle{32, true, true};
    bool target_search_enabled = true;
    double target_search_player_range_blocks = 32;
    std::uint32_t target_search_interval_ticks = 20;
    bool sleep_failure_skip_night = false;
};

MobAiSettings mobAiSettings(const toml::table &table, MobAiSettings settings)
{
    if (const auto *node = table.get("idle")) {
        require(node->is_table(), "Expected idle table.");
        const auto &idle = *node->as_table();
        keys(idle, {"enabled", "player-range-blocks", "disable-random-stroll", "disable-random-look-around"});
        settings.idle_enabled = boolean(idle, "enabled", settings.idle_enabled);
        if (const auto *range = idle.get("player-range-blocks")) {
            settings.idle.activation_range_blocks = playerRange(*range);
        }
        settings.idle.random_stroll = boolean(idle, "disable-random-stroll", settings.idle.random_stroll);
        settings.idle.random_look_around =
            boolean(idle, "disable-random-look-around", settings.idle.random_look_around);
    }
    if (const auto *node = table.get("target-search")) {
        require(node->is_table(), "Expected target-search table.");
        const auto &search = *node->as_table();
        keys(search, {"enabled", "player-range-blocks", "interval-ticks"});
        settings.target_search_enabled = boolean(search, "enabled", settings.target_search_enabled);
        if (const auto *range = search.get("player-range-blocks")) {
            settings.target_search_player_range_blocks = playerRange(*range);
        }
        if (const auto *interval = search.get("interval-ticks")) {
            require(interval->is_integer(), "interval-ticks must be an integer.");
            const auto ticks = *interval->value<std::int64_t>();
            require(ticks >= 1 && ticks <= std::numeric_limits<std::uint32_t>::max(),
                    "interval-ticks must be between 1 and 4294967295.");
            settings.target_search_interval_ticks = static_cast<std::uint32_t>(ticks);
        }
    }
    if (const auto *node = table.get("villagers")) {
        require(node->is_table(), "Expected villagers table.");
        const auto &villagers = *node->as_table();
        keys(villagers, {"sleep-failure-skip-night"});
        settings.sleep_failure_skip_night =
            boolean(villagers, "sleep-failure-skip-night", settings.sleep_failure_skip_night);
    }
    return settings;
}

MobAiDecision mobAiDecision(const MobAiSettings &settings)
{
    MobAiDecision result;
    result.sleep_failure_skip_night = settings.sleep_failure_skip_night;
    if (settings.idle_enabled) {
        result.animal = settings.idle;
    }
    if (settings.target_search_enabled) {
        result.target_search_player_range_blocks = settings.target_search_player_range_blocks;
        result.target_search_interval_ticks = settings.target_search_interval_ticks;
    }
    return result;
}

}  // namespace

MobAiPolicies::Snapshot MobAiPolicies::parse(const toml::table &root)
{
    Snapshot result;
    const auto *section = root.get("mob-ai");
    require(!section || section->is_table(), "Expected mob-ai table.");
    if (!section) {
        return result;
    }
    const auto &config = *section->as_table();
    keys(config, {"enabled", "idle", "target-search", "villagers", "overrides"});
    result.enabled = boolean(config, "enabled", false);
    const auto settings = mobAiSettings(config, MobAiSettings{});
    result.default_decision = mobAiDecision(settings);
    if (const auto *overrides = config.get("overrides")) {
        require(overrides->is_array(), "Expected mob-ai overrides array.");
        std::array<std::set<std::string, std::less<>>, 3> assigned;
        constexpr std::array features{"idle", "target-search", "villagers"};
        for (const auto &entry : *overrides->as_array()) {
            require(entry.is_table(), "Expected table for mob-ai override.");
            const auto &table = *entry.as_table();
            keys(table, {"types", "idle", "target-search", "villagers"});
            const auto types = strings(table.get("types"), "types");
            const auto decision = mobAiDecision(mobAiSettings(table, settings));
            for (const auto &type : types) {
                require(exactEntityType(type),
                        "Expected exact namespace:path entity type, got '" + type + "'.");
                auto &resolved = result.overrides.try_emplace(type, result.default_decision).first->second;
                for (std::size_t index = 0; index < features.size(); ++index) {
                    if (!table.contains(features[index])) {
                        continue;
                    }
                    require(assigned[index].insert(type).second,
                            "Multiple mob-ai " + std::string(features[index]) + " overrides for '" + type + "'.");
                    if (index == 0) {
                        resolved.animal = decision.animal;
                    }
                    else if (index == 1) {
                        resolved.target_search_interval_ticks = decision.target_search_interval_ticks;
                        resolved.target_search_player_range_blocks = decision.target_search_player_range_blocks;
                    }
                    else {
                        resolved.sleep_failure_skip_night = decision.sleep_failure_skip_night;
                    }
                }
            }
        }
    }
    return result;
}

Result<void> MobAiPolicies::initialize(const toml::table &config)
{
    ENDSTONE_CHECK(!initialized_, std::string("Mob AI policies are already initialized."));
    initialized_ = true;
    try {
        snapshot_ = parse(config);
        return {};
    }
    catch (const std::exception &error) {
        return nonstd::make_unexpected(std::string(error.what()));
    }
}

MobAiDecision MobAiPolicies::resolve(std::string_view type) const
{
    MobAiDecision result;
    if (!snapshot_.enabled) {
        return result;
    }
    const auto found = snapshot_.overrides.find(type);
    result = found == snapshot_.overrides.end() ? snapshot_.default_decision : found->second;
    result.sleep_failure_skip_night = result.sleep_failure_skip_night && type == "minecraft:villager_v2";
    return result;
}

}  // namespace endstone::core
